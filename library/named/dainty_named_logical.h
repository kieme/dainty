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

  template<class> struct t_is_logical : t_FALSE_ { };

  template<class T, class TAG, class... TAGS>
  struct t_is_logical<t_logical<T, TAG, TAGS...>> : t_TRUE_ { };

///////////////////////////////////////////////////////////////////////////////

namespace impl_ {
  template<typename, typename, typename> struct t_is_allowed_ : t_FALSE_ { };

  template<class C, class T,  class... TAGS, class T1, class... TAGS1>
  struct t_is_allowed_<C, t_logical<T,  TAGS...>, t_logical<T1, TAGS1...>> {
    using t_1_ = typename t_logical<T,  TAGS... >::t_flatten;
    using t_2_ = typename t_logical<T1, TAGS1...>::t_flatten;
    using t_result =
        t_and<t_and<t_or<t_is_subset_of<t_1_, t_2_>,
                         t_is_subset_of<t_2_, t_1_>>,
                    t_is_one_of<C, t_1_>,
              t_is_one_of<C, t_2_>>>;
  };
}

template<typename C, typename T, typename T1>
struct t_is_allowed : impl_::t_is_allowed_<C, T, T1>::t_result { };

///////////////////////////////////////////////////////////////////////////////

  enum t_allow_equal_operator_tag {};

  template<class T,  class... TAGS,
           class T1, class... TAGS1>
  constexpr t_bool operator==(t_logical<T,  TAGS...> lh,
                              t_logical<T1, TAGS1...> rh) noexcept {
    static_assert(
      t_is_allowed<t_allow_equal_operator_tag,
                   t_logical<T,  TAGS...>, t_logical<T1, TAGS1...>>::VALUE,
           "logical types not allowed to do equal comparison");
    return get(lh) == get(rh);
  }

///////////////////////////////////////////////////////////////////////////////

  enum t_allow_not_equal_operator_tag {};

  template<class T,  class... TAGS,
           class T1, class... TAGS1>
  constexpr t_bool operator!=(t_logical<T,  TAGS...> lh,
                              t_logical<T1, TAGS1...> rh) noexcept {

    static_assert(
      t_is_allowed<t_allow_not_equal_operator_tag,
                   t_logical<T,  TAGS...>, t_logical<T1, TAGS1...>>::VALUE,
           "logical types not allowed to do not_equal comparison");
    return get(lh) != get(rh);
  }

///////////////////////////////////////////////////////////////////////////////

  enum t_allow_less_operator_tag {};

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr t_bool operator<(t_logical<T,  TAGS...>  lh,
                             t_logical<T1, TAGS1...> rh) noexcept {
    static_assert(
      t_is_allowed<t_allow_less_operator_tag,
                   t_logical<T,  TAGS...>, t_logical<T1, TAGS1...>>::VALUE,
           "logical types not allowed to do less comparison");
    return get(lh) < get(rh);
  }

///////////////////////////////////////////////////////////////////////////////

namespace types {
  using t_n = t_logical<t_n_, t_n_tag_>;

  enum  t_max_n_tag_ {};
  using t_max_n = t_logical<t_n_, t_n_tag_, t_max_n_tag_>;

  enum  t_min_n_tag_ {};
  using t_min_n = t_logical<t_n_, t_n_tag_, t_min_n_tag_>;

  enum  t_by_n_tag_ {};
  using t_by_n = t_logical<t_n_, t_n_tag_,  t_by_n_tag_>;

///////////////////////////////////////////////////////////////////////////////

  using t_ix_tags_ = t_pack<t_ix_tag_, t_allow_equal_operator_tag>;
  using t_ix = t_logical<t_ix_, t_ix_tags_>;

  enum  t_begin_ix_tag_ {};
  using t_begin_ix = t_logical<t_ix_, t_ix_tags_, t_begin_ix_tag_>;

  enum  t_end_ix_tag_ {};
  using t_end_ix   = t_logical<t_ix_, t_ix_tags_, t_end_ix_tag_>;
}

///////////////////////////////////////////////////////////////////////////////
}
}

#endif
