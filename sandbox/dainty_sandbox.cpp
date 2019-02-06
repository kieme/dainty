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
#include "dainty_sandbox_impl.h"

///////////////////////////////////////////////////////////////////////////////

namespace dainty
{
namespace sandbox
{
  using t_thread_attr_ = os::t_pthread_attr;
  using named::t_ix;

  using named::utility::x_cast;
  using os::call_pthread_init;

///////////////////////////////////////////////////////////////////////////////

  t_void register_(t_err err, r_logic logic, p_logic_ext_ ext) {
    ERR_GUARD(err) {
      if (ext) {
        t_ix end = to_ix(logic.extlist_.get_size());
        for (t_ix ix{0}; ix < end; set(ix)++) {
          p_logic_ext_ ext_ptr = logic.extlist_.get(ix);
          if (ext == ext_ptr || ext_ptr->get_name() == ext->get_name()) {
            err = err::E_XXX; // already
            break;
          }
        }
        logic.extlist_.push_back(err, ext);
      } else
        err = err::E_XXX;
    }
  }

///////////////////////////////////////////////////////////////////////////////

  // t_logic can call impl
  t_logic::t_logic(t_err err, R_messenger_name name) noexcept : name_{name} {
  }

///////////////////////////////////////////////////////////////////////////////

  t_thread::t_thread(t_err err, R_thread_name name,
                     x_logic_ptr logic_ptr) noexcept {
    ERR_GUARD(err) {
      t_thread_logic_ptr_ ptr = new t_single_impl_{err, name,
                                                   x_cast(logic_ptr)};
      t_thread_ thread{err, name.get_cstr(), x_cast(ptr)};
    }
  }

  t_thread::t_thread(t_err err, R_thread_name name,
                     x_logic_ptrlist list) noexcept {
    ERR_GUARD(err) {
      t_thread_logic_ptr_ ptr = new t_shared_impl_{err, name, x_cast(list)};
      t_thread_ thread{err, name.get_cstr(), x_cast(ptr)};
    }
  }

  t_main::t_main(t_err err, R_thread_name name, x_logic_ptr logic) noexcept {
    ERR_GUARD(err) {
      t_single_impl_ impl{err, name, x_cast(logic)};
      t_thread_attr_ attr;
      call_pthread_init(err, attr);
      impl.update(err, attr);
      impl.prepare(err);
      if (!err)
        impl.run();
    }
  }

  t_main::t_main(t_err err, R_thread_name name,
                 x_logic_ptrlist list) noexcept {
    ERR_GUARD(err) {
      t_shared_impl_ impl{err, name, x_cast(list)};
      t_thread_attr_ attr;
      call_pthread_init(err, attr);
      impl.update(err, attr);
      impl.prepare(err);
      if (!err)
        impl.run();
    }
  }

///////////////////////////////////////////////////////////////////////////////
}
}
