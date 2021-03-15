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

#include "impl_/dainty_base_logical_impl.h"

/******************************************************************************

__DOC__

  t_logical

  has been designed for serveral reasons. The main motivation is to increase
  the use of static type checking of builtin types and to use it to assign
  logical meaning to values.

  e.g. t_u4 is an unsigned int with a width of 4 bytes. 0 < x < 2**32 - 1

  t_u4 only describes the builtin type and not what it is used for.

  So, you might use it as a index, or a maximin number or as a minimum number.
  These logical ideas are distinct from each other interms of their purpose.

  t_logical tries to solve this problem. Note this could have been part of the
  c++ language itself, but it is not.

  In the exmaple here, t_logical can be used to make these values distinct by
  making them different types.

  using t_u4_ix    = t_logical<t_u4_ix_,    t_ix_tag_>;
  using t_u4_n     = t_logical<t_u4_n_,     t_n_tag_>;
  using t_u4_n_min = t_logical<t_u4_n_min_, t_n_min_tag_>;
  using t_u4_n_max = t_logical<t_u4_n_max_, t_n_max_tag_>;

  All these types are now distinct. This is handy in overload resolution but it
  also documents the code.

  e.g. copy(t_u4_max{100}, t_p4_src{src}, t_p4_dest{dest});

  Apart from the type system, t_logical also supports three other important
  features:

    A inheritance based copy mechanism which resembles class inhertitance but
    not in all aspects. Think of it as allowing copying up the inheritance
    hierachy, without explicit validation required.

    A way to check values for their validity. The second template argument can
    provide a checking function. If it is available it will be used, otherwise
    it is ignored and not part of the compilation.

    Its last major feature is to allow certain operations or not. Operations
    are not inherited. Operations can be defined by the second parameter.


  to read or write values to a t_logical, a specific interface is required.
    get(t_n{7})

    t_n n{0};
    set(n, 10)

  use mk() to copy from one distinct logical type to another. It can optimize
  the checking.

    auto n  = t_n{50};
    auto ix = mk<t_ix>(n);

  standard logical types:

    t_i : signed integer

      t_i
      t_i1
      t_i2
      t_i4
      t_i1

    t_u (->t_i): unsigned integer

      t_u
      t_u1
      t_u2
      t_u4
      t_u1

    t_b (->t_u): binary value

      t_b
      t_b1
      t_b2
      t_b4
      t_b8

    t_h (->t_u): hexadecimal value

      t_h
      t_h1
      t_h2
      t_h4
      t_h8

    t_id (->[t_u|t_i]): indentifier

      t_id
      t_i_id
      t_u_id
      t_i1_id
      t_i2_id
      t_i4_id
      t_i1_id
      t_u1_id
      t_u2_id
      t_u4_id
      t_u1_id

    t_n (->[t_u|t_i]): <- number(quantity) value

      t_n
      t_i_n
      t_u_n
      t_i1_n
      t_i2_n
      t_i4_n
      t_i1_n
      t_u1_n
      t_u2_n
      t_u4_n
      t_u1_n

    t_n_min (->t_n): minimum number value

      t_n_min
      t_i_n_min
      t_u_n_min
      t_i1_n_min
      t_i2_n_min
      t_i4_n_min
      t_i1_n_min
      t_u1_n_min
      t_u2_n_min
      t_u4_n_min
      t_u1_n_min

    t_n_min (->t_n): maximum number value

      t_n_max
      t_i_n_max
      t_u_n_max
      t_i1_n_max
      t_i2_n_max
      t_i4_n_max
      t_i1_n_max
      t_u1_n_max
      t_u2_n_max
      t_u4_n_max
      t_u1_n_max

    t_ix (->t_id): index

      t_ix
      t_i_ix
      t_u_ix
      t_i1_ix
      t_i2_ix
      t_i4_ix
      t_i8_ix
      t_u1_ix
      t_u2_ix
      t_u4_ix
      t_u1_ix

    t_ix_begin (->t_ix): index begin

      t_ix_begin
      t_i_ix_begin
      t_u_ix_begin
      t_i1_ix_begin
      t_i2_ix_begin
      t_i4_ix_begin
      t_i1_ix_begin
      t_u1_ix_begin
      t_u2_ix_begin
      t_u4_ix_begin
      t_u1_ix_begin

    t_ix_end (t_ix_end): index end

      t_ix_end
      t_i_ix_end
      t_u_ix_end
      t_i1_ix_end
      t_i2_ix_end
      t_i4_ix_end
      t_i1_ix_end
      t_u1_ix_end
      t_u2_ix_end
      t_u4_ix_end
      t_u1_ix_end

    t_s (->t_n): size

      t_s
      t_u1_s
      t_u2_s
      t_u4_s
      t_u1_s

    t_s_min (->t_s): logical type for a minimum size

      t_s_min
      t_u1_s_min
      t_u2_s_min
      t_u4_s_min
      t_u1_s_min

    t_s_max (->t_s): logical type for a maximum size

      t_s_max
      t_u1_s_max
      t_u2_s_max
      t_u4_s_max
      t_u1_s_max

******************************************************************************/

namespace dainty
{
namespace base
{
namespace logical
{
  using types::t_int;
  using types::t_bool;
  using types::t_value_of;

  /////////////////////////////////////////////////////////////////////////////

  using types::t_truth_tag_;

  using types::t_truth_;
  using types::T_truth_;
  using types::r_truth_;
  using types::R_truth_;
  using types::p_truth_;
  using types::P_truth_;

  using types::TRUTHY_;
  using types::FALSEY_;

  /////////////////////////////////////////////////////////////////////////////

  using types::t_validity_tag_;

  using types::t_validity_;
  using types::T_validity_;
  using types::r_validity_;
  using types::p_validity_;
  using types::P_validity_;

  using types::VALID_;
  using types::INVALID_;

  /////////////////////////////////////////////////////////////////////////////

  using types::t_i_tag_;

  using types::t_i_;
  using types::T_i_;
  using types::r_i_;
  using types::R_i_;
  using types::p_i_;
  using types::P_i_;

  using types::t_i1_;
  using types::T_i1_;
  using types::r_i1_;
  using types::R_i1_;
  using types::p_i1_;
  using types::P_i1_;

  using types::t_i2_;
  using types::T_i2_;
  using types::r_i2_;
  using types::R_i2_;
  using types::p_i2_;
  using types::P_i2_;

  using types::t_i4_;
  using types::T_i4_;
  using types::r_i4_;
  using types::R_i4_;
  using types::p_i4_;
  using types::P_i4_;

  using types::t_i8_;
  using types::T_i8_;
  using types::r_i8_;
  using types::R_i8_;
  using types::p_i8_;
  using types::P_i8_;

  /////////////////////////////////////////////////////////////////////////////

  using types::t_u_tag_;

  using types::t_u_;
  using types::T_u_;
  using types::r_u_;
  using types::R_u_;
  using types::p_u_;
  using types::P_u_;

  using types::t_u1_;
  using types::T_u1_;
  using types::r_u1_;
  using types::R_u1_;
  using types::p_u1_;
  using types::P_u1_;

  using types::t_u2_;
  using types::T_u2_;
  using types::r_u2_;
  using types::R_u2_;
  using types::p_u2_;
  using types::P_u2_;

  using types::t_u4_;
  using types::T_u4_;
  using types::r_u4_;
  using types::R_u4_;
  using types::p_u4_;
  using types::P_u4_;

  using types::t_u8_;
  using types::T_u8_;
  using types::r_u8_;
  using types::R_u8_;
  using types::p_u8_;
  using types::P_u8_;

  using types::t_h_tag_;

  using types::t_h_;
  using types::T_h_;
  using types::r_h_;
  using types::R_h_;
  using types::p_h_;
  using types::P_h_;

  using types::t_u1_h_;
  using types::T_u1_h_;
  using types::r_u1_h_;
  using types::R_u1_h_;
  using types::p_u1_h_;
  using types::P_u1_h_;

  using types::t_u2_h_;
  using types::T_u2_h_;
  using types::r_u2_h_;
  using types::R_u2_h_;
  using types::p_u2_h_;
  using types::P_u2_h_;

  using types::t_u4_h_;
  using types::T_u4_h_;
  using types::r_u4_h_;
  using types::R_u4_h_;
  using types::p_u4_h_;
  using types::P_u4_h_;

  using types::t_u8_h_;
  using types::T_u8_h_;
  using types::r_u8_h_;
  using types::R_u8_h_;
  using types::p_u8_h_;
  using types::P_u8_h_;

  using types::t_b_tag_;

  using types::t_b_;
  using types::T_b_;
  using types::r_b_;
  using types::R_b_;
  using types::p_b_;
  using types::P_b_;

  using types::t_u1_b_;
  using types::T_u1_b_;
  using types::r_u1_b_;
  using types::R_u1_b_;
  using types::p_u1_b_;
  using types::P_u1_b_;

  using types::t_u2_b_;
  using types::T_u2_b_;
  using types::r_u2_b_;
  using types::R_u2_b_;
  using types::p_u2_b_;
  using types::P_u2_b_;

  using types::t_u4_b_;
  using types::T_u4_b_;
  using types::r_u4_b_;
  using types::R_u4_b_;
  using types::p_u4_b_;
  using types::P_u4_b_;

  using types::t_u8_b_;
  using types::T_u8_b_;
  using types::r_u8_b_;
  using types::R_u8_b_;
  using types::p_u8_b_;
  using types::P_u8_b_;

  using types::t_id_tag_;

  using types::t_id_;
  using types::T_id_;
  using types::r_id_;
  using types::R_id_;
  using types::p_id_;
  using types::P_id_;

  using types::t_i_id_;
  using types::T_i_id_;
  using types::r_i_id_;
  using types::R_i_id_;
  using types::p_i_id_;
  using types::P_i_id_;

  using types::t_u_id_;
  using types::T_u_id_;
  using types::r_u_id_;
  using types::R_u_id_;
  using types::p_u_id_;
  using types::P_u_id_;

  using types::t_i1_id_;
  using types::T_i1_id_;
  using types::r_i1_id_;
  using types::R_i1_id_;
  using types::p_i1_id_;
  using types::P_i1_id_;

  using types::t_i2_id_;
  using types::T_i2_id_;
  using types::r_i2_id_;
  using types::R_i2_id_;
  using types::p_i2_id_;
  using types::P_i2_id_;

  using types::t_i4_id_;
  using types::T_i4_id_;
  using types::r_i4_id_;
  using types::R_i4_id_;
  using types::p_i4_id_;
  using types::P_i4_id_;

  using types::t_i8_id_;
  using types::T_i8_id_;
  using types::r_i8_id_;
  using types::R_i8_id_;
  using types::p_i8_id_;
  using types::P_i8_id_;

  using types::t_u1_id_;
  using types::T_u1_id_;
  using types::r_u1_id_;
  using types::R_u1_id_;
  using types::p_u1_id_;
  using types::P_u1_id_;

  using types::t_u2_id_;
  using types::T_u2_id_;
  using types::r_u2_id_;
  using types::R_u2_id_;
  using types::p_u2_id_;
  using types::P_u2_id_;

  using types::t_u4_id_;
  using types::T_u4_id_;
  using types::r_u4_id_;
  using types::R_u4_id_;
  using types::p_u4_id_;
  using types::P_u4_id_;

  using types::t_u8_id_;
  using types::T_u8_id_;
  using types::r_u8_id_;
  using types::R_u8_id_;
  using types::p_u8_id_;
  using types::P_u8_id_;

  using types::t_n_tag_;

  using types::t_n_;
  using types::T_n_;
  using types::r_n_;
  using types::R_n_;
  using types::p_n_;
  using types::P_n_;

  using types::t_i_n_;
  using types::T_i_n_;
  using types::r_i_n_;
  using types::R_i_n_;
  using types::p_i_n_;
  using types::P_i_n_;

  using types::t_u_n_;
  using types::T_u_n_;
  using types::r_u_n_;
  using types::R_u_n_;
  using types::p_u_n_;
  using types::P_u_n_;

  using types::t_i1_n_;
  using types::T_i1_n_;
  using types::r_i1_n_;
  using types::R_i1_n_;
  using types::p_i1_n_;
  using types::P_i1_n_;

  using types::t_i2_n_;
  using types::T_i2_n_;
  using types::r_i2_n_;
  using types::R_i2_n_;
  using types::p_i2_n_;
  using types::P_i2_n_;

  using types::t_i4_n_;
  using types::T_i4_n_;
  using types::r_i4_n_;
  using types::R_i4_n_;
  using types::p_i4_n_;
  using types::P_i4_n_;

  using types::t_i8_n_;
  using types::T_i8_n_;
  using types::r_i8_n_;
  using types::R_i8_n_;
  using types::p_i8_n_;
  using types::P_i8_n_;

  using types::t_u1_n_;
  using types::T_u1_n_;
  using types::r_u1_n_;
  using types::R_u1_n_;
  using types::p_u1_n_;
  using types::P_u1_n_;

  using types::t_u2_n_;
  using types::T_u2_n_;
  using types::r_u2_n_;
  using types::R_u2_n_;
  using types::p_u2_n_;
  using types::P_u2_n_;

  using types::t_u4_n_;
  using types::T_u4_n_;
  using types::r_u4_n_;
  using types::R_u4_n_;
  using types::p_u4_n_;
  using types::P_u4_n_;

  using types::t_u8_n_;
  using types::T_u8_n_;
  using types::r_u8_n_;
  using types::R_u8_n_;
  using types::p_u8_n_;
  using types::P_u8_n_;

  using types::t_n_min_tag_;

  using types::t_n_min_;
  using types::T_n_min_;
  using types::r_n_min_;
  using types::R_n_min_;
  using types::p_n_min_;
  using types::P_n_min_;

  using types::t_i_n_min_;
  using types::T_i_n_min_;
  using types::r_i_n_min_;
  using types::R_i_n_min_;
  using types::p_i_n_min_;
  using types::P_i_n_min_;

  using types::t_u_n_min_;
  using types::T_u_n_min_;
  using types::r_u_n_min_;
  using types::R_u_n_min_;
  using types::p_u_n_min_;
  using types::P_u_n_min_;

  using types::t_i1_n_min_;
  using types::T_i1_n_min_;
  using types::r_i1_n_min_;
  using types::R_i1_n_min_;
  using types::p_i1_n_min_;
  using types::P_i1_n_min_;

  using types::t_i2_n_min_;
  using types::T_i2_n_min_;
  using types::r_i2_n_min_;
  using types::R_i2_n_min_;
  using types::p_i2_n_min_;
  using types::P_i2_n_min_;

  using types::t_i4_n_min_;
  using types::T_i4_n_min_;
  using types::r_i4_n_min_;
  using types::R_i4_n_min_;
  using types::p_i4_n_min_;
  using types::P_i4_n_min_;

  using types::t_i8_n_min_;
  using types::T_i8_n_min_;
  using types::r_i8_n_min_;
  using types::R_i8_n_min_;
  using types::p_i8_n_min_;
  using types::P_i8_n_min_;

  using types::t_u1_n_min_;
  using types::T_u1_n_min_;
  using types::r_u1_n_min_;
  using types::R_u1_n_min_;
  using types::p_u1_n_min_;
  using types::P_u1_n_min_;

  using types::t_u2_n_min_;
  using types::T_u2_n_min_;
  using types::r_u2_n_min_;
  using types::R_u2_n_min_;
  using types::p_u2_n_min_;
  using types::P_u2_n_min_;

  using types::t_u4_n_min_;
  using types::T_u4_n_min_;
  using types::r_u4_n_min_;
  using types::R_u4_n_min_;
  using types::p_u4_n_min_;
  using types::P_u4_n_min_;

  using types::t_u8_n_min_;
  using types::T_u8_n_min_;
  using types::r_u8_n_min_;
  using types::R_u8_n_min_;
  using types::p_u8_n_min_;
  using types::P_u8_n_min_;

  using types::t_n_max_tag_;

  using types::t_n_max_;
  using types::T_n_max_;
  using types::r_n_max_;
  using types::R_n_max_;
  using types::p_n_max_;
  using types::P_n_max_;

  using types::t_i_n_max_;
  using types::T_i_n_max_;
  using types::r_i_n_max_;
  using types::R_i_n_max_;
  using types::p_i_n_max_;
  using types::P_i_n_max_;

  using types::t_u_n_max_;
  using types::T_u_n_max_;
  using types::r_u_n_max_;
  using types::R_u_n_max_;
  using types::p_u_n_max_;
  using types::P_u_n_max_;

  using types::t_i1_n_max_;
  using types::T_i1_n_max_;
  using types::r_i1_n_max_;
  using types::R_i1_n_max_;
  using types::p_i1_n_max_;
  using types::P_i1_n_max_;

  using types::t_i2_n_max_;
  using types::T_i2_n_max_;
  using types::r_i2_n_max_;
  using types::R_i2_n_max_;
  using types::p_i2_n_max_;
  using types::P_i2_n_max_;

  using types::t_i4_n_max_;
  using types::T_i4_n_max_;
  using types::r_i4_n_max_;
  using types::R_i4_n_max_;
  using types::p_i4_n_max_;
  using types::P_i4_n_max_;

  using types::t_i8_n_max_;
  using types::T_i8_n_max_;
  using types::r_i8_n_max_;
  using types::R_i8_n_max_;
  using types::p_i8_n_max_;
  using types::P_i8_n_max_;

  using types::t_u1_n_max_;
  using types::T_u1_n_max_;
  using types::r_u1_n_max_;
  using types::R_u1_n_max_;
  using types::p_u1_n_max_;
  using types::P_u1_n_max_;

  using types::t_u2_n_max_;
  using types::T_u2_n_max_;
  using types::r_u2_n_max_;
  using types::R_u2_n_max_;
  using types::p_u2_n_max_;
  using types::P_u2_n_max_;

  using types::t_u4_n_max_;
  using types::T_u4_n_max_;
  using types::r_u4_n_max_;
  using types::R_u4_n_max_;
  using types::p_u4_n_max_;
  using types::P_u4_n_max_;

  using types::t_u8_n_max_;
  using types::T_u8_n_max_;
  using types::r_u8_n_max_;
  using types::R_u8_n_max_;
  using types::p_u8_n_max_;
  using types::P_u8_n_max_;

  using types::t_ix_tag_;

  using types::t_ix_;
  using types::T_ix_;
  using types::r_ix_;
  using types::R_ix_;
  using types::p_ix_;
  using types::P_ix_;

  using types::t_i_ix_;
  using types::T_i_ix_;
  using types::r_i_ix_;
  using types::R_i_ix_;
  using types::p_i_ix_;
  using types::P_i_ix_;

  using types::t_u_ix_;
  using types::T_u_ix_;
  using types::r_u_ix_;
  using types::R_u_ix_;
  using types::p_u_ix_;
  using types::P_u_ix_;

  using types::t_i1_ix_;
  using types::T_i1_ix_;
  using types::r_i1_ix_;
  using types::R_i1_ix_;
  using types::p_i1_ix_;
  using types::P_i1_ix_;

  using types::t_i2_ix_;
  using types::T_i2_ix_;
  using types::r_i2_ix_;
  using types::R_i2_ix_;
  using types::p_i2_ix_;
  using types::P_i2_ix_;

  using types::t_i4_ix_;
  using types::T_i4_ix_;
  using types::r_i4_ix_;
  using types::R_i4_ix_;
  using types::p_i4_ix_;
  using types::P_i4_ix_;

  using types::t_i8_ix_;
  using types::T_i8_ix_;
  using types::r_i8_ix_;
  using types::R_i8_ix_;
  using types::p_i8_ix_;
  using types::P_i8_ix_;

  using types::t_u1_ix_;
  using types::T_u1_ix_;
  using types::r_u1_ix_;
  using types::R_u1_ix_;
  using types::p_u1_ix_;
  using types::P_u1_ix_;

  using types::t_u2_ix_;
  using types::T_u2_ix_;
  using types::r_u2_ix_;
  using types::R_u2_ix_;
  using types::p_u2_ix_;
  using types::P_u2_ix_;

  using types::t_u4_ix_;
  using types::T_u4_ix_;
  using types::r_u4_ix_;
  using types::R_u4_ix_;
  using types::p_u4_ix_;
  using types::P_u4_ix_;

  using types::t_u8_ix_;
  using types::T_u8_ix_;
  using types::r_u8_ix_;
  using types::R_u8_ix_;
  using types::p_u8_ix_;
  using types::P_u8_ix_;

  using types::t_ix_begin_tag_;

  using types::t_ix_begin_;
  using types::T_ix_begin_;
  using types::r_ix_begin_;
  using types::R_ix_begin_;
  using types::p_ix_begin_;
  using types::P_ix_begin_;

  using types::t_i_ix_begin_;
  using types::T_i_ix_begin_;
  using types::r_i_ix_begin_;
  using types::R_i_ix_begin_;
  using types::p_i_ix_begin_;
  using types::P_i_ix_begin_;

  using types::t_u_ix_begin_;
  using types::T_u_ix_begin_;
  using types::r_u_ix_begin_;
  using types::R_u_ix_begin_;
  using types::p_u_ix_begin_;
  using types::P_u_ix_begin_;

  using types::t_i1_ix_begin_;
  using types::T_i1_ix_begin_;
  using types::r_i1_ix_begin_;
  using types::R_i1_ix_begin_;
  using types::p_i1_ix_begin_;
  using types::P_i1_ix_begin_;

  using types::t_i2_ix_begin_;
  using types::T_i2_ix_begin_;
  using types::r_i2_ix_begin_;
  using types::R_i2_ix_begin_;
  using types::p_i2_ix_begin_;
  using types::P_i2_ix_begin_;

  using types::t_i4_ix_begin_;
  using types::T_i4_ix_begin_;
  using types::r_i4_ix_begin_;
  using types::R_i4_ix_begin_;
  using types::p_i4_ix_begin_;
  using types::P_i4_ix_begin_;

  using types::t_i8_ix_begin_;
  using types::T_i8_ix_begin_;
  using types::r_i8_ix_begin_;
  using types::R_i8_ix_begin_;
  using types::p_i8_ix_begin_;
  using types::P_i8_ix_begin_;

  using types::t_u1_ix_begin_;
  using types::T_u1_ix_begin_;
  using types::r_u1_ix_begin_;
  using types::R_u1_ix_begin_;
  using types::p_u1_ix_begin_;
  using types::P_u1_ix_begin_;

  using types::t_u2_ix_begin_;
  using types::T_u2_ix_begin_;
  using types::r_u2_ix_begin_;
  using types::R_u2_ix_begin_;
  using types::p_u2_ix_begin_;
  using types::P_u2_ix_begin_;

  using types::t_u4_ix_begin_;
  using types::T_u4_ix_begin_;
  using types::r_u4_ix_begin_;
  using types::R_u4_ix_begin_;
  using types::p_u4_ix_begin_;
  using types::P_u4_ix_begin_;

  using types::t_u8_ix_begin_;
  using types::T_u8_ix_begin_;
  using types::r_u8_ix_begin_;
  using types::R_u8_ix_begin_;
  using types::p_u8_ix_begin_;
  using types::P_u8_ix_begin_;

  using types::t_ix_end_tag_;

  using types::t_ix_end_;
  using types::T_ix_end_;
  using types::r_ix_end_;
  using types::R_ix_end_;
  using types::p_ix_end_;
  using types::P_ix_end_;

  using types::t_i_ix_end_;
  using types::T_i_ix_end_;
  using types::r_i_ix_end_;
  using types::R_i_ix_end_;
  using types::p_i_ix_end_;
  using types::P_i_ix_end_;

  using types::t_u_ix_end_;
  using types::T_u_ix_end_;
  using types::r_u_ix_end_;
  using types::R_u_ix_end_;
  using types::p_u_ix_end_;
  using types::P_u_ix_end_;

  using types::t_i1_ix_end_;
  using types::T_i1_ix_end_;
  using types::r_i1_ix_end_;
  using types::R_i1_ix_end_;
  using types::p_i1_ix_end_;
  using types::P_i1_ix_end_;

  using types::t_i2_ix_end_;
  using types::T_i2_ix_end_;
  using types::r_i2_ix_end_;
  using types::R_i2_ix_end_;
  using types::p_i2_ix_end_;
  using types::P_i2_ix_end_;

  using types::t_i4_ix_end_;
  using types::T_i4_ix_end_;
  using types::r_i4_ix_end_;
  using types::R_i4_ix_end_;
  using types::p_i4_ix_end_;
  using types::P_i4_ix_end_;

  using types::t_i8_ix_end_;
  using types::T_i8_ix_end_;
  using types::r_i8_ix_end_;
  using types::R_i8_ix_end_;
  using types::p_i8_ix_end_;
  using types::P_i8_ix_end_;

  using types::t_u1_ix_end_;
  using types::T_u1_ix_end_;
  using types::r_u1_ix_end_;
  using types::R_u1_ix_end_;
  using types::p_u1_ix_end_;
  using types::P_u1_ix_end_;

  using types::t_u2_ix_end_;
  using types::T_u2_ix_end_;
  using types::r_u2_ix_end_;
  using types::R_u2_ix_end_;
  using types::p_u2_ix_end_;
  using types::P_u2_ix_end_;

  using types::t_u4_ix_end_;
  using types::T_u4_ix_end_;
  using types::r_u4_ix_end_;
  using types::R_u4_ix_end_;
  using types::p_u4_ix_end_;
  using types::P_u4_ix_end_;

  using types::t_u8_ix_end_;
  using types::T_u8_ix_end_;
  using types::r_u8_ix_end_;
  using types::R_u8_ix_end_;
  using types::p_u8_ix_end_;
  using types::P_u8_ix_end_;

  using types::t_s_tag_;

  using types::t_s_;
  using types::T_s_;
  using types::r_s_;
  using types::R_s_;
  using types::p_s_;
  using types::P_s_;

  using types::t_s_min_tag_;

  using types::t_s_min_;
  using types::T_s_min_;
  using types::r_s_min_;
  using types::R_s_min_;
  using types::p_s_min_;
  using types::P_s_min_;

  using types::t_s_max_tag_;

  using types::t_s_max_;
  using types::T_s_max_;
  using types::r_s_max_;
  using types::R_s_max_;
  using types::p_s_max_;
  using types::P_s_max_;

  using types::t_cnt_tag_;

  using types::t_cnt_;
  using types::T_cnt_;
  using types::r_cnt_;
  using types::p_cnt_;
  using types::P_cnt_;

  using types::t_u1_cnt_;
  using types::T_u1_cnt_;
  using types::r_u1_cnt_;
  using types::p_u1_cnt_;
  using types::P_u1_cnt_;

  using types::t_u2_cnt_;
  using types::T_u2_cnt_;
  using types::r_u2_cnt_;
  using types::p_u2_cnt_;
  using types::P_u2_cnt_;

  using types::t_u4_cnt_;
  using types::T_u4_cnt_;
  using types::r_u4_cnt_;
  using types::p_u4_cnt_;
  using types::P_u4_cnt_;

  using types::t_u8_cnt_;
  using types::T_u8_cnt_;
  using types::r_u8_cnt_;
  using types::p_u8_cnt_;
  using types::P_u8_cnt_;

  using types::t_bytes_tag_;

  using types::t_bytes_;
  using types::T_bytes_;
  using types::r_bytes_;
  using types::p_bytes_;
  using types::P_bytes_;

  using types::t_u1_bytes_;
  using types::T_u1_bytes_;
  using types::r_u1_bytes_;
  using types::p_u1_bytes_;
  using types::P_u1_bytes_;

  using types::t_u2_bytes_;
  using types::T_u2_bytes_;
  using types::r_u2_bytes_;
  using types::p_u2_bytes_;
  using types::P_u2_bytes_;

  using types::t_u4_bytes_;
  using types::T_u4_bytes_;
  using types::r_u4_bytes_;
  using types::p_u4_bytes_;
  using types::P_u4_bytes_;

  using types::t_u8_bytes_;
  using types::T_u8_bytes_;
  using types::r_u8_bytes_;
  using types::p_u8_bytes_;
  using types::P_u8_bytes_;

  using types::t_byte_tag_;

  using types::t_byte_;
  using types::T_byte_;
  using types::r_byte_;
  using types::p_byte_;
  using types::P_byte_;

  using types::t_fd_tag_;

  using types::t_fd_;
  using types::T_fd_;
  using types::r_fd_;
  using types::p_fd_;
  using types::P_fd_;

  using types::t_percentage_tag_;

  using types::t_percentage_;
  using types::T_percentage_;
  using types::r_percentage_;
  using types::p_percentage_;
  using types::P_percentage_;

  using types::t_seqno_tag_;

  using types::t_seqno_;
  using types::T_seqno_;
  using types::r_seqno_;
  using types::p_seqno_;
  using types::P_seqno_;

  using types::t_errn_tag_;

  using types::t_errn_;
  using types::T_errn_;
  using types::r_errn_;
  using types::p_errn_;
  using types::P_errn_;

  using types::t_nsec_tag_;

  using types::t_nsec_;
  using types::T_nsec_;
  using types::r_nsec_;
  using types::p_nsec_;
  using types::P_nsec_;

  using types::t_usec_tag_;

  using types::t_usec_;
  using types::T_usec_;
  using types::r_usec_;
  using types::p_usec_;
  using types::P_usec_;

  using types::t_msec_tag_;

  using types::t_msec_;
  using types::T_msec_;
  using types::r_msec_;
  using types::p_msec_;
  using types::P_msec_;

  using types::t_sec_tag_;

  using types::t_sec_;
  using types::T_sec_;
  using types::r_sec_;
  using types::p_sec_;
  using types::P_sec_;

  using types::t_min_tag_;

  using types::t_min_;
  using types::T_min_;
  using types::r_min_;
  using types::p_min_;
  using types::P_min_;

  using types::t_ticks_tag_;

  using types::t_ticks_;
  using types::T_ticks_;
  using types::r_ticks_;
  using types::p_ticks_;
  using types::P_ticks_;

  /////////////////////////////////////////////////////////////////////////////

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

  template<typename TAG, typename... Ls>
  using t_logical_tag = impl_::t_logical_tag<TAG, Ls...>;

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

  using t_truth = t_logical<t_truth_, t_truth_tag_>;

  constexpr t_truth TRUTHY{true};
  constexpr t_truth FALSY {false};

  /////////////////////////////////////////////////////////////////////////////

  using t_i  = t_logical<t_i_,  t_i_tag_, t_truth>;
  using t_i1 = t_logical<t_i1_, t_i_tag_, t_truth>;
  using t_i2 = t_logical<t_i2_, t_i_tag_, t_truth>;
  using t_i4 = t_logical<t_i4_, t_i_tag_, t_truth>;
  using t_i8 = t_logical<t_i8_, t_i_tag_, t_truth>;

  /////////////////////////////////////////////////////////////////////////////

  using t_u  = t_logical<t_u_,  t_u_tag_, t_i>;
  using t_u1 = t_logical<t_u1_, t_u_tag_, t_i2>;
  using t_u2 = t_logical<t_u2_, t_u_tag_, t_i4>;
  using t_u4 = t_logical<t_u4_, t_u_tag_, t_i8>;
  using t_u8 = t_logical<t_u8_, t_u_tag_, t_truth>;

  /////////////////////////////////////////////////////////////////////////////

  using t_b    = t_logical<t_b_,    t_b_tag_, t_u>;
  using t_u1_b = t_logical<t_u1_b_, t_b_tag_, t_u1>;
  using t_u2_b = t_logical<t_u2_b_, t_b_tag_, t_u2>;
  using t_u4_b = t_logical<t_u4_b_, t_b_tag_, t_u4>;
  using t_u8_b = t_logical<t_u8_b_, t_b_tag_, t_u8>;

  /////////////////////////////////////////////////////////////////////////////

  using t_h    = t_logical<t_h_,    t_h_tag_, t_u>;
  using t_u1_h = t_logical<t_u1_h_, t_h_tag_, t_u1>;
  using t_u2_h = t_logical<t_u2_h_, t_h_tag_, t_u2>;
  using t_u4_h = t_logical<t_u4_h_, t_h_tag_, t_u4>;
  using t_u8_h = t_logical<t_u8_h_, t_h_tag_, t_u8>;

  /////////////////////////////////////////////////////////////////////////////

  using t_n    = t_logical<t_n_,    t_n_tag_, t_u>;
  using t_u_n  = t_logical<t_u_n_,  t_n_tag_, t_u>;
  using t_u1_n = t_logical<t_u1_n_, t_n_tag_, t_u1>;
  using t_u2_n = t_logical<t_u2_n_, t_n_tag_, t_u2>;
  using t_u4_n = t_logical<t_u4_n_, t_n_tag_, t_u4>;
  using t_u8_n = t_logical<t_u8_n_, t_n_tag_, t_u8>;

  using t_i_n  = t_logical<t_i_n_,  t_n_tag_, t_i>;
  using t_i1_n = t_logical<t_i1_n_, t_n_tag_, t_i1>;
  using t_i2_n = t_logical<t_i2_n_, t_n_tag_, t_i2>;
  using t_i4_n = t_logical<t_i4_n_, t_n_tag_, t_i4>;
  using t_i8_n = t_logical<t_i8_n_, t_n_tag_, t_i8>;

  /////////////////////////////////////////////////////////////////////////////

  using t_u1_n_max = t_logical<t_u1_, t_n_max_tag_, t_u1_n, t_u1_n>;
  using t_u2_n_max = t_logical<t_u2_, t_n_max_tag_, t_u2_n, t_u2_n>;
  using t_u4_n_max = t_logical<t_u4_, t_n_max_tag_, t_u4_n, t_u4_n>;
  using t_u8_n_max = t_logical<t_u8_, t_n_max_tag_, t_u8_n, t_u8_n>;

  using t_i1_n_max = t_logical<t_i1_, t_n_max_tag_, t_i1_n, t_i1_n>;
  using t_i2_n_max = t_logical<t_i2_, t_n_max_tag_, t_i2_n, t_i2_n>;
  using t_i4_n_max = t_logical<t_i4_, t_n_max_tag_, t_i4_n, t_i4_n>;
  using t_i8_n_max = t_logical<t_i8_, t_n_max_tag_, t_i8_n, t_i8_n>;

  using t_i_n_max   = t_i8_n_max;
  using t_u_n_max   = t_u8_n_max;
  using t_n_max     = t_u8_n_max;

  /////////////////////////////////////////////////////////////////////////////

  using t_u1_n_min = t_logical<t_u1_, t_n_min_tag_, t_u1_n, t_u1_n>;
  using t_u2_n_min = t_logical<t_u2_, t_n_min_tag_, t_u2_n, t_u2_n>;
  using t_u4_n_min = t_logical<t_u4_, t_n_min_tag_, t_u4_n, t_u4_n>;
  using t_u8_n_min = t_logical<t_u8_, t_n_min_tag_, t_u8_n, t_u8_n>;

  using t_i1_n_min = t_logical<t_i1_, t_n_min_tag_, t_i1_n, t_i1_n>;
  using t_i2_n_min = t_logical<t_i2_, t_n_min_tag_, t_i2_n, t_i2_n>;
  using t_i4_n_min = t_logical<t_i4_, t_n_min_tag_, t_i4_n, t_i4_n>;
  using t_i8_n_min = t_logical<t_i8_, t_n_min_tag_, t_i8_n, t_i8_n>;

  using t_i_n_min   = t_i8_n_min;
  using t_u_n_min   = t_u8_n_min;
  using t_n_min     = t_u8_n_min;

  /////////////////////////////////////////////////////////////////////////////

  using t_u1_id = t_logical<t_u1_, t_id_tag_, t_u1>;
  using t_u2_id = t_logical<t_u2_, t_id_tag_, t_u2>;
  using t_u4_id = t_logical<t_u4_, t_id_tag_, t_u4>;
  using t_u8_id = t_logical<t_u8_, t_id_tag_, t_u8>;

  using t_i1_id = t_logical<t_i1_, t_id_tag_, t_i1>;
  using t_i2_id = t_logical<t_i2_, t_id_tag_, t_i2>;
  using t_i4_id = t_logical<t_i4_, t_id_tag_, t_i4>;
  using t_i8_id = t_logical<t_i8_, t_id_tag_, t_i8>;

  using t_i_id   = t_i8_id;
  using t_u_id   = t_u8_id;
  using t_id     = t_u8_id;

  /////////////////////////////////////////////////////////////////////////////

  using t_u1_ix = t_logical<t_u1_, t_ix_tag_, t_u1_id>;
  using t_u2_ix = t_logical<t_u2_, t_ix_tag_, t_u2_id>;
  using t_u4_ix = t_logical<t_u4_, t_ix_tag_, t_u4_id>;
  using t_u8_ix = t_logical<t_u8_, t_ix_tag_, t_u8_id>;

  using t_i1_ix = t_logical<t_i1_, t_ix_tag_, t_i1_id>;
  using t_i2_ix = t_logical<t_i2_, t_ix_tag_, t_i2_id>;
  using t_i4_ix = t_logical<t_i4_, t_ix_tag_, t_i4_id>;
  using t_i8_ix = t_logical<t_i8_, t_ix_tag_, t_i8_id>;

  using t_i_ix   = t_i8_ix;
  using t_u_ix   = t_u8_ix;
  using t_ix     = t_u8_ix;

  /////////////////////////////////////////////////////////////////////////////

  using t_u1_ix_begin = t_logical<t_u1_, t_ix_begin_tag_, t_u1_ix>;
  using t_u2_ix_begin = t_logical<t_u2_, t_ix_begin_tag_, t_u2_ix>;
  using t_u4_ix_begin = t_logical<t_u4_, t_ix_begin_tag_, t_u4_ix>;
  using t_u8_ix_begin = t_logical<t_u8_, t_ix_begin_tag_, t_u8_ix>;

  using t_i1_ix_begin = t_logical<t_i1_, t_ix_begin_tag_, t_i1_ix>;
  using t_i2_ix_begin = t_logical<t_i2_, t_ix_begin_tag_, t_i2_ix>;
  using t_i4_ix_begin = t_logical<t_i4_, t_ix_begin_tag_, t_i4_ix>;
  using t_i8_ix_begin = t_logical<t_i8_, t_ix_begin_tag_, t_i8_ix>;

  using t_i_ix_begin   = t_i8_ix_begin;
  using t_u_ix_begin   = t_u8_ix_begin;
  using t_ix_begin     = t_u8_ix_begin;

  /////////////////////////////////////////////////////////////////////////////

  using t_u1_ix_end = t_logical<t_u1_, t_ix_end_tag_, t_u1_ix>;
  using t_u2_ix_end = t_logical<t_u2_, t_ix_end_tag_, t_u2_ix>;
  using t_u4_ix_end = t_logical<t_u4_, t_ix_end_tag_, t_u4_ix>;
  using t_u8_ix_end = t_logical<t_u8_, t_ix_end_tag_, t_u8_ix>;

  using t_i1_ix_end = t_logical<t_i1_, t_ix_end_tag_, t_i1_ix>;
  using t_i2_ix_end = t_logical<t_i2_, t_ix_end_tag_, t_i2_ix>;
  using t_i4_ix_end = t_logical<t_i4_, t_ix_end_tag_, t_i4_ix>;
  using t_i8_ix_end = t_logical<t_i8_, t_ix_end_tag_, t_i8_ix>;

  using t_i_ix_end   = t_i8_ix_end;
  using t_u_ix_end   = t_u8_ix_end;
  using t_ix_end     = t_u8_ix_end;

  /////////////////////////////////////////////////////////////////////////////

  using t_u1_s = t_logical<t_u1_, t_s_tag_, t_u1_n>;
  using t_u2_s = t_logical<t_u2_, t_s_tag_, t_u2_n>;
  using t_u4_s = t_logical<t_u4_, t_s_tag_, t_u4_n>;
  using t_u8_s = t_logical<t_u8_, t_s_tag_, t_u8_n>;

  using t_s     = t_u8_s;

  /////////////////////////////////////////////////////////////////////////////

  using t_u1_s_max = t_logical<t_u1_, t_s_max_tag_, t_u1_s>;
  using t_u2_s_max = t_logical<t_u2_, t_s_max_tag_, t_u2_s>;
  using t_u4_s_max = t_logical<t_u4_, t_s_max_tag_, t_u4_s>;
  using t_u8_s_max = t_logical<t_u8_, t_s_max_tag_, t_u8_s>;

  using t_s_max     = t_u8_s_max;

  /////////////////////////////////////////////////////////////////////////////

  using t_u1_s_min = t_logical<t_u1_, t_s_min_tag_, t_u1_s>;
  using t_u2_s_min = t_logical<t_u2_, t_s_min_tag_, t_u2_s>;
  using t_u4_s_min = t_logical<t_u4_, t_s_min_tag_, t_u4_s>;
  using t_u8_s_min = t_logical<t_u8_, t_s_min_tag_, t_u8_s>;

  using t_s_min     = t_u8_s_min;

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

  constexpr auto U8_MIN  = MIN_of_<t_u1>;
  constexpr auto U8_MAX  = MAX_of_<t_u1>;

  constexpr auto U16_MIN = MIN_of_<t_u2>;
  constexpr auto U16_MAX = MAX_of_<t_u2>;

  constexpr auto U32_MIN = MIN_of_<t_u4>;
  constexpr auto U32_MAX = MAX_of_<t_u4>;

  constexpr auto U64_MIN = MIN_of_<t_u8>;
  constexpr auto U64_MAX = MAX_of_<t_u8>;

  constexpr auto U_MIN   = MIN_of_<t_u>;
  constexpr auto U_MAX   = MAX_of_<t_u>;

  /////////////////////////////////////////////////////////////////////////////

  constexpr auto I8_MIN  = MIN_of_<t_i1>;
  constexpr auto I8_MAX  = MAX_of_<t_i1>;

  constexpr auto I16_MIN = MIN_of_<t_i2>;
  constexpr auto I16_MAX = MAX_of_<t_i2>;

  constexpr auto I32_MIN = MIN_of_<t_i4>;
  constexpr auto I32_MAX = MAX_of_<t_i4>;

  constexpr auto I64_MIN = MIN_of_<t_i8>;
  constexpr auto I64_MAX = MAX_of_<t_i8>;

  constexpr auto I_MIN   = MIN_of_<t_i>;
  constexpr auto I_MAX   = MAX_of_<t_i>;

  /////////////////////////////////////////////////////////////////////////////

  constexpr auto U8_B_MIN  = MIN_of_<t_u1_b>;
  constexpr auto U8_B_MAX  = MAX_of_<t_u1_b>;

  constexpr auto U16_B_MIN = MIN_of_<t_u2_b>;
  constexpr auto U16_B_MAX = MAX_of_<t_u2_b>;

  constexpr auto U32_B_MIN = MIN_of_<t_u4_b>;
  constexpr auto U32_B_MAX = MAX_of_<t_u4_b>;

  constexpr auto U64_B_MIN = MIN_of_<t_u8_b>;
  constexpr auto U64_B_MAX = MAX_of_<t_u8_b>;

  constexpr auto U_B_MIN   = MIN_of_<t_b>;
  constexpr auto U_B_MAX   = MAX_of_<t_b>;

  /////////////////////////////////////////////////////////////////////////////

  constexpr auto U8_N_MIN  = MIN_of_<t_u1_n>;
  constexpr auto U8_N_MAX  = MAX_of_<t_u1_n>;

  constexpr auto U16_N_MIN = MIN_of_<t_u2_n>;
  constexpr auto U16_N_MAX = MAX_of_<t_u2_n>;

  constexpr auto U32_N_MIN = MIN_of_<t_u4_n>;
  constexpr auto U32_N_MAX = MAX_of_<t_u4_n>;

  constexpr auto U64_N_MIN = MIN_of_<t_u8_n>;
  constexpr auto U64_N_MAX = MAX_of_<t_u8_n>;

  constexpr auto I8_N_MIN  = MIN_of_<t_i1_n>;
  constexpr auto I8_N_MAX  = MAX_of_<t_i1_n>;

  constexpr auto I16_N_MIN = MIN_of_<t_i2_n>;
  constexpr auto I16_N_MAX = MAX_of_<t_i2_n>;

  constexpr auto I32_N_MIN = MIN_of_<t_i4_n>;
  constexpr auto I32_N_MAX = MAX_of_<t_i4_n>;

  constexpr auto I64_N_MIN = MIN_of_<t_i8_n>;
  constexpr auto I64_N_MAX = MAX_of_<t_i8_n>;

  constexpr auto N_MIN     = MIN_of_<t_n>;
  constexpr auto N_MAX     = MAX_of_<t_n>;

  /////////////////////////////////////////////////////////////////////////////

  constexpr auto U8_N_MIN_MIN  = MIN_of_<t_u1_n_min>;
  constexpr auto U8_N_MIN_MAX  = MAX_of_<t_u1_n_min>;

  constexpr auto U16_N_MIN_MIN = MIN_of_<t_u2_n_min>;
  constexpr auto U16_N_MIN_MAX = MAX_of_<t_u2_n_min>;

  constexpr auto U32_N_MIN_MIN = MIN_of_<t_u4_n_min>;
  constexpr auto U32_N_MIN_MAX = MAX_of_<t_u4_n_min>;

  constexpr auto U64_N_MIN_MIN = MIN_of_<t_u8_n_min>;
  constexpr auto U64_N_MIN_MAX = MAX_of_<t_u8_n_min>;

  constexpr auto I8_N_MIN_MIN  = MIN_of_<t_i1_n_min>;
  constexpr auto I8_N_MIN_MAX  = MAX_of_<t_i1_n_min>;

  constexpr auto I16_N_MIN_MIN = MIN_of_<t_i2_n_min>;
  constexpr auto I16_N_MIN_MAX = MAX_of_<t_i2_n_min>;

  constexpr auto I32_N_MIN_MIN = MIN_of_<t_i4_n_min>;
  constexpr auto I32_N_MIN_MAX = MAX_of_<t_i4_n_min>;

  constexpr auto I64_N_MIN_MIN = MIN_of_<t_i8_n_min>;
  constexpr auto I64_N_MIN_MAX = MAX_of_<t_i8_n_min>;

  constexpr auto N_MIN_MIN     = MIN_of_<t_n_min>;
  constexpr auto N_MIN_MAX     = MAX_of_<t_n_min>;

  /////////////////////////////////////////////////////////////////////////////

  constexpr auto U8_N_MAX_MIN  = MIN_of_<t_u1_n_max>;
  constexpr auto U8_N_MAX_MAX  = MAX_of_<t_u1_n_max>;

  constexpr auto U16_N_MAX_MIN = MIN_of_<t_u2_n_max>;
  constexpr auto U16_N_MAX_MAX = MAX_of_<t_u2_n_max>;

  constexpr auto U32_N_MAX_MIN = MIN_of_<t_u4_n_max>;
  constexpr auto U32_N_MAX_MAX = MAX_of_<t_u4_n_max>;

  constexpr auto U64_N_MAX_MIN = MIN_of_<t_u8_n_max>;
  constexpr auto U64_N_MAX_MAX = MAX_of_<t_u8_n_max>;

  constexpr auto I8_N_MAX_MIN  = MIN_of_<t_i1_n_max>;
  constexpr auto I8_N_MAX_MAX  = MAX_of_<t_i1_n_max>;

  constexpr auto I16_N_MAX_MIN = MIN_of_<t_i2_n_max>;
  constexpr auto I16_N_MAX_MAX = MAX_of_<t_i2_n_max>;

  constexpr auto I32_N_MAX_MIN = MIN_of_<t_i4_n_max>;
  constexpr auto I32_N_MAX_MAX = MAX_of_<t_i4_n_max>;

  constexpr auto I64_N_MAX_MIN = MIN_of_<t_i8_n_max>;
  constexpr auto I64_N_MAX_MAX = MAX_of_<t_i8_n_max>;

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

  /////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
