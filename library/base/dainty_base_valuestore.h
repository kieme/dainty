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

#ifndef _DAINTY_BASE_VALUESTORE_H_
#define _DAINTY_BASE_VALUESTORE_H_

#include <new>
#include "dainty_base.h"

namespace dainty
{
namespace base
{
namespace valuestore
{
///////////////////////////////////////////////////////////////////////////////

  using t_alignment = t_int;

  template<t_n_ N, t_alignment ALIGNMENT>
  struct t_store {
    alignas(ALIGNMENT) base::t_byte_ bytes[N];
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  inline
  T* default_construct(t_store<sizeof(T), alignof(T)>& store) noexcept {
    return new (store.bytes) T;
  }

  template<typename T>
  inline
  T* copy_construct(t_store<sizeof(T), alignof(T)>& store,
                    const T& value) noexcept {
    return new (store.bytes) T(value);
  }

  template<typename T>
  inline
  T* move_construct(t_store<sizeof(T), alignof(T)>& store,
                    T&& value) noexcept {
    return new (store.bytes) T(x_cast(value));
  }

  template<typename T, typename... Args>
  inline
  T* emplace_construct(t_store<sizeof(T), alignof(T)>& store,
                       Args&&... args) noexcept {
    return new (store.bytes) T(preserve<Args>(args)...);
  }

  template<typename T>
  inline
  t_void destruct(t_store<sizeof(T), alignof(T)>& store) noexcept {
    reinterpret_cast<T*>(store.bytes)->~T();
  }

  template<typename T>
  constexpr
  T* get_ptr(t_store<sizeof(T), alignof(T)>& store) noexcept {
    return reinterpret_cast<T*>(store.bytes);
  }

  template<typename T>
  constexpr
  const T* get_cptr(const t_store<sizeof(T), alignof(T)>& store) noexcept {
    return reinterpret_cast<const T*>(store.bytes);
  }

  template<typename T>
  constexpr
  T& get_ref(t_store<sizeof(T), alignof(T)>& store) noexcept {
    return *get_ptr(store);
  }

  template<typename T>
  constexpr
  const T& get_cref(const t_store<sizeof(T), alignof(T)>& store) noexcept {
    return *get_cptr(store);
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  class t_valuestore {
  public:
    using t_store = valuestore::t_store<sizeof(T), alignof(T)>;
    using r_store = typename base::t_prefix<t_store>::r_;
    using R_store = typename base::t_prefix<t_store>::R_;
    using t_value = typename base::t_prefix<T>::t_;
    using r_value = typename base::t_prefix<T>::r_;
    using R_value = typename base::t_prefix<T>::R_;
    using p_value = typename base::t_prefix<T>::p_;
    using P_value = typename base::t_prefix<T>::P_;
    using x_value = typename base::t_prefix<T>::x_;

    constexpr static t_n get_size() noexcept {
      return t_n{sizeof(T)};
    }

    t_valuestore()                               = default;
    t_valuestore(const t_valuestore&)            = delete;
    t_valuestore& operator=(const t_valuestore&) = delete;
    t_valuestore(t_valuestore&&)                 = delete;
    t_valuestore& operator=(t_valuestore&&)      = delete;

    inline
    p_value default_construct() noexcept {
      return valuestore::default_construct<T>(store);
    }

    inline
    p_value copy_construct(R_value value) noexcept {
      return valuestore::copy_construct<T>(store, value);
    }

    inline
    p_value move_construct(x_value value) noexcept {
      return valuestore::move_construct<T>(store, x_cast(value));
    }

    template<typename... Args>
    inline
    p_value emplace_construct(Args&&... args) noexcept {
      return valuestore::emplace_construct<T>(store, preserve<Args>(args)...);
    }

    inline
    t_void destruct() noexcept {
      valuestore::destruct<T>(store);
    }

    constexpr p_value  ptr()         noexcept { return mk_ptr(); }
    constexpr P_value  ptr() const   noexcept { return mk_ptr(); }
    constexpr P_value cptr() const   noexcept { return mk_ptr(); }

    constexpr r_value  ref()         noexcept { return  *ptr(); }
    constexpr R_value  ref() const   noexcept { return  *ptr(); }
    constexpr R_value cref() const   noexcept { return *cptr(); }

    constexpr r_store  store()       noexcept { return store_; }
    constexpr R_store  store() const noexcept { return store_; }
    constexpr R_store cstore() const noexcept { return store_; }

  private:
    constexpr
    p_value mk_ptr() noexcept {
      return valuestore::get_ptr<T>(store_);
    }

    constexpr
    P_value mk_ptr() const noexcept {
      return valuestore::get_cptr<T>(store_);
    }

    t_store store_;
  };

////////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
