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

#ifndef _DAINTY_OOPS_H_
#define _DAINTY_OOPS_H_

// oops: 'used to show recognition of a mistake or minor accident, ...'
//
// namespace <oops::>
//
//   is a small framework consisting of classes that formalize error handling
//   and encourage a certain convention/semantic.
//
//
//   motivation: <oops::> is the result of converging goals
//
//     - enforce error classification and handling.
//     - promote seperation of expected code execution from error handling code.
//     - promote and even force convention/semantic.
//     - allow for the deferral of decisions.
//     - strict interface description encouraged.
//     - static polyphormish allow for behaviour change without changing
//       use code.
//     - enhanced tracing supported.
//     - enhanced test possibilties because of what the classes knows.
//
//
//  terminology:
//
//    error:
//      an t_oops instance, also known as an oops, is synonymous to an error.
//
//    publish:
//      a valid, identifiable id is assigned to an error.
//
//    handling, handle block:
//      code block where an error will be inspected and an apporeriate
//      handling is done of the error.
//
//
//  error handling consists of:
//
//    - detection
//    - classification
//    - publish
//    - handling
//
//
//  framework:
//
//   - every error must be defined.
//     1. an error is identified by a id (enum or integer)
//     2. an error has a category: recoverable, unrecoverable, ignoreable.
//     3. an error has a description string.
//     4. an error can have a user specified property, like callback.
//
//   - every error must be handled.
//     1. an error owner must clear an error, when set, before it perish.
//     1. who can clear an error depends where it was published.
//
//   - how to react to an error, in part depends on the error context,
//     which is represented with t_ctxt (when not specialized) that allows
//     it self static specializations of its policy and printing.
//     1. default policy will assert when an unrecoverable error is set.
//     2. default policy will throw when a recoverable error is set.
//     3. default policy will do nothing when an ignorable error is set.
//
//     note: you can write your own context class, or provide your own
//           policy and/or printing functions.
//
//
// additional notes:
//
//     error clasification
//       - you must provide definitions which force you to think about your
//         errors.
//       - t_oops<...> can co-exist. should be defined. its now part of
//         your interface.
//       - can be used for program flow and handling of errors.
//
//     error publishing and handling
//       - error is set by assigning a value. this can trigger an assert,
//         an exception or just a value set. custom behavior can be defined
//         using a custom policy function or using a custom context.
//
//     support some of exception handling semantics:
//       - must handle at some point otherwise abort.
//       - must handle up the callstack.
//       - can only hold one error at a time.
//       - can only be cleared when set to void overuse of clear().
//       - can separate "normal" from the "error handling" using
//         deferred error handling that is innate to the use of t_oops.
//       - support c++ exception handling (t_except). does it by
//         default. see the default policy.
//
//     additional support:
//       - tag calls to determine "at what point" an error id is set.
//       - mark blocks to identify what will be executed next if no error is
//         set.
//       - defer to t_ctxt(temnplate) so behaviour, policy can change without
//          chaning existing code.
//
// two flags:
//
//   DAINTY_OOPS_BASIC  - only weak enforcement required.
//                 reduce t_oops use overhead, and no debug possible.
//   DAINTY_OOPS_TRACE  - track and print use path of owner
//

#include "dainty_base_assert.h"
#include "dainty_oops_ctxt.h"

namespace dainty
{
namespace oops
{
  using base::assert_now;

////////////////////////////////////////////////////////////////////////////////

  class t_except {
  public:
    inline t_except()               noexcept : ctxt_(nullptr) { }
    inline t_except(P_void ctxt)    noexcept : ctxt_(ctxt)    { }
    inline operator t_bool () const noexcept                  { return ctxt_; }

  private:
    template<p_what, class, class> friend class t_oops;
    P_void ctxt_;
  };
  using R_except = t_prefix<t_except>::R_;

////////////////////////////////////////////////////////////////////////////////

#ifndef DAINTY_OOPS_CTXT
  #define DAINTY_OOPS_CTXT t_ctxt<>
#else
  #include "dainty_oops_user.h"
#endif

  template<p_what WHAT = default_what, typename ID = t_id,
           typename CTXT = DAINTY_OOPS_CTXT>
  class t_oops {
  public:
    using r_oops = typename t_prefix<t_oops>::r_;
    using R_oops = typename t_prefix<t_oops>::R_;
    using x_oops = typename t_prefix<t_oops>::x_;
    using t_ctxt = typename t_prefix<CTXT>::t_;
    using p_ctxt = typename t_prefix<CTXT>::p_;
    using R_id   = typename t_prefix<ID>::R_;

    t_oops()       noexcept;
    t_oops(p_ctxt) noexcept;
    t_oops(R_oops) noexcept;
    template<p_what WHAT1, typename ID1, typename CTXT1>
    t_oops(t_oops<WHAT1, ID1, CTXT1>&) noexcept;
   ~t_oops();

    r_oops operator=(x_oops) = delete; // explicit
    r_oops operator=(R_oops) = delete; // explicit

    r_oops mark_block(P_filename, t_lineno) noexcept;
    r_oops tag       (t_tagid)              noexcept;

    r_oops operator=(R_id);
    r_oops operator=(R_info);

    t_info clear() noexcept;

    operator t_validity()         const noexcept;
    operator t_bool    ()         const noexcept;
    operator t_errn    ()         const noexcept;

    t_id     id        ()         const noexcept;
    t_tagid  tag       ()         const noexcept;
    t_bool   is_set    (r_info)   const noexcept;
    P_cstr   what      ()         const noexcept;
    t_void   print     ()         const noexcept;
    t_bool   knows     (R_except) const noexcept;

  private:
    template<p_what, typename, typename> friend class t_oops;

    p_ctxt ctxt_;
    t_data data_;
  };

////////////////////////////////////////////////////////////////////////////////

  template<p_what WHAT, typename ID, typename CTXT>
  inline
  t_bool operator==(const t_oops<WHAT, ID, CTXT>& lh, t_validity rh) noexcept {
    t_validity lh_valid = lh;
    return lh_valid == rh;
  }

  template<p_what WHAT, typename ID, typename CTXT>
  inline
  t_bool operator==(const t_oops<WHAT, ID, CTXT>& lh, t_errn rh) noexcept {
    t_errn lh_errn = lh;
    return lh_errn == rh;
  }

////////////////////////////////////////////////////////////////////////////////

#define DAINTY_OOPS_POSITION    dainty::oops::P_filename{__FILE__}, __LINE__
#define DAINTY_OOPS_BLOCK_GUARD(oops)             \
  (!oops.mark_block(DAINTY_OOPS_POSITION))
#define DAINTY_OOPS_BLOCK_GUARD_TAG(oops, id)     \
  (!oops.tag(id).mark_block(DAINTY_OOPS_POSITION))

////////////////////////////////////////////////////////////////////////////////

  template<p_what WHAT, typename ID, typename CTXT>
  inline
  t_oops<WHAT, ID, CTXT>::t_oops() noexcept
    : ctxt_{new t_ctxt}, data_{true, true} {
  }

  template<p_what WHAT, typename ID, typename CTXT>
  inline
  t_oops<WHAT, ID, CTXT>::t_oops(p_ctxt ctxt) noexcept
      : ctxt_{ctxt}, data_{true, false} {
    if (!ctxt_)
      assert_now(P_cstr{"oops->invalid_context"});
  }

  template<p_what WHAT, typename ID, typename CTXT>
  inline
  t_oops<WHAT, ID, CTXT>::t_oops(R_oops oops) noexcept
#ifndef DAINTY_OOPS_BASIC
    : ctxt_{oops.ctxt_}, data_(false, false, oops.data_.depth + 1) {
#else
    : ctxt_{oops.ctxt_}, data_{false, false} {
#endif
#ifdef DAINTY_OOPS_TRACE
    ctxt_->step_in(data_, WHAT);
#endif
  }

  template<p_what WHAT,  typename ID,  typename CTXT>
  template<p_what WHAT1, typename ID1, typename CTXT1>
  inline
  t_oops<WHAT, ID, CTXT>::t_oops(t_oops<WHAT1, ID1, CTXT1>& oops) noexcept
#ifndef DAINTY_OOPS_BASIC
    : ctxt_{oops.ctxt_}, data_(false, false, oops.data_.depth + 1) {
#else
    : ctxt_{oops.ctxt_}, data_{false, false} {
#endif
#ifdef DAINTY_OOPS_TRACE
    ctxt_->step_in(oops.data_, WHAT);
#endif
  }

  template<p_what WHAT, typename ID, typename CTXT>
  inline
  t_oops<WHAT, ID, CTXT>::~t_oops() {
#ifdef DAINTY_OOPS_TRACE
    ctxt_->step_out(data_, WHAT);
#endif
    if (data_.owner) {
      T_bool on = id();
      if (on)
        assert_now(P_cstr{"oops->unhandled"});
      if (data_.mem)
        delete ctxt_;
    }
  }

  template<p_what WHAT, typename ID, typename CTXT>
  inline
  t_oops<WHAT, ID, CTXT>::operator t_validity() const noexcept {
    return ctxt_ ? VALID : INVALID;
  }

  template<p_what WHAT, typename ID, typename CTXT>
  inline
  t_oops<WHAT, ID, CTXT>::operator t_bool() const noexcept {
    return id();
  }

  template<p_what WHAT, typename ID, typename CTXT>
  inline
  t_oops<WHAT, ID, CTXT>::operator t_errn() const noexcept {
    return t_errn(id());
  }

  template<p_what WHAT, typename ID, typename CTXT>
  inline
  t_id t_oops<WHAT, ID, CTXT>::id() const noexcept {
    return ctxt_->get_id();
  }

  template<p_what WHAT, typename ID, typename CTXT>
  inline
  t_tagid t_oops<WHAT, ID, CTXT>::tag() const noexcept {
    return data_.tag;
  }

  template<p_what WHAT, typename ID, typename CTXT>
  inline
  t_bool t_oops<WHAT, ID, CTXT>::is_set(r_info info) const noexcept {
    T_bool on = id();
    if (on)
      info = ctxt_->get_info();
    return on;
  }

  template<p_what WHAT, typename ID, typename CTXT>
  inline
  P_cstr t_oops<WHAT, ID, CTXT>::what() const noexcept {
    return ctxt_->what();
  }

  template<p_what WHAT, typename ID, typename CTXT>
  inline
  void t_oops<WHAT, ID, CTXT>::print() const noexcept {
    ctxt_->print(data_);
  }

  template<p_what WHAT, typename ID, typename CTXT>
  inline
  typename t_oops<WHAT, ID, CTXT>::r_oops
      t_oops<WHAT, ID, CTXT>::operator=(R_id value) {
    if (value) {
      T_bool on = id();
      if (!on) {
#ifndef DAINTY_OOPS_BASIC
        data_.set = true;
#endif
        ctxt_->set(value, WHAT, data_);
      } else
        assert_now(P_cstr{"oops->already_set"});
    } else
      assert_now(P_cstr{"oops->use_clear"});
    return *this;
  }

  template<p_what WHAT, typename ID, typename CTXT>
  inline
  typename t_oops<WHAT, ID, CTXT>::r_oops
      t_oops<WHAT, ID, CTXT>::operator=(R_info info) {
    if (info.id && info.what) {
      const t_bool on = id();
      if (!on) {
#ifndef DAINTY_OOPS_BASIC
        data_.set = true;
#endif
        ctxt_->set(info);
      } else
        assert_now(P_cstr{"oops->already_set"});
    } else
      assert_now(P_cstr{"oops->invalid_info"});
    return *this;
  }

  template<p_what WHAT, typename ID, typename CTXT>
  inline
  typename t_oops<WHAT, ID, CTXT>::r_oops
      t_oops<WHAT, ID, CTXT>::mark_block(P_filename file,
                                         t_lineno   line) noexcept {
#ifndef DAINTY_OOPS_BASIC
    data_.file = file;
    data_.line = line;
#endif
#ifdef DAINTY_OOPS_TRACE
    ctxt_->step_do(data_, WHAT);
#endif
    return *this;
  }

  template<p_what WHAT, typename ID, typename CTXT>
  inline
  typename t_oops<WHAT, ID, CTXT>::r_oops
      t_oops<WHAT, ID, CTXT>::tag(t_tagid tag) noexcept {
    if (!id())
      data_.tag = tag;
    return *this;
  }

  template<p_what WHAT, typename ID, typename CTXT>
  inline
  t_info t_oops<WHAT, ID, CTXT>::clear() noexcept {
    if (!id())
      assert_now(P_cstr{"oops->nothing_to_clear"});
#ifndef DAINTY_OOPS_BASIC
    T_depth depth = ctxt_->get_depth();
    if (data_.depth > depth || (data_.depth == depth && !data_.set))
      assert_now(P_cstr{"oops->cannot_be_cleared"});
    data_.set = false;
#endif
    return ctxt_->clear();
  }

  template<p_what WHAT, typename ID, typename CTXT>
  inline
  t_bool t_oops<WHAT, ID, CTXT>::knows(R_except except) const noexcept {
    return ctxt_ == except.ctxt_;
  }
}
}

#endif
