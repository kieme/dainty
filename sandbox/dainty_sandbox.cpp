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
  using r_err          = t_prefix<t_err>::r_;
  using t_thread_attr_ = os::t_pthread_attr;
  using named::t_ix;

  using named::terminal::t_out;
  using named::utility::x_cast;
  using os::call_pthread_init;

///////////////////////////////////////////////////////////////////////////////

  t_logic_ptrlist::t_logic_ptrlist(t_err err, t_n max) : list_{err, max} {
  }

  t_logic_ptrlist::t_logic_ptrlist(x_logic_ptrlist ptrlist)
    : list_{x_cast(ptrlist.list_)} {
  }

  t_void t_logic_ptrlist::push_back(t_err err, x_logic_ptr ptr) noexcept {
    ERR_GUARD(err) {
      // XXX check the names
      list_.push_back(err, x_cast(ptr));
    }
  }

  t_n t_logic_ptrlist::get_size() const noexcept {
    return list_.get_size();
  }

  p_logic_ptr t_logic_ptrlist::get(t_ix ix) noexcept {
    return list_.get(ix);
  }

  P_logic_ptr t_logic_ptrlist::get(t_ix ix) const noexcept {
    return list_.get(ix);
  }

///////////////////////////////////////////////////////////////////////////////

  t_void register_(t_err err, r_logic logic, p_logic_ext ext) {
    ERR_GUARD(err) {
      if (ext) {
        t_ix end = to_ix(logic.exts_.get_size());
        for (t_ix ix{0}; ix < end; set(ix)++) {
          p_logic_ext ext_ptr = logic.exts_.get(ix);
          if (ext == ext_ptr || ext_ptr->get_name() == ext->get_name()) {
            err = err::E_XXX; // already
            break;
          }
        }
        logic.exts_.push_back(err, ext);
      } else
        err = err::E_XXX;
    }
  }

  R_extension_name t_logic_ext::get_name() const noexcept {
    return name_;
  }

  r_logic t_logic_ext::get_logic() noexcept {
    return logic_;
  }

  R_logic t_logic_ext::get_logic() const noexcept {
    return logic_;
  }

///////////////////////////////////////////////////////////////////////////////

  t_logic::t_logic(t_err err, R_messenger_name name) noexcept : name_{name} {
  }

  t_logic::operator t_validity() const {
    return impl_ ? VALID : INVALID;
  }

  R_messenger_name t_logic::get_messenger_name() const noexcept {
    return name_;
  }

  R_logic_stats t_logic::get_logic_stats() const noexcept {
    return stats_;
  }

  t_void t_logic::enable_spin(t_err err, t_spin_cnt cnt) noexcept {
    ERR_GUARD(err) {
      if (impl_)
        return impl_->enable_spin(err, ix_, cnt);
      err = err::E_XXX;
    }
  }

  t_void t_logic::disable_spin() noexcept {
    if (impl_)
      return impl_->disable_spin(ix_);
  }

  t_msec t_logic::get_spin_period() const noexcept {
    if (impl_)
      return impl_->get_spin_period(ix_);
    return t_msec{0};
  }

  t_spin_cnt t_logic::get_spin_cnt() const noexcept {
    if (impl_)
      return impl_->get_spin_cnt(ix_);
    return t_spin_cnt{0};
  }

  t_timer_id t_logic::start_timer(t_err err, R_timer_name name,
                                  R_timer_params params) noexcept {
    ERR_GUARD(err) {
      if (impl_)
        return impl_->start_timer(err, ix_, name, params);
      err = err::E_XXX;
    }
    return t_timer_id{-1};
  }

  t_timer_id t_logic::start_timer(t_err err, R_timer_name name,
                                  R_timer_params params,
                                  x_timer_notify_ptr ptr) noexcept {
    ERR_GUARD(err) {
      if (impl_)
        return impl_->start_timer(err, ix_, name, params, x_cast(ptr));
      err = err::E_XXX;
    }
    return t_timer_id{-1};
  }

  t_void t_logic::restart_timer(t_err err, t_timer_id id,
                                R_timer_params params) noexcept {
    ERR_GUARD(err) {
      if (impl_)
        return impl_->restart_timer(err, ix_, id, params);
      err = err::E_XXX;
    }
  }

  t_timer_notify_ptr t_logic::stop_timer(t_timer_id id) noexcept {
    if (impl_)
      return impl_->stop_timer(ix_, id);
    return {nullptr, nullptr};
  }

  P_timer_info t_logic::get_timer(t_timer_id id) const noexcept {
    if (impl_)
      return impl_->get_timer(ix_, id);
    return nullptr;
  }

  t_fdevent_id t_logic::add_fdevent(t_err err, R_fdevent_name name,
                                    R_fdevent_params params) noexcept {
    ERR_GUARD(err) {
      if (impl_)
        return impl_->add_fdevent(err, ix_, name, params);
      err = err::E_XXX;
    }
    return t_fdevent_id{0};
  }

  t_fdevent_id t_logic::add_fdevent(t_err err, R_fdevent_name name,
                                    R_fdevent_params params,
                                    x_fdevent_notify_ptr ptr) noexcept {
    ERR_GUARD(err) {
      if (impl_)
        return impl_->add_fdevent(err, ix_, name, params, x_cast(ptr));
      err = err::E_XXX;
    }
    return t_fdevent_id{0};
  }

  t_fdevent_notify_ptr t_logic::del_fdevent(t_fdevent_id id) noexcept {
    if (impl_)
      return impl_->del_fdevent(ix_, id);
    return t_fdevent_notify_ptr{};
  }

  P_fdevent_info t_logic::get_fdevent(t_fdevent_id id) const noexcept {
    if (impl_)
      return impl_->get_fdevent(ix_, id);
    return nullptr;
  }

///////////////////////////////////////////////////////////////////////////////

  t_thread::t_thread(t_err err, R_name name, x_ptr logic_ptr,
                     R_params params) noexcept {
    ERR_GUARD(err) {
      auto impl = new t_single_impl_{err, name, params, x_cast(logic_ptr)};
      t_id id{impl->get_dispatcher_fd()};
      t_thread_ thread{err, name.get_cstr(), x_cast(impl)};
      if (!err)
        id_ = id;
    }
  }

  t_thread::t_thread(t_err err, R_name name, x_ptrlist list,
                     R_params params) noexcept {
    ERR_GUARD(err) {
      auto impl = new t_shared_impl_{err, name, params, x_cast(list)};
      t_id id{impl->get_dispatcher_fd()};
      t_thread_ thread{err, name.get_cstr(), x_cast(impl)};
      if (!err)
        id_ = id;
    }
  }

  t_id t_thread::get_id() const noexcept {
    return id_;
  }

  t_thread::operator t_validity() const noexcept {
    return id_ != BAD_ID ? VALID : INVALID;
  }

///////////////////////////////////////////////////////////////////////////////

  t_main::t_main(t_err err, R_name name, x_ptr logic,
                 R_params params) noexcept {
    ERR_GUARD(err) {
      t_single_impl_ impl{err, name, params, x_cast(logic)};
      t_id id{impl.get_dispatcher_fd()};
      t_thread_attr_ attr;
      call_pthread_init(err, attr);
      impl.update(err, attr);
      impl.prepare(err);
      if (!err) {
        id_ = id;
        impl.run();
      }
    }
  }

  t_main::t_main(t_err err, R_name name, x_ptrlist list,
                 R_params params) noexcept {
    ERR_GUARD(err) {
      t_shared_impl_ impl{err, name, params, x_cast(list)};
      t_id id{impl.get_dispatcher_fd()};
      t_thread_attr_ attr;
      call_pthread_init(err, attr);
      impl.update(err, attr);
      impl.prepare(err);
      if (!err) {
        id_ = id;
        impl.run();
      }
    }
  }

  t_id t_main::get_id() const noexcept {
    return id_;
  }

  t_main::operator t_validity() const noexcept {
    return id_ != BAD_ID ? VALID : INVALID;
  }

///////////////////////////////////////////////////////////////////////////////

  class t_thread_of_control_ {
  public:
    virtual ~t_thread_of_control_() { };
    virtual t_id get_id() const noexcept = 0;
  };
  using p_thread_of_control_ = t_prefix<t_thread_of_control_>::p_;

  class t_thread_control_ final : public t_thread_of_control_ {
  public:
    using R_name    = R_thread_name;
    using R_params  = R_thread_params;
    using x_ptr     = x_logic_ptr;
    using x_ptrlist = x_logic_ptrlist;

    t_thread_control_(r_err err, R_name name, x_ptr ptr,
                      R_params params) noexcept
      : thread_{err, name, x_cast(ptr), params} {
    }
    t_thread_control_(r_err err, R_name name, x_ptrlist ptrlist,
                      R_params params) noexcept
      : thread_{err, name, x_cast(ptrlist), params} {
    }
    virtual t_id get_id() const noexcept override final {
      return thread_.get_id();
    }
  private:
    t_thread thread_;
  };

  class t_main_control_ final : public t_thread_of_control_ {
  public:
    using R_name    = R_thread_name;
    using R_params  = R_thread_params;
    using x_ptr     = x_logic_ptr;
    using x_ptrlist = x_logic_ptrlist;

    t_main_control_(r_err err, R_name name, x_ptr ptr,
                    R_params params) noexcept
      : main_{err, name, x_cast(ptr), params} {
    }
    t_main_control_(r_err err, R_name name, x_ptrlist ptrlist,
                    R_params params) noexcept
      : main_{err, name, x_cast(ptrlist), params} {
    }
    virtual t_id get_id() const noexcept override final {
      return main_.get_id();
    }
  private:
    t_main main_;
  };

///////////////////////////////////////////////////////////////////////////////

  t_sandbox::t_sandbox(t_err err, R_name name, x_ptr ptr, t_control control,
                       R_params params) noexcept {
    ERR_GUARD(err) {
      p_thread_of_control_ p = nullptr;
      switch (control) {
        case IN_CURRENT_THREAD:
          p = new t_main_control_  {err, name, x_cast(ptr), params};
          break;
        case IN_NEW_THREAD:
          p = new t_thread_control_{err, name, x_cast(ptr), params};
          break;
      }
      thread_of_control_ = p;
    }
  }

  t_sandbox::t_sandbox(t_err err, R_name name, x_ptrlist ptrlist,
                       t_control control, R_params params) noexcept {
    ERR_GUARD(err) {
      p_thread_of_control_ p = nullptr;
      switch (control) {
        case IN_CURRENT_THREAD:
          p = new t_main_control_  {err, name, x_cast(ptrlist), params};
          break;
        case IN_NEW_THREAD:
          p = new t_thread_control_{err, name, x_cast(ptrlist), params};
          break;
      }
      thread_of_control_ = p;
    }
  }

  t_sandbox::~t_sandbox() {
    if (thread_of_control_)
      delete thread_of_control_;
  }

  t_id t_sandbox::get_id() const noexcept {
    return thread_of_control_ ? thread_of_control_->get_id() : BAD_ID;
  }

  t_sandbox::operator t_validity() const noexcept {
    return get_id() != BAD_ID ? VALID : INVALID;
  }

///////////////////////////////////////////////////////////////////////////////

  t_void wait_services() noexcept {
    // make sure messaging and tracing is up and running
  }

///////////////////////////////////////////////////////////////////////////////

  t_void request_death(t_id id) noexcept {
    if (id != BAD_ID)
      os::call_close(id);
    else {
      t_out{"request death of a sandbox has failed"};
    }
  }

///////////////////////////////////////////////////////////////////////////////
}
}
