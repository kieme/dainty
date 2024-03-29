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

#ifndef _DAINTY_CONTAINER_VALUESTORE_H_
#define _DAINTY_CONTAINER_VALUESTORE_H_

#include <memory>
#include "dainty_container_bytebuf.h"

namespace dainty
{
namespace container
{
namespace valuestore
{
///////////////////////////////////////////////////////////////////////////////

  using base::types::t_prefix;
  using base::types::t_void;
  using base::types::t_uchar;

  using base::specific::t_n;

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  inline
  T* default_construct_(T* ptr) {
    return new (ptr) T;
  }

  template<typename T>
  inline
  T* copy_construct_(T* ptr, const T& value) {
    return new (ptr) T(value);
  }

  template<typename T>
  inline
  T* move_construct_(T* ptr, T&& value) {
    return new (ptr) T(base::x_cast(value));
  }

  template<typename T, typename... Args>
  inline
  T* emplace_construct_(T* ptr, Args&&... args) {
    return new (ptr) T(base::f_cast<Args>(args)...);
  }

  template<typename T>
  inline
  void destruct_(T* ptr) {
    ptr->~T();
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  class t_valuestore {
    enum t_bytes_tag_ { };
  public:
    using t_bytes = bytebuf::t_bytebuf<t_bytes_tag_, sizeof(T)>;
    using r_bytes = typename t_prefix<t_bytes>::r_;
    using R_bytes = typename t_prefix<t_bytes>::R_;
    using t_value = typename t_prefix<T>::t_;
    using r_value = typename t_prefix<T>::r_;
    using R_value = typename t_prefix<T>::R_;
    using p_value = typename t_prefix<T>::p_;
    using P_value = typename t_prefix<T>::P_;
    using x_value = typename t_prefix<T>::x_;

    constexpr static t_n get_size() {
      return t_n{sizeof(T)};
    }

    t_valuestore() = default;

    inline
    p_value default_construct() {
      return default_construct_(ptr());
    }

    inline
    p_value copy_construct(R_value value) {
      return copy_construct_(ptr(), value);
    }

    inline
    p_value move_construct(x_value value) {
      return move_construct_(ptr(), base::x_cast(value));
    }

    template<typename... Args>
    inline
    p_value emplace_construct(Args&&... args) {
      return emplace_construct_(ptr(), base::f_cast<Args>(args)...);
    }

    inline
    t_void destruct() {
      destruct_(ptr());
    }

    inline p_value  ptr()         { return mk_ptr(); }
    inline P_value  ptr() const   { return mk_ptr(); }
    inline P_value cptr() const   { return mk_ptr(); }

    inline r_value  ref()         { return  *ptr(); }
    inline R_value  ref() const   { return  *ptr(); }
    inline R_value cref() const   { return *cptr(); }

    inline r_bytes  bytes()       { return store_; }
    inline R_bytes  bytes() const { return store_; }
    inline R_bytes cbytes() const { return store_; }

  private:
    inline
    p_value mk_ptr() {
      return reinterpret_cast<p_value>(store_.ptr());
    }

    inline
    P_value mk_ptr() const {
      return reinterpret_cast<P_value>(store_.cptr());
    }

    alignas(t_value) t_bytes store_;
  };

////////////////////////////////////////////////////////////////////////////////

  template<typename T>
  inline
  t_void destruct_(t_valuestore<T>* store, t_n n) {
    t_n::t_value max = get(n);
    for (decltype(max) ix = 0; ix < max; ++ix)
      store[ix].destruct();
  }

////////////////////////////////////////////////////////////////////////////////

// XXX must be replace with type traits

#define DAINTY_CONTAINER_VALUESTORE_DESTRUCT_IMPL(type)  \
  inline                                                 \
  t_void destruct_(t_valuestore<type>*, t_n) {           \
  }

  DAINTY_CONTAINER_VALUESTORE_DESTRUCT_IMPL(base::types::t_bool)

  DAINTY_CONTAINER_VALUESTORE_DESTRUCT_IMPL(base::types::t_char)

  DAINTY_CONTAINER_VALUESTORE_DESTRUCT_IMPL(base::types::t_short)

  DAINTY_CONTAINER_VALUESTORE_DESTRUCT_IMPL(base::types::t_int)

  DAINTY_CONTAINER_VALUESTORE_DESTRUCT_IMPL(base::types::t_long)

  DAINTY_CONTAINER_VALUESTORE_DESTRUCT_IMPL(base::types::t_llong)

  DAINTY_CONTAINER_VALUESTORE_DESTRUCT_IMPL(base::types::t_uchar)

  DAINTY_CONTAINER_VALUESTORE_DESTRUCT_IMPL(base::types::t_ushort)

  DAINTY_CONTAINER_VALUESTORE_DESTRUCT_IMPL(base::types::t_uint)

  DAINTY_CONTAINER_VALUESTORE_DESTRUCT_IMPL(base::types::t_ulong)

  DAINTY_CONTAINER_VALUESTORE_DESTRUCT_IMPL(base::types::t_ullong)

////////////////////////////////////////////////////////////////////////////////

#define DAINTY_CONTAINER_VALUESTORE_IMPL(type)                    \
  inline                                                          \
  type* construct_(type* ptr) {                                   \
    *ptr = type();                                                \
     return ptr;                                                  \
  }                                                               \
                                                                  \
  inline                                                          \
  type* construct_(type* ptr, const type& value) {                \
    *ptr = value;                                                 \
    return ptr;                                                   \
  }                                                               \
                                                                  \
  inline                                                          \
  type* construct_(type* ptr, type&& value) {                     \
    *ptr = value;                                                 \
    return ptr;                                                   \
  }                                                               \
                                                                  \
  inline                                                          \
  t_void destruct_(type*) {                                       \
  }

  DAINTY_CONTAINER_VALUESTORE_IMPL(base::types::t_bool)

  DAINTY_CONTAINER_VALUESTORE_IMPL(base::types::t_char)

  DAINTY_CONTAINER_VALUESTORE_IMPL(base::types::t_short)

  DAINTY_CONTAINER_VALUESTORE_IMPL(base::types::t_int)

  DAINTY_CONTAINER_VALUESTORE_IMPL(base::types::t_long)

  DAINTY_CONTAINER_VALUESTORE_IMPL(base::types::t_llong)

  DAINTY_CONTAINER_VALUESTORE_IMPL(base::types::t_uchar)

  DAINTY_CONTAINER_VALUESTORE_IMPL(base::types::t_ushort)

  DAINTY_CONTAINER_VALUESTORE_IMPL(base::types::t_uint)

  DAINTY_CONTAINER_VALUESTORE_IMPL(base::types::t_ulong)

  DAINTY_CONTAINER_VALUESTORE_IMPL(base::types::t_ullong)

////////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
