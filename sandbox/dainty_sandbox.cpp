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

#include <iostream>
#include "dainty_named_utility.h"
#include "dainty_mt_event_dispatcher.h"
#include "dainty_sandbox.h"

///////////////////////////////////////////////////////////////////////////////

namespace dainty
{
namespace sandbox
{
  using namespace named::utility;
  using named::P_cstr;
  using messaging::t_messenger;
  using messaging::message::t_message;;
  using mt::event_dispatcher::RD;
  using mt::event_dispatcher::QUIT_EVENT_LOOP;
  using t_key                     = t_logic::t_messenger_key;
  using t_dispatcher_             = mt::event_dispatcher::t_dispatcher;
  using t_dispatcher_logic_       = t_dispatcher_::t_logic;
  using t_dispatcher_event_logic_ = mt::event_dispatcher::t_event_logic;

///////////////////////////////////////////////////////////////////////////////

  struct t_kill_message : public t_message {
    t_kill_message() : t_message{t_n{0}} {
      set(t_id{t_domain{1}, t_user{1}, t_version{0}}, t_n{0});
    }
  };

///////////////////////////////////////////////////////////////////////////////

  class t_impl_ : public t_dispatcher_logic_,
                  public t_dispatcher_event_logic_ {
  public:
    using t_logic_                  = sandbox::t_logic;
    using t_err                     = t_logic_::t_err;
    using r_err                     = t_prefix<t_err>::r_;
    using t_fd                      = t_logic_::t_fd;
    using t_stats                   = t_logic_::t_stats;
    using t_label                   = t_logic_::t_label;
    using t_message_logic           = t_logic_::t_message_logic;
    using R_password                = t_logic_::R_password;
    using t_message                 = t_logic_::t_message;
    using x_message                 = t_logic_::x_message;
    using t_messages                = t_logic_::t_messages;
    using t_message_id              = t_logic_::t_message_id;
    using R_message_id              = t_logic_::R_message_id;
    using t_multiple_of_100ms       = t_logic_::t_multiple_of_100ms;
    using t_messenger_scope         = t_logic_::t_messenger_scope;
    using r_messenger_scope         = t_logic_::r_messenger_scope;
    using p_messenger_name_list     = t_logic_::p_messenger_name_list;
    using R_messenger_password      = t_logic_::R_messenger_password;
    using t_messenger_prio          = t_logic_::t_messenger_prio;
    using t_messenger_user          = t_logic_::t_messenger_user;
    using p_messenger_user          = t_logic_::p_messenger_user;
    using t_messenger_name          = t_logic_::t_messenger_name;
    using R_messenger_name          = t_logic_::R_messenger_name;
    using t_messenger_key           = t_logic_::t_messenger_key;
    using R_messenger_key           = t_logic_::R_messenger_key;
    using t_messenger_state         = t_logic_::t_messenger_state;
    using t_messenger_params        = t_logic_::t_messenger_params;
    using r_messenger_params        = t_logic_::r_messenger_params;
    using r_messenger_timer_params  = t_logic_::r_messenger_timer_params;
    using R_messenger_timer_params  = t_logic_::R_messenger_timer_params;
    using r_messenger_group_list    = t_logic_::r_messenger_group_list;
    using r_messenger_monitor_list  = t_logic_::r_messenger_monitor_list;
    using R_messenger_create_params = t_logic_::R_messenger_create_params;

    t_impl_(r_err err, p_logic logic, R_messenger_name name,
            R_messenger_create_params params) : logic_{logic},
            dispatcher_{err, {t_n{100}, P_cstr{"epoll_service"}}},
            msgr_{messaging::create_messenger(err, name, {})} {
            // timer
      ERR_GUARD(err) {
        dispatcher_.add_event(err, {msgr_.get_fd(), RD}, this);
        //dispatcher_.add_event(err, {msgr_.get_fd(), RD}, this); timer
      }
      valid_ = err ? INVALID : VALID;
    }

    ~t_impl_() {
    }

    operator t_validity() const {
      return valid_;
    }

    t_messenger_key get_key() const {
      return msgr_.get_key();
    }

    t_messenger_name get_name(r_err err) const {
      return msgr_.get_name(err);
    }

    t_void get_params(r_err err, r_messenger_params params) const {
      return msgr_.get_params(err, params);
    }

    t_void get_stats(r_err, r_stats, t_bool) {
      //XXX - counters for everything
    }

    t_void post_message(r_err err, R_messenger_key key, x_message msg) {
      msgr_.post_message(err, key, x_cast(msg));
    }

    t_void update_scope(r_err err, t_messenger_scope scope) {
      msgr_.update_scope(err, scope);
    }

    t_void update_alive_period(r_err err, t_multiple_of_100ms factor) {
      msgr_.update_alive_period(err, factor);
    }

    t_void start_message_timer(r_err err, R_messenger_timer_params params) {
      msgr_.start_timer(err, params);
    }

    t_void stop_message_timer(r_err err) {
      msgr_.stop_timer(err);
    }

    t_void query_message_timer(r_err err,
                               r_messenger_timer_params params) const {
      msgr_.query_timer(err, params);
    }

    t_void add_monitor(r_err err, R_messenger_name name, t_messenger_prio prio,
                       t_messenger_user user) {
      msgr_.add_monitor(err, name, prio, user);
    }

    t_void remove_monitor(r_err err, R_messenger_name name,
                          p_messenger_user user) {
      msgr_.remove_monitor(err, name, user);
    }

    t_messenger_key is_monitored(r_err err, R_messenger_name name,
                                 p_messenger_user user) const {
      return msgr_.is_monitored(err, name, user);
    }

    t_void get_monitored(r_err err, r_messenger_monitor_list list) const {
      msgr_.get_monitored(err, list);
    }

    t_void add_to_group(r_err err, R_messenger_password password,
                        R_messenger_name group, t_messenger_prio prio,
                        t_messenger_user user) {
      msgr_.add_to_group(err, password, group, prio, user);
    }

    t_void remove_from_group(r_err err, R_messenger_password password,
                             R_messenger_name group, p_messenger_user user) {
      msgr_.remove_from_group(err, password, group, user);
    }

    t_bool is_in_group(r_err err, R_messenger_name group,
                       p_messenger_user user) const {
      return msgr_.is_in_group(err, group, user);
    }

    t_void get_groups(r_err err, r_messenger_group_list list) {
      msgr_.get_groups(err, list);
    }

    t_void create_group(r_err err, R_password password, R_messenger_name group,
                               t_messenger_scope scope) {
      messaging::create_group(err, password, group, scope);
    }

    t_void destroy_group(r_err err, R_password password,
                         R_messenger_name group) {
      messaging::destroy_group(err, password, group);
    }

    t_bool is_group(r_err err, R_messenger_name group,
                    r_messenger_scope scope, p_messenger_name_list list) {
      return messaging::is_group(err, group, scope, list);
    }

    t_void add_another_to_group(r_err err, R_messenger_password password,
                                R_messenger_name name,
                                R_messenger_name group, t_messenger_prio prio,
                                t_messenger_user user) {
      messaging::add_messenger_to_group(err, password, name, group, prio, user);
    }

    t_void remove_another_from_group(r_err err, R_messenger_password password,
                                     R_messenger_name name,
                                     R_messenger_name group,
                                     p_messenger_user user) {
      messaging::remove_messenger_from_group(err, password, name, group, user);
    }

    t_bool is_another_in_group(r_err err, R_messenger_name name,
                               R_messenger_name group, p_messenger_user user) {
      return messaging::is_messenger_in_group(err, name, group, user);
    }

    t_msec get_max_wait() const {
      return t_msec{0};
    }

    t_void set_max_wait(r_err, t_msec) {
    }

    t_void register_message_logic(r_err, R_message_id, R_label,
                                         p_message_logic) {
    }

    p_message_logic unregister_message_logic(r_err, R_label) {
      return nullptr;
    }

    p_message_logic is_message_logic_registered(r_err, R_label) const {
      return nullptr;
    }

    t_void add_event_monitor(r_err, t_fd, R_label, t_messenger_user) {
    }

    t_void remove_event_monitor(r_err, t_fd, p_label, p_messenger_user) {
    }

    t_bool is_event_monitored(r_err, t_fd, p_label, p_messenger_user) {
      return true;
    }

    t_void update(t_err, os::r_pthread_attr) noexcept {
    }

    t_void prepare(t_err) noexcept {
    }

    t_void run() noexcept {
      t_err err;

      do {
        if (get(timeout_))
          dispatcher_.event_loop(err, this, t_usec{get(timeout_)*1000});
        else
          dispatcher_.event_loop(err, this);
      } while (!quit_ && !err);

      if (err) {
        err.clear();
      }
      std::cout << "sandbox died" << std::endl;
    }

    virtual t_void may_reorder_events(r_event_infos) override {
      // wakeup
    }

    virtual t_void notify_event_remove(r_event_info) override {
    }

    virtual t_quit notify_timeout(t_usec) override {
      return true;
    }

    virtual t_quit notify_error(t_errn) override {
      return true;
    }

    virtual t_quit notify_events_processed() override {
      return true;
    }

    virtual t_name get_name() const override {
      return t_name{};
    }

    virtual t_action notify_event(r_event_params params) override {
      quit_ = true;
      return {QUIT_EVENT_LOOP};
    }

  private:
    p_logic         logic_;
    t_dispatcher_   dispatcher_;
    t_messenger     msgr_;
    t_bool          quit_    = false;
    t_msec          timeout_ = t_msec{0};
    t_validity      valid_   = INVALID;
    t_messenger_key key_     = t_messenger_key{0};
  };

///////////////////////////////////////////////////////////////////////////////

  t_logic::t_logic(t_err err, R_messenger_name name,
                   R_messenger_create_params params)
      : tracer_{tracing::make_tracer(err, name.mk_range())} {
    ERR_GUARD(err) {
      impl_ = new t_impl_{err, this, name, params};
      if (impl_ == INVALID)
        err = err::E_XXX;
      valid_ = err ? INVALID : VALID;
    }
  }

  t_logic::~t_logic() {
  }

  t_logic::operator t_validity() const {
    return valid_;
  }

  t_logic::t_messenger_key t_logic::get_key() const {
    if (impl_ == VALID && *impl_ == VALID)
      return impl_->get_key();
    return t_messenger_key{0};
  }

  t_logic::t_messenger_name t_logic::get_name(t_err err) const {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        return impl_->get_name(err);
      err = err::E_XXX;
    }
    return t_messenger_name{};
  }

  t_void t_logic::get_params(t_err err, r_messenger_params params) const {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        impl_->get_params(err, params);
      else
        err = err::E_XXX;
    }
  }

  t_void t_logic::get_stats(t_err err, r_stats stats, t_bool reset) {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        impl_->get_stats(err, stats, reset);
      else
        err = err::E_XXX;
    }
  }

  t_void t_logic::post_message(t_err err, R_messenger_key key, x_message msg) {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        impl_->post_message(err, key, x_cast(msg));
      else
        err = err::E_XXX;
    }
  }

  t_void t_logic::update_scope(t_err err, t_messenger_scope scope) {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        impl_->update_scope(err, scope);
      else
        err = err::E_XXX;
    }
  }

  t_void t_logic::update_alive_period(t_err err, t_multiple_of_100ms factor) {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        impl_->update_alive_period(err, factor);
      else
        err = err::E_XXX;
    }
  }

  t_void t_logic::start_message_timer(t_err err,
                                      R_messenger_timer_params params) {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        impl_->start_message_timer(err, params);
      else
        err = err::E_XXX;
    }
  }

  t_void t_logic::stop_message_timer(t_err err) {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        impl_->stop_message_timer(err);
      else
        err = err::E_XXX;
    }
  }

  t_void t_logic::query_message_timer(t_err err,
                                      r_messenger_timer_params params) const {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        impl_->query_message_timer(err, params);
      else
        err = err::E_XXX;
    }
  }

  t_void t_logic::add_monitor(t_err err, R_messenger_name name,
                              t_messenger_prio prio, t_messenger_user user) {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        impl_->add_monitor(err, name, prio, user);
      else
        err = err::E_XXX;
    }
  }

  t_void t_logic::remove_monitor(t_err err, R_messenger_name name,
                                 p_messenger_user user) {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        impl_->remove_monitor(err, name, user);
      else
        err = err::E_XXX;
    }
  }

  t_logic::t_messenger_key
      t_logic::is_monitored(t_err err, R_messenger_name name,
                                       p_messenger_user user) const {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        return impl_->is_monitored(err, name, user);
      err = err::E_XXX;
    }
    return t_messenger_key{0};
  }

  t_void t_logic::get_monitored(t_err err, r_messenger_monitor_list list) const {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        impl_->get_monitored(err, list);
      else
        err = err::E_XXX;
    }
  }

  t_void t_logic::add_to_group(t_err err, R_messenger_password password,
                               R_messenger_name group, t_messenger_prio prio,
                               t_messenger_user user) {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        impl_->add_to_group(err, password, group, prio, user);
      else
        err = err::E_XXX;
    }
  }

  t_void t_logic::remove_from_group(t_err err, R_messenger_password password,
                                    R_messenger_name group,
                                    p_messenger_user user) {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        impl_->remove_from_group(err, password, group, user);
      else
        err = err::E_XXX;
    }
  }

  t_bool t_logic::is_in_group(t_err err, R_messenger_name group,
                              p_messenger_user user) const {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        return impl_->is_in_group(err, group, user);
      err = err::E_XXX;
    }
    return false;
  }

  t_void t_logic::get_groups(t_err err,
                             r_messenger_group_list list) const {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        impl_->get_groups(err, list);
      else
        err = err::E_XXX;
    }
  }

  t_void t_logic::create_group(t_err err, R_password password,
                               R_messenger_name group,
                               t_messenger_scope scope) {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        impl_->create_group(err, password, group, scope);
      else
        err = err::E_XXX;
    }
  }

  t_void t_logic::destroy_group(t_err err, R_password password,
                                R_messenger_name group) {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        impl_->destroy_group(err, password, group);
      else
        err = err::E_XXX;
    }
  }

  t_bool t_logic::is_group(t_err err, R_messenger_name group,
                           r_messenger_scope scope,
                           p_messenger_name_list list) {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        return impl_->is_group(err, group, scope, list);
      err = err::E_XXX;
    }
    return false;
  }

  t_void t_logic::add_another_to_group(t_err err,
                                       R_messenger_password password,
                                       R_messenger_name name,
                                       R_messenger_name group,
                                       t_messenger_prio prio,
                                       t_messenger_user user) {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        impl_->add_another_to_group(err, password, name, group, prio, user);
      else
        err = err::E_XXX;
    }
  }

  t_void t_logic::remove_another_from_group(t_err err,
                                            R_messenger_password password,
                                            R_messenger_name name,
                                            R_messenger_name group,
                                            p_messenger_user user) {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        impl_->remove_another_from_group(err, password, name, group, user);
      else
        err = err::E_XXX;
    }
  }

  t_bool t_logic::is_another_in_group(t_err err, R_messenger_name name,
                                      R_messenger_name group,
                                      p_messenger_user user) {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        return impl_->is_another_in_group(err, name, group, user);
      err = err::E_XXX;
    }
    return false;
  }

  t_msec t_logic::get_max_wait() const {
    if (impl_ == VALID && *impl_ == VALID)
      return impl_->get_max_wait();
    return t_msec{0};
  }

  t_void t_logic::set_max_wait(t_err err, t_msec msec) {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        impl_->set_max_wait(err, msec);
      else
        err = err::E_XXX;
    }
  }

  t_void t_logic::register_message_logic(t_err err, R_message_id mid,
                                         R_label label,
                                         p_message_logic logic) {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        impl_->register_message_logic(err, mid, label, logic);
      else
        err = err::E_XXX;
    }
  }

  p_message_logic t_logic::unregister_message_logic(t_err err, R_label label) {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        return impl_->unregister_message_logic(err, label);
      err = err::E_XXX;
    }
    return nullptr;
  }

  p_message_logic t_logic::is_message_logic_registered(t_err err,
                                                       R_label label) const {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        return impl_->is_message_logic_registered(err, label);
      err = err::E_XXX;
    }
    return nullptr;
  }

  t_void t_logic::add_event_monitor(t_err err, t_fd fd, R_label label,
                                    t_messenger_user user) {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        impl_->add_event_monitor(err, fd, label, user);
      else
        err = err::E_XXX;
    }
  }

  t_void t_logic::remove_event_monitor(t_err err, t_fd fd, p_label label,
                                       p_messenger_user user) {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        impl_->remove_event_monitor(err, fd, label, user);
      else
        err = err::E_XXX;
    }
  }

  t_bool t_logic::is_event_monitored(t_err err, t_fd fd, p_label label,
                                     p_messenger_user user) {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        return impl_->is_event_monitored(err, fd, label, user);
      err = err::E_XXX;
    }
    return false;
  }

  t_void t_logic::update(t_thread_err err, os::r_pthread_attr attr) noexcept {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        impl_->update(err, attr);
      else
        err = err::E_XXX;
    }
  }

  t_void t_logic::prepare(t_thread_err err) noexcept {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        impl_->prepare(err);
      else
        err = err::E_XXX;
    }
  }

  t_void t_logic::run() noexcept {
    impl_->run();
  }

///////////////////////////////////////////////////////////////////////////////

  t_void send_killmsg(t_key key) {
    if (get(key)) {
      t_logic::t_err err;
      messaging::post_message(err, key, t_kill_message{});
      if (err) {
        // XXX-now
        err.clear();
      }
    }
  }

  t_sandbox::t_sandbox(t_err err, R_name name, t_ptr_ ptr) {
    ERR_GUARD(err) {
      if (ptr == VALID && *ptr == VALID) {
        key_ = ptr->get_key();
        t_thread_ thread{err, name.get_cstr(), {ptr.release()}};
        if (err)
          reset(key_);
      } else
        err = err::E_XXX;
    }
  }

  t_sandbox::t_sandbox(x_sandbox sandbox) {
    if (sandbox == VALID)
      reset(key_ ,reset(sandbox.key_));
  }

  t_sandbox::~t_sandbox() {
    send_killmsg(key_);
  }

  r_sandbox t_sandbox::operator=(x_sandbox sandbox) {
    send_killmsg(t_key{reset(key_, reset(sandbox.key_))});
  }

  t_sandbox::operator t_validity() const {
    return get(key_) ? VALID : INVALID;
  }

///////////////////////////////////////////////////////////////////////////////

  t_main::t_main(t_err err, R_name, t_ptr_ ptr) { // XXX - strange
    ERR_GUARD(err) {
      if (ptr == VALID && *ptr == VALID) {
        key_ = ptr->get_key();
        ptr->prepare(err);
        if (!err)
          ptr->run();
        else
          reset(key_);
      } else
        err = err::E_XXX;
    }
  }

  t_main::operator t_validity() const {
    return get(key_) ? VALID : INVALID;
  }

///////////////////////////////////////////////////////////////////////////////

  t_void start(err::t_err err) {
    ERR_GUARD(err) {
      tracing::start(err);
      messaging::start(err, P_cstr{"messaging"});
    }
  }

///////////////////////////////////////////////////////////////////////////////
}
}

