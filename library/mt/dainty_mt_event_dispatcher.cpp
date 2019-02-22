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

#include "dainty_os_fdbased.h"
#include "dainty_container_list.h"
#include "dainty_container_freelist.h"
#include "dainty_mt_event_dispatcher.h"

namespace dainty
{
namespace mt
{
namespace event_dispatcher
{
  using named::t_n_;
  using named::t_ix_;
  using named::P_cstr;
  using named::t_uint32;
  using os::fdbased::t_epoll;
  using err::r_err;

  using os::t_epoll_event;
  using p_epoll_event = t_prefix<t_epoll_event>::p_;

  using t_id     = container::freelist::t_id;
  using t_events = container::freelist::t_freelist<t_event_info>;
  using r_events = t_prefix<t_events>::r_;
  using p_logic = t_dispatcher::p_logic;

///////////////////////////////////////////////////////////////////////////////

  inline t_id to_(t_event_id id) {
    return t_id(get(id));
  }

  inline t_id to_(t_uint32 id) {
    return t_id(id);
  }

  inline t_event_id to_(t_id id) {
    return t_event_id(get(id));
  }

///////////////////////////////////////////////////////////////////////////////

  class t_impl_ {
  public:
    T_params params;

    t_impl_(R_params _params) noexcept
      : params(_params), events_{params.max}, infos_{params.max}  {
    }

    t_impl_(r_err err, R_params _params) noexcept
      : params(_params), events_{params.max}, infos_{err, params.max} {
    }

    virtual ~t_impl_() {
    }

    virtual operator t_validity() const noexcept {
      return events_ == VALID && infos_ == VALID ? VALID : INVALID;
    }

///////////////////////////////////////////////////////////////////////////////

    virtual t_errn add_event(        r_event_info) noexcept = 0;
    virtual t_void add_event(r_err,  r_event_info) noexcept = 0;
    virtual t_errn del_event(        r_event_info) noexcept = 0;
    virtual t_void del_event(r_err,  r_event_info) noexcept = 0;

    virtual t_errn wait_events(       r_events, r_event_infos,
                                      t_msec = t_msec{0})  noexcept = 0;
    virtual t_void wait_events(r_err, r_events, r_event_infos,
                                      t_msec = t_msec{0})  noexcept = 0;

///////////////////////////////////////////////////////////////////////////////

    t_bool get_events(r_event_ids ids) const noexcept {
      events_.each([&ids](auto id, auto&) mutable {
                   ids.push_back(to_(id)); });
      return !ids.is_empty();
    }

    P_event_info get_event(t_event_id id) const noexcept {
      return events_.get(to_(id));
    }

    P_event_info get_event(r_err err, t_event_id id) const noexcept {
      P_event_info info = events_.get(to_(id));
      if (info)
        return info;
      err = err::E_XXX;
      return nullptr;
    }

    t_void display() const noexcept {
      // access events
    }

    t_event_id add_event(R_event_params params, p_event_logic logic) noexcept {
      auto result = events_.insert({logic, params});
      if (result) {
        auto id_ = to_(result.id);
        result.ptr->id = id_;
        if (add_event(*result.ptr) == VALID)
          return id_;
        events_.erase(result.id);
      }
      return BAD_EVENT_ID;
    }

    t_event_id add_event(r_err err, R_event_params params,
                         p_event_logic logic) noexcept {
      auto result = events_.insert(err, {logic, params});
      if (result) {
        auto id_ = to_(result.id);
        result.ptr->id = id_;
        add_event(err, *result.ptr);
        if (!err)
          return id_;
        events_.erase(result.id);
      }
      return BAD_EVENT_ID;
    }

    p_event_logic del_event(t_event_id id) noexcept {
      auto id_  = to_(id);
      auto info = events_.get(id_);
      if (info) {
        del_event(*info);
        events_.erase(id_);
      }
      return nullptr;
    }

    p_event_logic del_event(r_err err, t_event_id id) noexcept {
      auto id_  = to_(id);
      auto info = events_.get(err, id_);
      if (info) {
        del_event(err, *info);
        events_.erase(id_);
      }
      return nullptr;
    }

    t_void clear_events() noexcept {
      events_.each([this](auto, auto& info) {
        del_event(info);
      });
      events_.clear();
    }

    t_void clear_events(r_err) noexcept {
      events_.each([this](auto, auto& info) { //XXX - do you need err?
        del_event(info); // XXX this version may be removed
      });
      events_.clear();
    }

///////////////////////////////////////////////////////////////////////////////

    t_quit process_events(r_event_infos infos, p_logic logic) noexcept {
      if (!infos.is_empty()) {
        logic->notify_dispatcher_reorder(infos);
        t_ix_ end = get(infos.get_size());
        for (t_ix_ ix = 0; ix < end; ++ix) {
          auto     info   = infos.get(t_ix{ix});
          t_action action =
            info->logic ?
              info->logic->notify_dispatcher_event(info->id, info->params) :
                    logic->notify_dispatcher_event(info->id, info->params);
          switch (action.cmd) {
            case CONTINUE: {
              p_event_logic next = action.next;
              if (next)
                info->logic = next;
            } break;
            case REMOVE_EVENT:
              logic->notify_dispatcher_removed(*info);
              del_event(info->id);
              break;
            case QUIT_EVENT_LOOP:
              return QUIT;
          }
        }
      }
      return DONT_QUIT;
    }

///////////////////////////////////////////////////////////////////////////////

    t_n event_loop(p_logic logic) noexcept {
      return event_loop(logic, t_msec{0});
    }

    t_n event_loop(r_err err, p_logic logic) noexcept {
      return event_loop(err, logic, t_msec{0});
    }

    t_n event_loop(p_logic logic, t_msec msec) noexcept {
      t_n_   cnt  = 0;
      t_quit quit{false};
      do {
        t_errn errn = wait_events(events_, infos_, msec);
        if (errn == VALID) {
          if (!infos_.is_empty())
            quit = process_events(infos_, logic);
          else
            quit = logic->notify_dispatcher_timeout(msec);
        } else {
          quit = logic->notify_dispatcher_error(errn);
          if (quit == DONT_QUIT)
            errn = t_errn{0};
        }

        if (quit == DONT_QUIT)
          quit = logic->notify_dispatcher_processed(msec);

        infos_.clear();
        ++cnt;
      } while (quit == DONT_QUIT);
      return t_n{cnt};
    }

    t_n event_loop(r_err err, p_logic logic, t_msec msec) noexcept {
      t_n_   cnt = 0;
      t_quit quit{false};
      do {
        wait_events(err, events_, infos_, msec);
        if (!err) {
          if (!infos_.is_empty())
            quit = process_events(infos_, logic);
          else
            quit = logic->notify_dispatcher_timeout(msec);
        } else {
          quit = logic->notify_dispatcher_error(t_errn(err.id()));
          if (quit == DONT_QUIT)
            err.clear();
        }

        if (quit == DONT_QUIT)
          quit = logic->notify_dispatcher_processed(msec);

        infos_.clear();
        ++cnt;
      } while (quit == DONT_QUIT);
      return t_n{cnt};
    }

  private:
    t_events       events_;
    t_event_infos  infos_;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_epoll_impl_ : public t_impl_ {
  public:
    t_epoll_impl_(R_params _params) noexcept
      : t_impl_{_params}, epoll_events_{new t_epoll_event[get(_params.max)]},
        epoll_{} {
    }

    t_epoll_impl_(r_err err, R_params _params) noexcept
      : t_impl_{_params}, epoll_events_{new t_epoll_event[get(_params.max)]},
        epoll_{err} {
    }

    ~t_epoll_impl_() {
      delete [] epoll_events_;
    }

    operator t_validity() const noexcept override {
      return t_impl_::operator t_validity() == VALID &&
             epoll_events_ && epoll_ == VALID ? VALID : INVALID;
    }

    t_errn add_event(r_event_info info) noexcept override {
      t_epoll::t_event_data data;
      data.u32 = get(info.id);
      return epoll_.add_event(info.params.fd,
                              info.params.type == RD_EVENT ? EPOLLIN : EPOLLOUT,
                              data);
    }

    t_void add_event(r_err err,  r_event_info info) noexcept override {
      t_epoll::t_event_data data;
      data.u32 = get(info.id);
      epoll_.add_event(err, info.params.fd,
                       info.params.type == RD_EVENT ? EPOLLIN : EPOLLOUT, data);
    }

    t_errn del_event(r_event_info info) noexcept override {
      return epoll_.del_event(info.params.fd);
    }

    t_void del_event(r_err err, r_event_info info) noexcept override {
      epoll_.del_event(err, info.params.fd);
    }

    t_errn wait_events(r_events events, r_event_infos infos,
                       t_msec msec) noexcept override {
      auto verify = get(msec) ? epoll_.wait(epoll_events_, params.max, msec)
                              : epoll_.wait(epoll_events_, params.max);
      if (verify == VALID) {
        for (t_n_ cnt = 0; cnt < get(verify); ++cnt)
          infos.push_back(events.get(to_(epoll_events_[0].data.u32)));
      }
      return verify.errn;
    }

    t_void wait_events(r_err err, r_events events,
                       r_event_infos infos, t_msec msec) noexcept override {
      t_n_ n = get((get(msec) ?
                      epoll_.wait(err, epoll_events_, params.max, msec) :
                      epoll_.wait(err, epoll_events_, params.max)));
      if (!err) {
        for (t_n_ cnt = 0; cnt < n; ++cnt)
          infos.push_back(events.get(to_(epoll_events_[0].data.u32)));
      }
    }

  private:
    p_epoll_event epoll_events_;
    t_epoll       epoll_;
  };

///////////////////////////////////////////////////////////////////////////////

  t_n get_supported_services() noexcept {
    return t_n{1};
  }

  t_service_name get_supported_service(t_ix) noexcept {
    return "epoll_service";
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_dispatcher::t_logic
      ::notify_dispatcher_reorder(r_event_infos) noexcept {
    // XXX provide default reorder - thats reorders on prio
  }

  t_void t_dispatcher::t_logic
      ::notify_dispatcher_removed(r_event_info) noexcept {
  }

  t_quit t_dispatcher::t_logic
      ::notify_dispatcher_processed(r_msec) noexcept {
    return DONT_QUIT;
  }

///////////////////////////////////////////////////////////////////////////////

  t_dispatcher::t_dispatcher(R_params params) noexcept {
    if (params.service_name == P_cstr("epoll_service"))
      impl_ = new t_epoll_impl_{params};
    else {
      // not yet supported - required for non-linux systems
    }
  }

  t_dispatcher::t_dispatcher(t_err err, R_params params) noexcept {
    ERR_GUARD(err) {
      if (params.service_name == P_cstr("epoll_service"))
        impl_ = new t_epoll_impl_{err, params};
      else {
        err = err::E_XXX;
        // not yet supported - required for non-linux systems
      }
    }
  }

  t_dispatcher::t_dispatcher(x_dispatcher dispatcher) noexcept
    : impl_{dispatcher.impl_.release()} {
  }

  t_dispatcher::~t_dispatcher() noexcept {
    if (*this == VALID)
      impl_->clear_events();
  }

  t_dispatcher::operator t_validity() const noexcept {
    return impl_ == VALID && *impl_ == VALID ? VALID : INVALID;
  }

  t_params t_dispatcher::get_params() const noexcept {
    if (*this == VALID)
      return impl_->params;
    return t_params{t_n{0}, ""};
  }

  t_void t_dispatcher::display() const noexcept {
    if (*this == VALID)
      impl_->display();
  }

  t_event_id t_dispatcher::add_event(R_event_params params,
                                     p_event_logic logic) noexcept {
    if (*this == VALID)
      return impl_->add_event(params, logic);
    return t_event_id{0};
  }

  t_event_id t_dispatcher::add_event(t_err err, R_event_params params,
                                     p_event_logic logic) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        return impl_->add_event(err, params, logic);
      err = err::E_XXX;
    }
    return t_event_id{0};
  }

  p_event_logic t_dispatcher::del_event(t_event_id id) noexcept {
    if (*this == VALID)
      return impl_->del_event(id);
    return nullptr;
  }

  p_event_logic t_dispatcher::del_event(t_err err, t_event_id id) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        return impl_->del_event(err, id);
      err = err::E_XXX;
    }
    return nullptr;
  }

  t_void t_dispatcher::clear_events() noexcept {
    if (*this == VALID)
      impl_->clear_events();
  }

  t_void t_dispatcher::clear_events(t_err err) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        impl_->clear_events(err);
      else
        err = err::E_XXX;
    }
  }

  P_event_info t_dispatcher::get_event(t_event_id id) const noexcept {
    if (*this == VALID)
      return impl_->get_event(id);
    return nullptr;
  }

  P_event_info t_dispatcher::get_event(t_err err,
                                       t_event_id id) const noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        return impl_->get_event(err, id);
      err = err::E_XXX;
    }
    return nullptr;
  }

  t_bool t_dispatcher::get_events(r_event_ids ids) const noexcept {
    if (*this == VALID)
      return impl_->get_events(ids);
    return false;
  }

  t_n t_dispatcher::event_loop(p_logic logic) noexcept {
    if (*this == VALID)
      return impl_->event_loop(logic);
    return t_n{0};
  }

  t_n t_dispatcher::event_loop(t_err err, p_logic logic) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        return impl_->event_loop(err, logic);
      err = err::E_XXX;
    }
    return t_n{0};
  }

  t_n t_dispatcher::event_loop(p_logic logic, t_msec msec) noexcept {
    if (*this == VALID)
      return impl_->event_loop(logic, msec);
    return t_n{0};
  }

  t_n t_dispatcher::event_loop(t_err err, p_logic logic, t_msec msec) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        return impl_->event_loop(err, logic, msec);
      err = err::E_XXX;
    }
    return t_n{0};
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}
