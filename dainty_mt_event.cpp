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
#include "dainty_os_threading.h"
#include "dainty_mt_event.h"

namespace dainty
{
namespace mt
{
namespace event
{
  using namespace dainty::os::threading;
  using namespace dainty::os::fdbased;

  using err::r_err;

///////////////////////////////////////////////////////////////////////////////

  class t_impl_ {
  public:
    using r_logic = t_processor::r_logic;

    t_impl_(r_err err) noexcept : eventfd_(err, t_n{0}) {
    }

    operator t_validity() const noexcept {
      return (eventfd_ == VALID) ?  VALID : INVALID;
    }

    t_void process(r_err err, r_logic logic, t_n max) noexcept {
      ERR_GUARD(err) {
        for (t_n_ n = get(max); !err && n; --n) {
          t_cnt cnt{0};
          eventfd_.read(err, set(cnt));
          if (!err)
            logic.async_process(cnt);
        }
      }
    }

    t_errn post(t_user, t_cnt cnt) noexcept {
      return eventfd_.write(get(cnt));
    }

    t_void post(r_err err, t_user, t_cnt cnt) noexcept {
      ERR_GUARD(err) {
        eventfd_.write(err, get(cnt));
      }
    }

    t_fd get_fd() const noexcept {
      return eventfd_.get_fd();
    }

    t_client make_client(t_user user) noexcept {
      // NOTE: future, we have information on clients.
      return {this, user};
    }

    t_client make_client(r_err, t_user user) noexcept {
      // NOTE: future, we have information on clients.
      return {this, user};
    }

  private:
    t_eventfd eventfd_;
  };

///////////////////////////////////////////////////////////////////////////////

  t_errn t_client::post(t_cnt cnt) noexcept {
    if (impl_)
      return impl_->post(user_, cnt);
    return t_errn{-1};
  }

  t_void t_client::post(t_err err, t_cnt cnt) noexcept {
    ERR_GUARD(err) {
      if (impl_ && *impl_ == VALID)
        impl_->post(err, user_, cnt);
      else
        err = err::E_XXX;
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_processor::t_processor(t_err err) noexcept {
    ERR_GUARD(err) {
      impl_ = new t_impl_(err);
      if (impl_) {
        if (err)
          delete named::reset(impl_);
      } else
        err = err::E_XXX;
    }
  }

  t_processor::~t_processor() {
    if (impl_) { // NOTE: can check to see if clients exists
      delete impl_;
    }
  }

  t_client t_processor::make_client(t_user user) noexcept {
    if (impl_)
      return impl_->make_client(user);
    return {};
  }

  t_client t_processor::make_client(t_err err, t_user user) noexcept {
    ERR_GUARD(err) {
      if (impl_ && *impl_ == VALID)
        return impl_->make_client(err, user);
      err = err::E_XXX;
    }
    return {};
  }

  t_void t_processor::process(t_err err, t_logic& logic, t_n max) noexcept {
    ERR_GUARD(err) {
      if (impl_ && *impl_ == VALID)
        impl_->process(err, logic, max);
      else
        err = err::E_XXX;
    }
  }

  t_fd t_processor::get_fd() const noexcept {
    if (impl_)
      return impl_->get_fd();
    return BAD_FD;
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}
