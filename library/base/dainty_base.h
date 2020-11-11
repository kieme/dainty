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

#ifndef _DAINTY_BASE_H_
#define _DAINTY_BASE_H_

#include "dainty_base_types.h"
#include "dainty_base_buf.h"
#include "dainty_base_valuestore.h"
#include "dainty_base_ptr.h"
#include "dainty_base_range.h"
#include "dainty_base_traits.h"
#include "dainty_base_string.h"
#include "dainty_base_terminal.h"
#include "dainty_base_numeric.h"
#include "dainty_base_specific.h"
#include "dainty_base_logical.h"
#include "dainty_base_util.h"
#include "dainty_base_assert.h"

// base: 'give a name to'
//
//   <base> name builtin types and specify a simple naming convention
//   that should be followed by projects that use it.
//
//   the focus was on the embedded world that often use c++03.
//   (reason for not using type traits, constexpr, etc.). The API will
//   be changed to support c++17 features.
//
//   basic naming convention:
//
//     N01. use lowercase characters except for a few cases mentioned below.
//     N02. functions, method names must be verbs.
//     N03. classes, struct, types names must be nouns.
//     N04. literal and constant values should be written in capital letters.
//     N05. all types names are prefixed. (t_prefix<xxx>)
//     N06. types must be prefixed with t_.
//          (avoid conflict with std:: convention).
//     N07. pointer definitions must be prefixed p_/P_.
//     N08. lvalue reference definitions must be prefixed r_/R_.
//     N09. rvalue reference definitions must be prefixed x_.
//     N10. a capital prefix indicates the content is const.
//     N11. internal functions, methods, variables, types must have an
//          postfix _. API users should never use these.
//     N12. when using <dainty::base> builtin types on interfaces, use
//          specializations of t_specific, to assign purpose to a type.
//     N13. use '_' as delimiter within long names to improve readability.
//     N14. use <dainty::base> builtin types instead of <std>.
//     N15. use qualified names or "using " when using <dainty::base>.
//     N16. prefix a const "type" name with c but not the parameter itself.
//           e.g. t_id  id; -> non-const id.
//                t_cid id; -> const id. but const is not identifyable with
//                             the variable name.
//     N17. when writing classes/structs using the following naming convention
//          for function parameters, data memebers and types.
//          // -> t_param, param, param_
//          e.g class t_class {
//              public:
//                using t_param = ... ;
//                void set(const t_param& param) {
//                  param_ = param;
//                }
//              private:
//                t_param param_; // param_ is internal
//              };
//     N18.1 when a naming conflict exit, prefix a variable with _.
//           struct t_pair {
//             t_first  first;   // first is not internal
//             t_second second;
//             t_pair(t_first _first, t_second _second)
//               : first(_first), second{_second) {
//             }
//           };
//
//   predefined t_specific types for interface use:
//
//      Z01: use type t_ix that is intended to index into something.
//      Z02: use type t_n that is intended to represent a number of elements.
//      Z03: use t_validity to express if something is valid.
//
//   file naming convention:
//
//      F01: dainty_<namespace>.h of a namespace located withint dainty.
//      F02: dainty_<namespace>_<purpose>.h a functionality within namespace.
//      F03: the cpp files names are the same as the associated header files.
//           dainty_oops.h and dainty_oops.cpp
//      F04: example cpp files use the name of the functionality.
//           dainty_oops.cpp then dainty_oops_example<n>.cpp
//      F05: test cpp files use the name of the functionality.
//           dainty_oops.cpp then dainty_oops_test<n>.cpp
//
//   design naming: <xxx> is a functionality.
//
//     data:
//
//      D01: t_<xxx>_params - this is the params of a
//      D02: t_<xxx>_stats  - counter statit
//      D03: t_<xxx>_info   - both runtime, params and other visible properties.
//      D04: t_<xxx>_ctxt   - embeds info and containers private runtime properties.
//      D05: t_<xxx>_data   - encapculates all internal data and provide methods
//                       to consistently manipulate that data.
//
//     operations:
//
//      D10: t_<xxx>_logic - where specific logic is located.
//                           it typically embeds t_<xxx>_data.
//

// should set be allowed. its meant only to transfer data through an interface.
//
// t_specific:
//
//   note: with t_specific<> assign a purpose to a builtin type.
//
//   e.g. the demonstation use the classic set_point(int x, int y) example.
//
//   enum t_x_ { };
//   enum t_y_ { };
//   typedef base::t_specific<base::t_uint, t_x_> t_xpoint;
//   typedef base::t_specific<base::t_uint, t_y_> t_ypoint;
//
//   void set_point(t_xpoint x, t_ypoint y);
//   void set_point(int x, int y);
//
//   void set_point(5, 10);
//   void set_point(xpoint(5), ypoint(10));
//
//  Why is such a feature not part of the cpp language?
//
//    e.g. using t_xpoint = explicit unsigned int; // make type t_xpoint
//         using t_ypoint = explicit unsigned int; // make type t_ypoint
//
//    t_xpoint and t_ypoint are different types.
//
//    unsigned int x = t_xpoint(1) // ok
//    t_xpoint x = 1;              // nok
//    t_xpoint x(1);               // ok
//    t_xpoint x{1};               // ok
//    t_xpoint x(-1);              // nok
//    t_xpoint x{-1)}              // nok

namespace dainty
{
namespace base
{
///////////////////////////////////////////////////////////////////////////////

  using util::t_void_size_ptr;
  using util::t_void_size_cptr;
  using util::t_multiple;
  using util::multiple_of;

  using util::t_id_pair;
  using util::t_block;
  using util::t_verifiable;
  using util::operator==;
  using util::operator!=;

  using types::t_emplace_it;
  using types::t_fmt;
  using types::t_fmt_va;

  using types::EMPLACE_IT;
  using types::FMT;         // XXX better to have a range for it
  using types::FMT_IT;
  using types::FMT_VA_IT;

  using util::reset;
  using util::x_cast;
  using util::swap;

///////////////////////////////////////////////////////////////////////////////
}
}

#endif
