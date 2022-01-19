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

#ifndef _DAINTY_TEST_H_
#define _DAINTY_TEST_H_

#include "dainty_util.h"
#include "dainty_valuestore.h"
#include "dainty_test_types.h"
#include "dainty_test_impl_.h"

namespace dainty
{
namespace test
{
  /////////////////////////////////////////////////////////////////////////////

  template<typename T = t_void>
  class t_class {
    using r_class_ = types::r_of<t_class>;
    using R_class_ = types::R_of<t_class>;
    using x_class_ = types::x_of<t_class>;

  public:
    t_class();
    t_class(R_class_);
    t_class(x_class_);
   ~t_class();

    r_class_ operator=(R_class_);
    r_class_ operator=(x_class_);

    static t_void set_monitor(t_class_info&);

    T&       set();
    const T& Get() const;

  private:
    valuestore::t_valuestore<T> value_;
    static t_class_info* instance_;
  };

  ///////////////////////////////////////////////////////////////////////////

  template<>
  class t_class<t_void> {
    using r_class_ = types::r_of<t_class>;
    using R_class_ = types::R_of<t_class>;
    using x_class_ = types::x_of<t_class>;

  public:
    t_class();
    t_class(R_class_);
    t_class(x_class_);
   ~t_class();

    r_class_ operator=(R_class_);
    r_class_ operator=(x_class_);

    static t_void set_monitor(t_class_info&);

  private:
    static t_class_info* instance_;
  };

  ///////////////////////////////////////////////////////////////////////////

  template<typename T>       T& set(T&);
  template<typename T>       T& set(t_class<T>&);
  template<typename T> const T& Get(const T&);
  template<typename T> const T& Get(const t_class<T>&);

  ///////////////////////////////////////////////////////////////////////////
}
}

#include "impl_/dainty_test_.icpp"

#endif
