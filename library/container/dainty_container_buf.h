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

#ifndef _DAINTY_CONTAINER_STORAGE_H_
#define _DAINTY_CONTAINER_STORAGE_H_

#include "dainty_named_ptr.h"
#include "dainty_named_range.h"
#include "dainty_container_valuestore.h"

// offer a buffer thats uninitialized but correctly aligned
// it could be used by nearly all my container structures.
// it takes care where the memory comes from - maybe use an allocator
//
//                              class t_buf<T, 0, t_size_dynamic>
// template<typename T, t_n_ N> class t_buf<T, N, t_size_dynamic>
// template<typename T, t_n_ N> class t_buf<T, N, t_size_static>
// template<typename T>         class t_buf<T, 1, t_size_static>

namespace dainty
{
namespace container
{
namespace buf
{
  using named::VALID;
  using named::INVALID;
  using named::t_void;
  using named::t_bool;
  using named::t_validity;
  using named::t_prefix;
  using named::t_emplace_it;
  using named::t_n;
  using named::t_n_;
  using named::t_ix;
  using named::t_ix_;
  using named::ptr::t_ptr;
  using named::range::mk_range;
  using named::range::mk_crange;
  using named::range::t_range;
  using named::range::t_crange;
  using container::valuestore::t_valuestore;

////////////////////////////////////////////////////////////////////////////////

  enum t_size_dynamic { };
  enum t_size_static  { };

  template<typename, t_n_, typename = t_size_dynamic>
  class t_buf;

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
    using t_ptr_ = named::ptr::t_ptr<t_valuestore<T>[], t_buf,
	                             named::ptr::t_deleter>;
    t_n_   max_;
    t_ptr_ ptr_;
  };

////////////////////////////////////////////////////////////////////////////////

  template<typename T, t_n_ N>
  class t_buf<T, N, t_size_dynamic> {
  public:
    using t_value      = typename t_prefix<T>::t_;
    using r_value      = typename t_prefix<T>::r_;
    using R_value      = typename t_prefix<T>::R_;
    using p_value      = typename t_prefix<T>::p_;
    using P_value      = typename t_prefix<T>::P_;
    using x_value      = typename t_prefix<T>::x_;

    t_buf()    noexcept;
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

    template<typename F> t_n shrink (t_n by, F&&) noexcept; // func(range)
    template<typename F> t_n enlarge(t_n by, F&&) noexcept; // func(range, range)

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
    using t_ptr_ = named::ptr::t_ptr<t_valuestore<T>[], t_buf,
	                             named::ptr::t_no_deleter>;
    t_n_            max_;
    t_valuestore<T> store_[N];
    t_ptr_          ptr_;
  };

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

  template<typename T, t_n_ N>
  class t_buf<T, N, t_size_static> {
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

    t_n get_capacity() const noexcept;

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
    t_valuestore<T> store_[N];
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename T> struct t_buf<T, 0, t_size_static>;

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
      ptr_[ix].move_construct(named::x_cast(value)) : nullptr;
  }

  template<typename T>
  template<typename... Args>
  inline
  typename t_buf<T, 0, t_size_dynamic>::p_value
      t_buf<T, 0, t_size_dynamic>
        ::construct(t_emplace_it, t_ix ix, Args&&... args) noexcept {
    return get(ix) < max_ ?
      ptr_[ix].emplace_construct(named::preserve<Args>(args)...) : nullptr;
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

  template<typename T, t_n_ N>
  inline
  t_buf<T, N, t_size_dynamic>::t_buf() noexcept : max_{N}, ptr_{store_} {
  }

  template<typename T, t_n_ N>
  inline
  t_buf<T, N, t_size_dynamic>::t_buf(t_n max) noexcept
      : max_{get(max)}, ptr_{new t_valuestore<t_value>[max_]} { // XXX - 6
  }

  template<typename T, t_n_ N>
  template<typename F>
  inline
  t_buf<T, N, t_size_dynamic>::t_buf(t_n max, t_emplace_it,
                                              F&& func) noexcept
      : max_{get(max)}, ptr_{new t_valuestore<t_value>[max_]} { // XXX - 7
    func(max, ptr_[t_ix{0}].ptr());
  }

  template<typename T, t_n_ N>
  inline
  t_bool t_buf<T, N, t_size_dynamic>::use_heap() const noexcept {
    return true; //XXX - 8
  }

  template<typename T, t_n_ N>
  inline
  t_buf<T, N, t_size_dynamic>::operator t_validity() const noexcept {
    return ptr_;
  }

  template<typename T, t_n_ N>
  inline
  t_n t_buf<T, N, t_size_dynamic>::get_capacity() const noexcept {
    return t_n{max_};
  }

  template<typename T, t_n_ N>
  inline
  typename t_buf<T, N, t_size_dynamic>::p_value
      t_buf<T, N, t_size_dynamic>::construct(t_ix ix) noexcept {
    return get(ix) < max_ ? ptr_[ix].default_construct() : nullptr;
  }

  template<typename T, t_n_ N>
  inline
  typename t_buf<T, N, t_size_dynamic>::p_value
      t_buf<T, N, t_size_dynamic>
        ::construct(t_ix ix, R_value value) noexcept {
    return get(ix) < max_ ? ptr_[ix].copy_construct(value) : nullptr;
  }

  template<typename T, t_n_ N>
  inline
  typename t_buf<T, N, t_size_dynamic>::p_value
      t_buf<T, N, t_size_dynamic>
        ::construct(t_ix ix, x_value value) noexcept {
    return get(ix) < max_ ?
      ptr_[ix].move_construct(named::x_cast(value)) : nullptr;
  }

  template<typename T, t_n_ N>
  template<typename... Args>
  inline
  typename t_buf<T, N, t_size_dynamic>::p_value
      t_buf<T, N, t_size_dynamic>
        ::construct(t_emplace_it, t_ix ix, Args&&... args) noexcept {
    return get(ix) < max_ ?
      ptr_[ix].emplace_construct(named::preserve<Args>(args)...) : nullptr;
  }

  template<typename T, t_n_ N>
  inline
  t_void t_buf<T, N, t_size_dynamic>::destruct(t_ix ix) noexcept {
    if (get(ix) < max_)
      ptr_[ix].destruct();
  }

  template<typename T, t_n_ N>
  inline
  t_n t_buf<T, N, t_size_dynamic>::realloc_shrink(t_n by) noexcept {
    //XXX - 9
    return t_n{0};
  }

  template<typename T, t_n_ N>
  inline
  t_n t_buf<T, N, t_size_dynamic>::realloc_enlarge(t_n by) noexcept {
    //XXX - 10
    return t_n{0};
  }

  template<typename T, t_n_ N>
  template<typename F>
  inline
  t_n t_buf<T, N, t_size_dynamic>::shrink(t_n by, F&& func) noexcept {
    //XXX - 11
    return t_n{0};
  }

  template<typename T, t_n_ N>
  template<typename F>
  inline
  t_n t_buf<T, N, t_size_dynamic>::enlarge(t_n by, F&& func) noexcept {
    //XXX - 12
    return t_n{0};
  }

  template<typename T, t_n_ N>
  inline
  typename t_buf<T, N, t_size_dynamic>::p_value
      t_buf<T, N, t_size_dynamic>::get_ptr() noexcept {
    return ptr_[0].ptr();
  }

  template<typename T, t_n_ N>
  inline
  typename t_buf<T, N, t_size_dynamic>::P_value
      t_buf<T, N, t_size_dynamic>::get_ptr() const noexcept {
    return ptr_[0].cptr();
  }

  template<typename T, t_n_ N>
  inline
  typename t_buf<T, N, t_size_dynamic>::P_value
      t_buf<T, N, t_size_dynamic>::get_cptr() const noexcept {
    return ptr_[0].cptr();
  }

  template<typename T, t_n_ N>
  inline
  typename t_buf<T, N, t_size_dynamic>::p_value
      t_buf<T, N, t_size_dynamic>::get_ptr(t_ix ix) noexcept {
    return get(ix) < max_ ? ptr_[ix].ptr() : nullptr;
  }

  template<typename T, t_n_ N>
  inline
  typename t_buf<T, N, t_size_dynamic>::P_value
      t_buf<T, N, t_size_dynamic>::get_ptr(t_ix ix) const noexcept {
    return get(ix) < max_ ? ptr_[ix].ptr() : nullptr;
  }

  template<typename T, t_n_ N>
  inline
  typename t_buf<T, N, t_size_dynamic>::P_value
      t_buf<T, N, t_size_dynamic>::get_cptr(t_ix ix) const noexcept {
    return get(ix) < max_ ? ptr_[ix].cptr() : nullptr;
  }

  template<typename T, t_n_ N>
  inline
  typename t_buf<T, N, t_size_dynamic>::r_value
      t_buf<T, N, t_size_dynamic>::get_ref(t_ix ix) noexcept {
    return ptr_[ix].ref();
  }

  template<typename T, t_n_ N>
  inline
  typename t_buf<T, N, t_size_dynamic>::R_value
      t_buf<T, N, t_size_dynamic>::get_ref(t_ix ix) const noexcept {
    return ptr_[ix].ref();
  }

  template<typename T, t_n_ N>
  inline
  typename t_buf<T, N, t_size_dynamic>::R_value
      t_buf<T, N, t_size_dynamic>::get_cref(t_ix ix) const noexcept {
    return ptr_[ix].cref();
  }

  template<typename T, t_n_ N>
  inline
  typename t_buf<T, N, t_size_dynamic>::r_value
      t_buf<T, N, t_size_dynamic>::operator[](t_ix ix) noexcept {
    return get_ref(ix);
  }

  template<typename T, t_n_ N>
  inline
  typename t_buf<T, N, t_size_dynamic>::R_value
      t_buf<T, N, t_size_dynamic>::operator[](t_ix ix) const noexcept {
    return get_cref(ix);
  }

  template<typename T, t_n_ N>
  template<typename TAG>
  inline
  t_range<T, TAG> t_buf<T, N, t_size_dynamic>::mk_range() noexcept {
    return {ptr_[0].ptr(), t_n{max_}};
  }

  template<typename T, t_n_ N>
  template<typename TAG>
  inline
  t_crange<T, TAG> t_buf<T, N, t_size_dynamic>::mk_range() const noexcept {
    return {ptr_[0].ptr(), t_n{max_}};
  }

  template<typename T, t_n_ N>
  template<typename TAG>
  inline
  t_crange<T, TAG> t_buf<T, N, t_size_dynamic>
      ::mk_crange() const noexcept {
    return {ptr_[0].cptr(), t_n{max_}};
  }

  template<typename T, t_n_ N>
  template<typename TAG>
  inline
  t_range<T, TAG> t_buf<T, N, t_size_dynamic>
      ::mk_range(t_ix ix) noexcept {
    return buf::mk_range(mk_range(), ix);
  }

  template<typename T, t_n_ N>
  template<typename TAG>
  inline
  t_crange<T, TAG> t_buf<T, N, t_size_dynamic>
      ::mk_range(t_ix ix) const noexcept {
    return buf::mk_range(mk_range(), ix);
  }

  template<typename T, t_n_ N>
  template<typename TAG>
  inline
  t_crange<T, TAG> t_buf<T, N, t_size_dynamic>
      ::mk_crange(t_ix ix) const noexcept {
    return buf::mk_crange(mk_crange(), ix);
  }

  template<typename T, t_n_ N>
  template<typename TAG>
  inline
  t_range<T, TAG> t_buf<T, N, t_size_dynamic>
      ::mk_range (t_ix begin, t_ix end) noexcept {
    return buf::mk_range(mk_range(), begin, end);
  }

  template<typename T, t_n_ N>
  template<typename TAG>
  inline
  t_crange<T, TAG> t_buf<T, N, t_size_dynamic>
      ::mk_range (t_ix begin, t_ix end) const noexcept {
    return buf::mk_range(mk_range(), begin, end);
  }

  template<typename T, t_n_ N>
  template<typename TAG>
  inline
  t_crange<T, TAG> t_buf<T, N, t_size_dynamic>
      ::mk_crange(t_ix begin, t_ix end) const noexcept {
    return buf::mk_crange(mk_crange(), begin, end);
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T, t_n_ N>
  template<typename F>
  inline
  t_buf<T, N, t_size_static>::t_buf(t_emplace_it, F&& func) noexcept {
    func(t_n{N}, store_[t_ix{0}].ptr());
  }

  template<typename T, t_n_ N>
  inline
  t_n t_buf<T, N, t_size_static>::get_capacity() const noexcept {
    return t_n{N};
  }

  template<typename T, t_n_ N>
  inline
  typename t_buf<T, N, t_size_static>::p_value
      t_buf<T, N, t_size_static>::construct(t_ix ix) noexcept {
    return get(ix) < N ? store_[ix].default_construct() : nullptr;
  }

  template<typename T, t_n_ N>
  inline
  typename t_buf<T, N, t_size_static>::p_value
      t_buf<T, N, t_size_static>
        ::construct(t_ix ix, R_value value) noexcept {
    return get(ix) < N ? store_[ix].copy_construct(value) : nullptr;
  }

  template<typename T, t_n_ N>
  inline
  typename t_buf<T, N, t_size_static>::p_value
      t_buf<T, N, t_size_static>
        ::construct(t_ix ix, x_value value) noexcept {
    return get(ix) < N ?
      store_[ix].move_construct(named::x_cast(value)) : nullptr;
  }

  template<typename T, t_n_ N>
  template<typename... Args>
  inline
  typename t_buf<T, N, t_size_static>::p_value
      t_buf<T, N, t_size_static>
        ::construct(t_emplace_it, t_ix ix, Args&&... args) noexcept {
    return get(ix) < N ?
      store_[ix].emplace_construct(named::preserve<Args>(args)...) : nullptr;
  }

  template<typename T, t_n_ N>
  inline
  t_void t_buf<T, N, t_size_static>::destruct(t_ix ix) noexcept {
    if (get(ix) < N)
      store_[ix].destruct();
  }

  template<typename T, t_n_ N>
  inline
  typename t_buf<T, N, t_size_static>::p_value
      t_buf<T, N, t_size_static>::get_ptr() noexcept {
    return store_[0].ptr();
  }

  template<typename T, t_n_ N>
  inline
  typename t_buf<T, N, t_size_static>::P_value
      t_buf<T, N, t_size_static>::get_ptr() const noexcept {
    return store_[0].cptr();
  }

  template<typename T, t_n_ N>
  inline
  typename t_buf<T, N, t_size_static>::P_value
      t_buf<T, N, t_size_static>::get_cptr() const noexcept {
    return store_[0].cptr();
  }

  template<typename T, t_n_ N>
  inline
  typename t_buf<T, N, t_size_static>::p_value
      t_buf<T, N, t_size_static>::get_ptr(t_ix ix) noexcept {
    return get(ix) < N ? store_[ix].ptr() : nullptr;
  }

  template<typename T, t_n_ N>
  inline
  typename t_buf<T, N, t_size_static>::P_value
      t_buf<T, N, t_size_static>::get_ptr(t_ix ix) const noexcept {
    return get(ix) < N ? store_[ix].ptr() : nullptr;
  }

  template<typename T, t_n_ N>
  inline
  typename t_buf<T, N, t_size_static>::P_value
      t_buf<T, N, t_size_static>::get_cptr(t_ix ix) const noexcept {
    return get(ix) < N ? store_[ix].cptr() : nullptr;
  }

  template<typename T, t_n_ N>
  inline
  typename t_buf<T, N, t_size_static>::r_value
      t_buf<T, N, t_size_static>::get_ref(t_ix ix) noexcept {
    return store_[ix].ref();
  }

  template<typename T, t_n_ N>
  inline
  typename t_buf<T, N, t_size_static>::R_value
      t_buf<T, N, t_size_static>::get_ref(t_ix ix) const noexcept {
    return store_[ix].ref();
  }

  template<typename T, t_n_ N>
  inline
  typename t_buf<T, N, t_size_static>::R_value
      t_buf<T, N, t_size_static>::get_cref(t_ix ix) const noexcept {
    return store_[ix].cref();
  }

  template<typename T, t_n_ N>
  inline
  typename t_buf<T, N, t_size_static>::r_value
      t_buf<T, N, t_size_static>::operator[](t_ix ix) noexcept {
    return get_ref(ix);
  }

  template<typename T, t_n_ N>
  inline
  typename t_buf<T, N, t_size_static>::R_value
      t_buf<T, N, t_size_static>::operator[](t_ix ix) const noexcept {
    return get_cref(ix);
  }

  template<typename T, t_n_ N>
  template<typename TAG>
  inline
  t_range<T, TAG> t_buf<T, N, t_size_static>::mk_range() noexcept {
    return {store_[0].ptr(), t_n{N}};
  }

  template<typename T, t_n_ N>
  template<typename TAG>
  inline
  t_crange<T, TAG> t_buf<T, N, t_size_static>::mk_range() const noexcept {
    return {store_[0].ptr(), t_n{N}};
  }

  template<typename T, t_n_ N>
  template<typename TAG>
  inline
  t_crange<T, TAG> t_buf<T, N, t_size_static>
      ::mk_crange() const noexcept {
    return {store_[0].cptr(), t_n{N}};
  }

  template<typename T, t_n_ N>
  template<typename TAG>
  inline
  t_range<T, TAG> t_buf<T, N, t_size_static>
      ::mk_range(t_ix ix) noexcept {
    return buf::mk_range(mk_range(), ix);
  }

  template<typename T, t_n_ N>
  template<typename TAG>
  inline
  t_crange<T, TAG> t_buf<T, N, t_size_static>
      ::mk_range(t_ix ix) const noexcept {
    return buf::mk_range(mk_range(), ix);
  }

  template<typename T, t_n_ N>
  template<typename TAG>
  inline
  t_crange<T, TAG> t_buf<T, N, t_size_static>
      ::mk_crange(t_ix ix) const noexcept {
    return buf::mk_crange(mk_crange(), ix);
  }

  template<typename T, t_n_ N>
  template<typename TAG>
  inline
  t_range<T, TAG> t_buf<T, N, t_size_static>
      ::mk_range (t_ix begin, t_ix end) noexcept {
    return buf::mk_range(mk_range(), begin, end);
  }

  template<typename T, t_n_ N>
  template<typename TAG>
  inline
  t_crange<T, TAG> t_buf<T, N, t_size_static>
      ::mk_range (t_ix begin, t_ix end) const noexcept {
    return buf::mk_range(mk_range(), begin, end);
  }

  template<typename T, t_n_ N>
  template<typename TAG>
  inline
  t_crange<T, TAG> t_buf<T, N, t_size_static>
      ::mk_crange(t_ix begin, t_ix end) const noexcept {
    return buf::mk_crange(mk_crange(), begin, end);
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  template<typename F>
  inline
  t_buf<T, 1, t_size_static>::t_buf(t_emplace_it, F&& func) noexcept {
    func(t_n{1}, store_.ptr());
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
