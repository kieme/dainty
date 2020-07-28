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

#ifndef _DAINTY_BASE_UTILITY_H_
#define _DAINTY_BASE_UTILITY_H_

#include "dainty_base_types.h"
#include "dainty_base_traits.h"
#include "dainty_base_specific.h"

namespace dainty
{
namespace base
{
namespace utility
{
///////////////////////////////////////////////////////////////////////////////

  template<typename TAG>
  struct t_user {
    union {
      types::t_int64 id;
      types::p_void  ptr;
      types::P_void  cptr;
      types::t_char  buf[sizeof(types::t_int64)]; // 8 bytes
    };
    constexpr t_user()                    noexcept : id  {0L}    { }
    constexpr t_user(types::t_int64  _id) noexcept : id  {_id}   { }
    constexpr t_user(types::p_void  _ptr) noexcept : ptr {_ptr}  { }
    constexpr t_user(types::P_void _cptr) noexcept : cptr{_cptr} { }
  };

  template<typename TAG>
  constexpr
  types::t_bool operator==(const t_user<TAG>& lh,
                           const t_user<TAG>& rh) noexcept {
    return lh.id == rh.id;
  }

  template<typename TAG>
  constexpr
  types::t_bool operator!=(const t_user<TAG>& lh,
                           const t_user<TAG>& rh) noexcept {
    return lh.id != rh.id;
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG>
  struct t_block {
    using t_value = T;

    t_value       value;
    specific::t_n n = specific::t_n{0};

    constexpr t_block() noexcept = default;
    constexpr t_block(t_value _value, specific::t_n _n) noexcept
      : value{_value}, n{_n} {
    }
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG, T INIT_VALUE,
                       typename U = types::t_uint>
  struct t_id_pair {
    using t_value   = specific::t_specific<T, TAG>;
    using t_user_no = specific::t_specific<U, TAG>;

    constexpr static t_user_no BAD_USER_NO{0};

    constexpr t_id_pair() noexcept : value{INIT_VALUE}, user_no{BAD_USER_NO} {
    }

    constexpr explicit t_id_pair(t_value _value, t_user_no _user_no) noexcept
      : value{_value}, user_no{_user_no} {
    }

    constexpr operator specific::t_validity() const noexcept {
      return user_no != BAD_USER_NO ? specific::VALID : specific::INVALID;
    }

    constexpr t_id_pair release() {
      t_id_pair tmp = *this;
      value    = t_value {INIT_VALUE};
      user_no  = BAD_USER_NO;
      return tmp;
    }

    t_value   value;
    t_user_no user_no;
  };

  template<typename T, typename TAG, T BAD_VALUE>
  constexpr types::t_bool operator==(const t_id_pair<T, TAG, BAD_VALUE> lh,
                                     const t_id_pair<T, TAG, BAD_VALUE> rh) {
    return get(lh.user_no) == get(rh.user_no) &&
           get(lh.value)   == get(rh.value);
  }

  template<typename T, typename TAG, T BAD_VALUE>
  constexpr types::t_bool operator!=(const t_id_pair<T, TAG, BAD_VALUE> lh,
                                     const t_id_pair<T, TAG, BAD_VALUE> rh) {
    return !(lh == rh);
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  class t_verifiable;

  template<typename T, typename TAG, typename CHECK>
  class t_verifiable<specific::t_specific<T, TAG, CHECK>> {
  public:
    using t_value = specific::t_specific<T, TAG, CHECK>;
    using T_value = typename types::t_prefix<t_value>::T_;

    constexpr t_verifiable(t_value _value, specific::t_errn _errn) noexcept
      : value{_value}, errn{_errn} {
    }

    constexpr operator specific::t_validity() const noexcept {
      return errn == specific::NO_ERRN ? specific::VALID : specific::INVALID;
    }

    constexpr operator t_value () const noexcept {
      return value;
    }

    T_value value;
    specific::t_errn  errn;
  };

  template<typename T>
  constexpr const T& get(const t_verifiable<T>& verifiable) noexcept {
    return verifiable.value;
  }

  template<class T, class TAG, class CHECK>
  constexpr
  T get(const t_verifiable<specific::t_specific<T, TAG, CHECK>>& verifiable)
      noexcept {
    return get(verifiable.value);
  }

///////////////////////////////////////////////////////////////////////////////

  enum t_emplace_it { EMPLACE_IT  }; // XXX TRY_EMPLACE ?
  enum t_fmt        { FMT, FMT_IT }; // XXX FMT is deprecated
  enum t_fmt_va     { FMT_VA_IT   };

///////////////////////////////////////////////////////////////////////////////

  template<types::t_n_ N, typename TAG>
  class t_multiple {
  public:
    constexpr static specific::t_n of() noexcept {
      return specific::t_n{N};
    }
    constexpr t_multiple(types::t_n_ _value) noexcept : value(_value) {
    } // XXX

    specific::t_n value;
  };

  template<types::t_n_ N, typename TAG>
  constexpr specific::t_n multiple_of(t_multiple<N, TAG> multiple) {
    return specific::t_n{N*get(multiple.value)};
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename TAG>
  class t_void_size_ptr {
  public:
    using r_void_size_ptr = typename types::t_prefix<t_void_size_ptr<TAG>>::r_;

    constexpr t_void_size_ptr() noexcept = default;

    constexpr
    t_void_size_ptr(types::p_void _ptr, specific::t_n _n) noexcept
      : ptr{_ptr}, n{_n} {
    }

    template<typename T>
    constexpr t_void_size_ptr(T* _ptr) noexcept : ptr{_ptr}, n{sizeof(T)} {
    }

    template<typename T>
    constexpr r_void_size_ptr operator=(T* _ptr) noexcept {
      ptr = _ptr;
      n   = specific::t_n{sizeof(T)};
      return *this;
    }

    constexpr operator specific::t_validity() const noexcept {
      return ptr ? specific::VALID : specific::INVALID;
    }

    types::p_void ptr = nullptr;
    specific::t_n n   = specific::t_n{0};
  };

  template<typename TAG>
  class t_void_size_cptr { // t_void_sized_ptr ?
  public:
    using r_void_size_cptr = typename types::t_prefix<t_void_size_cptr<TAG>>::r_;
    using R_void_size_ptr  = typename types::t_prefix<t_void_size_ptr<TAG>>::R_;

    constexpr t_void_size_cptr() noexcept = default;

    constexpr
    t_void_size_cptr(types::P_void _ptr, specific::t_n _n) noexcept
      : ptr{_ptr}, n{_n} {
    }

    template<typename T>
    constexpr
    t_void_size_cptr(T* _ptr) noexcept : ptr{_ptr}, n{sizeof(T)} {
    }

    constexpr t_void_size_cptr(R_void_size_ptr _ptr) noexcept
      : ptr{_ptr.ptr}, n{_ptr.n} {
    }

    constexpr
    r_void_size_cptr operator=(R_void_size_ptr _ptr) noexcept {
      ptr = _ptr.ptr;
      n   = _ptr.n;
      return *this;
    }

    template<typename T>
    constexpr r_void_size_cptr operator=(T* _ptr) noexcept {
      ptr = _ptr;
      n   = specific::t_n{sizeof(T)};
      return *this;
    }

    constexpr operator specific::t_validity() const noexcept {
      return ptr ? specific::VALID : specific::INVALID;
    }

    types::P_void ptr = nullptr;
    specific::t_n n   = specific::t_n{0};
  };

///////////////////////////////////////////////////////////////////////////////

  constexpr types::t_bool reset(types::r_bool ref,
                                types::t_bool value) noexcept {
    types::t_bool tmp = ref;
    ref = value;
    return tmp;
  }

  template<class T>
  constexpr T reset(T& ref, T value) noexcept {
    T tmp = ref;
    ref = value;
    return tmp;
  }

  template<class T>
  constexpr T* reset(T*& ref, T* value) noexcept {
    T* tmp = ref;
    ref = value;
    return tmp;
  }

  template<class T, class TAG>
  constexpr specific::t_specific<T, TAG>
      reset(specific::t_specific<T, TAG>& ref, T value) noexcept {
    specific::t_specific<T, TAG> tmp{ref};
    ref = specific::t_specific<T, TAG>{value};
    return tmp;
  }

  template<class T, class TAG>
  constexpr specific::t_specific<T, TAG>
      reset(specific::t_specific<T, TAG>& ref,
            specific::t_specific<T, TAG>  value) noexcept {
    specific::t_specific<T, TAG> tmp{ref};
    ref = value;
    return tmp;
  }

  template<class TAG>
  constexpr t_user<TAG> reset(t_user<TAG>& ref,
                              types::t_int64 value) noexcept {
    return t_user<TAG>{reset(ref.id, value)};
  }

///////////////////////////////////////////////////////////////////////////////

  constexpr types::t_bool reset(types::r_bool ref) noexcept {
    types::t_bool tmp = ref;
    ref = false;
    return tmp;
  }

  template<class T>
  constexpr T reset(T& ref) noexcept {
    T tmp = ref;
    ref = 0;
    return tmp;
  }

  template<class T>
  constexpr T* reset(T*& ref) noexcept {
    T* tmp = ref;
    ref = nullptr;
    return tmp;
  }

  constexpr specific::t_fd reset(specific::r_fd fd) noexcept {
    return reset(fd, specific::BAD_FD);
  }

  template<class T, class TAG>
  constexpr specific::t_specific<T, TAG>
      reset(specific::t_specific<T, TAG>& ref) noexcept {
    specific::t_specific<T, TAG> tmp{ref};
    ref = specific::t_specific<T, TAG>{0};
    return tmp;
  }

  template<class T, class TAG>
  constexpr specific::t_specific<T*, TAG>
      reset(specific::t_specific<T*, TAG>& ref) noexcept {
    specific::t_specific<T*, TAG> tmp{ref};
    ref = specific::t_specific<T*, TAG>{nullptr};
    return tmp;
  }

  template<class TAG>
  constexpr specific::t_specific<types::t_bool, TAG>
      reset(specific::t_specific<types::t_bool, TAG>& ref) noexcept {
    specific::t_specific<types::t_bool, TAG> tmp{ref};
    ref = specific::t_specific<types::t_bool, TAG>{false};
    return tmp;
  }

  template<class TAG>
  constexpr t_user<TAG> reset(t_user<TAG>& ref) noexcept {
    return t_user<TAG>{reset(ref.id)};
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  constexpr auto x_cast(T&& in)
      -> typename traits::t_remove_ref<T>::t_result&& {
    return static_cast<typename traits::t_remove_ref<T>::t_result&&>(in);
  }

  template<typename T>
  constexpr T&& preserve(typename traits::t_remove_ref<T>::t_result& in) {
    return static_cast<T&&>(in);
  }

  template<typename T>
  constexpr T&& preserve(typename traits::t_remove_ref<T>::t_result&& in) {
    return x_cast(in);
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  inline
  types::t_void swap(T& lh, T& rh) {
    T tmp {x_cast(lh)};
    lh = x_cast(rh);
    rh = x_cast(tmp);
  }

///////////////////////////////////////////////////////////////////////////////

  /* Not sure why it does work when using it with literal arguments
  template<typename T, typename T1>
  constexpr
  auto py_or(T&& first, T1&& second) noexcept {
    if constexpr (first != 0)
      return preserve<T>(first);
    else
      return preserve<T1>(second);
  }

  template<typename T>
  constexpr
  auto py_or(T&& first, T&& second) noexcept {
    if (first)
      return preserve<T>(first);
    return preserve<T>(second);
  }
  */

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
