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

#ifndef _DAINTY_SANDBOX_LOGIC_EXT_MESSENGER_H_
#define _DAINTY_SANDBOX_LOGIC_EXT_MESSENGER_H_

#include "dainty_sandbox_logic_api.h"
#include "dainty_sandbox_logic_ext.h"
#include "dainty_sandbox_logic_ext_messenger_api.h"
#include "dainty_sandbox_logic_ext_messenger_notify.h"

namespace dainty
{
namespace sandbox
{
namespace logic_messenger_ext
{
///////////////////////////////////////////////////////////////////////////////

  class t_logic_messenger_ext : public t_logic_messenger_ext_api,
                                public t_logic_messenger_ext_notify {
  public:
    t_logic_messenger_ext(t_err, r_logic) noexcept;

    operator t_validity() const noexcept override final;

///////////////////////////////////////////////////////////////////////////////

    t_messenger_key  get_key   ()                          const noexcept override final;
    t_messenger_name get_name  (t_err)                     const noexcept override final;
    t_void           get_params(t_err, r_messenger_params) const noexcept override final;

///////////////////////////////////////////////////////////////////////////////

    t_void post_msg(t_err, R_messenger_key, x_messenger_msg) noexcept override final;

///////////////////////////////////////////////////////////////////////////////

    t_void update_scope       (t_err, t_messenger_scope)   noexcept override final;
    t_void update_alive_period(t_err, t_multiple_of_100ms) noexcept override final;

///////////////////////////////////////////////////////////////////////////////

    t_void add_monitor(t_err, R_messenger_name,
                       t_messenger_prio = t_messenger_prio(0),
                       t_messenger_user = t_messenger_user()) noexcept override final;
    t_void remove_monitor(t_err, R_messenger_name,
                          p_messenger_user = nullptr) noexcept override final;
    t_messenger_key is_monitored(t_err, R_messenger_name,
                                 p_messenger_user = nullptr) const noexcept override final;
    t_void get_monitored(t_err,
                         r_messenger_monitor_list) const noexcept override final;

///////////////////////////////////////////////////////////////////////////////

    t_void add_to_group(t_err, R_messenger_password,
                        R_messenger_name group,
                        t_messenger_prio = t_messenger_prio(0),
                        t_messenger_user = t_messenger_user()) noexcept override final;
    t_void remove_from_group(t_err, R_messenger_password,
                             R_messenger_name,
                             p_messenger_user = nullptr) noexcept override final;
    t_bool is_in_group(t_err, R_messenger_name,
                       p_messenger_user = nullptr)   const noexcept override final;
    t_void get_groups(t_err, r_messenger_group_list) const noexcept override final;

///////////////////////////////////////////////////////////////////////////////

    t_void create_group (t_err, R_messenger_password, R_messenger_name,
                         t_messenger_scope) noexcept override final;
    t_void destroy_group(t_err, R_messenger_password,
                         R_messenger_name) noexcept override final;
    t_bool is_group     (t_err, R_messenger_name, r_messenger_scope,
                         p_messenger_name_list = nullptr) const noexcept override final;

///////////////////////////////////////////////////////////////////////////////

    t_void add_to_group(t_err, R_messenger_password,
                        R_messenger_name name,
                        R_messenger_name group,
                        t_messenger_prio = t_messenger_prio(0),
                        t_messenger_user = t_messenger_user()) noexcept override final;
    t_void remove_from_group(t_err, R_messenger_password,
                             R_messenger_name,
                             R_messenger_name group,
                             p_messenger_user = nullptr) noexcept override final;
    t_bool is_in_group(t_err, R_messenger_name,
                       R_messenger_name group,
                       p_messenger_user = nullptr) const noexcept override final;

  private:
    using r_ext_ = t_prefix<t_logic_messenger_ext>::r_;

    class t_impl_ : public t_logic_ext,
                    public t_logic_messenger_ext_api {
    public:
      t_impl_(t_err, r_logic, r_ext_) noexcept;

///////////////////////////////////////////////////////////////////////////////

      t_void notify_start(t_err) noexcept override final;
      t_void notify_cleanup()    noexcept override final;

      t_void notify_timeout(t_timer_id,
                            R_timer_params)   noexcept override final;
      t_void notify_fdevent(t_fdevent_id,
                            R_fdevent_params) noexcept override final;

///////////////////////////////////////////////////////////////////////////////

      t_messenger_key  get_key   ()                          const noexcept override final;
      t_messenger_name get_name  (t_err)                     const noexcept override final;
      t_void           get_params(t_err, r_messenger_params) const noexcept override final;

///////////////////////////////////////////////////////////////////////////////

      t_void post_msg(t_err, R_messenger_key, x_messenger_msg) noexcept override final;

///////////////////////////////////////////////////////////////////////////////

      t_void update_scope       (t_err, t_messenger_scope)   noexcept override final;
      t_void update_alive_period(t_err, t_multiple_of_100ms) noexcept override final;

///////////////////////////////////////////////////////////////////////////////

      t_void add_monitor(t_err, R_messenger_name, t_messenger_prio,
                         t_messenger_user) noexcept override final;
      t_void remove_monitor(t_err, R_messenger_name,
                            p_messenger_user) noexcept override final;
      t_messenger_key is_monitored(t_err, R_messenger_name,
                                   p_messenger_user) const noexcept override final;
      t_void get_monitored(t_err,
                           r_messenger_monitor_list) const noexcept override final;

///////////////////////////////////////////////////////////////////////////////

      t_void add_to_group(t_err, R_messenger_password, R_messenger_name group,
                          t_messenger_prio, t_messenger_user) noexcept override final;
      t_void remove_from_group(t_err, R_messenger_password,
                               R_messenger_name, p_messenger_user) noexcept override final;
      t_bool is_in_group(t_err, R_messenger_name,
                         p_messenger_user)   const noexcept override final;
      t_void get_groups(t_err, r_messenger_group_list) const noexcept override final;

///////////////////////////////////////////////////////////////////////////////

      t_void create_group (t_err, R_messenger_password, R_messenger_name,
                           t_messenger_scope) noexcept override final;
      t_void destroy_group(t_err, R_messenger_password,
                           R_messenger_name) noexcept override final;
      t_bool is_group     (t_err, R_messenger_name, r_messenger_scope,
                           p_messenger_name_list) const noexcept override final;

///////////////////////////////////////////////////////////////////////////////

      t_void add_to_group(t_err, R_messenger_password, R_messenger_name name,
                          R_messenger_name group, t_messenger_prio,
                          t_messenger_user) noexcept override final;
      t_void remove_from_group(t_err, R_messenger_password,
                               R_messenger_name, R_messenger_name group,
                               p_messenger_user) noexcept override final;
      t_bool is_in_group(t_err, R_messenger_name, R_messenger_name group,
                         p_messenger_user) const noexcept override final;

///////////////////////////////////////////////////////////////////////////////

    private:
      r_ext_ ext_;
    };

    t_impl_ impl_;
  };

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
