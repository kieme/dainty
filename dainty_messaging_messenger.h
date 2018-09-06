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

#ifndef _DAINTY_MESSAGING_MESSENGER_H_
#define _DAINTY_MESSAGING_MESSENGER_H_

#include <map>
#include "dainty_named.h"
#include "dainty_named_string.h"
#include "dainty_container_list.h"
#include "dainty_messaging_err.h"
#include "dainty_messaging_message.h"

namespace dainty
{
namespace messaging
{
namespace messenger
{
  using named::t_bool;
  using named::t_void;
  using named::t_n;
  using named::t_validity;
  using named::t_prefix;
  using named::string::t_string;
  using named::VALID;
  using named::INVALID;
  using named::t_fd;
  using err::t_err;
  using message::t_message;
  using message::r_message;
  using message::t_multiple_of_100ms;

  using t_user     = message::t_messenger_user;
  using t_key      = message::t_messenger_key;
  using t_name     = message::t_messenger_name;
  using t_prio     = message::t_messenger_prio;
  using R_key      = t_prefix<t_key>::R_;
  using r_name     = t_prefix<t_name>::r_;
  using R_name     = t_prefix<t_name>::R_;
  using p_user     = t_prefix<t_user>::p_;
  using t_messages = container::list::t_list<t_message>;
  using r_messages = t_prefix<t_messages>::r_;

///////////////////////////////////////////////////////////////////////////////

  enum  t_password_tag_ { };
  using t_password = t_string<t_password_tag_, 16>;
  using R_password = t_prefix<t_password>::R_;

///////////////////////////////////////////////////////////////////////////////

  enum t_visibility {
    VISIBILITY_OFF,
    VISIBILITY_PROCESS,
    VISIBILITY_NODE,
    VISIBILITY_SYSTEM
  };

  enum  t_visibility_name_tag_ { };
  using t_visibility_name = t_string<t_visibility_name_tag_, 14>;

  t_visibility_name to_name(t_visibility);

///////////////////////////////////////////////////////////////////////////////

  class t_timer_params {
  public:
    t_multiple_of_100ms factor;
    t_bool              periodic;
    t_prio              prio;
    t_user              user;

    inline
    t_timer_params(t_multiple_of_100ms _factor   = t_multiple_of_100ms(0),
                   t_bool              _periodic = false,
                   t_prio              _prio     = t_prio(0),
                   t_user              _user     = t_user(0L))
      : factor(_factor), periodic(_periodic), prio(_prio), user(_user) {
    }
  };
  using r_timer_params = t_prefix<t_timer_params>::r_;
  using R_timer_params = t_prefix<t_timer_params>::R_;

///////////////////////////////////////////////////////////////////////////////

  class t_group {
  public:
    t_name     name;
    t_password password;
    t_prio     prio;
    t_user     user;

    inline
    t_group() : prio(0), user(0L) {
    }

    inline
    t_group(R_name _name, R_password _password, t_prio _prio, t_user _user)
      : name(_name), password(_password), prio(_prio), user(_user) {
    }
  };
  using R_group = t_prefix<t_group>::R_;

  using t_group_list = std::map<t_name, t_group>;
  using r_group_list = t_prefix<t_group_list>::r_;
  using R_group_list = t_prefix<t_group_list>::R_;
  using p_group_list = t_prefix<t_group_list>::p_;

///////////////////////////////////////////////////////////////////////////////

  class t_monitor {
  public:
    t_name name;
    t_prio prio;
    t_user user;
    t_key  key;

    inline
    t_monitor() : prio{0}, user{0L}, key{0} {
    }

    inline
    t_monitor(R_name _name, t_prio _prio, t_user _user, t_key _key)
      : name{_name}, prio{_prio}, user{_user}, key{_key} {
    }
  };

  using t_monitor_list = std::map<t_name, t_monitor>;
  using r_monitor_list = t_prefix<t_monitor_list>::r_;
  using R_monitor_list = t_prefix<t_monitor_list>::R_;
  using p_monitor_list = t_prefix<t_monitor_list>::p_;

///////////////////////////////////////////////////////////////////////////////

  class t_params {
  public:
    // queue size
    t_visibility        visibility;
    t_multiple_of_100ms alive_factor;
    t_timer_params      timer_params;
    t_group_list        group_list;
    t_monitor_list      monitor_list;

    inline
    t_params(t_visibility        _visibility   = VISIBILITY_PROCESS,
             t_multiple_of_100ms _alive_factor = t_multiple_of_100ms(0),
             R_timer_params      _timer_params = t_timer_params())
      : visibility(_visibility), alive_factor(_alive_factor),
        timer_params(_timer_params) {
    }

    inline
    t_params(t_visibility        _visibility,
             t_multiple_of_100ms _alive_factor,
             R_timer_params      _timer_params,
             R_group_list        _group_list,
             R_monitor_list      _monitor_list)
      : visibility(_visibility), alive_factor(_alive_factor),
        timer_params(_timer_params), group_list(_group_list),
        monitor_list(_monitor_list) {
    }
  };
  using r_params = t_prefix<t_params>::r_;
  using R_params = t_prefix<t_params>::R_;

///////////////////////////////////////////////////////////////////////////////

  class t_id {
  public:
    t_key key;
    t_fd  fd;

    inline t_id() : key(0), fd(-1) {
    }

    inline t_id(R_key _key, t_fd _fd) : key(_key), fd(_fd) {
    }

    inline operator t_validity() const {
      return get(key) && get(fd) != -1 ? VALID : INVALID;
    }
  };
  using R_id = t_prefix<t_id>::R_;

///////////////////////////////////////////////////////////////////////////////

  class t_messenger;
  using r_messenger = t_prefix<t_messenger>::r_;
  using R_messenger = t_prefix<t_messenger>::R_;
  using x_messenger = t_prefix<t_messenger>::x_;

  class t_messenger {
  public:
     t_messenger(x_messenger);
     t_messenger(R_messenger)           = delete;
     r_messenger operator=(R_messenger) = delete;
     r_messenger operator=(x_messenger) = delete;
    ~t_messenger();

    operator t_validity() const;

    t_fd   get_fd    () const;
    t_key  get_key   () const;
    t_name get_name  (t_err) const;
    t_void get_params(t_err, r_params) const;

    t_void update_visibility  (t_err, t_visibility);
    t_void update_alive_period(t_err, t_multiple_of_100ms);

    t_void post_message (t_err, R_key, r_message) const;
    t_void wait_message (t_err, r_messages) const;

    t_void start_timer(t_err, R_timer_params);
    t_void stop_timer (t_err);
    t_void query_timer(t_err, r_timer_params) const;

    t_void add_to_group     (t_err, R_password, R_name, t_prio = t_prio(0),
                                    t_user = t_user());
    t_void remove_from_group(t_err, R_password, R_name, p_user = nullptr);
    t_bool is_in_group      (t_err, R_name, p_user = nullptr) const;
    t_void get_groups       (t_err, r_group_list) const;

    t_void add_monitor   (t_err, R_name, t_prio = t_prio(0), t_user = t_user());
    t_void remove_monitor(t_err, R_name, p_user = nullptr);
    t_key  is_monitored  (t_err, R_name, p_user = nullptr) const;
    t_void get_monitored (t_err, r_monitor_list) const;

  private:
    friend t_messenger mk_(R_id);
    inline t_messenger(R_id id) : id_(id) {
    }

    t_id id_;
  };

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
