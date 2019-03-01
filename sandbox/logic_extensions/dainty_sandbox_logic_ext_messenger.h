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

#include "dainty_container_maybe.h"
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
  using container::maybe::t_maybe;

///////////////////////////////////////////////////////////////////////////////

  struct t_logic_messenger_ext_params {
    t_messenger_scope   scope        {messaging::messenger::SCOPE_PROCESS};
    t_multiple_of_100ms alive_period {10};
  };
  using T_logic_messenger_ext_params
    = t_prefix<t_logic_messenger_ext_params>::T_;
  using R_logic_messenger_ext_params
    = t_prefix<t_logic_messenger_ext_params>::R_;

///////////////////////////////////////////////////////////////////////////////

  class t_logic_messenger_ext;
  using x_logic_messenger_ext = t_prefix<t_logic_messenger_ext>::x_;
  using r_logic_messenger_ext = t_prefix<t_logic_messenger_ext>::r_;
  using R_logic_messenger_ext = t_prefix<t_logic_messenger_ext>::R_;

  class t_logic_messenger_ext : public t_logic_messenger_ext_api,
                                public t_logic_messenger_ext_notify {
    using api_    = t_logic_messenger_ext_api;
    using notify_ = t_logic_messenger_ext_notify;
  public:
    using t_messenger_name = api_::t_messenger_name;
    using R_messenger_name = api_::R_messenger_name;
    using t_messenger_msg  = api_::t_messenger_msg;
    using x_messenger_msg  = api_::x_messenger_msg;
    using t_messenger_key  = api_::t_messenger_key;
    using t_messenger_prio = api_::t_messenger_prio;
    using t_messenger_user = api_::t_messenger_user;

    t_logic_messenger_ext(t_err, r_logic, R_logic_messenger_ext_params) noexcept;
   ~t_logic_messenger_ext();

    t_logic_messenger_ext()                                = delete;
    t_logic_messenger_ext(R_logic_messenger_ext)           = delete;
    t_logic_messenger_ext(x_logic_messenger_ext)           = delete;
    r_logic_messenger_ext operator=(R_logic_messenger_ext) = delete;
    r_logic_messenger_ext operator=(x_logic_messenger_ext) = delete;

///////////////////////////////////////////////////////////////////////////////

    operator t_validity() const noexcept;

///////////////////////////////////////////////////////////////////////////////

    t_messenger_key  get_messenger_key   ()                   const noexcept override final;
    t_messenger_name get_messenger_name  ()                   const noexcept override final;
    t_void           get_messenger_params(r_messenger_params) const noexcept override final;

///////////////////////////////////////////////////////////////////////////////

    t_void post_messenger_msg(t_err, t_messenger_key, x_messenger_msg) noexcept override final;

///////////////////////////////////////////////////////////////////////////////

    t_void update_messenger_scope       (t_err, t_messenger_scope)   noexcept override final;
    t_void update_messenger_alive_period(t_err, t_multiple_of_100ms) noexcept override final;

///////////////////////////////////////////////////////////////////////////////

    t_messenger_msg_notify_id
      add_messenger_msg_notify(t_err, R_messenger_msg_notify_params) noexcept override final;
    t_messenger_msg_notify_id
      add_messenger_msg_notify(t_err, R_messenger_msg_notify_params,
                                x_messenger_msg_notify_ptr)          noexcept override final;

    t_messenger_msg_notify_ptr
      remove_messenger_msg_notify(t_messenger_msg_notify_id)       noexcept override final;
    P_messenger_msg_notify_params
      get_messenger_msg_notify   (t_messenger_msg_notify_id) const noexcept override final;

///////////////////////////////////////////////////////////////////////////////

    t_messenger_monitor_id
      add_messenger_monitor(t_err, R_messenger_name,
                             R_messenger_monitor_params)     noexcept override final;
    t_messenger_monitor_id
      add_messenger_monitor(t_err, R_messenger_name,
                             R_messenger_monitor_params,
                             x_messenger_monitor_notify_ptr) noexcept override final;

    t_messenger_monitor_notify_ptr
      remove_messenger_monitor(t_messenger_monitor_id)       noexcept override final;
    P_messenger_monitor_params
      get_messenger_monitor   (t_messenger_monitor_id) const noexcept override final;

///////////////////////////////////////////////////////////////////////////////

    t_void add_messenger_to_group(t_err, R_messenger_password,
                                  R_messenger_name group,
                                  t_messenger_prio = t_messenger_prio(0),
                                  t_messenger_user = t_messenger_user()) noexcept override final;
    t_void remove_messenger_from_group(t_err, R_messenger_password,
                                       R_messenger_name,
                                       p_messenger_user = nullptr) noexcept override final;
    t_bool is_messenger_in_group(t_err, R_messenger_name,
                                 p_messenger_user = nullptr)   const noexcept override final;
    t_void get_messenger_groups(t_err, r_messenger_group_list) const noexcept override final;

///////////////////////////////////////////////////////////////////////////////

    t_void create_messenger_group (t_err, R_messenger_password, R_messenger_name,
                                   t_messenger_scope) noexcept override final;
    t_void destroy_messenger_group(t_err, R_messenger_password,
                                   R_messenger_name) noexcept override final;
    t_bool is_messenger_group     (t_err, R_messenger_name, r_messenger_scope,
                                   p_messenger_name_list = nullptr) const noexcept override final;

///////////////////////////////////////////////////////////////////////////////

    t_void add_messenger_to_group(t_err, R_messenger_password,
                                  R_messenger_name name,
                                  R_messenger_name group,
                                  t_messenger_prio = t_messenger_prio(0),
                                  t_messenger_user = t_messenger_user()) noexcept override final;
    t_void remove_messenger_from_group(t_err, R_messenger_password,
                                       R_messenger_name,
                                       R_messenger_name group,
                                       p_messenger_user = nullptr) noexcept override final;
    t_bool is_messenger_in_group(t_err, R_messenger_name,
                                 R_messenger_name group,
                                 p_messenger_user = nullptr) const noexcept override final;

///////////////////////////////////////////////////////////////////////////////

  private:
    using r_ext_ = t_prefix<t_logic_messenger_ext>::r_;

    class t_impl_ : public t_logic_ext {
    public:
      using r_err = t_prefix<t_err>::r_;

      t_impl_(r_err, r_logic, r_ext_, R_logic_messenger_ext_params) noexcept;

///////////////////////////////////////////////////////////////////////////////

      operator t_validity() const noexcept;

///////////////////////////////////////////////////////////////////////////////

      t_void notify_start(t_err) noexcept override final;
      t_void notify_cleanup()    noexcept override final;

      t_void notify_timeout(t_timer_id,
                            R_timer_params)   noexcept override final;
      t_void notify_fdevent(t_fdevent_id,
                            R_fdevent_params) noexcept override final;

///////////////////////////////////////////////////////////////////////////////

      t_messenger_key  get_messenger_key_   ()                   const noexcept;
      t_messenger_name get_messenger_name_  ()                   const noexcept;
      t_void           get_messenger_params_(r_messenger_params) const noexcept;

///////////////////////////////////////////////////////////////////////////////

      t_void post_messenger_msg_(r_err, t_messenger_key,
                                 x_messenger_msg) noexcept;

///////////////////////////////////////////////////////////////////////////////

      t_void update_messenger_scope_       (r_err,
                                            t_messenger_scope)   noexcept;
      t_void update_messenger_alive_period_(r_err,
                                            t_multiple_of_100ms) noexcept;

///////////////////////////////////////////////////////////////////////////////

      t_messenger_msg_notify_id
        add_messenger_msg_notify_(r_err, R_messenger_msg_notify_params) noexcept;
      t_messenger_msg_notify_id
        add_messenger_msg_notify_(r_err, R_messenger_msg_notify_params,
                                  x_messenger_msg_notify_ptr)           noexcept;

      t_messenger_msg_notify_ptr
        remove_messenger_msg_notify_(t_messenger_msg_notify_id)       noexcept;
      P_messenger_msg_notify_params
        get_messenger_msg_notify_   (t_messenger_msg_notify_id) const noexcept;

///////////////////////////////////////////////////////////////////////////////

      t_messenger_monitor_id
        add_messenger_monitor_(r_err, R_messenger_name,
                               R_messenger_monitor_params)     noexcept;
      t_messenger_monitor_id
        add_messenger_monitor_(r_err, R_messenger_name,
                               R_messenger_monitor_params,
                               x_messenger_monitor_notify_ptr) noexcept;

      t_messenger_monitor_notify_ptr
        remove_messenger_monitor_(t_messenger_monitor_id)       noexcept;
      P_messenger_monitor_params
        get_messenger_monitor_   (t_messenger_monitor_id) const noexcept;

///////////////////////////////////////////////////////////////////////////////

      t_void add_messenger_to_group_(r_err, R_messenger_password,
                                     R_messenger_name group,
                                     t_messenger_prio,
                                     t_messenger_user)      noexcept;
      t_void remove_messenger_from_group_(r_err, R_messenger_password,
                                          R_messenger_name,
                                          p_messenger_user) noexcept;
      t_bool is_messenger_in_group_(r_err, R_messenger_name,
                                    p_messenger_user)   const noexcept;
      t_void get_messenger_groups_(r_err, r_messenger_group_list) const noexcept;

///////////////////////////////////////////////////////////////////////////////

      t_void create_messenger_group_ (r_err, R_messenger_password,
                                      R_messenger_name,
                                      t_messenger_scope) noexcept;
      t_void destroy_messenger_group_(r_err, R_messenger_password,
                                      R_messenger_name)  noexcept;
      t_bool is_messenger_group_     (r_err, R_messenger_name,
                                      r_messenger_scope,
                                      p_messenger_name_list) const noexcept;

///////////////////////////////////////////////////////////////////////////////

      t_void add_messenger_to_group_(r_err, R_messenger_password,
                                     R_messenger_name name,
                                     R_messenger_name group, t_messenger_prio,
                                     t_messenger_user) noexcept;
      t_void remove_messenger_from_group_(r_err, R_messenger_password,
                                          R_messenger_name, R_messenger_name,
                                          p_messenger_user) noexcept;
      t_bool is_messenger_in_group_(r_err, R_messenger_name,
                                    R_messenger_name group,
                                    p_messenger_user) const noexcept;

///////////////////////////////////////////////////////////////////////////////

    private:
      using t_maybe_messenger = t_maybe<messaging::t_messenger>;
      using t_messaging_err   = messaging::t_err;

      T_logic_messenger_ext_params params_;
      r_ext_                       ext_;
      t_maybe_messenger            messenger_;
      t_fdevent_id                 ev_id_;
      t_messaging_err              err_;
    };

    t_impl_ impl_;
  };

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
