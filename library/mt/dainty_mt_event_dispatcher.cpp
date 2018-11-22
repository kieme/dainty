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
#include "dainty_mt_event_dispatcher.h"

namespace dainty
{
namespace mt
{
namespace event_dispatcher
{
  using named::t_n_;
  using named::P_cstr;
  using os::fdbased::t_epoll;
  using os::t_epoll_event;
  using err::r_err;

  using t_events = container::freelist::t_freelist<t_event_info>;
  using r_events = named::t_prefix<t_events>::r_;

///////////////////////////////////////////////////////////////////////////////

  using p_logic = t_dispatcher::p_logic;

  class t_impl_ {
  public:
    const t_params params;

    t_impl_(R_params _params) : params(_params), events_{params.max} {
      infos_.reserve(get(params.max));
    }

    t_impl_(r_err err, R_params _params)
      : params(_params), events_{params.max} { // XXX - no real check
      ERR_GUARD(err) {
        infos_.reserve(get(params.max));
      }
    }

    virtual ~t_impl_() {
    }

    virtual operator t_validity() const {
      return events_ == VALID ? VALID : INVALID;
    }

///////////////////////////////////////////////////////////////////////////////

    virtual t_errn add_event(        r_event_info) = 0;
    virtual t_void add_event(r_err,  r_event_info) = 0;
    virtual t_errn del_event(        r_event_info) = 0;
    virtual t_void del_event(r_err,  r_event_info) = 0;

    virtual t_errn wait_events(       r_events, r_event_infos) = 0;
    virtual t_void wait_events(r_err, r_events, r_event_infos) = 0;
    virtual t_errn wait_events(       r_events, r_event_infos, t_usec) = 0;
    virtual t_void wait_events(r_err, r_events, r_event_infos, t_usec) = 0;

///////////////////////////////////////////////////////////////////////////////

    t_bool fetch_events(r_ids ids) const {
      events_.each([&ids](t_id id, const t_event_info&) mutable {
                   ids.push_back(id); });
      return !ids.empty();
    }

    P_event_info get_event(t_id id) const {
      return events_.get(id);
    }

    P_event_info get_event (r_err err, t_id id) const {
      P_event_info info = events_.get(id);
      if (info)
        return info;
      err = err::E_XXX;
      return nullptr;
    }

    t_void display() const {
      // access events
    }

    t_id add_event(R_event_params params, p_event_logic logic) {
      auto result = events_.insert({logic, params});
      if (result) {
        result.ptr->id = result.id;
        if (add_event(*result.ptr) == VALID)
          return result.id;
        events_.erase(result.id);
      }
      return t_id{0};
    }

    t_id add_event(r_err err, R_event_params params, p_event_logic logic) {
      auto result = events_.insert(err, {logic, params});
      if (result) {
        result.ptr->id = result.id;
        add_event(err, *result.ptr);
        if (!err)
          return result.id;
        events_.erase(result.id);
      }
      return t_id{0};
    }

    p_event_logic del_event(t_id id) {
      auto info = events_.get(id);
      if (info) {
        del_event(*info);
        events_.erase(id);
      }
      return nullptr;
    }

    p_event_logic del_event(r_err err, t_id id) {
      auto info = events_.get(err, id);
      if (info) {
        del_event(err, *info);
        events_.erase(id);
      }
      return nullptr;
    }

    t_void clear_events() {
      events_.each([this](t_id, r_event_info& info) {
        del_event(info);
      });
      events_.clear();
    }

    t_void clear_events(r_err) {
      events_.each([this](t_id, r_event_info& info) { //XXX - do you need err?
        del_event(info); // XXX this version may be removed
      });
      events_.clear();
    }

///////////////////////////////////////////////////////////////////////////////

    t_quit process_events(r_event_infos infos, p_logic logic) {
      if (!infos.empty()) {
        logic->may_reorder_events(infos);
        for (auto info : infos) {
          t_action action = info->logic->notify_event(info->params);
          switch (action.cmd) {
            case CONTINUE: {
              t_event_logic* next = action.next;
              if (next)
                info->logic = next;
            } break;
            case REMOVE_EVENT:
              del_event(info->id);
              break;
            case QUIT_EVENT_LOOP:
              return true;
          }
        }
        return logic->notify_events_processed();
      }
      return false;
    }

///////////////////////////////////////////////////////////////////////////////

    t_n event_loop(p_logic logic) {
      t_n_ cnt = 0;
      t_quit quit = false;
      do {
        auto errn = wait_events(events_, infos_);
        if (errn == VALID) {
          if (!infos_.empty())
            quit = process_events(infos_, logic);
          else
            quit = true;
        } else
          quit = logic->notify_error(errn);
        infos_.clear();
        ++cnt;
      } while (!quit);
      return t_n{cnt};
    }

    t_n event_loop(r_err err, p_logic logic) {
      t_n_ cnt = 0;
      t_quit quit = false;
      do {
        wait_events(err, events_, infos_);
        if (!err) {
          if (!infos_.empty())
            quit = process_events(infos_, logic);
          else
            quit = true;
        } else
          quit = logic->notify_error(t_errn(err.id()));
        infos_.clear();
        ++cnt;
      } while (!quit);
      return t_n{cnt};
    }

    t_n event_loop(p_logic logic, t_usec usec) {
      t_n_ cnt = 0;
      t_quit quit = false;
      do {
        t_errn errn = wait_events(events_, infos_, usec);
        if (errn == VALID) {
          if (!infos_.empty())
            quit = process_events(infos_, logic);
          else
            quit = logic->notify_timeout(usec);
        } else
          quit = logic->notify_error(errn);
        infos_.clear();
        ++cnt;
      } while (!quit);
      return t_n{cnt};
    }

    t_n event_loop(r_err err, p_logic logic, t_usec usec) {
      t_n_ cnt = 0;
      t_quit quit = false;
      do {
        wait_events(err, events_, infos_, usec);
        if (!err) {
          if (!infos_.empty())
            quit = process_events(infos_, logic);
          else
            quit = logic->notify_timeout(usec);
        } else
          quit = logic->notify_error(t_errn(err.id()));
        infos_.clear();
        ++cnt;
      } while (!quit);
      return t_n{cnt};
    }

  private:
    t_events       events_;
    t_event_infos  infos_;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_epoll_impl_ : public t_impl_ {
  public:
    t_epoll_impl_(R_params _params)
      : t_impl_{_params}, epoll_events_{new t_epoll_event[get(_params.max)]},
        epoll_{} {
    }

    t_epoll_impl_(r_err err, R_params _params)
      : t_impl_{_params}, epoll_events_{new t_epoll_event[get(_params.max)]},
        epoll_{err} {
    }

    ~t_epoll_impl_() {
      delete [] epoll_events_;
    }

    virtual operator t_validity() const override {
      return t_impl_::operator t_validity() == VALID &&
             epoll_events_ && epoll_ == VALID ? VALID : INVALID;
    }

    virtual t_errn add_event(r_event_info info) override {
     t_epoll::t_event_data data;
     data.u32 = get(info.id);
     return epoll_.add_event(info.params.fd,
                             info.params.type == RD ? EPOLLIN : EPOLLOUT,
                             data);
    }

    virtual t_void add_event(r_err err,  r_event_info info) override {
     t_epoll::t_event_data data;
     data.u32 = get(info.id);
     epoll_.add_event(err, info.params.fd,
                      info.params.type == RD ? EPOLLIN : EPOLLOUT, data);
    }

    virtual t_errn del_event(r_event_info info) override {
      return epoll_.del_event(info.params.fd);
    }

    virtual t_void del_event(r_err err, r_event_info info) override {
      epoll_.del_event(err, info.params.fd);
    }

    virtual t_errn wait_events(r_events events, r_event_infos infos) override {
      auto verify = epoll_.wait(epoll_events_, params.max);
      if (verify == VALID) {
        for (t_n_ cnt = 0; cnt < get(verify); ++cnt)
          infos.push_back(events.get(t_id{epoll_events_[0].data.u32}));
      }
      return verify.errn;
    }

    virtual t_void wait_events(r_err err, r_events events,
                               r_event_infos infos) override {
      t_n_ n = get(epoll_.wait(err, epoll_events_, params.max));
      if (!err) {
        for (t_n_ cnt = 0; cnt < n; ++cnt)
          infos.push_back(events.get(t_id{epoll_events_[0].data.u32}));
      }
    }

    virtual t_errn wait_events(r_events events, r_event_infos infos,
                               t_usec usec) override {
      auto verify = epoll_.wait(epoll_events_, params.max, usec);
      if (verify == VALID) {
        for (t_n_ cnt = 0; cnt < get(verify); ++cnt)
          infos.push_back(events.get(t_id{epoll_events_[0].data.u32}));
      }
      return verify.errn;
    }

    virtual t_void wait_events(r_err err, r_events events,
                               r_event_infos infos, t_usec usec) override {
      t_n_ n = get(epoll_.wait(err, epoll_events_, params.max, usec));
      if (!err) {
        for (t_n_ cnt = 0; cnt < n; ++cnt)
          infos.push_back(events.get(t_id{epoll_events_[0].data.u32}));
      }
    }

  private:
    t_epoll_event* epoll_events_;
    t_epoll        epoll_;
  };

///////////////////////////////////////////////////////////////////////////////

  t_n get_supported_services() {
     return t_n{1};
  }

  t_service_name get_supported_service(t_ix) {
    return "epoll_service";
  }

///////////////////////////////////////////////////////////////////////////////

  t_dispatcher::t_dispatcher(R_params params) {
    if (params.service_name == P_cstr("epoll_service"))
      impl_ = new t_epoll_impl_(params);
    else if (params.service_name == P_cstr("select_service")) {
    }
  }

  t_dispatcher::t_dispatcher(t_err err, R_params params) {
    ERR_GUARD(err) {
      if (params.service_name == P_cstr("epoll_service"))
        impl_ = new t_epoll_impl_(err, params);
      else if (params.service_name == P_cstr("select_service")) {
      } else
        err = err::E_XXX;
    }
  }

  t_dispatcher::t_dispatcher(x_dispatcher dispatcher)
    : impl_{dispatcher.impl_.release()} {
  }

  t_dispatcher::~t_dispatcher() {
    impl_.clear();
  }

  t_dispatcher::operator t_validity() const {
    return impl_ == VALID && *impl_ == VALID ? VALID : INVALID;
  }

  t_params t_dispatcher::get_params() const {
    if (*this == VALID)
      return impl_->params;
    return t_params{t_n{0}, ""};
  }

  t_void t_dispatcher::display() const {
    if (*this == VALID)
      impl_->display();
  }

  t_id t_dispatcher::add_event(R_event_params params, p_event_logic logic) {
    if (*this == VALID)
      return impl_->add_event(params, logic);
    return t_id{0};
  }

  t_id t_dispatcher::add_event(t_err err, R_event_params params,
                               p_event_logic logic) {
    ERR_GUARD(err) {
      if (*this == VALID)
        return impl_->add_event(err, params, logic);
      err = err::E_XXX;
    }
    return t_id{0};
  }

  p_event_logic t_dispatcher::del_event(t_id id) {
    if (*this == VALID)
      return impl_->del_event(id);
    return nullptr;
  }

  p_event_logic t_dispatcher::del_event(t_err err, t_id id) {
    ERR_GUARD(err) {
      if (*this == VALID)
        return impl_->del_event(err, id);
      err = err::E_XXX;
    }
    return nullptr;
  }

  t_void t_dispatcher::clear_events() {
    if (*this == VALID)
      impl_->clear_events();
  }

  t_void t_dispatcher::clear_events(t_err err) {
    ERR_GUARD(err) {
      if (*this == VALID)
        impl_->clear_events(err);
      else
        err = err::E_XXX;
    }
  }

  P_event_info t_dispatcher::get_event(t_id id) const {
    if (*this == VALID)
      return impl_->get_event(id);
    return nullptr;
  }

  P_event_info t_dispatcher::get_event(t_err err, t_id id) const {
    ERR_GUARD(err) {
      if (*this == VALID)
        return impl_->get_event(err, id);
      err = err::E_XXX;
    }
    return nullptr;
  }

  t_bool t_dispatcher::fetch_events(r_ids ids) const {
    if (*this == VALID)
      return impl_->fetch_events(ids);
    return false;
  }

  t_n t_dispatcher::event_loop(p_logic logic) {
    if (*this == VALID)
      return impl_->event_loop(logic);
    return t_n{0};
  }

  t_n t_dispatcher::event_loop(t_err err, p_logic logic) {
    ERR_GUARD(err) {
      if (*this == VALID)
        return impl_->event_loop(err, logic);
      err = err::E_XXX;
    }
    return t_n{0};
  }

  t_n t_dispatcher::event_loop(p_logic logic, t_usec usec) {
    if (*this == VALID)
      return impl_->event_loop(logic, usec);
    return t_n{0};
  }

  t_n t_dispatcher::event_loop(t_err err, p_logic logic, t_usec usec) {
    ERR_GUARD(err) {
      if (*this == VALID)
        return impl_->event_loop(err, logic, usec);
      err = err::E_XXX;
    }
    return t_n{0};
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}
