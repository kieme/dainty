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

#include "dainty_base.h"
#include "dainty_base_terminal.h"
#include "dainty_sandbox_impl.h"

///////////////////////////////////////////////////////////////////////////////

namespace dainty
{
namespace sandbox
{
///////////////////////////////////////////////////////////////////////////////

  using namespace base::terminal;

  using base::specific::t_ix;
  using base::specific::t_percentage;

  using mt::event_dispatcher::DONT_QUIT;
  using mt::event_dispatcher::QUIT;
  using mt::event_dispatcher::RD_EVENT;
  using mt::event_dispatcher::QUIT_EVENT_LOOP;
  using mt::event_dispatcher::CONTINUE;

///////////////////////////////////////////////////////////////////////////////

  t_impl_::t_impl_(t_err err, R_thread_name name, R_thread_params params,
                   t_n max_logics) noexcept
      : name_      {name},
        params_    {params},
        tmrs_      {err, {t_n{100}, t_percentage{20}, "timerfd_service"}},
        dispatcher_{err, {t_n{100}, "epoll_service"}},
        logics_    {err, max_logics}  {
    ERR_GUARD(err) {
    }
  }

  t_impl_::~t_impl_() {
    t_out{"t_impl_ dies graciously"};
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_impl_::register_logic(t_err err, p_logic logic) noexcept {
    ERR_GUARD(err) {
      t_out{FMT, "register logic name - %s",
            get(logic->get_logic_name().get_cstr())};
      t_ix pos = to_ix(logics_.get_size());
      auto entry = logics_.push_back(err);
      if (entry) {
        entry->logic        = logic;
        entry->logic->ix_   = pos;
        entry->logic->impl_ = this;
      }
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_impl_::enable_spin(t_err err, t_ix ix, t_spin_cnt cnt) noexcept {
    ERR_GUARD(err) {
      t_spin_cnt_ _cnt = get(cnt);
      if (_cnt) {
        auto logic_entry = logics_.get(ix);
        logic_entry->spin_cnt_max = _cnt;
        logic_entry->spin_cnt     = 0;

        if (!spin_cnt_ || get(logics_.get_size()) == 1)
          spin_cnt_ = _cnt;
        else
          spin_cnt_ = 1;
      } else
        disable_spin(ix);
    }
  }

  t_void t_impl_::disable_spin(t_ix ix) noexcept {
    auto logic_entry = logics_.get(ix);
    logic_entry->spin_cnt_max = 0;
    logic_entry->spin_cnt     = 0;

    t_ix_ cnt = 0, end = get(logics_.get_size());
    for (; cnt < end; ++cnt)
      if (logics_.get(t_ix{cnt})->spin_cnt_max)
        break;

    if (cnt == end)
      spin_cnt_ = 0;
  }

  t_spin_cnt t_impl_::get_spin_cnt(t_ix ix) const noexcept {
    auto logic_entry = logics_.get(ix);
    return t_spin_cnt{logic_entry->spin_cnt_max};
  }

  t_msec t_impl_::get_spin_period(t_ix ix) const noexcept {
    return t_msec{spin_period_};
  }

///////////////////////////////////////////////////////////////////////////////

  t_timer_id t_impl_::start_timer(t_err err, t_ix ix, R_timer_name name,
                                  R_timer_params params) noexcept {
    ERR_GUARD(err) {
      auto logic_entry = logics_.get(ix);
      if (!logic_entry->timer_ids.is_full()) {
        auto result = timers_.insert(err);
        if (result) {
          auto id = mk_timer_id_(result.id, generate_timer_unique_id_());
          auto tmr_id = tmrs_.start_timer(err, {params.periodic,
                                                params.timeout,
                                                params.early,
                                                params.prio,
                                                get(id)});
          if (!err) {
            auto timer_entry   = result.ptr;
            timer_entry->logic_ix = ix;
            timer_entry->name     = name;
            timer_entry->params   = params;
            timer_entry->tmr_id   = tmr_id;
            timer_entry->id       = id;
            logic_entry->timer_ids.push_back(id);
            return id;
          }
          timers_.erase(result.id);
        }
      } else
        err = err::E_XXX;
    }
    return t_timer_id{0};
  }

  t_timer_id t_impl_::start_timer(t_err err, t_ix ix, R_timer_name name,
                                  R_timer_params params,
                                  x_timer_notify_ptr ptr) noexcept {
    ERR_GUARD(err) {
      auto logic_entry = logics_.get(ix);
      if (!logic_entry->timer_ids.is_full()) {
        auto result = timers_.insert(err);
        if (result) {
          auto id = mk_timer_id_(result.id, generate_timer_unique_id_());
          auto tmr_id = tmrs_.start_timer(err, {params.periodic,
                                                params.timeout,
                                                params.early,
                                                params.prio,
                                                get(id)});
          if (!err) {
            auto timer_entry   = result.ptr;
            timer_entry->logic_ix   = ix;
            timer_entry->name       = name;
            timer_entry->params     = params;
            timer_entry->tmr_id     = tmr_id;
            timer_entry->id         = id;
            timer_entry->notify_ptr = base::x_cast(ptr);
            logic_entry->timer_ids.push_back(id);
            return id;
          }
          timers_.erase(result.id);
        }
      } else
        err = err::E_XXX;
    }
    return t_timer_id{0};
  }

  t_void t_impl_::restart_timer(t_err err, t_ix ix, t_timer_id id) noexcept {
    ERR_GUARD(err) {
      auto timer_entry = get_entry_(id, ix);
      if (timer_entry)
        tmrs_.restart_timer(err, timer_entry->tmr_id);
      else
        err = err::E_XXX;
    }
  }

  t_void t_impl_::restart_timer(t_err err, t_ix ix, t_timer_id id,
                                R_timer_params params) noexcept {
    ERR_GUARD(err) {
      auto timer_entry = get_entry_(id, ix);
      if (timer_entry) {
        tmrs_.restart_timer(err, timer_entry->tmr_id, params);
        if (!err)
          timer_entry->params = params;
      } else
        err = err::E_XXX;
    }
  }

  t_bool t_impl_::stop_timer(t_ix ix, t_timer_id id) noexcept {
    auto timer_entry = get_entry_(id, ix);
    if (timer_entry)
      return tmrs_.stop_timer(timer_entry->tmr_id);
    return false;
  }

  t_timer_notify_ptr t_impl_::clear_timer(t_ix ix, t_timer_id id) noexcept {
    t_timer_notify_ptr tmp;
    auto timer_entry = get_entry_(id, ix);
    if (timer_entry) {
      tmrs_.clear_timer(timer_entry->tmr_id);

      if (timer_entry->notify_ptr == VALID)
        tmp = base::x_cast(timer_entry->notify_ptr);

      auto logic_entry = logics_.get(ix);
      logic_entry->timer_ids.erase(
        logic_entry->timer_ids.find_if(
          [&id](auto& _id) -> t_bool { return id == _id; }));

      timers_.erase(get_timers_id_(id));
    }
    return tmp;
  }

  P_timer_params t_impl_::get_timer(t_ix ix, t_timer_id id) const noexcept {
    auto timer_entry = get_entry_(id, ix);
    if (timer_entry)
      return &timer_entry->params;
    return nullptr;
  }

///////////////////////////////////////////////////////////////////////////////

  t_fdevent_id t_impl_::add_fdevent(t_err err, t_ix ix, R_fdevent_name name,
                                    R_fdevent_params params) noexcept {
    ERR_GUARD(err) {
      auto logic_entry = logics_.get(ix);
      if (!logic_entry->event_ids.is_full()) {
        auto result = fdevents_.insert(err);
        if (result) {
          auto id = mk_fdevent_id_(result.id, generate_fdevent_unique_id_());
          auto ev_id = dispatcher_.add_event(err, {params.fd,
                                                   params.type,
                                                   params.prio,
                                                   get(id)});
          if (!err) {
            auto fd_entry      = result.ptr;
            fd_entry->logic_ix = ix;
            fd_entry->name     = name;
            fd_entry->params   = params;
            fd_entry->ev_id    = ev_id;
            fd_entry->id       = id;
            logic_entry->event_ids.push_back(id);
            return id;
          }
          fdevents_.erase(result.id);
        }
      } else
        err = err::E_XXX;
    }
    return t_fdevent_id{0};
  }

  t_fdevent_id t_impl_::add_fdevent(t_err err, t_ix ix, R_fdevent_name name,
                                    R_fdevent_params params,
                                    t_fdevent_notify_ptr notify_ptr) noexcept {
    ERR_GUARD(err) {
      auto logic_entry = logics_.get(ix);
      if (!logic_entry->event_ids.is_full()) {
        auto result = fdevents_.insert(err);
        if (result) {
          auto id = mk_fdevent_id_(result.id, generate_fdevent_unique_id_());
          auto ev_id = dispatcher_.add_event(err, {params.fd,
                                                   params.type,
                                                   params.prio,
                                                   get(id)});
          if (!err) {
            auto fd_entry        = result.ptr;
            fd_entry->logic_ix   = ix;
            fd_entry->name       = name;
            fd_entry->params     = params;
            fd_entry->ev_id      = ev_id;
            fd_entry->notify_ptr = base::x_cast(notify_ptr);
            fd_entry->id         = id;
            logic_entry->event_ids.push_back(id);
            return id;
          }
          fdevents_.erase(result.id);
        }
      } else
        err = err::E_XXX;
    }
    return t_fdevent_id{0};
  }

  t_fdevent_notify_ptr t_impl_::del_fdevent(t_ix ix,
                                            t_fdevent_id id) noexcept {
    t_fdevent_notify_ptr tmp;
    auto fd_entry = get_entry_(id, ix);
    if (fd_entry) {
      dispatcher_.del_event(fd_entry->ev_id);

      if (fd_entry->notify_ptr == VALID)
        tmp = base::x_cast(fd_entry->notify_ptr);

      auto logic_entry = logics_.get(ix);
      logic_entry->event_ids.erase(
        logic_entry->event_ids.find_if(
          [&id](auto& _id) -> t_bool { return id == _id; }));

      fdevents_.erase(get_fdevents_id_(id));
    }
    return tmp;
  }

  P_fdevent_params t_impl_::get_fdevent(t_ix ix,
                                        t_fdevent_id id) const noexcept {
    auto fd_entry = get_entry_(id, ix);
    if (fd_entry)
      return &fd_entry->params;
    return nullptr;
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_impl_::start_extensions_(t_err err, p_logic logic) noexcept {
    ERR_GUARD(err) {
      t_ix_ end = get(logic->exts_.get_size());
      for (t_ix_ ix = 0; !err && ix < end; ++ix) {
        logic->exts_.get(t_ix{ix})->notify_start(err);
        if (err && ix) {
          for (--ix; ix; --ix)
            logic->exts_.get(t_ix{ix})->notify_cleanup();
          logic->exts_.get(t_ix{ix})->notify_cleanup();
        }
      }
    }
  }

  t_void t_impl_::cleanup_extensions_(p_logic logic) noexcept {
    t_ix_ ix = get(logic->exts_.get_size());
    if (ix) {
      for (--ix; ix; --ix)
        logic->exts_.get(t_ix{ix})->notify_cleanup();
      logic->exts_.get(t_ix{ix})->notify_cleanup();
    }
  }

  t_void t_impl_::start_(t_err err) noexcept {
    ERR_GUARD(err) {
      t_ix_ end = get(logics_.get_size());
      for (t_ix_ ix = 0; !err && ix < end; ++ix) {
        auto logic = logics_.get(t_ix{ix})->logic;
        start_extensions_(err, logic);
        logic->notify_start(err);
        last_ = os::clock::monotonic_now(err);
        if (err && ix) {
          for (--ix; ix; --ix) {
            auto logic = logics_.get(t_ix{ix})->logic;
            logic->notify_cleanup();
            cleanup_extensions_(logic);
          }
          auto logic = logics_.get(t_ix{ix})->logic;
          logic->notify_cleanup();
          cleanup_extensions_(logic);
        }
      }
    }
  }

  t_void t_impl_::cleanup_(t_err err) noexcept {
    ERR_GUARD(err) {
      t_ix_ ix = get(logics_.get_size());
      if (ix) {
        for (--ix; ix; --ix) {
          auto logic = logics_.get(t_ix{ix})->logic;
          logic->notify_cleanup();
          cleanup_extensions_(logic);
        }
        auto logic = logics_.get(t_ix{ix})->logic;
        logic->notify_cleanup();
        cleanup_extensions_(logic);
      }
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_impl_::p_fdevent_entry_
      t_impl_::get_entry_(t_fdevent_id id, t_ix ix) noexcept {
    auto fd_entry = fdevents_.get(get_fdevents_id_(id));
    return (fd_entry &&
            id == fd_entry->id &&
            ix == fd_entry->logic_ix) ? fd_entry : nullptr;
  }

  t_impl_::P_fdevent_entry_
      t_impl_::get_entry_(t_fdevent_id id, t_ix ix) const noexcept {
    auto fd_entry = fdevents_.get(get_fdevents_id_(id));
    return (fd_entry &&
            id == fd_entry->id &&
            ix == fd_entry->logic_ix) ? fd_entry : nullptr;
  }

  t_freelist_id_value_ t_impl_::generate_fdevent_unique_id_() noexcept {
    // XXX
    return 0;
  }

  t_fdevent_id t_impl_::mk_fdevent_id_(t_fdevents_id_ id,
                                       t_freelist_id_value_) const noexcept {
    // XXX
    return t_fdevent_id(get(id));
  }

   t_impl_::t_fdevents_id_
       t_impl_::get_fdevents_id_(t_fdevent_id id) const noexcept {
    // XXX
    return t_fdevents_id_(get(id));
  }

  t_freelist_id_value_ t_impl_::get_unique_id_(t_fdevent_id) const noexcept {
    // XXX
    return 0;
  }

///////////////////////////////////////////////////////////////////////////////

  t_impl_::p_timer_entry_
      t_impl_::get_entry_(t_timer_id id, t_ix ix) noexcept {
    auto timer_entry = timers_.get(get_timers_id_(id));
    return (timer_entry &&
            id == timer_entry->id &&
            ix == timer_entry->logic_ix) ? timer_entry : nullptr;
  }

  t_impl_::P_timer_entry_
      t_impl_::get_entry_(t_timer_id id, t_ix ix) const noexcept {
    auto timer_entry = timers_.get(get_timers_id_(id));
    return (timer_entry &&
            id == timer_entry->id &&
            ix == timer_entry->logic_ix) ? timer_entry : nullptr;
  }

  t_freelist_id_value_ t_impl_::generate_timer_unique_id_() noexcept {
    // XXX
    return 0;
  }

  t_timer_id t_impl_::mk_timer_id_(t_timers_id_ id,
                                   t_freelist_id_value_) const noexcept {
    // XXX
    return t_timer_id(get(id));
  }

   t_impl_::t_timers_id_
       t_impl_::get_timers_id_(t_timer_id id) const noexcept {
    // XXX
    return t_timers_id_(get(id));
  }

  t_freelist_id_value_ t_impl_::get_unique_id_(t_timer_id) const noexcept {
    // XXX
    return 0;
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_impl_::update(base1_::t_err err,
                         base1_::r_pthread_attr attr) noexcept {
    ERR_GUARD(err) {
      // XXX - can use params_ to set attr - maybe part of Params
    }
  }

  t_void t_impl_::prepare(base1_::t_err err) noexcept {
    ERR_GUARD(err) {
      tmrs_ev_id_  = dispatcher_.add_event(err, {tmrs_.get_fd(), RD_EVENT});

      // XXX - can use params_ for further preparation
    }
  }

  t_void t_impl_::run() noexcept {
    event_loop_();
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_impl_::notify_dispatcher_reorder(r_event_infos infos) noexcept {
    t_out{"t_impl_::notify_reorder"};
    t_time now     = os::clock::monotonic_now();
    t_msec expired = (now - last_).to<t_msec>();
    last_          = now;
    logics_.each([this, expired](auto& entry) {
      entry.logic->exts_.each([expired](auto entry) {
          entry->notify_wakeup(expired);
        });
      entry.logic->notify_wakeup(expired);
    });
  }

  t_void t_impl_::notify_dispatcher_removed(r_event_info info) noexcept {
    t_out{"t_impl_::notify_removed"};
    // if a fd close it will be removed
    // should we have a notify?
    //XXX
  }

  t_impl_::t_quit t_impl_::notify_dispatcher_timeout(t_msec msec) noexcept {
    t_time now     = os::clock::monotonic_now();
    t_msec expired = (now - last_).to<t_msec>();
    last_          = now;

    logics_.each([this, expired](auto& entry) {
        if (entry.spin_cnt_max) {
          entry.spin_cnt += spin_cnt_;
          if (entry.spin_cnt == entry.spin_cnt_max) {
            entry.spin_cnt = 0;
            entry.logic->exts_.each([expired](auto entry) {
                entry->notify_wakeup(expired);
              });
            entry.logic->notify_wakeup(expired);
          }
        }
      });

    return DONT_QUIT;
  }

  t_impl_::t_quit t_impl_::notify_dispatcher_error(t_errn errn) noexcept {
    t_out{"t_impl_::notify_error - prepare to die"};
    return QUIT;
  }

  t_impl_::t_quit t_impl_::notify_dispatcher_processed(r_msec msec) noexcept {
    logics_.each([this](auto& entry) {
      entry.logic->exts_.each([](auto entry) {
          entry->notify_complete();
        });
      entry.logic->notify_complete(); // XXX QUIT here if I want
    });
    msec = t_msec{spin_cnt_ * spin_period_};
    return DONT_QUIT;
  }

  t_impl_::t_action
      t_impl_::notify_dispatcher_event(t_event_id ev_id,
                                       r_event_params params) noexcept {
    if (ev_id == tmrs_ev_id_) {
      t_out{"internal timer expired - process timeouts"};
      tmrs_.process(*this);
    } else {
      t_fdevent_id id(params.user.id);
      auto entry_id = get_fdevents_id_(id);
      auto fd_entry = fdevents_.get(entry_id);

      if (fd_entry && fd_entry->id == id) {
         if (fd_entry->notify_ptr == VALID)
           fd_entry->notify_ptr->notify_fdevent(id, fd_entry->params);
         else {
           auto logic_entry = logics_.get(fd_entry->logic_ix);
           logic_entry->logic->notify_fdevent(id, fd_entry->params);
        }
      } else {
        t_out{"this should never happen - recv unknown fd event"};
      }
    }
    return t_action{CONTINUE};
  }

  t_void t_impl_::notify_timers_overrun(t_timer_id,
                                        t_msec) noexcept {
    t_out{"t_impl_::notify_timers_overrun"};
  }

  t_void t_impl_::notify_timers_reorder(r_timer_infos) noexcept {
    t_out{"t_impl_::notify_timers_reorder"};
    //XXX
  }

  t_void t_impl_::notify_timers_timeout(t_timer_id id,
                                        R_timer_params params) noexcept {
    auto entry_id    = get_timers_id_(id);
    auto timer_entry = timers_.get(entry_id);

    if (timer_entry && timer_entry->id == id) {
       if (timer_entry->notify_ptr == VALID)
         timer_entry->notify_ptr->notify_timeout(id, timer_entry->params);
       else {
         auto logic_entry = logics_.get(timer_entry->logic_ix);
         logic_entry->logic->notify_timeout(id, timer_entry->params);
      }
    } else {
      t_out{"this should never happen - recv unknown timer event"};
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_impl_::event_loop_() noexcept {
    t_err err;

    start_(err.tag(1));
    dispatcher_.event_loop(err.tag(2), *this,
                           t_msec{spin_cnt_ * spin_period_});
    cleanup_(err.tag(3));

    if (err) {
      switch (err.tag()) {
        case 1: { t_out{"failed in tag(1)"}; } break;
        case 2: { t_out{"failed in tag(2)"}; } break;
        case 3: { t_out{"failed in tag(3)"}; } break;
      }
      err.print();
      err.clear();
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_single_impl_::t_single_impl_(t_err err, R_name name, R_params params,
                                 x_ptr ptr) noexcept
    : t_impl_{err, name, params, t_n{1}},
      ptr_{base::x_cast(ptr)} {
    ERR_GUARD(err) {
      register_logic(err, ptr_.get());
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_shared_impl_::t_shared_impl_(t_err err, R_name name, R_params params,
                                 x_ptrlist ptrlist) noexcept
    : t_impl_{err, name, params, ptrlist.get_size()},
      ptrlist_{base::x_cast(ptrlist)} {
    ERR_GUARD(err) {
      t_ix_ end = get(ptrlist_.get_size());
      for (t_ix_ ix = 0; !err && ix < end; ++ix)
        register_logic(err, ptrlist_.get(t_ix{ix})->get());
    }
  }

///////////////////////////////////////////////////////////////////////////////

}
}
