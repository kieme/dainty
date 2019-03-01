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

#include "dainty_named_utility.h"
#include "dainty_messaging.h"
#include "dainty_sandbox_logic_ext_messenger.h"

namespace dainty
{
namespace sandbox
{
namespace logic_messenger_ext
{
  using named::utility::x_cast;

///////////////////////////////////////////////////////////////////////////////

  t_logic_messenger_ext::t_impl_
      ::t_impl_(r_err err, r_logic logic, r_ext_ ext,
                R_logic_messenger_ext_params params) noexcept
        : t_logic_ext{"messenger", logic}, params_{params}, ext_{ext},
          ev_id_{BAD_FDEVENT_ID} {
    ERR_GUARD(err) {
      register_(err, logic, this);
    }
  }

  t_logic_messenger_ext::t_impl_::operator t_validity() const noexcept {
    return messenger_ == VALID &&
           get(messenger_) == VALID && !err_ ? VALID : INVALID;
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_logic_messenger_ext::t_impl_::notify_start(t_err err) noexcept {
    ERR_GUARD(err) {
      t_messenger_name name{get_logic_name().mk_range()};
      messenger_.emplace(messaging::create_messenger(err.tag(1), name,
                                                     {params_.scope,
                                                      params_.alive_period}));
      ev_id_ = add_fdevent(err.tag(2), "messenger", {set(messenger_).get_fd(),
                                                     FDEVENT_READ});
      ext_.notify_messenger_start(err.tag(3));

      if (err) {
        switch (err.tag()) {
          case 3: del_fdevent(ev_id_);
          case 2: messenger_.release();
          case 1:
            break;
        }
      }
    }
  }

  t_void t_logic_messenger_ext::t_impl_::notify_cleanup() noexcept {
    del_fdevent(ev_id_);
    ext_.notify_messenger_cleanup();
    messenger_.release();
    // clear everything explicitly - don't wait for deletion
  }

  t_void t_logic_messenger_ext::t_impl_
      ::notify_timeout(t_timer_id, R_timer_params) noexcept {
    //
  }

  t_void t_logic_messenger_ext::t_impl_
      ::notify_fdevent(t_fdevent_id, R_fdevent_params) noexcept {
    // means data came in - must look at domain - forward message.
    // messenger file descriptor
  }

///////////////////////////////////////////////////////////////////////////////

  t_messenger_key t_logic_messenger_ext
      ::t_impl_::get_messenger_key_() const noexcept {
    if (*this == VALID)
      return get(messenger_).get_key();
    return t_messenger_key{0};
  }

  t_messenger_name t_logic_messenger_ext::t_impl_
      ::get_messenger_name_() const noexcept {
    if (*this == VALID)
      return get(messenger_).get_name(err_); //XXX
    return t_messenger_name{};
  }

  t_void t_logic_messenger_ext::t_impl_
      ::get_messenger_params_(r_messenger_params params) const noexcept {
    if (*this == VALID)
      get(messenger_).get_params(err_, params);
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_logic_messenger_ext::t_impl_
      ::post_messenger_msg_(r_err err, t_messenger_key key,
                            x_messenger_msg msg) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        set(messenger_).post_message(err, key, x_cast(msg));
      else
        err = err::E_XXX;
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_logic_messenger_ext::t_impl_
      ::update_messenger_scope_(r_err err, t_messenger_scope scope) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        set(messenger_).update_scope(err, scope);
      else
        err = err::E_XXX;
    }
  }

  t_void t_logic_messenger_ext::t_impl_
      ::update_messenger_alive_period_(r_err err,
                                       t_multiple_of_100ms alive) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        set(messenger_).update_alive_period(err, alive);
      else
        err = err::E_XXX;
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_messenger_msg_notify_id t_logic_messenger_ext::t_impl_
      ::add_messenger_msg_notify_(r_err err,
                                  R_messenger_msg_notify_params params) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID) {
        // XXX
      } else
        err = err::E_XXX;
    }
    return 0;
  }

  t_messenger_msg_notify_id t_logic_messenger_ext::t_impl_
      ::add_messenger_msg_notify_(r_err err,
                                  R_messenger_msg_notify_params params,
                                  x_messenger_msg_notify_ptr ptr) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID) {
        // XXX
      } else
        err = err::E_XXX;
    }
    return 0;
  }

  t_messenger_msg_notify_ptr t_logic_messenger_ext::t_impl_
      ::remove_messenger_msg_notify_(t_messenger_msg_notify_id id) noexcept {
    // XXX
    return {};
  }

  P_messenger_msg_notify_params t_logic_messenger_ext::t_impl_
      ::get_messenger_msg_notify_(t_messenger_msg_notify_id id) const noexcept {
    // XXX
    return nullptr;
  }

///////////////////////////////////////////////////////////////////////////////

  t_messenger_monitor_id t_logic_messenger_ext::t_impl_
      ::add_messenger_monitor_(r_err err,
                               R_messenger_name name,
                               R_messenger_monitor_params params) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID) {
        // XXX
      } else
        err = err::E_XXX;
    }
    return 0;
  }

  t_messenger_monitor_id t_logic_messenger_ext::t_impl_
      ::add_messenger_monitor_(r_err err,
                               R_messenger_name name,
                               R_messenger_monitor_params params,
                               x_messenger_monitor_notify_ptr ptr) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID) {
        // XXX
      } else
        err = err::E_XXX;
    }
    return 0;
  }

  t_messenger_monitor_notify_ptr t_logic_messenger_ext::t_impl_
      ::remove_messenger_monitor_(t_messenger_monitor_id id) noexcept {
    // XXX
    return {};
  }

  P_messenger_monitor_params t_logic_messenger_ext::t_impl_
      ::get_messenger_monitor_(t_messenger_monitor_id id) const noexcept {
    // XXX
    return nullptr;
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_logic_messenger_ext::t_impl_
      ::add_messenger_to_group_(r_err err,
                                R_messenger_password password,
                                R_messenger_name group,
                                t_messenger_prio prio,
                                t_messenger_user user) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        set(messenger_).add_to_group(err, password, group, prio, user);
      else
        err = err::E_XXX;
    }
  }

  t_void t_logic_messenger_ext::t_impl_
      ::remove_messenger_from_group_(r_err err,
                                     R_messenger_password password,
                                     R_messenger_name group,
                                     p_messenger_user user) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        set(messenger_).remove_from_group(err, password, group, user);
      else
        err = err::E_XXX;
    }
  }

  t_bool t_logic_messenger_ext::t_impl_
      ::is_messenger_in_group_(r_err err,
                               R_messenger_name group,
                               p_messenger_user user) const noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        return get(messenger_).is_in_group(err, group, user);
      err = err::E_XXX;
    }
    return false;
  }

  t_void t_logic_messenger_ext::t_impl_
      ::get_messenger_groups_(r_err err,
                              r_messenger_group_list list) const noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        get(messenger_).get_groups(err, list);
      else
        err = err::E_XXX;
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_logic_messenger_ext::t_impl_
      ::create_messenger_group_(r_err err,
                                R_messenger_password password,
                                R_messenger_name  group,
                                t_messenger_scope scope) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        messaging::create_group(err, password, group, scope);
      else
        err = err::E_XXX;
    }
  }

  t_void t_logic_messenger_ext::t_impl_
      ::destroy_messenger_group_(r_err err,
                                 R_messenger_password password,
                                 R_messenger_name group) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        messaging::destroy_group(err, password, group);
      else
        err = err::E_XXX;
    }
  }

  t_bool t_logic_messenger_ext::t_impl_
      ::is_messenger_group_(r_err err,
                            R_messenger_name group,
                            r_messenger_scope scope,
                            p_messenger_name_list list) const noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        return messaging::is_group(err, group, scope, list);
      err = err::E_XXX;
    }
    return false;
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_logic_messenger_ext::t_impl_
      ::add_messenger_to_group_(r_err err,
                                R_messenger_password password,
                                R_messenger_name name,
                                R_messenger_name group,
                                t_messenger_prio prio,
                                t_messenger_user user) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        messaging::add_messenger_to_group(err, password, name, group,
                                          prio, user);
      else
        err = err::E_XXX;
    }
  }

  t_void t_logic_messenger_ext::t_impl_
      ::remove_messenger_from_group_(r_err err,
                                     R_messenger_password password,
                                     R_messenger_name name,
                                     R_messenger_name group,
                                     p_messenger_user user) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        messaging::remove_messenger_from_group(err, password, name, group,
                                               user);
      else
        err = err::E_XXX;
    }
  }

  t_bool t_logic_messenger_ext::t_impl_
      ::is_messenger_in_group_(r_err err,
                               R_messenger_name name,
                               R_messenger_name group,
                               p_messenger_user user) const noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        return messaging::is_messenger_in_group(err, name, group, user);
      err = err::E_XXX;
    }
    return false;
  }

///////////////////////////////////////////////////////////////////////////////

  t_logic_messenger_ext
    ::t_logic_messenger_ext(t_err err, r_logic logic,
                            R_logic_messenger_ext_params params) noexcept
      : impl_{err, logic, *this, params} {
  }

  t_logic_messenger_ext::~t_logic_messenger_ext() {
  }

  t_logic_messenger_ext::operator t_validity() const noexcept {
    return impl_;
  }

  t_messenger_key t_logic_messenger_ext::get_messenger_key() const noexcept {
    return impl_.get_messenger_key_();
  }

  t_messenger_name t_logic_messenger_ext::get_messenger_name() const noexcept {
    return impl_.get_messenger_name_();
  }

  t_void t_logic_messenger_ext
      ::get_messenger_params(r_messenger_params params) const noexcept {
    return impl_.get_messenger_params_(params);
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_logic_messenger_ext
      ::post_messenger_msg(t_err err, t_messenger_key key,
                           x_messenger_msg msg) noexcept {
    impl_.post_messenger_msg_(err, key, x_cast(msg));
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_logic_messenger_ext
      ::update_messenger_scope(t_err err, t_messenger_scope scope) noexcept {
    impl_.update_messenger_scope_(err, scope);
  }

  t_void t_logic_messenger_ext
      ::update_messenger_alive_period(t_err err,
                                      t_multiple_of_100ms multiple) noexcept {
    impl_.update_messenger_alive_period_(err, multiple);
  }

///////////////////////////////////////////////////////////////////////////////

  t_messenger_msg_notify_id t_logic_messenger_ext
      ::add_messenger_msg_notify(t_err err,
                                 R_messenger_msg_notify_params params) noexcept {
    return impl_.add_messenger_msg_notify_(err, params);
  }

  t_messenger_msg_notify_id t_logic_messenger_ext
      ::add_messenger_msg_notify(t_err err,
                                 R_messenger_msg_notify_params params,
                                 x_messenger_msg_notify_ptr ptr) noexcept {
    return impl_.add_messenger_msg_notify_(err, params, x_cast(ptr));
  }

  t_messenger_msg_notify_ptr t_logic_messenger_ext
      ::remove_messenger_msg_notify(t_messenger_msg_notify_id id) noexcept {
    return impl_.remove_messenger_msg_notify_(id);
  }

  P_messenger_msg_notify_params t_logic_messenger_ext
      ::get_messenger_msg_notify(t_messenger_msg_notify_id id) const noexcept {
    return impl_.get_messenger_msg_notify_(id);
  }

///////////////////////////////////////////////////////////////////////////////

  t_messenger_monitor_id t_logic_messenger_ext
      ::add_messenger_monitor(t_err err, R_messenger_name name,
                              R_messenger_monitor_params params) noexcept {
    return impl_.add_messenger_monitor_(err, name, params);
  }

  t_messenger_monitor_id t_logic_messenger_ext
      ::add_messenger_monitor(t_err err, R_messenger_name name,
                              R_messenger_monitor_params params,
                              x_messenger_monitor_notify_ptr ptr) noexcept {
    return impl_.add_messenger_monitor_(err, name, params, x_cast(ptr));
  }

  t_messenger_monitor_notify_ptr t_logic_messenger_ext
      ::remove_messenger_monitor(t_messenger_monitor_id id) noexcept {
    return impl_.remove_messenger_monitor_(id);
  }

  P_messenger_monitor_params t_logic_messenger_ext
      ::get_messenger_monitor(t_messenger_monitor_id id) const noexcept {
    return impl_.get_messenger_monitor_(id);
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_logic_messenger_ext
      ::add_messenger_to_group(t_err err,
                               R_messenger_password password,
                               R_messenger_name group,
                               t_messenger_prio prio,
                               t_messenger_user user) noexcept {
    impl_.add_messenger_to_group_(err, password, group, prio, user);
  }

  t_void t_logic_messenger_ext
      ::remove_messenger_from_group(t_err err, R_messenger_password password,
                                    R_messenger_name name,
                                    p_messenger_user user) noexcept {
    impl_.remove_messenger_from_group_(err, password, name, user);
  }

  t_bool t_logic_messenger_ext
      ::is_messenger_in_group(t_err err, R_messenger_name name,
                              p_messenger_user user) const noexcept {
    return impl_.is_messenger_in_group_(err, name, user);
  }

  t_void t_logic_messenger_ext
      ::get_messenger_groups(t_err err,
                             r_messenger_group_list list) const noexcept {
    ERR_GUARD(err) {
      impl_.get_messenger_groups_(err, list);
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_logic_messenger_ext
      ::create_messenger_group(t_err err, R_messenger_password password,
                               R_messenger_name name,
                               t_messenger_scope scope) noexcept {
    impl_.create_messenger_group_(err, password, name, scope);
  }

  t_void t_logic_messenger_ext
      ::destroy_messenger_group(t_err err, R_messenger_password password,
                                R_messenger_name name) noexcept {
    impl_.destroy_messenger_group_(err, password, name);
  }

  t_bool t_logic_messenger_ext
      ::is_messenger_group(t_err err, R_messenger_name name,
                           r_messenger_scope scope,
                           p_messenger_name_list list) const noexcept {
    return impl_.is_messenger_group_(err, name, scope, list);
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_logic_messenger_ext
      ::add_messenger_to_group(t_err err, R_messenger_password password,
                               R_messenger_name name, R_messenger_name group,
                               t_messenger_prio prio,
                               t_messenger_user user) noexcept {
    impl_.add_messenger_to_group_(err, password, name, group, prio, user);
  }

  t_void t_logic_messenger_ext
      ::remove_messenger_from_group(t_err err, R_messenger_password password,
                                    R_messenger_name name,
                                    R_messenger_name group,
                                    p_messenger_user user) noexcept {
    impl_.remove_messenger_from_group_(err, password, name, group, user);
  }

  t_bool t_logic_messenger_ext
      ::is_messenger_in_group(t_err err, R_messenger_name name,
                              R_messenger_name group,
                              p_messenger_user user) const noexcept {
    return impl_.is_messenger_in_group_(err, name, group, user);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}
