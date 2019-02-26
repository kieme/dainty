/******************************************************************************

MIT License

Copyright (c) 2018 frits.germs@gmx.net

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

#ifndef _DAINTY_SANDBOX_LOGIC_EXT_MESSENGER_API_H_
#define _DAINTY_SANDBOX_LOGIC_EXT_MESSENGER_API_H_

#include "dainty_messaging.h"

namespace dainty
{
namespace sandbox
{
namespace logic_messenger_ext
{
  using named::t_void;
  using named::t_bool;
  using t_err = err::t_err;

  using t_messenger_msg           = messaging::message::t_message;
  using x_messenger_msg           = messaging::message::x_message;
  using t_messenger_msgs          = messaging::messenger::t_messages;
  using t_messenger_msg_id        = messaging::message::t_id;
  using R_messenger_msg_id        = messaging::message::R_id;
  using t_multiple_of_100ms       = messaging::t_multiple_of_100ms;
  using t_messenger_scope         = messaging::t_messenger_scope;
  using r_messenger_scope         = messaging::r_messenger_scope;
  using p_messenger_name_list     = messaging::p_messenger_name_list;
  using R_messenger_password      = messaging::R_messenger_password;
  using t_messenger_prio          = messaging::t_messenger_prio;
  using t_messenger_user          = messaging::t_messenger_user;
  using p_messenger_user          = messaging::p_messenger_user;
  using t_messenger_name          = messaging::t_messenger_name;
  using R_messenger_name          = messaging::R_messenger_name;
  using t_messenger_key           = messaging::t_messenger_key;
  using R_messenger_key           = messaging::R_messenger_key;
  using t_messenger_state         = messaging::message::t_messenger_state;
  using t_messenger_params        = messaging::t_messenger_params;
  using r_messenger_params        = messaging::messenger::r_params;
  using r_messenger_group_list    = messaging::messenger::r_group_list;
  using r_messenger_monitor_list  = messaging::messenger::r_monitor_list;
  using R_messenger_create_params = messaging::R_messenger_create_params;

///////////////////////////////////////////////////////////////////////////////

  class t_logic_messenger_ext_api {
  public:
    using t_messenger_msg           = messaging::message::t_message;
    using x_messenger_msg           = messaging::message::x_message;
    using t_messenger_msgs          = messaging::messenger::t_messages;
    using t_messenger_msg_id        = messaging::message::t_id;
    using R_messenger_msg_id        = messaging::message::R_id;
    using t_multiple_of_100ms       = messaging::t_multiple_of_100ms;
    using t_messenger_scope         = messaging::t_messenger_scope;
    using r_messenger_scope         = messaging::r_messenger_scope;
    using p_messenger_name_list     = messaging::p_messenger_name_list;
    using R_messenger_password      = messaging::R_messenger_password;
    using t_messenger_prio          = messaging::t_messenger_prio;
    using t_messenger_user          = messaging::t_messenger_user;
    using p_messenger_user          = messaging::p_messenger_user;
    using t_messenger_name          = messaging::t_messenger_name;
    using R_messenger_name          = messaging::R_messenger_name;
    using t_messenger_key           = messaging::t_messenger_key;
    using R_messenger_key           = messaging::R_messenger_key;
    using t_messenger_state         = messaging::message::t_messenger_state;
    using t_messenger_params        = messaging::t_messenger_params;
    using r_messenger_params        = messaging::messenger::r_params;
    using r_messenger_group_list    = messaging::messenger::r_group_list;
    using r_messenger_monitor_list  = messaging::messenger::r_monitor_list;
    using R_messenger_create_params = messaging::R_messenger_create_params;

    virtual ~t_logic_messenger_ext_api() {}

///////////////////////////////////////////////////////////////////////////////

    virtual operator t_validity() const noexcept = 0;

///////////////////////////////////////////////////////////////////////////////

    virtual t_messenger_key  get_key   ()      const noexcept = 0;
    virtual t_messenger_name get_name  (t_err) const noexcept = 0;
    virtual t_void           get_params(t_err,
                                        r_messenger_params) const noexcept = 0;

///////////////////////////////////////////////////////////////////////////////

    virtual t_void post_msg(t_err, R_messenger_key,
                            x_messenger_msg) noexcept = 0;

///////////////////////////////////////////////////////////////////////////////

    virtual t_void update_scope       (t_err, t_messenger_scope)   noexcept = 0;
    virtual t_void update_alive_period(t_err, t_multiple_of_100ms) noexcept = 0;

///////////////////////////////////////////////////////////////////////////////

    virtual t_void add_monitor(t_err, R_messenger_name,
                               t_messenger_prio = t_messenger_prio(0),
                               t_messenger_user = t_messenger_user()) noexcept = 0;
    virtual t_void remove_monitor(t_err, R_messenger_name,
                                  p_messenger_user = nullptr) noexcept = 0;
    virtual t_messenger_key is_monitored(t_err, R_messenger_name,
                                         p_messenger_user = nullptr) const noexcept = 0;
    virtual t_void get_monitored(t_err,
                                 r_messenger_monitor_list) const noexcept = 0;

///////////////////////////////////////////////////////////////////////////////

    virtual t_void add_to_group(t_err, R_messenger_password,
                                R_messenger_name group,
                                t_messenger_prio = t_messenger_prio(0),
                                t_messenger_user = t_messenger_user()) noexcept = 0;
    virtual t_void remove_from_group(t_err, R_messenger_password,
                                     R_messenger_name,
                                     p_messenger_user = nullptr) noexcept = 0;
    virtual t_bool is_in_group(t_err, R_messenger_name,
                               p_messenger_user = nullptr)   const noexcept = 0;
    virtual t_void get_groups(t_err, r_messenger_group_list) const noexcept = 0;

///////////////////////////////////////////////////////////////////////////////

    virtual t_void create_group (t_err, R_messenger_password, R_messenger_name,
                                 t_messenger_scope) noexcept = 0;
    virtual t_void destroy_group(t_err, R_messenger_password,
                                 R_messenger_name) noexcept = 0;
    virtual t_bool is_group     (t_err, R_messenger_name, r_messenger_scope,
                                 p_messenger_name_list = nullptr) const noexcept = 0;

///////////////////////////////////////////////////////////////////////////////

    virtual t_void add_to_group(t_err, R_messenger_password,
                                R_messenger_name name,
                                R_messenger_name group,
                                t_messenger_prio = t_messenger_prio(0),
                                t_messenger_user = t_messenger_user()) noexcept = 0;
    virtual t_void remove_from_group(t_err, R_messenger_password,
                                     R_messenger_name,
                                     R_messenger_name group,
                                     p_messenger_user = nullptr) noexcept = 0;
    virtual t_bool is_in_group(t_err, R_messenger_name,
                               R_messenger_name group,
                               p_messenger_user = nullptr) const noexcept = 0;

///////////////////////////////////////////////////////////////////////////////
  };

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
