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

#ifndef _DAINTY_NAMED_LOGICAL_H_
#define _DAINTY_NAMED_LOGICAL_H_

#include "dainty_named_types.h"
#include "dainty_named_traits.h"

namespace dainty
{
namespace named
{
///////////////////////////////////////////////////////////////////////////////

  template<class, class TAG, class...> class t_logical;

  template<class T, class TAG, class... TAGS>
  constexpr T get(t_logical<T, TAG, TAGS...>) noexcept;

///////////////////////////////////////////////////////////////////////////////

  template<class T, class TAG, class... TAGS>
  class t_logical {
  public:
    static_assert(t_least_one_is_true<T, t_is_fundamental,
                                         t_is_ptr>::VALUE, "wrong type");

    using t_value   = T;
    using t_tags    = t_pack<TAG, TAGS...>;
    using t_flatten = typename t_flatten_tree<t_tags>::t_value;

    constexpr explicit t_logical(t_value value) noexcept
      : value_{value} // XXX - write tests
    { }

    template<typename T1, typename... TAGS1>
    constexpr t_logical(t_logical<T1, TAGS1...> logical) noexcept {
      static_assert(t_is_subset_of<t_flatten,
                                   typename t_logical<T1, TAGS1...>::t_flatten
                                  >::VALUE,
                     "cannot copy type. types don't give permission.");
      value_ = logical.value_;
    }

    template<typename T1, typename... TAGS1>
    constexpr t_logical& operator=(t_logical<T1, TAGS1...> logical) noexcept {
      static_assert(t_is_subset_of<t_flatten,
                                   typename t_logical<T1, TAGS1...>::t_flatten
                                  >::VALUE,
                     "cannot copy type. types don't give permission.");
      value_ = logical.value_;
      return *this;
    }

    t_logical()                            = delete;  // for clarity
    t_logical(const t_logical&)            = default; // for clarity
    t_logical& operator=(const t_logical&) = default; // for clarity

  private:
    template<class T1, class TAG1, class... TAGS1> friend class t_logical;
    template<class T1, class TAG1, class... TAGS1>
    friend constexpr T1  get(t_logical<T1, TAG1, TAGS1...>) noexcept;
    t_value value_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<class T, class TAG, class... TAGS>
  constexpr
  T get(t_logical<T, TAG, TAGS...> logical) noexcept { return logical.value_; }

///////////////////////////////////////////////////////////////////////////////

  template<class T,  class TAG,  class... TAGS,
           class T1, class TAG1, class... TAGS1>
  constexpr t_bool operator==(t_logical<T,  TAG,  TAGS...> lh,
                              t_logical<T1, TAG1, TAGS1...> rh) noexcept {

    static_assert(
      t_or<t_is_subset_of<typename t_logical<T,  TAGS...>::t_flatten,
                          typename t_logical<T1, TAGS1...>::t_flatten>,
           t_is_subset_of<typename t_logical<T1, TAGS1...>::t_flatten,
                          typename t_logical<T, TAGS...>::t_flatten>>::VALUE,
           "logical types cannot be compared");
    return get(lh) == get(rh);
  }

  template<class T,  class TAG,  class... TAGS,
           class T1, class TAG1, class... TAGS1>
  constexpr t_bool operator!=(t_logical<T,  TAG,  TAGS...> lh,
                              t_logical<T1, TAG1, TAGS1...> rh) noexcept {

    static_assert(
      t_or<t_is_subset_of<typename t_logical<T,  TAGS...>::t_flatten,
                          typename t_logical<T1, TAGS1...>::t_flatten>,
           t_is_subset_of<typename t_logical<T1, TAGS1...>::t_flatten,
                          typename t_logical<T, TAGS...>::t_flatten>>::VALUE,
           "logical types cannot be compared");
    return get(lh) != get(rh);
  }

///////////////////////////////////////////////////////////////////////////////

  template<class> struct t_is_logical : t_FALSE_ { };

  template<class T, class TAG, class... TAGS>
  struct t_is_logical<t_logical<T, TAG, TAGS...>> : t_TRUE_ { };

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
}
}

#endif
