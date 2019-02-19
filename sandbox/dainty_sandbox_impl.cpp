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
  using mt::event_dispatcher::RD_EVENT;
  using mt::event_dispatcher::QUIT_EVENT_LOOP;
  using mt::event_dispatcher::CONTINUE;

///////////////////////////////////////////////////////////////////////////////

  t_impl_::t_impl_(t_err err, R_thread_name name, R_thread_params params,
                   t_n max_logics) noexcept
      : name_      {name},
        params_    {params},
        closefd_   {os::call_eventfd(err, t_n{0})},
        dispatcher_{err, {t_n{100}, "epoll_service"}},
        logics_    {err, max_logics}  {
    ERR_GUARD(err) {
    }
  }

  t_impl_::~t_impl_() {
    os::call_close(closefd_); // replace later
    t_out{"t_impl_ dies graciously"};
  }

  t_fd t_impl_::get_close_fd() const noexcept {
    return closefd_;
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_impl_::register_logic(t_err err, p_logic logic) noexcept {
    ERR_GUARD(err) {
      t_out{FMT, "register logic name - %s",
            get(logic->get_messenger_name().get_cstr())};
      t_ix pos = to_ix(logics_.get_size());
      auto entry = logics_.push_back(err);
      if (entry) {
       entry->logic = logic;

       // tracing service   - tracer
       // messaging service - messenger

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

    t_ix cnt{0}, end_ix{to_ix(logics_.get_size())};
    for (; cnt < end_ix; ++set(cnt))
      if (logics_.get(cnt)->spin_cnt_max)
        break;

    if (cnt == end_ix)
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

  P_timer_params t_impl_::get_timer(t_ix ix, t_timer_id id) const noexcept {
    //XXX
    return nullptr;
  }

///////////////////////////////////////////////////////////////////////////////

  t_fdevent_id t_impl_::add_fdevent(t_err err, t_ix ix, R_fdevent_name name,
                                    R_fdevent_params params) noexcept {
    ERR_GUARD(err) {
      auto logic_entry = logics_.get(ix);
      if (!logic_entry->fds_ids.is_full()) {
        auto result = fdevents_.insert(err);
        if (result) {
          t_fdevent_id id = mk_(result.id, generate_fdevent_session_id_());
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
            logic_entry->fds_ids.push_back(id);
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
      if (!logic_entry->fds_ids.is_full()) {
        auto result = fdevents_.insert(err);
        if (result) {
          t_fdevent_id id = mk_(result.id, generate_fdevent_session_id_());
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
            fd_entry->notify_ptr = x_cast(notify_ptr);
            fd_entry->id         = id;
            logic_entry->fds_ids.push_back(id);
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
    auto entry_id = get_id_(id);
    auto fd_entry = fdevents_.get(entry_id);

    if (fd_entry && id == fd_entry->id && ix == fd_entry->logic_ix) {
      dispatcher_.del_event(fd_entry->ev_id);

      if (fd_entry->notify_ptr == VALID)
        tmp = x_cast(fd_entry->notify_ptr);

      auto logic_entry = logics_.get(ix);
      logic_entry->fds_ids.erase(
        logic_entry->fds_ids.find_if(
          [&id](auto& _id) -> t_bool { return id == _id; }));

      fdevents_.erase(entry_id);
    }
    return tmp;
  }

  P_fdevent_params t_impl_::get_fdevent(t_ix ix,
                                        t_fdevent_id id) const noexcept {
    auto entry_id = get_id_(id);
    auto fd_entry = fdevents_.get(entry_id);

    if (fd_entry && id == fd_entry->id && ix == fd_entry->logic_ix)
      return &fd_entry->params;

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

///////////////////////////////////////////////////////////////////////////////

  t_freelist_entry_id_ t_impl_::generate_fdevent_session_id_() {
    return 0;
  }

  // should not be class methods  change later
  t_fdevent_id t_impl_::mk_(t_fdevents_entry_id_, t_freelist_entry_id_) const {
    return t_fdevent_id{0};
  }

   t_impl_::t_fdevents_entry_id_ t_impl_::get_id_(t_fdevent_id) const {
    return t_fdevents_entry_id_{0};
  }

  t_freelist_entry_id_ t_impl_::get_session_id_(t_fdevent_id) const {
    return 0;
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_impl_::update(base1::t_err err,
                         base1::r_pthread_attr attr) noexcept {
    ERR_GUARD(err) {
      // XXX - can use params_ to set attr
    }
  }

  t_void t_impl_::prepare(base1::t_err err) noexcept {
    ERR_GUARD(err) {
      dispatcher_.add_event(err, {closefd_, RD_EVENT});
      // XXX - can use params_ for further preparation
    }
  }

  t_void t_impl_::run() noexcept {
    event_loop_();
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_impl_::notify_reorder(r_event_infos infos) noexcept {
    t_out{"t_impl_::notify_reorder"};
    //XXX
  }

  t_void t_impl_::notify_removed(r_event_info info) noexcept {
    t_out{"t_impl_::notify_removed"};
    // if a fd close it will be removed
    //XXX
  }

  t_impl_::t_quit t_impl_::notify_timeout(t_msec msec) noexcept {
    t_ix end_ix = to_ix(logics_.get_size());
    for (t_ix ix{0}; ix < end_ix; ++set(ix)) {
      auto logic_entry = logics_.get(ix);
      if (logic_entry->spin_cnt_max) {
        logic_entry->spin_cnt += spin_cnt_;
        if (logic_entry->spin_cnt == logic_entry->spin_cnt_max) {
          logic_entry->spin_cnt = 0;
          logic_entry->logic->notify_spin(t_msec{spin_period_*
                                                 logic_entry->spin_cnt_max});
        }
      }
    }
    return t_quit{false};
  }

  t_impl_::t_quit t_impl_::notify_error(t_errn errn) noexcept {
    t_out{"t_impl_::notify_error - prepare to die"};
    return t_quit{true};
  }

  t_impl_::t_quit t_impl_::notify_processed(r_msec msec) noexcept {
    msec = t_msec{spin_cnt_ * spin_period_};
    return t_quit{false};
  }

  t_impl_::t_action t_impl_::notify_event(t_event_id ev_id,
                                          r_event_params params) noexcept {
    t_fdevent_id id(params.user.id); // seession ddata in it too
    auto entry_id = get_id_(id);
    auto fd_entry = fdevents_.get(entry_id);

    if (fd_entry && fd_entry->id == id) {
       if (fd_entry->notify_ptr == VALID)
         fd_entry->notify_ptr->notify_fdevent(id, fd_entry->params);
       else {
         auto logic_entry = logics_.get(fd_entry->logic_ix);
         logic_entry->logic->notify_fdevent(id, fd_entry->params);
      }
    } else {
      t_out{"this should never happen - recv unknown event"};
    }
    return t_action{CONTINUE};
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_impl_::event_loop_() noexcept {
    t_err err;

    start_(err.tag(1));
    dispatcher_.event_loop(err.tag(2), this,
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

