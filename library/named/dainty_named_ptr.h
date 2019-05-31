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

#ifndef _DAINTY_NAMED_PTR_H_
#define _DAINTY_NAMED_PTR_H_

#include "dainty_named.h"

namespace dainty
{
namespace named
{
namespace ptr
{
  using named::t_ix;
  using named::t_validity;
  using named::VALID;
  using named::INVALID;

///////////////////////////////////////////////////////////////////////////////

  template<typename D>
  struct t_del_ {
    t_del_() = default;
    template<typename E>
    t_del_(E&& _deleter) : deleter{named::preserve<E>(_deleter)} { }
    D deleter;
  };

  template<typename D>
  struct t_del_<D*> {
    t_del_(D* _deleter) : deleter(_deleter) {
    }
    D* deleter = nullptr; // address must be the same
  };

///////////////////////////////////////////////////////////////////////////////

  enum t_deleter    {};
  enum t_no_deleter {};

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG, typename D = t_no_deleter>
  class t_ptr { //1
  public:
    using t_value = typename t_prefix<T>::t_;
    using p_value = typename t_prefix<T>::p_;
    using P_value = typename t_prefix<T>::P_;
    using r_value = typename t_prefix<T>::r_;
    using R_value = typename t_prefix<T>::R_;
    using r_ptr   = typename t_prefix<t_ptr>::r_;
    using x_ptr   = typename t_prefix<t_ptr>::x_;

     t_ptr() = default;
     t_ptr(p_value);
     template<typename E>
     t_ptr(p_value, E&& deleter);
     t_ptr(x_ptr);
    ~t_ptr();

    r_ptr operator=(p_value);
    r_ptr operator=(x_ptr);

    p_value release();
    t_void  clear();

    operator t_validity() const;

    p_value get();
    P_value get() const;

    p_value operator->();
    P_value operator->() const;
    r_value operator* ();
    R_value operator* () const;

  private:
    template<typename, typename, typename> friend class t_ptr;
    template<typename, typename, typename> friend class t_cptr;
    p_value   ptr_ = nullptr;
    t_del_<D> del_;
  };

  template<typename T, typename TAG>
  class t_ptr<T, TAG, t_deleter> { //2
  public:
    using t_value = typename t_prefix<T>::t_;
    using p_value = typename t_prefix<T>::p_;
    using P_value = typename t_prefix<T>::P_;
    using r_value = typename t_prefix<T>::r_;
    using R_value = typename t_prefix<T>::R_;
    using r_ptr   = typename t_prefix<t_ptr>::r_;
    using x_ptr   = typename t_prefix<t_ptr>::x_;

     t_ptr() = default;
     t_ptr(p_value value);
     t_ptr(x_ptr ptr);
    ~t_ptr();

    r_ptr operator=(p_value);
    r_ptr operator=(x_ptr);

    p_value release();
    t_void  clear();

    operator t_validity() const;

    p_value get();
    P_value get() const;

    p_value operator->();
    P_value operator->() const;
    r_value operator* ();
    R_value operator* () const;

  private:
    template<typename, typename, typename> friend class t_ptr;
    template<typename, typename, typename> friend class t_cptr;
    p_value ptr_ = nullptr;
  };

  template<typename T, typename TAG>
  class t_ptr<T, TAG, t_no_deleter> { //3
  public:
    using t_value = typename t_prefix<T>::t_;
    using p_value = typename t_prefix<T>::p_;
    using P_value = typename t_prefix<T>::P_;
    using r_value = typename t_prefix<T>::r_;
    using R_value = typename t_prefix<T>::R_;
    using r_ptr   = typename t_prefix<t_ptr>::r_;
    using R_ptr   = typename t_prefix<t_ptr>::R_;

    t_ptr() = default;
    t_ptr(p_value);
    t_ptr(R_ptr);
    template<typename D>
    t_ptr(const t_ptr<T, TAG, D>&);
    // XXX rvalue must fail

    r_ptr operator=(p_value);
    r_ptr operator=(R_ptr);
    template<typename D>
    r_ptr operator=(const t_ptr<T, TAG, D>&);

    p_value release();
    t_void  clear();

    operator t_validity() const;

    p_value get();
    P_value get() const;

    p_value operator->();
    P_value operator->() const;
    r_value operator* ();
    R_value operator* () const;

  private:
    template<typename, typename, typename> friend class t_cptr;
    p_value ptr_ = nullptr;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG, typename D = t_no_deleter>
  class t_cptr { //4
  public:
    using t_value = typename t_prefix<T>::t_;
    using P_value = typename t_prefix<T>::P_;
    using R_value = typename t_prefix<T>::R_;
    using r_cptr  = typename t_prefix<t_cptr>::r_;
    using x_cptr  = typename t_prefix<t_cptr>::x_;
    using x_ptr   = typename t_prefix<t_ptr<T, TAG, D>>::x_;

     t_cptr() = default;
     t_cptr(P_value value) : ptr_{value}                   { }
     template<typename E>
     t_cptr(P_value value, E&& deleter)
       : ptr_{value}, del_{named::preserve<E>(deleter)}  { }
     t_cptr(x_cptr);
     t_cptr(x_ptr);
    ~t_cptr();

    r_cptr operator=(P_value);
    r_cptr operator=(x_cptr);
    r_cptr operator=(x_ptr);

    P_value release()           { return named::reset(ptr_); }
    t_void  clear();

    operator t_validity() const { return ptr_ ? VALID : INVALID; }

    P_value get() const         { return  ptr_; }

    P_value operator->() const  { return  ptr_; }
    R_value operator* () const  { return *ptr_; }

  private:
    template<typename, typename, typename> friend class t_cptr;
    P_value   ptr_ = nullptr;
    t_del_<D> del_;
  };

  template<typename T, typename TAG>
  class t_cptr<T, TAG, t_deleter> { //5
  public:
    using t_value = typename t_prefix<T>::t_;
    using P_value = typename t_prefix<T>::P_;
    using R_value = typename t_prefix<T>::R_;
    using r_cptr  = typename t_prefix<t_cptr>::r_;
    using x_cptr  = typename t_prefix<t_cptr>::x_;
    using x_ptr   = typename t_prefix<t_ptr<T, TAG, t_deleter>>::x_;

     t_cptr() = default;
     t_cptr(P_value value) : ptr_{value}         { }
     t_cptr(x_cptr ptr)    : ptr_{ptr.release()} { }
     t_cptr(x_ptr  ptr)    : ptr_{ptr.release()} { }
    ~t_cptr();

    r_cptr operator=(P_value);
    r_cptr operator=(x_cptr);

    P_value release()           { return named::reset(ptr_); }
    t_void  clear();

    operator t_validity() const { return ptr_ ? VALID : INVALID; }

    P_value get() const         { return  ptr_; }

    P_value operator->() const  { return  ptr_; }
    R_value operator* () const  { return *ptr_; }

  private:
    template<typename, typename, typename> friend class t_cptr;
    P_value ptr_ = nullptr;
  };

  template<typename T, typename TAG>
  class t_cptr<T, TAG, t_no_deleter> { //&
  public:
    using t_value = typename t_prefix<T>::t_;
    using P_value = typename t_prefix<T>::P_;
    using R_value = typename t_prefix<T>::R_;
    using r_cptr  = typename t_prefix<t_cptr>::r_;
    using R_cptr  = typename t_prefix<t_cptr>::R_;

    t_cptr() = default;
    t_cptr(P_value value) : ptr_{value}                    { }
    t_cptr(R_cptr ptr)    : ptr_{ptr.get()}                { }
    template<typename D>
    t_cptr(const t_cptr<T, TAG, D>& ptr) : ptr_{ptr.ptr_} { }
    template<typename D>
    t_cptr(const t_ptr<T, TAG, D>& ptr)  : ptr_{ptr.ptr_} { }

    r_cptr operator=(P_value value)     { ptr_ = value;     return *this; }
    r_cptr operator=(R_cptr ptr)        { ptr_ = ptr.get(); return *this; }

    template<typename D>
    r_cptr operator=(const t_cptr<T, TAG, D>& ptr) {
      ptr_ = ptr.ptr_;
      return *this;
    }

    template<typename D>
    r_cptr operator=(const t_ptr<T, TAG, D>& ptr) {
      ptr_ = ptr.ptr_;
      return *this;
    }

    P_value release()                   { return named::reset(ptr_); }
    t_void  clear()                     { named::reset(ptr_); }

    operator t_validity() const         { return ptr_ ? VALID : INVALID; }

    P_value get() const                 { return  ptr_; }

    P_value operator->() const          { return  ptr_; }
    R_value operator* () const          { return *ptr_; }

  private:
    P_value ptr_ = nullptr;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG, typename D>
  class t_ptr<T[], TAG, D> { //7
  public:
    using t_value = typename t_prefix<T>::t_;
    using p_value = typename t_prefix<T>::p_;
    using P_value = typename t_prefix<T>::P_;
    using r_value = typename t_prefix<T>::r_;
    using R_value = typename t_prefix<T>::R_;
    using r_ptr   = typename t_prefix<t_ptr>::r_;
    using x_ptr   = typename t_prefix<t_ptr>::x_;

     t_ptr() = default;
     t_ptr(p_value value) : ptr_{value}                    { }
     template<typename E>
     t_ptr(p_value value, E&& deleter)
       : ptr_{value}, del_{named::preserve<E>(deleter)}  { }
     t_ptr(x_ptr);
    ~t_ptr();

    r_ptr operator=(p_value);
    r_ptr operator=(x_ptr);

    p_value release()                 { return named::reset(ptr_); }
    t_void  clear();

    operator t_validity() const       { return ptr_ ? VALID : INVALID; }

    p_value get()                     { return  ptr_; }
    P_value get() const               { return  ptr_; }

    r_value operator[](t_ix ix)       { return  ptr_[get(ix)]; }
    R_value operator[](t_ix ix) const { return  ptr_[get(ix)]; }

  private:
    template<typename, typename, typename> friend class t_ptr;
    template<typename, typename, typename> friend class t_cptr;
    p_value   ptr_ = nullptr;
    t_del_<D> del_;
  };

  template<typename T, typename TAG>
  class t_ptr<T[], TAG, t_deleter> { //8
  public:
    using t_value = typename t_prefix<T>::t_;
    using p_value = typename t_prefix<T>::p_;
    using P_value = typename t_prefix<T>::P_;
    using r_value = typename t_prefix<T>::r_;
    using R_value = typename t_prefix<T>::R_;
    using r_ptr   = typename t_prefix<t_ptr>::r_;
    using x_ptr   = typename t_prefix<t_ptr>::x_;

     t_ptr() = default;
     t_ptr(p_value value) : ptr_{value}         { }
     t_ptr(x_ptr ptr)     : ptr_{ptr.release()} { }
    ~t_ptr();

    r_ptr operator=(p_value);
    r_ptr operator=(x_ptr);

    p_value release()                 { return named::reset(ptr_); }
    t_void  clear();

    operator t_validity() const       { return ptr_ ? VALID : INVALID; }

    p_value get()                     { return  ptr_; }
    P_value get() const               { return  ptr_; }

    r_value operator[](t_ix ix)       { return  ptr_[named::get(ix)]; }
    R_value operator[](t_ix ix) const { return  ptr_[named::get(ix)]; }

  private:
    template<typename, typename, typename> friend class t_ptr;
    template<typename, typename, typename> friend class t_cptr;
    p_value ptr_ = nullptr;
  };

  template<typename T, typename TAG>
  class t_ptr<T[], TAG, t_no_deleter> { //9
  public:
    using t_value = typename t_prefix<T>::t_;
    using p_value = typename t_prefix<T>::p_;
    using P_value = typename t_prefix<T>::P_;
    using r_value = typename t_prefix<T>::r_;
    using R_value = typename t_prefix<T>::R_;
    using r_ptr   = typename t_prefix<t_ptr>::r_;
    using R_ptr   = typename t_prefix<t_ptr>::R_;

    t_ptr() = default;
    t_ptr(p_value value) : ptr_{value}                               { }
    t_ptr(R_ptr ptr)     : ptr_{ptr.ptr_}                            { }
    template<typename D>
    t_ptr(const t_ptr<T[], TAG, D>& ptr) : ptr_{ptr.ptr_}            { }

    r_ptr operator=(p_value value)    { ptr_ = value;    return *this; }
    r_ptr operator=(R_ptr ptr)        { ptr_ = ptr.ptr_; return *this; }

    template<typename D>
    r_ptr operator=(const t_ptr<T[], TAG, D>& ptr) {
      ptr_ = ptr.ptr_;
      return *this;
    }

    p_value release()                 { return named::reset(ptr_); }
    t_void  clear()                   { named::reset(ptr_); }

    operator t_validity() const       { return ptr_ ? VALID : INVALID; }

    p_value get()                     { return  ptr_; }
    P_value get() const               { return  ptr_; }

    r_value operator[](t_ix ix)       { return  ptr_[named::get(ix)]; }
    R_value operator[](t_ix ix) const { return  ptr_[named::get(ix)]; }

  private:
    template<typename, typename, typename> friend class t_cptr;
    p_value ptr_ = nullptr;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG, typename D>
  class t_cptr<T[], TAG, D> { //10
  public:
    using t_value = typename t_prefix<T>::t_;
    using P_value = typename t_prefix<T>::P_;
    using R_value = typename t_prefix<T>::R_;
    using r_cptr  = typename t_prefix<t_cptr>::r_;
    using x_cptr  = typename t_prefix<t_cptr>::x_;
    using x_ptr   = typename t_prefix<t_ptr<T[], TAG, D>>::x_;

     t_cptr() = default;
     t_cptr(P_value value) : ptr_{value}                    { }
     template<typename E>
     t_cptr(P_value value, E&& deleter)
       : ptr_{value}, del_{named::preserve<E>(deleter)}   { }
     t_cptr(x_cptr);
     t_cptr(x_ptr);
    ~t_cptr();

    r_cptr operator=(P_value);
    r_cptr operator=(x_cptr);
    r_cptr operator=(x_ptr);

    P_value release()                 { return named::reset(ptr_); }
    t_void  clear();

    operator t_validity() const       { return ptr_ ? VALID : INVALID; }

    P_value get() const               { return  ptr_; }

    R_value operator[](t_ix ix) const { return  ptr_[named::get(ix)]; }

  private:
    template<typename, typename, typename> friend class t_cptr;
    P_value   ptr_ = nullptr;
    t_del_<D> del_;
  };

  template<typename T, typename TAG>
  class t_cptr<T[], TAG, t_deleter> { //11
  public:
    using t_value = typename t_prefix<T>::t_;
    using P_value = typename t_prefix<T>::P_;
    using R_value = typename t_prefix<T>::R_;
    using r_cptr  = typename t_prefix<t_cptr>::r_;
    using x_cptr  = typename t_prefix<t_cptr>::x_;
    using x_ptr   = typename t_prefix<t_ptr<T[], TAG, t_deleter>>::x_;

     t_cptr() = default;
     t_cptr(P_value value) : ptr_{value}         { }
     t_cptr(x_cptr ptr)    : ptr_{ptr.release()} { }
     t_cptr(x_ptr  ptr)    : ptr_{ptr.release()} { }
    ~t_cptr();

    r_cptr operator=(P_value);
    r_cptr operator=(x_cptr);
    r_cptr operator=(x_ptr);

    P_value release()                 { return named::reset(ptr_); }
    t_void  clear();

    operator t_validity() const       { return ptr_ ? VALID : INVALID; }

    P_value get() const               { return  ptr_; }

    R_value operator[](t_ix ix) const { return  ptr_[get(ix)]; }

  private:
    template<typename, typename, typename> friend class t_cptr;
    P_value ptr_ = nullptr;
  };

  template<typename T, typename TAG>
  class t_cptr<T[], TAG, t_no_deleter> { //12
  public:
    using t_value = typename t_prefix<T>::t_;
    using P_value = typename t_prefix<T>::P_;
    using R_value = typename t_prefix<T>::R_;
    using r_cptr  = typename t_prefix<t_cptr>::r_;
    using R_cptr  = typename t_prefix<t_cptr>::R_;

    t_cptr() = default;
    t_cptr(P_value value) : ptr_{value}    { }
    t_cptr(R_cptr ptr)    : ptr_{ptr.ptr_} { }
    template<typename D>
    t_cptr(const t_cptr<T[], TAG, D>& ptr) : ptr_{ptr.ptr_}          { }
    template<typename D>
    t_cptr(const t_ptr<T[], TAG, D>& ptr)  : ptr_{ptr.ptr_}          { }

    r_cptr operator=(P_value value)   { ptr_ = value;    return *this; }
    r_cptr operator=(R_cptr ptr)      { ptr_ = ptr.ptr_; return *this; }

    template<typename D>
    r_cptr operator=(const t_cptr<T[], TAG, D>& ptr) {
      ptr_ = ptr.ptr_;
      return *this;
    }

    template<typename D>
    r_cptr operator=(const t_ptr<T[], TAG, D>& ptr) {
      ptr_ = ptr.ptr_;
      return *this;
    }

    P_value release()                 { return named::reset(ptr_); }
    t_void  clear()                   { named::reset(ptr_); }

    operator t_validity() const       { return ptr_ ? VALID : INVALID; }

    P_value get() const               { return  ptr_; }

    R_value operator[](t_ix ix) const { return  ptr_[get(ix)]; }

  private:
    P_value ptr_ = nullptr;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG, typename D>
  inline
  t_ptr<T, TAG, D>::t_ptr(p_value value) : ptr_{value} {
  }

  template<typename T, typename TAG, typename D>
  template<typename E>
  inline
  t_ptr<T, TAG, D>::t_ptr(p_value value, E&& deleter)
    : ptr_{value}, del_{named::preserve<E>(deleter)} {
  }

  template<typename T, typename TAG, typename D>
  inline
  t_ptr<T, TAG, D>::t_ptr(x_ptr ptr)
    : ptr_{ptr.release()}, del_{named::x_cast(ptr.del_)} {
  }

  template<typename T, typename TAG, typename D>
  inline
  t_ptr<T, TAG, D>::~t_ptr() {
    clear();
  }

  template<typename T, typename TAG, typename D>
  inline
  typename t_ptr<T, TAG, D>::r_ptr t_ptr<T, TAG, D>::operator=(p_value value) {
    if (ptr_)
      del_.deleter(named::reset(ptr_, value));
    else
      ptr_ = value;
    return *this;
  }

  template<typename T, typename TAG, typename D>
  inline
  typename t_ptr<T, TAG, D>::r_ptr t_ptr<T, TAG, D>::operator=(x_ptr ptr) {
    if (ptr_)
      del_.deleter(named::reset(ptr_, ptr.release()));
    else
      ptr_ = ptr.release();
    del_ = named::x_cast(ptr.del_);
    return *this;
  }

  template<typename T, typename TAG, typename D>
  inline
  t_ptr<T, TAG, D>::operator t_validity() const {
    return ptr_ ? VALID : INVALID;
  }

  template<typename T, typename TAG, typename D>
  inline
  typename t_ptr<T, TAG, D>::p_value t_ptr<T, TAG, D>::release() {
    return named::reset(ptr_);
  }

  template<typename T, typename TAG, typename D>
  inline
  t_void t_ptr<T, TAG, D>::clear() {
    if (ptr_)
      del_.deleter(named::reset(ptr_));
  }

  template<typename T, typename TAG, typename D>
  inline
  typename t_ptr<T, TAG, D>::p_value t_ptr<T, TAG, D>::get() {
    return ptr_;
  }

  template<typename T, typename TAG, typename D>
  inline
  typename t_ptr<T, TAG, D>::P_value t_ptr<T, TAG, D>::get() const {
    return ptr_;
  }

  template<typename T, typename TAG, typename D>
  inline
  typename t_ptr<T, TAG, D>::p_value t_ptr<T, TAG, D>::operator->() {
    return ptr_;
  }

  template<typename T, typename TAG, typename D>
  inline
  typename t_ptr<T, TAG, D>::P_value t_ptr<T, TAG, D>::operator->() const {
    return ptr_;
  }

  template<typename T, typename TAG, typename D>
  inline
  typename t_ptr<T, TAG, D>::r_value t_ptr<T, TAG, D>::operator*() {
    return *ptr_;
  }

  template<typename T, typename TAG, typename D>
  inline
  typename t_ptr<T, TAG, D>::R_value t_ptr<T, TAG, D>::operator*() const {
    return *ptr_;
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG>
  inline
  t_ptr<T, TAG, t_deleter>::t_ptr(p_value value) : ptr_{value} {
  }

  template<typename T, typename TAG>
  inline
  t_ptr<T, TAG, t_deleter>::t_ptr(x_ptr ptr) : ptr_{ptr.release()} {
  }

  template<typename T, typename TAG>
  inline
  t_ptr<T, TAG, t_deleter>::~t_ptr() {
    clear();
  }

  template<typename T, typename TAG>
  inline
  typename t_ptr<T, TAG, t_deleter>::r_ptr
      t_ptr<T, TAG, t_deleter>::operator=(p_value value) {
    if (ptr_)
      delete named::reset(ptr_, value);
    else
      ptr_ = value;
    return *this;
  }

  template<typename T, typename TAG>
  inline
  typename t_ptr<T, TAG, t_deleter>::r_ptr
      t_ptr<T, TAG, t_deleter>::operator=(x_ptr ptr) {
    if (ptr_)
      delete named::reset(ptr_, ptr.release());
    else
      ptr_ = ptr.release();
    return *this;
  }

  template<typename T, typename TAG>
  inline
  t_ptr<T, TAG, t_deleter>::operator t_validity() const {
    return ptr_ ? VALID : INVALID;
  }

  template<typename T, typename TAG>
  inline
  typename t_ptr<T, TAG, t_deleter>::p_value
      t_ptr<T, TAG, t_deleter>::release() {
    return named::reset(ptr_);
  }

  template<typename T, typename TAG>
  inline
  t_void t_ptr<T, TAG, t_deleter>::clear() {
    if (ptr_)
      delete named::reset(ptr_);
  }

  template<typename T, typename TAG>
  inline
  typename t_ptr<T, TAG, t_deleter>::p_value
      t_ptr<T, TAG, t_deleter>::get() {
    return ptr_;
  }

  template<typename T, typename TAG>
  inline
  typename t_ptr<T, TAG, t_deleter>::P_value
      t_ptr<T, TAG, t_deleter>::get() const {
    return ptr_;
  }

  template<typename T, typename TAG>
  inline
  typename t_ptr<T, TAG, t_deleter>::p_value
      t_ptr<T, TAG, t_deleter>::operator->() {
    return  ptr_;
  }

  template<typename T, typename TAG>
  inline
  typename t_ptr<T, TAG, t_deleter>::P_value
      t_ptr<T, TAG, t_deleter>::operator->() const {
    return  ptr_;
  }

  template<typename T, typename TAG>
  inline
  typename t_ptr<T, TAG, t_deleter>::r_value
      t_ptr<T, TAG, t_deleter>::operator*() {
    return *ptr_;
  }

  template<typename T, typename TAG>
  inline
  typename t_ptr<T, TAG, t_deleter>::R_value
      t_ptr<T, TAG, t_deleter>::operator*() const {
    return *ptr_;
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG>
  inline
  t_ptr<T, TAG, t_no_deleter>::t_ptr(p_value value) : ptr_{value} {
  }

  template<typename T, typename TAG>
  inline
  t_ptr<T, TAG, t_no_deleter>::t_ptr(R_ptr ptr) : ptr_{ptr.ptr_} {
  }

  template<typename T, typename TAG>
  template<typename D>
  inline
  t_ptr<T, TAG, t_no_deleter>::t_ptr(const t_ptr<T, TAG, D>& ptr)
    : ptr_{ptr.get()} {
  }

  template<typename T, typename TAG>
  inline
  typename t_ptr<T, TAG, t_no_deleter>::r_ptr
      t_ptr<T, TAG, t_no_deleter>::operator=(p_value value) {
    ptr_ = value;
    return *this;
  }

  template<typename T, typename TAG>
  inline
  typename t_ptr<T, TAG, t_no_deleter>::r_ptr
      t_ptr<T, TAG, t_no_deleter>::operator=(R_ptr ptr) {
    ptr_ = ptr.ptr_;
    return *this;
  }

  template<typename T, typename TAG>
  template<typename D>
  inline
  typename t_ptr<T, TAG, t_no_deleter>::r_ptr
      t_ptr<T, TAG, t_no_deleter>::operator=(const t_ptr<T, TAG, D>& ptr) {
    ptr_ = ptr.get();
    return *this;
  }

  template<typename T, typename TAG>
  inline
  typename t_ptr<T, TAG, t_no_deleter>::p_value
      t_ptr<T, TAG, t_no_deleter>::release() {
    return named::reset(ptr_);
   }

  template<typename T, typename TAG>
  inline
  t_void t_ptr<T, TAG, t_no_deleter>::clear() {
    named::reset(ptr_);
  }

  template<typename T, typename TAG>
  inline
  t_ptr<T, TAG, t_no_deleter>::operator t_validity() const {
    return ptr_ ? VALID : INVALID;
  }

  template<typename T, typename TAG>
  inline
  typename t_ptr<T, TAG, t_no_deleter>::p_value
      t_ptr<T, TAG, t_no_deleter>::get() {
    return  ptr_;
  }

  template<typename T, typename TAG>
  inline
  typename t_ptr<T, TAG, t_no_deleter>::P_value
      t_ptr<T, TAG, t_no_deleter>::get() const {
    return ptr_;
  }

  template<typename T, typename TAG>
  inline
  typename t_ptr<T, TAG, t_no_deleter>::p_value
      t_ptr<T, TAG, t_no_deleter>::operator->() {
    return ptr_;
  }

  template<typename T, typename TAG>
  inline
  typename t_ptr<T, TAG, t_no_deleter>::P_value
      t_ptr<T, TAG, t_no_deleter>::operator->() const {
    return  ptr_;
  }

  template<typename T, typename TAG>
  inline
  typename t_ptr<T, TAG, t_no_deleter>::r_value
      t_ptr<T, TAG, t_no_deleter>::operator*() {
    return *ptr_;
  }

  template<typename T, typename TAG>
  inline
  typename t_ptr<T, TAG, t_no_deleter>::R_value
      t_ptr<T, TAG, t_no_deleter>::operator*() const {
    return *ptr_;
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG, typename D>
  inline
  t_cptr<T, TAG, D>::~t_cptr() {
    clear();
  }

  template<typename T, typename TAG, typename D>
  inline
  typename t_cptr<T, TAG, D>::r_cptr
      t_cptr<T, TAG, D>::operator=(P_value value) {
    if (ptr_)
      del_.deleter(named::reset(ptr_, value));
    else
      ptr_ = value;
    return *this;
  }

  template<typename T, typename TAG, typename D>
  inline
  typename t_cptr<T, TAG, D>::r_cptr
      t_cptr<T, TAG, D>::operator=(x_cptr ptr) {
    if (ptr_)
      del_.deleter(named::reset(ptr_, ptr.release()));
    else
      ptr_ = ptr.release();
    del_ = named::x_cast(ptr.del_);
    return *this;
  }

  template<typename T, typename TAG, typename D>
  inline
  t_void t_cptr<T, TAG, D>::clear() {
    if (ptr_)
      del_.deleter(named::reset(ptr_));
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG>
  inline
  t_cptr<T, TAG, t_deleter>::~t_cptr() {
    clear();
  }

  template<typename T, typename TAG>
  inline
  typename t_cptr<T, TAG, t_deleter>::r_cptr
      t_cptr<T, TAG, t_deleter>::operator=(P_value value) {
    if (ptr_)
      delete named::reset(ptr_, value);
    else
      ptr_ = value;
    return *this;
  }

  template<typename T, typename TAG>
  inline
  typename t_cptr<T, TAG, t_deleter>::r_cptr
      t_cptr<T, TAG, t_deleter>::operator=(x_cptr ptr) {
    if (ptr_)
      delete named::reset(ptr_, ptr.release());
    else
      ptr_ = ptr.release();
    return *this;
  }

  template<typename T, typename TAG>
  inline
  t_void t_cptr<T, TAG, t_deleter>::clear() {
    if (ptr_)
      delete named::reset(ptr_);
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG, typename D>
  inline
  t_ptr<T[], TAG, D>::~t_ptr() {
    clear();
  }

  template<typename T, typename TAG, typename D>
  inline
  typename t_ptr<T[], TAG, D>::r_ptr
      t_ptr<T[], TAG, D>::operator=(p_value value) {
    if (ptr_)
      del_.deleter(named::reset(ptr_, value));
    else
      ptr_ = value;
    return *this;
  }

  template<typename T, typename TAG, typename D>
  inline
  typename t_ptr<T[], TAG, D>::r_ptr t_ptr<T[], TAG, D>::operator=(x_ptr ptr) {
    if (ptr_)
      del_.deleter(named::reset(ptr_, ptr.release()));
    else
      ptr_ = ptr.release();
    del_ =  named::x_cast(ptr.del_);
    return *this;
  }

  template<typename T, typename TAG, typename D>
  inline
  t_void t_ptr<T[], TAG, D>::clear() {
    if (ptr_)
      del_.deleter(named::reset(ptr_));
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG>
  inline
  t_ptr<T[], TAG, t_deleter>::~t_ptr() {
    clear();
  }

  template<typename T, typename TAG>
  inline
  typename t_ptr<T[], TAG, t_deleter>::r_ptr
      t_ptr<T[], TAG, t_deleter>::operator=(p_value value) {
    if (ptr_)
      delete [] named::reset(ptr_, value);
    else
      ptr_ = value;
    return *this;
  }

  template<typename T, typename TAG>
  inline
  typename t_ptr<T[], TAG, t_deleter>::r_ptr
      t_ptr<T[], TAG, t_deleter>::operator=(x_ptr ptr) {
    if (ptr_)
      delete [] named::reset(ptr_, ptr.release());
    else
      ptr_ = ptr.release();
    return *this;
  }

  template<typename T, typename TAG>
  inline
  t_void t_ptr<T[], TAG, t_deleter>::clear() {
    if (ptr_)
      delete [] named::reset(ptr_);
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG, typename D>
  inline
  t_cptr<T[], TAG, D>::~t_cptr() {
    clear();
  }

  template<typename T, typename TAG, typename D>
  inline
  typename t_cptr<T[], TAG, D>::r_cptr
      t_cptr<T[], TAG, D>::operator=(P_value value) {
    if (ptr_)
      del_.deleter(named::reset(ptr_, value));
    else
      ptr_ = value;
    return *this;
  }

  template<typename T, typename TAG, typename D>
  inline
  typename t_cptr<T[], TAG, D>::r_cptr
      t_cptr<T[], TAG, D>::operator=(x_cptr ptr) {
    if (ptr_)
      del_.deleter(named::reset(ptr_, ptr.release()));
    else
      ptr_ = ptr.release();
    del_ = named::x_cast(ptr.del_);
    return *this;
  }

  template<typename T, typename TAG, typename D>
  inline
  t_void t_cptr<T[], TAG, D>::clear() {
    if (ptr_)
      del_.deleter(named::reset(ptr_));
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG>
  inline
  t_cptr<T[], TAG, t_deleter>::~t_cptr() {
    clear();
  }

  template<typename T, typename TAG>
  inline
  typename t_cptr<T[], TAG, t_deleter>::r_cptr
      t_cptr<T[], TAG, t_deleter>::operator=(P_value value) {
    if (ptr_)
      delete [] named::reset(ptr_, value);
    else
      ptr_ = value;
    return *this;
  }

  template<typename T, typename TAG>
  inline
  typename t_cptr<T[], TAG, t_deleter>::r_cptr
      t_cptr<T[], TAG, t_deleter>::operator=(x_cptr ptr) {
    if (ptr_)
      delete [] named::reset(ptr_, ptr.release());
    else
      ptr_ = ptr.release();
    return *this;
  }

  template<typename T, typename TAG>
  inline
  t_void t_cptr<T[], TAG, t_deleter>::clear() {
    if (ptr_)
      delete [] named::reset(ptr_);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
