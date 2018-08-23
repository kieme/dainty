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

#include "dainty_named.h"
#include "dainty_named_string.h"
#include "dainty_container_list.h"
#include "dainty_messaging_err.h"
#include "dainty_messaging_messages.h"

namespace dainty
{
namespace messaging
{
namespace messenger
{
///////////////////////////////////////////////////////////////////////////////

  using library::types::fd_t;
  using library::types::errorid_t;
  using library::types::npushlist_t;
  using library::string::nstring_t;

  using messages_t = npushlist_t<message_t, 0>;

  enum password_tag_t { };
  typedef nstring_t<32, password_tag_t> password_t;

///////////////////////////////////////////////////////////////////////////////

  enum t_visibility {
    visibility_off,
    visibility_process,
    visibility_node,
    visibility_system
  };

  enum visibility_string_tag_t { };
  typedef nstring_t<14, visibility_string_tag_t>
    visibility_string_t;

  visibility_string_t to_string(t_visibility);

///////////////////////////////////////////////////////////////////////////////

  struct t_timer_params {
    multiple_of_100ms_t factor_;
    bool_t              periodic_;
    messenger_prio_t    prio_;
    messenger_user_t    user_;

    t_timer_params(multiple_of_100ms_t factor
                               = multiple_of_100ms_t(0),
                             bool_t periodic = false,
                             messenger_prio_t prio = messenger_prio_t(0),
                             messenger_user_t user = messenger_user_t())
      : factor_(factor), periodic_(periodic), prio_(prio), user_(user) { }
  };

///////////////////////////////////////////////////////////////////////////////

  struct t_create_params {
    t_visibility   visibility_;
    multiple_of_100ms_t      alive_factor_;
    t_timer_params timer_params_;
    t_create_params()
      : visibility_(visibility_process), alive_factor_(0) { }
    t_create_params(t_visibility visibility,
                              multiple_of_100ms_t alive_factor
                                = multiple_of_100ms_t(0),
                              const t_timer_params& timer_params
                                = t_timer_params())
      : visibility_(visibility), alive_factor_(alive_factor),
        timer_params_(timer_params) { }
  };

  struct t_params {
    t_visibility   visibility_;   // visibility of messenger
    multiple_of_100ms_t      alive_factor_; // zero - disable
    t_timer_params timer_params_; // timer properties
    messenger_memberlist_t   groups_;       // belong to these groups
    messenger_memberlist_t   monitor_;      // monitor these

    t_params(t_visibility visibility
                         = visibility_process, // share threads
                       multiple_of_100ms_t alive_factor
                         = multiple_of_100ms_t(0),       // disabled - 0
                       const t_timer_params& timer_params
                         = t_timer_params(),   // disabled
                       const messenger_memberlist_t& groups
                         = messenger_memberlist_t(),     // none
                       const messenger_memberlist_t& monitor
                         = messenger_memberlist_t())     // none
      : visibility_(visibility), alive_factor_(alive_factor),
        timer_params_(timer_params), groups_(groups), monitor_(monitor) {
    }
  };

///////////////////////////////////////////////////////////////////////////////

  struct t_id {
    typedef t_key key_t;

    t_id() : key_(0), fd_(-1) { }
    t_id(key_t key, fd_t fd) : key_(key), fd_(fd) { }

    inline operator bool_t() const {
      return get(key_) != 0 && get(fd_) != -1;
    }

    inline key_t get_key() const {
      return key_;
    }

    t_id release() {
      t_id tmp(*this);
      set(key_) = 0;
      set(fd_)  = -1;
      return tmp;
    }

    key_t  key_;
    fd_t   fd_;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_messenger {
  public:
    typedef t_key           key_t;
    typedef messenger_name_t          name_t;
    typedef t_params        params_t;
    typedef t_timer_params  timer_params_t;
    typedef t_visibility    t_visibility;
    typedef messenger_prio_t          prio_t;
    typedef messenger_user_t          user_t;
    typedef messenger_memberlist_t    list_t;
    typedef messenger_memberkeylist_t keylist_t;

    t_messenger() { }
    t_messenger(const t_messenger&);             // fake move
    t_messenger& operator=(const t_messenger&);  // fake move
    ~t_messenger();

    operator bool_t () const                                 { return id_; }

    fd_t   get_fd    () const                            { return id_.fd_; }
    key_t  get_key   () const                      { return id_.get_key(); }
    name_t get_name  () const;
    bool_t get_params(params_t&) const;

    bool_t update_visibility(t_visibility);
    bool_t update_alive_period(multiple_of_100ms_t);

    bool_t post_message (const key_t&, message_t&) const;
    bool_t wait_message (messages_t&) const;
    bool_t check_message(messages_t&) const;

    bool_t start_timer(const timer_params_t&);
    bool_t stop_timer ();
    bool_t query_timer(timer_params_t&) const;

    bool_t add_to_group(const password_t&, const name_t&,
                        prio_t = prio_t(0), user_t = user_t());
    bool_t remove_from_group(const password_t&, const name_t&, user_t* = 0);
    bool_t is_in_group (const name_t&, user_t* = 0) const;
    bool_t get_groups  (list_t&) const; // maybe keylist

    bool_t add_monitor   (const name_t&, prio_t = prio_t(0),
                          user_t = user_t());
    bool_t remove_monitor(const name_t&, user_t* = 0);
    key_t  is_monitored  (const name_t&, user_t* = 0) const;
    bool_t get_monitored (keylist_t&) const;

  private: // only messaging service can create a messenger
    typedef t_id id_t;
    typedef t_create_params create_params_t;
    t_messenger(const id_t& id) : id_(id) { }
    mutable id_t id_;

    friend t_messenger create_messenger(errorid_t&,
                                              const name_t&,
                                              const create_params_t&);
    friend bool_t destroy_messenger(t_messenger);
  };

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
