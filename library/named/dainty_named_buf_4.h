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

#ifndef _DAINTY_NAMED_BUF_4_H_
#define _DAINTY_NAMED_BUF_4_H_

#include "dainty_named_range.h"
#include "dainty_named_buf_impl.h"

namespace dainty
{
namespace named
{
namespace buf
{
///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  class t_buf<T, 1, t_size_static> {
  public:
    using t_value = typename t_prefix<T>::t_;
    using r_value = typename t_prefix<T>::r_;
    using R_value = typename t_prefix<T>::R_;
    using p_value = typename t_prefix<T>::p_;
    using P_value = typename t_prefix<T>::P_;
    using x_value = typename t_prefix<T>::x_;

    t_buf() noexcept = default;
    template<typename F>
    t_buf(t_emplace_it, F&&) noexcept;

    t_buf(const t_buf&)            = delete;
    t_buf& operator=(const t_buf&) = delete;

    t_bool   use_heap    () const noexcept;
    operator t_validity  () const noexcept;
    t_n      get_capacity() const noexcept;

    p_value construct()          noexcept;
    p_value construct(R_value)   noexcept;
    p_value construct(x_value)   noexcept;
    template<typename... Args>
    p_value construct(t_emplace_it, Args&&...) noexcept;
    t_void  destruct () noexcept;

    p_value get_ptr ()       noexcept;
    P_value get_ptr () const noexcept;
    P_value get_cptr() const noexcept;

    r_value get_ref ()       noexcept;
    R_value get_ref () const noexcept;
    R_value get_cref() const noexcept;

    template<typename TAG> t_range <T, TAG> mk_range ()       noexcept;
    template<typename TAG> t_crange<T, TAG> mk_range () const noexcept;
    template<typename TAG> t_crange<T, TAG> mk_crange() const noexcept;

  private:
    t_valuestore<T> store_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  template<typename F>
  inline
  t_buf<T, 1, t_size_static>::t_buf(t_emplace_it, F&& func) noexcept {
    func(t_n{1}, store_.ptr());
  }

  template<typename T>
  inline
  t_bool t_buf<T, 1, t_size_static>::use_heap() const noexcept {
    return false;
  }

  template<typename T>
  inline
  t_buf<T, 1, t_size_static>::operator t_validity() const noexcept {
    return VALID;;
  }

  template<typename T>
  inline
  t_n t_buf<T, 1, t_size_static>::get_capacity() const noexcept {
    return t_n{1};
  }

  template<typename T>
  inline
  typename t_buf<T, 1, t_size_static>::p_value
      t_buf<T, 1, t_size_static>::construct() noexcept {
    return store_.default_construct();
  }

  template<typename T>
  inline
  typename t_buf<T, 1, t_size_static>::p_value
      t_buf<T, 1, t_size_static>::construct(R_value value) noexcept {
    return store_.copy_construct(value);
  }

  template<typename T>
  inline
  typename t_buf<T, 1, t_size_static>::p_value
      t_buf<T, 1, t_size_static>::construct(x_value value) noexcept {
    return store_.move_construct(named::x_cast(value));
  }

  template<typename T>
  template<typename... Args>
  inline
  typename t_buf<T, 1, t_size_static>::p_value
      t_buf<T, 1, t_size_static>::construct(t_emplace_it, Args&&... args) noexcept {
    return store_.emplace_construct(named::preserve<Args>(args)...);
  }

  template<typename T>
  inline
  t_void t_buf<T, 1, t_size_static>::destruct() noexcept {
    store_.destruct();
  }

  template<typename T>
  inline
  typename t_buf<T, 1, t_size_static>::p_value
      t_buf<T, 1, t_size_static>::get_ptr() noexcept {
    return store_.ptr();
  }

  template<typename T>
  inline
  typename t_buf<T, 1, t_size_static>::P_value
      t_buf<T, 1, t_size_static>::get_ptr() const noexcept {
    return store_.ptr();
  }

  template<typename T>
  inline
  typename t_buf<T, 1, t_size_static>::P_value
      t_buf<T, 1, t_size_static>::get_cptr() const noexcept {
    return store_.cptr();
  }

  template<typename T>
  inline
  typename t_buf<T, 1, t_size_static>::r_value
      t_buf<T, 1, t_size_static>::get_ref() noexcept {
    return store_.ref();
  }

  template<typename T>
  inline
  typename t_buf<T, 1, t_size_static>::R_value
      t_buf<T, 1, t_size_static>::get_ref() const noexcept {
    return store_.ref();
  }

  template<typename T>
  inline
  typename t_buf<T, 1, t_size_static>::R_value
      t_buf<T, 1, t_size_static>::get_cref() const noexcept {
    return store_.cref();
  }

  template<typename T>
  template<typename TAG>
  t_range <T, TAG> t_buf<T, 1, t_size_static>::mk_range() noexcept {
    return {store_.ptr(), t_n{1}};
  }

  template<typename T>
  template<typename TAG>
  inline
  t_crange<T, TAG> t_buf<T, 1, t_size_static>::mk_range() const noexcept {
    return {store_.ptr(), t_n{1}};
  }

  template<typename T>
  template<typename TAG>
  inline
  t_crange<T, TAG> t_buf<T, 1, t_size_static>::mk_crange() const noexcept {
    return {store_.cptr(), t_n{1}};
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
