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

#ifndef _DAINTY_MESSAGING_MESSAGE_H_
#define _DAINTY_MESSAGING_MESSAGE_H_

#include "dainty_base.h"
#include "dainty_base_string.h"
#include "dainty_container_bytebuf.h"

namespace dainty
{
namespace messaging
{
namespace message
{
///////////////////////////////////////////////////////////////////////////////

  using base::types::t_prefix;
  using base::types::t_uint16;
  using base::types::t_uint64;
  using base::types::t_uchar;
  using base::types::t_bool;
  using base::types::r_bool;
  using base::types::t_n_;

  using base::specific::t_specific;
  using base::specific::t_n;
  using base::specific::r_n;
  using base::specific::t_ix;
  using base::specific::t_validity;
  using base::specific::VALID;
  using base::specific::INVALID;

  using base::string::t_string;

  using base::t_multiple;

  using container::bytebuf::t_view;
  using container::bytebuf::t_cview;

///////////////////////////////////////////////////////////////////////////////

  using p_byte = container::bytebuf::p_byte_;
  using P_byte = container::bytebuf::P_byte_;

///////////////////////////////////////////////////////////////////////////////

  enum  t_bytebuf_tag_ { };
  using t_bytebuf = container::bytebuf::t_bytebuf<t_bytebuf_tag_, 0>;
  using R_bytebuf = t_prefix<t_bytebuf>::R_;

  enum t_messenger_state {
    STATE_AVAILABLE,
    STATE_AVAILABLE_UNRESPONSIVE,
    STATE_UNAVAILABLE,
  };
  using r_messenger_state = t_prefix<t_messenger_state>::r_;

///////////////////////////////////////////////////////////////////////////////

  enum  t_messenger_user_tag_ {};
  using t_messenger_user = base::t_user<t_messenger_user_tag_>;
  using r_messenger_user = t_prefix<t_messenger_user>::r_;

  enum  t_messenger_key_tag_ {};
  using t_messenger_key_ = t_uint64;
  using t_messenger_key  = t_specific<t_messenger_key_, t_messenger_key_tag_>;
  using r_messenger_key  = t_prefix<t_messenger_key>::r_;
  using R_messenger_key  = t_prefix<t_messenger_key>::R_;

  enum  t_messenger_name_tag_ {};
  using t_messenger_name = t_string<t_messenger_name_tag_, 32>;
  using r_messenger_name = t_prefix<t_messenger_name>::r_;

  enum  t_messenger_prio_tag_ {};
  using t_messenger_prio_ = t_uint16;
  using t_messenger_prio  = t_specific<t_messenger_prio_, t_messenger_prio_tag_>;
  using r_messenger_prio  = t_prefix<t_messenger_prio>::r_;

  enum  t_multiple_of_100ms_tag_ {};
  using t_multiple_of_100ms = t_multiple<100, t_multiple_of_100ms_tag_>;
  using r_multiple_of_100ms = t_prefix<t_multiple_of_100ms>::r_;

  enum  t_domain_tag_ {};
  using t_domain_ = t_uchar;
  using t_domain  = t_specific<t_domain_, t_domain_tag_>;

  enum  t_user_tag_ {};
  using t_user_ = t_uchar;
  using t_user  = t_specific<t_user_, t_user_tag_>;

  enum  t_version_tag_ {};
  using t_version_ = t_uchar;
  using t_version  = t_specific<t_version_, t_version_tag_>;

///////////////////////////////////////////////////////////////////////////////

  constexpr t_domain  MESSAGING        {0};
  constexpr t_user    MESSAGING_NOTIFY {1};
  constexpr t_user    MESSAGING_TIMEOUT{2};
  constexpr t_user    MESSAGING_ALIVE  {3};
  constexpr t_user    MESSAGING_FAILED {4};
  constexpr t_version V1{1};

///////////////////////////////////////////////////////////////////////////////

  class t_id {
  public:
    constexpr
    t_id(t_domain _domain, t_user _user, t_version _version)
      : domain(_domain), user(_user), version(_version) {
    }

    t_domain  domain;  // zero is reserved for system
    t_user    user;    // packet type
    t_version version; // version of the same packet type
  };
  using r_id = t_prefix<t_id>::r_;
  using R_id = t_prefix<t_id>::R_;

  constexpr t_id NOTIFY_MSG_ID {MESSAGING, MESSAGING_NOTIFY,  V1};
  constexpr t_id TIMEOUT_MSG_ID{MESSAGING, MESSAGING_TIMEOUT, V1};
  constexpr t_id ALIVE_MSG_ID  {MESSAGING, MESSAGING_ALIVE,   V1};
  constexpr t_id FAILED_MSG_ID {MESSAGING, MESSAGING_FAILED,  V1};

  constexpr
  t_bool operator==(R_id lh, R_id rh) {
    return lh.domain == rh.domain && lh.user == rh.user;
  }

  constexpr
  t_bool operator!=(R_id lh, R_id rh) {
    return lh.domain != rh.domain || lh.user != rh.user;
  }

///////////////////////////////////////////////////////////////////////////////

  class t_message;
  using r_message = t_prefix<t_message>::r_;
  using x_message = t_prefix<t_message>::x_;
  using R_message = t_prefix<t_message>::R_;

  class t_message {
  public:
    using t_key     = t_messenger_key;
    using r_key     = r_messenger_key;
    using t_n       = message::t_n;
    using r_n       = message::r_n;
    using t_uint16  = message::t_uint16;
    using t_id      = message::t_id;
    using t_domain  = message::t_domain;
    using t_user    = message::t_user;
    using t_version = message::t_version;

    t_message();
    t_message(t_n);
    t_message(x_message);
    t_message(R_message);

    r_message operator=(x_message);
    r_message operator=(R_message);

    t_validity set(R_id, t_n len, t_uint16 cnt = 0);
    t_validity get(r_id       id,
                   r_n        len,
                   r_key      dst,
                   r_key      src,
                   t_uint16&  cnt);

    operator t_validity() const;
    t_n get_capacity() const;

    p_byte  data();
    P_byte  data() const;
    P_byte cdata() const;

    t_view  mk_view ();
    t_cview mk_view () const;
    t_cview mk_cview() const;

    t_view  mk_view (t_ix begin);
    t_cview mk_view (t_ix begin) const;
    t_cview mk_cview(t_ix begin) const;

    t_view  mk_view (t_ix begin, t_ix end);
    t_cview mk_view (t_ix begin, t_ix end) const;
    t_cview mk_cview(t_ix begin, t_ix end) const;

    t_message clone() const;

  private:
    t_message(R_bytebuf);

  protected:
    t_bytebuf buf_;
  };

  t_messenger_key read_dst(R_message);
  t_messenger_key read_src(R_message);
  t_id            read_id (R_message);
  t_n             read_len(R_message);

///////////////////////////////////////////////////////////////////////////////

  class t_notify_message;
  using r_notify_message = t_prefix<t_notify_message>::r_;
  using x_notify_message = t_prefix<t_notify_message>::x_;

  class t_notify_message : public t_message {
  public:
    t_notify_message();

    inline
    t_notify_message(x_message msg) : t_message(base::x_cast(msg)) {
    }

    inline
    t_notify_message(x_notify_message msg) : t_message(base::x_cast(msg)) {
    }

    inline
    r_notify_message operator=(x_notify_message msg) {
      t_message::operator=(base::x_cast(msg));
      return *this;
    }

    t_validity get(r_messenger_state,
                   r_messenger_name,
                   r_messenger_key,
                   r_messenger_prio,
                   r_messenger_user);
  };

///////////////////////////////////////////////////////////////////////////////

  class t_timeout_message;
  using r_timeout_message = t_prefix<t_timeout_message>::r_;
  using x_timeout_message = t_prefix<t_timeout_message>::x_;

  class t_timeout_message : public t_message {
  public:
    t_timeout_message();

    inline
    t_timeout_message(x_message msg) : t_message(base::x_cast(msg)) {
    }

    inline
    t_timeout_message(x_timeout_message msg) : t_message(base::x_cast(msg)) {
    }

    inline
    r_timeout_message operator=(x_timeout_message msg) {
      t_message::operator=(base::x_cast(msg));
      return *this;
    }

    t_validity get(r_bool periodic,
                   r_multiple_of_100ms,
                   r_messenger_key,
                   r_messenger_prio,
                   r_messenger_user);
  };

///////////////////////////////////////////////////////////////////////////////

  class t_fail_message;
  using r_fail_message = t_prefix<t_fail_message>::r_;
  using x_fail_message = t_prefix<t_fail_message>::x_;

  class t_fail_message : public t_message {
  public:
    enum t_reason {
      reason_messenger_noexist = 1
    };
    using r_reason = t_prefix<t_reason>::r_;

    inline
    t_fail_message(t_n n) : t_message(n) {
    }

    inline
    t_fail_message(x_message msg) : t_message(base::x_cast(msg)) {
    }

    inline
    t_fail_message(x_fail_message msg) : t_message(base::x_cast(msg)) {
    }

    inline
    r_fail_message operator=(x_fail_message msg) {
      t_message::operator=(base::x_cast(msg));
      return *this;
    }

    t_validity get(r_reason, r_message send_message);
  };

///////////////////////////////////////////////////////////////////////////////

  class t_alive_message;
  using r_alive_message = t_prefix<t_alive_message>::r_;
  using x_alive_message = t_prefix<t_alive_message>::x_;

  class t_alive_message : public t_message {
  public:
    t_alive_message();

    inline
    t_alive_message(x_message msg) : t_message(base::x_cast(msg)) {
    }

    inline
    t_alive_message(x_alive_message msg) : t_message(base::x_cast(msg)) {
    }

    inline
    r_alive_message operator=(x_alive_message msg) {
      t_message::operator=(base::x_cast(msg));
      return *this;
    }

    t_validity get(); // not sure what to pass yet
  };

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
