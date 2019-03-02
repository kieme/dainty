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
        : t_logic_ext   {"messenger", logic},
          params_       {params},
          ext_          {ext},
          ev_id_        {BAD_FDEVENT_ID},
          mons_         {err, params_.monitors_max},
          monitors_     {err, params_.monitors_max},
          msg_notifs_   {err, params_.msg_notifiers_max},
          msg_notifiers_{err, params_.msg_notifiers_max} {
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
      name += "_messenger";
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
  }

  t_void t_logic_messenger_ext::t_impl_
      ::notify_fdevent(t_fdevent_id, R_fdevent_params) noexcept {
    t_messenger_msgs msgs{t_n{5}};
    set(messenger_).wait_message(err_, msgs);
    if (!err_) {
      // domain - system - alive/monitor
      // domain - user   - user
    } else {
    }
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
      return get(messenger_).get_name(err_);
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

  t_logic_messenger_ext::t_impl_::t_id_value_
    t_logic_messenger_ext::t_impl_
      ::generate_msg_notify_unique_id_() noexcept {
    return 0; // XXX
  }

  t_logic_messenger_ext::t_impl_::p_msg_notify_entry_
    t_logic_messenger_ext::t_impl_
      ::get_entry_(t_msg_notify_id id) noexcept {
    return msg_notifiers_.get(t_id_(get(id)));
  }

  t_logic_messenger_ext::t_impl_::P_msg_notify_entry_
    t_logic_messenger_ext::t_impl_
      ::get_entry_(t_msg_notify_id id) const noexcept {
    return msg_notifiers_.get(t_id_(get(id)));
  }

  t_logic_messenger_ext::t_impl_::t_msg_notify_id
    t_logic_messenger_ext::t_impl_
      ::mk_msg_notify_id_(t_id_ id, t_id_value_ value) const noexcept {
    return t_msg_notify_id(get(id)); // XXX - ignore unique
  }

  t_logic_messenger_ext::t_impl_::t_id_
    t_logic_messenger_ext::t_impl_
      ::get_msg_notifiers_id_(t_msg_notify_id id) const noexcept {
    return t_id_(get(id));
  }

  t_logic_messenger_ext::t_impl_::t_id_value_
    t_logic_messenger_ext::t_impl_
      ::get_unique_id_(t_msg_notify_id id) const noexcept {
    return 0; // XXX
  }

  t_messenger_msg_notify_id t_logic_messenger_ext::t_impl_
      ::add_messenger_msg_notify_(r_err err,
                                  R_messenger_msg_notify_params params) noexcept {
    return add_messenger_msg_notify_(err, params, {});
  }

  t_messenger_msg_notify_id t_logic_messenger_ext::t_impl_
      ::add_messenger_msg_notify_(r_err err,
                                  R_messenger_msg_notify_params params,
                                  x_messenger_msg_notify_ptr ptr) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID) {
        if (!msg_notifiers_.is_full()) {
          t_id_ id = msg_notifs_.find_if([&params](auto& entry) -> t_bool {
                       return entry.domain == params.domain;
                     });

          p_msg_notif_entry_ msg_notif_entry = nullptr;
          if (id != BAD_ID)
            msg_notif_entry = msg_notifs_.get(id);
          else {
            auto result = msg_notifs_.insert();
            msg_notif_entry = result.ptr;
            msg_notif_entry->domain = params.domain;
            id = result.id;
          }

          auto result = msg_notifiers_.insert();
          msg_notif_entry->ids.push_back(result.id);

          auto msg_id = mk_msg_notify_id_(result.id,
                                          generate_msg_notify_unique_id_());
          auto entry        = result.ptr;
          entry->id         = msg_id;
          entry->entry_id   = id;
          entry->notify_ptr = x_cast(ptr);
          entry->params     = params;
          return msg_id;
        } else
          err = err::E_XXX;
      } else
        err = err::E_XXX;
    }
    return BAD_MSG_NOTIFY_ID;
  }

  t_messenger_msg_notify_ptr t_logic_messenger_ext::t_impl_
      ::remove_messenger_msg_notify_(t_messenger_msg_notify_id _id) noexcept {
    t_messenger_msg_notify_ptr tmp;
    if (*this == VALID) {
      auto id    = get_msg_notifiers_id_(_id);
      auto entry = msg_notifiers_.get(id);
      if (entry) {
        if (entry->notify_ptr == VALID)
          tmp = x_cast(entry->notify_ptr);

        auto notif_entry = msg_notifs_.get(entry->entry_id);
        if (notif_entry->ids.get_size() == t_n{1})
          msg_notifs_.erase(entry->entry_id);
        else
          notif_entry->ids.erase(
            notif_entry->ids.find_if([id](auto id1) -> t_bool {
                                      return id == id1;
                                     }));
        msg_notifiers_.erase(id);
      }
    }
    return tmp;
  }

  P_messenger_msg_notify_params t_logic_messenger_ext::t_impl_
      ::get_messenger_msg_notify_(t_messenger_msg_notify_id id) const noexcept {
    if (*this == VALID) {
      auto entry = get_entry_(id);
      if (entry)
        return &entry->params;
    }
    return nullptr;
  }

///////////////////////////////////////////////////////////////////////////////

  t_logic_messenger_ext::t_impl_::t_id_value_
    t_logic_messenger_ext::t_impl_::generate_monitor_unique_id_() noexcept {
    return 0;
  }

  t_logic_messenger_ext::t_impl_::p_monitor_entry_
    t_logic_messenger_ext::t_impl_
      ::get_entry_(t_monitor_id id) noexcept {
    return monitors_.get(t_id_(get(id)));
  }

  t_logic_messenger_ext::t_impl_::P_monitor_entry_
    t_logic_messenger_ext::t_impl_
      ::get_entry_(t_monitor_id id) const noexcept {
    return monitors_.get(t_id_(get(id)));
  }

  t_logic_messenger_ext::t_impl_::t_monitor_id
    t_logic_messenger_ext::t_impl_
      ::mk_monitor_id_(t_id_ id, t_id_value_) const noexcept {
    return t_monitor_id(get(id));
  }

  t_logic_messenger_ext::t_impl_::t_id_
    t_logic_messenger_ext::t_impl_
      ::get_monitor_id_(t_monitor_id id) const noexcept {
    return t_id_(get(id));
  }

  t_logic_messenger_ext::t_impl_::t_id_value_
    t_logic_messenger_ext::t_impl_
      ::get_unique_id_(t_monitor_id) const noexcept {
    return 0;
  }

  t_messenger_monitor_id t_logic_messenger_ext::t_impl_
      ::add_messenger_monitor_(r_err err,
                               R_messenger_name name,
                               R_messenger_monitor_params params) noexcept {
    return add_messenger_monitor_(err, name, params, {});
  }

  t_messenger_monitor_id t_logic_messenger_ext::t_impl_
      ::add_messenger_monitor_(r_err err,
                               R_messenger_name name,
                               R_messenger_monitor_params params,
                               x_messenger_monitor_notify_ptr ptr) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID) {
        if (!monitors_.is_full()) {
          t_id_ id = mons_.find_if([&params](auto& entry) -> t_bool {
                       return entry.name == params.name;
                     });

          p_mon_entry_ mon_entry = nullptr;
          if (id != BAD_ID)
            mon_entry = mons_.get(id);
          else {
            auto result = mons_.insert();
            mon_entry = result.ptr;
            mon_entry->name = params.name;
            id = result.id;

            set(messenger_).add_monitor(err, params.name.mk_range());
          }
          if (!err) {
            auto result = monitors_.insert();
            mon_entry->ids.push_back(result.id);

            auto mon_id = mk_monitor_id_(result.id,
                                         generate_monitor_unique_id_());
            auto entry        = result.ptr;
            entry->id         = mon_id;
            entry->entry_id   = id;
            entry->notify_ptr = x_cast(ptr);
            entry->params     = params;
            return mon_id;
          }
          mons_.erase(id);
        } else
          err = err::E_XXX;
      } else
        err = err::E_XXX;
    }
    return BAD_MONITOR_ID;
  }

  t_messenger_monitor_notify_ptr t_logic_messenger_ext::t_impl_
      ::remove_messenger_monitor_(t_messenger_monitor_id _id) noexcept {
    t_messenger_monitor_notify_ptr tmp;
    if (*this == VALID) {
      auto id    = get_monitor_id_(_id);
      auto entry = monitors_.get(id);
      if (entry) {
        if (entry->notify_ptr == VALID)
          tmp = x_cast(entry->notify_ptr);

        auto mon_entry = mons_.get(entry->entry_id);
        if (mon_entry->ids.get_size() == t_n{1}) {
          set(messenger_).remove_monitor(err_, mon_entry->name.mk_range());
          mons_.erase(entry->entry_id);
        } else
          mon_entry->ids.erase(
            mon_entry->ids.find_if([id](auto id1) -> t_bool {
                                      return id == id1;
                                   }));
        monitors_.erase(id);
      }
    }
    return tmp;
  }

  P_messenger_monitor_params t_logic_messenger_ext::t_impl_
      ::get_messenger_monitor_(t_messenger_monitor_id id) const noexcept {
    if (*this == VALID) {
      auto entry = get_entry_(id);
      if (entry)
        return &entry->params;
    }
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
                                R_messenger_name group,
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
