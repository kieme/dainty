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

#ifndef _DAINTY_BASE_LOGICAL_H_
#define _DAINTY_BASE_LOGICAL_H_

#include "dainty_base_logical_impl.h"

namespace dainty
{
namespace base
{
namespace logical
{
  /////////////////////////////////////////////////////////////////////////////

  using types::t_n_;
  using types::t_ix_;
  using types::t_u8_;
  using types::t_u16_;
  using types::t_u32_;
  using types::t_u64_;
  using types::t_u_;
  using types::t_i8_;
  using types::t_i16_;
  using types::t_i32_;
  using types::t_i64_;
  using types::t_i_;
  using types::t_int;
  using types::t_pack;
  using types::t_bool;
  using types::t_void;
  using types::t_value_of;

  using types::t_op_less_tag;
  using types::t_op_less_equal_tag;
  using types::t_op_larger_tag;
  using types::t_op_larger_equal_tag;
  using types::t_op_equal_tag;
  using types::t_op_not_equal_tag;
  using types::t_op_plus_tag;
  using types::t_op_minus_tag;
  using types::t_op_multiply_tag;
  using types::t_op_divide_tag;
  using types::t_op_mod_tag;
  using types::t_op_self_plus_tag;
  using types::t_op_self_minus_tag;
  using types::t_op_self_multiply_tag;
  using types::t_op_self_divide_tag;
  using types::t_op_self_mod_tag;
  using types::t_op_and_tag;
  using types::t_op_or_tag;
  using types::t_op_not_tag;
  using types::t_op_negate_tag;
  using types::t_op_incr_tag;
  using types::t_op_decr_tag;
  using types::t_op_post_incr_tag;
  using types::t_op_post_decr_tag;
  using types::t_op_bin_xor_tag;
  using types::t_op_bin_and_tag;
  using types::t_op_bin_ior_tag;
  using types::t_op_bin_lshift_tag;
  using types::t_op_bin_rshift_tag;
  using types::t_op_bin_self_xor_tag;
  using types::t_op_bin_self_and_tag;
  using types::t_op_bin_self_ior_tag;
  using types::t_op_bin_self_lshift_tag;
  using types::t_op_bin_self_rshift_tag;
  using types::t_ops_compare_tag;
  using types::t_ops_arithmetic_tag;
  using types::t_ops_arithmetic_self_tag;
  using types::t_ops_or_and_tag;
  using types::t_ops_bin_tag;
  using types::t_ops_value_tag;

  using impl_::t_property;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG, typename... Ls>
  using t_logical = impl_::t_logical<T, TAG, Ls...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename L, typename T>
  constexpr
  L mk(T value) noexcept {
    return impl_::mk_<L>(value);
  }

  template<typename L, typename T, typename TAG, typename... Ls>
  constexpr
  L mk(t_logical<T, TAG, Ls...> logical) noexcept {
    return impl_::mk_<L>(logical);
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG, typename... Ls>
  constexpr
  T get(t_logical<T, TAG, Ls...> logical) noexcept {
    return impl_::get_(logical);
  }

  template<typename T, typename TAG, typename... Ls, typename T1>
  constexpr
  t_logical<T, TAG, Ls...>& set(t_logical<T, TAG, Ls...>& logical,
                                T1 value) noexcept {
    return impl_::set_(logical, value);
  }

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... Ls1>
  constexpr
  t_logical<T, TAG, Ls...>& set(t_logical<T,  TAG,  Ls...>& logical,
                                t_logical<T1, TAG1, Ls1...> value) noexcept {
    return impl_::set_(logical, value);
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_logical_value
    = impl_::t_is_logical_value<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_logical_value
    = impl_::t_is_not_logical_value<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_logical_value
    = impl_::t_opt_if_logical_value<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_logical_value
    = impl_::t_opt_if_not_logical_value<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_logical_value
    = impl_::t_if_logical_value<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_logical_value
    = impl_::t_if_not_logical_value<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename L, typename... Ls>
  using t_is_logical         = impl_::t_is_logical<L, Ls...>;

  template<typename L, typename... Ls>
  using t_is_not_logical     = impl_::t_is_not_logical<L, Ls...>;

  template<typename O, typename L, typename... Ls>
  using t_opt_if_logical     = impl_::t_opt_if_logical<O, L, Ls...>;

  template<typename O, typename L, typename... Ls>
  using t_opt_if_not_logical = impl_::t_opt_if_not_logical<O, L, Ls...>;

  template<typename L, typename... Ls>
  using t_if_logical         = impl_::t_if_logical<L, Ls...>;

  template<typename L, typename... Ls>
  using t_if_not_logical     = impl_::t_if_not_logical<L, Ls...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename T1, typename... Ts>
  using t_is_logical_data
    = impl_::t_is_logical_data<T, T1, Ts...>;

  template<typename T, typename T1, typename... Ts>
  using t_is_not_logical_data
    = impl_::t_is_not_logical_data<T, T1, Ts...>;

  template<typename O, typename T, typename T1, typename... Ts>
  using t_opt_if_logical_data
    = impl_::t_opt_if_logical_data<O, T, T1, Ts...>;

  template<typename O, typename T, typename T1, typename... Ts>
  using t_opt_if_not_logical_data
    = impl_::t_opt_if_not_logical_data<O, T, T1, Ts...>;

  template<typename T, typename T1, typename... Ts>
  using t_if_logical_data
    = impl_::t_if_logical_data<T, T1, Ts...>;

  template<typename T, typename T1, typename... Ts>
  using t_if_not_logical_data
    = impl_::t_if_not_logical_data<T, T1, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename OP, typename L, typename... Ls>
  using t_is_logical_op
    = impl_::t_is_logical_op<OP, L, Ls...>;

  template<typename OP, typename L, typename... Ls>
  using t_is_not_logical_op
    = impl_::t_is_not_logical_op<OP, L, Ls...>;

  template<typename O, typename OP, typename L, typename... Ls>
  using t_opt_if_logical_op
    = impl_::t_opt_if_logical_op<O, OP, L, Ls...>;

  template<typename O, typename OP, typename L, typename... Ls>
  using t_opt_if_not_logical_op
    = impl_::t_opt_if_not_logical_op<O, OP, L, Ls...>;

  template<typename OP, typename L, typename... Ls>
  using t_if_logical_op
    = impl_::t_if_logical_op<OP, L, Ls...>;

  template<typename OP, typename L, typename... Ls>
  using t_if_not_logical_op
     = impl_::t_if_logical_op<OP, L, Ls...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_ops_of  = impl_::t_ops_of<T>;
  template<typename T> using t_tag_of  = impl_::t_tag_of<T>;
  template<typename T> using t_tags_of = impl_::t_tags_of<T>;

  /////////////////////////////////////////////////////////////////////////////

  struct t_truth_tag_ {
    using t_ops = t_pack<t_op_equal_tag, t_op_not_equal_tag>;
  };

  using t_truth = t_logical<t_bool, t_truth_tag_>;

  constexpr t_truth TRUTHY{true};
  constexpr t_truth FALSY {false};

  /////////////////////////////////////////////////////////////////////////////

  struct t_i_tag_ {
    using t_ops = t_ops_value_tag;
  };
  using t_i8  = t_logical<t_i8_,  t_i_tag_, t_truth>;
  using t_i16 = t_logical<t_i16_, t_i_tag_, t_truth>;
  using t_i32 = t_logical<t_i32_, t_i_tag_, t_truth>;
  using t_i64 = t_logical<t_i64_, t_i_tag_, t_truth>;

  using t_i   = t_i64;

  /////////////////////////////////////////////////////////////////////////////

  struct t_u_tag_ {
    using t_ops = t_ops_value_tag;
  };
  using t_u8  = t_logical<t_u8_,  t_u_tag_, t_i16>;
  using t_u16 = t_logical<t_u16_, t_u_tag_, t_i32>;
  using t_u32 = t_logical<t_u32_, t_u_tag_, t_i64>;
  using t_u64 = t_logical<t_u64_, t_u_tag_, t_truth>;

  using t_u   = t_u64;

  /////////////////////////////////////////////////////////////////////////////

  struct t_b_tag_ { // binary
    using t_ops = t_ops_value_tag;
  };
  using t_u8_b  = t_logical<t_u8_,  t_b_tag_, t_u8>;
  using t_u16_b = t_logical<t_u16_, t_b_tag_, t_u16>;
  using t_u32_b = t_logical<t_u32_, t_b_tag_, t_u32>;
  using t_u64_b = t_logical<t_u64_, t_b_tag_, t_u64>;

  using t_b     = t_u64_b;

  /////////////////////////////////////////////////////////////////////////////

  struct t_n_tag_ {
    using t_ops = t_ops_value_tag;
  };
  using t_u8_n  = t_logical<t_u8_,  t_n_tag_, t_u8>;
  using t_u16_n = t_logical<t_u16_, t_n_tag_, t_u16>;
  using t_u32_n = t_logical<t_u32_, t_n_tag_, t_u32>;
  using t_u64_n = t_logical<t_u64_, t_n_tag_, t_u64>;

  using t_i8_n  = t_logical<t_i8_,  t_n_tag_, t_i8>;
  using t_i16_n = t_logical<t_i16_, t_n_tag_, t_i16>;
  using t_i32_n = t_logical<t_i32_, t_n_tag_, t_i32>;
  using t_i64_n = t_logical<t_i64_, t_n_tag_, t_i64>;

  using t_n     = t_u64_n;

  /////////////////////////////////////////////////////////////////////////////

  struct t_n_max_tag_ {
    using t_ops = t_ops_value_tag;
  };
  using t_u8_n_max  = t_logical<t_u8_,  t_n_max_tag_, t_u8_n>;
  using t_u16_n_max = t_logical<t_u16_, t_n_max_tag_, t_u16_n>;
  using t_u32_n_max = t_logical<t_u32_, t_n_max_tag_, t_u32_n>;
  using t_u64_n_max = t_logical<t_u64_, t_n_max_tag_, t_u64_n>;

  using t_i8_n_max  = t_logical<t_i8_,  t_n_max_tag_, t_i8_n>;
  using t_i16_n_max = t_logical<t_i16_, t_n_max_tag_, t_i16_n>;
  using t_i32_n_max = t_logical<t_i32_, t_n_max_tag_, t_i32_n>;
  using t_i64_n_max = t_logical<t_i64_, t_n_max_tag_, t_i64_n>;

  using t_n_max     = t_u64_n_max;

  /////////////////////////////////////////////////////////////////////////////

  struct t_n_min_tag_ {
    using t_ops = t_ops_value_tag;
  };
  using t_u8_n_min  = t_logical<t_u8_,  t_n_min_tag_, t_u8_n>;
  using t_u16_n_min = t_logical<t_u16_, t_n_min_tag_, t_u16_n>;
  using t_u32_n_min = t_logical<t_u32_, t_n_min_tag_, t_u32_n>;
  using t_u64_n_min = t_logical<t_u64_, t_n_min_tag_, t_u64_n>;

  using t_i8_n_min  = t_logical<t_i8_,  t_n_min_tag_, t_i8_n>;
  using t_i16_n_min = t_logical<t_i16_, t_n_min_tag_, t_i16_n>;
  using t_i32_n_min = t_logical<t_i32_, t_n_min_tag_, t_i32_n>;
  using t_i64_n_min = t_logical<t_i64_, t_n_min_tag_, t_i64_n>;

  using t_n_min     = t_u64_n_min;

  /////////////////////////////////////////////////////////////////////////////

  struct t_ix_tag_ {
    using t_ops = t_ops_value_tag;
  };
  using t_u8_ix  = t_logical<t_u8_,  t_ix_tag_, t_u8_n>;
  using t_u16_ix = t_logical<t_u16_, t_ix_tag_, t_u16_n>;
  using t_u32_ix = t_logical<t_u32_, t_ix_tag_, t_u32_n>;
  using t_u64_ix = t_logical<t_u64_, t_ix_tag_, t_u64_n>;

  using t_i8_ix  = t_logical<t_i8_,  t_ix_tag_, t_i8_n>;
  using t_i16_ix = t_logical<t_i16_, t_ix_tag_, t_i16_n>;
  using t_i32_ix = t_logical<t_i32_, t_ix_tag_, t_i32_n>;
  using t_i64_ix = t_logical<t_i64_, t_ix_tag_, t_i64_n>;

  using t_ix     = t_u64_ix;

  /////////////////////////////////////////////////////////////////////////////

  struct t_ix_begin_tag_ {
    using t_ops = t_ops_value_tag;
  };
  using t_u8_ix_begin  = t_logical<t_u8_,  t_ix_begin_tag_, t_u8_ix>;
  using t_u16_ix_begin = t_logical<t_u16_, t_ix_begin_tag_, t_u16_ix>;
  using t_u32_ix_begin = t_logical<t_u32_, t_ix_begin_tag_, t_u32_ix>;
  using t_u64_ix_begin = t_logical<t_u64_, t_ix_begin_tag_, t_u64_ix>;

  using t_i8_ix_begin  = t_logical<t_i8_,  t_ix_begin_tag_, t_i8_ix>;
  using t_i16_ix_begin = t_logical<t_i16_, t_ix_begin_tag_, t_i16_ix>;
  using t_i32_ix_begin = t_logical<t_i32_, t_ix_begin_tag_, t_i32_ix>;
  using t_i64_ix_begin = t_logical<t_i64_, t_ix_begin_tag_, t_i64_ix>;

  using t_ix_begin     = t_u64_ix_begin;

  /////////////////////////////////////////////////////////////////////////////

  struct t_ix_end_tag_ {
    using t_ops = t_ops_value_tag;
  };
  using t_u8_ix_end  = t_logical<t_u8_,  t_ix_end_tag_, t_u8_ix>;
  using t_u16_ix_end = t_logical<t_u16_, t_ix_end_tag_, t_u16_ix>;
  using t_u32_ix_end = t_logical<t_u32_, t_ix_end_tag_, t_u32_ix>;
  using t_u64_ix_end = t_logical<t_u64_, t_ix_end_tag_, t_u64_ix>;

  using t_i8_ix_end  = t_logical<t_i8_,  t_ix_end_tag_, t_i8_ix>;
  using t_i16_ix_end = t_logical<t_i16_, t_ix_end_tag_, t_i16_ix>;
  using t_i32_ix_end = t_logical<t_i32_, t_ix_end_tag_, t_i32_ix>;
  using t_i64_ix_end = t_logical<t_i64_, t_ix_end_tag_, t_i64_ix>;

  using t_ix_end     = t_u64_ix_end;

  /////////////////////////////////////////////////////////////////////////////

  template<typename L, typename L1, typename... Ls>
  constexpr
  auto max_of(L1 l, Ls... ls) noexcept -> L {
    return impl_::max_of_<L>(l, ls...);
  }

  template<typename L, typename L1, typename... Ls>
  constexpr
  auto min_of(L1 l, Ls... ls) noexcept -> L {
    return impl_::min_of_<L>(l, ls...);
  }

  template<typename L, typename L1, typename... Ls>
  constexpr
  auto sum_of(L1 l, Ls... ls) noexcept -> L {
    return impl_::sum_of_<L>(l, ls...);
  }

  template<typename L, typename L1, typename... Ls>
  constexpr
  auto avg_of(L1 l, Ls... ls) noexcept -> L {
    return impl_::avg_of_<L>(l, ls...);
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename L>
  constexpr auto MIN_of_ = L{t_property<t_value_of<L>>::MIN};

  template<typename L>
  constexpr auto MAX_of_ = L{t_property<t_value_of<L>>::MAX};

  /////////////////////////////////////////////////////////////////////////////

  constexpr auto U8_MIN  = MIN_of_<t_u8>;
  constexpr auto U8_MAX  = MAX_of_<t_u8>;

  constexpr auto U16_MIN = MIN_of_<t_u16>;
  constexpr auto U16_MAX = MAX_of_<t_u16>;

  constexpr auto U32_MIN = MIN_of_<t_u32>;
  constexpr auto U32_MAX = MAX_of_<t_u32>;

  constexpr auto U64_MIN = MIN_of_<t_u64>;
  constexpr auto U64_MAX = MAX_of_<t_u64>;

  constexpr auto U_MIN   = MIN_of_<t_u>;
  constexpr auto U_MAX   = MAX_of_<t_u>;

  /////////////////////////////////////////////////////////////////////////////

  constexpr auto I8_MIN  = MIN_of_<t_i8>;
  constexpr auto I8_MAX  = MAX_of_<t_i8>;

  constexpr auto I16_MIN = MIN_of_<t_i16>;
  constexpr auto I16_MAX = MAX_of_<t_i16>;

  constexpr auto I32_MIN = MIN_of_<t_i32>;
  constexpr auto I32_MAX = MAX_of_<t_i32>;

  constexpr auto I64_MIN = MIN_of_<t_i64>;
  constexpr auto I64_MAX = MAX_of_<t_i64>;

  constexpr auto I_MIN   = MIN_of_<t_i>;
  constexpr auto I_MAX   = MAX_of_<t_i>;

  /////////////////////////////////////////////////////////////////////////////

  constexpr auto U8_B_MIN  = MIN_of_<t_u8_b>;
  constexpr auto U8_B_MAX  = MAX_of_<t_u8_b>;

  constexpr auto U16_B_MIN = MIN_of_<t_u16_b>;
  constexpr auto U16_B_MAX = MAX_of_<t_u16_b>;

  constexpr auto U32_B_MIN = MIN_of_<t_u32_b>;
  constexpr auto U32_B_MAX = MAX_of_<t_u32_b>;

  constexpr auto U64_B_MIN = MIN_of_<t_u64_b>;
  constexpr auto U64_B_MAX = MAX_of_<t_u64_b>;

  constexpr auto U_B_MIN   = MIN_of_<t_b>;
  constexpr auto U_B_MAX   = MAX_of_<t_b>;

  /////////////////////////////////////////////////////////////////////////////

  constexpr auto U8_N_MIN  = MIN_of_<t_u8_n>;
  constexpr auto U8_N_MAX  = MAX_of_<t_u8_n>;

  constexpr auto U16_N_MIN = MIN_of_<t_u16_n>;
  constexpr auto U16_N_MAX = MAX_of_<t_u16_n>;

  constexpr auto U32_N_MIN = MIN_of_<t_u32_n>;
  constexpr auto U32_N_MAX = MAX_of_<t_u32_n>;

  constexpr auto U64_N_MIN = MIN_of_<t_u64_n>;
  constexpr auto U64_N_MAX = MAX_of_<t_u64_n>;

  constexpr auto I8_N_MIN  = MIN_of_<t_i8_n>;
  constexpr auto I8_N_MAX  = MAX_of_<t_i8_n>;

  constexpr auto I16_N_MIN = MIN_of_<t_i16_n>;
  constexpr auto I16_N_MAX = MAX_of_<t_i16_n>;

  constexpr auto I32_N_MIN = MIN_of_<t_i32_n>;
  constexpr auto I32_N_MAX = MAX_of_<t_i32_n>;

  constexpr auto I64_N_MIN = MIN_of_<t_i64_n>;
  constexpr auto I64_N_MAX = MAX_of_<t_i64_n>;

  constexpr auto N_MIN     = MIN_of_<t_n>;
  constexpr auto N_MAX     = MAX_of_<t_n>;

  /////////////////////////////////////////////////////////////////////////////

  constexpr auto U8_N_MIN_MIN  = MIN_of_<t_u8_n_min>;
  constexpr auto U8_N_MIN_MAX  = MAX_of_<t_u8_n_min>;

  constexpr auto U16_N_MIN_MIN = MIN_of_<t_u16_n_min>;
  constexpr auto U16_N_MIN_MAX = MAX_of_<t_u16_n_min>;

  constexpr auto U32_N_MIN_MIN = MIN_of_<t_u32_n_min>;
  constexpr auto U32_N_MIN_MAX = MAX_of_<t_u32_n_min>;

  constexpr auto U64_N_MIN_MIN = MIN_of_<t_u64_n_min>;
  constexpr auto U64_N_MIN_MAX = MAX_of_<t_u64_n_min>;

  constexpr auto I8_N_MIN_MIN  = MIN_of_<t_i8_n_min>;
  constexpr auto I8_N_MIN_MAX  = MAX_of_<t_i8_n_min>;

  constexpr auto I16_N_MIN_MIN = MIN_of_<t_i16_n_min>;
  constexpr auto I16_N_MIN_MAX = MAX_of_<t_i16_n_min>;

  constexpr auto I32_N_MIN_MIN = MIN_of_<t_i32_n_min>;
  constexpr auto I32_N_MIN_MAX = MAX_of_<t_i32_n_min>;

  constexpr auto I64_N_MIN_MIN = MIN_of_<t_i64_n_min>;
  constexpr auto I64_N_MIN_MAX = MAX_of_<t_i64_n_min>;

  constexpr auto N_MIN_MIN     = MIN_of_<t_n_min>;
  constexpr auto N_MIN_MAX     = MAX_of_<t_n_min>;

  /////////////////////////////////////////////////////////////////////////////

  constexpr auto U8_N_MAX_MIN  = MIN_of_<t_u8_n_max>;
  constexpr auto U8_N_MAX_MAX  = MAX_of_<t_u8_n_max>;

  constexpr auto U16_N_MAX_MIN = MIN_of_<t_u16_n_max>;
  constexpr auto U16_N_MAX_MAX = MAX_of_<t_u16_n_max>;

  constexpr auto U32_N_MAX_MIN = MIN_of_<t_u32_n_max>;
  constexpr auto U32_N_MAX_MAX = MAX_of_<t_u32_n_max>;

  constexpr auto U64_N_MAX_MIN = MIN_of_<t_u64_n_max>;
  constexpr auto U64_N_MAX_MAX = MAX_of_<t_u64_n_max>;

  constexpr auto I8_N_MAX_MIN  = MIN_of_<t_i8_n_max>;
  constexpr auto I8_N_MAX_MAX  = MAX_of_<t_i8_n_max>;

  constexpr auto I16_N_MAX_MIN = MIN_of_<t_i16_n_max>;
  constexpr auto I16_N_MAX_MAX = MAX_of_<t_i16_n_max>;

  constexpr auto I32_N_MAX_MIN = MIN_of_<t_i32_n_max>;
  constexpr auto I32_N_MAX_MAX = MAX_of_<t_i32_n_max>;

  constexpr auto I64_N_MAX_MIN = MIN_of_<t_i64_n_max>;
  constexpr auto I64_N_MAX_MAX = MAX_of_<t_i64_n_max>;

  constexpr auto N_MAX_MIN     = MIN_of_<t_n_max>;
  constexpr auto N_MAX_MAX     = MAX_of_<t_n_max>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  t_bool operator==(t_logical<T,  TAG,  Ls...>  lh,
                    t_logical<T1, TAG1, L1s...> rh) noexcept {
    static_assert(t_is_logical_op<t_op_equal_tag,
                                 t_logical<T,  TAG,  Ls...>,
                                 t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do equal operation");
    return get(lh) == get(rh);
  }

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  t_bool operator!=(t_logical<T,  TAG,  Ls...>  lh,
                    t_logical<T1, TAG1, L1s...> rh) noexcept {
    static_assert(t_is_logical_op<t_op_not_equal_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do not_equal operation");
    return get(lh) != get(rh);
  }

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  t_bool operator<(t_logical<T,  TAG,  Ls...>  lh,
                   t_logical<T1, TAG1, L1s...> rh) noexcept {
    static_assert(t_is_logical_op<t_op_less_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do less operation");
    return get(lh) < get(rh);
  }

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  t_bool operator>(t_logical<T,  TAG,  Ls...>  lh,
                   t_logical<T1, TAG1, L1s...> rh) noexcept {
    static_assert(t_is_logical_op<t_op_larger_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do larger operation");
    return get(lh) > get(rh);
  }

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  t_bool operator<=(t_logical<T,  TAG,  Ls...>  lh,
                    t_logical<T1, TAG1, L1s...> rh) noexcept {
    static_assert(t_is_logical_op<t_op_less_equal_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do less_equal operation");
    return get(lh) <= get(rh);
  }

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  t_bool operator>=(t_logical<T,  TAG,  Ls...>  lh,
                    t_logical<T1, TAG1, L1s...> rh) noexcept {
    static_assert(t_is_logical_op<t_op_larger_equal_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do larger_equal operation");
    return get(lh) >= get(rh);
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator+(t_logical<T,  TAG,  Ls...>  lh,
                 t_logical<T1, TAG1, L1s...> rh) noexcept
      -> impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                           t_logical<T1, TAG1, L1s...>> {
    static_assert(t_is_logical_op<t_op_plus_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do plus operation");
    return impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                             t_logical<T1, TAG1, L1s...>>
      {(get(lh) + get(rh))};
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator+(t_logical<T,  TAG, Ls...> lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(t_is_logical_op<t_op_plus_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do plus operation");
    return t_logical<T, TAG, Ls...>(get(lh) + rh);
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator+(T lh, t_logical<T,  TAG, Ls...> rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(t_is_logical_op<t_op_plus_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do plus operation");
    return t_logical<T, TAG, Ls...>(lh + get(rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator-(t_logical<T,  TAG,  Ls...>  lh,
                 t_logical<T1, TAG1, L1s...> rh) noexcept
      -> impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                           t_logical<T1, TAG1, L1s...>> {
    static_assert(t_is_logical_op<t_op_minus_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do minus operation");
    return impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                             t_logical<T1, TAG1, L1s...>>
      {(get(lh) - get(rh))};
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator-(t_logical<T,  TAG, Ls...> lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(t_is_logical_op<t_op_minus_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do minus operation");
    return t_logical<T, TAG, Ls...>(get(lh) - rh);
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator-(T lh, t_logical<T,  TAG, Ls...> rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(t_is_logical_op<t_op_minus_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do minus operation");
    return t_logical<T, TAG, Ls...>(lh - get(rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator*(t_logical<T,  TAG,  Ls...>  lh,
                 t_logical<T1, TAG1, L1s...> rh) noexcept
      -> impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                           t_logical<T1, TAG1, L1s...>> {
    static_assert(t_is_logical_op<t_op_multiply_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do multiply operation");
    return impl_::t_largest_<t_logical<T, TAG,  Ls...>,
                             t_logical<T1,TAG1, L1s...>>
      {(get(lh) * get(rh))};
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator*(t_logical<T,  TAG, Ls...> lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(t_is_logical_op<t_op_multiply_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do multiply operation");
    return t_logical<T, TAG, Ls...>(get(lh) * rh);
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator*(T lh, t_logical<T,  TAG, Ls...> rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(t_is_logical_op<t_op_multiply_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do multiply operation");
    return t_logical<T, TAG, Ls...>(lh * get(rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator/(t_logical<T,  TAG,  Ls...>  lh,
                 t_logical<T1, TAG1, L1s...> rh) noexcept
      -> impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                           t_logical<T1, TAG1, L1s...>> {
    static_assert(t_is_logical_op<t_op_divide_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do divide operation");
    return impl_::t_largest_<t_logical<T, TAG,  Ls...>,
                             t_logical<T1,TAG1, L1s...>>
      {(get(lh) / get(rh))};
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator/(t_logical<T, TAG, Ls...> lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(t_is_logical_op<t_op_divide_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do divide operation");
    return t_logical<T, TAG, Ls...>(get(lh) / rh);
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator/(T lh, t_logical<T, TAG, Ls...> rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(t_is_logical_op<t_op_divide_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do divide operation");
    return t_logical<T, TAG, Ls...>(lh / get(rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator%(t_logical<T,  TAG,  Ls...>  lh,
                 t_logical<T1, TAG1, L1s...> rh) noexcept
      -> impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                           t_logical<T1, TAG1, L1s...>> {
    static_assert(t_is_logical_op<t_op_mod_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do mod operation");
    return impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                             t_logical<T1, TAG1, L1s...>>
      {(get(lh) % get(rh))};
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator%(t_logical<T,  TAG, Ls...> lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(t_is_logical_op<t_op_mod_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do mod operation");
    return t_logical<T, TAG, Ls...>(get(lh) % rh);
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator%(T lh, t_logical<T,  TAG, Ls...> rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(t_is_logical_op<t_op_mod_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do mod operation");
    return t_logical<T, TAG, Ls...>(lh % get(rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator+=(t_logical<T,  TAG,  Ls...>& lh,
                  t_logical<T1, TAG1, L1s...> rh) noexcept
      -> t_logical<T,  TAG, Ls...>& {
    static_assert(t_is_logical_op<t_op_self_plus_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do self plus operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) + get(rh)));
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator+=(t_logical<T,  TAG, Ls...>& lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...>& {
    static_assert(t_is_logical_op<t_op_self_plus_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do self plus operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) + rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator-=(t_logical<T,  TAG,  Ls...>& lh,
                  t_logical<T1, TAG1, L1s...> rh) noexcept
      -> t_logical<T,  TAG, Ls...>& {
    static_assert(t_is_logical_op<t_op_self_minus_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do self minus operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) - get(rh)));
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator-=(t_logical<T,  TAG, Ls...>& lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...>& {
    static_assert(t_is_logical_op<t_op_self_minus_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do self minus operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) - rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator*=(t_logical<T,  TAG,  Ls...>& lh,
                  t_logical<T1, TAG1, L1s...> rh) noexcept
      -> t_logical<T,  TAG, Ls...>& {
    static_assert(t_is_logical_op<t_op_self_multiply_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do self multiply operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) * get(rh)));
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator*=(t_logical<T,  TAG, Ls...>& lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...>& {
    static_assert(t_is_logical_op<t_op_self_multiply_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do self multiply operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) * rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator/=(t_logical<T,  TAG,  Ls...>& lh,
                  t_logical<T1, TAG1, L1s...> rh) noexcept
      -> t_logical<T,  TAG, Ls...>& {
    static_assert(t_is_logical_op<t_op_self_divide_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do self divide operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) / get(rh)));
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator/=(t_logical<T,  TAG, Ls...>& lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...>& {
    static_assert(t_is_logical_op<t_op_self_divide_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do self divide operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) / rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator%=(t_logical<T,  TAG,  Ls...>& lh,
                  t_logical<T1, TAG1, L1s...> rh) noexcept
      -> t_logical<T,  TAG, Ls...>& {
    static_assert(t_is_logical_op<t_op_self_mod_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do self mod operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) % get(rh)));
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator%=(t_logical<T,  TAG, Ls...>& lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...>& {
    static_assert(t_is_logical_op<t_op_self_mod_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do self mod operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) % rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  t_bool operator&&(t_logical<T,  TAG,  Ls...>  lh,
                    t_logical<T1, TAG1, L1s...> rh) noexcept {
    static_assert(t_is_logical_op<t_op_and_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do and operation");
    return get(lh) && get(rh);
  }

  template<typename T,  typename TAG,  typename... Ls, typename U>
  constexpr
  t_bool operator&&(t_logical<T, TAG, Ls...> lh, const U& rh) noexcept {
    static_assert(t_is_logical_op<t_op_and_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do and operation");
    return get(lh) && rh;
  }

  template<typename T,  typename TAG,  typename... Ls, typename U>
  constexpr
  t_bool operator&&(const U& lh, t_logical<T, TAG, Ls...> rh) noexcept {
    static_assert(t_is_logical_op<t_op_and_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do and operation");
    return lh && get(rh);
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  t_bool operator||(t_logical<T,  TAG,  Ls...>  lh,
                    t_logical<T1, TAG1, L1s...> rh) noexcept {
    static_assert(t_is_logical_op<t_op_or_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do or operation");
    return get(lh) && get(rh);
  }

  template<typename T,  typename TAG,  typename... Ls, typename U>
  constexpr
  t_bool operator||(t_logical<T, TAG, Ls...> lh, const U& rh) noexcept {
    static_assert(t_is_logical_op<t_op_or_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do or operation");
    return get(lh) && rh;
  }

  template<typename T,  typename TAG,  typename... Ls, typename U>
  constexpr
  t_bool operator||(const U& lh, t_logical<T, TAG, Ls...> rh) noexcept {
    static_assert(t_is_logical_op<t_op_or_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do or operation");
    return lh && get(rh);
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator-(t_logical<T, TAG, Ls...> lh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(t_is_logical_op<t_op_negate_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical type not allowed to negate operation");
    return t_logical<T, TAG, Ls...>{-get(lh)};
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator++(t_logical<T, TAG, Ls...>& lh) noexcept
      -> t_logical<T, TAG, Ls...>& {
    static_assert(t_is_logical_op<t_op_incr_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical type not allowed increment operation");
    return (lh = t_logical<T, TAG, Ls...>{get(lh) + 1});
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator--(t_logical<T, TAG, Ls...>& lh) noexcept
      -> t_logical<T, TAG, Ls...>& {
    static_assert(t_is_logical_op<t_op_decr_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical type not allowed to decrement operation");
    return (lh = t_logical<T, TAG, Ls...>{get(lh) - 1});
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator++(t_logical<T, TAG, Ls...>& lh, t_int) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(t_is_logical_op<t_op_post_incr_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical type not allowed post increment operation");
    t_logical<T, TAG, Ls...> tmp = lh;
    lh = t_logical<T, TAG, Ls...>{get(lh) + 1};
    return tmp;
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator--(t_logical<T, TAG, Ls...>& lh, t_int) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(t_is_logical_op<t_op_post_decr_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical type not allowed to post decrement operation");
    t_logical<T, TAG, Ls...> tmp = lh;
    lh = t_logical<T, TAG, Ls...>{get(lh) - 1};
    return tmp;
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator^(t_logical<T,  TAG,  Ls...>  lh,
                 t_logical<T1, TAG1, L1s...> rh) noexcept
      -> impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                           t_logical<T1, TAG1, L1s...>> {
    static_assert(t_is_logical_op<t_op_bin_xor_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do bin xor operation");
    return impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                             t_logical<T1, TAG1, L1s...>>
      {(get(lh) ^ get(rh))};
  }

  template<typename T, typename TAG, typename... Ls>
  constexpr
  auto operator^(t_logical<T, TAG, Ls...> lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(t_is_logical_op<t_op_bin_xor_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do bin xor operation");
    return t_logical<T, TAG, Ls...>(get(lh) ^ rh);
  }

  template<typename T, typename TAG, typename... Ls>
  constexpr
  auto operator^(T lh, t_logical<T, TAG, Ls...> rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(t_is_logical_op<t_op_bin_xor_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do bin xor operation");
    return t_logical<T, TAG, Ls...>(lh ^ get(rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator&(t_logical<T,  TAG,  Ls...>  lh,
                 t_logical<T1, TAG1, L1s...> rh) noexcept
      -> impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                           t_logical<T1, TAG1, L1s...>> {
    static_assert(t_is_logical_op<t_op_bin_and_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do bin and operation");
    return impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                             t_logical<T1, TAG1, L1s...>>
      {(get(lh) & get(rh))};
  }

  template<typename T, typename TAG, typename... Ls>
  constexpr
  auto operator&(t_logical<T, TAG, Ls...> lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(t_is_logical_op<t_op_bin_and_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do bin and operation");
    return t_logical<T, TAG, Ls...>(get(lh) & rh);
  }

  template<typename T, typename TAG, typename... Ls>
  constexpr
  auto operator&(T lh, t_logical<T, TAG, Ls...> rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(t_is_logical_op<t_op_bin_and_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do bin and operation");
    return t_logical<T, TAG, Ls...>(lh & get(rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator|(t_logical<T,  TAG,  Ls...>  lh,
                 t_logical<T1, TAG1, L1s...> rh) noexcept
      -> impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                           t_logical<T1, TAG1, L1s...>> {
    static_assert(t_is_logical_op<t_op_bin_ior_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do bin ior operation");
    return impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                             t_logical<T1, TAG1, L1s...>>
      {(get(lh) | get(rh))};
  }

  template<typename T, typename TAG, typename... Ls>
  constexpr
  auto operator|(t_logical<T, TAG, Ls...> lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(t_is_logical_op<t_op_bin_ior_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do bin ior operation");
    return t_logical<T, TAG, Ls...>(get(lh) | rh);
  }

  template<typename T, typename TAG, typename... Ls>
  constexpr
  auto operator|(T lh, t_logical<T, TAG, Ls...> rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(t_is_logical_op<t_op_bin_ior_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do bin ior operation");
    return t_logical<T, TAG, Ls...>(lh | get(rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator<<(t_logical<T,  TAG,  Ls...>  lh,
                 t_logical<T1, TAG1, L1s...> rh) noexcept
      -> impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                           t_logical<T1, TAG1, L1s...>> {
    static_assert(t_is_logical_op<t_op_bin_lshift_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do bin lshift operation");
    return impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                             t_logical<T1, TAG1, L1s...>>
      {(get(lh) << get(rh))};
  }

  template<typename T, typename TAG, typename... Ls>
  constexpr
  auto operator<<(t_logical<T, TAG, Ls...> lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(t_is_logical_op<t_op_bin_lshift_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do bin lshift operation");
    return t_logical<T, TAG, Ls...>(get(lh) << rh);
  }

  template<typename T, typename TAG, typename... Ls>
  constexpr
  auto operator<<(T lh, t_logical<T, TAG, Ls...> rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(t_is_logical_op<t_op_bin_lshift_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do bin lshift operation");
    return t_logical<T, TAG, Ls...>(lh << get(rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator>>(t_logical<T,  TAG,  Ls...>  lh,
                 t_logical<T1, TAG1, L1s...> rh) noexcept
      -> impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                           t_logical<T1, TAG1, L1s...>> {
    static_assert(t_is_logical_op<t_op_bin_rshift_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do bin rshift operation");
    return impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                             t_logical<T1, TAG1, L1s...>>
      {(get(lh) >> get(rh))};
  }

  template<typename T, typename TAG, typename... Ls>
  constexpr
  auto operator>>(t_logical<T, TAG, Ls...> lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(t_is_logical_op<t_op_bin_rshift_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do bin rshift operation");
    return t_logical<T, TAG, Ls...>(get(lh) >> rh);
  }

  template<typename T, typename TAG, typename... Ls>
  constexpr
  auto operator>>(T lh, t_logical<T, TAG, Ls...> rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(t_is_logical_op<t_op_bin_rshift_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do bin rshift operation");
    return t_logical<T, TAG, Ls...>(lh >> get(rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator^=(t_logical<T,  TAG,  Ls...>& lh,
                  t_logical<T1, TAG1, L1s...> rh) noexcept
      -> t_logical<T,  TAG, Ls...>& {
    static_assert(t_is_logical_op<t_op_bin_self_xor_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do ^= operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) ^ get(rh)));
  }

  template<typename T, typename TAG, typename... Ls>
  constexpr
  auto operator^=(t_logical<T, TAG, Ls...>& lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...>& {
    static_assert(t_is_logical_op<t_op_bin_self_xor_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do ^= operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) ^ rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator&=(t_logical<T,  TAG,  Ls...>& lh,
                  t_logical<T1, TAG1, L1s...> rh) noexcept
      -> t_logical<T,  TAG, Ls...>& {
    static_assert(t_is_logical_op<t_op_bin_self_and_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do ^= operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) & get(rh)));
  }

  template<typename T, typename TAG, typename... Ls>
  constexpr
  auto operator&=(t_logical<T, TAG, Ls...>& lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...>& {
    static_assert(t_is_logical_op<t_op_bin_self_and_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do ^= operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) & rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator|=(t_logical<T,  TAG,  Ls...>& lh,
                  t_logical<T1, TAG1, L1s...> rh) noexcept
      -> t_logical<T,  TAG, Ls...>& {
    static_assert(t_is_logical_op<t_op_bin_self_ior_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do |= operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) | get(rh)));
  }

  template<typename T, typename TAG, typename... Ls>
  constexpr
  auto operator|=(t_logical<T, TAG, Ls...>& lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...>& {
    static_assert(t_is_logical_op<t_op_bin_self_ior_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do |= operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) | rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator<<=(t_logical<T,  TAG,  Ls...>& lh,
                   t_logical<T1, TAG1, L1s...> rh) noexcept
      -> t_logical<T,  TAG, Ls...>& {
    static_assert(t_is_logical_op<t_op_bin_self_lshift_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do <<= operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) << get(rh)));
  }

  template<typename T, typename TAG, typename... Ls>
  constexpr
  auto operator<<=(t_logical<T, TAG, Ls...>& lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...>& {
    static_assert(t_is_logical_op<t_op_bin_self_lshift_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do <<= operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) << rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator>>=(t_logical<T,  TAG,  Ls...>& lh,
                   t_logical<T1, TAG1, L1s...> rh) noexcept
      -> t_logical<T,  TAG, Ls...>& {
    static_assert(t_is_logical_op<t_op_bin_self_rshift_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do >>= operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) >> get(rh)));
  }

  template<typename T, typename TAG, typename... Ls>
  constexpr
  auto operator>>=(t_logical<T, TAG, Ls...>& lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...>& {
    static_assert(t_is_logical_op<t_op_bin_self_rshift_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do >>= operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) >> rh));
  }

  /*
  /////////////////////////////////////////////////////////////////////////////

  constexpr
  t_u operator""_u(unsigned long long value) noexcept {
    return t_u{value};
  }

  constexpr
  t_u64 operator""_64u(unsigned long long value) noexcept {
    return t_u64{value};
  }

  /////////////////////////////////////////////////////////////////////////////

  constexpr
  t_b operator""_b(unsigned long long value) noexcept {
    return t_b{value};
  }

  constexpr
  t_64b operator""_64b(unsigned long long value) noexcept {
    return t_64b{value};
  }

  /////////////////////////////////////////////////////////////////////////////

  constexpr
  t_n operator""_n(unsigned long long value) noexcept {
    return t_n{value};
  }

  constexpr
  t_n_min operator""_n_min(unsigned long long value) noexcept {
    return t_n_min{value};
  }

  constexpr
  t_n_max operator""_n_max(unsigned long long value) noexcept {
    return t_n_max{value};
  }

  /////////////////////////////////////////////////////////////////////////////

  constexpr
  t_ix operator""_ix(unsigned long long value) noexcept {
    return t_ix{value};
  }

  constexpr
  t_ix_begin operator""_ix_begin(unsigned long long value) noexcept {
    return t_ix_begin{value};
  }

  constexpr
  t_ix_end operator""_ix_end(unsigned long long value) noexcept {
    return t_ix_end{value};
  }

  /////////////////////////////////////////////////////////////////////////////
  */
}
}
}

#endif
