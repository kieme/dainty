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

#include "dainty_named.h"
#include "dainty_named_utility.h"
#include "dainty_named_terminal.h"
#include "dainty_sandbox_impl.h"

///////////////////////////////////////////////////////////////////////////////

namespace dainty
{
namespace sandbox
{
  using namespace dainty::named::terminal;
  using t_ix = named::t_ix;
  using named::utility::x_cast;

///////////////////////////////////////////////////////////////////////////////

  t_impl_::t_impl_(t_err err, R_thread_name name, R_thread_params params,
                   t_n max_logics) noexcept
      : name_      {name},
        params_    {params},
        dispatcher_{err, {t_n{100}, "epoll_service"}},
        logics_    {err, max_logics}  {
    ERR_GUARD(err) {
    }
  }

  t_impl_::~t_impl_() {
    t_out{"t_impl_ dies graciously"};
  }

  t_void t_impl_::update(base1::t_err err,
                         base1::r_pthread_attr attr) noexcept {
    ERR_GUARD(err) {
      // XXX - can use params_ to set attr
    }
  }

  t_void t_impl_::prepare(base1::t_err err) noexcept {
    ERR_GUARD(err) {
      // XXX - can use params_ for further preparation
    }
  }

  t_void t_impl_::run() noexcept {
    event_loop_();
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_impl_::register_logic(t_err err, p_logic logic) noexcept {
    ERR_GUARD(err) {
      t_out{FMT, "register logic name - %s",
            get(logic->get_messenger_name().get_cstr())};
      t_ix pos = to_ix(logics_.get_size());
      auto entry = logics_.push_back(err, logic);

      // do everything you need
      // tracing service   - tracer
      // messaging service - messenger

      entry->logic->ix_   = pos;
      entry->logic->impl_ = this;
    }
  }

  t_fd t_impl_::get_dispatcher_fd() const noexcept {
    return dispatcher_.get_fd_impl_();
  }

  t_void t_impl_::enable_spin(t_err err, t_ix ix, t_spin_cnt cnt) noexcept {
    ERR_GUARD(err) {
      t_spin_cnt_ _cnt = get(cnt);
      if (_cnt) {
        auto logic_entry = logics_.get(ix);
        logic_entry->spin_cnt_max = _cnt;
        logic_entry->spin_cnt     = 0;

        if (!spin_cnt_)
          spin_cnt_ = _cnt;
        else
          spin_cnt_ = 1; //XXX - can improve greatly - leave for later
      } else
        disable_spin(ix);
    }
  }

  t_void t_impl_::disable_spin(t_ix ix) noexcept {
    auto logic_entry = logics_.get(ix);
    logic_entry->spin_cnt_max = 0;
    logic_entry->spin_cnt     = 0;

    t_ix cnt{0}, end_ix{to_ix(logics_.get_size())};
    for (; cnt < end_ix; ++set(cnt))
      if (logics_.get(cnt)->spin_cnt_max)
        break;

    if (cnt == end_ix) // XXX - simplistic
      spin_cnt_ = 0;
  }

  t_spin_cnt t_impl_::get_spin_cnt(t_ix ix) const noexcept {
    auto logic_entry = logics_.get(ix);
    return t_spin_cnt{logic_entry->spin_cnt_max};
  }

  t_msec t_impl_::get_spin_period(t_ix ix) const noexcept {
    return t_msec{spin_period_};
  }

  t_timer_id t_impl_::start_timer(t_err err, t_ix ix, R_timer_name name,
                                  R_timer_params params) noexcept {
    ERR_GUARD(err) {
    //XXX
    }
    return t_timer_id{0};
  }

  t_timer_id t_impl_::start_timer(t_err err, t_ix ix, R_timer_name name,
                                  R_timer_params params,
                                  x_timer_notify_ptr ptr) noexcept {
    ERR_GUARD(err) {
    //XXX
    }
    return t_timer_id{0};
  }

  t_void t_impl_::restart_timer(t_err err, t_ix ix, t_timer_id id,
                               R_timer_params params) noexcept {
    ERR_GUARD(err) {
    //XXX
    }
  }

  t_timer_notify_ptr t_impl_::stop_timer(t_ix ix, t_timer_id id) noexcept {
    //XXX
    return {};
  }

  P_timer_info t_impl_::get_timer(t_ix ix, t_timer_id id) const noexcept {
    //XXX
    return nullptr;
  }

  t_fdevent_id t_impl_::add_fdevent(t_err err, t_ix ix, R_fdevent_name name,
                                    R_fdevent_params params) noexcept {
    ERR_GUARD(err) {
    //XXX
    }
    return t_fdevent_id{0};
  }

  t_fdevent_id t_impl_::add_fdevent(t_err err, t_ix ix, R_fdevent_name name,
                                    R_fdevent_params params,
                                    t_fdevent_notify_ptr ptr) noexcept {
    ERR_GUARD(err) {
    //XXX
    }
    return t_fdevent_id{0};
  }

  t_fdevent_notify_ptr t_impl_::del_fdevent(t_ix ix,
                                           t_fdevent_id id) noexcept {
    //XXX
    return t_fdevent_notify_ptr{};
  }

  P_fdevent_info t_impl_::get_fdevent(t_ix ix,
                                      t_fdevent_id id)  const noexcept {
    //XXX
    return nullptr;
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_impl_::start_extensions_(t_err err, p_logic logic) noexcept {
    ERR_GUARD(err) {
      t_ix end = to_ix(logic->exts_.get_size());
      for (t_ix ix{0}; !err && ix < end; set(ix)++) {
        logic->exts_.get(ix)->notify_start(err);
        if (err) {
          if (get(ix)) {
            for (set(ix)--; get(ix); set(ix)--)
              logic->exts_.get(ix)->notify_cleanup();
            logic->exts_.get(ix)->notify_cleanup();
          }
        }
      }
    }
  }

  t_void t_impl_::cleanup_extensions_(p_logic logic) noexcept {
    t_ix ix = to_ix(logic->exts_.get_size());
    if (get(ix)) {
      for (set(ix)--; get(ix); set(ix)--)
        logic->exts_.get(ix)->notify_cleanup();
      logic->exts_.get(ix)->notify_cleanup();
    }
  }

  t_void t_impl_::start_(t_err err) noexcept {
    ERR_GUARD(err) {
      t_ix end = to_ix(logics_.get_size());
      for (t_ix ix{0}; !err && ix < end; set(ix)++) {
        auto logic = logics_.get(ix)->logic;
        start_extensions_(err, logic);
        logic->notify_start(err);
        if (err) {
          if (get(ix)) {
            for (set(ix)--; get(ix); set(ix)--) {
              auto logic = logics_.get(ix)->logic;
              logic->notify_cleanup();
              cleanup_extensions_(logic);
            }
            auto logic = logics_.get(ix)->logic;
            logic->notify_cleanup();
            cleanup_extensions_(logic);
          }
        }
      }
    }
  }

  t_void t_impl_::cleanup_(t_err err) noexcept {
    ERR_GUARD(err) {
      t_ix ix = to_ix(logics_.get_size());
      if (get(ix)) {
        for (set(ix)--; get(ix); set(ix)--) {
          auto logic = logics_.get(ix)->logic;
          logic->notify_cleanup();
          cleanup_extensions_(logic);
        }
        auto logic = logics_.get(ix)->logic;
        logic->notify_cleanup();
        cleanup_extensions_(logic);
      }
    }
  }

  t_void t_impl_::event_loop_() noexcept {
    t_err err;

    start_(err.tag(1));
    t_out{"t_impl_::enter event loop"};
    dispatcher_.event_loop(err.tag(2), this,
                           t_usec{spin_cnt_ * 1000 * spin_period_});
    t_out{"t_impl_::leave event loop"};
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

  t_void t_impl_::notify_may_reorder(r_event_infos infos) noexcept {
    //XXX
  }

  t_void t_impl_::notify_removed(r_event_info info) noexcept {
    // if a fd close it will be removed
    //XXX
  }

  t_impl_::t_quit t_impl_::notify_timeout(t_usec usec) noexcept {
    // go through all those that want to wait
    return t_quit{true}; //XXX
  }

  t_impl_::t_quit t_impl_::notify_error(t_errn errn) noexcept {
    t_out{"t_impl_::notify_error - prepare to die"};
    return t_quit{true}; //XXX
  }

  t_impl_::t_quit t_impl_::notify_all_processed(r_usec usec) noexcept {
    usec = t_usec{spin_cnt_ * 1000 * spin_period_};
    return t_quit{false}; //XXX
  }

  t_impl_::t_action t_impl_::notify_event(r_event_params params) noexcept {
    return t_action{}; //XXX
  }

///////////////////////////////////////////////////////////////////////////////

  t_single_impl_::t_single_impl_(t_err err, R_name name, R_params params,
                                 x_ptr ptr) noexcept
    : t_impl_{err, name, params, t_n{1}},
      ptr_{x_cast(ptr)} {
    ERR_GUARD(err) {
      register_logic(err, ptr_.get());
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_shared_impl_::t_shared_impl_(t_err err, R_name name, R_params params,
                                 x_ptrlist ptrlist) noexcept
    : t_impl_{err, name, params, ptrlist.get_size()},
      ptrlist_{x_cast(ptrlist)} {
    ERR_GUARD(err) {
      t_ix end = to_ix(ptrlist_.get_size());
      for (t_ix ix{0}; !err && ix < end; ++set(ix))
        register_logic(err, ptrlist_.get(ix)->get());
    }
  }

///////////////////////////////////////////////////////////////////////////////

}
}

