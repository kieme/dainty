#ifndef _DAINTY_MESSAGING_MESSAGES_H_
#define _DAINTY_MESSAGING_MESSAGES_H_

#include "dainty_named.h"
#include "dainty_named_string.h"
#include "dainty_container_bytebuf.h"

namespace dainty
{
namespace messaging
{
///////////////////////////////////////////////////////////////////////////////

  using named::t_n_;
  using named::t_n;
  using named::t_validity;
  using named::VALID;
  using named::INVALID;

  using named::t_bool;
  using named::t_uint64;
  using named::t_uint32;
  using named::t_int32;
  using named::t_uchar;
  using named::t_uint16;
  using named::t_explicit;
  using named::string::t_string;

  //using named::bytebuf_t;
  //typedef library::types::bytebuf_byte_t byte_t;

  enum t_t_messageype {
    MSG_NOTIFY,  // when messenger/alias becomes avaibles
    MSG_TIMEOUT, // timeout
    MSG_ALIVE,   // message that requires processing
    MSG_FAIL     // when send message failed
  };

  enum t_messenger_state {
    STATE_AVAILABLE,
    STATE_AVAILABLE_UNRESPONSIVE,
    STATE_UNAVAILABLE,
  };

///////////////////////////////////////////////////////////////////////////////

  enum  t_messenger_user_tag_ { };
  using t_messenger_user  = named::t_messenger_user<t_messenger_user_tag_>;

  enum  t_messenger_key_tag_ { };
  using t_messenger_key   = t_explicit<t_uint64, t_messenger_key_tag_>;

  enum  t_messenger_name_tag_ { };
  using t_messenger_name  = t_string<t_messenger_name_tag_, 32>;

  enum  t_message_domain_tag_ { };
  using t_message_domain  = t_explicit<t_uchar, t_message_domain_tag_>;

  enum  t_message_user_tag_ { };
  using t_message_user    = t_explicit<t_uchar, t_message_user_tag_>;

  enum  t_message_version_tag_ { };
  using t_message_version = t_explicit<t_uchar, t_message_version_tag_>;

  enum  t_messenger_prio_tag_ { };
  using t_messenger_prio  = t_explicit<t_uint16, t_messenger_prio_tag_>;

  enum  t_multiple_of_100ms_tag_ { };
  using t_multiple_of_100ms = t_multiple<100, t_multiple_of_100ms_tag_>

///////////////////////////////////////////////////////////////////////////////

  class t_message_id {
  public:
    using t_domain  = t_message_domain;
    using t_user    = t_message_user;
    using t_version = t_message_version;

    t_message_id(t_domain _domain, t_user _user, t_version _version)
      : domain(_domain), user(_user), version(_version) {
    }

    t_domain  domain;
    t_user    user;
    t_version version;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_message {
    typedef bytebuf_t buf_t;
  public:
    typedef t_messenger_key   t_key;
    typedef t_message_version t_version;
    typedef t_message_id      t_id;
    typedef t_message_user    t_messenger_user;
    typedef t_message_domain  domain_t;
    typedef buf_t::byte_t     byte_t;
    typedef buf_t::t_content  t_content;

    t_message();
    explicit t_message(t_uint32);
    explicit t_message(t_content);

    t_message& operator=(t_content);

    t_bool set(const t_id&, t_uint32 length, t_uint16 cnt = 0);
    t_bool get(t_id&      id,
               t_uint32&  length,
               t_key&     dst,
               t_key&     src,
               t_uint16&  cnt);

    operator t_bool      () const;
    operator t_validity  () const;

    t_bool   is_empty    () const;
    t_uint32 get_capacity() const;

          byte_t*       data();
    const byte_t*       data() const;
    const byte_t* const_data() const;

    t_content release();
    t_content clone() const;
    t_content clone(t_uint32) const;

  private:
    buf_t buf_;
  };

  t_messenger_key  read_dst(const t_message&);
  t_messenger_key  read_src(const t_message&);
  t_message_id     read_id (const t_message&);
  t_uint32         read_len(const t_message&);

///////////////////////////////////////////////////////////////////////////////

  class t_notify_message : public t_message {
  public:
    t_notify_message();
    explicit t_notify_message(t_content content) : t_message(content) {
    }

    t_notify_message& operator=(t_content content) {
      t_message::operator=(content);
      return *this;
    }

    t_bool get(t_messenger_state&,
               t_messenger_name&,
               t_messenger_key&,
               t_messenger_prio&,
               t_messenger_user&);
  };

///////////////////////////////////////////////////////////////////////////////

  class t_timeout_message : public t_message {
  public:
    t_timeout_message();
    explicit t_timeout_message(t_content content) : t_message(content) {
    }

    t_timeout_message& operator=(t_content content) {
      t_message::operator=(content);
      return *this;
    }

    t_bool get(t_bool& periodic,
               t_multiple_of_100ms&,
               t_messenger_key&,
               t_messenger_prio&,
               t_messenger_user&);
  };

///////////////////////////////////////////////////////////////////////////////

  class t_message_fail : public t_message {
  public:
    enum reason_t {
      reason_messenger_noexist = 1
    };

    t_message_fail(t_uint32 n) : t_message(n) {
    }

    explicit t_message_fail(t_content content) : t_message(content) {
    }

    t_message_fail& operator=(t_content content) {
      t_message::operator=(content);
      return *this;
    }

    t_bool get(reason_t&, t_message& send_message);
  };

///////////////////////////////////////////////////////////////////////////////

  class t_alive_message : public t_message {
  public:
    t_alive_message();

    explicit t_alive_message(t_content content) : t_message(content) {
    }

    t_alive_message& operator=(t_content content) {
      t_message::operator=(content);
      return *this;
    }

    t_bool get(); // not sure what to pass yet
  };

///////////////////////////////////////////////////////////////////////////////
}
}

#endif
