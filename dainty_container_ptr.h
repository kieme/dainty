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

#ifndef _DAINTY_CONTIANER_PTR_H_
#define _DAINTY_CONTIANER_PTR_H_

#include "dainty_named.h"
#include "dainty_named_utility.h"

namespace dainty
{
namespace container
{
namespace ptr
{
  using named::t_void;
  using named::utility::reset;

  template<typename T>
  inline t_void default_deleter(T* t) {
    delete t;
  }

  template<typename T>
  struct t_passable_ptr {
  public:
    using t_deleter = t_void (*)(T*);

    t_passable_ptr() = default;

    inline
    t_passable_ptr(T* ptr, t_deleter deleter = default_deleter<T>)
      : ptr_(ptr), deleter_(deleter) {
    }

    t_passable_ptr(const t_passable_ptr&) = delete;

    inline
    t_passable_ptr(t_passable_ptr&& ptr)
      : ptr_(reset(ptr.ptr_)), deleter_(reset(ptr.deleter_)) {
    }

    inline
    ~t_passable_ptr() {
      if (deleter_ && ptr_)
        deleter_(ptr_);
    }

    inline
    t_passable_ptr& operator=(t_passable_ptr&& ptr) {
      if (deleter_ && ptr_)
        deleter_(ptr_);
      ptr_     = reset(ptr.ptr_);
      deleter_ = reset(ptr.deleter_);
      return *this;
    }

    t_passable_ptr& operator=(const t_passable_ptr&) = delete;

    inline
    operator bool() const       { return ptr_;  }

    inline
    T& operator*()              { return *ptr_; }

    inline
    T* operator->()             { return ptr_;  }

    inline
    const T& operator*()  const { return *ptr_; }

    inline
    const T* operator->() const { return ptr_;  }

  private:
    T*        ptr_     = nullptr;
    t_deleter deleter_ = nullptr;
  };

  template<typename T>
  struct t_passable_ptr<const T> {
  public:
    using t_deleter = t_void (*)(const T*);

    t_passable_ptr() = default;

    inline
    t_passable_ptr(const T* ptr, t_deleter func = default_deleter<const T>)
      : ptr_(ptr), deleter_(func) {
    }

    t_passable_ptr(const t_passable_ptr&) = delete;

    inline
    t_passable_ptr(t_passable_ptr&& ptr)
      : ptr_(reset(ptr.ptr_)), deleter_(reset(ptr.deleter_)) {
    }

    inline
    ~t_passable_ptr() {
      if (deleter_ && ptr_)
        deleter_(ptr_);
    }

    inline
    t_passable_ptr& operator=(t_passable_ptr&& ptr) {
      if (deleter_ && ptr_)
        deleter_(ptr_);
      ptr_     = reset(ptr.ptr_);
      deleter_ = reset(ptr.deleter_);
      return *this;
    }

    t_passable_ptr& operator=(const t_passable_ptr&) = delete;

    inline
    operator bool() const       { return ptr_;  }

    inline
    const T& operator*()  const { return *ptr_; }

    inline
    const T* operator->() const { return ptr_;  }

  private:
    const T*  ptr_     = nullptr;
    t_deleter deleter_ = nullptr;
  };

  template<typename T>        struct t_passable_ptr<T*>;
  template<typename T>        struct t_passable_ptr<T&>;
  template<typename T, int N> struct t_passable_ptr<T[N]>;
  template<typename T>        struct t_passable_ptr<T[]>;
  template<typename T>        struct t_passable_ptr<const T*>;
  template<typename T>        struct t_passable_ptr<const T&>;
  template<typename T, int N> struct t_passable_ptr<const T[N]>;
  template<typename T>        struct t_passable_ptr<const T[]>;
  template<typename T>        struct t_passable_ptr<T* const>;
}
}
}

#endif
