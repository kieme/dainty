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

#ifndef _DAINTY_BASE_BUF_5_H_
#define _DAINTY_BASE_BUF_5_H_

#include "dainty_base_buf_1.h"

namespace dainty
{
namespace base
{
namespace buf
{
////////////////////////////////////////////////////////////////////////////////

  template<typename T, t_n_ N, typename A>
  class t_buf {
  public:
    using t_alloc      = A;
    using t_value      = typename impl_::t_prefix<T>::t_;
    using r_value      = typename impl_::t_prefix<T>::r_;
    using R_value      = typename impl_::t_prefix<T>::R_;
    using p_value      = typename impl_::t_prefix<T>::p_;
    using P_value      = typename impl_::t_prefix<T>::P_;
    using x_value      = typename impl_::t_prefix<T>::x_;
    using t_buf_range  = impl_::t_buf_range<t_value>;
    using t_buf_crange = impl_::t_buf_crange<t_value>;

    t_buf()    noexcept;
    t_buf(t_n) noexcept;
    template<typename F>
    t_buf(t_n, t_emplace_it, F&&) noexcept;
   ~t_buf();

    t_buf(const t_buf&)            = delete;
    t_buf& operator=(const t_buf&) = delete;

    t_bool   use_heap    () const noexcept;
    t_n      get_capacity() const noexcept;
    t_void   clear       ()       noexcept;

    operator t_validity  () const noexcept;
    operator t_buf_range ()       noexcept;
    operator t_buf_crange() const noexcept;

    p_value construct(t_ix)          noexcept;
    p_value construct(t_ix, R_value) noexcept;
    p_value construct(t_ix, x_value) noexcept;
    template<typename... Args>
    p_value construct(t_emplace_it, t_ix, Args&&...) noexcept;
    t_void  destruct (t_ix) noexcept;

    p_value get_ptr (t_ix)       noexcept;
    P_value get_ptr (t_ix) const noexcept;
    P_value get_cptr(t_ix) const noexcept;

    r_value get_ref (t_ix)       noexcept;
    R_value get_ref (t_ix) const noexcept;
    R_value get_cref(t_ix) const noexcept;

    r_value operator[](t_ix)       noexcept;
    R_value operator[](t_ix) const noexcept;

    p_value  begin()       noexcept;
    P_value  begin() const noexcept;
    P_value cbegin() const noexcept;

    p_value  end  ()       noexcept;
    P_value  end  () const noexcept;
    P_value cend  () const noexcept;

    t_buf_range  mk_range()                     noexcept;
    t_buf_range  mk_range(t_begin_ix)           noexcept;
    t_buf_range  mk_range(t_begin_ix, t_end_ix) noexcept;
    t_buf_range  mk_range(t_n)                  noexcept;
    t_buf_range  mk_range(t_begin_ix, t_n)      noexcept;

    t_buf_crange mk_range()                      const noexcept;
    t_buf_crange mk_range(t_begin_ix)            const noexcept;
    t_buf_crange mk_range(t_begin_ix, t_end_ix)  const noexcept;
    t_buf_crange mk_range(t_n)                   const noexcept;
    t_buf_crange mk_range(t_begin_ix, t_n)       const noexcept;

    t_buf_crange mk_crange()                     const noexcept;
    t_buf_crange mk_crange(t_begin_ix)           const noexcept;
    t_buf_crange mk_crange(t_begin_ix, t_end_ix) const noexcept;
    t_buf_crange mk_crange(t_n)                  const noexcept;
    t_buf_crange mk_crange(t_begin_ix, t_n)      const noexcept;

    t_void      enlarge_by(t_n) noexcept;
    t_void      resize_to (t_n) noexcept;
    t_buf_range release   ()    noexcept; // allow release

  private:
    using t_valuestore = impl_::t_valuestore<t_value>;
    using t_ptr_       = impl_::t_ptr_<t_value>;

    t_bool is_valid_  (t_ix) const noexcept;
    t_ix   get_end_ix_()     const noexcept;

    t_valuestore store_[N];
    t_n          max_;
    t_ptr_       ptr_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename T, t_n_ N, typename A>
  inline
  t_buf<T, N, A>::t_buf() noexcept : max_{N}, ptr_{store_} {
  }

  template<typename T, t_n_ N, typename A>
  inline
  t_buf<T, N, A>::t_buf(t_n max) noexcept
      : max_{N}, ptr_{store_} {
    if (t_n{N} < max) {
      max_ = max;
      ptr_ = t_alloc::template alloc_<t_valuestore>(max);
    }
  }

  template<typename T, t_n_ N, typename A>
  template<typename F>
  inline
  t_buf<T, N, A>::t_buf(t_n max, t_emplace_it, F&& func) noexcept
      : max_{N}, ptr_{store_} {
    if (N < max) {
      max_ = max;
      ptr_ = t_alloc::template alloc_<t_valuestore>(max);
    }
    func(max, ptr_.get()); // XXX
  }

  template<typename T, t_n_ N, typename A>
  inline
  t_buf<T, N, A>::~t_buf() {
    if (use_heap())
      t_alloc::dealloc_(ptr_.release());
  }

  template<typename T, t_n_ N, typename A>
  inline
  t_bool t_buf<T, N, A>::is_valid_(t_ix ix) const noexcept {
    return get(ix) < get(max_);
  }

  template<typename T, t_n_ N, typename A>
  inline
  t_ix t_buf<T, N, A>::get_end_ix_() const noexcept {
    return t_ix{get(max_)};
  }

  template<typename T, t_n_ N, typename A>
  inline
  t_bool t_buf<T, N, A>::use_heap() const noexcept {
    return ptr_.get() != store_;
  }

  template<typename T, t_n_ N, typename A>
  inline
  t_void t_buf<T, N, A>::clear() noexcept {
    if (use_heap()) {
      t_alloc::dealloc_(ptr_.release());
      max_ = t_n{N};
      ptr_ = store_;
    }
  }

  template<typename T, t_n_ N, typename A>
  inline
  t_n t_buf<T, N, A>::get_capacity() const noexcept {
    return max_;
  }

  template<typename T, t_n_ N, typename A>
  inline
  t_buf<T, N, A>::operator t_validity() const noexcept {
    return ptr_;
  }

  template<typename T, t_n_ N, typename A>
  inline
  t_buf<T, N, A>::operator t_buf_range() noexcept {
    return t_buf_range{begin(), max_};
  }

  template<typename T, t_n_ N, typename A>
  inline
  t_buf<T, N, A>::operator t_buf_crange() const noexcept {
    return t_buf_crange{cbegin(), max_};
  }

  template<typename T, t_n_ N, typename A>
  inline
  typename t_buf<T, N, A>::p_value
      t_buf<T, N, A>::construct(t_ix ix) noexcept {
    if (is_valid_(ix))
      return ptr_[ix].default_construct();
    return nullptr;
  }

  template<typename T, t_n_ N, typename A>
  inline
  typename t_buf<T, N, A>::p_value
      t_buf<T, N, A>
        ::construct(t_ix ix, R_value value) noexcept {
    if (is_valid_(ix))
      return ptr_[ix].copy_construct(value);
    return nullptr;
  }

  template<typename T, t_n_ N, typename A>
  inline
  typename t_buf<T, N, A>::p_value
      t_buf<T, N, A>
        ::construct(t_ix ix, x_value value) noexcept {
    if (is_valid_(ix))
      return ptr_[ix].move_construct(util::x_cast(value));
    return nullptr;
  }

  template<typename T, t_n_ N, typename A>
  template<typename... Args>
  inline
  typename t_buf<T, N, A>::p_value
      t_buf<T, N, A>
        ::construct(t_emplace_it, t_ix ix, Args&&... args) noexcept {
    if (is_valid_(ix))
      return ptr_[ix].emplace_construct(util::preserve<Args>(args)...);
    return nullptr;
  }

  template<typename T, t_n_ N, typename A>
  inline
  t_void t_buf<T, N, A>::destruct(t_ix ix) noexcept {
    if (is_valid_(ix))
      ptr_[ix].destruct();
  }

  template<typename T, t_n_ N, typename A>
  inline
  t_void t_buf<T, N, A>::enlarge_by(t_n by) noexcept {
    resize_to(max_ + by);
  }

  template<typename T, t_n_ N, typename A>
  inline
  t_void t_buf<T, N, A>::resize_to(t_n n) noexcept {
    if (n <= t_n{N}) {
      if (use_heap()) {
        t_alloc::dealloc_(ptr_.release());
        ptr_ = store_;
        max_ = t_n{N};
      }
    } else {
      if (use_heap())
        ptr_ = t_alloc::template realloc_<t_valuestore>(ptr_.release(), n);
      else
        ptr_ = t_alloc::template alloc_<t_valuestore>(n);
      max_ = n;
    }
  }

  template<typename T, t_n_ N, typename A>
  inline
  typename t_buf<T, N, A>::p_value
      t_buf<T, N, A>::begin() noexcept {
    return ptr_[0_ix].ptr();
  }

  template<typename T, t_n_ N, typename A>
  inline
  typename t_buf<T, N, A>::P_value
      t_buf<T, N, A>::begin() const noexcept {
    return ptr_[0_ix].cptr();
  }

  template<typename T, t_n_ N, typename A>
  inline
  typename t_buf<T, N, A>::P_value
      t_buf<T, N, A>::cbegin() const noexcept {
    return ptr_[0_ix].cptr();
  }

  template<typename T, t_n_ N, typename A>
  inline
  typename t_buf<T, N, A>::p_value
      t_buf<T, N, A>::end() noexcept {
    return ptr_[get_end_ix_()].ptr();
  }

  template<typename T, t_n_ N, typename A>
  inline
  typename t_buf<T, N, A>::P_value
      t_buf<T, N, A>::end() const noexcept {
    return ptr_[get_end_ix_()].cptr();
  }

  template<typename T, t_n_ N, typename A>
  inline
  typename t_buf<T, N, A>::P_value
      t_buf<T, N, A>::cend() const noexcept {
    return ptr_[get_end_ix_()].cptr();
  }

  template<typename T, t_n_ N, typename A>
  inline
  typename t_buf<T, N, A>::p_value
      t_buf<T, N, A>::get_ptr(t_ix ix) noexcept {
    if (is_valid_(ix))
      return ptr_[ix].ptr();
    return nullptr;
  }

  template<typename T, t_n_ N, typename A>
  inline
  typename t_buf<T, N, A>::P_value
      t_buf<T, N, A>::get_ptr(t_ix ix) const noexcept {
    if (is_valid_(ix))
      return ptr_[ix].cptr();
    return nullptr;
  }

  template<typename T, t_n_ N, typename A>
  inline
  typename t_buf<T, N, A>::P_value
      t_buf<T, N, A>::get_cptr(t_ix ix) const noexcept {
    if (is_valid_(ix))
      return ptr_[ix].cptr();
    return nullptr;
  }

  template<typename T, t_n_ N, typename A>
  inline
  typename t_buf<T, N, A>::r_value
      t_buf<T, N, A>::get_ref(t_ix ix) noexcept {
    return ptr_[ix].ref();
  }

  template<typename T, t_n_ N, typename A>
  inline
  typename t_buf<T, N, A>::R_value
      t_buf<T, N, A>::get_ref(t_ix ix) const noexcept {
    return ptr_[ix].ref();
  }

  template<typename T, t_n_ N, typename A>
  inline
  typename t_buf<T, N, A>::R_value
      t_buf<T, N, A>::get_cref(t_ix ix) const noexcept {
    return ptr_[ix].cref();
  }

  template<typename T, t_n_ N, typename A>
  inline
  typename t_buf<T, N, A>::r_value
      t_buf<T, N, A>::operator[](t_ix ix) noexcept {
    return get_ref(ix);
  }

  template<typename T, t_n_ N, typename A>
  inline
  typename t_buf<T, N, A>::R_value
      t_buf<T, N, A>::operator[](t_ix ix) const noexcept {
    return get_cref(ix);
  }

  template<typename T, t_n_ N, typename A>
  inline
  typename t_buf<T, N, A>::t_buf_range
      t_buf<T, N, A>::mk_range() noexcept {
    return static_cast<t_buf_range>(*this);
  }

  template<typename T, t_n_ N, typename A>
  inline
  typename t_buf<T, N, A>::t_buf_range
      t_buf<T, N, A>::mk_range(t_begin_ix begin) noexcept {
    return impl_::mk_range(static_cast<t_buf_range>(*this), begin);
  }

  template<typename T, t_n_ N, typename A>
  inline
  typename t_buf<T, N, A>::t_buf_range
      t_buf<T, N, A>::mk_range(t_begin_ix begin, t_end_ix end) noexcept {
    return impl_::mk_range(static_cast<t_buf_range>(*this), begin, end);
  }

  template<typename T, t_n_ N, typename A>
  inline
  typename t_buf<T, N, A>::t_buf_range
      t_buf<T, N, A>::mk_range(t_n n) noexcept {
    return impl_::mk_range(static_cast<t_buf_range>(*this), n);
  }

  template<typename T, t_n_ N, typename A>
  inline
  typename t_buf<T, N, A>::t_buf_range
      t_buf<T, N, A>::mk_range(t_begin_ix begin, t_n n) noexcept {
    return impl_::mk_range(static_cast<t_buf_range>(*this), begin, n);
  }

  template<typename T, t_n_ N, typename A>
  inline
  typename t_buf<T, N, A>::t_buf_crange
      t_buf<T, N, A>::mk_range() const noexcept {
    return static_cast<t_buf_crange>(*this);
  }

  template<typename T, t_n_ N, typename A>
  inline
  typename t_buf<T, N, A>::t_buf_crange
      t_buf<T, N, A>::mk_range(t_begin_ix begin) const noexcept {
    return impl_::mk_range(static_cast<t_buf_crange>(*this), begin);
  }

  template<typename T, t_n_ N, typename A>
  inline
  typename t_buf<T, N, A>::t_buf_crange
      t_buf<T, N, A>::mk_range(t_begin_ix begin, t_end_ix end) const noexcept {
    return impl_::mk_range(static_cast<t_buf_crange>(*this), begin, end);
  }

  template<typename T, t_n_ N, typename A>
  inline
  typename t_buf<T, N, A>::t_buf_crange
      t_buf<T, N, A>::mk_range(t_n n) const noexcept {
    return impl_::mk_range(static_cast<t_buf_crange>(*this), n);
  }

  template<typename T, t_n_ N, typename A>
  inline
  typename t_buf<T, N, A>::t_buf_crange
      t_buf<T, N, A>::mk_range(t_begin_ix begin, t_n n) const noexcept {
    return impl_::mk_range(static_cast<t_buf_crange>(*this), begin, n);
  }

  template<typename T, t_n_ N, typename A>
  inline
  typename t_buf<T, N, A>::t_buf_crange
      t_buf<T, N, A>::mk_crange() const noexcept {
    return static_cast<t_buf_crange>(*this);
  }

  template<typename T, t_n_ N, typename A>
  inline
  typename t_buf<T, N, A>::t_buf_crange
      t_buf<T, N, A>::mk_crange(t_begin_ix begin) const noexcept {
    return impl_::mk_range(static_cast<t_buf_crange>(*this), begin);
  }

  template<typename T, t_n_ N, typename A>
  inline
  typename t_buf<T, N, A>::t_buf_crange
      t_buf<T, N, A>::mk_crange(t_begin_ix begin, t_end_ix end) const noexcept {
    return impl_::mk_range(static_cast<t_buf_crange>(*this), begin, end);
  }

  template<typename T, t_n_ N, typename A>
  inline
  typename t_buf<T, N, A>::t_buf_crange
      t_buf<T, N, A>::mk_crange(t_n n) const noexcept {
    return impl_::mk_range(static_cast<t_buf_crange>(*this), n);
  }

  template<typename T, t_n_ N, typename A>
  inline
  typename t_buf<T, N, A>::t_buf_crange
      t_buf<T, N, A>::mk_crange(t_begin_ix begin, t_n n) const noexcept {
    return impl_::mk_range(static_cast<t_buf_crange>(*this), begin, n);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
