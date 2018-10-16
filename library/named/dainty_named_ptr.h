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

  enum t_deleter    {};
  enum t_no_deleter {};

  template<typename T, typename TAG, typename D = t_no_deleter>
  class t_ptr {
  public:
    using t_value = typename t_prefix<T>::t_;
    using p_value = typename t_prefix<T>::p_;
    using P_value = typename t_prefix<T>::P_;
    using r_value = typename t_prefix<T>::r_;
    using R_value = typename t_prefix<T>::R_;
    using r_ptr   = typename t_prefix<t_ptr>::r_;
    using x_ptr   = typename t_prefix<t_ptr>::x_;

     t_ptr() = default;
     t_ptr(p_value value) : ptr_{value}           { }
     t_ptr(x_ptr ptr)     : ptr_{reset(ptr.ptr_)} { }
    ~t_ptr();

    r_ptr operator=(p_value);
    r_ptr operator=(x_ptr);

    p_value release()           { return reset(ptr_); }
    t_void  clear();

    operator t_validity() const { return ptr_ ? VALID : INVALID; }

    p_value get()               { return  ptr_; }
    P_value get() const         { return  ptr_; }

    p_value operator->()        { return  ptr_; }
    P_value operator->() const  { return  ptr_; }
    r_value operator* ()        { return *ptr_; }
    R_value operator* () const  { return *ptr_; }

  private:
    p_value ptr_ = nullptr;
  };

  template<typename T, typename TAG>
  class t_ptr<T, TAG, t_deleter> {
  public:
    using t_value = typename t_prefix<T>::t_;
    using p_value = typename t_prefix<T>::p_;
    using P_value = typename t_prefix<T>::P_;
    using r_value = typename t_prefix<T>::r_;
    using R_value = typename t_prefix<T>::R_;
    using r_ptr   = typename t_prefix<t_ptr>::r_;
    using x_ptr   = typename t_prefix<t_ptr>::x_;

     t_ptr() = default;
     t_ptr(p_value value) : ptr_{value}           { }
     t_ptr(x_ptr ptr)     : ptr_{reset(ptr.ptr_)} { }
    ~t_ptr();

    r_ptr operator=(p_value);
    r_ptr operator=(x_ptr);

    p_value release()           { return reset(ptr_); }
    t_void  clear();

    operator t_validity() const { return ptr_ ? VALID : INVALID; }

    p_value get()               { return  ptr_; }
    P_value get() const         { return  ptr_; }

    p_value operator->()        { return  ptr_; }
    P_value operator->() const  { return  ptr_; }
    r_value operator* ()        { return *ptr_; }
    R_value operator* () const  { return *ptr_; }

  private:
    p_value ptr_ = nullptr;
  };

  template<typename T, typename TAG>
  class t_ptr<T, TAG, t_no_deleter> {
  public:
    using t_value = typename t_prefix<T>::t_;
    using p_value = typename t_prefix<T>::p_;
    using P_value = typename t_prefix<T>::P_;
    using r_value = typename t_prefix<T>::r_;
    using R_value = typename t_prefix<T>::R_;
    using r_ptr   = typename t_prefix<t_ptr>::r_;
    using R_ptr   = typename t_prefix<t_ptr>::R_;

    t_ptr() = default;
    t_ptr(p_value value) : ptr_{value}    { }
    t_ptr(R_ptr ptr)     : ptr_{ptr.ptr_} { }

    r_ptr operator=(p_value value) { ptr_ = value;    return *this; }
    r_ptr operator=(R_ptr ptr)     { ptr_ = ptr.ptr_; return *this; }

    p_value release()              { return reset(ptr_); }
    t_void  clear()                { reset(ptr_); }

    operator t_validity() const    { return ptr_ ? VALID : INVALID; }

    p_value get()                  { return  ptr_; }
    P_value get() const            { return  ptr_; }

    p_value operator->()           { return  ptr_; }
    P_value operator->() const     { return  ptr_; }
    r_value operator* ()           { return *ptr_; }
    R_value operator* () const     { return *ptr_; }

  private:
    p_value ptr_ = nullptr;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG, typename D = t_no_deleter>
  class t_cptr {
  public:
    using t_value = typename t_prefix<T>::t_;
    using P_value = typename t_prefix<T>::P_;
    using R_value = typename t_prefix<T>::R_;
    using r_cptr  = typename t_prefix<t_cptr>::r_;
    using x_cptr  = typename t_prefix<t_cptr>::x_;

     t_cptr() = default;
     t_cptr(P_value value) : ptr_{value}        { }
     t_cptr(x_cptr ptr) : ptr_{reset(ptr.ptr_)} { }
    ~t_cptr();

    r_cptr operator=(P_value);
    r_cptr operator=(x_cptr);

    P_value release()           { return reset(ptr_); }
    t_void  clear();

    operator t_validity() const { return ptr_ ? VALID : INVALID; }

    P_value get() const         { return  ptr_; }

    P_value operator->() const  { return  ptr_; }
    R_value operator* () const  { return *ptr_; }

  private:
    P_value ptr_ = nullptr;
  };

  template<typename T, typename TAG>
  class t_cptr<T, TAG, t_deleter> {
  public:
    using t_value = typename t_prefix<T>::t_;
    using P_value = typename t_prefix<T>::P_;
    using R_value = typename t_prefix<T>::R_;
    using r_cptr  = typename t_prefix<t_cptr>::r_;
    using x_cptr  = typename t_prefix<t_cptr>::x_;

     t_cptr() = default;
     t_cptr(P_value value) : ptr_{value}        { }
     t_cptr(x_cptr ptr) : ptr_{reset(ptr.ptr_)} { }
    ~t_cptr();

    r_cptr operator=(P_value);
    r_cptr operator=(x_cptr);

    P_value release()           { return reset(ptr_); }
    t_void  clear();

    operator t_validity() const { return ptr_ ? VALID : INVALID; }

    P_value get() const         { return  ptr_; }

    P_value operator->() const  { return  ptr_; }
    R_value operator* () const  { return *ptr_; }

  private:
    P_value ptr_ = nullptr;
  };

  template<typename T, typename TAG>
  class t_cptr<T, TAG, t_no_deleter> {
  public:
    using t_value = typename t_prefix<T>::t_;
    using P_value = typename t_prefix<T>::P_;
    using R_value = typename t_prefix<T>::R_;
    using r_cptr  = typename t_prefix<t_cptr>::r_;
    using R_cptr  = typename t_prefix<t_cptr>::R_;

    t_cptr() = default;
    t_cptr(P_value value) : ptr_{value} { }
    t_cptr(R_cptr ptr) : ptr_{ptr.ptr_} { }

    r_cptr operator=(P_value value)     { ptr_ = value;    return *this; }
    r_cptr operator=(R_cptr ptr)        { ptr_ = ptr.ptr_; return *this; }

    P_value release()                   { return reset(ptr_); }
    t_void  clear()                     { reset(ptr_); }

    operator t_validity() const         { return ptr_ ? VALID : INVALID; }

    P_value get() const                 { return  ptr_; }

    P_value operator->() const          { return  ptr_; }
    R_value operator* () const          { return *ptr_; }

  private:
    P_value ptr_ = nullptr;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG, typename D>
  class t_ptr<T[], TAG, D> {
  public:
    using t_value = typename t_prefix<T>::t_;
    using p_value = typename t_prefix<T>::p_;
    using P_value = typename t_prefix<T>::P_;
    using r_value = typename t_prefix<T>::r_;
    using R_value = typename t_prefix<T>::R_;
    using r_ptr   = typename t_prefix<t_ptr>::r_;
    using x_ptr   = typename t_prefix<t_ptr>::x_;

     t_ptr() = default;
     t_ptr(p_value value) : ptr_{value}           { }
     t_ptr(x_ptr ptr)     : ptr_{reset(ptr.ptr_)} { }
    ~t_ptr();

    r_ptr operator=(p_value);
    r_ptr operator=(x_ptr);

    p_value release()                 { return reset(ptr_); }
    t_void  clear();

    operator t_validity() const       { return ptr_ ? VALID : INVALID; }

    p_value get()                     { return  ptr_; }
    P_value get() const               { return  ptr_; }

    r_value operator[](t_ix ix)       { return  ptr_[get(ix)]; }
    R_value operator[](t_ix ix) const { return  ptr_[get(ix)]; }

  private:
    p_value ptr_ = nullptr;
  };

  template<typename T, typename TAG>
  class t_ptr<T[], TAG, t_deleter> {
  public:
    using t_value = typename t_prefix<T>::t_;
    using p_value = typename t_prefix<T>::p_;
    using P_value = typename t_prefix<T>::P_;
    using r_value = typename t_prefix<T>::r_;
    using R_value = typename t_prefix<T>::R_;
    using r_ptr   = typename t_prefix<t_ptr>::r_;
    using x_ptr   = typename t_prefix<t_ptr>::x_;

     t_ptr() = default;
     t_ptr(p_value value) : ptr_{value}           { }
     t_ptr(x_ptr ptr)     : ptr_{reset(ptr.ptr_)} { }
    ~t_ptr();

    r_ptr operator=(p_value);
    r_ptr operator=(x_ptr);

    p_value release()                 { return reset(ptr_); }
    t_void  clear();

    operator t_validity() const       { return ptr_ ? VALID : INVALID; }

    p_value get()                     { return  ptr_; }
    P_value get() const               { return  ptr_; }

    r_value operator[](t_ix ix)       { return  ptr_[named::get(ix)]; }
    R_value operator[](t_ix ix) const { return  ptr_[named::get(ix)]; }

  private:
    p_value ptr_ = nullptr;
  };

  template<typename T, typename TAG>
  class t_ptr<T[], TAG, t_no_deleter> {
  public:
    using t_value = typename t_prefix<T>::t_;
    using p_value = typename t_prefix<T>::p_;
    using P_value = typename t_prefix<T>::P_;
    using r_value = typename t_prefix<T>::r_;
    using R_value = typename t_prefix<T>::R_;
    using r_ptr   = typename t_prefix<t_ptr>::r_;
    using R_ptr   = typename t_prefix<t_ptr>::R_;

    t_ptr() = default;
    t_ptr(p_value value) : ptr_{value}    { }
    t_ptr(R_ptr ptr)     : ptr_{ptr.ptr_} { }

    r_ptr operator=(p_value value)    { ptr_ = value;    return *this; }
    r_ptr operator=(R_ptr ptr)        { ptr_ = ptr.ptr_; return *this; }

    p_value release()                 { return reset(ptr_); }
    t_void  clear()                   { reset(ptr_); }

    operator t_validity() const       { return ptr_ ? VALID : INVALID; }

    p_value get()                     { return  ptr_; }
    P_value get() const               { return  ptr_; }

    r_value operator[](t_ix ix)       { return  ptr_[named::get(ix)]; }
    R_value operator[](t_ix ix) const { return  ptr_[named::get(ix)]; }

  private:
    p_value ptr_ = nullptr;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG, typename D>
  class t_cptr<T[], TAG, D> {
  public:
    using t_value = typename t_prefix<T>::t_;
    using P_value = typename t_prefix<T>::P_;
    using R_value = typename t_prefix<T>::R_;
    using r_cptr  = typename t_prefix<t_cptr>::r_;
    using x_cptr  = typename t_prefix<t_cptr>::x_;

     t_cptr() = default;
     t_cptr(P_value value) : ptr_{value}           { }
     t_cptr(x_cptr ptr)    : ptr_{reset(ptr.ptr_)} { }
    ~t_cptr();

    r_cptr operator=(P_value);
    r_cptr operator=(x_cptr);

    P_value release()                 { return reset(ptr_); }
    t_void  clear();

    operator t_validity() const       { return ptr_ ? VALID : INVALID; }

    P_value get() const               { return  ptr_; }

    R_value operator[](t_ix ix) const { return  ptr_[named::get(ix)]; }

  private:
    P_value ptr_ = nullptr;
  };

  template<typename T, typename TAG>
  class t_cptr<T[], TAG, t_deleter> {
  public:
    using t_value = typename t_prefix<T>::t_;
    using P_value = typename t_prefix<T>::P_;
    using R_value = typename t_prefix<T>::R_;
    using r_cptr  = typename t_prefix<t_cptr>::r_;
    using x_cptr  = typename t_prefix<t_cptr>::x_;

     t_cptr() = default;
     t_cptr(P_value value) : ptr_{value}           { }
     t_cptr(x_cptr ptr)    : ptr_{reset(ptr.ptr_)} { }
    ~t_cptr();

    r_cptr operator=(P_value);
    r_cptr operator=(x_cptr);

    P_value release()                 { return reset(ptr_); }
    t_void  clear();

    operator t_validity() const       { return ptr_ ? VALID : INVALID; }

    P_value get() const               { return  ptr_; }

    R_value operator[](t_ix ix) const { return  ptr_[get(ix)]; }

  private:
    P_value ptr_ = nullptr;
  };

  template<typename T, typename TAG>
  class t_cptr<T[], TAG, t_no_deleter> {
  public:
    using t_value = typename t_prefix<T>::t_;
    using P_value = typename t_prefix<T>::P_;
    using R_value = typename t_prefix<T>::R_;
    using r_cptr  = typename t_prefix<t_cptr>::r_;
    using R_cptr  = typename t_prefix<t_cptr>::R_;

    t_cptr() = default;
    t_cptr(P_value value) : ptr_{value}    { }
    t_cptr(R_cptr ptr)    : ptr_{ptr.ptr_} { }

    r_cptr operator=(P_value value)   { ptr_ = value;    return *this; }
    r_cptr operator=(R_cptr ptr)      { ptr_ = ptr.ptr_; return *this; }

    P_value release()                 { return reset(ptr_); }
    t_void  clear()                   { reset(ptr_); }

    operator t_validity() const       { return ptr_ ? VALID : INVALID; }

    P_value get() const               { return  ptr_; }

    R_value operator[](t_ix ix) const { return  ptr_[get(ix)]; }

  private:
    P_value ptr_ = nullptr;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG, typename D>
  inline
  t_ptr<T, TAG, D>::~t_ptr() {
    clear();
  }

  template<typename T, typename TAG, typename D>
  inline
  typename t_ptr<T, TAG, D>::r_ptr
      t_ptr<T, TAG, D>::operator=(p_value value) {
    if (ptr_) {
      D d;
      d(reset(ptr_, value));
    } else
      ptr_ = value;
    return *this;
  }

  template<typename T, typename TAG, typename D>
  inline
  typename t_ptr<T, TAG, D>::r_ptr t_ptr<T, TAG, D>::operator=(x_ptr ptr) {
    if (ptr_) {
      D d;
      d(reset(ptr_, reset(ptr.ptr_)));
    } else
      ptr_ = reset(ptr.ptr_);
    return *this;
  }

  template<typename T, typename TAG, typename D>
  inline
  t_void t_ptr<T, TAG, D>::clear() {
    if (ptr_) {
      D d;
      d(reset(ptr_));
    }
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
      delete reset(ptr_, value);
    else
      ptr_ = value;
    return *this;
  }

  template<typename T, typename TAG>
  inline
  typename t_ptr<T, TAG, t_deleter>::r_ptr
      t_ptr<T, TAG, t_deleter>::operator=(x_ptr ptr) {
    if (ptr_)
      delete reset(ptr_, reset(ptr.ptr_));
    else
      ptr_ = reset(ptr.ptr_);
    return *this;
  }

  template<typename T, typename TAG>
  inline
  t_void t_ptr<T, TAG, t_deleter>::clear() {
    if (ptr_)
      delete reset(ptr_);
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
    if (ptr_) {
      D d;
      d(reset(ptr_, value));
    } else
      ptr_ = value;
    return *this;
  }

  template<typename T, typename TAG, typename D>
  inline
  typename t_cptr<T, TAG, D>::r_cptr
      t_cptr<T, TAG, D>::operator=(x_cptr ptr) {
    if (ptr_) {
      D d;
      d(reset(ptr_, reset(ptr.ptr_)));
    } else
      ptr_ = reset(ptr.ptr_);
    return *this;
  }

  template<typename T, typename TAG, typename D>
  inline
  t_void t_cptr<T, TAG, D>::clear() {
    if (ptr_) {
      D d;
      d(reset(ptr_));
    }
  }

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
      delete reset(ptr_, value);
    else
      ptr_ = value;
    return *this;
  }

  template<typename T, typename TAG>
  inline
  typename t_cptr<T, TAG, t_deleter>::r_cptr
      t_cptr<T, TAG, t_deleter>::operator=(x_cptr ptr) {
    if (ptr_)
      delete reset(ptr_, reset(ptr.ptr_));
    else
      ptr_ = reset(ptr.ptr_);
    return *this;
  }

  template<typename T, typename TAG>
  inline
  t_void t_cptr<T, TAG, t_deleter>::clear() {
    if (ptr_)
      delete reset(ptr_);
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
    if (ptr_) {
      D d;
      d(reset(ptr_, value));
    } else
      ptr_ = value;
    return *this;
  }

  template<typename T, typename TAG, typename D>
  inline
  typename t_ptr<T[], TAG, D>::r_ptr t_ptr<T[], TAG, D>::operator=(x_ptr ptr) {
    if (ptr_) {
      D d;
      d(reset(ptr_, reset(ptr.ptr_)));
    } else
      ptr_ = reset(ptr.ptr_);
    return *this;
  }

  template<typename T, typename TAG, typename D>
  inline
  t_void t_ptr<T[], TAG, D>::clear() {
    if (ptr_) {
      D d;
      d(reset(ptr_));
    }
  }

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
      delete [] reset(ptr_, value);
    else
      ptr_ = value;
    return *this;
  }

  template<typename T, typename TAG>
  inline
  typename t_ptr<T[], TAG, t_deleter>::r_ptr
      t_ptr<T[], TAG, t_deleter>::operator=(x_ptr ptr) {
    if (ptr_)
      delete [] reset(ptr_, reset(ptr.ptr_));
    else
      ptr_ = reset(ptr.ptr_);
    return *this;
  }

  template<typename T, typename TAG>
  inline
  t_void t_ptr<T[], TAG, t_deleter>::clear() {
    if (ptr_)
      delete [] reset(ptr_);
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
    if (ptr_) {
      D d;
      d(reset(ptr_, value));
    } else
      ptr_ = value;
    return *this;
  }

  template<typename T, typename TAG, typename D>
  inline
  typename t_cptr<T[], TAG, D>::r_cptr
      t_cptr<T[], TAG, D>::operator=(x_cptr ptr) {
    if (ptr_) {
      D d;
      d(reset(ptr_, reset(ptr.ptr_)));
    } else
      ptr_ = reset(ptr.ptr_);
    return *this;
  }

  template<typename T, typename TAG, typename D>
  inline
  t_void t_cptr<T[], TAG, D>::clear() {
    if (ptr_) {
      D d;
      d(reset(ptr_));
    }
  }

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
      delete [] reset(ptr_, value);
    else
      ptr_ = value;
    return *this;
  }

  template<typename T, typename TAG>
  inline
  typename t_cptr<T[], TAG, t_deleter>::r_cptr
      t_cptr<T[], TAG, t_deleter>::operator=(x_cptr ptr) {
    if (ptr_)
      delete [] reset(ptr_, reset(ptr.ptr_));
    else
      ptr_ = reset(ptr.ptr_);
    return *this;
  }

  template<typename T, typename TAG>
  inline
  t_void t_cptr<T[], TAG, t_deleter>::clear() {
    if (ptr_)
      delete [] reset(ptr_);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
