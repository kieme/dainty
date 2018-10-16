/******************************************************************************

 MIT License

 Copyright (c) 2018 kieme, frits.germs@gmx.net

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

******************************************************************************/

#include <map>
#include "dainty_named_terminal.h"
#include "dainty_container_maybe.h"
#include "dainty_container_freelist.h"
#include "dainty_os_threading.h"
#include "dainty_os_clock.h"
#include "dainty_mt_event_dispatcher.h"
#include "dainty_mt_waitable_chained_queue.h"
#include "dainty_mt_command.h"
#include "dainty_mt_thread.h"
#include "dainty_messaging_message.h"
#include "dainty_messaging_messenger.h"
#include "dainty_tracing.h"
#include "dainty_messaging.h"

using namespace dainty;
using namespace dainty::container;
using namespace dainty::named;
using namespace dainty::named::terminal;
using namespace dainty::mt;
using namespace dainty::os;
using namespace dainty::tracing;
using namespace dainty::messaging;

using string::FMT;
using any::t_any;
using maybe::t_maybe;
using freelist::t_freelist;
using threading::t_mutex_lock;
using clock::t_time;
using thread::t_thread;
using event_dispatcher::t_dispatcher;
using event_dispatcher::t_event_logic;
using event_dispatcher::CONTINUE;
using event_dispatcher::QUIT_EVENT_LOOP;
using event_dispatcher::RD;
using messenger::t_multiple_of_100ms;
using message::t_message;
using message::x_message;
using message::R_message;
using message::t_messenger_key_;
using message::t_messenger_state;
using message::t_messenger_prio;
using messaging::err::r_err;

using t_ctxt_id                   = freelist::t_id;
using t_thd_err                   = t_thread::t_logic::t_err;
using t_cmd_err                   = command::t_processor::t_logic::t_err;
using t_cmd_client                = command::t_client;
using t_cmd_processor             = command::t_processor;
using r_cmd_processor             = t_prefix<t_cmd_processor>::r_;
using r_cmd_processor_logic       = t_prefix<t_cmd_processor::t_logic>::r_;
using t_cmd                       = command::t_command;
using t_any_user                  = any::t_user;
using x_any                       = t_prefix<t_any>::x_;
using t_que_chain                 = waitable_chained_queue::t_chain;
using t_que_client                = waitable_chained_queue::t_client;
using t_que_processor             = waitable_chained_queue::t_processor;
using r_que_processor             = t_prefix<t_que_processor>::r_;
using r_que_processor_logic       = t_prefix<t_que_processor::t_logic>::r_;
using t_event_cmd                 = event_dispatcher::t_cmd;
using r_event_cmd                 = t_prefix<t_event_cmd>::r_;
using r_messenger_key             = message::r_messenger_key;
using t_messenger_key             = messenger::t_key;
using R_messenger_key             = messenger::R_key;
using r_messenger_messages        = messenger::r_messages;
using r_messenger_timer_params    = messenger::r_timer_params;
using R_messenger_timer_params    = messenger::R_timer_params;
using t_messenger_password        = messenger::t_password;
using t_maybe_messenger_processor = messenger::t_maybe_processor_;
using r_maybe_messenger_processor = messenger::r_maybe_processor_;
using x_maybe_messenger_processor = messenger::x_maybe_processor_;
using t_messenger_processor       = messenger::t_processor_;
using t_messenger_client          = messenger::t_client_;
using x_messenger_client          = t_prefix<t_messenger_client>::x_;
using r_messenger_monitor_list    = messenger::r_monitor_list;

namespace dainty
{
namespace named
{
  inline t_bool operator<(R_messenger_key lh, R_messenger_key rh) {
    return get(lh) < get(rh);
  }

  inline t_bool operator<(t_messenger_prio lh, t_messenger_prio rh) {
    return get(lh) < get(rh);
  }
}

namespace messaging
{
  constexpr message::t_domain MAIN{0};

////////////////////////////////////////////////////////////////////////////////

  /*
  tracing::t_tracer& tr_() {
    static tracing::t_err err;
    static tracing::t_tracer tr = make_tracer(err, P_cstr{"messaging"});
    if (tr == INVALID)
      tr = make_tracer(err, P_cstr{"messaging"});
    if (err)
      err.clear(); // XXX
    return tr;
  }
  */

////////////////////////////////////////////////////////////////////////////////

  inline t_bool operator==(R_messenger_key lh, R_messenger_key rh) {
    return get(lh) == get(rh);
  }

  inline t_bool operator!=(R_messenger_key lh, R_messenger_key rh) {
    return get(lh) != get(rh);
  }

////////////////////////////////////////////////////////////////////////////////

  // 64              bit offset                 0
  // --------------------------------------------
  // | remote(32) | seq(16) | id(15) | group(1) |
  // --------------------------------------------
  //
  // - 2*15 = 32K messengers and 32K messenger groups.
  // - reusing the same id has a likelihood of 1 out of 65K.
  // - remote 32 bits used to identify remote. will use later.

  struct t_messenger_key_params {
    t_bool    group = false;
    t_bool    local = false;
    t_ctxt_id id    = t_ctxt_id{0};
    t_uint16  seq   = 0;

    inline t_messenger_key_params() = default;
    inline t_messenger_key_params(t_bool _group, t_bool _local, t_ctxt_id _id,
                                  t_uint16 _seq)
      : group(_group), local(_local), id(_id), seq(_seq) {
    }
  };

  inline t_messenger_key make_key(t_uint16 seq, t_ctxt_id id) {
    return t_messenger_key{((t_messenger_key_)get(id) << 1) |
                           ((t_messenger_key_)seq << 16)};
  }

  inline t_messenger_key make_group_key(t_uint16 seq, t_ctxt_id id) {
    return t_messenger_key{get(make_key(seq, id)) | 1};
  }

  inline t_messenger_key make_remote_key(//xxxx,
                                         R_messenger_key key) {
    return t_messenger_key{get(key) /* | use upper 32 bits xxxx */};
  }

  inline t_bool is_remote(R_messenger_key key) {
    return get(key) >> 32;
  }

  inline t_bool is_local(R_messenger_key key) {
    return !is_remote(key);
  }

  inline t_bool is_group(R_messenger_key key) {
    return get(key) & 1;
  }

  inline t_bool is_messenger(R_messenger_key key) {
    return !is_group(key);
  }

  inline t_ctxt_id get_ctxt_id(R_messenger_key key) {
    return t_ctxt_id{0x7fff & (get(key) >> 1)};
  }

  inline t_bool is_valid(R_messenger_key key) {
    return get(key);
  }

  inline t_messenger_key_params get_key_params(R_messenger_key key) {
    return {is_group(key), is_local(key), get_ctxt_id(key),
            (t_uint16)(0xff & (get(key) >> 16))};
  }

  constexpr t_bool operator==(R_messenger_key key, t_validity validity) {
    return get(key) == (validity == VALID);
  }

///////////////////////////////////////////////////////////////////////////////

namespace message
{
///////////////////////////////////////////////////////////////////////////////

  struct t_hdr_ {
    t_n_             len;
    t_messenger_key_ dst;
    t_messenger_key_ src;
    t_uchar          domain;
    t_user_          user;
    t_version_       version;
    t_uint16         cnt;
    t_uint16         seq;
  };
  using t_hdr_ptr_  = bytebuf::t_ptr <t_hdr_>;
  using t_hdr_cptr_ = bytebuf::t_cptr<t_hdr_>;

  t_bool write_hdr_(r_message       msg,
                    R_id            id,
                    t_n             _len,
                    R_messenger_key _dst,
                    R_messenger_key _src,
                    t_uint16        _cnt,
                    t_uint16        _seq) {
    if (get(msg.get_capacity()) >= sizeof(t_hdr_)) {
      t_hdr_ptr_ hdr{msg.mk_view(t_ix{0}, t_ix{sizeof(t_hdr_)})};
      hdr->len     = get(_len);
      hdr->dst     = get(_dst);
      hdr->src     = get(_src);
      hdr->domain  = get(id.domain);
      hdr->user    = get(id.user);
      hdr->version = get(id.version);
      hdr->cnt     = _cnt;
      hdr->seq     = _seq;
      return true;
    }
    return false;
  }

  t_bool write_hdr_(r_message msg, R_id id, t_n _len, t_uint16 _cnt) {
    if (get(msg.get_capacity()) >= sizeof(t_hdr_)) {
      t_hdr_ptr_ hdr{msg.mk_view(t_ix{0}, t_ix{sizeof(t_hdr_)})};
      hdr->len     = get(_len);
      hdr->domain  = get(id.domain);
      hdr->user    = get(id.user);
      hdr->version = get(id.version);
      hdr->cnt     = _cnt;
      return true;
    }
    return false;
  }

  t_bool write_hdr_(r_message       msg,
                    R_id            id,
                    R_messenger_key _dst,
                    R_messenger_key _src,
                    t_uint16        _seq) {
    if (get(msg.get_capacity()) >= sizeof(t_hdr_)) {
      t_hdr_ptr_ hdr{msg.mk_view(t_ix{0}, t_ix{sizeof(t_hdr_)})};
      hdr->dst     = get(_dst);
      hdr->src     = get(_src);
      hdr->domain  = get(id.domain);
      hdr->user    = get(id.user);
      hdr->version = get(id.version);
      hdr->seq     = _seq;
      return true;
    }
    return false;
  }

  t_bool write_hdr_(r_message       msg,
                    R_messenger_key _dst,
                    R_messenger_key _src) {
    if (get(msg.get_capacity()) >= sizeof(t_hdr_)) {
      t_hdr_ptr_ hdr{msg.mk_view(t_ix{0}, t_ix{sizeof(t_hdr_)})};
      hdr->dst = get(_dst);
      hdr->src = get(_src);
      return true;
    }
    return false;
  }

  t_bool read_hdr_(R_message       msg,
                   r_id            id,
                   t_n&            _len,
                   r_messenger_key _dst,
                   r_messenger_key _src,
                   t_uint16&       _cnt,
                   t_uint16&       _seq) {
    if (get(msg.get_capacity()) >= sizeof(t_hdr_)) {
      t_hdr_cptr_ hdr{msg.mk_cview(t_ix{0}, t_ix{sizeof(t_hdr_)})};
      set(_len)       = hdr->len;
      set(_dst)       = hdr->dst;
      set(_src)       = hdr->src;
      set(id.domain)  = hdr->domain;
      set(id.user)    = hdr->user;
      set(id.version) = hdr->version;
      _cnt            = hdr->cnt;
      _seq            = hdr->seq;
      return true;
    }
    return false;
  }

  inline
  t_messenger_key read_dst(R_message msg) {
    if (msg == VALID && get(msg.get_capacity()) >= sizeof(t_hdr_)) {
      t_hdr_cptr_ hdr{msg.mk_cview(t_ix{0}, t_ix{sizeof(t_hdr_)})};
      return t_messenger_key{hdr->dst};
    }
    return t_messenger_key{0};
  }

  inline
  t_messenger_key read_src(R_message msg) {
    if (msg == VALID && get(msg.get_capacity()) >= sizeof(t_hdr_)) {
      t_hdr_cptr_ hdr{msg.mk_cview(t_ix{0}, t_ix{sizeof(t_hdr_)})};
      return t_messenger_key{hdr->src};
    }
    return t_messenger_key{0};
  }

  inline
  t_n read_len(R_message msg) {
    if (msg == VALID && get(msg.get_capacity()) >= sizeof(t_hdr_)) {
      t_hdr_cptr_ hdr{msg.mk_cview(t_ix{0}, t_ix{sizeof(t_hdr_)})};
      return t_n{hdr->len};
    }
    return t_n{0};
  }

  inline
  t_id read_id(R_message msg) {
    if (msg == VALID && get(msg.get_capacity()) >= (sizeof(t_hdr_))) {
      t_hdr_cptr_ hdr{msg.mk_cview(t_ix{0}, t_ix{sizeof(t_hdr_)})};
      return t_id{t_domain {hdr->domain},
                  t_user   {hdr->user},
                  t_version{hdr->version}};
    }
    return t_id{t_domain{0}, t_user{0}, t_version{0}};
  }

///////////////////////////////////////////////////////////////////////////////

  t_message::t_message() {
  }

  t_message::t_message(t_n n) : buf_{t_n{named::get(n) + sizeof(t_hdr_)}} {
  }

  t_message::t_message(R_bytebuf buf) : buf_{buf} {
  }

  t_message::t_message(x_message msg) : buf_{std::move(msg.buf_)} {
  }

  t_message::t_message(R_message msg) : buf_{msg.buf_} {
  }

  r_message t_message::operator=(x_message msg) {
    buf_ = std::move(msg.buf_);
    return *this;
  }

  r_message t_message::operator=(R_message msg) {
    buf_ = msg.buf_;
    return *this;
  }

  t_bool t_message::set(R_id id, t_n len, t_uint16 cnt) {
    return write_hdr_(*this, id, len, cnt);
  }

  t_bool t_message::get(r_id      id,
                        t_n&      len,
                        t_key&    dst,
                        t_key&    src,
                        t_uint16& cnt) {
    t_uint16 seq = 0;
    return read_hdr_(*this, id, len, dst, src, cnt, seq);
  }

  t_message::operator t_validity () const {
    return buf_;
  }

  t_n t_message::get_capacity() const {
    return buf_.get_capacity();
  }

  p_byte t_message::data() {
    return buf_.ptr();
  }

  P_byte t_message::data() const {
    return buf_.ptr();
  }

  P_byte t_message::cdata() const {
    return buf_.cptr();
  }

  t_view t_message::mk_view() {
    return container::bytebuf::mk_view(buf_);
  }

  t_cview t_message::mk_view() const {
    return container::bytebuf::mk_cview(buf_);
  }

  t_cview t_message::mk_cview() const {
    return container::bytebuf::mk_cview(buf_);
  }

  t_view t_message::mk_view(t_ix begin) {
    return container::bytebuf::mk_view(buf_, begin);
  }

  t_cview t_message::mk_view(t_ix begin) const {
    return container::bytebuf::mk_cview(buf_, begin);
  }

  t_cview t_message::mk_cview(t_ix begin) const {
    return container::bytebuf::mk_cview(buf_, begin);
  }

  t_view t_message::mk_view(t_ix begin, t_ix end) {
    return container::bytebuf::mk_view(buf_, begin, end);
  }

  t_cview t_message::mk_view(t_ix begin, t_ix end) const {
    return container::bytebuf::mk_cview(buf_, begin, end);
  }

  t_cview t_message::mk_cview(t_ix begin, t_ix end) const {
    return container::bytebuf::mk_cview(buf_, begin, end);
  }

  t_message t_message::clone() const {
    return buf_;
  }

///////////////////////////////////////////////////////////////////////////////

  struct t_notify_ {
    t_messenger_state state;
    t_messenger_key_  key;
    t_messenger_prio_ prio;
    t_messenger_user  user;
    t_messenger_name  name;
  };
  using t_notify_ptr_  = bytebuf::t_ptr <t_notify_>;
  using t_notify_cptr_ = bytebuf::t_cptr<t_notify_>;

  t_bool write_notify_msg_(r_message         msg,
                           R_messenger_key   dst,
                           t_messenger_state state,
                           R_messenger_name  name,
                           R_messenger_key   key,
                           t_messenger_prio  prio,
                           t_messenger_user  user) {
    const t_n_ max = sizeof(t_hdr_) + sizeof(t_notify_);
    if (msg == VALID && get(msg.get_capacity()) >= max) {

      write_hdr_(msg,
                 t_id{MAIN,
                 t_user{MSG_NOTIFY},
                 t_version{1}},
                 t_n{sizeof(t_notify_)},
                 dst,
                 t_messenger_key{0},
                 1,
                 0);

      t_notify_ptr_ notify{msg.mk_view(t_ix{sizeof(t_hdr_)},
                                       t_ix{sizeof(t_hdr_) +
                                            sizeof(t_notify_)})};
      notify->state = state;
      notify->key   = get(key);
      notify->prio  = get(prio);
      notify->user  = user;
      notify->name  = name;
      return true;
    }
    return false;
  }

  t_bool read_notify_msg_(R_message          msg,
                          t_messenger_state& state,
                          t_messenger_name&  name,
                          t_messenger_key&   key,
                          t_messenger_prio&  prio,
                          t_messenger_user&  user) {
    const t_n_ max = sizeof(t_hdr_) + sizeof(t_notify_);
    if (msg == VALID && get(msg.get_capacity()) >= max) {
      t_notify_cptr_ notify{msg.mk_cview(t_ix{sizeof(t_hdr_)},
                                         t_ix{sizeof(t_hdr_) +
                                              sizeof(t_notify_)})};
      state      = notify->state;
      set(key)   = notify->key;
      set(prio)  = notify->prio;
      user       = notify->user;
      name       = notify->name;
      return true;
    }
    return false;
  }

  t_notify_message::t_notify_message() : t_message{t_n{sizeof(t_notify_)}} {
  }

  t_bool t_notify_message::get(t_messenger_state& state,
                               t_messenger_name&  name,
                               t_messenger_key&   key,
                               t_messenger_prio&  prio,
                               t_messenger_user&  user) {
    return read_notify_msg_(*this, state, name, key, prio, user);
  }

///////////////////////////////////////////////////////////////////////////////

  struct t_timeout_ {
    t_bool            periodic;
    t_messenger_key_  key;
    t_n_              multiple;
    t_messenger_prio_ prio;
    t_messenger_user  user;
  };
  using t_timeout_ptr_  = bytebuf::t_ptr <t_timeout_>;
  using t_timeout_cptr_ = bytebuf::t_cptr<t_timeout_>;


  t_bool write_timeout_msg_(r_message           msg,
                            t_bool              periodic,
                            t_multiple_of_100ms multiple,
                            R_messenger_key     key,
                            t_messenger_prio    prio,
                            t_messenger_user    user) {
    const t_n_ max = sizeof(t_hdr_) + sizeof(t_timeout_);
    if (msg == VALID && get(msg.get_capacity()) >= max) { // XXX

      write_hdr_(msg,
                 t_id{MAIN,
                 t_user{MSG_TIMEOUT},
                 t_version{1}},
                 t_n{sizeof(t_timeout_)},
                 key,
                 t_messenger_key{0},
                 1,
                 0);

      t_timeout_ptr_ timeout{msg.mk_view(t_ix{sizeof(t_hdr_)},
                                         t_ix{sizeof(t_hdr_) +
                                              sizeof(t_timeout_)})};
      timeout->periodic = periodic;
      timeout->key      = get(key);
      timeout->prio     = get(prio);
      timeout->user     = user;
      timeout->multiple = get(multiple.value);
      return true;
    }
    return false;
  }

  t_bool read_timeout_msg_(R_message            msg,
                           t_bool&              periodic,
                           t_multiple_of_100ms& multiple,
                           t_messenger_key&     key,
                           t_messenger_prio&    prio,
                           t_messenger_user&    user) {
    const t_n_ max = sizeof(t_hdr_) + sizeof(t_timeout_);
    if (msg == VALID && get(msg.get_capacity()) >= max) { // XXX
      t_timeout_cptr_ timeout{msg.mk_cview(t_ix{sizeof(t_hdr_)},
                                           t_ix{sizeof(t_hdr_) +
                                                sizeof(t_timeout_)})};
      periodic            = timeout->periodic;
      set(key)            = timeout->key;
      set(prio)           = timeout->prio;
      user                = timeout->user;
      set(multiple.value) = timeout->multiple;
      return true;
    }
    return false;
  }

  t_timeout_message::t_timeout_message() : t_message{t_n{sizeof(t_timeout_)}} {
  }

  t_bool t_timeout_message::get(t_bool&              periodic,
                                t_multiple_of_100ms& multiple,
                                t_messenger_key&     key,
                                t_messenger_prio&    prio,
                                t_messenger_user&    user) {
    return read_timeout_msg_(*this, periodic, multiple, key, prio, user);
  }

///////////////////////////////////////////////////////////////////////////////

  t_bool write_fail_msg_(r_message                msg,
                         t_fail_message::t_reason, //XXX - must write it
                         R_messenger_key          key,
                         R_message                fail_msg) {
    const t_n_ max = sizeof(t_hdr_) + get(fail_msg.get_capacity());
    if (msg == VALID && fail_msg == VALID && get(msg.get_capacity()) >= max) {

      write_hdr_(msg,
                 t_id{MAIN,
                 t_user{MSG_FAIL},
                 t_version{1}},
                 fail_msg.get_capacity(),
                 key,
                 key,
                 1,
                 0);

      msg.mk_view(t_ix{sizeof(t_hdr_)},
                  t_ix{sizeof(t_hdr_) +
                       get(fail_msg.get_capacity())}) = fail_msg.mk_view();
      return true;
    }
    return false;
  }

  t_bool read_fail_msg_(R_message                 msg,
                        t_fail_message::t_reason& reason,
                        r_message                 fail_msg) {
    const t_n_ max = sizeof(t_hdr_) + get(fail_msg.get_capacity());
    if (msg == VALID && fail_msg == VALID && get(msg.get_capacity()) >= max) {
      t_hdr_cptr_ hdr{msg.mk_cview(t_ix{0}, t_ix{sizeof(t_hdr_)})};
      t_n_ len  = hdr->len;
      reason = (t_fail_message::t_reason)hdr->user;

      if (fail_msg == INVALID || len > get(fail_msg.get_capacity())) {
        t_message tmp{t_n{len}};
        fail_msg = std::move(tmp);
      }
      if (fail_msg == VALID) {
        fail_msg.mk_view() = msg.mk_view(t_ix{sizeof(t_hdr_)}, t_ix{len});
        return true;
      }
    }
    return false;
  }

  t_bool t_fail_message::get(t_reason& reason, r_message fail_msg) {
    return read_fail_msg_(*this, reason, fail_msg);
  }

///////////////////////////////////////////////////////////////////////////////

  struct t_alive_ {
    int i_;
  };

  t_bool write_alive_msg_(r_message msg, R_messenger_key key) {
    const t_n_ max = sizeof(t_hdr_) + sizeof(t_alive_);
    if (msg == VALID && get(msg.get_capacity()) >= max) {
      write_hdr_(msg, t_id{MAIN, t_user{MSG_ALIVE}, t_version{1}},
                 t_n{sizeof(t_alive_)}, key, t_messenger_key{0}, 1, 0);
      return true;
    }
    return false;
  }

  t_bool read_alive_msg_(R_message msg) {
    const t_n_ max = sizeof(t_hdr_) + sizeof(t_alive_);
    if (msg == VALID && get(msg.get_capacity()) >= max) {
      return true;
    }
    return false;
  }

  t_alive_message::t_alive_message() : t_message{t_n{sizeof(t_alive_)}} {
  }

  t_bool t_alive_message::get() {
    return read_alive_msg_(*this);
  }

  inline t_bool operator==(R_message, R_message) { return false; }
}

///////////////////////////////////////////////////////////////////////////////

  struct t_update_params_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 1;
    R_params params;

    inline
    t_update_params_cmd_(R_params _params) : t_cmd{cmd_id}, params(_params) {
    };
  };
  using r_update_params_cmd_ = t_prefix<t_update_params_cmd_>::r_;

  struct t_fetch_params_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 2;
    r_params params;

    inline
    t_fetch_params_cmd_(r_params _params) : t_cmd{cmd_id}, params(_params) {
    };
  };
  using r_fetch_params_cmd_ = t_prefix<t_fetch_params_cmd_>::r_;

  struct t_create_messenger_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 3;
    R_messenger_name            name;
    R_messenger_create_params   params;
    t_messenger_key             id = t_messenger_key{0};
    t_maybe_messenger_processor processor;

    inline
    t_create_messenger_cmd_(R_messenger_name          _name,
                            R_messenger_create_params _params)
      : t_cmd{cmd_id}, name(_name), params(_params)  {
    }
  };
  using r_create_messenger_cmd_ = t_prefix<t_create_messenger_cmd_>::r_;

  struct t_destroy_messenger_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 4;
    R_messenger_key id;

    inline
    t_destroy_messenger_cmd_(R_messenger_key _id) : t_cmd{cmd_id}, id(_id) {
    }
  };
  using r_destroy_messenger_cmd_ = t_prefix<t_destroy_messenger_cmd_>::r_;

  struct t_is_messenger_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 5;
    R_messenger_name   name;
    r_messenger_params params;
    t_bool             found = false;

    inline
    t_is_messenger_cmd_(R_messenger_name _name, r_messenger_params _params)
      : t_cmd{cmd_id}, name{_name}, params{_params} {
    }
  };
  using r_is_messenger_cmd_ = t_prefix<t_is_messenger_cmd_>::r_;

  struct t_is_messenger_info_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 6;
    R_messenger_name name;
    r_messenger_info info;
    t_bool           clearstats;
    t_bool           found = false;

    inline
    t_is_messenger_info_cmd_(R_messenger_name _name,
                             r_messenger_info _info, t_bool _clearstats)
      : t_cmd{cmd_id}, name{_name}, info{_info}, clearstats{_clearstats} {
    }
  };
  using r_is_messenger_info_cmd_ = t_prefix<t_is_messenger_info_cmd_>::r_;

  struct t_fetch_messengers_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 7;
    r_messenger_infos infos;
    t_bool            clearstats;

    inline
    t_fetch_messengers_cmd_(r_messenger_infos _infos, t_bool _clearstats)
      : t_cmd{cmd_id}, infos{_infos}, clearstats{_clearstats} {
    }
  };
  using r_fetch_messengers_cmd_ = t_prefix<t_fetch_messengers_cmd_>::r_;

  struct t_create_group_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 8;
    R_password        password;
    R_messenger_name  name;
    t_messenger_scope scope;

    inline
    t_create_group_cmd_(R_password        _password,
                        R_messenger_name  _name,
                        t_messenger_scope _scope)
      : t_cmd{cmd_id}, password{_password}, name{_name}, scope{_scope} {
    }
  };
  using r_create_group_cmd_ = t_prefix<t_create_group_cmd_>::r_;

  struct t_destroy_group_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 9;
    R_password       password;
    R_messenger_name name;

    inline
    t_destroy_group_cmd_(R_password _password, R_messenger_name _name)
      : t_cmd{cmd_id}, password{_password}, name{_name} {
    }
  };
  using r_destroy_group_cmd_ = t_prefix<t_destroy_group_cmd_>::r_;

  struct t_is_group_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 10;
    R_messenger_name      name;
    r_messenger_scope     scope;
    p_messenger_name_list name_list;
    t_bool                found = false;

    inline
    t_is_group_cmd_(R_messenger_name      _name,
                    r_messenger_scope     _scope,
                    p_messenger_name_list _name_list)
      : t_cmd{cmd_id}, name{_name}, scope{_scope}, name_list{_name_list} {
    }
  };
  using r_is_group_cmd_ = t_prefix<t_is_group_cmd_>::r_;

  struct t_add_messenger_to_group_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 11;
    R_messenger_password password;
    R_messenger_name     name;
    R_messenger_name     group;
    t_messenger_prio     prio;
    t_messenger_user     user;

    t_add_messenger_to_group_cmd_(R_messenger_password _password,
                                  R_messenger_name _name,
                                  R_messenger_name _group,
                                  t_messenger_prio _prio,
                                  t_messenger_user _user)
      : t_cmd{cmd_id}, password{_password}, name{_name}, group{_group},
        prio{_prio}, user{_user} {
    }
  };
  using r_add_messenger_to_group_cmd_ =
    t_prefix<t_add_messenger_to_group_cmd_>::r_;

  struct t_remove_messenger_from_group_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 12;
    R_messenger_password password;
    R_messenger_name     name;
    R_messenger_name     group;
    p_messenger_user     user;

    inline
    t_remove_messenger_from_group_cmd_(R_messenger_password _password,
                                       R_messenger_name _name,
                                       R_messenger_name _group,
                                       p_messenger_user _user)
      : t_cmd{cmd_id}, password{_password}, name{_name}, group{_group},
        user{_user} {
    }
  };
  using r_remove_messenger_from_group_cmd_ =
    t_prefix<t_remove_messenger_from_group_cmd_>::r_;

  struct t_is_messenger_in_group_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 13;
    R_messenger_name name;
    R_messenger_name group;
    p_messenger_user user;
    t_bool           found = false;

    inline
    t_is_messenger_in_group_cmd_(R_messenger_name _name,
                                 R_messenger_name _group,
                                 p_messenger_user _user)
      : t_cmd{cmd_id}, name{_name}, group{_group}, user{_user} {
    }
  };
  using r_is_messenger_in_group_cmd_ =
    t_prefix<t_is_messenger_in_group_cmd_>::r_;

  struct t_fetch_messenger_groups_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 14;
    R_messenger_name       name;
    r_messenger_group_list group_list;

    inline
    t_fetch_messenger_groups_cmd_(R_messenger_name       _name,
                                  r_messenger_group_list _group_list)
      : t_cmd{cmd_id}, name{_name}, group_list{_group_list} {
    }
  };
  using r_fetch_messenger_groups_cmd_ =
    t_prefix<t_fetch_messenger_groups_cmd_>::r_;

  struct t_who_is_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 15;
    R_messenger_key  key;
    r_messenger_name name;
    p_bool           group;
    p_bool           local;

    inline
    t_who_is_cmd_(R_messenger_key _key, r_messenger_name _name,
                  p_bool _group, p_bool _local)
      : t_cmd{cmd_id}, key{_key}, name{_name}, group{_group}, local{_local} {
    }
  };
  using r_who_is_cmd_ = t_prefix<t_who_is_cmd_>::r_;

  struct t_get_name_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 16;
    R_messenger_key   id;
    t_messenger_name name;

    inline
    t_get_name_cmd_(R_messenger_key _id) : t_cmd{cmd_id}, id{_id} {
    }
  };
  using r_get_name_cmd_ = t_prefix<t_get_name_cmd_>::r_;

  struct t_get_params_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 17;
    R_messenger_key    id;
    r_messenger_params params;

    inline
    t_get_params_cmd_(R_messenger_key _id, r_messenger_params _params)
      : t_cmd{cmd_id}, id{_id}, params{_params} {
    }
  };
  using r_get_params_cmd_ = t_prefix<t_get_params_cmd_>::r_;

  struct t_update_scope_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 18;
    R_messenger_key   id;
    t_messenger_scope scope;

    inline
    t_update_scope_cmd_(R_messenger_key _id, t_messenger_scope _scope)
      : t_cmd{cmd_id}, id{_id}, scope{_scope} {
    }
  };
  using r_update_scope_cmd_ = t_prefix<t_update_scope_cmd_>::r_;

  struct t_update_alive_period_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 19;
    R_messenger_key     id;
    t_multiple_of_100ms multiple;

    inline
    t_update_alive_period_cmd_(R_messenger_key     _id,
                               t_multiple_of_100ms _multiple)
      : t_cmd{cmd_id}, id{_id}, multiple{_multiple} {
    }
  };
  using r_update_alive_period_cmd_ = t_prefix<t_update_alive_period_cmd_>::r_;

  struct t_start_timer_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 20;
    R_messenger_key          id;
    R_messenger_timer_params params;

    inline
    t_start_timer_cmd_(R_messenger_key _id, R_messenger_timer_params _params)
      : t_cmd{cmd_id}, id{_id}, params{_params} {
    }
  };
  using r_start_timer_cmd_ = t_prefix<t_start_timer_cmd_>::r_;

  struct t_stop_timer_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 21;
    R_messenger_key id;

    inline
    t_stop_timer_cmd_(R_messenger_key _id) : t_cmd{cmd_id}, id{_id} {
    }
  };
  using r_stop_timer_cmd_ = t_prefix<t_stop_timer_cmd_>::r_;

  struct t_query_timer_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 22;
    R_messenger_key          id;
    r_messenger_timer_params params;

    inline
    t_query_timer_cmd_(R_messenger_key _id, r_messenger_timer_params _params)
      : t_cmd{cmd_id}, id{_id}, params{_params} {
    }
  };
  using r_query_timer_cmd_ = t_prefix<t_query_timer_cmd_>::r_;

  struct t_add_to_group_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 23;
    R_messenger_key      id;
    R_messenger_password password;
    R_messenger_name     group;
    t_messenger_prio     prio;
    t_messenger_user     user;

    inline
    t_add_to_group_cmd_(R_messenger_key      _id,
                        R_messenger_password _password,
                        R_messenger_name     _group,
                        t_messenger_prio     _prio,
                        t_messenger_user     _user)
     : t_cmd{cmd_id}, id{_id}, password{_password}, group{_group}, prio{_prio},
       user{_user} {
    }
  };
  using r_add_to_group_cmd_ = t_prefix<t_add_to_group_cmd_>::r_;

  struct t_remove_from_group_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 24;
    R_messenger_key      id;
    R_messenger_password password;
    R_messenger_name     group;
    p_messenger_user     user;

    inline
    t_remove_from_group_cmd_(R_messenger_key      _id,
                             R_messenger_password _password,
                             R_messenger_name     _group,
                             p_messenger_user     _user)
     : t_cmd{cmd_id}, id{_id}, password{_password}, group{_group}, user{_user} {
    }
  };
  using r_remove_from_group_cmd_ = t_prefix<t_remove_from_group_cmd_>::r_;

  struct t_is_in_group_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 25;
    R_messenger_key  id;
    R_messenger_name group;
    p_messenger_user user;
    t_bool           found = false;

    inline
    t_is_in_group_cmd_(R_messenger_key  _id,
                       R_messenger_name _group,
                       p_messenger_user _user)
      : t_cmd{cmd_id}, id{_id}, group{_group}, user{_user} {
    }
  };
  using r_is_in_group_cmd_ = t_prefix<t_is_in_group_cmd_>::r_;

  struct t_get_groups_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 26;
    R_messenger_key        id;
    r_messenger_group_list group_list;

    inline
    t_get_groups_cmd_(R_messenger_key _id, r_messenger_group_list _group_list)
      : t_cmd{cmd_id}, id{_id}, group_list{_group_list} {
    }
  };
  using r_get_groups_cmd_ = t_prefix<t_get_groups_cmd_>::r_;

  struct t_add_monitor_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 27;
    R_messenger_key  id;
    R_messenger_name name;
    t_messenger_prio prio;
    t_messenger_user user;

    inline
    t_add_monitor_cmd_(R_messenger_key  _id,
                       R_messenger_name _name,
                       t_messenger_prio _prio,
                       t_messenger_user _user)
      : t_cmd{cmd_id}, id{_id}, name{_name}, prio{_prio}, user{_user} {
    }
  };
  using r_add_monitor_cmd_ = t_prefix<t_add_monitor_cmd_>::r_;

  struct t_remove_monitor_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 28;
    R_messenger_key  id;
    R_messenger_name name;
    p_messenger_user user;

    inline
    t_remove_monitor_cmd_(R_messenger_key  _id,
                          R_messenger_name _name,
                          p_messenger_user _user)
      : t_cmd{cmd_id}, id{_id}, name{_name}, user{_user} {
    }
  };
  using r_remove_monitor_cmd_ = t_prefix<t_remove_monitor_cmd_>::r_;

  struct t_is_monitored_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 29;
    t_messenger_key  key = t_messenger_key{0};
    R_messenger_key  id;
    R_messenger_name name;
    p_messenger_user user;

    inline
    t_is_monitored_cmd_(R_messenger_key  _id,
                        R_messenger_name _name,
                        p_messenger_user _user)
      : t_cmd{cmd_id}, id{_id}, name{_name}, user{_user} {
    }
  };
  using r_is_monitored_cmd_ = t_prefix<t_is_monitored_cmd_>::r_;

  struct t_get_monitored_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 30;
    R_messenger_key          id;
    r_messenger_monitor_list monitor_list;

    inline
    t_get_monitored_cmd_(R_messenger_key          _id,
                         r_messenger_monitor_list _monitor_list)
      : t_cmd{cmd_id}, id{_id}, monitor_list{_monitor_list} {
    }
  };
  using r_get_monitored_cmd_ = t_prefix<t_get_monitored_cmd_>::r_;

  struct t_clean_death_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 31;

    inline
    t_clean_death_cmd_() : t_cmd{cmd_id} {
    };
  };
  using r_clean_death_cmd_ = t_prefix<t_clean_death_cmd_>::r_;

///////////////////////////////////////////////////////////////////////////////

  using t_msgs_         = std::multimap<t_messenger_prio, t_message>;
  using r_msgs_         = t_prefix<t_msgs_>::r_;
  using t_msgs_entry_   = t_msgs_::value_type;
  using t_lookup_       = std::map<t_messenger_name, t_messenger_key>;
  using t_lookup_entry_ = t_lookup_::value_type;

  struct t_tmr_ {
    t_messenger_timer_params params;
    t_time                   start;
    t_multiple_of_100ms      cnt = t_multiple_of_100ms{0};
  };
  using t_tmr_lookup_       = std::map<t_messenger_key, t_tmr_>;
  using t_tmr_lookup_entry_ = t_tmr_lookup_::value_type;

  struct t_alive_info_ {
    t_uint16            cnt;
    t_multiple_of_100ms current;
    t_multiple_of_100ms max;

    t_alive_info_() : cnt{0}, current{0}, max{2} {
    }
  };

  struct t_msgr_ctxt_ {
    t_messenger_client client;
    t_alive_info_      alive;
    t_messenger_info   info;

    t_msgr_ctxt_(x_messenger_client _client) : client{std::move(_client)} {
    }
  };
  using r_msgr_ctxt_  = t_prefix<t_msgr_ctxt_>::r_;
  using t_msgr_ctxts_ = t_freelist<t_msgr_ctxt_, 300>;

  struct t_grp_member_ {
    t_messenger_password password;
    t_messenger_key      key;
    t_messenger_prio     prio;
    t_messenger_user     user;

    t_grp_member_(R_messenger_password _password, R_messenger_key _key,
                  t_messenger_prio _prio, t_messenger_user _user)
      : password{_password}, key{_key}, prio{_prio}, user{_user} {
    }
  };
  using r_grp_member_ = t_prefix<t_grp_member_>::r_;
  using t_grp_member_lookup_ = std::map<t_messenger_name, t_grp_member_>;
  using t_grp_member_lookup_entry_ = t_grp_member_lookup_::value_type;

  struct t_grp_ctxt_ {
    t_messenger_key      key   = t_messenger_key{0};
    t_messenger_scope    scope;
    t_messenger_name     name;
    t_password           password;
    t_grp_member_lookup_ members;
  };
  using p_grp_ctxt_  = t_prefix<t_grp_ctxt_>::p_;
  using R_grp_ctxt_  = t_prefix<t_grp_ctxt_>::R_;
  using t_grp_ctxts_ = t_freelist<t_grp_ctxt_, 2000>;

  struct t_monitored_by_ {
    t_messenger_key  key;
    t_messenger_prio prio;
    t_messenger_user user;

    t_monitored_by_(R_messenger_key _key, t_messenger_prio _prio,
                    t_messenger_user _user)
      : key{_key}, prio{_prio}, user{_user} {
    }
  };
  using R_monitored_by_ = t_prefix<t_monitored_by_>::R_;
  using t_monitored_by_lookup_ = std::map<t_messenger_name, t_monitored_by_>;
  using t_monitored_by_lookup_entry_ = t_monitored_by_lookup_::value_type;

  struct t_monitored_ {
    t_messenger_state      state = message::STATE_UNAVAILABLE;
    t_messenger_key        key   = t_messenger_key{0};
    t_monitored_by_lookup_ by;
  };
  using R_monitored_ = t_prefix<t_monitored_>::R_;
  using r_monitored_ = t_prefix<t_monitored_>::r_;

  using t_monitored_lookup_ = std::map<t_messenger_name, t_monitored_>;
  using t_monitored_lookup_entry_ = t_monitored_lookup_::value_type;

///////////////////////////////////////////////////////////////////////////////

  class t_data_ {
  public:
    t_name       name;
    t_params     params;

    t_data_(R_name _name, R_params _params) : name{_name}, params(_params) {
    }

    t_void update_msgs(r_msgs_ msgs, t_messenger_state state,
                       p_grp_ctxt_ ctxt, r_grp_member_ member) {
      auto key_params{get_key_params(member.key)};
      if (!key_params.group || !key_params.local) {
        message::t_notify_message msg;
        message::write_notify_msg_(msg,
                                   member.key,
                                   state,
                                   ctxt->name,
                                   ctxt->key,
                                   member.prio,
                                   member.user);
        msgs.insert(t_msgs_entry_{member.prio, std::move(msg)});
      } else {
        auto ctxt = grp_ctxts_.get(key_params.id);
        if (ctxt && is_valid(ctxt->key))
          for (auto& member : ctxt->members)
            update_msgs(msgs, state, ctxt, member.second);
      }
    }

    t_void update_msgs(r_msgs_ msgs, t_messenger_state state,
                       p_grp_ctxt_ ctxt) {
      for (auto& member : ctxt->members)
        update_msgs(msgs, state, ctxt, member.second);
    }

    t_void update_msgs(r_msgs_ msgs, R_messenger_name name,
                       R_monitored_ monitored, R_monitored_by_ monitor) {
      message::t_notify_message msg;
      message::write_notify_msg_(msg,
                                 monitor.key,
                                 monitored.state,
                                 name,
                                 monitored.key,
                                 monitor.prio,
                                 monitor.user);
      msgs.insert(t_msgs_entry_{monitor.prio, std::move(msg)});
    }

    t_void update_msgs(r_msgs_ msgs, R_messenger_name name,
                       R_monitored_ monitored) {
      for (auto& monitor : monitored.by)
        update_msgs(msgs, name, monitored, monitor.second);
    }

    t_void add_messenger(r_err err, r_msgs_ msgs, r_messenger_key id,
                         r_maybe_messenger_processor processor,
                         R_messenger_name name,
                         R_messenger_create_params params) {
      auto p = lookup_.insert(t_lookup_entry_{name, t_messenger_key{0}});
      if (p.second) {
        processor.emplace(err, t_n{100});
        mt::chained_queue::t_user user{0L};
        auto r = msgr_ctxts_.insert(err, set(processor).make_client(err, user));
        if (r) {
          auto& ctxt = *r;
          id = make_key(0/* sequence - XXX */, r.id);
          ctxt.info.key    = p.first->second = id;
          ctxt.info.name   = name;
          ctxt.info.params.scope        = params.scope;
          ctxt.info.params.alive_factor = params.alive_factor;
          ctxt.info.params.timer_params = params.timer_params;

          if (get(ctxt.info.params.timer_params.factor.value)) {
            auto& tmr  = timers_[ctxt.info.key];
            tmr.params = ctxt.info.params.timer_params;
            tmr.cnt    = t_multiple_of_100ms{0};
            tmr.start  = t_time();
          }

          auto m = monitored_.find(ctxt.info.name);
          if (m != monitored_.end()) {
            m->second.key   = ctxt.info.key;
            m->second.state = message::STATE_AVAILABLE;
            update_msgs(msgs, m->first, m->second);
          }
        } else {
          err = err::E_XXX;
          lookup_.erase(p.first);
          processor.release();
        }
      } else
        err = err::E_XXX;
    }

    t_void destroy_messenger(r_err err, r_msgs_ msgs, R_messenger_key key) {
      auto mid  = get_ctxt_id(key);
      auto ctxt = msgr_ctxts_.get(mid);
      if (ctxt) {
        for (auto& monitor : ctxt->info.params.monitor_list) {
          auto m = monitored_.find(monitor.first);
          r_monitored_ entry = m->second;
          entry.by.erase(ctxt->info.name);
          if (entry.by.empty())
            monitored_.erase(m);
        }

        for (auto& group : ctxt->info.params.group_list) {
          auto g     = lookup_.find(group.first);
          auto gid   = get_ctxt_id(g->second);
          auto gctxt = grp_ctxts_.get(gid);
          gctxt->members.erase(ctxt->info.name);
          if (gctxt->members.empty() && !is_valid(gctxt->key)) {
            grp_ctxts_.erase(gid);
            lookup_.erase(g);
          }
        }

        timers_.erase(ctxt->info.key);

        auto m = monitored_.find(ctxt->info.name);
        if (m != monitored_.end()) {
          set(m->second.key) = 0;
          m->second.state = message::STATE_UNAVAILABLE;
          update_msgs(msgs, m->first, m->second);
        }

        lookup_.erase(ctxt->info.name);
        msgr_ctxts_.erase(mid);
      } else
        err = err::E_XXX;
    }

    t_bool is_messenger(r_err err, R_messenger_name name,
                        r_messenger_params params) {
      auto m = lookup_.find(name);
      if (m != lookup_.end()) {
        if (!is_group(m->second) && is_local(m->second)) {
          auto ctxt = msgr_ctxts_.get(get_ctxt_id(m->second));
          params = ctxt->info.params;
          return true;
        }
      } else
        err = err::E_XXX;
      return false;
    }

    t_bool is_messenger(r_err err, R_messenger_name name,
                        r_messenger_info info, t_bool clearstats) {
      auto m = lookup_.find(name);
      if (m != lookup_.end()) {
        if (!is_group(m->second) && is_local(m->second)) {
          auto ctxt = msgr_ctxts_.get(get_ctxt_id(m->second));
          info = ctxt->info;
          if (clearstats)
            ctxt->info.stats.reset();
          return true;
        }
      } else
        err = err::E_XXX;
      return false;
    }

    t_void get_messenger_name(r_err err, r_messenger_name name,
                              R_messenger_key key) {
      auto mid  = get_ctxt_id(key);
      auto ctxt = msgr_ctxts_.get(mid);
      if (ctxt)
         name = ctxt->info.name;
      else
        err = err::E_XXX;
    }

    t_void create_group(r_err err, r_msgs_ msgs, R_password password,
                        R_messenger_name name, t_messenger_scope scope) {
      p_grp_ctxt_ ctxt = nullptr;
      auto pair = lookup_.insert(t_lookup_entry_{name, t_messenger_key{0}});
      if (pair.second) {
        auto result = grp_ctxts_.insert(err);
        if (result) {
          pair.first->second = make_group_key(0, result.id); //XXX - grpup key;
          ctxt = result.ptr;
          ctxt->key      = pair.first->second;
          ctxt->name     = name;
          ctxt->scope    = scope;
          ctxt->password = password;
          update_msgs(msgs, message::STATE_AVAILABLE, ctxt);
        } else {
          lookup_.erase(pair.first);
          err = err::E_XXX;
        }
      } else {
        if (pair.first != lookup_.end()) {
          auto& key = pair.first->second;
          if (is_group(key) && is_local(key)) {
            ctxt = grp_ctxts_.get(get_ctxt_id(key));
            if (!is_valid(ctxt->key)) {
              ctxt->key      = key;
              ctxt->scope    = scope;
              ctxt->password = password;
              update_msgs(msgs, message::STATE_AVAILABLE, ctxt);
            } else
              err = err::E_XXX;
          } else
            err = err::E_XXX;
        } else
          err = err::E_XXX;
      }

      if (!err && ctxt) {
        auto monitored = monitored_.find(name);
        if (monitored != monitored_.end()) {
          monitored->second.key   = pair.first->second;
          monitored->second.state = message::STATE_AVAILABLE;
          update_msgs(msgs, monitored->first, monitored->second);
        }
      }
    }

    t_void destroy_group(r_err err, r_msgs_ msgs, R_password password,
                         R_messenger_name name) {
      auto itr = lookup_.find(name);
      if (itr != lookup_.end()) {
        auto& key = itr->second;
        if (is_group(key) && is_local(key)) {
          p_grp_ctxt_ ctxt = grp_ctxts_.get(get_ctxt_id(key));
          if (is_valid(ctxt->key)) {
            if (password == ctxt->password) {
              update_msgs(msgs, message::STATE_UNAVAILABLE, ctxt);
              set(ctxt->key) = 0;
            } else
              err = err::E_XXX;
          } else
            err = err::E_XXX;
        } else
           err = err::E_XXX;
      } else
        err = err::E_XXX;

      if (!err) {
        auto monitored = monitored_.find(name);
        if (monitored != monitored_.end()) {
          monitored->second.state = message::STATE_UNAVAILABLE;
          update_msgs(msgs, monitored->first, monitored->second);
        }
      }
    }

    t_void add_messenger_to_group(r_err err, r_msgs_ msgs,
                                  R_messenger_password password,
                                  R_messenger_name name, R_messenger_name group,
                                  t_messenger_prio prio,
                                  t_messenger_user user) {
      auto msgr = lookup_.find(name);
      if (msgr != lookup_.end()) {
        p_grp_ctxt_ ctxt = nullptr;
        auto pair = lookup_.insert(t_lookup_entry_{group, t_messenger_key{0}});
        if (pair.second) {
          auto result = grp_ctxts_.insert(err);
          if (result) {
            pair.first->second = make_group_key(0, result.id); //XXX - grpup key;
            ctxt = result.ptr;
            ctxt->name = name;
          } else {
            lookup_.erase(pair.first);
            err = err::E_XXX;
          }
        } else {
          if (pair.first != lookup_.end()) {
            auto& key = pair.first->second;
            if (is_group(key) && is_local(key))
              ctxt = grp_ctxts_.get(get_ctxt_id(key));
            else
              err = err::E_XXX;
          } else
            err = err::E_XXX;
        }
        if (!err && ctxt) {
          t_grp_member_lookup_entry_ entry{name,
                                           t_grp_member_{password,
                                                         msgr->second, prio,
                                                         user}};
          auto pair = ctxt->members.insert(std::move(entry));
          if (!pair.second)
            err = err::E_XXX;
          else if (is_valid(ctxt->key))
            update_msgs(msgs, message::STATE_AVAILABLE, ctxt,
                        pair.first->second);
        }
      }
    }

    t_void remove_messenger_from_group(r_err err,
                                       R_messenger_password password,
                                       R_messenger_name name,
                                       R_messenger_name group,
                                       p_messenger_user user) {
      auto grp = lookup_.find(group);
      if (grp != lookup_.end()) {
        auto& key = grp->second;
        if (is_group(key) && is_local(key)) {
          auto id   = get_ctxt_id(key);
          auto ctxt = grp_ctxts_.get(id);
          auto mgr = ctxt->members.find(name);
          if (mgr != ctxt->members.end()) {
            if (mgr->second.password == password) {
              if (user)
                *user = mgr->second.user;
              ctxt->members.erase(mgr);
              if (ctxt->members.empty() && !is_valid(ctxt->key)) {
                grp_ctxts_.erase(id);
                lookup_.erase(grp);
              }
            } else
              err = err::E_XXX;
          } else
            err = err::E_XXX;
        } else
          err = err::E_XXX;
      } else
        err = err::E_XXX;
    }

    t_bool is_messenger_group(r_err err, R_messenger_name group,
                              r_messenger_scope scope,
                              p_messenger_name_list name_list) {
      auto grp = lookup_.find(group);
      if (grp != lookup_.end()) {
        auto& key = grp->second;
        if (is_group(key) && is_local(key)) {
          auto id   = get_ctxt_id(key);
          auto ctxt = grp_ctxts_.get(id);
          scope = ctxt->scope;
          if (name_list) {
            for (auto& member : ctxt->members)
              name_list->push_back(member.first);
          }
          return true;
        }
      }
      return false;
    }

    t_void add_messenger_to_group(r_err err, r_msgs_ msgs,
                                  R_messenger_password password,
                                  R_messenger_key key, R_messenger_name group,
                                  t_messenger_prio prio,
                                  t_messenger_user user) {
      if (!is_group(key) && is_local(key)) {
        auto ctxt = msgr_ctxts_.get(get_ctxt_id(key));
        if (ctxt) {
          add_messenger_to_group(err, msgs, password, ctxt->info.name, group,
                                 prio, user);
          if (!err) {
            messenger::t_group_list::value_type
              entry{group, messenger::t_group{group, password, prio, user}};
            ctxt->info.params.group_list.insert(std::move(entry));
          }
        } else
          err = err::E_XXX;
      } else
        err = err::E_XXX;
    }

    t_void remove_messenger_from_group(r_err err, R_messenger_password password,
                                       R_messenger_key key,
                                       R_messenger_name group,
                                       p_messenger_user user) {
      if (!is_group(key) && is_local(key)) {
        auto ctxt = msgr_ctxts_.get(get_ctxt_id(key));
        if (ctxt) {
          remove_messenger_from_group(err, password, ctxt->info.name, group,
                                      user);
          if (!err)
            ctxt->info.params.group_list.erase(group);
        } else
          err = err::E_XXX;
      } else
        err = err::E_XXX;
    }

    t_void add_monitor(r_err err, r_msgs_ msgs, R_messenger_key key,
                       R_messenger_name name, t_messenger_prio prio,
                       t_messenger_user user) {
      if (!is_group(key) && is_local(key)) {
        auto ctxt = msgr_ctxts_.get(get_ctxt_id(key));
        if (ctxt) {
          messenger::t_monitor_list::value_type
            tmp{name, messenger::t_monitor{name, prio, user}};
          auto mpair = ctxt->info.params.monitor_list.insert(std::move(tmp));
          if (mpair.second) {
            t_monitored_lookup_entry_ tmp{name, t_monitored_{}};
            auto pair = monitored_.insert(std::move(tmp));
            if (pair.second || pair.first != monitored_.end()) {
              auto& monitored = pair.first->second;
              t_monitored_by_lookup_entry_
                tmp{ctxt->info.name, t_monitored_by_{key, prio, user}};
              auto member = monitored.by.insert(std::move(tmp));
              if (member.second) {
                auto lk = lookup_.find(name);
                if (lk != lookup_.end()) {
                  auto& key = lk->second;
                  if (is_local(key) && is_group(key)) {
                    auto gctxt = grp_ctxts_.get(get_ctxt_id(key));
                    monitored.state = is_valid(gctxt->key) ?
                                        message::STATE_AVAILABLE :
                                        message::STATE_UNAVAILABLE;
                  } else
                    monitored.state = message::STATE_AVAILABLE;

                  if (monitored.state == message::STATE_AVAILABLE)
                    update_msgs(msgs, name, monitored, member.first->second);
                }
                return;
              } else
                err = err::E_XXX;
              if (monitored.by.empty())
                monitored_.erase(pair.first);
            } else
              err = err::E_XXX;
            ctxt->info.params.monitor_list.erase(mpair.first);
          } else
            err = err::E_XXX;
        } else
          err = err::E_XXX;
      } else
        err = err::E_XXX;
    }

    t_void remove_monitor(r_err err, R_messenger_key key,
                          R_messenger_name name, p_messenger_user user) {
      if (!is_group(key) && is_local(key)) {
        auto ctxt = msgr_ctxts_.get(get_ctxt_id(key));
        if (ctxt) {
          auto monitor = ctxt->info.params.monitor_list.find(name);
          if (monitor != ctxt->info.params.monitor_list.end()) {
            ctxt->info.params.monitor_list.erase(monitor);
            auto monitored = monitored_.find(name);
            if (monitored != monitored_.end()) {
              auto monitor = monitored->second.by.find(ctxt->info.name);
              if (monitor != monitored->second.by.end()) {
                if (user)
                  *user = monitor->second.user;
                monitored->second.by.erase(monitor);
                if (monitored->second.by.empty())
                  monitored_.erase(monitored);
              } else
                err = err::E_XXX;
            } else
              err = err::E_XXX;
          } else
            err = err::E_XXX;
        } else
          err = err::E_XXX;
      } else
        err = err::E_XXX;
    }

    t_bool is_messenger_in_group(r_err err, R_messenger_name name,
                                 R_messenger_name group,
                                 p_messenger_user user) {
      auto grp = lookup_.find(group);
      if (grp != lookup_.end()) {
        auto& key = grp->second;
        if (is_group(key) && is_local(key)) {
          auto id   = get_ctxt_id(key);
          auto ctxt = grp_ctxts_.get(id);
          auto member = ctxt->members.find(name);
          if (member != ctxt->members.end()) {
            if (user)
              *user = member->second.user;
            return true;
          }
        }
      }
      return false;
    }

    t_void who_is(r_err err, R_messenger_key key,
                  r_messenger_name name, p_bool group, p_bool local) {
      auto key_params = get_key_params(key);
      if (key_params.local) {
        if (key_params.group) {
          auto ctxt = grp_ctxts_.get(key_params.id);
          name = ctxt->name;
        } else {
          auto ctxt = msgr_ctxts_.get(key_params.id);
          name = ctxt->info.name;
        }
      } else
        err = err::E_XXX; // XXX remote -table

      if (!err) {
        if (group)
          *group = key_params.group;
        if (local)
          *local = key_params.local;
      }
    }

    t_void forward_msgs(r_msgs_ msgs) {
      for (auto& msg : msgs) {
        auto key_params = get_key_params(message::read_dst(msg.second));
        if (key_params.local) {
          if (key_params.group) {
            auto ctxt = grp_ctxts_.get(key_params.id);
            if (ctxt) { // XXX - seq not checked
            } else {
              // trace and send back if it is in this process
            }
          } else {
            auto ctxt = msgr_ctxts_.get(key_params.id);
            if (ctxt) { // XXX - seq not checked
              auto chain = ctxt->client.acquire();
              chain.head->ref().emplace<t_message>({0L},
                                                   std::move(msg.second));
              auto errn = ctxt->client.insert(chain);
              if (errn == INVALID) {
              }
            } else {
              // trace and send back if it is in this process
            }
          }
        } else {
          // XXX- remote not supported
        }
      }
      msgs.clear();
    }

  private:
    t_msgr_ctxts_       msgr_ctxts_;
    t_grp_ctxts_        grp_ctxts_;
    t_lookup_           lookup_;
    t_monitored_lookup_ monitored_;
    t_tmr_lookup_       timers_;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_logic_ : public t_thread::t_logic,
                   public t_cmd_processor::t_logic,
                   public t_que_processor::t_logic,
                   public t_dispatcher::t_logic {
  public:
    t_logic_(r_err err, R_name name, R_params params)
      : data_         {name, params},
        cmd_processor_{err},
        que_processor_{err, data_.params.queuesize},
        dispatcher_   {err, {t_n{2}, "epoll_service"}} {
    }

    t_cmd_client make_cmd_client() {
      return cmd_processor_.make_client(command::t_user{0L});
    }

    t_que_client make_que_client() {
      return que_processor_.make_client(waitable_chained_queue::t_user{0L});
    }

    virtual t_void update(t_thd_err, r_pthread_attr) noexcept override {
      t_out{"messaging: update"};
    }

    virtual t_void prepare(t_thd_err) noexcept override {
      t_out{"messaging: prepare"};
    }

    virtual p_void run() noexcept override {
      t_out{"messaging: run"};

      err::t_err err;
      {
        t_cmd_proxy_ cmd_proxy{err, ev_cmd_, cmd_processor_, *this};
        dispatcher_.add_event (err, {cmd_processor_.get_fd(), RD}, &cmd_proxy);

        t_que_proxy_ que_proxy{err, ev_cmd_, que_processor_, *this};
        dispatcher_.add_event (err, {que_processor_.get_fd(), RD}, &que_proxy);

        // timed messages
        // fdtimer
        // tipc topology service
        dispatcher_.event_loop(err, this);
      }

      if (err) {
        err.print();
        err.clear();
      }

      return nullptr;
    }

    virtual t_void may_reorder_events (r_event_infos) override {
      t_out{"messaging: may_reorder_events"};
    }

    virtual t_void notify_event_remove(r_event_info) override {
      t_out{"messaging: notify_event_remove"};
    }

    virtual t_quit notify_timeout(t_usec) override {
      t_out{"messaging: notify_timeout"};
      return true; // not required
    }

    virtual t_quit notify_error(t_errn)  override {
      t_out{"messaging: notify_error"};
      return true; // die
    }

    virtual t_quit notify_events_processed() override {
      t_out{"messaging: notify_events_processed"};
      data_.forward_msgs(msgs_);
      return false;
    }

    t_void process_chain(t_chain& chain) {
      for (auto item = chain.head; item; item = item->next())
        msgs_.insert(
          t_msgs_entry_{t_messenger_prio{0},
                        std::move(item->ref().any.ref<t_message>())});
    }

    virtual t_void async_process(t_chain chain) noexcept override {
      t_out{"messaging: recv"};
      process_chain(chain);
    }

    virtual t_void async_process(t_user, p_command) noexcept override {
      t_out{"messaging: p_command"};
    }

///////////////////////////////////////////////////////////////////////////////

    t_void process(err::t_err, r_update_params_cmd_) noexcept {
      t_out{"messaging: r_update_params_cmd_"};
      //XXX- later
    }

    t_void process(err::t_err err, r_fetch_params_cmd_ cmd) noexcept {
      t_out{"messaging: r_fetch_params_cmd_"};
      cmd.params = data_.params;
    }

    t_void process(err::t_err err, r_create_messenger_cmd_ cmd) noexcept {
      t_out{"messaging: r_create_messenger_cmd_"};
      data_.add_messenger(err, msgs_, cmd.id, cmd.processor, cmd.name,
                          cmd.params);
    }

    t_void process(err::t_err err, r_destroy_messenger_cmd_ cmd) noexcept {
      t_out{"messaging: r_destroy_messenger_cmd_"};
      data_.destroy_messenger(err, msgs_, cmd.id);
    }

    t_void process(err::t_err err, r_is_messenger_cmd_ cmd) noexcept {
      t_out{"messaging: r_is_messenger_cmd_"};
      cmd.found = data_.is_messenger(err, cmd.name, cmd.params);
    }

    t_void process(err::t_err err, r_is_messenger_info_cmd_ cmd) noexcept {
      t_out{"messaging: r_is_messenger_info_cmd_"};
      cmd.found = data_.is_messenger(err, cmd.name, cmd.info, cmd.clearstats);
    }

    t_void process(err::t_err, r_fetch_messengers_cmd_) noexcept {
      t_out{"messaging: r_fetch_messengers_cmd_"};
      //XXX- later
    }

    t_void process(err::t_err err, r_create_group_cmd_ cmd) noexcept {
      t_out{"messaging: r_create_group_cmd_"};
      data_.create_group(err, msgs_, cmd.password, cmd.name, cmd.scope);
    }

    t_void process(err::t_err err, r_destroy_group_cmd_ cmd) noexcept {
      t_out{"messaging: r_destroy_group_cmd_"};
      data_.destroy_group(err, msgs_, cmd.password, cmd.name);
    }

    t_void process(err::t_err err, r_is_group_cmd_ cmd) noexcept {
      t_out{"messaging: r_is_group_cmd_"};
      cmd.found = data_.is_messenger_group(err, cmd.name, cmd.scope,
                                           cmd.name_list);
    }

    t_void process(err::t_err err, r_add_messenger_to_group_cmd_ cmd) noexcept {
      t_out{"messaging: r_add_messenger_to_group_cmd_"};
      data_.add_messenger_to_group(err, msgs_, cmd.password, cmd.name,
                                   cmd.group, cmd.prio, cmd.user);
    }

    t_void process(err::t_err err,
                   r_remove_messenger_from_group_cmd_ cmd) noexcept {
      t_out{P_cstr{"messaging: r_destroy_messenger_from_group_cmd_"}};
      data_.remove_messenger_from_group(err, cmd.password, cmd.name, cmd.group,
                                        cmd.user);
    }

    t_void process(err::t_err err, r_is_messenger_in_group_cmd_ cmd) noexcept {
      t_out{"messaging: r_is_messenger_in_group_cmd_"};
      cmd.found = data_.is_messenger_in_group(err, cmd.name, cmd.group,
                                              cmd.user);
    }

    t_void process(err::t_err err, r_fetch_messenger_groups_cmd_ cmd) noexcept {
      t_out{"messaging: r_fetch_messenger_groups_cmd_"};
      //XXX- later
    }

    t_void process(err::t_err err, r_who_is_cmd_ cmd) noexcept {
      t_out{"messaging: r_who_is_cmd_"};
      data_.who_is(err, cmd.key, cmd.name, cmd.group, cmd.local);
    }

    t_void process(err::t_err err, r_get_name_cmd_ cmd) noexcept {
      t_out{"messaging: r_get_name_cmd_"};
      data_.get_messenger_name(err, cmd.name, cmd.id);
    }

    t_void process(err::t_err err, r_get_params_cmd_ cmd) noexcept {
      t_out{"messaging: r_get_params_cmd_"};
      //XXX-later - big step
    }

    t_void process(err::t_err, r_update_scope_cmd_) noexcept {
      t_out{"messaging: r_update_scope_cmd_"};
      //XXX-later - big one
    }

    t_void process(err::t_err, r_update_alive_period_cmd_) noexcept {
      t_out{"messaging: r_update_alive_period_cmd_"};
      //XXX-now
    }

    t_void process(err::t_err, r_start_timer_cmd_) noexcept {
      t_out{"messaging: r_start_timer_cmd_"};
      //XXX-now
    }

    t_void process(err::t_err, r_stop_timer_cmd_) noexcept {
      t_out{"messaging: r_stop_timer_cmd_"};
      //XXX-now
    }

    t_void process(err::t_err, r_query_timer_cmd_) noexcept {
      t_out{"messaging: r_query_timer_cmd_"};
      //XXX-now
    }

    t_void process(err::t_err err, r_add_to_group_cmd_ cmd) noexcept {
      t_out{"messaging: r_add_to_group_cmd_"};
      data_.add_messenger_to_group(err, msgs_, cmd.password, cmd.id,
                                   cmd.group, cmd.prio, cmd.user);
    }

    t_void process(err::t_err err, r_remove_from_group_cmd_ cmd) noexcept {
      t_out{"messaging: r_remove_from_group_cmd_"};
      data_.remove_messenger_from_group(err, cmd.password, cmd.id, cmd.group,
                                        cmd.user);
    }

    t_void process(err::t_err, r_is_in_group_cmd_) noexcept {
      t_out{"messaging: r_is_in_group_cmd_"};
      //XXX-later
    }

    t_void process(err::t_err, r_get_groups_cmd_) noexcept {
      t_out{"messaging: r_get_groups_cmd_"};
      //XXX-later
    }

    t_void process(err::t_err err, r_add_monitor_cmd_ cmd) noexcept {
      t_out{"messaging: r_add_monitor_cmd_"};
      data_.add_monitor(err, msgs_, cmd.id, cmd.name, cmd.prio, cmd.user);
    }

    t_void process(err::t_err err, r_remove_monitor_cmd_ cmd) noexcept {
      t_out{"messaging: r_remove_monitor_cmd_"};
      data_.remove_monitor(err, cmd.id, cmd.name, cmd.user);
    }

    t_void process(err::t_err, r_is_monitored_cmd_) noexcept {
      t_out{"messaging: r_is_monitored_cmd_"};
      //XXX-later
    }

    t_void process(err::t_err, r_get_monitored_cmd_) noexcept {
      t_out{"messaging: r_get_monitored_cmd_"};
      //XXX-later
    }

    t_void process(err::t_err, r_clean_death_cmd_) noexcept {
      t_out{"messaging: r_clean_death_cmd_"};
      ev_cmd_ = QUIT_EVENT_LOOP;
    }

///////////////////////////////////////////////////////////////////////////////

    virtual t_void process(t_cmd_err err, t_user,
                           r_command cmd) noexcept override {
      ERR_GUARD(err) {
        switch (cmd.id) {
          case t_update_params_cmd_::cmd_id:
            process(err, static_cast<r_update_params_cmd_>(cmd));
            break;
          case t_fetch_params_cmd_::cmd_id:
            process(err, static_cast<r_fetch_params_cmd_>(cmd));
            break;
          case t_create_messenger_cmd_::cmd_id:
            process(err, static_cast<r_create_messenger_cmd_>(cmd));
            break;
          case t_destroy_messenger_cmd_::cmd_id:
            process(err, static_cast<r_destroy_messenger_cmd_>(cmd));
            break;
          case t_is_messenger_cmd_::cmd_id:
            process(err, static_cast<r_is_messenger_cmd_>(cmd));
            break;
          case t_is_messenger_info_cmd_::cmd_id:
            process(err, static_cast<r_is_messenger_info_cmd_>(cmd));
            break;
          case t_fetch_messengers_cmd_::cmd_id:
            process(err, static_cast<r_fetch_messengers_cmd_>(cmd));
            break;
          case t_create_group_cmd_::cmd_id:
            process(err, static_cast<r_create_group_cmd_>(cmd));
            break;
          case t_destroy_group_cmd_::cmd_id:
            process(err, static_cast<r_destroy_group_cmd_>(cmd));
            break;
          case t_is_group_cmd_::cmd_id:
            process(err, static_cast<r_is_group_cmd_>(cmd));
            break;
          case t_add_messenger_to_group_cmd_::cmd_id:
            process(err, static_cast<r_add_messenger_to_group_cmd_>(cmd));
            break;
          case t_remove_messenger_from_group_cmd_::cmd_id:
            process(err, static_cast<r_remove_messenger_from_group_cmd_>(cmd));
            break;
          case t_is_messenger_in_group_cmd_::cmd_id:
            process(err, static_cast<r_is_messenger_in_group_cmd_>(cmd));
            break;
          case t_fetch_messenger_groups_cmd_::cmd_id:
            process(err, static_cast<r_fetch_messenger_groups_cmd_>(cmd));
            break;
          case t_who_is_cmd_::cmd_id:
            process(err, static_cast<r_who_is_cmd_>(cmd));
            break;
          case t_get_name_cmd_::cmd_id:
            process(err, static_cast<r_get_name_cmd_>(cmd));
            break;
          case t_get_params_cmd_::cmd_id:
            process(err, static_cast<r_get_params_cmd_>(cmd));
            break;
          case t_update_scope_cmd_::cmd_id:
            process(err, static_cast<r_update_scope_cmd_>(cmd));
            break;
          case t_update_alive_period_cmd_::cmd_id:
            process(err, static_cast<r_update_alive_period_cmd_>(cmd));
            break;
          case t_start_timer_cmd_::cmd_id:
            process(err, static_cast<r_start_timer_cmd_>(cmd));
            break;
          case t_stop_timer_cmd_::cmd_id:
            process(err, static_cast<r_stop_timer_cmd_>(cmd));
            break;
          case t_query_timer_cmd_::cmd_id:
            process(err, static_cast<r_query_timer_cmd_>(cmd));
            break;
          case t_add_to_group_cmd_::cmd_id:
            process(err, static_cast<r_add_to_group_cmd_>(cmd));
            break;
          case t_remove_from_group_cmd_::cmd_id:
            process(err, static_cast<r_remove_from_group_cmd_>(cmd));
            break;
          case t_is_in_group_cmd_::cmd_id:
            process(err, static_cast<r_is_in_group_cmd_>(cmd));
            break;
          case t_get_groups_cmd_::cmd_id:
            process(err, static_cast<r_get_groups_cmd_>(cmd));
            break;
          case t_add_monitor_cmd_::cmd_id:
            process(err, static_cast<r_add_monitor_cmd_>(cmd));
            break;
          case t_remove_monitor_cmd_::cmd_id:
            process(err, static_cast<r_remove_monitor_cmd_>(cmd));
            break;
          case t_is_monitored_cmd_::cmd_id:
            process(err, static_cast<r_is_monitored_cmd_>(cmd));
            break;
          case t_get_monitored_cmd_::cmd_id:
            process(err, static_cast<r_get_monitored_cmd_>(cmd));
            break;
          case t_clean_death_cmd_::cmd_id:
            process(err, static_cast<r_clean_death_cmd_>(cmd));
            break;
          default:
            // XXX- 16
            break;
        }
      }
    }

///////////////////////////////////////////////////////////////////////////////

  private:
    class t_cmd_proxy_ : public t_event_logic {
    public:
      t_cmd_proxy_(r_err err, r_event_cmd ev_cmd, r_cmd_processor processor,
                   r_cmd_processor_logic logic)
        : err_(err), ev_cmd_(ev_cmd), processor_(processor), logic_{logic} {
      }

      virtual t_name get_name() const override {
        return {"cmd logic"};
      }

      virtual t_action notify_event(r_event_params) override { //XXX - args?
        ev_cmd_ = CONTINUE;
        processor_.process(err_, logic_);
        return ev_cmd_;
      }

    private:
      r_err                 err_;
      r_event_cmd           ev_cmd_;
      r_cmd_processor       processor_;
      r_cmd_processor_logic logic_;
    };

    class t_que_proxy_ : public t_event_logic {
    public:
      t_que_proxy_(r_err err, r_event_cmd ev_cmd, r_que_processor processor,
                   r_que_processor_logic logic)
        : err_(err), ev_cmd_(ev_cmd), processor_(processor), logic_{logic} {
      }

      virtual t_name get_name() const override {
        return {"queue logic"};
      }

      virtual t_action notify_event(r_event_params)  override { //XXX - args?
        ev_cmd_ = CONTINUE;
        processor_.process_available(err_, logic_);
        return ev_cmd_;
      }

    private:
      r_err                 err_;
      r_event_cmd           ev_cmd_;
      r_que_processor       processor_;
      r_que_processor_logic logic_;
    };

    t_event_cmd     ev_cmd_;
    t_data_         data_;
    t_msgs_         msgs_;
    t_cmd_processor cmd_processor_;
    t_que_processor que_processor_;
    t_dispatcher    dispatcher_;
  };

///////////////////////////////////////////////////////////////////////////////

namespace messenger
{
  inline t_messenger mk_() {
    return {};
  }

  inline t_messenger mk_(R_key id, x_maybe_processor_ processor) {
    return t_messenger{id, std::move(processor)};
  }
}

///////////////////////////////////////////////////////////////////////////////

  class t_messaging_ {
  public:
    t_messaging_(r_err err, R_name name, R_params params)
      : logic_     {err, name, params},
        cmd_client_{logic_.make_cmd_client()},
        que_client_{logic_.make_que_client()},
        thread_    {err, P_cstr{"messaging"}, {&logic_, nullptr}} {
    }

    t_void update(r_err err, R_params params) {
      t_update_params_cmd_ cmd{params};
      cmd_client_.request(err, cmd);
    }

    t_void fetch(r_err err, r_params params) {
      t_fetch_params_cmd_ cmd{params};
      cmd_client_.request(err, cmd);
    }

    t_messenger create_messenger(r_err err, R_messenger_name name,
                                 R_messenger_create_params params) {
      t_create_messenger_cmd_ cmd{name, params};
      cmd_client_.request(err, cmd);
      return messenger::mk_(cmd.id, std::move(cmd.processor));
    }

    t_void destroy_messenger(R_messenger_key id) {
      t_destroy_messenger_cmd_ cmd{id};
      t_err err; //XXX
      cmd_client_.request(err, cmd);
    }

    t_bool is_messenger(r_err err, R_messenger_name name,
                        r_messenger_params params) {
      t_is_messenger_cmd_ cmd{name, params};
      cmd_client_.request(err, cmd);
      return cmd.found;
    }

    t_bool is_messenger(r_err err, R_messenger_name name,
                        r_messenger_info info, t_bool clearstats) {
      t_is_messenger_info_cmd_ cmd{name, info, clearstats};
      cmd_client_.request(err, cmd);
      return cmd.found;
    }

    t_void fetch_messengers(r_err err, r_messenger_infos infos,
                            t_bool clearstats) {
      t_fetch_messengers_cmd_ cmd{infos, clearstats};
      cmd_client_.request(err, cmd);
    }

    t_void create_group(r_err err, R_password password, R_messenger_name group,
                        t_messenger_scope scope) {
      t_create_group_cmd_ cmd{password, group, scope};
      cmd_client_.request(err, cmd);
    }

    t_void destroy_group(r_err err, R_password password,
                         R_messenger_name group) {
      t_destroy_group_cmd_ cmd{password, group};
      cmd_client_.request(err, cmd);
    }

    t_bool is_group(r_err err, R_messenger_name group, r_messenger_scope scope,
                    p_messenger_name_list name_list) {
      t_is_group_cmd_ cmd{group, scope, name_list};
      cmd_client_.request(err, cmd);
      return cmd.found;
    }

    t_void add_messenger_to_group(r_err err, R_messenger_password password,
                                  R_messenger_name name,
                                  R_messenger_name group,
                                  t_messenger_prio prio,
                                  t_messenger_user user) {
      t_add_messenger_to_group_cmd_ cmd{password, name, group, prio, user};
      cmd_client_.request(err, cmd);
    }

    t_void remove_messenger_from_group(r_err err, R_messenger_password password,
                                       R_messenger_name name,
                                       R_messenger_name group,
                                       p_messenger_user user) {
      t_remove_messenger_from_group_cmd_ cmd{password, name, group, user};
      cmd_client_.request(err, cmd);
    }

    t_bool is_messenger_in_group(r_err err, R_messenger_name name,
                                 R_messenger_name group,
                                 p_messenger_user user) {
      t_is_messenger_in_group_cmd_ cmd{name, group, user};
      cmd_client_.request(err, cmd);
      return cmd.found;
    }

    t_void fetch_messenger_groups(r_err err, R_messenger_name name,
                                  r_messenger_group_list group_list) {
      t_fetch_messenger_groups_cmd_ cmd{name, group_list};
      cmd_client_.request(err, cmd);
    }

    t_void who_is(r_err err, R_messenger_key key, r_messenger_name name,
                             p_bool group, p_bool local) {
      t_who_is_cmd_ cmd{key, name, group, local};
      cmd_client_.request(err, cmd);
    }

    t_void post_message(r_err err, R_messenger_key key, x_message msg) {
      t_que_chain chain = que_client_.waitable_acquire(err);
      if (!err) {
        // XXX-now
        chain.head->ref().any.emplace<t_message>({0L}, std::move(msg));
        que_client_.insert(err, chain);
      }
    }

    t_messenger_name get_name(r_err err, R_messenger_key id) {
      t_get_name_cmd_ cmd{id};
      cmd_client_.request(err, cmd);
      return cmd.name;
    }

    t_void get_params(r_err err, R_messenger_key id,
                      r_messenger_params params) {
      t_get_params_cmd_ cmd{id, params};
      cmd_client_.request(err, cmd);
    }

    t_void update_scope(r_err err, R_messenger_key id,
                        t_messenger_scope scope) {
      t_update_scope_cmd_ cmd{id, scope};
      cmd_client_.request(err, cmd);
    }

    t_void update_alive_period(r_err err, R_messenger_key id,
                               t_multiple_of_100ms multiple) {
      t_update_alive_period_cmd_ cmd{id, multiple};
      cmd_client_.request(err, cmd);
    }

    t_void post_message(r_err err, R_messenger_key id, R_messenger_key key,
                        x_message msg) {
      t_que_chain chain = que_client_.waitable_acquire(err);
      if (!err) {
        // XXX-now
        chain.head->ref().any.emplace<t_message>({0L}, std::move(msg));
        que_client_.insert(err, chain);
      }
    }

    t_void start_timer(r_err err, R_messenger_key id,
                       R_messenger_timer_params params) {
      t_start_timer_cmd_ cmd{id, params};
      cmd_client_.request(err, cmd);
    }

    t_void stop_timer(r_err err, R_messenger_key id) {
      t_stop_timer_cmd_ cmd{id};
      cmd_client_.request(err, cmd);
    }

    t_void query_timer(r_err err, R_messenger_key id,
                       r_messenger_timer_params params) {
      t_query_timer_cmd_ cmd{id, params};
      cmd_client_.request(err, cmd);
    }

    t_void add_to_group(r_err err, R_messenger_key id,
                        R_messenger_password password,
                        R_messenger_name group,
                        t_messenger_prio prio,
                        t_messenger_user user) {
      t_add_to_group_cmd_ cmd{id, password, group, prio, user};
      cmd_client_.request(err, cmd);
    }

    t_void remove_from_group(r_err err, R_messenger_key id,
                             R_messenger_password password,
                             R_messenger_name group,
                             p_messenger_user user) {
      t_remove_from_group_cmd_ cmd{id, password, group, user};
      cmd_client_.request(err, cmd);
    }

    t_bool is_in_group(r_err err, R_messenger_key id,
                       R_messenger_name group,
                       p_messenger_user user) {
      t_is_in_group_cmd_ cmd{id, group, user};
      cmd_client_.request(err, cmd);
      return cmd.found;
    }

    t_void get_groups(r_err err, R_messenger_key id,
                      r_messenger_group_list group_list) {
      t_get_groups_cmd_ cmd{id, group_list};
      cmd_client_.request(err, cmd);
    }

    t_void add_monitor(r_err err, R_messenger_key id,
                       R_messenger_name name, t_messenger_prio prio,
                       t_messenger_user user) {
      t_add_monitor_cmd_ cmd{id, name, prio, user};
      cmd_client_.request(err, cmd);
    }

    t_void remove_monitor(r_err err, R_messenger_key id,
                          R_messenger_name name,
                          p_messenger_user user) {
      t_remove_monitor_cmd_ cmd{id, name, user};
      cmd_client_.request(err, cmd);
    }

    t_messenger_key is_monitored(r_err err, R_messenger_key id,
                                 R_messenger_name name,
                                 p_messenger_user user) {
      t_is_monitored_cmd_ cmd{id, name, user};
      cmd_client_.request(err, cmd);
      return cmd.key;
    }

    t_void get_monitored(r_err err, R_messenger_key id,
                         r_messenger_monitor_list monitor_list) {
      t_get_monitored_cmd_ cmd{id, monitor_list};
      cmd_client_.request(err, cmd);
    }

    t_void clean_death() {
      t_clean_death_cmd_ cmd;
      t_err err; //XXX
      cmd_client_.request(err, cmd);
    }

  private:
    t_logic_     logic_;
    t_cmd_client cmd_client_;
    t_que_client que_client_;
    t_thread     thread_;
  };
  using p_messaging_ = t_prefix<t_messaging_>::p_;

///////////////////////////////////////////////////////////////////////////////

  p_messaging_ mr_ = nullptr; // atomic or shared_ptr

///////////////////////////////////////////////////////////////////////////////

namespace messenger
{
  t_scope_name to_name(t_scope scope) {
    const char* tbl_[] = { "scope_off",
                           "scope_process",
                           "scope_node",
                           "scope_system" };
    return P_cstr{tbl_[scope]};
  }

  t_messenger::t_messenger(R_key id, x_maybe_processor_ processor)
    : id_(id), processor_{std::move(processor)} {
  }

  t_messenger::t_messenger(x_messenger messenger)
    : id_{messenger.id_}, processor_{std::move(messenger.processor_)} {
    set(messenger.id_) = 0;
  }

  t_messenger::~t_messenger() {
    if (mr_)
      mr_->destroy_messenger(id_);
  }

  t_messenger::operator t_validity() const {
    return processor_;
  }

  t_fd t_messenger::get_fd() const {
    if (processor_ == VALID)
      return get(processor_).get_fd();
    return BAD_FD;
  }

  t_key t_messenger::get_key() const {
    return id_;
  }

  t_name t_messenger::get_name(t_err err) const {
    ERR_GUARD(err) {
      if (mr_)
        return mr_->get_name(err, id_);
      err = err::E_XXX;
    }
    return {};
  }

  t_void t_messenger::get_params(t_err err, r_params params) const {
    ERR_GUARD(err) {
      if (mr_)
        mr_->get_params(err, id_, params);
      else
        err = err::E_XXX;
    }
  }

  t_void t_messenger::update_scope(t_err err, t_scope scope) {
    ERR_GUARD(err) {
      if (mr_)
        mr_->update_scope(err, id_, scope);
      else
        err = err::E_XXX;
    }
  }

  t_void t_messenger::update_alive_period(t_err err,
                                          t_multiple_of_100ms multiple) {
    ERR_GUARD(err) {
      if (mr_)
        mr_->update_alive_period(err, id_, multiple);
      else
        err = err::E_XXX;
    }
  }

  t_void t_messenger::post_message(t_err err, R_key key, x_message message) {
    ERR_GUARD(err) {
      if (mr_)
        mr_->post_message(err, id_, key, std::move(message));
      else
        err = err::E_XXX;
    }
  }

  struct t_get_messages_ : public t_processor_::t_logic {
    r_messages messages;

    inline t_get_messages_(r_messages _messages) : messages{_messages} {
    }

    t_void async_process(t_chain chain) noexcept {
      for (auto item = chain.head; item; item = item->next()) {
        //messages.push_back(std::move(ref.msg));
      }
    }
  };

  t_void t_messenger::wait_message(t_err err, r_messages messages) {
    ERR_GUARD(err) {
      if (mr_ && processor_ == VALID) {
        t_get_messages_ get_messages{messages};
        set(processor_).process(err, get_messages);
      } else
        err = err::E_XXX;
    }
  }

  t_void t_messenger::start_timer(t_err err, R_timer_params params) {
    ERR_GUARD(err) {
      if (mr_)
        mr_->start_timer(err, id_, params);
      else
        err = err::E_XXX;
    }
  }

  t_void t_messenger::stop_timer(t_err err) {
    ERR_GUARD(err) {
      if (mr_)
        mr_->stop_timer(err, id_);
      else
        err = err::E_XXX;
    }
  }

  t_void t_messenger::query_timer(t_err err, r_timer_params params) const {
    ERR_GUARD(err) {
      if (mr_)
        mr_->query_timer(err, id_, params);
      else
        err = err::E_XXX;
    }
  }

  t_void t_messenger::add_to_group(t_err err, R_password password,
                                   R_name name, t_prio prio, t_user user) {
    ERR_GUARD(err) {
      if (mr_)
        mr_->add_to_group(err, id_, password, name, prio, user);
      else
        err = err::E_XXX;
    }
  }

  t_void t_messenger::remove_from_group(t_err err, R_password password,
                                        R_name name, p_user user) {
    ERR_GUARD(err) {
      if (mr_)
        mr_->remove_from_group(err, id_, password, name, user);
      else
        err = err::E_XXX;
    }
  }

  t_bool t_messenger::is_in_group(t_err err, R_name name, p_user user) const {
    ERR_GUARD(err) {
      if (mr_)
        return mr_->is_in_group(err, id_, name, user);
      err = err::E_XXX;
    }
    return false;
  }

  t_void t_messenger::get_groups(t_err err, r_group_list group_list) const {
    ERR_GUARD(err) {
      if (mr_)
        mr_->get_groups(err, id_, group_list);
      else
        err = err::E_XXX;
    }
  }

  t_void t_messenger::add_monitor(t_err err, R_name name, t_prio prio,
                                  t_user user) {
    ERR_GUARD(err) {
      if (mr_)
        mr_->add_monitor(err, id_, name, prio, user);
      else
        err = err::E_XXX;
    }
  }

  t_void t_messenger::remove_monitor(t_err err, R_name name, p_user user) {
    ERR_GUARD(err) {
      if (mr_)
        mr_->remove_monitor(err, id_, name, user);
      else
        err = err::E_XXX;
    }
  }

  t_key t_messenger::is_monitored(t_err err, R_name name, p_user user) const {
    ERR_GUARD(err) {
      if (mr_)
        return mr_->is_monitored(err, id_, name, user);
      err = err::E_XXX;
    }
    return t_key{0};
  }

  t_void t_messenger::get_monitored(t_err err,
                                    r_monitor_list monitor_list) const {
    ERR_GUARD(err) {
      if (mr_)
        mr_->get_monitored(err, id_, monitor_list);
      else
        err = err::E_XXX;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////

  t_scope_name to_name(t_scope scope) {
    const char* tbl_[] = { "scope_off",
                           "scope_local",
                           "scope_slave",
                           "scope_master" };
    return P_cstr{tbl_[scope]};
  }

  t_messenger_create_params default_messenger_create_params() {
    return {messenger::SCOPE_PROCESS,
            t_multiple_of_100ms{2},
            R_messenger_timer_params{}};
  }

  t_void start(t_err err, R_name name, P_params params) {
    ERR_GUARD(err) {
      static t_mutex_lock lock(err);
      <% auto scope = lock.make_locked_scope(err);
        if (!err) {
          if (mr_ == nullptr) {
            mr_ = new t_messaging_{err, name, params ? *params : t_params{}};
            if (err)
              delete named::reset(mr_);
          }
        }
      %>
    }
  }

  t_void destroy() {
    if (mr_) {
      mr_->clean_death();
      delete named::reset(mr_);
    }
  }

  t_bool is_running() {
    if (mr_)
      return true;
    return false;
  }

  t_void update(t_err err, R_params params) {
    ERR_GUARD(err) {
      if (mr_)
        mr_->update(err, params);
      else
        err = err::E_XXX;
    }
  }

  t_void fetch(t_err err, r_params params) {
    ERR_GUARD(err) {
      if (mr_)
        mr_->fetch(err, params);
      else
        err = err::E_XXX;
    }
  }

  t_messenger create_messenger(t_err err, R_messenger_name name,
                               R_messenger_create_params params) {
    ERR_GUARD(err) {
      if (mr_)
        return mr_->create_messenger(err, name, params);
      err = err::E_XXX;
    }
    return messenger::mk_();
  }

  t_bool is_messenger(t_err err, R_messenger_name name,
                      r_messenger_params params) {
    ERR_GUARD(err) {
      if (mr_)
        return mr_->is_messenger(err, name, params);
      err = err::E_XXX;
    }
    return false;
  }

  t_bool is_messenger(t_err err, R_messenger_name name,
                      r_messenger_info info, t_bool clearstats) {
    ERR_GUARD(err) {
      if (mr_)
        return mr_->is_messenger(err, name, info, clearstats);
      err = err::E_XXX;
    }
    return false;
  }

  t_void fetch_messengers(t_err err, r_messenger_infos infos,
                          t_bool clearstats) {
    ERR_GUARD(err) {
      if (mr_)
        mr_->fetch_messengers(err, infos, clearstats);
      else
        err = err::E_XXX;
    }
  }

  t_void create_group(t_err err, R_password password, R_messenger_name name,
                      t_messenger_scope scope) {
    ERR_GUARD(err) {
      if (mr_)
        mr_->create_group(err, password, name, scope);
      else
        err = err::E_XXX;
    }
  }

  t_void destroy_group(t_err err, R_password password, R_messenger_name name) {
    ERR_GUARD(err) {
      if (mr_)
        mr_->destroy_group(err, password, name);
      else
        err = err::E_XXX;
    }
  }

  t_bool is_group(t_err err, R_messenger_name name, r_messenger_scope scope,
                  p_messenger_name_list name_list) {
    ERR_GUARD(err) {
      if (mr_)
        return mr_->is_group(err, name, scope, name_list);
      err = err::E_XXX;
    }
    return false;
  }

  t_void add_messenger_to_group(t_err err, R_messenger_password password,
                                R_messenger_name name,
                                R_messenger_name group,
                                t_messenger_prio prio,
                                t_messenger_user user) {
    ERR_GUARD(err) {
      if (mr_)
        mr_->add_messenger_to_group(err, password, name, group, prio, user);
      else
        err = err::E_XXX;
    }
  }

  t_void remove_messenger_from_group(t_err err, R_messenger_password password,
                                     R_messenger_name name,
                                     R_messenger_name group,
                                     p_messenger_user user) {
    ERR_GUARD(err) {
      if (mr_)
        mr_->remove_messenger_from_group(err, password, name, group, user);
      else
        err = err::E_XXX;
    }
  }

  t_bool is_messenger_in_group(t_err err, R_messenger_name name,
                               R_messenger_name group, p_messenger_user user) {
    ERR_GUARD(err) {
      if (mr_)
        return mr_->is_messenger_in_group(err, name, group, user);
      err = err::E_XXX;
    }
    return false;
  }

  t_void fetch_messenger_groups(t_err err, R_messenger_name name,
                                r_messenger_group_list group_list) {
    ERR_GUARD(err) {
      if (mr_)
        mr_->fetch_messenger_groups(err, name, group_list);
      else
        err = err::E_XXX;
    }
  }

  t_void who_is(t_err err, R_messenger_key key, r_messenger_name name,
                p_bool group, p_bool local) {
    ERR_GUARD(err) {
      if (mr_)
        mr_->who_is(err, key, name, group, local);
      else
        err = err::E_XXX;
    }
  }

  t_void post_message(t_err err, R_messenger_key key, x_message msg) {
    ERR_GUARD(err) {
      if (mr_)
        mr_->post_message(err, key, std::move(msg));
      else
        err = err::E_XXX;
    }
  }

///////////////////////////////////////////////////////////////////////////////

  struct automatic_start_ {
    automatic_start_() {
      t_err err;
      start(err, P_cstr{"local"});
      if (err) {
        // what to do
      }
    }

    ~automatic_start_() {
      destroy();
    }
  };

  automatic_start_ start_;

///////////////////////////////////////////////////////////////////////////////
}
}
