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
#include "dainty_sandbox_logic_ext_messenger.h"

namespace dainty
{
namespace sandbox
{
namespace logic_messenger_ext
{
  using named::utility::x_cast;

///////////////////////////////////////////////////////////////////////////////

  t_logic_messenger_ext::t_impl_::t_impl_(r_err err, r_logic logic,
                                          r_ext_ ext) noexcept
      : t_logic_ext{"messenger", logic}, ext_{ext} {
    ERR_GUARD(err) {
      register_(err, logic, this);
    }
  }

  t_logic_messenger_ext::t_impl_::operator t_validity() const noexcept {
    return INVALID;
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_logic_messenger_ext::t_impl_::notify_start(t_err err) noexcept {
    ERR_GUARD(err) {
      ext_.notify_messenger_start(err);
    }
  }

  t_void t_logic_messenger_ext::t_impl_::notify_cleanup() noexcept {
    ext_.notify_messenger_cleanup();
  }

  t_void t_logic_messenger_ext::t_impl_
      ::notify_timeout(t_timer_id, R_timer_params) noexcept {
  }

  t_void t_logic_messenger_ext::t_impl_
      ::notify_fdevent(t_fdevent_id, R_fdevent_params) noexcept {
  }

///////////////////////////////////////////////////////////////////////////////

  t_messenger_key t_logic_messenger_ext::t_impl_::get_key_() const noexcept {
    return t_messenger_key{0};
  }

  t_messenger_name t_logic_messenger_ext::t_impl_
      ::get_name_() const noexcept {
    return t_messenger_name{};
  }

  t_void t_logic_messenger_ext::t_impl_
      ::get_params_(r_messenger_params) const noexcept {
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_logic_messenger_ext::t_impl_
      ::post_msg_(r_err, t_messenger_key, x_messenger_msg) noexcept {
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_logic_messenger_ext::t_impl_
      ::update_scope_(r_err, t_messenger_scope) noexcept {
  }

  t_void t_logic_messenger_ext::t_impl_
      ::update_alive_period_(r_err, t_multiple_of_100ms) noexcept {
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_logic_messenger_ext::t_impl_
      ::add_monitor_(r_err, R_messenger_name, t_messenger_prio,
                     t_messenger_user) noexcept {
  }

  t_void t_logic_messenger_ext::t_impl_
      ::remove_monitor_(r_err, R_messenger_name,
                        p_messenger_user) noexcept {
  }

  t_messenger_key t_logic_messenger_ext::t_impl_
      ::is_monitored_(r_err, R_messenger_name,
                      p_messenger_user) const noexcept {
    return t_messenger_key{0};
  }

  t_void t_logic_messenger_ext::t_impl_
      ::get_monitored_(r_err, r_messenger_monitor_list) const noexcept {
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_logic_messenger_ext::t_impl_
      ::add_to_group_(r_err, R_messenger_password,
                      R_messenger_name group, t_messenger_prio,
                      t_messenger_user) noexcept {
  }

  t_void t_logic_messenger_ext::t_impl_
      ::remove_from_group_(r_err, R_messenger_password,
                           R_messenger_name,
                           p_messenger_user) noexcept {
  }

  t_bool t_logic_messenger_ext::t_impl_
      ::is_in_group_(r_err, R_messenger_name,
                     p_messenger_user) const noexcept {
    return false;
  }

  t_void t_logic_messenger_ext::t_impl_
      ::get_groups_(r_err, r_messenger_group_list) const noexcept {
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_logic_messenger_ext::t_impl_
      ::create_group_(r_err, R_messenger_password, R_messenger_name,
                      t_messenger_scope) noexcept {
  }

  t_void t_logic_messenger_ext::t_impl_
      ::destroy_group_(r_err, R_messenger_password,
                       R_messenger_name) noexcept {
  }

  t_bool t_logic_messenger_ext::t_impl_
      ::is_group_(r_err, R_messenger_name, r_messenger_scope,
                  p_messenger_name_list) const noexcept {
    return false;
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_logic_messenger_ext::t_impl_
      ::add_to_group_(r_err, R_messenger_password, R_messenger_name name,
                      R_messenger_name group, t_messenger_prio,
                      t_messenger_user) noexcept {
  }

  t_void t_logic_messenger_ext::t_impl_
      ::remove_from_group_(r_err, R_messenger_password, R_messenger_name,
                           R_messenger_name group,
                           p_messenger_user) noexcept {
  }

  t_bool t_logic_messenger_ext::t_impl_
      ::is_in_group_(r_err, R_messenger_name,
                     R_messenger_name group,
                     p_messenger_user) const noexcept {
    return false;
  }

///////////////////////////////////////////////////////////////////////////////

  t_logic_messenger_ext::t_logic_messenger_ext(t_err err,
                                               r_logic logic) noexcept
    : impl_{err, logic, *this} {
  }

  t_logic_messenger_ext::operator t_validity() const noexcept {
    return impl_;
  }

///////////////////////////////////////////////////////////////////////////////

  t_logic_messenger_ext::~t_logic_messenger_ext() {
  }

  t_messenger_key t_logic_messenger_ext::get_key() const noexcept {
    return impl_.get_key_();
  }

  t_messenger_name t_logic_messenger_ext::get_name() const noexcept {
    return impl_.get_name_();
  }

  t_void t_logic_messenger_ext
      ::get_params(r_messenger_params params) const noexcept {
    return impl_.get_params_(params);
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_logic_messenger_ext::post_msg(t_err err, t_messenger_key key,
                                         x_messenger_msg msg) noexcept {
    impl_.post_msg_(err, key, x_cast(msg));
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_logic_messenger_ext::update_scope(t_err err,
                                             t_messenger_scope scope) noexcept {
    impl_.update_scope_(err, scope);
  }

  t_void t_logic_messenger_ext
      ::update_alive_period(t_err err, t_multiple_of_100ms multiple) noexcept {
    impl_.update_alive_period_(err, multiple);
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_logic_messenger_ext::add_monitor(t_err err,
                                            R_messenger_name name,
                                            t_messenger_prio prio,
                                            t_messenger_user user) noexcept {
    impl_.add_monitor_(err, name, prio, user);
  }

  t_void t_logic_messenger_ext::remove_monitor(t_err err,
                                               R_messenger_name name,
                                               p_messenger_user user) noexcept {
    ERR_GUARD(err) {
      impl_.remove_monitor_(err, name, user);
    }
  }

  t_messenger_key t_logic_messenger_ext
      ::is_monitored(t_err err, R_messenger_name name,
                     p_messenger_user user) const noexcept {
    return impl_.is_monitored_(err, name, user);
  }

  t_void t_logic_messenger_ext
      ::get_monitored(t_err err, r_messenger_monitor_list list) const noexcept {
    impl_.get_monitored_(err, list);
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_logic_messenger_ext::add_to_group(t_err err,
                                             R_messenger_password password,
                                             R_messenger_name group,
                                             t_messenger_prio prio,
                                             t_messenger_user user) noexcept {
    impl_.add_to_group_(err, password, group, prio, user);
  }

  t_void t_logic_messenger_ext
      ::remove_from_group(t_err err, R_messenger_password password,
                          R_messenger_name name,
                          p_messenger_user user) noexcept {
    impl_.remove_from_group_(err, password, name, user);
  }

  t_bool t_logic_messenger_ext
      ::is_in_group(t_err err, R_messenger_name name,
                    p_messenger_user user) const noexcept {
    return impl_.is_in_group_(err, name, user);
  }

  t_void t_logic_messenger_ext
      ::get_groups(t_err err, r_messenger_group_list list) const noexcept {
    ERR_GUARD(err) {
      impl_.get_groups_(err, list);
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_logic_messenger_ext
      ::create_group(t_err err, R_messenger_password password,
                     R_messenger_name name,
                     t_messenger_scope scope) noexcept {
    impl_.create_group_(err, password, name, scope);
  }

  t_void t_logic_messenger_ext
      ::destroy_group(t_err err, R_messenger_password password,
                      R_messenger_name name) noexcept {
    impl_.destroy_group_(err, password, name);
  }

  t_bool t_logic_messenger_ext
      ::is_group(t_err err, R_messenger_name name,
                 r_messenger_scope scope,
                 p_messenger_name_list list) const noexcept {
    return impl_.is_group_(err, name, scope, list);
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_logic_messenger_ext
      ::add_to_group(t_err err, R_messenger_password password,
                     R_messenger_name name, R_messenger_name group,
                     t_messenger_prio prio, t_messenger_user user) noexcept {
    impl_.add_to_group_(err, password, name, group, prio, user);
  }

  t_void t_logic_messenger_ext
      ::remove_from_group(t_err err, R_messenger_password password,
                          R_messenger_name name, R_messenger_name group,
                          p_messenger_user user) noexcept {
    impl_.remove_from_group_(err, password, name, group, user);
  }

  t_bool t_logic_messenger_ext
      ::is_in_group(t_err err, R_messenger_name name, R_messenger_name group,
                    p_messenger_user user) const noexcept {
    return impl_.is_in_group_(err, name, group, user);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}
