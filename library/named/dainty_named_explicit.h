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

#ifndef _DAINTY_NAMED_EXPLICIT_H_
#define _DAINTY_NAMED_EXPLICIT_H_

#include "dainty_named_types.h"
#include "dainty_named_traits.h"

namespace dainty
{
namespace named
{
///////////////////////////////////////////////////////////////////////////////

  template<typename T> struct t_explicit_value;

  template<> struct t_explicit_value<t_bool>   { using t_value = t_bool;   };
  template<> struct t_explicit_value<t_char>   { using t_value = t_char;   };
  template<> struct t_explicit_value<t_uchar>  { using t_value = t_uchar;  };
  template<> struct t_explicit_value<t_int>    { using t_value = t_int;    };
  template<> struct t_explicit_value<t_uint>   { using t_value = t_uint;   };
  template<> struct t_explicit_value<t_short>  { using t_value = t_short;  };
  template<> struct t_explicit_value<t_ushort> { using t_value = t_ushort; };
  template<> struct t_explicit_value<t_long>   { using t_value = t_long;   };
  template<> struct t_explicit_value<t_ulong>  { using t_value = t_ulong;  };
  template<> struct t_explicit_value<t_llong>  { using t_value = t_llong;  };
  template<> struct t_explicit_value<t_ullong> { using t_value = t_ullong; };
  template<> struct t_explicit_value<t_double> { using t_value = t_double; };
  template<> struct t_explicit_value<t_void>   { using t_value = t_void;   };

  template<typename T>
  struct t_explicit_value<const T> {
    using t_value = const typename t_explicit_value<T>::t_value;
  };

  template<typename T>
  struct t_explicit_value<T*> {
    using t_value = typename t_explicit_value<T>::t_value*;
  };

  template<typename T> struct t_explicit_value<T&>;
  template<typename T> struct t_explicit_value<T**>;

///////////////////////////////////////////////////////////////////////////////

  template<class, class, class> class t_explicit;
  template<class T, class TAG, class CHECK>
  constexpr T get(t_explicit<T, TAG, CHECK>) noexcept;

  template<class T, class TAG>
  struct t_check {
    using t_value = typename t_explicit_value<T>::t_value;
    using t_tag   = TAG;
    constexpr static t_value test(t_value value) { return value; }
  };

  template<class T, class TAG, class CHECK = void>
  class t_explicit {
  public:
    using t_value = typename t_explicit_value<T>::t_value;
    using t_tag   = TAG;
    using t_check = CHECK;

    constexpr
    explicit t_explicit(t_value value)
#ifndef DAINTY_EXPLICIT_CHECK
      : value_{value}
#else
      : value_{CHECK::test(value)}
#endif
    { }

    template<class T1, class CHECK1>
    constexpr
    explicit t_explicit(t_explicit<T1, TAG, CHECK1> value)
#ifndef DAINTY_EXPLICIT_CHECK
      : value_{value}
#else
      : value_{CHECK::test(value.value_)}
#endif
    { }

    template<class T1, class CHECK1>
    constexpr
    t_explicit& operator=(t_explicit<T1, TAG, CHECK1> value) {
#ifndef DAINTY_EXPLICIT_CHECK
      value_ = value;
#else
      value_ = CHECK::test(value.value_);
#endif
    }

    t_explicit() = delete;                              // for clarity
    t_explicit(const t_explicit&) = default;            // for clarity
    t_explicit& operator=(const t_explicit&) = default; // for clarity

  private:
    template<class T1, class TAG1, class CHECK1> friend class t_explicit;
    template<class T1, class TAG1, class CHECK1>
    friend constexpr T1  get(t_explicit<T1, TAG1, CHECK1>) noexcept;
    t_value value_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<class T, class TAG>
  class t_explicit<T, TAG, void> {
  public:
    using t_value = typename t_explicit_value<T>::t_value;
    using t_tag   = TAG;
    using t_check = void;

    constexpr
    explicit t_explicit(t_value value) noexcept : value_{value} {
    }

    template<class T1, class CHECK1>
    constexpr
    explicit t_explicit(t_explicit<T1, TAG, CHECK1> value) noexcept
      : value_{value} {
    }

    template<class T1, class CHECK1>
    constexpr
    t_explicit& operator=(t_explicit<T1, TAG, CHECK1> value) noexcept {
      value_ = value;
      return *this;
    }

    t_explicit() = delete;                              // for clarity
    t_explicit(const t_explicit&) = default;            // for clarity
    t_explicit& operator=(const t_explicit&) = default; // for clarity

  private:
    template<class T1, class TAG1, class CHECK1> friend class t_explicit;
    template<class T1, class TAG1, class CHECK1>
    friend constexpr T1 get(t_explicit<T1, TAG1, CHECK1>) noexcept;
    t_value value_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<class T, class TAG, class CHECK>
  constexpr
  T get(t_explicit<T, TAG, CHECK> value) noexcept { return value.value_; }

///////////////////////////////////////////////////////////////////////////////

  template<class T, class TAG, class CHECK>
  constexpr
  t_bool operator==(t_explicit<T, TAG, CHECK> lh,
                    t_explicit<T, TAG, CHECK> rh) noexcept {
    return get(lh) == get(rh);
  }

  template<class T, class TAG, class CHECK>
  constexpr
  t_bool operator!=(t_explicit<T, TAG, CHECK> lh,
                    t_explicit<T, TAG, CHECK> rh) noexcept {
    return get(lh) != get(rh);
  }

///////////////////////////////////////////////////////////////////////////////

  template<class> struct t_is_explicit_;

  template<class T, class TAG, class CHECK>
  struct t_is_explicit_<t_explicit<T, TAG, CHECK>> {
    using t_result_ = t_explicit<T, TAG, CHECK>;
  };

  template<class E, class T, class TAG, class CHECK>
  constexpr
  E transform(t_explicit<T, TAG, CHECK> src) {
    return typename t_is_explicit_<E>::t_result_{get(src)};
  }

///////////////////////////////////////////////////////////////////////////////
}
}

#endif
