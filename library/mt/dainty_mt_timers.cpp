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

#include "dainty_named_utility.h"
#include "dainty_container_freelist.h"
#include "dainty_container_ptrlist.h"
#include "dainty_os_clock.h"
#include "dainty_os_fdbased.h"
#include "dainty_mt_timers.h"

namespace dainty
{
namespace mt
{
namespace timers
{
  using named::t_ix_;
  using named::t_msec;
  using named::t_cstr_cptr;
  using os::clock::monotonic_now;
  using container::freelist::t_freelist;
  using container::ptrlist::t_ptrlist;

  using t_tmr_          = os::clock::t_timer;
  using r_err           = t_prefix<t_err>::r_;
  using p_logic         = t_timers::p_logic;
  using t_timerfd_      = os::fdbased::t_timerfd;
  using t_timerfd_flags = t_timerfd_::t_flags;
  using t_timerfd_spec  = t_timerfd_::t_timerspec;
  using t_time          = os::clock::t_time;
  using R_time          = os::clock::R_time;

///////////////////////////////////////////////////////////////////////////////

  class t_impl_ {
  public:
    t_impl_(R_params params) noexcept
      : params_       {params},
        timers_       {params.max},
        active_timers_{params.max},
        timerfd_      {t_timerfd_flags{0}} {
    }

    t_impl_(r_err err, R_params params) noexcept
      : params_       {params},
        timers_       {err, params.max},
        active_timers_{err, params.max},
        timerfd_      {err, t_timerfd_flags{0}} {
    }

    operator t_validity() const noexcept {
      return timerfd_       == VALID &&
             timers_        == VALID &&
             active_timers_ == VALID ? VALID : INVALID;
    }

    t_fd get_fd() const noexcept {
      return timerfd_.get_fd();
    }

    t_timer_id start_timer (R_timer_params params,
                            p_timer_logic logic) noexcept {
      auto result = timers_.insert();
      if (result) {
        auto now   = monotonic_now();
        auto entry = result.ptr;

        entry->info.id     = t_timer_id(get(result.id));
        entry->info.params = params;
        entry->info.logic  = logic;
        entry->periodic    = params.periodic;
        entry->timeout     = params.timeout;
        entry->early       = params.early;
        entry->start       = now;
        entry->running     = entry->timeout;

        t_errn errn{0};
        if (recalc_current_(now, entry->timeout))
          errn = set_timer_();

        if (get(errn) == 0) {
          active_timers_.push_back(entry);
          return t_timer_id(get(result.id));
        }
        timers_.erase(result.id);
      }
      return t_timer_id{BAD_TIMER_ID};
    }

    t_timer_id start_timer(r_err err, R_timer_params params,
                           p_timer_logic logic) noexcept {
      auto result = timers_.insert(err);
      if (result) {
        auto now   = monotonic_now();
        auto entry = result.ptr;

        entry->info.id     = t_timer_id(get(result.id));
        entry->info.params = params;
        entry->info.logic  = logic;
        entry->periodic    = params.periodic;
        entry->timeout     = params.timeout;
        entry->early       = params.early;
        entry->start       = now;
        entry->running     = entry->timeout;

        if (recalc_current_(now, entry->timeout))
          set_timer_(err);

        if (!err) {
          active_timers_.push_back(entry);
          return t_timer_id(get(result.id));
        }
        timers_.erase(result.id);
      }
      return t_timer_id{BAD_TIMER_ID};
    }

    t_errn restart_timer(t_timer_id id, R_timer_params params) noexcept {
      auto entry = timers_.get(t_timers_id_(get(id)));
      if (entry) {
        auto now     = monotonic_now();
        auto running = entry->running;

        entry->info.params = params;
        entry->periodic    = params.periodic;
        entry->timeout     = params.timeout;
        entry->early       = params.early;
        entry->start       = now;
        entry->running     = entry->timeout;

        t_errn errn{0};
        if (recalc_current_(now, entry->timeout, running))
          errn = set_timer_();

        if (get(errn) == 0 && !running)
          active_timers_.push_back(entry);

        return errn;
      }
      return t_errn{-1};
    }

    t_void restart_timer(r_err err, t_timer_id id,
                         R_timer_params params) noexcept {
      auto entry = timers_.get(t_timers_id_(get(id)));
      if (entry) {
        auto now     = monotonic_now();
        auto running = entry->running;

        entry->info.params = params;
        entry->periodic    = params.periodic;
        entry->timeout     = params.timeout;
        entry->early       = params.early;
        entry->start       = now;
        entry->running     = entry->timeout;

        if (recalc_current_(now, entry->timeout, running))
          set_timer_(err);

        if (!err && !running)
          active_timers_.push_back(err, entry);
      }
    }

    t_errn restart_timer(t_timer_id id) noexcept {
      auto entry = timers_.get(t_timers_id_(get(id)));
      if (entry) {
        auto now     = monotonic_now();
        auto running = entry->running;

        entry->running = entry->timeout;

        t_errn errn{-1};
        if (recalc_current_(now, entry->timeout, running))
          errn = set_timer_();

        if (get(errn) == 0 && !running)
          active_timers_.push_back(entry);

        return errn;
      }
      return t_errn{-1};
    }

    t_void restart_timer(r_err err, t_timer_id id) noexcept {
      auto entry = timers_.get(t_timers_id_(get(id)));
      if (entry) {
        auto now     = monotonic_now();
        auto running = entry->running;

        entry->running = entry->timeout;

        if (recalc_current_(now, entry->timeout, running))
          set_timer_(err);

        if (!err && !running) // XXX no proper cleanup
          active_timers_.push_back(err, entry);
      }
    }

    t_bool stop_timer(t_timer_id id) noexcept {
      auto entry = timers_.get(t_timers_id_(get(id)));
      if (entry && entry->running) {
        t_ix_ end = get(active_timers_.get_size());
        for (t_ix_ ix = 0; ix < end; ++ix) {
          if (entry == active_timers_.get(t_ix{ix})) {
            if (current_ == entry->running) {
              active_timers_.erase(t_ix{ix});
              if (recalc_current_all_(monotonic_now()))
                set_timer_(); //XXX
            }
            entry->running = t_msec{0};
            return true;
          }
        }
      }
      return false;
    }

    p_timer_logic clear_timer(t_timer_id id) noexcept {
      t_timers_id_ _id(get(id));
      auto entry = timers_.get(_id);
      if (entry && entry->running) {
        t_ix_ end = get(active_timers_.get_size());
        for (t_ix_ ix = 0; ix < end; ++ix) {
          if (entry == active_timers_.get(t_ix{ix})) {
            if (current_ == entry->running) {
              active_timers_.erase(t_ix{ix});
              if (recalc_current_all_(monotonic_now()))
                set_timer_(); //XXX
            }
            break;
          }
        }
        auto tmp = entry->info.logic;
        timers_.erase(_id);
        return tmp;

      }
      return nullptr;
    }

    t_void clear_timers() noexcept {
      if (current_) {
        current_ = t_msec{0};
        last_    = t_msec{0};
        set_timer_();
        active_timers_.clear();
        timers_.clear();
      }
    }

    t_bool is_timer_running(t_timer_id id) const noexcept {
      auto entry = timers_.get(t_timers_id_(get(id)));
      if (entry) {
        t_ix_ end = get(active_timers_.get_size());
        for (t_ix_ ix = 0; ix < end; ++ix)
          if (entry == active_timers_.get(t_ix{ix}))
            return true;
      }
      return false;
    }

    P_timer_info get_info(t_timer_id id) const noexcept {
      auto entry = timers_.get(t_timers_id_(get(id)));
      if (entry)
        return &entry->info;
      return nullptr;
    }

    t_void get_timer_ids(r_timer_ids ids) const noexcept {
      timers_.each([&ids](auto, auto& entry) mutable {
                     ids.push_back(entry.info.id); });
    }

    t_errn process(p_logic logic) noexcept {
      //XXX
      return t_errn{-1};
    }

    t_void process(r_err err, p_logic logic) noexcept {
      t_timerfd_::t_data data;
      timerfd_.read(err, data);
      if (!err) {
        if (current_ && !active_timers_.is_empty()) {
          t_time now = monotonic_now();
          t_timer_infos expired_timers{err, params_.max};
          if (!err) {
            if (recalc_current_all_(now, expired_timers))
              set_timer_(err);

            logic->notify_timers_reorder(expired_timers);

            t_ix_ end = get(expired_timers.get_size());
            for (t_ix_ ix = 0; ix < end; ++ix) {
              auto entry = timers_.get(
                t_timers_id_(get(expired_timers.get(t_ix{ix})->id)));

              if (get(params_.overrun)) {
                entry->end = monotonic_now();
                t_msec expired{get(entry->start.to<t_msec>()) -
                               get(entry->end.to<t_msec>())};
                t_msec timeout{entry->info.params.timeout};
                t_msec overrun{get(expired) - get(timeout)};
                if ((get(overrun)/get(timeout))*100 >= get(params_.overrun))
                  logic->notify_timers_overrun(entry->info.id, overrun);
              }

              if (entry->info.logic)
                entry->info.logic->notify_timers_timeout(entry->info.id,
                                                         entry->info.params);
              else
                logic->notify_timers_timeout(entry->info.id,
                                             entry->info.params);

              if (entry->periodic) {
                entry->start = now;
                entry->end   = t_msec{0};
              } else {
                entry->start = t_msec{0};
                entry->end   = t_msec{0};
              }
            }

            logic->notify_timers_processed();
          }
        } else {
          current_ = t_msec{0};
          last_    = t_msec{0};
          err = err::E_XXX;
        }
      }
    }

  private:
    struct t_timer_entry_ {
      t_timer_info info;
      t_bool  periodic;
      t_time  timeout;
      t_time  early;
      t_time  running; // its running if not zero
      t_time  start;
      t_time  end;
    };
    using t_timers_       = t_freelist<t_timer_entry_>;
    using t_timers_id_    = t_timers_::t_id;
    using t_timer_ptrlist = t_ptrlist <t_timer_entry_>;

    t_void set_timer_(r_err err) noexcept {
      ERR_GUARD(err) {
        t_time tmp;
        t_timerfd_spec spec;
        spec.it_interval = os::clock::to_(tmp);
        spec.it_value    = os::clock::to_(current_);
        timerfd_.set_time(err, t_timerfd_flags{0}, spec);
      }
    }

    t_errn set_timer_() noexcept {
      t_time tmp;
      t_timerfd_spec spec;
      spec.it_interval = os::clock::to_(tmp);
      spec.it_value    = os::clock::to_(current_);
      return timerfd_.set_time(t_timerfd_flags{0}, spec);
    }

    t_bool recalc_current_all_(R_time now) noexcept {
      t_time expired = now - last_;
      t_time lowest;

      t_ix_ end = get(active_timers_.get_size());
      for (t_ix_ ix = 0; ix < end; ++ix) {
        auto entry = active_timers_.get(t_ix{ix});
        entry->running -= expired;
        if (entry->running < lowest)
          lowest = entry->running;
      }

      if (lowest) {
        last_    = now;
        current_ = lowest;
        return true;
      } else if (current_) {
        last_    = t_msec{0};
        current_ = t_msec{0};
        return true;
      }
      return false;
    }

    t_bool recalc_current_all_(R_time now,
                               r_timer_infos expired_timers) noexcept {
      t_time expired = now - last_;
      t_time lowest;

      t_ix_ end = get(active_timers_.get_size());
      for (t_ix_ ix = 0; ix < end; /*none*/) {
        auto entry = active_timers_.get(t_ix{ix});
        if (expired > entry->running)
          entry->running = t_msec{0};
        else
          entry->running -= expired;

        if (entry->running <= entry->early) {
          entry->end = now;
          expired_timers.push_back(&entry->info);
          if (entry->periodic)
            entry->running = entry->timeout;
          else {
            entry->running = t_msec{0};
            active_timers_.erase(t_ix{ix});
            end = get(active_timers_.get_size());
            continue;
          }
        }

        if (entry->running < lowest)
          lowest = entry->running;

        ++ix;
      }

      if (lowest) {
        last_    = now;
        current_ = lowest;
        return true;
      }

      last_    = t_msec{0};
      current_ = t_msec{0};
      return false;
    }

    t_bool recalc_current_(R_time now, R_time timeout) noexcept {
      t_bool updated = false;
      if (current_) {
        auto expired = now - last_;

        t_time remain;
        if (current_ > expired)
          remain  = current_ - expired;
        else {
          // XXX not sure - inform logic
        }

        if (timeout < remain) {
          current_ = timeout;
          updated  = true;
          last_    = now;

          t_ix_ end = get(active_timers_.get_size());
          for (t_ix_ ix = 0; ix < end; ++ix) {
            auto entry = active_timers_.get(t_ix{ix});
          if (expired > entry->running)
            entry->running = t_msec{0};
          else
            entry->running -= expired;
          }
        }
      } else {
        last_    = now;
        current_ = timeout;
        updated  = true;
      }
      return updated;
    }

    t_bool recalc_current_(R_time now, R_time timeout,
                           R_time running) noexcept {
      return current_ != running ?  recalc_current_    (now, timeout) :
                                    recalc_current_all_(now);
    }

    T_params        params_;
    t_timers_       timers_;
    t_timer_ptrlist active_timers_;
    t_timerfd_      timerfd_;
    t_time          current_;
    t_time          last_;
  };

///////////////////////////////////////////////////////////////////////////////

  t_n get_supported_services() noexcept {
    return t_n{1};
  }

  t_service_name get_supported_service(t_ix) noexcept {
    return "timerfd_service";
  }

///////////////////////////////////////////////////////////////////////////////

  t_void
      t_timers::t_logic::notify_timers_reorder(r_timer_infos) noexcept {
    // XXX provide default reorder - thats reorders on prio
  }

  t_void t_timers::t_logic::notify_timers_overrun(t_timer_id,
                                                  t_msec) noexcept {
  }

  t_void t_timers::t_logic::notify_timers_processed() noexcept {
  }

///////////////////////////////////////////////////////////////////////////////

  t_timers::t_timers(R_params params) noexcept {
    if (params.service_name == t_cstr_cptr{"timerfd_service"})
      impl_ = new t_impl_{params};
    else {
      // not yet supported - required for non-linux systems
    }
  }

  t_timers::t_timers(t_err err, R_params params) noexcept {
    ERR_GUARD(err) {
      if (params.service_name == t_cstr_cptr{"timerfd_service"})
        impl_ = new t_impl_{err, params};
      else {
        err = err::E_XXX;
        // not yet supported - required for non-linux systems
      }
    }
  }

  t_timers::t_timers(x_timers timers) noexcept
    : impl_{timers.impl_.release()} {
  }

  t_timers::~t_timers() {
    if (*this == VALID)
      impl_->clear_timers();
  }

  t_timers::operator t_validity() const noexcept {
    return impl_ == VALID && *impl_ == VALID ? VALID : INVALID;
  }

  t_fd t_timers::get_fd() const noexcept {
    if (*this == VALID)
      return impl_->get_fd();
    return BAD_FD;
  }

  t_timer_id t_timers::start_timer(R_timer_params params,
                                   p_timer_logic logic) noexcept {
    if (*this == VALID)
      return impl_->start_timer(params, logic);
    return BAD_TIMER_ID;
  }

  t_timer_id t_timers::start_timer(t_err err, R_timer_params params,
                                   p_timer_logic logic) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        return impl_->start_timer(err, params, logic);
      err = err::E_XXX;
    }
    return BAD_TIMER_ID;
  }

  t_errn t_timers::restart_timer(t_timer_id id,
                                 R_timer_params params) noexcept {
    if (*this == VALID)
      return impl_->restart_timer(id, params);
    return t_errn{-1};
  }

  t_void t_timers::restart_timer(t_err err, t_timer_id id,
                                 R_timer_params params) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        impl_->restart_timer(err, id, params);
      else
        err = err::E_XXX;
    }
  }

  t_errn t_timers::restart_timer(t_timer_id id) noexcept {
    if (*this == VALID)
      return impl_->restart_timer(id);
    return t_errn{-1};
  }

  t_void t_timers::restart_timer(t_err err, t_timer_id id) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        impl_->restart_timer(err, id);
      else
        err = err::E_XXX;
    }
  }

  t_bool t_timers::stop_timer(t_timer_id id) noexcept {
    if (*this == VALID)
      return impl_->stop_timer(id);
    return false;
  }

  p_timer_logic t_timers::clear_timer(t_timer_id id) noexcept {
    if (*this == VALID)
      return impl_->clear_timer(id);
    return nullptr;
  }

  t_void t_timers::clear_timers() noexcept {
    if (*this == VALID)
      impl_->clear_timers();
  }

  t_bool t_timers::is_timer_running(t_timer_id id) const noexcept {
    if (*this == VALID)
      return impl_->is_timer_running(id);
    return false;
  }

  P_timer_info t_timers::get_info(t_timer_id id) const noexcept {
    if (*this == VALID)
      return impl_->get_info(id);
    return nullptr;
  }

  P_timer_info t_timers::get_info(t_err err, t_timer_id id) const noexcept {
    ERR_GUARD(err) {
      if (*this == VALID) {
        auto info = impl_->get_info(id);
        if (!info)
          err = err::E_XXX;
        return info;
      }
      err = err::E_XXX;
    }
    return nullptr;
  }

  t_void t_timers::get_timer_ids(r_timer_ids ids) const noexcept {
    if (*this == VALID)
      impl_->get_timer_ids(ids);
  }

  t_errn t_timers::process(p_logic logic) noexcept {
    if (*this == VALID)
      return impl_->process(logic);
    return t_errn{-1};
  }

  t_void t_timers::process(t_err err, p_logic logic) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        return impl_->process(err, logic);
      err = err::E_XXX;
    }
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}
