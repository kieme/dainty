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

#ifndef _DAINTY_BASE_BUF_1_H_
#define _DAINTY_BASE_BUF_1_H_

#include "dainty_base_buf_impl.h"

namespace dainty
{
namespace base
{
namespace buf
{
////////////////////////////////////////////////////////////////////////////////

  template<typename T>
  class t_buf<T, 0, t_size_dynamic> {
  public:
    using t_value = typename t_prefix<T>::t_;
    using r_value = typename t_prefix<T>::r_;
    using R_value = typename t_prefix<T>::R_;
    using p_value = typename t_prefix<T>::p_;
    using P_value = typename t_prefix<T>::P_;
    using x_value = typename t_prefix<T>::x_;

    t_buf(t_n) noexcept;
    template<typename F>
    t_buf(t_n, t_emplace_it, F&&) noexcept;

    t_buf(const t_buf&)            = delete;
    t_buf& operator=(const t_buf&) = delete;

    t_bool   use_heap    () const noexcept;
    operator t_validity  () const noexcept;
    t_n      get_capacity() const noexcept;

    p_value construct(t_ix)          noexcept;
    p_value construct(t_ix, R_value) noexcept;
    p_value construct(t_ix, x_value) noexcept;
    template<typename... Args>
    p_value construct(t_emplace_it, t_ix, Args&&...) noexcept;
    t_void  destruct (t_ix) noexcept;

    p_value get_ptr ()       noexcept;
    P_value get_ptr () const noexcept;
    P_value get_cptr() const noexcept;

    p_value get_ptr (t_ix)       noexcept;
    P_value get_ptr (t_ix) const noexcept;
    P_value get_cptr(t_ix) const noexcept;

    r_value get_ref (t_ix)       noexcept;
    R_value get_ref (t_ix) const noexcept;
    R_value get_cref(t_ix) const noexcept;

    r_value operator[](t_ix)       noexcept;
    R_value operator[](t_ix) const noexcept;

    t_n realloc_shrink (t_n by) noexcept;
    t_n realloc_enlarge(t_n by) noexcept;

    template<typename F> t_n shrink (t_n by, F&&) noexcept;
    template<typename F> t_n enlarge(t_n by, F&&) noexcept;

    template<typename TAG> t_range <T, TAG> mk_range ()       noexcept;
    template<typename TAG> t_crange<T, TAG> mk_range () const noexcept;
    template<typename TAG> t_crange<T, TAG> mk_crange() const noexcept;

    template<typename TAG> t_range <T, TAG> mk_range (t_ix)       noexcept;
    template<typename TAG> t_crange<T, TAG> mk_range (t_ix) const noexcept;
    template<typename TAG> t_crange<T, TAG> mk_crange(t_ix) const noexcept;

    template<typename TAG> t_range <T, TAG> mk_range (t_ix, t_ix)       noexcept;
    template<typename TAG> t_crange<T, TAG> mk_range (t_ix, t_ix) const noexcept;
    template<typename TAG> t_crange<T, TAG> mk_crange(t_ix, t_ix) const noexcept;

  private:
    using t_ptr_ = base::ptr::t_ptr<t_valuestore<T>[], t_buf,
	                             base::ptr::t_deleter>;
    t_n_   max_;
    t_ptr_ ptr_;
  };

////////////////////////////////////////////////////////////////////////////////

  template<typename T>
  inline
  t_buf<T, 0, t_size_dynamic>::t_buf(t_n max) noexcept
      : max_{get(max)}, ptr_{new t_valuestore<t_value>[max_]} {
  }

  template<typename T>
  template<typename F>
  inline
  t_buf<T, 0, t_size_dynamic>::t_buf(t_n max, t_emplace_it,
                                              F&& func) noexcept
      : max_{get(max)}, ptr_{new t_valuestore<t_value>[max_]} {
    func(max, ptr_[t_ix{0}].ptr());
  }

  template<typename T>
  inline
  t_bool t_buf<T, 0, t_size_dynamic>::use_heap() const noexcept {
    return true;
  }

  template<typename T>
  inline
  t_buf<T, 0, t_size_dynamic>::operator t_validity () const noexcept {
    return ptr_;
  }

  template<typename T>
  inline
  t_n t_buf<T, 0, t_size_dynamic>::get_capacity() const noexcept {
    return t_n{max_};
  }

  template<typename T>
  inline
  typename t_buf<T, 0, t_size_dynamic>::p_value
      t_buf<T, 0, t_size_dynamic>::construct(t_ix ix) noexcept {
    return get(ix) < max_ ? ptr_[ix].default_construct() : nullptr;
  }

  template<typename T>
  inline
  typename t_buf<T, 0, t_size_dynamic>::p_value
      t_buf<T, 0, t_size_dynamic>
        ::construct(t_ix ix, R_value value) noexcept {
    return get(ix) < max_ ? ptr_[ix].copy_construct(value) : nullptr;
  }

  template<typename T>
  inline
  typename t_buf<T, 0, t_size_dynamic>::p_value
      t_buf<T, 0, t_size_dynamic>
        ::construct(t_ix ix, x_value value) noexcept {
    return get(ix) < max_ ?
      ptr_[ix].move_construct(base::x_cast(value)) : nullptr;
  }

  template<typename T>
  template<typename... Args>
  inline
  typename t_buf<T, 0, t_size_dynamic>::p_value
      t_buf<T, 0, t_size_dynamic>
        ::construct(t_emplace_it, t_ix ix, Args&&... args) noexcept {
    return get(ix) < max_ ?
      ptr_[ix].emplace_construct(base::preserve<Args>(args)...) : nullptr;
  }

  template<typename T>
  inline
  t_void t_buf<T, 0, t_size_dynamic>::destruct(t_ix ix) noexcept {
    if (get(ix) < max_)
      ptr_[ix].destruct();
  }

  template<typename T>
  inline
  t_n t_buf<T, 0, t_size_dynamic>::realloc_shrink(t_n by) noexcept {
    //XXX - 1
    return t_n{0};
  }

  template<typename T>
  inline
  t_n t_buf<T, 0, t_size_dynamic>::realloc_enlarge(t_n by) noexcept {
    //XXX - 2
    return t_n{0};
  }

  template<typename T>
  template<typename F>
  inline
  t_n t_buf<T, 0, t_size_dynamic>::shrink(t_n by, F&& func) noexcept {
    //XXX - 3
    return t_n{0};
  }

  template<typename T>
  template<typename F>
  inline
  t_n t_buf<T, 0, t_size_dynamic>::enlarge(t_n by, F&& func) noexcept {
    //XXX - 4
    return t_n{0};
  }

  template<typename T>
  inline
  typename t_buf<T, 0, t_size_dynamic>::p_value
      t_buf<T, 0, t_size_dynamic>::get_ptr() noexcept {
    return ptr_[0].ptr();
  }

  template<typename T>
  inline
  typename t_buf<T, 0, t_size_dynamic>::P_value
      t_buf<T, 0, t_size_dynamic>::get_ptr() const noexcept {
    return ptr_[0].cptr();
  }

  template<typename T>
  inline
  typename t_buf<T, 0, t_size_dynamic>::P_value
      t_buf<T, 0, t_size_dynamic>::get_cptr() const noexcept {
    return ptr_[0].cptr();
  }

  template<typename T>
  inline
  typename t_buf<T, 0, t_size_dynamic>::p_value
      t_buf<T, 0, t_size_dynamic>::get_ptr(t_ix ix) noexcept {
    return get(ix) < max_ ? ptr_[ix].ptr() : nullptr;
  }

  template<typename T>
  inline
  typename t_buf<T, 0, t_size_dynamic>::P_value
      t_buf<T, 0, t_size_dynamic>::get_ptr(t_ix ix) const noexcept {
    return get(ix) < max_ ? ptr_[ix].ptr() : nullptr;
  }

  template<typename T>
  inline
  typename t_buf<T, 0, t_size_dynamic>::P_value
      t_buf<T, 0, t_size_dynamic>::get_cptr(t_ix ix) const noexcept {
    return get(ix) < max_ ? ptr_[ix].cptr() : nullptr;
  }

  template<typename T>
  inline
  typename t_buf<T, 0, t_size_dynamic>::r_value
      t_buf<T, 0, t_size_dynamic>::get_ref(t_ix ix) noexcept {
    return ptr_[ix].ref();
  }

  template<typename T>
  inline
  typename t_buf<T, 0, t_size_dynamic>::R_value
      t_buf<T, 0, t_size_dynamic>::get_ref(t_ix ix) const noexcept {
    return ptr_[ix].ref();
  }

  template<typename T>
  inline
  typename t_buf<T, 0, t_size_dynamic>::R_value
      t_buf<T, 0, t_size_dynamic>::get_cref(t_ix ix) const noexcept {
    return ptr_[ix].cref();
  }

  template<typename T>
  inline
  typename t_buf<T, 0, t_size_dynamic>::r_value
      t_buf<T, 0, t_size_dynamic>::operator[](t_ix ix) noexcept {
    return get_ref(ix);
  }

  template<typename T>
  inline
  typename t_buf<T, 0, t_size_dynamic>::R_value
      t_buf<T, 0, t_size_dynamic>::operator[](t_ix ix) const noexcept {
    return get_cref(ix);
  }

  template<typename T>
  template<typename TAG>
  inline
  t_range<T, TAG> t_buf<T, 0, t_size_dynamic>::mk_range() noexcept {
    return {ptr_[0].ptr(), t_n{max_}};
  }

  template<typename T>
  template<typename TAG>
  inline
  t_crange<T, TAG> t_buf<T, 0, t_size_dynamic>::mk_range() const noexcept {
    return {ptr_[0].ptr(), t_n{max_}};
  }

  template<typename T>
  template<typename TAG>
  inline
  t_crange<T, TAG> t_buf<T, 0, t_size_dynamic>
      ::mk_crange() const noexcept {
    return {ptr_[0].cptr(), t_n{max_}};
  }

  template<typename T>
  template<typename TAG>
  inline
  t_range<T, TAG> t_buf<T, 0, t_size_dynamic>
      ::mk_range(t_ix ix) noexcept {
    return buf::mk_range(mk_range(), ix);
  }

  template<typename T>
  template<typename TAG>
  inline
  t_crange<T, TAG> t_buf<T, 0, t_size_dynamic>
      ::mk_range(t_ix ix) const noexcept {
    return buf::mk_range(mk_range(), ix);
  }

  template<typename T>
  template<typename TAG>
  inline
  t_crange<T, TAG> t_buf<T, 0, t_size_dynamic>
      ::mk_crange(t_ix ix) const noexcept {
    return buf::mk_crange(mk_crange(), ix);
  }

  template<typename T>
  template<typename TAG>
  inline
  t_range<T, TAG> t_buf<T, 0, t_size_dynamic>
      ::mk_range (t_ix begin, t_ix end) noexcept {
    return buf::mk_range(mk_range(), begin, end);
  }

  template<typename T>
  template<typename TAG>
  inline
  t_crange<T, TAG> t_buf<T, 0, t_size_dynamic>
      ::mk_range (t_ix begin, t_ix end) const noexcept {
    return buf::mk_range(mk_range(), begin, end);
  }

  template<typename T>
  template<typename TAG>
  inline
  t_crange<T, TAG> t_buf<T, 0, t_size_dynamic>
      ::mk_crange(t_ix begin, t_ix end) const noexcept {
    return buf::mk_crange(mk_crange(), begin, end);
  }

////////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
