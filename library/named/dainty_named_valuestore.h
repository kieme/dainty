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

#ifndef _DAINTY_NAMED_VALUESTORE_H_
#define _DAINTY_NAMED_VALUESTORE_H_

#include "dainty_named.h"

namespace dainty
{
namespace named
{
namespace valuestore
{
///////////////////////////////////////////////////////////////////////////////

  template<t_n_ N>
  struct t_store { named::t_byte_ bytes[N]; };

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  class t_valuestore {
  public:
    using t_store = valuestore::t_store<sizeof(T)>;
    using r_store = typename named::t_prefix<t_store>::r_;
    using R_store = typename named::t_prefix<t_store>::R_;
    using t_value = typename named::t_prefix<T>::t_;
    using r_value = typename named::t_prefix<T>::r_;
    using R_value = typename named::t_prefix<T>::R_;
    using p_value = typename named::t_prefix<T>::p_;
    using P_value = typename named::t_prefix<T>::P_;
    using x_value = typename named::t_prefix<T>::x_;

    constexpr static t_n get_size() noexcept {
      return t_n{sizeof(T)};
    }

    template<typename U>
    constexpr void* operator new(__SIZE_TYPE__, U* some_where) {
      return some_where;
    }

    t_valuestore()                               = default;
    t_valuestore(const t_valuestore&)            = delete;
    t_valuestore& operator=(const t_valuestore&) = delete;
    t_valuestore(t_valuestore&&)                 = delete;
    t_valuestore& operator=(t_valuestore&&)      = delete;

    inline
    p_value default_construct() noexcept {
      return new (ptr()) T;
    }

    inline
    p_value copy_construct(R_value value) noexcept {
      return new (ptr()) T(value);
    }

    inline
    p_value move_construct(x_value value) noexcept {
      return new (ptr()) T(named::x_cast(value));
    }

    template<typename... Args>
    inline
    p_value emplace_construct(Args&&... args) noexcept {
      return new (ptr()) T(named::preserve<Args>(args)...);
    }

    inline
    t_void destruct() noexcept {
      ptr()->~T();
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
    inline
    p_value mk_ptr() noexcept {
      return reinterpret_cast<p_value>(store_.bytes);
    }

    inline
    P_value mk_ptr() const noexcept {
      return reinterpret_cast<P_value>(store_.bytes);
    }

    alignas(t_value) t_store store_;
  };

////////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
