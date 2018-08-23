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
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/eventfd.h>
#include <sys/timerfd.h>
#include <sys/epoll.h>

#include "framework_library_types.h"
#include "framework_library_thread.h"
#include "framework_library_time.h"
#include "framework_library_types_nfreelist.h"
#include "framework_service_trace.h"
#include "framework_service_messaging.h"

namespace dainty
{
namespace messaging
{
namespace messenger
{
///////////////////////////////////////////////////////////////////////////////

  struct member_t {
    password_t       password_;
    messenger_prio_t prio_;
    messenger_user_t user_;
    member_t() : prio_(0), user_(0) { }
    member_t(messenger_prio_t prio)
      : prio_(prio), user_(0) { }
    member_t(const password_t& password,
                       messenger_prio_t prio,
                       messenger_user_t user)
      : password_(password), prio_(prio), user_(user) { }
  };
  typedef std::map<messenger_name_t, messenger_member_t>
    messenger_memberlist_t;

  struct keymember_t {
    key_t    key_;
    member_t params_;
    keymember_t() : key_(0) { }
    keymember_t(const messenger_key_t& key) : key_(key) { }
    keymember_t(const messenger_key_t& key,
                          const messenger_member_t& params)
    : key_(key), params_(params) { }
  };
  typedef std::map<messenger_name_t, keymember_t>
    messenger_memberkeylist_t;

}
      using namespace library::time;
      using namespace library::thread;
      using namespace library::types;
      using namespace library;

  typedef fd_queue_t::item_t<bytebuf_t> queueitem_t;

////////////////////////////////////////////////////////////////////////////////

  namespace {
    const message_domain_t service_domain(0);
    const messenger_key_t  empty_key(0);
  }

////////////////////////////////////////////////////////////////////////////////

  trace::source_t& tr_src() {
    static trace::sourceowner_t src(trace::create_source("messaging"));
    if (!src)
      src = trace::create_source("messaging");
    return *src;
  }

////////////////////////////////////////////////////////////////////////////////

  inline bool_t operator==(const messenger_key_t& lh,
                           const messenger_key_t& rh) {
    return get(lh) == get(rh);
  }

  inline bool_t operator!=(const messenger_key_t& lh,
                           const messenger_key_t& rh) {
    return get(lh) != get(rh);
  }

  inline bool_t operator<(const messenger_key_t& lh,
                          const messenger_key_t& rh) {
    return get(lh) < get(rh);
  }

////////////////////////////////////////////////////////////////////////////////

  inline bool_t operator<(messenger_prio_t lh, messenger_prio_t rh) {
    return get(lh) < get(rh);
  }

////////////////////////////////////////////////////////////////////////////////

  inline messenger_key_t make_key(ushort_t seq, ushort_t id) {
    return messenger_key_t(((messenger_key_t::native_t)id  << 1) |
                           ((messenger_key_t::native_t)seq << 16));
  }

  inline messenger_key_t make_group_key(ushort_t seq, ushort_t id) {
    return messenger_key_t(get(make_key(seq, id)) | 1);
  }

  inline messenger_key_t make_remote_key(ushort_t remote_seq,
                                         ushort_t remote_id,
                                         const messenger_key_t& key) {
    return messenger_key_t(((messenger_key_t::native_t)remote_id  << 32) |
                           ((messenger_key_t::native_t)remote_seq << 48) |
                           get(key));
  }

  inline bool is_remote(const messenger_key_t& key) {
    return get(key) >> 32;
  }

  inline bool is_local(const messenger_key_t& key) {
    return !is_remote(key);
  }

  inline bool is_group(const messenger_key_t& key) {
    return get(key) & 1;
  }

  inline bool_t get_key_local_params(const messenger_key_t& key,
                                     ushort_t& seq, ushort_t& id) {
    messenger_key_t::native_t value = get(key);
    seq = 0xff & (value >> 16);
    id =  0x8f & (value >> 1);
    return seq && !(value >> 32);
  }

  inline bool_t get_key_params(const messenger_key_t& key,
                               bool_t& local, bool_t& group,
                               ushort_t& seq, ushort_t& id) {
    local = is_local(key);
    group = is_group(key);
    if (local)
      return get_key_local_params(key, seq, id);
    seq = 0xff & (get(key) >> 32);
    id =  0xff & (get(key) >> 48);
    return seq;
  }

////////////////////////////////////////////////////////////////////////////////

  messenger_visibility_string_t
    to_string(messenger_visibility_t visibility) {
    messenger_visibility_string_t tmp;
    switch (visibility) {
      case messenger_visibility_off:     tmp = "off";     break;
      case messenger_visibility_process: tmp = "process"; break;
      case messenger_visibility_node:    tmp = "node";    break;
      case messenger_visibility_system:  tmp = "system";  break;
    }
    return tmp;
  }

////////////////////////////////////////////////////////////////////////////////

  struct message_header_data_t {
    uint_t                    len_;
    messenger_key_t::native_t dst_;
    messenger_key_t::native_t src_;
    uchar_t                   domain_;
    uchar_t                   user_;
    uchar_t                   version_;
    ushort_t                  cnt_;
    uint_t                    seq_;
   };

  bool_t write_message_header(message_t&          message,
                              const message_id_t& id,
                              uint_t              len,
                              messenger_key_t     dst,
                              messenger_key_t     src,
                              ushort_t            cnt,
                              uint_t              seq) {
    if (message.capacity() >= sizeof(message_header_data_t)) {
      message_header_data_t* hdr = (message_header_data_t*)message.data();
      hdr->len_      = len;
      hdr->dst_      = get(dst);
      hdr->src_      = get(src);
      hdr->domain_   = get(id.domain_);
      hdr->user_     = get(id.user_);
      hdr->version_  = get(id.version_);
      hdr->cnt_      = cnt;
      hdr->seq_      = seq;
      return true;
    }
    return false;
  }

  bool_t write_message_header(message_t&          message,
                              const message_id_t& id,
                              uint_t              len,
                              ushort_t            cnt) {
    if (message.capacity() >= sizeof(message_header_data_t)) {
      message_header_data_t* hdr = (message_header_data_t*)message.data();
      hdr->len_     = len;
      hdr->domain_  = get(id.domain_);
      hdr->user_    = get(id.user_);
      hdr->version_ = get(id.version_);
      hdr->cnt_     = cnt;
      return true;
    }
    return false;
  }

  bool_t write_message_header(message_t&          message,
                              const message_id_t& id,
                              messenger_key_t     dst,
                              messenger_key_t     src,
                              uint_t              seq) {
    if (message.capacity() >= sizeof(message_header_data_t)) {
      message_header_data_t* hdr = (message_header_data_t*)message.data();
      hdr->dst_     = get(dst);
      hdr->src_     = get(src);
      hdr->domain_  = get(id.domain_);
      hdr->user_    = get(id.user_);
      hdr->version_ = get(id.version_);
      hdr->seq_     = seq;
      return true;
    }
    return false;
  }

  bool_t write_message_header(message_t&      message,
                              messenger_key_t dst,
                              messenger_key_t src) {
    if (message.capacity() >= sizeof(message_header_data_t)) {
      message_header_data_t* hdr = (message_header_data_t*)message.data();
      hdr->dst_     = get(dst);
      hdr->src_     = get(src);
      return true;
    }
    return false;
  }

  bool_t read_message_header(const message_t& message,
                             message_id_t&    id,
                             uint_t&          len,
                             messenger_key_t& dst,
                             messenger_key_t& src,
                             ushort_t&        cnt,
                             uint_t&          seq) {
    if (message.capacity() >= sizeof(message_header_data_t)) {
      const message_header_data_t*
        hdr((const message_header_data_t*)message.const_data());
      len              = hdr->len_;
      set(dst)         = hdr->dst_;
      set(src)         = hdr->src_;
      set(id.domain_)  = hdr->domain_;
      set(id.user_)    = hdr->user_;
      set(id.version_) = hdr->version_;
      cnt              = hdr->cnt_;
      seq              = hdr->seq_;
      return true;
    }
    return false;
  }

  messenger_key_t read_dst(const message_t& message) {
    if (message && message.capacity() >= sizeof(message_header_data_t)) {
      // hack
      const message_header_data_t*
        hdr((const message_header_data_t*)message.const_data());
      // hack
      return messenger_key_t(hdr->dst_);
    }
    return empty_key;
  }

  messenger_key_t read_src(const message_t& message) {
    if (message && message.capacity() >= sizeof(message_header_data_t)) {
      // hack
      const message_header_data_t*
        hdr((const message_header_data_t*)message.const_data());
      // hack
      return messenger_key_t(hdr->src_);
    }
    return empty_key;
  }

  uint_t read_len(const message_t& message) {
    if (message && message.capacity() >= sizeof(message_header_data_t)) {
      // hack
      const message_header_data_t*
        hdr((const message_header_data_t*)message.const_data());
      // hack
      return hdr->len_;
    }
    return 0;
  }

  message_id_t read_id(const message_t& message) {
    if (message &&
        message.capacity() >= (sizeof(message_header_data_t))) {
      // hack
      const message_header_data_t* data =
       (const message_header_data_t*)message.const_data();
      // hack
      return message_id_t(message_domain_t (data->domain_),
                          message_user_t   (data->user_),
                          message_version_t(data->version_));
    }
    return message_id_t(message_domain_t(0), message_user_t(0),
                        message_version_t(0));
  }

////////////////////////////////////////////////////////////////////////////////

  message_t::message_t() {
  }

  message_t::message_t(uint_t n) : buf_(n + sizeof(message_header_data_t)) {
  }

  message_t::message_t(content_t content) : buf_(content) {
  }

  message_t& message_t::operator=(content_t content) {
    buf_ = content;
    return *this;
  }

  bool_t message_t::set(const id_t& id, uint_t length, ushort_t cnt) {
    return write_message_header(*this, id, length, cnt);
  }

  bool_t message_t::get(id_t& id, uint_t& length, key_t& dst, key_t& src,
                        ushort_t& cnt) {
    uint_t seq = 0;
    return read_message_header(*this, id, length, dst, src, cnt, seq);
  }

  bool_t message_t::is_empty() const {
    return !(bool)buf_;
  }

  uint_t message_t::capacity() const {
    return buf_.capacity();
  }

  message_t::operator bool_t () const {
    return (bool)buf_;
  }

  byte_t* message_t::data() {
    return buf_.data();
  }

  const byte_t* message_t::data() const {
    return buf_.data();
  }

  const byte_t* message_t::const_data() const {
    return buf_.const_data();
  }

  message_t::content_t message_t::release() {
    return buf_.release();
  }

  message_t::content_t message_t::clone() const {
    return buf_.clone();
  }

  message_t::content_t message_t::clone(uint_t n) const {
    return buf_.clone(n);
  }

////////////////////////////////////////////////////////////////////////////////

  struct message_notify_data_t {
    messenger_state_t          state_;
    messenger_key_t::native_t  key_;
    messenger_prio_t::native_t prio_;
    uint_t                     user_;
    messenger_name_t           name_; // another hack
  };

  bool_t write_notify_message(message_t&              message,
                              const messenger_key_t&  dst,
                              messenger_state_t       state,
                              const messenger_name_t& name,
                              const messenger_key_t&  key,
                              messenger_prio_t        prio,
                              messenger_user_t        user) {
    const uint_t max_size = sizeof(message_header_data_t) +
                            sizeof(message_notify_data_t);
    if (message.capacity() >= max_size) {
      write_message_header(message,
                           message_id_t(service_domain,
                                        message_user_t(message_type_notify),
                                        message_version_t(1)),
                           sizeof(message_notify_data_t),
                           dst,
                           empty_key,
                           1, 0);

      message_notify_data_t* data =
       (message_notify_data_t*)(message.data() +
                                sizeof(message_header_data_t));
      data->state_ = state;
      data->key_   = get(key);
      data->prio_  = get(prio);
      data->user_  = user.uint_;
      data->name_  = name;
      return true;
    }
    return false;
  }

  bool_t read_notify_message(const message_t&   message,
                             messenger_state_t& state,
                             messenger_name_t&  name,
                             messenger_key_t&   key,
                             messenger_prio_t&  prio,
                             messenger_user_t&  user) {
    if (message &&
        message.capacity() >= (sizeof(message_header_data_t) +
                               sizeof(message_notify_data_t))) {
      // hack
      const message_notify_data_t* data =
       (const message_notify_data_t*)(message.const_data() +
                                      sizeof(message_header_data_t));
      state      = data->state_;
      set(key)   = data->key_;
      set(prio)  = data->prio_;
      user.uint_ = data->user_;
      name       = data->name_;
      // hack
      return true;
    }
    return false;
  }

  notify_message_t::notify_message_t()
    : message_t(sizeof(message_notify_data_t)) {
  }

  bool_t notify_message_t::get(messenger_state_t& state,
                               messenger_name_t&  name,
                               messenger_key_t&   key,
                               messenger_prio_t&  prio,
                               messenger_user_t&  user) {
    return read_notify_message(*this, state, name, key, prio, user);
  }

////////////////////////////////////////////////////////////////////////////////

  struct message_timeout_data_t {
    bool_t                        periodic_;
    messenger_key_t::native_t     key_;
    multiple_of_100ms_t::native_t factor_;
    messenger_prio_t::native_t    prio_;
    uint_t                        user_;
  };

  bool_t write_timeout_message(message_t&             message,
                               bool_t                 periodic,
                               multiple_of_100ms_t    factor,
                               const messenger_key_t& key,
                               messenger_prio_t       prio,
                               messenger_user_t       user) {
    const uint_t max_size = sizeof(message_header_data_t) +
                            sizeof(message_timeout_data_t);
    if (message.capacity() >= max_size) {
      write_message_header(message,
                           message_id_t(service_domain,
                                        message_user_t(
                                          message_type_timeout),
                                        message_version_t(1)),
                           sizeof(message_timeout_data_t),
                           key,
                           empty_key,
                           1, 0);

      message_timeout_data_t* data =
       (message_timeout_data_t*)(message.data() +
                                 sizeof(message_header_data_t));
      data->periodic_  = periodic;
      data->key_       = get(key);
      data->prio_      = get(prio);
      data->user_      = user.uint_;
      data->factor_    = get(factor);
      return true;
    }
    return false;
  }

  bool_t read_timeout_message(const message_t&     message,
                              bool_t&              periodic,
                              multiple_of_100ms_t& factor,
                              messenger_key_t&     key,
                              messenger_prio_t&    prio,
                              messenger_user_t&    user) {
    if (message &&
        message.capacity() >= (sizeof(message_header_data_t) +
                               sizeof(message_timeout_data_t))) {
      // hack
      const message_timeout_data_t* data =
       (const message_timeout_data_t*)(message.const_data() +
                                       sizeof(message_header_data_t));
      periodic    = data->periodic_;
      set(key)    = data->key_;
      set(prio)   = data->prio_;
      user.uint_  = data->user_;
      set(factor) = data->factor_;
      return true;
    }
    return false;
  }

  timeout_message_t::timeout_message_t()
    : message_t(sizeof(message_timeout_data_t)) {
  }

  bool_t timeout_message_t::get(bool_t&              periodic,
                                multiple_of_100ms_t& factor,
                                messenger_key_t&     key,
                                messenger_prio_t&    prio,
                                messenger_user_t&    user) {
    return read_timeout_message(*this, periodic, factor, key, prio, user);
  }

////////////////////////////////////////////////////////////////////////////////

  bool_t write_failed_message(message_t&                 message,
                              failed_message_t::reason_t reason,
                              const messenger_key_t&     key,
                              const message_t&           send_message) {
    const uint_t max_size = sizeof(message_header_data_t) +
                                   send_message.capacity();
    if (message.capacity() >= max_size) {
      write_message_header(message,
                           message_id_t(service_domain,
                                        message_user_t(
                                          message_type_failed),
                                        message_version_t(1)),
                           send_message.capacity(),
                           key,
                           key,
                           1, 0);
      memcpy(message.data() + sizeof(message_header_data_t),
             send_message.data(), send_message.capacity());
      return true;
    }
    return false;
  }

  bool_t read_failed_message(const message_t& message,
                             failed_message_t::reason_t& reason,
                             message_t& send_message) {
    if (message && message.capacity() >= sizeof(message_header_data_t)) {
      // hack
      const message_header_data_t* data =
       (const message_header_data_t*)(message.const_data());
      reason      = (failed_message_t::reason_t)data->user_;
      uint_t len  = data->len_;

      if (!send_message || len > send_message.capacity()) {
        message_t tmp(len);
        send_message = tmp.release();
      }
      if (send_message) {
        memcpy(send_message.data(),
               message.const_data() + sizeof(message_header_data_t),
               len);
        return true;
      }
    }
    return false;
  }

  bool_t failed_message_t::get(reason_t& reason,
                                      message_t& send_message) {
    return read_failed_message(*this, reason, send_message);
  }

////////////////////////////////////////////////////////////////////////////////

  struct message_alive_data_t {
    int i_; // not sute yet
  };

  bool_t write_alive_message(message_t& message, messenger_key_t& key) {
    if (message.capacity() >= (sizeof(message_header_data_t) +
                               sizeof(message_alive_data_t))) {
      write_message_header(message,
                           message_id_t(service_domain,
                                        message_user_t(message_type_alive),
                                        message_version_t(1)),
                           sizeof(message_alive_data_t),
                           key, empty_key, 1, 0);
      return true;
    }
    return false;
  }

  bool_t read_alive_message(const message_t& message) {
    if (message.capacity() >= (sizeof(message_header_data_t) +
                               sizeof(message_alive_data_t))) {
      return true;
    }
    return false;
  }

  alive_message_t::alive_message_t()
    : message_t(sizeof(message_alive_data_t)) {
  }

  bool_t alive_message_t::get() {
    return read_alive_message(*this);
  }

////////////////////////////////////////////////////////////////////////////////

  namespace {
    struct prio_message_t {
      messenger_key_t      key_;
      messenger_user_t     user_;
      bytebuf_t::content_t content_;
      prio_message_t() : key_(0), user_(0) { }
      prio_message_t(const messenger_key_t& key) : key_(key), user_(0) { }
      prio_message_t(const messenger_key_t& key,
                     messenger_user_t user) : key_(key), user_(user) { }
    };
    typedef std::multimap<messenger_prio_t, prio_message_t> prio_messages_t;

     enum { FD_CMD_EVENT = 1,
            FD_TIPCTOPO_EVENT,
            FD_TIMEOUT_EVENT,
            FD_TIPCLOCAL_EVENT,
            FD_TIPCREMOTE_EVENT,
            FD_USER_EVENT };

    class command_engine_t {
    public:
      command_engine_t() : errorid_(0),
                           done_   (false),
                           fd_     (::eventfd(0, 0)) {
        if (get(fd_) == -1        ||
            !mutex_.is_valid()    ||
            !seqmutex_.is_valid() ||
            !cond_.is_valid())
          errorid_ = -1;
      }

      ~command_engine_t() {
        ::close(get(fd_));
      }

      messenger_name_t
        call_messenger_get_name(const messenger_key_t& key) {
        <% mutexguard_t guard(seqmutex_);
          <% mutexguard_t guard(mutex_);
            call_messenger_get_name_.key_ = &key;
            if (request_processing(CALLID_MESSENGER_GET_NAME))
              return call_messenger_get_name_.name_;
          %>
        %>
        return messenger_name_t();
      }

      bool_t call_messenger_get_params(const messenger_key_t& key,
                                       messenger_params_t& params) {
        <% mutexguard_t guard(seqmutex_);
          <% mutexguard_t guard(mutex_);
            call_messenger_get_params_.key_    = &key;
            call_messenger_get_params_.params_ = &params;
            if (request_processing(CALLID_MESSENGER_GET_PARAMS))
              return call_messenger_get_params_.result_;
          %>
        %>
        return false;
      }

      bool_t call_messenger_update_visibility(const messenger_key_t& key,
                                              messenger_visibility_t
                                                visibility) {
        <% mutexguard_t guard(seqmutex_);
          <% mutexguard_t guard(mutex_);
            call_messenger_update_visibility_.key_        = &key;
            call_messenger_update_visibility_.visibility_ = visibility;
            if (request_processing(CALLID_MESSENGER_UPDATE_VISIBILITY))
              return call_messenger_update_visibility_.result_;
          %>
        %>
        return false;
      }

      bool_t
        call_messenger_update_alive_period(const messenger_key_t& key,
                                           multiple_of_100ms_t factor) {
        <% mutexguard_t guard(seqmutex_);
          <% mutexguard_t guard(mutex_);
            call_messenger_update_alive_period_.key_    = &key;
            call_messenger_update_alive_period_.factor_ = factor;
            if (request_processing(CALLID_MESSENGER_UPDATE_ALIVE_PERIOD))
              return call_messenger_update_alive_period_.result_;
          %>
        %>
        return false;
      }

      bool_t
        call_messenger_start_timer(const messenger_key_t& key,
                                   const messenger_timer_params_t& params) {
        <% mutexguard_t guard(seqmutex_);
          <% mutexguard_t guard(mutex_);
            call_messenger_start_timer_.key_    = &key;
            call_messenger_start_timer_.params_ = &params;
            if (request_processing(CALLID_MESSENGER_START_TIMER))
              return call_messenger_start_timer_.result_;
          %>
        %>
        return false;
      }

      bool_t call_messenger_stop_timer(const messenger_key_t& key) {
        <% mutexguard_t guard(seqmutex_);
          <% mutexguard_t guard(mutex_);
            call_messenger_stop_timer_.key_ = &key;
            if (request_processing(CALLID_MESSENGER_STOP_TIMER))
              return call_messenger_stop_timer_.result_;
          %>
        %>
        return false;
      }

      bool_t call_messenger_query_timer(const messenger_key_t& key,
                                        messenger_timer_params_t& params) {
        <% mutexguard_t guard(seqmutex_);
          <% mutexguard_t guard(mutex_);
            call_messenger_query_timer_.key_    = &key;
            call_messenger_query_timer_.params_ = &params;
            if (request_processing(CALLID_MESSENGER_QUERY_TIMER))
              return call_messenger_query_timer_.result_;
          %>
        %>
        return false;
      }

      bool_t call_messenger_add_to_group(const messenger_key_t& key,
                                         const password_t& password,
                                         const messenger_name_t& name,
                                         messenger_prio_t prio,
                                         messenger_user_t user) {
        <% mutexguard_t guard(seqmutex_);
          <% mutexguard_t guard(mutex_);
            call_messenger_add_to_group_.password_ = &password;
            call_messenger_add_to_group_.key_      = &key;
            call_messenger_add_to_group_.name_     = &name;
            call_messenger_add_to_group_.prio_     = prio;
            call_messenger_add_to_group_.user_     = user;
            if (request_processing(CALLID_MESSENGER_ADD_TO_GROUP))
              return call_messenger_add_to_group_.result_;
          %>
        %>
        return false;
      }

      bool_t
        call_messenger_remove_from_group(const messenger_key_t& key,
                                         const password_t& password,
                                         const messenger_name_t& name,
                                         messenger_user_t* user) {
        <% mutexguard_t guard(seqmutex_);
          <% mutexguard_t guard(mutex_);
            call_messenger_remove_from_group_.password_ = &password;
            call_messenger_remove_from_group_.key_      = &key;
            call_messenger_remove_from_group_.name_     = &name;
            call_messenger_remove_from_group_.user_     = user;
            if (request_processing(CALLID_MESSENGER_REMOVE_FROM_GROUP))
              return call_messenger_remove_from_group_.result_;
          %>
        %>
        return false;
      }

      bool_t call_messenger_is_in_group(const messenger_key_t& key,
                                        const messenger_name_t& name,
                                        messenger_user_t* user) {
        <% mutexguard_t guard(seqmutex_);
          <% mutexguard_t guard(mutex_);
            call_messenger_is_in_group_.key_  = &key;
            call_messenger_is_in_group_.name_ = &name;
            call_messenger_is_in_group_.user_ = user;
            if (request_processing(CALLID_MESSENGER_IS_IN_GROUP))
              return call_messenger_is_in_group_.result_;
          %>
        %>
        return false;
      }

      bool_t call_messenger_get_groups(const messenger_key_t& key,
                                       messenger_memberlist_t& list) {
        <% mutexguard_t guard(seqmutex_);
          <% mutexguard_t guard(mutex_);
            call_messenger_get_groups_.key_  = &key;
            call_messenger_get_groups_.list_ = &list;
            if (request_processing(CALLID_MESSENGER_GET_GROUPS))
              return call_messenger_get_groups_.result_;
          %>
        %>
        return false;
      }

      bool_t call_messenger_add_monitor(const messenger_key_t& key,
                                        const messenger_name_t& name,
                                        messenger_prio_t prio,
                                        messenger_user_t user) {
        <% mutexguard_t guard(seqmutex_);
          <% mutexguard_t guard(mutex_);
            call_messenger_add_monitor_.key_  = &key;
            call_messenger_add_monitor_.name_ = &name;
            call_messenger_add_monitor_.prio_ = prio;
            call_messenger_add_monitor_.user_ = user;
            if (request_processing(CALLID_MESSENGER_ADD_MONITOR))
              return call_messenger_add_monitor_.result_;
          %>
        %>
        return false;
      }

      bool_t call_messenger_remove_monitor(const messenger_key_t& key,
                                           const messenger_name_t& name,
                                           messenger_user_t* user) {
        <% mutexguard_t guard(seqmutex_);
          <% mutexguard_t guard(mutex_);
            call_messenger_remove_monitor_.key_  = &key;
            call_messenger_remove_monitor_.name_ = &name;
            call_messenger_remove_monitor_.user_ = user;
            if (request_processing(CALLID_MESSENGER_REMOVE_MONITOR))
              return call_messenger_remove_monitor_.result_;
          %>
        %>
        return false;
      }

      messenger_key_t
        call_messenger_is_monitored(const messenger_key_t& key,
                                    const messenger_name_t& name,
                                    messenger_user_t* user) {
        <% mutexguard_t guard(seqmutex_);
          <% mutexguard_t guard(mutex_);
            call_messenger_is_monitored_.key_  = &key;
            call_messenger_is_monitored_.name_ = &name;
            call_messenger_is_monitored_.user_ = user;
            if (request_processing(CALLID_MESSENGER_IS_MONITORED))
              return call_messenger_is_monitored_.namekey_;
          %>
        %>
        return messenger_key_t(0ULL);
      }

      bool_t call_messenger_get_monitored(const messenger_key_t& key,
                                          messenger_memberkeylist_t& list) {
        <% mutexguard_t guard(seqmutex_);
          <% mutexguard_t guard(mutex_);
            call_messenger_get_monitored_.key_  = &key;
            call_messenger_get_monitored_.list_ = &list;
            if (request_processing(CALLID_MESSENGER_GET_MONITORED))
              return call_messenger_get_monitored_.result_;
          %>
        %>
        return false;
      }

      void_t call_update_params(const params_t& params) {
        <% mutexguard_t guard(seqmutex_);
          <% mutexguard_t guard(mutex_);
            call_update_params_.params_ = &params;
            request_processing(CALLID_UPDATE_PARAMS);
          %>
        %>
      }

      void_t call_fetch_params (params_t& params) {
        <% mutexguard_t guard(seqmutex_);
          <% mutexguard_t guard(mutex_);
            call_fetch_params_.params_ = &params;
            request_processing(CALLID_FETCH_PARAMS);
          %>
        %>
      }

      bool_t call_create_group(const password_t& password,
                               const messenger_name_t& name,
                               messenger_visibility_t visibility) {
        <% mutexguard_t guard(seqmutex_);
          <% mutexguard_t guard(mutex_);
            call_create_group_.password_   = &password;
            call_create_group_.name_       = &name;
            call_create_group_.visibility_ = visibility;
            if (request_processing(CALLID_CREATE_GROUP))
              return call_create_group_.result_;
          %>
        %>
        return false;
      }

      bool_t call_destroy_group(const password_t& password,
                                const messenger_name_t& name) {
        <% mutexguard_t guard(seqmutex_);
          <% mutexguard_t guard(mutex_);
            call_destroy_group_.password_ = &password;
            call_destroy_group_.name_     = &name;
            if (request_processing(CALLID_DESTROY_GROUP))
              return call_destroy_group_.result_;
          %>
        %>
        return false;
      }

      bool_t call_fetch_group(const messenger_name_t& name,
                              messenger_visibility_t& visibility,
                              messenger_memberlist_t* list) {
        <% mutexguard_t guard(seqmutex_);
          <% mutexguard_t guard(mutex_);
            call_fetch_group_.name_       = &name;
            call_fetch_group_.visibility_ = &visibility;
            call_fetch_group_.list_       = list;
            if (request_processing(CALLID_FETCH_GROUP))
              return call_fetch_group_.result_;
          %>
        %>
        return false;
      }

      bool_t call_add_messenger_to_group(const password_t& password,
                                         const messenger_name_t& name,
                                         const messenger_name_t& group,
                                         messenger_prio_t prio,
                                         messenger_user_t user) {
        <% mutexguard_t guard(seqmutex_);
          <% mutexguard_t guard(mutex_);
            call_add_messenger_to_group_.password_ = &password;
            call_add_messenger_to_group_.name_     = &name;
            call_add_messenger_to_group_.group_    = &group;
            call_add_messenger_to_group_.prio_     = prio;
            call_add_messenger_to_group_.user_     = user;
            if (request_processing(CALLID_ADD_MESSENGER_TO_GROUP))
              return call_add_messenger_to_group_.result_;
          %>
        %>
        return false;
      }

      bool_t call_remove_messenger_from_group(const password_t& password,
                                              const messenger_name_t& name,
                                              const messenger_name_t& group,
                                              messenger_user_t* user) {
        <% mutexguard_t guard(seqmutex_);
          <% mutexguard_t guard(mutex_);
            call_remove_messenger_from_group_.password_ = &password;
            call_remove_messenger_from_group_.name_     = &name;
            call_remove_messenger_from_group_.group_    = &group;
            call_remove_messenger_from_group_.user_     = user;
            if (request_processing(CALLID_REMOVE_MESSENGER_FROM_GROUP))
              return call_remove_messenger_from_group_.result_;
          %>
        %>
        return false;
      }

      bool_t call_is_messenger_in_group(const messenger_name_t& name,
                                        const messenger_name_t& group,
                                        messenger_user_t* user) {
        <% mutexguard_t guard(seqmutex_);
          <% mutexguard_t guard(mutex_);
            call_is_messenger_in_group_.name_  = &name;
            call_is_messenger_in_group_.group_ = &group;
            call_is_messenger_in_group_.user_  = user;
            if (request_processing(CALLID_IS_MESSENGER_IN_GROUP))
              return call_is_messenger_in_group_.result_;
          %>
        %>
        return false;
      }

      bool_t call_fetch_messenger_groups(const messenger_name_t& name,
                                         messenger_memberlist_t& list) {
        <% mutexguard_t guard(seqmutex_);
          <% mutexguard_t guard(mutex_);
            call_fetch_messenger_groups_.name_ = &name;
            call_fetch_messenger_groups_.list_ = &list;
            if (request_processing(CALLID_FETCH_MESSENGER_GROUPS))
              return call_fetch_messenger_groups_.result_;
          %>
        %>
        return false;
      }

      messenger_id_t
        call_create_messenger(errorid_t& errorid,
                              const messenger_name_t& name,
                              const messenger_create_params_t& params) {
        <% mutexguard_t guard(seqmutex_);
          <% mutexguard_t guard(mutex_);
            call_create_messenger_.errorid_ = &errorid;
            call_create_messenger_.name_    = &name;
            call_create_messenger_.params_  = &params;
            if (request_processing(CALLID_CREATE_MESSENGER))
              return call_create_messenger_.id_;
          %>
        %>
        return messenger_id_t();
      }

      bool_t call_fetch_messenger(const messenger_name_t& name,
                                  messenger_params_t& params) {
        <% mutexguard_t guard(seqmutex_);
          <% mutexguard_t guard(mutex_);
            call_fetch_messenger_.name_   = &name;
            call_fetch_messenger_.params_ = &params;
            if (request_processing(CALLID_FETCH_MESSENGER))
              return call_fetch_messenger_.result_;
          %>
        %>
        return false;
      }

      bool_t call_fetch_messenger(const messenger_name_t& name,
                                  messenger_info_t& info,
                                  bool_t clearstats) {
        <% mutexguard_t guard(seqmutex_);
          <% mutexguard_t guard(mutex_);
            call_fetchclear_messenger_.name_       = &name;
            call_fetchclear_messenger_.info_       = &info;
            call_fetchclear_messenger_.clearstats_ = clearstats;
            if (request_processing(CALLID_FETCHCLEAR_MESSENGER))
              return call_fetchclear_messenger_.result_;
          %>
        %>
        return false;
      }

      bool_t call_fetch_messengers(messenger_infos_t& infos,
                                   bool_t clearstats) {
        <% mutexguard_t guard(seqmutex_);
          <% mutexguard_t guard(mutex_);
            call_fetch_messengers_.infos_      = &infos;
            call_fetch_messengers_.clearstats_ = clearstats;
            if (request_processing(CALLID_FETCH_MESSENGERS))
              return call_fetch_messengers_.result_;
          %>
        %>
        return false;
      }

      bool_t call_destroy_messenger(const messenger_key_t& key) {
        <% mutexguard_t guard(seqmutex_);
          <% mutexguard_t guard(mutex_);
            call_destroy_messenger_.key_ = &key;
            if (request_processing(CALLID_DESTROY_MESSENGER))
              return call_destroy_messenger_.result_;
          %>
        %>
        return false;
      }

      bool_t call_who_is(const messenger_key_t& key, messenger_name_t& name,
                         bool_t* group, bool_t* local) {
        <% mutexguard_t guard(seqmutex_);
          <% mutexguard_t guard(mutex_);
            call_who_is_.key_        = &key;
            call_who_is_.name_       = &name;
            call_who_is_.group_      = group;
            call_who_is_.local_      = local;
            if (request_processing(CALLID_WHO_IS))
              return call_who_is_.result_;
          %>
        %>
        return false;
      }

////////////////////////////////////////////////////////////////////////////////

      bool_t is_valid() const  { return !errorid_; }
      fd_t   get_fd  () const  { return fd_;       }

      bool_t process_command(prio_messages_t&);

////////////////////////////////////////////////////////////////////////////////

    private:
      enum callid_t {
        CALLID_BAD = 0,
        CALLID_MESSENGER_GET_NAME,
        CALLID_MESSENGER_GET_PARAMS,
        CALLID_MESSENGER_UPDATE_VISIBILITY,
        CALLID_MESSENGER_UPDATE_ALIVE_PERIOD,
        CALLID_MESSENGER_START_TIMER,
        CALLID_MESSENGER_STOP_TIMER,
        CALLID_MESSENGER_QUERY_TIMER,
        CALLID_MESSENGER_ADD_TO_GROUP,
        CALLID_MESSENGER_REMOVE_FROM_GROUP,
        CALLID_MESSENGER_IS_IN_GROUP,
        CALLID_MESSENGER_GET_GROUPS,
        CALLID_MESSENGER_ADD_MONITOR,
        CALLID_MESSENGER_REMOVE_MONITOR,
        CALLID_MESSENGER_IS_MONITORED,
        CALLID_MESSENGER_GET_MONITORED,
        CALLID_UPDATE_PARAMS,
        CALLID_FETCH_PARAMS,
        CALLID_CREATE_GROUP,
        CALLID_DESTROY_GROUP,
        CALLID_FETCH_GROUP,
        CALLID_ADD_MESSENGER_TO_GROUP,
        CALLID_REMOVE_MESSENGER_FROM_GROUP,
        CALLID_IS_MESSENGER_IN_GROUP,
        CALLID_FETCH_MESSENGER_GROUPS,
        CALLID_CREATE_MESSENGER,
        CALLID_FETCH_MESSENGER,
        CALLID_FETCHCLEAR_MESSENGER,
        CALLID_FETCH_MESSENGERS,
        CALLID_DESTROY_MESSENGER,
        CALLID_WHO_IS };

      bool_t request_processing(callid_t id) {
        ullong_t value = id;
        if (::write(get(fd_), &value, sizeof(value)) == sizeof(value)) {
          done_ = false;
          while (!done_)
            cond_.wait(mutex_);
        } else
          return false;
        return true;
      }

      errorid_t              errorid_;
      bool_t                 done_;
      mutex_t                seqmutex_;
      mutex_t                mutex_;
      conditional_variable_t cond_;
      fd_t                   fd_;

      struct call_messenger_get_name_t {
        const messenger_key_t* key_;
        messenger_name_t       name_;
      } call_messenger_get_name_;

      struct call_messenger_get_params_t {
        const messenger_key_t* key_;
        messenger_params_t*    params_;
        bool_t                 result_;
      } call_messenger_get_params_;

      struct call_messenger_update_visibility_t {
        const messenger_key_t* key_;
        messenger_visibility_t visibility_;
        bool_t                 result_;
      } call_messenger_update_visibility_;

      struct call_messenger_start_timer_t {
        const messenger_key_t*           key_;
        const messenger_timer_params_t*  params_;
        bool_t                           result_;
      } call_messenger_start_timer_;

      struct call_messenger_stop_timer_t {
        const messenger_key_t* key_;
        bool_t result_;
      } call_messenger_stop_timer_;

      struct call_messenger_query_timer_t {
        const messenger_key_t*    key_;
        messenger_timer_params_t* params_;
        bool_t                    result_;
      } call_messenger_query_timer_;

      struct call_messenger_add_to_group_t {
        const password_t*       password_;
        const messenger_key_t*  key_;
        const messenger_name_t* name_;
        messenger_prio_t        prio_;
        messenger_user_t        user_;
        bool_t                  result_;
        call_messenger_add_to_group_t() : prio_(0) { }
      } call_messenger_add_to_group_;

      struct call_messenger_remove_from_group_t {
        const password_t*       password_;
        const messenger_key_t*  key_;
        const messenger_name_t* name_;
        messenger_user_t*       user_;
        bool_t                  result_;
      } call_messenger_remove_from_group_;

      struct call_messenger_is_in_group_t {
        const messenger_key_t*  key_;
        const messenger_name_t* name_;
        messenger_user_t*       user_;
        bool_t                  result_;
      } call_messenger_is_in_group_;

      struct call_messenger_get_groups_t {
        const messenger_key_t*  key_;
        messenger_memberlist_t* list_;
        bool_t                  result_;
      } call_messenger_get_groups_;

      struct call_messenger_add_monitor_t {
        const messenger_key_t*  key_;
        const messenger_name_t* name_;
        messenger_prio_t        prio_;
        messenger_user_t        user_;
        bool_t                  result_;
        call_messenger_add_monitor_t() : prio_(0) { }
      } call_messenger_add_monitor_;

      struct call_messenger_remove_monitor_t {
        const messenger_key_t*  key_;
        const messenger_name_t* name_;
        messenger_user_t*       user_;
        bool_t                  result_;
      } call_messenger_remove_monitor_;

      struct call_messenger_is_monitored_t {
        const messenger_key_t*  key_;
        const messenger_name_t* name_;
        messenger_user_t*       user_;
        messenger_key_t         namekey_;
        call_messenger_is_monitored_t() : namekey_(0) { }
      } call_messenger_is_monitored_;

      struct call_messenger_get_monitored_t {
        const messenger_key_t*     key_;
        messenger_memberkeylist_t* list_;
        bool_t                     result_;
      } call_messenger_get_monitored_;

      struct call_messenger_update_alive_period_t {
        const messenger_key_t* key_;
        multiple_of_100ms_t factor_;
        bool_t              result_;
        call_messenger_update_alive_period_t() : factor_(0) { }
      } call_messenger_update_alive_period_;

      struct call_update_params_t {
        const params_t* params_;
      } call_update_params_;

      struct call_fetch_params_t {
        params_t* params_;
      } call_fetch_params_;

      struct call_create_group_t {
        const password_t*       password_;
        const messenger_name_t* name_;
        messenger_visibility_t  visibility_;
        bool_t                  result_;
      } call_create_group_;

      struct call_destroy_group_t {
        const password_t*       password_;
        const messenger_name_t* name_;
        bool_t                  result_;
      } call_destroy_group_;

      struct call_fetch_group_t {
        const messenger_name_t* name_;
        messenger_visibility_t* visibility_;
        messenger_memberlist_t* list_;
        bool_t                  result_;
      } call_fetch_group_;

      struct call_add_messenger_to_group_t {
        const password_t*       password_;
        const messenger_name_t* name_;
        const messenger_name_t* group_;
        messenger_prio_t        prio_;
        messenger_user_t        user_;
        bool_t                  result_;
        call_add_messenger_to_group_t() : prio_(0) { }
      } call_add_messenger_to_group_;

      struct call_remove_messenger_from_group_t {
        const password_t*       password_;
        const messenger_name_t* name_;
        const messenger_name_t* group_;
        messenger_user_t*       user_;
        bool_t                  result_;
      } call_remove_messenger_from_group_;

      struct call_is_messenger_in_group_t {
        const messenger_name_t* name_;
        const messenger_name_t* group_;
        messenger_user_t*       user_;
        bool_t                  result_;
      } call_is_messenger_in_group_;

      struct call_fetch_messenger_groups_t {
        const messenger_name_t* name_;
        messenger_memberlist_t* list_;
        bool_t                  result_;
      } call_fetch_messenger_groups_;

      struct call_create_messenger_t {
        errorid_t*                       errorid_;
        const messenger_name_t*          name_;
        const messenger_create_params_t* params_;
        messenger_id_t            id_;
      } call_create_messenger_;

      struct call_fetch_messenger_t {
        const messenger_name_t* name_;
        messenger_params_t*     params_;
        bool_t                  result_;
      } call_fetch_messenger_;

      struct call_fetchclear_messenger_t {
        const messenger_name_t* name_;
        messenger_info_t*       info_;
        bool_t                  clearstats_;
        bool_t                  result_;
      } call_fetchclear_messenger_;

      struct call_fetch_messengers_t {
        messenger_infos_t* infos_;
        bool_t             clearstats_;
        bool_t             result_;
      } call_fetch_messengers_;

      struct call_destroy_messenger_t {
        const messenger_key_t* key_;
        bool_t                 result_;
      } call_destroy_messenger_;

      struct call_who_is_t {
        const messenger_key_t* key_;
        messenger_name_t*      name_;
        bool_t*                group_;
        bool_t*                local_;
        bool_t                 result_;
      } call_who_is_;
    };

////////////////////////////////////////////////////////////////////////////////

    struct threadhandler_t : detachedthread_t::handler_t
    {
      bool_t prepare();
      void_t process();
    };

////////////////////////////////////////////////////////////////////////////////

    struct timer_info_t {
      nanoseconds_t    start_timestamp_;
      nanoseconds_t    max_;
      nanoseconds_t    current_;
      bool_t           periodic_;
      messenger_prio_t prio_;
      messenger_user_t user_;
      timer_info_t() : start_timestamp_(0), max_(0), current_(0),
                       periodic_(false), prio_(0) { }
      timer_info_t(nanoseconds_t max,
                   bool_t periodic) : start_timestamp_(0), max_(max),
                   current_(0), periodic_(periodic), prio_(0) { }
    };
    typedef std::map<messenger_key_t, timer_info_t> timerlist_t;

    struct alive_info_t {
      uint_t              cnt_;
      multiple_of_100ms_t current_;
      multiple_of_100ms_t max_;
      alive_info_t() : cnt_(0), current_(0), max_(2) { }
    };

    struct messenger_ctxt_t {
      fd_queue_t        queue_;
      alive_info_t      alive_;
      messenger_info_t  info_;
      messenger_ctxt_t() : queue_(200) { }
    };

    struct member_t {
      password_t       password_;
      messenger_key_t  key_;
      messenger_prio_t prio_;
      messenger_user_t user_;
      member_t(const password_t& password,
               const messenger_key_t& key,
               messenger_prio_t prio,
               messenger_user_t user)
        : password_(password), key_(key), prio_(prio), user_(user) { }
    };
    typedef std::map<messenger_name_t, member_t> memberlist_t;

    struct messenger_groupinfo_t {
      bool_t                 exist_;
      messenger_key_t        key_;
      messenger_name_t       name_;
      password_t             password_; // group creation/deletion
      messenger_visibility_t visibility_;
      memberlist_t           members_;
      messenger_groupinfo_t() : exist_(false), key_(0) { }
    };

    struct monitored_t {
      messenger_state_t state_;
      messenger_key_t   key_;
      memberlist_t      members_;
      monitored_t() : state_(messenger_state_unavailable), key_(0) { }
      monitored_t(const messenger_key_t& key) : key_(key) { }
    };

    typedef nfreelist_t<messenger_ctxt_t, 300> messenger_freelist_t;
    typedef nfreelist_t<messenger_groupinfo_t, 2000>
      messengergroup_freelist_t;

    typedef std::map<messenger_name_t, messenger_key_t>
      messenger_lookup_t;

    typedef std::map<messenger_name_t, monitored_t>
      monitored_lookup_t;

    typedef std::map<messenger_name_t, messenger_memberlist_t>
      messenger_grouplookup_t;

    typedef uint_t tipcport_t;

    struct remote_info_t {
      fd_t         fd_;    // file desciptor - for named port
      remotename_t name_;  // remote name
      tipcport_t   port_;  // remote address
      tipcport_t   self_;  // learned on my port
      uint_t       rxseq_; // last seq received
      uint_t       txseq_; // last seq sent
      remote_info_t() : fd_(0) { }
    };

    //typedef nfreelist_t<> messenger_remote_freelist;

    class ctxt_t {
    public:
      static bool& init() {
        return ctxt_.init_;
      }

      static errorid_t& errorid() {
        return ctxt_.errorid_;
      }

      static params_t& params() {
        return ctxt_.params_;
      }

      static command_engine_t& cmd_engine() {
        return ctxt_.cmd_engine_;
      }

      static fd_queue_t& queue() {
        return ctxt_.queue_;
      }

      static fd_t& epoll_fd() {
        return ctxt_.epollfd_;
      }

      static fd_t& timer_fd() {
        return ctxt_.timerfd_;
      }

      static timerlist_t& timer_list() {
        return ctxt_.timerlist_;
      }

      static messenger_lookup_t& messenger_lookup() {
        return ctxt_.lookup_;
      }

      static monitored_lookup_t& monitored_lookup() {
        return ctxt_.monitoredlookup_;
      }

      static messenger_grouplookup_t& messenger_grouplookup() {
        return ctxt_.grouplookup_;
      }

      static messenger_freelist_t& messenger_list() {
        return ctxt_.messengers_;
      }

      static messengergroup_freelist_t& messengergroup_list() {
        return ctxt_.messengergroups_;
      }

      static ushort_t& seq() {
        return ctxt_.seq_;
      }

      static ushort_t& grpseq() {
        return ctxt_.grpseq_;
      }

      static messenger_key_t& alivegrpkey() {
        return ctxt_.alivegrpkey_;
      }

    private:
      ctxt_t() : init_       (false),
                 errorid_    (0),
                 params_     (remote_visibility_off),
                 seq_        (0),
                 grpseq_     (0),
                 epollfd_    (::epoll_create(100)), // any number > 0
                 timerfd_    (::timerfd_create(CLOCK_MONOTONIC, 0)),
                 queue_      (2000),
                 alivegrpkey_(0),
                 lookup_     (),
                 thread_     ("messaging", &threadhandler_) {
        if (get(epollfd_) == -1     ||
            get(timerfd_) == -1     ||
            !cmd_engine_.is_valid() ||
            !queue_.is_valid()      ||
            !thread_.is_valid())
          errorid_ = -1;
      }

      ~ctxt_t() {
        if (get(epollfd_) != -1)
          ::close(get(epollfd_));
        if (get(timerfd_) != -1)
          ::close(get(timerfd_));
      }

      bool_t                    init_;
      errorid_t                 errorid_;
      params_t                  params_;
      ushort_t                  seq_;
      ushort_t                  grpseq_;
      fd_t                      epollfd_;
      fd_t                      timerfd_;
      fd_queue_t                queue_;
      messenger_key_t           alivegrpkey_;
      timerlist_t               timerlist_;
      messenger_lookup_t        lookup_;
      messenger_grouplookup_t   grouplookup_;
      monitored_lookup_t        monitoredlookup_;
      messenger_freelist_t      messengers_;
      messengergroup_freelist_t messengergroups_;
// remote freelist
      command_engine_t          cmd_engine_;
      threadhandler_t           threadhandler_;
      detachedthread_t          thread_;

      static ctxt_t ctxt_;
    };
    ctxt_t ctxt_t::ctxt_; // construct before main

////////////////////////////////////////////////////////////////////////////////

    void_t update_prio_messages(prio_messages_t& prio_messages,
                                const messenger_name_t& name,
                                messenger_state_t state,
                                const messenger_key_t& key,
                                const messenger_groupinfo_t& info) {
      const memberlist_t& list = info.members_;
      uint_t n = list.size();
      if (n) {
        for (memberlist_t::const_iterator i(list.begin());
             i != list.end(); ++i) {
          bool_t local = false, group = false;
          ushort_t seq = 0, id = 0;
          if (get_key_params(i->second.key_, local, group, seq, id)) {
            if (local) {
              if (!group) {
                prio_messages_t::value_type
                  tmp(i->second.prio_, prio_message_t(i->second.key_,
                                                      i->second.user_));
                prio_messages_t::iterator j(prio_messages.insert(tmp));

                notify_message_t message;
                write_notify_message(message,
                                     i->second.key_,
                                     state,
                                     name,
                                     key,
                                     i->second.prio_,
                                     i->second.user_);
                j->second.content_ = message.release();
              } else {
                messenger_groupinfo_t* sinfo =
                  ctxt_t::messengergroup_list().get(nfreelist_id_t(id));
                if (sinfo && sinfo->exist_) {
                   update_prio_messages(prio_messages, name, state, key,
                                        *sinfo);
                }
              }
            } else {
               // see if remote is on same unit
            }
          }
        }
      }
    }

    void_t update_prio_messages(prio_messages_t& prio_messages,
                                const messenger_name_t& name,
                                const monitored_t& monitored) {
      const memberlist_t& list = monitored.members_;
      uint_t n = list.size();
      if (n) {
        for (memberlist_t::const_iterator i(list.begin());
             i != list.end(); ++i) {
          prio_messages_t::value_type
            tmp(i->second.prio_, prio_message_t(i->second.key_,
                                                i->second.user_));
          prio_messages_t::iterator j(prio_messages.insert(tmp));

          notify_message_t message;
          write_notify_message(message,
                               i->second.key_,
                               monitored.state_,
                               name,
                               monitored.key_,
                               i->second.prio_,
                               i->second.user_);
          j->second.content_ = message.release();
        }
      }
    }

////////////////////////////////////////////////////////////////////////////////

    bool_t process_add_messenger_to_group(prio_messages_t& prio_messages,
                                          const password_t& password,
                                          const messenger_name_t& name,
                                          const messenger_name_t& group,
                                          messenger_prio_t prio,
                                          messenger_user_t user) {
      messenger_lookup_t& lookup = ctxt_t::messenger_lookup();
      messenger_lookup_t::iterator i(lookup.find(name));
      if (i != lookup.end()) {
        messenger_name_t mod_name(string::cstr, "@");
        const messenger_name_t* gn = &group;
        if (*group.c_str() != '@') {
          mod_name += group;
          gn = &mod_name;
        }

        messenger_grouplookup_t& glookup = ctxt_t::messenger_grouplookup();
        std::pair<messenger_grouplookup_t::iterator, bool> j(
          glookup.insert(messenger_grouplookup_t::value_type(name,
                           messenger_memberlist_t())));
        if (j.second)
          j.first->second.insert(messenger_memberlist_t::value_type(
                                   *gn, messenger_member_t(password, prio,
                                                           user)));
        else if (j.first != glookup.end()) {
          messenger_memberlist_t& members = j.first->second;
          std::pair<messenger_memberlist_t::iterator, bool> k(
            members.insert(messenger_memberlist_t::value_type(
              *gn, messenger_member_t(password, prio, user))));
          if (!k.second && k.first != members.end())
            return false;
        } else
          return false;

        messenger_lookup_t::value_type tmp(*gn, messenger_key_t(0));
        std::pair<messenger_lookup_t::iterator, bool> p(
          ctxt_t::messenger_lookup().insert(tmp));
        if (p.second) {
          messengergroup_freelist_t::result_t r(
            ctxt_t::messengergroup_list().insert());
          if (r) {
            p.first->second = make_group_key(++ctxt_t::grpseq(),
                                             get(r.id_));
            messenger_groupinfo_t* info = r.ptr_;
            info->name_  = *gn;
            info->key_   = p.first->second;
            info->exist_ = false;
            memberlist_t::value_type tmp(
              i->first, member_t(password, i->second, prio, user));
            info->members_.insert(tmp);
            return true;
          }
          ctxt_t::messenger_lookup().erase(p.first);
        } else if (p.first != ctxt_t::messenger_lookup().end()) {
          bool_t local = false, group = false;
          ushort_t seq = 0, id = 0;
          if (get_key_params(p.first->second, local, group, seq, id) &&
              local && group) {
            messenger_groupinfo_t* info = ctxt_t::messengergroup_list()
              .get(nfreelist_id_t(id));
            if (info) {
              memberlist_t::value_type
                tmp(i->first, member_t(password, i->second, prio, user));
              if (info->members_.insert(tmp).second) {
                if (info->exist_) {
                  update_prio_messages(prio_messages, *gn,
                                       messenger_state_available,
                                       info->key_, *info);
                }
                return true;
              }
            }
          }
        }
        if (j.first->second.size() == 1)
          glookup.erase(j.first);
        else
          j.first->second.erase(*gn);
      }
      return false;
    }

    bool_t
      process_remove_messenger_from_group(const password_t& password,
                                          const messenger_name_t& name,
                                          const messenger_name_t& group,
                                          messenger_user_t* user) {
      messenger_name_t mod_name(string::cstr, "@");
      const messenger_name_t* gn = &group;
      if (*group.c_str() != '@') {
        mod_name += group;
        gn = &mod_name;
      }
      messenger_lookup_t& lookup = ctxt_t::messenger_lookup();
      messenger_lookup_t::iterator i(lookup.find(*gn));
      if (i != lookup.end()) {
        bool_t local = false, group = false;
        ushort_t seq = 0, id = 0;
        if (get_key_params(i->second, local, group, seq, id) &&
            local && group) {
          messenger_groupinfo_t* info = ctxt_t::messengergroup_list()
            .get(nfreelist_id_t(id));
          if (info) {
            memberlist_t& list = info->members_;
            memberlist_t::iterator j(list.find(name));
            if (j != list.end()) {
              if (j->second.password_ == password) {
                if (user)
                  *user = j->second.user_;
                list.erase(j);
                if (list.empty() && !info->exist_) {
                  ctxt_t::messengergroup_list().erase(nfreelist_id_t(id));
                  lookup.erase(i);
                }

                messenger_grouplookup_t& glookup =
                  ctxt_t::messenger_grouplookup();
                messenger_grouplookup_t::iterator k(glookup.find(name));
                if (k->second.size() == 1)
                  glookup.erase(k);
                else
                  k->second.erase(*gn);
                return true;
              }
            }
          }
        }
      }
      return false;
    }

    void_t process_new_messenger(prio_messages_t& prio_messages,
                                 messenger_ctxt_t& ctxt) {
      monitored_lookup_t::iterator m(
        ctxt_t::monitored_lookup().find(ctxt.info_.name_));
      if (m != ctxt_t::monitored_lookup().end()) {
        m->second.key_ = ctxt.info_.key_;
        m->second.state_ = messenger_state_available;
        update_prio_messages(prio_messages, m->first, m->second);
      }

      if (get(ctxt.info_.params_.timer_params_.factor_)) {
        messenger_timer_params_t& params = ctxt.info_.params_.timer_params_;
        timer_info_t& info = ctxt_t::timer_list()[ctxt.info_.key_];
        info.max_             = 100000000LL * get(params.factor_);
        info.current_         = 0;
        info.start_timestamp_ = 0; // timestamp
        info.periodic_        = params.periodic_;
        info.prio_            = params.prio_;
        info.user_            = params.user_;
      }
    }

    void_t process_remove_messenger(prio_messages_t& prio_messages,
                                    messenger_ctxt_t& ctxt) {
      messenger_memberlist_t& monitor = ctxt.info_.params_.monitor_;
      for (messenger_memberlist_t::iterator i = monitor.begin();
           i != monitor.end(); ++i) {
        monitored_lookup_t::iterator p(ctxt_t::monitored_lookup().find(
          i->first));
        monitored_t& entry = p->second;
        entry.members_.erase(ctxt.info_.name_);
        if (entry.members_.empty())
          ctxt_t::monitored_lookup().erase(p);
      }

      /*
      messenger_memberlist_t& groups = ctxt.info_.params_.groups_;
      for (messenger_memberlist_t::iterator i = groups.begin();
          i != groups.end(); ++i)
         process_remove_messenger_from_group(i->second.password_,
                                             ctxt.info_.name_,
                                             i->first, 0);
      */
      //process_remove_messenger_from_groups(ctxt.info_.name_);
      //xxx

      ctxt_t::timer_list().erase(ctxt.info_.key_);

      monitored_lookup_t::iterator p(ctxt_t::monitored_lookup().find(
        ctxt.info_.name_));
      if (p != ctxt_t::monitored_lookup().end()) {
        set(p->second.key_) = 0;
        p->second.state_ = messenger_state_unavailable;
        update_prio_messages(prio_messages, p->first, p->second);
      }
    }

////////////////////////////////////////////////////////////////////////////////

    void_t process_update_params(prio_messages_t& prio_messages,
                                 const params_t& params) {
      // must still implement functionality
      //
      // - if already connected to local and remote connections,
      //   then update with new name.
      // - this means sending to all units
      // visibility will close and open sockets
    }

    void_t process_fetch_params(params_t& params) {
      params = ctxt_t::params();
    }

    bool_t process_create_group(prio_messages_t& prio_messages,
                                const password_t& password,
                                const messenger_name_t& name,
                                messenger_visibility_t visibility) {
      messenger_name_t mod_name(string::cstr, "@");
      const messenger_name_t* n = &name;
      if (*name.c_str() != '@') {
        mod_name += name;
        n = &mod_name;
      }
      messenger_key_t key(0);
      messenger_lookup_t::value_type tmp(*n, key);
      std::pair<messenger_lookup_t::iterator, bool> p(
        ctxt_t::messenger_lookup().insert(tmp));
      if (p.second) {
        messengergroup_freelist_t::result_t r(
          ctxt_t::messengergroup_list().insert());
        if (r) {
          key = make_group_key(++ctxt_t::grpseq(), get(r.id_));
          p.first->second = key;
          messenger_groupinfo_t* info = r.ptr_;
          info->key_        = p.first->second;
          info->name_       = *n;
          info->exist_      = true;
          info->password_   = password;
          info->visibility_ = visibility;
        } else {
          ctxt_t::messenger_lookup().erase(p.first);
          return false;
        }
      } else if (p.first != ctxt_t::messenger_lookup().end()) {
        key = p.first->second;
        bool_t local = false, group = false;
        ushort_t seq = 0, id = 0;
        if (get_key_params(key, local, group, seq, id) &&
            local && group) {
          messenger_groupinfo_t* info =
            ctxt_t::messengergroup_list().get(nfreelist_id_t(id));
          if (!info->exist_) {
            info->exist_      = true;
            info->password_   = password;
            info->visibility_ = visibility;
            update_prio_messages(prio_messages, *n,
                                 messenger_state_available, info->key_,
                                 *info);
          }
        }
      } else
        return false;

      monitored_lookup_t::iterator m(
        ctxt_t::monitored_lookup().find(*n));
      if (m != ctxt_t::monitored_lookup().end()) {
        m->second.key_ = key;
        m->second.state_ = messenger_state_available;
        update_prio_messages(prio_messages, m->first, m->second);
      }

      if (*n == "@messaging.alive_info")
        ctxt_t::alivegrpkey() = key;

      return true;
    }

    bool_t process_destroy_group(prio_messages_t& prio_messages,
                                 const password_t& password,
                                 const messenger_name_t& name) {
      messenger_name_t mod_name(string::cstr, "@");
      const messenger_name_t* n = &name;
      if (*name.c_str() != '@') {
        mod_name += name;
        n = &mod_name;
      }
      messenger_lookup_t::iterator p(
        ctxt_t::messenger_lookup().find(*n));
      if (p != ctxt_t::messenger_lookup().end()) {
        messenger_key_t key(p->second);
        bool_t local = false, group = false;
        ushort_t seq = 0, id = 0;
        if (get_key_params(key, local, group, seq, id) &&
            local && group) {
          messenger_groupinfo_t* info =
            ctxt_t::messengergroup_list().get(nfreelist_id_t(id));
          if (info->exist_ && password == info->password_) {
            info->exist_ = false;
            update_prio_messages(prio_messages, *n,
                                 messenger_state_unavailable, key, *info);
            if (info->members_.empty()) {
              ctxt_t::messengergroup_list().erase(nfreelist_id_t(id));
              ctxt_t::messenger_lookup().erase(p);
            }

            monitored_lookup_t::iterator m(
              ctxt_t::monitored_lookup().find(*n));
            if (m != ctxt_t::monitored_lookup().end()) {
              set(m->second.key_) = 0;
              m->second.state_ = messenger_state_unavailable;
              update_prio_messages(prio_messages, m->first, m->second);
            }

            if (get(key) == get(ctxt_t::alivegrpkey()))
              set(ctxt_t::alivegrpkey()) = 0;

            return true;
          }
        }
      }
      return false;
    }

    bool_t process_fetch_group(const messenger_name_t& name,
                               messenger_visibility_t& visibility,
                               messenger_memberlist_t* list) {
      messenger_name_t mod_name(string::cstr, "@");
      const messenger_name_t* n = &name;
      if (*name.c_str() != '@') {
        mod_name += name;
        n = &mod_name;
      }
      messenger_lookup_t& lookup = ctxt_t::messenger_lookup();
      messenger_lookup_t::iterator i(lookup.find(*n));
      if (i != lookup.end()) {
        bool_t local = false, group = false;
        ushort_t seq = 0, id = 0;
        if (get_key_params(i->second, local, group, seq, id)
            && local && group) {
          messenger_groupinfo_t* info = ctxt_t::messengergroup_list()
            .get(nfreelist_id_t(id));
          if (info && info->exist_) {
            visibility = info->visibility_;
            bool_t has_members = info->members_.empty();
            if (list && has_members) {
              memberlist_t& members = info->members_;
              for (memberlist_t::iterator j(members.begin());
                   j != members.end(); ++j) {
                messenger_memberlist_t::value_type
                  tmp(j->first, messenger_member_t(j->second.password_,
                                                   j->second.prio_,
                                                   j->second.user_));
                list->insert(tmp);
              }
            }
            return has_members;
          }
        }
      }
      return false;
    }

    bool_t process_is_messenger_in_group(const messenger_name_t& name,
                                         const messenger_name_t& group,
                                         messenger_user_t* user) {
      messenger_name_t mod_name(string::cstr, "@");
      const messenger_name_t* gn = &group;
      if (*group.c_str() != '@') {
        mod_name += group;
        gn = &mod_name;
      }

      messenger_grouplookup_t& glookup = ctxt_t::messenger_grouplookup();
      messenger_grouplookup_t::iterator j(glookup.find(name));
      if (j != glookup.end()) {
        messenger_memberlist_t::iterator l(j->second.find(*gn));
        if (l != j->second.end()) {
          if (user)
            *user = l->second.user_;
          return true;
        }
      }
      return false;
    }

    bool_t process_fetch_messenger_groups(const messenger_name_t& name,
                                          messenger_memberlist_t& list) {
      messenger_grouplookup_t& glookup = ctxt_t::messenger_grouplookup();
      messenger_grouplookup_t::iterator i(glookup.find(name));
      if (i != glookup.end()) {
        list = i->second;
        return true;
      }
      return false;
    }

    messenger_id_t
      process_create_messenger(prio_messages_t& prio_messages,
                               errorid_t& errorid,
                               const messenger_name_t& name,
                               const messenger_create_params_t& params) {
      messenger_freelist_t::result_t r(ctxt_t::messenger_list().insert());
      if (r) {
        messenger_name_t mod_name(string::cstr, "@");
        const messenger_name_t* n = &name;
        if (*name.c_str() != '@') {
           mod_name += name;
           n = &mod_name;
        }
        std::pair<messenger_lookup_t::iterator, bool> p(
          ctxt_t::messenger_lookup().insert(
            messenger_lookup_t::value_type(*n, messenger_key_t(0))));
        if (p.second) {
          messenger_key_t key = make_key(++ctxt_t::seq(), get(r.id_));
          p.first->second = key;
          messenger_info_t& info = r.ptr_->info_;
          info.key_    = key;
          info.name_   = *n;
          info.params_.visibility_   = params.visibility_;
          info.params_.alive_factor_ = params.alive_factor_;
          info.params_.timer_params_ = params.timer_params_;
          process_new_messenger(prio_messages, *r.ptr_);
          return messenger_id_t(p.first->second, r.ptr_->queue_.get_fd());
        }
        ctxt_t::messenger_list().erase(r.id_);
      }
      errorid = 10;
      return messenger_id_t();
    }

    bool_t process_fetch_messenger(const messenger_name_t& name,
                                   messenger_params_t& params) {
     messenger_name_t mod_name(string::cstr, "@");
      const messenger_name_t* n = &name;
      if (*name.c_str() != '@') {
         mod_name += name;
         n = &mod_name;
      }
      messenger_lookup_t& lookup = ctxt_t::messenger_lookup();
      messenger_lookup_t::iterator i(lookup.find(*n));
      if (i != lookup.end()) {
        ushort seq = 0, id = 0;
        if (get_key_local_params(i->second, seq, id)) {
          messenger_ctxt_t* ctxt = ctxt_t::messenger_list().
            get(nfreelist_id_t(id));
          params = ctxt->info_.params_;
          messenger_grouplookup_t& glookup
             = ctxt_t::messenger_grouplookup();
          messenger_grouplookup_t::iterator i(glookup.find(name));
          if (i != glookup.end())
            params.groups_ = i->second;
          else
            params.groups_.clear();
          return true;
        }
      }
      return false;
    }

    bool_t process_fetch_messenger(const messenger_name_t& name,
                                   messenger_info_t& info,
                                   bool_t clearstats) {
     messenger_name_t mod_name(string::cstr, "@");
      const messenger_name_t* n = &name;
      if (*name.c_str() != '@') {
         mod_name += name;
         n = &mod_name;
      }
      messenger_lookup_t& lookup = ctxt_t::messenger_lookup();
      messenger_lookup_t::iterator i(lookup.find(*n));
      if (i != lookup.end()) {
        ushort seq = 0, id = 0;
        if (get_key_local_params(i->second, seq, id)) {
          messenger_ctxt_t* ctxt = ctxt_t::messenger_list().
            get(nfreelist_id_t(id));
          info = ctxt->info_;
          if (clearstats)
            ctxt->info_.stats_.reset();
          return true;
        }
      }
      return false;
    }

    bool_t process_fetch_messengers(messenger_infos_t& infos,
                                    bool_t clearstats) {
      messenger_freelist_t& list = ctxt_t::messenger_list();
      uint_t n = list.size();
      for (uint_t i = 0; n && i < list.capacity(); ++i) {
        messenger_ctxt_t* ctxt = list.get(nfreelist_id_t(i));
        if (ctxt) {
          --n;
          infos.push_back(ctxt->info_);
          if (clearstats)
            ctxt->info_.stats_.reset();
        }
      }
      return list.size();
    }

    bool_t process_destroy_messenger(prio_messages_t& prio_messages,
                                     const messenger_key_t& key) {
      ushort seq = 0, id = 0;
      if (get_key_local_params(key, seq, id)) {
        messenger_ctxt_t* ctxt = ctxt_t::messenger_list().
         get(nfreelist_id_t(id));
        if (ctxt->info_.key_ == key) {
          process_remove_messenger(prio_messages, *ctxt);

          ctxt_t::messenger_lookup().erase(ctxt->info_.name_);
          ctxt_t::messenger_list().erase(nfreelist_id_t(id));
          return true;
        }
      }
      return false;
    }

    bool_t process_who_is(const messenger_key_t& key,
                          messenger_name_t& name, bool_t* group,
                          bool_t* local) {
      bool_t l = false, g = false;
      ushort_t seq = 0, id = 0;
      if (get_key_params(key, l, g, seq, id)) {
        if (local)
          *local = l;
        if (group)
          *group = g;
        if (l) {
          if (!g) {
            messenger_ctxt_t* ctxt = ctxt_t::messenger_list()
              .get(nfreelist_id_t(id));
            if (ctxt && ctxt->info_.key_ == key) {
              name = ctxt->info_.name_;
              return true;
            }
          } else {
            messenger_groupinfo_t* info = ctxt_t::messengergroup_list()
              .get(nfreelist_id_t(id));
            if (info && info->exist_ && info->key_ == key) {
              name = info->name_;
              return true;
            }
          }
        } else {
          messenger_lookup_t& lookup = ctxt_t::messenger_lookup();
          for (messenger_lookup_t::iterator i(lookup.begin());
               i != lookup.end(); ++i) {
            if (i->second == key) {
              name = i->first;
              return true;
            }
          }
        }
      }
      return false;
    }

////////////////////////////////////////////////////////////////////////////////

    messenger_name_t
      process_messenger_get_name(const messenger_key_t& key) {
      ushort seq = 0, id = 0;
      if (get_key_local_params(key, seq, id)) {
        messenger_ctxt_t* ctxt = ctxt_t::messenger_list().
         get(nfreelist_id_t(id));
        if (ctxt && ctxt->info_.key_ == key)
          return ctxt->info_.name_;
      }
      return messenger_name_t();
    }

    bool_t process_messenger_get_params(const messenger_key_t& key,
                                        messenger_params_t& params) {
      ushort seq = 0, id = 0;
      if (get_key_local_params(key, seq, id)) {
        messenger_ctxt_t* ctxt = ctxt_t::messenger_list().
         get(nfreelist_id_t(id));
        if (ctxt && ctxt->info_.key_ == key) {
          params = ctxt->info_.params_;
          messenger_grouplookup_t& glookup
             = ctxt_t::messenger_grouplookup();
          messenger_grouplookup_t::iterator i(glookup.find(
                                                ctxt->info_.name_));
          if (i != glookup.end())
            params.groups_ = i->second;
          else
            params.groups_.clear();
          return true;
        }
      }
      return false;
    }

    bool_t
      process_messenger_update_visibility(prio_messages_t& prio_messages,
                                          const messenger_key_t& key,
                                          messenger_visibility_t
                                            visibility) {
      // must still implement functionality
      return false;
    }

    bool_t
      process_messenger_update_alive_period(const messenger_key_t& key,
                                            multiple_of_100ms_t factor) {
      // will only set the value if it has not expired yet and when
      // setting the value will not lead to an immediate reset.
      ushort seq = 0, id = 0;
      if (get_key_local_params(key, seq, id)) {
        messenger_ctxt_t* ctxt = ctxt_t::messenger_list().
         get(nfreelist_id_t(id));
        if (ctxt && ctxt->info_.key_ == key) {
          if (!get(ctxt->alive_.max_) || ctxt->alive_.cnt_ < 100) {
            ctxt->alive_.max_ = factor;
            set(ctxt->alive_.current_) = 0;
            ctxt->alive_.cnt_ = 0;
            return true;
          }
        }
      }
      return false;
    }

    bool_t
      process_messenger_start_timer(const messenger_key_t& key,
                                    const messenger_timer_params_t&
                                      params) {
      ushort seq = 0, id = 0;
      if (get_key_local_params(key, seq, id)) {
        messenger_ctxt_t* ctxt = ctxt_t::messenger_list().
         get(nfreelist_id_t(id));
        if (ctxt && ctxt->info_.key_ == key) {
          ctxt->info_.params_.timer_params_ = params;

          // remember interface values must be checked
          timer_info_t& info = ctxt_t::timer_list()[key];
          info.max_             = 100000000LL * get(params.factor_);
          info.current_         = 0;
          info.start_timestamp_ = 0; // timestamp
          info.periodic_        = params.periodic_;
          info.prio_            = params.prio_;
          info.user_            = params.user_;
          return true;
        }
      }
      return false;
    }

    bool_t process_messenger_stop_timer(const messenger_key_t& key) {
      ushort seq = 0, id = 0;
      if (get_key_local_params(key, seq, id)) {
        messenger_ctxt_t* ctxt = ctxt_t::messenger_list().
         get(nfreelist_id_t(id));
        if (ctxt && ctxt->info_.key_ == key) {
          timerlist_t::iterator i(ctxt_t::timer_list().find(key));
          if (i != ctxt_t::timer_list().end()) {
            ctxt_t::timer_list().erase(i);
            return true;
          }
        }
      }
      return false;
    }

    bool_t process_messenger_query_timer(const messenger_key_t& key,
                                         messenger_timer_params_t& params) {
      ushort seq = 0, id = 0;
      if (get_key_local_params(key, seq, id)) {
        messenger_ctxt_t* ctxt = ctxt_t::messenger_list().
         get(nfreelist_id_t(id));
        if (ctxt && ctxt->info_.key_ == key) {
          params = ctxt->info_.params_.timer_params_;
          timerlist_t::iterator i(ctxt_t::timer_list().find(key));
          if (i != ctxt_t::timer_list().end())
            return true; // maybe add info
        }
      }
      return false;
    }

    bool_t process_messenger_add_to_group(prio_messages_t& prio_messages,
                                          const messenger_key_t& key,
                                          const password_t& password,
                                          const messenger_name_t& group,
                                          messenger_prio_t prio,
                                          messenger_user_t user) {
      ushort seq = 0, id = 0;
      if (get_key_local_params(key, seq, id)) {
        messenger_ctxt_t* ctxt = ctxt_t::messenger_list().
         get(nfreelist_id_t(id));
        if (ctxt && ctxt->info_.key_ == key)
          return process_add_messenger_to_group(prio_messages, password,
                                                ctxt->info_.name_, group,
                                                prio, user);
      }
      return false;
    }

    bool_t
      process_messenger_remove_from_group(const messenger_key_t& key,
                                          const password_t& password,
                                          const messenger_name_t& group,
                                          messenger_user_t* user) {
      ushort seq = 0, id = 0;
      if (get_key_local_params(key, seq, id)) {
        messenger_ctxt_t* ctxt = ctxt_t::messenger_list().
         get(nfreelist_id_t(id));
        if (ctxt && ctxt->info_.key_ == key)
          return process_remove_messenger_from_group(password,
                                                     ctxt->info_.name_,
                                                     group, user);
      }
      return false;
    }

    bool_t process_messenger_is_in_group(const messenger_key_t& key,
                                         const messenger_name_t& group,
                                         messenger_user_t* user) {
      ushort seq = 0, id = 0;
      if (get_key_local_params(key, seq, id)) {
        messenger_ctxt_t* ctxt = ctxt_t::messenger_list().
         get(nfreelist_id_t(id));
        if (ctxt && ctxt->info_.key_ == key) {
          messenger_grouplookup_t& lookup = ctxt_t::messenger_grouplookup();
          messenger_grouplookup_t::iterator i(lookup.find(ctxt->info_.
                                                            name_));
          if (i != lookup.end()) {
            messenger_memberlist_t::iterator p(i->second.find(group));
            if (p != i->second.end()) {
              if (user)
                *user = p->second.user_;
              return true;
            }
          }
        }
      }
      return false;
    }

    bool_t process_messenger_get_groups(const messenger_key_t& key,
                                        messenger_memberlist_t& list) {
      ushort seq = 0, id = 0;
      if (get_key_local_params(key, seq, id)) {
        messenger_ctxt_t* ctxt = ctxt_t::messenger_list().
         get(nfreelist_id_t(id));
        if (ctxt && ctxt->info_.key_ == key) {
          messenger_grouplookup_t& lookup = ctxt_t::messenger_grouplookup();
          messenger_grouplookup_t::iterator i(lookup.find(ctxt->info_.
                                                            name_));
          if (i != lookup.end()) {
            list = i->second;
            return !list.empty();
          }
        }
      }
      return false;
    }

    bool_t process_messenger_add_monitor(prio_messages_t& prio_messages,
                                         const messenger_key_t& key,
                                         const messenger_name_t& name,
                                         messenger_prio_t prio,
                                         messenger_user_t user) {
      ushort seq = 0, id = 0;
      if (get_key_local_params(key, seq, id)) {
        messenger_ctxt_t* ctxt = ctxt_t::messenger_list().
          get(nfreelist_id_t(id));
        if (ctxt && ctxt->info_.key_ == key) {
          messenger_memberlist_t& monitor = ctxt->info_.params_.monitor_;
          messenger_member_t params("", prio, user);
          std::pair<messenger_memberlist_t::iterator, bool> p(
            monitor.insert(
              messenger_memberlist_t::value_type(name, params)));
          if (p.second) {
            monitored_t& entry = ctxt_t::monitored_lookup()[name];
            if (entry.members_.empty()) { // we created it
              messenger_lookup_t::iterator p(
                ctxt_t::messenger_lookup().find(name));
              if (p != ctxt_t::messenger_lookup().end()) {
                bool_t local = false, group = false;
                ushort_t seq = 0, id = 0;
                get_key_params(p->second, local, group, seq, id);
                if (!group) {
                  if (!get(ctxt->alive_.max_) || ctxt->alive_.cnt_ < 100)
                    entry.state_ = messenger_state_available;
                  else
                    entry.state_ = messenger_state_available_unresponsive;
                  entry.key_ = p->second;
                } else {
                  messenger_groupinfo_t* info =
                    ctxt_t::messengergroup_list().get(nfreelist_id_t(id));
                  if (info && info->exist_) {
                    entry.key_ = p->second;
                    entry.state_ = messenger_state_available;
                  }
                }
              }
            }
            entry.members_.
              insert(memberlist_t::value_type(ctxt->info_.name_,
                                              member_t("", key, prio,
                                                       user)));
            if (get(entry.key_)) {
              prio_messages_t::value_type tmp(prio,
                                              prio_message_t(key, user));
              prio_messages_t::iterator j(prio_messages.insert(tmp));
              notify_message_t message;
              write_notify_message(message,
                                   key,
                                   entry.state_,
                                   name,
                                   entry.key_,
                                   prio,
                                   user);
              printf("state is = %d\n", (int)entry.state_);
              j->second.content_ = message.release();
            }
            return true;
          }
        }
      }
      return false;
    }

    bool_t process_messenger_remove_monitor(const messenger_key_t& key,
                                            const messenger_name_t& name,
                                            messenger_user_t* user) {
      ushort seq = 0, id = 0;
      if (get_key_local_params(key, seq, id)) {
        messenger_ctxt_t* ctxt = ctxt_t::messenger_list().
         get(nfreelist_id_t(id));
        if (ctxt && ctxt->info_.key_ == key) {
          messenger_memberlist_t& monitor = ctxt->info_.params_.monitor_;
          messenger_memberlist_t::iterator i(monitor.find(name));
          if (i != monitor.end()) {
            if (user)
              *user = i->second.user_;
            monitor.erase(i);
            monitored_lookup_t::iterator p(ctxt_t::monitored_lookup().find(
              name));
            if (p != ctxt_t::monitored_lookup().end()) {
              monitored_t& entry = p->second;
              entry.members_.erase(ctxt->info_.name_);
              return true;
            }
          }
        }
      }
      return false;
    }

    messenger_key_t
      process_messenger_is_monitored(const messenger_key_t& key,
                                     const messenger_name_t& name,
                                     messenger_user_t* user) {
      ushort seq = 0, id = 0;
      if (get_key_local_params(key, seq, id)) {
        messenger_ctxt_t* ctxt = ctxt_t::messenger_list().
         get(nfreelist_id_t(id));
        if (ctxt && ctxt->info_.key_ == key) {
          messenger_memberlist_t& monitor = ctxt->info_.params_.monitor_;
          messenger_memberlist_t::iterator i(monitor.find(name));
          if (i != monitor.end()) {
            if (user)
              *user = i->second.user_;
            monitored_lookup_t::iterator p(ctxt_t::monitored_lookup().find( 
              name));
            if (p != ctxt_t::monitored_lookup().end())
              return p->second.key_;
          }
        }
      }
      return messenger_key_t(0);
    }

    bool_t
      process_messenger_get_monitored(const messenger_key_t& key,
                                      messenger_memberkeylist_t& list) {
      ushort seq = 0, id = 0;
      if (get_key_local_params(key, seq, id)) {
        messenger_ctxt_t* ctxt = ctxt_t::messenger_list().
         get(nfreelist_id_t(id));
        if (ctxt && ctxt->info_.key_ == key) {
          messenger_memberlist_t& monitor = ctxt->info_.params_.monitor_;
          for (messenger_memberlist_t::iterator i(monitor.begin());
               i != monitor.end(); ++i) {
            messenger_keymember_t& entry = list[i->first];
            entry.params_ = i->second;

            monitored_lookup_t::iterator p(ctxt_t::monitored_lookup().find( 
              i->first));
            if (p != ctxt_t::monitored_lookup().end())
              entry.key_ = p->second.key_;
          }
        }
      }
      return false;
    }

////////////////////////////////////////////////////////////////////////////////

    bool_t command_engine_t
      ::process_command(prio_messages_t& prio_messages) {
      bool_t success = false;
      ullong_t id = 0;
      if (::read(get(fd_), &id, sizeof(id)) == sizeof(id)) {
        <% mutexguard_t guard(mutex_);
          switch (id) {
            case CALLID_BAD:
              break;

            case CALLID_MESSENGER_GET_NAME:
              call_messenger_get_name_.name_ =
                process_messenger_get_name
                  (*call_messenger_get_name_.key_);
              success = true;
              break;

            case CALLID_MESSENGER_GET_PARAMS:
              call_messenger_get_params_.result_ =
                process_messenger_get_params
                  (*call_messenger_get_params_.key_,
                   *call_messenger_get_params_.params_);
              success = true;
              break;

            case CALLID_MESSENGER_UPDATE_VISIBILITY:
              call_messenger_update_visibility_.result_ =
                process_messenger_update_visibility
                  (prio_messages,
                   *call_messenger_update_visibility_.key_,
                    call_messenger_update_visibility_.visibility_);
              success = true;
              break;

            case CALLID_MESSENGER_UPDATE_ALIVE_PERIOD:
              call_messenger_update_alive_period_.result_ =
                process_messenger_update_alive_period
                  (*call_messenger_update_alive_period_.key_,
                    call_messenger_update_alive_period_.factor_);
              success = true;
              break;

            case CALLID_MESSENGER_START_TIMER:
              call_messenger_start_timer_.result_ =
                process_messenger_start_timer(
                  *call_messenger_start_timer_.key_,
                  *call_messenger_start_timer_.params_);
              success = true;
              break;

            case CALLID_MESSENGER_STOP_TIMER:
              call_messenger_stop_timer_.result_ =
                process_messenger_stop_timer(
                  *call_messenger_stop_timer_.key_);
              success = true;
              break;

            case CALLID_MESSENGER_QUERY_TIMER:
              call_messenger_query_timer_.result_ =
                process_messenger_query_timer(
                  *call_messenger_query_timer_.key_,
                  *call_messenger_query_timer_.params_);
              success = true;
              break;

            case CALLID_MESSENGER_ADD_TO_GROUP:
              call_messenger_add_to_group_.result_ =
                process_messenger_add_to_group(
                  prio_messages,
                  *call_messenger_add_to_group_.key_,
                  *call_messenger_add_to_group_.password_,
                  *call_messenger_add_to_group_.name_,
                   call_messenger_add_to_group_.prio_,
                   call_messenger_add_to_group_.user_);
              success = true;
              break;

            case CALLID_MESSENGER_REMOVE_FROM_GROUP:
              call_messenger_remove_from_group_.result_ =
                process_messenger_remove_from_group(
                  *call_messenger_remove_from_group_.key_,
                  *call_messenger_remove_from_group_.password_,
                  *call_messenger_remove_from_group_.name_,
                   call_messenger_remove_from_group_.user_);
              success = true;
              break;

            case CALLID_MESSENGER_IS_IN_GROUP:
              call_messenger_is_in_group_.result_ =
                process_messenger_is_in_group(
                  *call_messenger_is_in_group_.key_,
                  *call_messenger_is_in_group_.name_,
                   call_messenger_is_in_group_.user_);
              success = true;
              break;

            case CALLID_MESSENGER_GET_GROUPS:
              call_messenger_get_groups_.result_ =
                 process_messenger_get_groups(
                   *call_messenger_get_groups_.key_,
                   *call_messenger_get_groups_.list_);
              success = true;
              break;

            case CALLID_MESSENGER_ADD_MONITOR:
              call_messenger_add_monitor_.result_ =
                process_messenger_add_monitor(
                  prio_messages,
                  *call_messenger_add_monitor_.key_,
                  *call_messenger_add_monitor_.name_,
                   call_messenger_add_monitor_.prio_,
                   call_messenger_add_monitor_.user_);
              success = true;
              break;

            case CALLID_MESSENGER_REMOVE_MONITOR:
              call_messenger_remove_monitor_.result_ =
                process_messenger_remove_monitor(
                  *call_messenger_remove_monitor_.key_,
                  *call_messenger_remove_monitor_.name_,
                   call_messenger_remove_monitor_.user_);
              success = true;
              break;

            case CALLID_MESSENGER_IS_MONITORED:
              call_messenger_is_monitored_.namekey_ =
                process_messenger_is_monitored(
                  *call_messenger_is_monitored_.key_,
                  *call_messenger_is_monitored_.name_,
                   call_messenger_is_monitored_.user_);
              success = true;
              break;

            case CALLID_MESSENGER_GET_MONITORED:
              call_messenger_get_monitored_.result_ =
                process_messenger_get_monitored(
                  *call_messenger_get_monitored_.key_,
                  *call_messenger_get_monitored_.list_);
              success = true;
              break;

            case CALLID_UPDATE_PARAMS:
              process_update_params(
                prio_messages,
                *call_update_params_.params_);
              success = true;
              break;

            case CALLID_FETCH_PARAMS:
              process_fetch_params(
                *call_fetch_params_.params_);
              success = true;
              break;

            case CALLID_CREATE_GROUP:
              call_create_group_.result_ =
                process_create_group(
                  prio_messages,
                  *call_create_group_.password_,
                  *call_create_group_.name_,
                   call_create_group_.visibility_);
              success = true;
              break;

            case CALLID_DESTROY_GROUP:
              call_destroy_group_.result_ =
                process_destroy_group(
                  prio_messages,
                  *call_destroy_group_.password_,
                  *call_destroy_group_.name_);
              success = true;
              break;

            case CALLID_FETCH_GROUP:
              call_fetch_group_.result_ =
                process_fetch_group(
                  *call_fetch_group_.name_,
                  *call_fetch_group_.visibility_,
                   call_fetch_group_.list_);
              success = true;
              break;

            case CALLID_ADD_MESSENGER_TO_GROUP:
              call_add_messenger_to_group_.result_ =
                process_add_messenger_to_group(
                  prio_messages,
                  *call_add_messenger_to_group_.password_,
                  *call_add_messenger_to_group_.name_,
                  *call_add_messenger_to_group_.group_,
                   call_add_messenger_to_group_.prio_,
                   call_add_messenger_to_group_.user_);
              success = true;
              break;

            case CALLID_REMOVE_MESSENGER_FROM_GROUP:
              call_remove_messenger_from_group_.result_ =
                process_remove_messenger_from_group(
                  *call_remove_messenger_from_group_.password_,
                  *call_remove_messenger_from_group_.name_,
                  *call_remove_messenger_from_group_.group_,
                   call_remove_messenger_from_group_.user_);
              success = true;
              break;

            case CALLID_IS_MESSENGER_IN_GROUP:
              call_is_messenger_in_group_.result_ =
                process_is_messenger_in_group(
                  *call_is_messenger_in_group_.name_,
                  *call_is_messenger_in_group_.group_,
                   call_is_messenger_in_group_.user_);
              success = true;
              break;

            case CALLID_FETCH_MESSENGER_GROUPS:
              call_fetch_messenger_groups_.result_ =
                process_fetch_messenger_groups(
                  *call_fetch_messenger_groups_.name_,
                  *call_fetch_messenger_groups_.list_);
              success = true;
              break;

            case CALLID_CREATE_MESSENGER:
              call_create_messenger_.id_ =
                process_create_messenger(
                  prio_messages,
                  *call_create_messenger_.errorid_,
                  *call_create_messenger_.name_,
                  *call_create_messenger_.params_);
              success = true;
              break;

            case CALLID_FETCH_MESSENGER:
              call_fetch_messenger_.result_ =
                process_fetch_messenger(
                  *call_fetch_messenger_.name_,
                  *call_fetch_messenger_.params_);
              success = true;
              break;

            case CALLID_FETCHCLEAR_MESSENGER:
              call_fetchclear_messenger_.result_ =
                process_fetch_messenger(
                  *call_fetchclear_messenger_.name_,
                  *call_fetchclear_messenger_.info_,
                   call_fetchclear_messenger_.clearstats_);
              success = true;
              break;

            case CALLID_FETCH_MESSENGERS:
              call_fetch_messengers_.result_ =
                process_fetch_messengers(
                  *call_fetch_messengers_.infos_,
                   call_fetch_messengers_.clearstats_);
              success = true;
              break;

            case CALLID_DESTROY_MESSENGER:
              call_destroy_messenger_.result_ =
                process_destroy_messenger(
                  prio_messages,
                  *call_destroy_messenger_.key_);
              success = true;
              break;

            case CALLID_WHO_IS:
              call_who_is_.result_ =
                process_who_is(*call_who_is_.key_,
                               *call_who_is_.name_,
                                call_who_is_.group_,
                                call_who_is_.local_);
              success = true;
              break;

            default:
              break;
              // programming error
          }
          done_   = true;
        %>
      } else { // failed  - but must unblock calling thread
        <% mutexguard_t guard(mutex_);
          done_   = true;
        %>
      }
      cond_.signal();
      return success;
    }

////////////////////////////////////////////////////////////////////////////////

    void_t dispatch_prio_messages(prio_messages_t& prio_messages) {
      for (prio_messages_t::iterator i = prio_messages.begin();
           i != prio_messages.end(); ++i) {
        bool_t local = false, group = false;
        ushort_t seq = 0, id = 0;
        get_key_params(i->second.key_, local, group, seq, id);
        if (local) {
          messenger_ctxt_t* ctxt = ctxt_t::messenger_list()
            .get(nfreelist_id_t(id));
          if (ctxt && ctxt->info_.key_ == i->second.key_) {
            queueitem_t* item = fd_queue_t::create_item<bytebuf_t>();
            if (item) {
              item->value() = i->second.content_;

              TRACE_0(tr_src(), trace::level_debug,
                      ("message: -> %s", ctxt->info_.name_.c_str()));

              if (!ctxt->queue_.insert(item)) {
                fd_queue_t::destroy(item);
                TRACE_0(tr_src(), trace::level_warning,
                        ("message: -> %s failed", ctxt->info_.name_.c_str()));
              }
            } else {
            }
          }
        } else {
          // build all remotes then send once the function is done
        }
      }
    }

    void_t update_prio_messages(prio_messages_t& prio_messages,
                                const messenger_key_t& key,
                                messenger_prio_t prio,
                                message_t& message) {
      bool_t local = false, group = false;
      ushort_t seq = 0, id = 0;
      get_key_params(key, local, group, seq, id);
      if (local && group) {
        messenger_groupinfo_t* info = ctxt_t::messengergroup_list()
          .get(nfreelist_id_t(id));
        if (info && info->exist_ && info->key_ == key) {
          uint_t n = info->members_.size();
          for (memberlist_t::iterator j = info->members_.begin();
               j != info->members_.end(); ++j) {
            message_t tmp(--n ? message.clone() : message.release());
            update_prio_messages(prio_messages, j->second.key_,
                                 j->second.prio_, tmp);
          }
        } else {
          // create failed message, back to the source.
        }
      } else {
        // detect if key messenger is reachable
        prio_messages_t::value_type tmp(prio, key);
        prio_messages_t::iterator i(prio_messages.insert(tmp));
        i->second.content_ = message.release();
      }
    }

    void_t handle_local_user_messages(prio_messages_t& prio_messages) {
      for (queueitem_t* item = ctxt_t::queue().remove<bytebuf_t>();
           item; ) { /** optimze to use queueitems later **/

        message_t message(item->value().release());
        message_id_t id(read_id(message));
        switch (get(id.domain_)) {
          case 0: {
            switch (get(id.user_)) {
              case message_type_alive: {
                messenger_key_t key(read_dst(message));
                bool_t local = false, group = false;
                ushort_t seq = 0, id = 0;
                if (get_key_params(key, local, group, seq, id) &&
                    local && !group) {
                  messenger_ctxt_t* ctxt = ctxt_t::messenger_list()
                    .get(nfreelist_id_t(id));
                  if (ctxt && ctxt->info_.key_ == key) {
                    if (ctxt->alive_.cnt_ == 100) {
                      monitored_lookup_t::iterator m(
                        ctxt_t::monitored_lookup().find(ctxt->info_.name_));
                      if (m != ctxt_t::monitored_lookup().end()) {
                        m->second.state_ = messenger_state_available;
                        update_prio_messages(prio_messages, m->first,
                                             m->second);
                      }

                      if (get(ctxt_t::alivegrpkey())) {
                        ushort_t seq = 0, id = 0;
                        if (get_key_local_params(key, seq, id)) {
                          messenger_groupinfo_t* info =
                            ctxt_t::messengergroup_list().get(
                              nfreelist_id_t(id));
                          update_prio_messages(prio_messages, info->name_,
                                               messenger_state_available,
                                               key, *info);
                        }
                      }
                    }
                    ctxt->alive_.cnt_ = 0;
                    set(ctxt->alive_.current_) = 0;
                  }
                }
              } break;

              default: { // bad
                printf("received unexpected message\n");
                break;
              }
            }
          } break;

          default: {
            TRACE_0(tr_src(), trace::level_debug,
                    ("user(%d) message: -> %llu", get(id.user_),
                    get(read_dst(message))));
            update_prio_messages(prio_messages, read_dst(message),
                                 messenger_prio_t(0), message);
          } break;
        }

        queueitem_t* next = item->next();
        fd_queue_t::destroy(item);
        item = next;
      }
    }

    void_t handle_timeouts(prio_messages_t& prio_messages) {
      // get timestamp
      ullong_t expiries = 0;
      if (::read(get(ctxt_t::timer_fd()), &expiries, sizeof(expiries)) ==
          sizeof(expiries)) {
        // determine skew - recalculate
        if (expiries >= 1) {
          for (timerlist_t::iterator i(ctxt_t::timer_list().begin());
               i != ctxt_t::timer_list().end(); /* erase in table */) {
            timer_info_t& info = i->second;
            info.current_ += 100000000;
            if (info.current_ > info.max_) {
              timeout_message_t message;
              multiple_of_100ms_t factor(info.max_/100000000);
              if (write_timeout_message(message,
                                        info.periodic_,
                                        factor,
                                        i->first,
                                        info.prio_,
                                        info.user_)) {
                prio_messages_t::value_type tmp(info.prio_, i->first);
                prio_messages_t::iterator p(prio_messages.insert(tmp));
                p->second.content_ = message.release();
              }

              if (info.periodic_) {
                info.current_ = 0;
                ++i;
              } else
                ctxt_t::timer_list().erase(i++);
            } else
              ++i;
          }

          uint_t n = ctxt_t::messenger_list().size();
          for (nfreelist_id_t::native_t id = 0;
               n && id < ctxt_t::messenger_list().capacity(); ++id, --n) {
            messenger_ctxt_t* ctxt = ctxt_t::messenger_list()
              .get(nfreelist_id_t(id));
            if (ctxt) {
              alive_info_t& alive = ctxt->alive_;
              if (get(alive.max_)) {
                ++set(alive.current_);
                if (get(alive.current_) >= get(alive.max_) &&
                    alive.cnt_ < 100) {
                  ++alive.cnt_;
                  if ((alive.cnt_ % 10) == 0) {
                    if (alive.cnt_ != 100) {
                      alive_message_t message;
                      if (write_alive_message(message,
                                              ctxt->info_.key_)) {
                        prio_messages_t::value_type tmp(messenger_prio_t(0),
                                                        ctxt->info_.key_);
                        prio_messages_t::iterator p(
                          prio_messages.insert(tmp));
                        p->second.content_ = message.release();
                      } else {
                      }
                    } else {
                      monitored_lookup_t::iterator m(
                        ctxt_t::monitored_lookup().find(ctxt->info_.name_));
                      if (m != ctxt_t::monitored_lookup().end()) {
                        m->second.state_ =
                          messenger_state_available_unresponsive;
                        update_prio_messages(prio_messages, m->first,
                                             m->second);
                      }

                      if (get(ctxt_t::alivegrpkey())) {
                        ushort_t seq = 0, id = 0;
                        if (get_key_local_params(ctxt->info_.key_, seq,
                                                 id)) {
                          messenger_groupinfo_t* info =
                            ctxt_t::messengergroup_list().get(
                              nfreelist_id_t(id));
                          update_prio_messages(
                            prio_messages,
                            ctxt->info_.name_,
                            messenger_state_available_unresponsive,
                            ctxt->info_.key_, *info);
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        } else
          TRACE_0(tr_src(), trace::level_debug, ("oops"));
      }
    }

////////////////////////////////////////////////////////////////////////////////

    void_t event_loop() {
      while (1) {
        prio_messages_t prio_messages;
        epoll_event     events[10];

        int n = epoll_wait(get(ctxt_t::epoll_fd()), events, 10, -1);

        TRACE_0(tr_src(), trace::level_debug, ("epoll event"));

        if (n == -1) {
          perror("epoll returned -1");
          continue;
        }

        bool cmd_ev        = false,
             tipctopo_ev   = false,
             timeout_ev    = false,
             tipclocal_ev  = false,
             tipcremote_ev = false,
             user_ev       = false;

        for (int i = 0; i < n; ++i) {
          epoll_event& ev = events[i];
          switch (ev.data.u32) {
            case FD_CMD_EVENT:        cmd_ev        = true; break;
            case FD_TIMEOUT_EVENT:    timeout_ev    = true; break;
            case FD_TIPCTOPO_EVENT:   tipctopo_ev   = true; break;
            case FD_TIPCLOCAL_EVENT:  tipclocal_ev  = true; break;
            case FD_TIPCREMOTE_EVENT: tipcremote_ev = true; break;
            case FD_USER_EVENT:       user_ev       = true; break;
          }
        }

        if (cmd_ev) {
          if (!ctxt_t::cmd_engine().process_command(prio_messages)) {
            // something internal went wrong
            // update prio_messages
          }
        }

        if (tipctopo_ev) {
          // update tables
          // update_prio_messages
        }

        if (timeout_ev) {
          handle_timeouts(prio_messages);
        }

        if (tipclocal_ev) {
          // relay message
          // update_prio_messages
        }

        if (tipcremote_ev) {
          // relay message
          // update_prio_messages
        }

        if (user_ev) {
          handle_local_user_messages(prio_messages);
        }

        if (!prio_messages.empty())
          dispatch_prio_messages(prio_messages);
      }
    }

////////////////////////////////////////////////////////////////////////////////

    bool_t threadhandler_t::prepare() {
      epoll_event cmd_event;
      cmd_event.events   = EPOLLIN;
      cmd_event.data.u32 = FD_CMD_EVENT;

      epoll_event timer_event;
      timer_event.events   = EPOLLIN;
      timer_event.data.u32 = FD_TIMEOUT_EVENT;

      epoll_event user_event;
      user_event.events   = EPOLLIN;
      user_event.data.u32 = FD_USER_EVENT;

      itimerspec timespecs;
      timespecs.it_interval.tv_sec  = 0;
      timespecs.it_interval.tv_nsec = 100000000;
      timespecs.it_value.tv_sec     = 0;
      timespecs.it_value.tv_nsec    = 100000000;

      if ((::epoll_ctl(get(ctxt_t::epoll_fd()), EPOLL_CTL_ADD,
                       get(ctxt_t::cmd_engine().get_fd()),
                       &cmd_event) == 0) &&
          (::epoll_ctl(get(ctxt_t::epoll_fd()), EPOLL_CTL_ADD,
                       get(ctxt_t::timer_fd()),
                       &timer_event) == 0) &&
          (::epoll_ctl(get(ctxt_t::epoll_fd()), EPOLL_CTL_ADD,
                       get(ctxt_t::queue().get_fd()),
                       &user_event) == 0) &&
          (timerfd_settime(get(ctxt_t::timer_fd()), 0,
                          &timespecs, 0) == 0)) {
        ctxt_t::init() = true;
        return true;
      }
      return false;
    }

    void_t threadhandler_t::process() {
      event_loop();
    }
  }

////////////////////////////////////////////////////////////////////////////////

  messenger_create_params_t default_messenger_create_params() {
    return messenger_create_params_t(messenger_visibility_process,
                                     multiple_of_100ms_t(0));
  }

////////////////////////////////////////////////////////////////////////////////

  bool_t check_timer_params(const messenger_timer_params_t& params) {
    return get(params.factor_);
  }

  bool_t check_alive_period(multiple_of_100ms_t factor) {
    // zero could be valid - disabled checking
    // but a huge value might be invalid
    return true;
  }

  bool_t check_name(const messenger_name_t& name) {
    // might not allow certain characters
    return name.length() >= 4;
  }

  bool_t check_password(const password_t&) {
    // might enforce a policy
    return true;
  }

  bool_t check_params(const params_t&) {
    // check alias name
    return true;
  }

  bool_t check_params(const messenger_create_params_t& params) {
    // check alive
    // check timer
    return true;
  }

////////////////////////////////////////////////////////////////////////////////

  messenger_name_t messenger_owner_t::get_name() const {
    return id_ ?  ctxt_t::cmd_engine().
                    call_messenger_get_name(id_.get_key())
               : messenger_name_t();
  }

  bool_t messenger_owner_t::get_params(messenger_params_t& params) const {
    return id_ ? ctxt_t::cmd_engine().
                   call_messenger_get_params(id_.get_key(), params)
               : false;
  }

  bool_t messenger_owner_t::update_visibility(visibility_t visibility) {
    return id_ ? ctxt_t::cmd_engine().
                   call_messenger_update_visibility(id_.get_key(),
                                                    visibility)
               : false;
  }

  bool_t
    messenger_owner_t::update_alive_period(multiple_of_100ms_t factor) {
    return (id_ && check_alive_period(factor))
             ? ctxt_t::cmd_engine().
                 call_messenger_update_alive_period(id_.get_key(), factor)
             : false;
  }

  bool_t messenger_owner_t::post_message(const messenger_key_t& key,
                                         message_t& message) const {
    if (id_ && message && read_len(message) <= message.capacity()) {
      message_id_t id(read_id(message));
      if ((get(key) || (get(id.domain_) == get(service_domain) &&
                        get(id.user_)   == message_type_alive)) &&
          write_message_header(message, key, id_.get_key())) {
        queueitem_t* item = fd_queue_t::create_item<bytebuf_t>();
        if (item) {
          item->value() = message.release();
          if (ctxt_t::queue().insert(item))
            return true;
        }
        fd_queue_t::destroy(item);
      }
    }
    return false;
  }

  bool_t messenger_owner_t::wait_message(messages_t& messages) const {
    key_t key = id_.get_key();
    ushort_t id = 0, seq = 0;
    if (get_key_local_params(key, seq, id)) {
      <% // add lock to owner to protect when called from n-threads
        messenger_ctxt_t* ctxt =
          ctxt_t::messenger_list().get(nfreelist_id_t(id));
        if (ctxt && ctxt->info_.key_ == key) {
          uint_t size = 0;
          queueitem_t* head = ctxt->queue_.remove<bytebuf_t>(&size);
          if (head) {
            messages.clear_resize(size);
            while (head) {
              queueitem_t* item = head;
              message_t* msg = messages.push_back();
              *msg = item->value().release();
              head = head->next();
              fd_queue_t::destroy(item);
            }
            return true;
          }
        }
      %>
    }
    return false;
  }

  bool_t messenger_owner_t::check_message(messages_t& messages) const {
    key_t key = id_.get_key();
    ushort_t id = 0, seq = 0;
    if (get_key_local_params(key, seq, id)) {
      <% // add lock to owner to protect when called from n-threads
        messenger_ctxt_t* ctxt =
          ctxt_t::messenger_list().get(nfreelist_id_t(id));
        if (ctxt && ctxt->info_.key_ == key) {
          if (ctxt->queue_.size()) {
            uint_t size = 0;
            queueitem_t* head = ctxt->queue_.remove<bytebuf_t>(&size);
            if (head) {
              messages.clear_resize(size);
              while (head) {
                queueitem_t* item = head;
                message_t* msg = messages.push_back();
                *msg = item->value().release();
                head = head->next();
                fd_queue_t::destroy(item);
              }
              return true;
            }
          }
        }
      %>
    }
    return false;
  }

  bool_t messenger_owner_t
    ::start_timer(const messenger_timer_params_t& params) {
    return (id_ && check_timer_params(params))
             ? ctxt_t::cmd_engine().
                 call_messenger_start_timer(id_.get_key(), params)
             : false;
  }

  bool_t messenger_owner_t::stop_timer() {
    return id_ ? ctxt_t::cmd_engine().
                   call_messenger_stop_timer(id_.get_key())
               : false;
  }

  bool_t messenger_owner_t
    ::query_timer(messenger_timer_params_t& params) const {
    return id_ ? ctxt_t::cmd_engine().
                   call_messenger_query_timer(id_.get_key(), params)
               : false;
  }

  bool_t messenger_owner_t::add_to_group(const password_t& password,
                                         const messenger_name_t& name,
                                         messenger_prio_t prio,
                                         messenger_user_t user) {
    return (id_ && check_name(name))
             ? ctxt_t::cmd_engine().
                 call_messenger_add_to_group(id_.get_key(), password,
                                             name, prio, user)
             : false;
  }

  bool_t messenger_owner_t
    ::remove_from_group(const password_t& password,
                        const messenger_name_t& name,
                        messenger_user_t* user) {
    return (id_ && check_name(name))
             ? ctxt_t::cmd_engine().
                 call_messenger_remove_from_group(id_.get_key(), password,
                                                  name, user)
             : false;
  }

  bool_t messenger_owner_t::is_in_group(const messenger_name_t& name,
                                        messenger_user_t* user) const {
    return (id_ && check_name(name))
             ? ctxt_t::cmd_engine().
                 call_messenger_is_in_group(id_.get_key(), name, user)
             : false;
  }

  bool_t messenger_owner_t::get_groups(messenger_memberlist_t& list) const {
    return id_ ? ctxt_t::cmd_engine().
                   call_messenger_get_groups(id_.get_key(), list)
               : false;
  }

  bool_t messenger_owner_t::add_monitor(const messenger_name_t& name,
                                        messenger_prio_t prio,
                                        messenger_user_t user) {
    return (id_ && check_name(name))
             ? ctxt_t::cmd_engine().
                 call_messenger_add_monitor(id_.get_key(), name, prio,
                                            user)
             : false;
  }

  bool_t messenger_owner_t::remove_monitor(const messenger_name_t& name,
                                           messenger_user_t* user) {
    return (id_ && check_name(name))
             ? ctxt_t::cmd_engine().
                 call_messenger_remove_monitor(id_.get_key(), name, user)
             : false;
  }

  messenger_key_t
    messenger_owner_t::is_monitored(const messenger_name_t& name,
                                    messenger_user_t* user) const {
    return (id_ && check_name(name))
             ? ctxt_t::cmd_engine().
                 call_messenger_is_monitored(id_.get_key(), name, user)
             : messenger_key_t(0);
  }

  bool_t messenger_owner_t
    ::get_monitored(messenger_memberkeylist_t& list) const {
    return id_ ? ctxt_t::cmd_engine().
                   call_messenger_get_monitored(id_.get_key(), list)
               : false;
  }

////////////////////////////////////////////////////////////////////////////////

  messenger_owner_t
    ::messenger_owner_t(const messenger_owner_t& owner)
     : id_(owner.id_.release()) {
  }

  messenger_owner_t&
    messenger_owner_t::operator=(const messenger_owner_t& owner) {
    if (id_)
      destroy_messenger(*this);
    id_ = owner.id_.release();
    return *this;
  }

  messenger_owner_t::~messenger_owner_t() {
    if (id_)
      destroy_messenger(*this);
  }

////////////////////////////////////////////////////////////////////////////////

  bool_t is_running() {
    return ctxt_t::init() && !ctxt_t::errorid();
  }

  void_t update_params(const params_t& params) {
    if (is_running() && check_params(params))
      ctxt_t::cmd_engine().call_update_params(params);
  }

  void_t fetch_params(params_t& params) {
    if (is_running())
      ctxt_t::cmd_engine().call_fetch_params(params);
  }

  bool_t create_group(const password_t& password,
                      const messenger_name_t& name,
                      messenger_visibility_t visibility) {
    return (is_running() && check_name(name) && check_password(password))
             ? ctxt_t::cmd_engine().
                 call_create_group(password, name, visibility)
      : false;
  }

  bool_t destroy_group(const password_t& password,
                       const messenger_name_t& name) {
    return (is_running() && check_name(name) && check_password(password))
             ? ctxt_t::cmd_engine().call_destroy_group(password, name)
             : false;
  }

  bool_t fetch_group(const messenger_name_t& name,
                     messenger_visibility_t& visibility,
                     messenger_memberlist_t* list) {
    return (is_running() && check_name(name))
             ? ctxt_t::cmd_engine().
                 call_fetch_group(name, visibility, list)
             : false;
  }

  bool_t add_messenger_to_group(const password_t& password,
                                const messenger_name_t& name,
                                const messenger_name_t& group,
                                messenger_prio_t prio,
                                messenger_user_t user) {
    return (is_running() && check_name(name) && check_name(group) &&
            name != group)
             ? ctxt_t::cmd_engine().
                 call_add_messenger_to_group(password, name, group, prio,
                                             user)
             : false;
  }

  bool_t remove_messenger_from_group(const password_t& password,
                                     const messenger_name_t& name,
                                     const messenger_name_t& group,
                                     messenger_user_t* user) {
    return (is_running() && check_name(name) && check_name(group) &&
            name != group)
             ? ctxt_t::cmd_engine().
                 call_remove_messenger_from_group(password, name, group,
                                                  user)
             : false;
  }

  bool_t is_messenger_in_group(const messenger_name_t& name,
                               const messenger_name_t& group,
                               messenger_user_t* user) {
    return (is_running() && check_name(name) && check_name(group) &&
            name != group)
             ? ctxt_t::cmd_engine().
                 call_is_messenger_in_group(name, group, user)
             : false;
  }

  bool_t fetch_messenger_groups(const messenger_name_t& name,
                                messenger_memberlist_t& list) {
    return (is_running() && check_name(name))
             ? ctxt_t::cmd_engine().call_fetch_messenger_groups(name, list)
             : false;
  }

  messenger_owner_t
    create_messenger(const messenger_name_t& name,
                     const messenger_create_params_t& params) {
    errorid_t errorid = 0;
    return create_messenger(errorid, name, params);
  }

  messenger_owner_t
    create_messenger(errorid_t& errorid,
                     const messenger_name_t& name,
                     const messenger_create_params_t& params) {
    return (!errorid && is_running() && check_name(name) &&
            check_params(params))
             ? messenger_owner_t(ctxt_t::cmd_engine().
                                   call_create_messenger(errorid, name,
                                                         params))
             : messenger_owner_t();
  }

  bool_t fetch_messenger(const messenger_name_t& name,
                         messenger_params_t& params) {
    return (is_running() && check_name(name))
             ? ctxt_t::cmd_engine().call_fetch_messenger(name, params)
             : false;
  }

  bool_t fetch_messenger(const messenger_name_t& name,
                         messenger_info_t& info,
                         bool_t clearstats) {
    return (is_running() && check_name(name))
             ? ctxt_t::cmd_engine().
                 call_fetch_messenger(name, info, clearstats)
             : false;
  }

  bool_t fetch_messengers(messenger_infos_t& infos, bool_t clearstats) {
    return is_running() ? ctxt_t::cmd_engine().
                            call_fetch_messengers(infos, clearstats)
                        : false;
  }

  bool_t destroy_messenger(messenger_owner_t owner) {
    return (is_running() && owner)
             ? ctxt_t::cmd_engine().
                 call_destroy_messenger(owner.id_.release().get_key())
             : false;
  }

  bool_t who_is(const messenger_key_t& key, messenger_name_t& name,
                bool_t* group, bool_t* local) {
    return (is_running() && get(key) && check_name(name))
             ? ctxt_t::cmd_engine().call_who_is(key, name, group, local)
             : false;
  }

  bool_t post_message(const messenger_key_t& key, message_t& message) {
    if (message && read_len(message) <= message.capacity()) {
      message_id_t id(read_id(message));
      if ((get(key) || (get(id.domain_) == get(service_domain) &&
                        get(id.user_)   == message_type_alive)) &&
          write_message_header(message, key, empty_key)) {
        queueitem_t* item = fd_queue_t::create_item<bytebuf_t>();
        if (item) {
          item->value() = message.release();
          if (ctxt_t::queue().insert(item))
            return true;
        }
        fd_queue_t::destroy(item);
      }
    }
    return false;
  }

////////////////////////////////////////////////////////////////////////////////
}
}
