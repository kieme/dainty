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

  t_impl_::t_impl_(t_err err, R_thread_name name) noexcept {
    ERR_GUARD(err) {
      // create_event_loop
    }
  }

  t_void t_impl_::update(base::t_err err,
                         base::r_pthread_attr attr) noexcept {
    ERR_GUARD(err) {
    }
  }

  t_void t_impl_::prepare(base::t_err err) noexcept {
    ERR_GUARD(err) {
        // build structures
    }
  }

  t_void t_impl_::start_extensions(t_err err, p_logic logic) noexcept {
    ERR_GUARD(err) {
      t_ix end = to_ix(logic->extlist_.get_size());
      for (t_ix ix{0}; ix < end; set(ix)++) {
        logic->extlist_.get(ix)->start(err);
        if (err) {
          if (get(ix)) {
            for (set(ix)--; get(ix); set(ix)--)
              logic->extlist_.get(ix)->cleanup();
            logic->extlist_.get(ix)->cleanup();
          }
          break;
        }
      }
    }
  }

  t_void t_impl_::cleanup_extensions(p_logic logic) noexcept {
    t_ix ix = to_ix(logic->extlist_.get_size());
    if (get(ix)) {
      for (set(ix)--; get(ix); set(ix)--)
        logic->extlist_.get(ix)->cleanup();
      logic->extlist_.get(ix)->cleanup();
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_single_impl_::t_single_impl_(t_err err, R_thread_name name,
                                 x_logic_ptr ptr) noexcept
    : t_impl_{err, name}, logic_{x_cast(ptr)} {
    ERR_GUARD(err) {
      // let logic impl be set
    }
  }

  t_void t_single_impl_::run() noexcept {
    t_err err;

    start_extensions(err, logic_.get());
    logic_->start(err);

    t_out{"event_loop"};
    // event loop(logic_);

    logic_->cleanup();
    cleanup_extensions(logic_.get());
  }

///////////////////////////////////////////////////////////////////////////////

  t_shared_impl_::t_shared_impl_(t_err err, R_thread_name name,
                                 x_logic_ptrlist list) noexcept
    : t_impl_{err, name}, list_{x_cast(list)} {
    ERR_GUARD(err) {
      // let logic impl be set
    }
  }

  // api that allow extensions - might not need it
  t_void t_shared_impl_::run() noexcept {
    /*
    t_err err;

    for () {
      start_extensions(err, logic_.get());
      logic_->start(err);
    }

    t_out{"event_loop"};
    // event loop(logic_List);

    for () {
      logic_->cleanup();
      cleanup_extensions(logic_.get());
    }
    */
  }

///////////////////////////////////////////////////////////////////////////////

}
}

