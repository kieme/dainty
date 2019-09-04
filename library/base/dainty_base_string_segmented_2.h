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

#ifndef _DAINTY_BASE_STRING_SEGMENTED_2_H_
#define _DAINTY_BASE_STRING_SEGMENTED_2_H_

#include "dainty_base_string_segmented_impl.h"

namespace dainty
{
namespace base
{
namespace string
{
namespace segmented
{
///////////////////////////////////////////////////////////////////////////////

  using impl_::t_citr;
  using impl_::t_result;
  using impl_::t_user;
  using impl_::t_n;
  using impl_::t_bool;
  using impl_::t_void;
  using impl_::t_char;
  using impl_::t_crange;
  using impl_::t_seg_no;
  using impl_::t_id;

  using impl_::BAD_ID;
  using impl_::BAD_RANGE;

  using impl_::operator==;
  using impl_::operator!=;

///////////////////////////////////////////////////////////////////////////////

  template<class TAG, t_n_ N>
  class t_segmented<TAG, N, t_overflow_grow> {
    using t_impl_ = impl_::t_impl_<t_overflow_grow>;
    using t_self_ = t_segmented<TAG, N, t_overflow_grow>;
    using r_self_ = typename t_prefix<t_self_>::r_;
    using R_self_ = typename t_prefix<t_self_>::R_;
    using x_self_ = typename t_prefix<t_self_>::x_;

  public:
    t_segmented()                 noexcept = default;
    t_segmented(t_char, t_crange) noexcept;
    t_segmented(R_self_)          noexcept;
    t_segmented(x_self_)          noexcept;
    r_self_ operator=(R_self_)    noexcept;
    r_self_ operator=(x_self_)    noexcept;

    template<t_n_ N1, typename O>
    t_segmented(const t_segmented<TAG, N1, O>&) noexcept;
    template<t_n_ N1>
    t_segmented(t_segmented<TAG, N1, t_overflow_grow>&&) noexcept;
    template<typename O>
    t_segmented(t_segmented<TAG, 0, O>&&) noexcept;

    template<t_n_ N1, typename O>
    r_self_ operator=(const t_segmented<TAG, N1, O>&) noexcept;
    template<t_n_ N1>
    r_self_ operator=(t_segmented<TAG, N1, t_overflow_grow>&&) noexcept;
    template<typename O>
    r_self_ operator=(t_segmented<TAG, 0, O>&&) noexcept;

    t_n      get_segs_num()               const noexcept;
    t_n      get_capacity()               const noexcept;
    t_n      get_size    ()               const noexcept;
    t_bool   is_empty    ()               const noexcept;
    t_crange get         (t_seg_no)       const noexcept;
    t_crange get         (t_id)           const noexcept;
    t_crange front       ()               const noexcept;
    t_crange back        ()               const noexcept;
    t_id     find        (t_crange)       const noexcept;
    t_id     find_next   (t_crange, t_id) const noexcept;
    t_crange operator[]  (t_seg_no)       const noexcept;
    t_crange operator[]  (t_id)           const noexcept;

    t_bool   assign   (t_char, t_crange)               noexcept;
    t_result push_back(          t_user)               noexcept;
    t_result push_back(t_crange, t_user = 0)           noexcept;
    t_id     insert   (t_id,               t_user)     noexcept;
    t_id     insert   (t_id,     t_crange, t_user = 0) noexcept;
    t_id     insert   (t_seg_no,           t_user)     noexcept;
    t_id     insert   (t_seg_no, t_crange, t_user = 0) noexcept;
    t_bool   change   (t_seg_no,           t_user)     noexcept;
    t_bool   change   (t_id,               t_user)     noexcept;
    t_bool   change   (t_seg_no, t_crange, t_user = 0) noexcept;
    t_bool   change   (t_id,     t_crange, t_user = 0) noexcept;
    t_bool   remove   (t_seg_no, t_n = t_n{1})         noexcept;
    t_bool   remove   (t_id,     t_n = t_n{1})         noexcept;
    t_void   clear    ()                               noexcept;

    t_citr begin() const noexcept;
    t_citr end  () const noexcept;

    template<typename F>
    t_void each(F&&) const noexcept;

    template<typename BY, typename TO>
    t_void generate(BY&&, TO&&) const noexcept;

    template<class TAG1, t_n_ N1, typename O1>
    t_bool is_equal     (const t_segmented<TAG1, N1, O1>&) const noexcept;

    template<class TAG1, t_n_ N1, typename O1>
    t_bool is_less      (const t_segmented<TAG1, N1, O1>&) const noexcept;

    template<class TAG1, t_n_ N1, typename O1>
    t_bool is_less_equal(const t_segmented<TAG1, N1, O1>&) const noexcept;

  private:
    template<typename, t_n_, typename> friend class t_segmented;

    using t_store_ = buf::t_buf<t_char, N, buf::t_size_dynamic>;

    t_store_ store_;
    t_impl_  impl_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<class TAG, t_n_ N>
  inline
  t_segmented<TAG, N, t_overflow_grow>::
      t_segmented(t_char delimit, t_crange range) noexcept {
    impl_.assign(store_, delimit, range);
  }

  template<class TAG, t_n_ N>
  inline
  t_segmented<TAG, N, t_overflow_grow>::
    t_segmented(R_self_ segmented) noexcept
      : store_{segmented.get_size()} {
    impl_.assign(store_, segmented.begin(), segmented.end());
  }

  template<class TAG, t_n_ N>
  inline
  t_segmented<TAG, N, t_overflow_grow>::
    t_segmented(x_self_ segmented) noexcept
      : store_{segmented.get_size()} {
    impl_.assign(store_, segmented.begin(), segmented.end()); // XXX - copy!
  }

  template<class TAG, t_n_ N>
  inline
  typename t_segmented<TAG, N, t_overflow_grow>::r_self_
    t_segmented<TAG, N, t_overflow_grow>::
      operator=(R_self_ segmented) noexcept {
    impl_.assign(store_, segmented.begin(), segmented.end());
    return *this;
  }

  template<class TAG, t_n_ N>
  inline
  typename t_segmented<TAG, N, t_overflow_grow>::r_self_
    t_segmented<TAG, N, t_overflow_grow>::
      operator=(x_self_ segmented) noexcept {
    impl_.assign(store_, segmented.begin(), segmented.end()); // XXX - copy!
    return *this;
  }

  template<class TAG, t_n_ N>
  template<t_n_ N1, typename O>
  inline
  t_segmented<TAG, N, t_overflow_grow>::
    t_segmented(const t_segmented<TAG, N1, O>& segmented) noexcept
      : store_{segmented.get_size()} {
    impl_.assign(store_, segmented.begin(), segmented.end());
  }

  template<class TAG, t_n_ N>
  template<t_n_ N1>
  inline
  t_segmented<TAG, N, t_overflow_grow>::
    t_segmented(t_segmented<TAG, N1, t_overflow_grow>&& segmented) noexcept
      : store_{segmented.get_size()} {
    impl_.assign(store_, segmented.begin(), segmented.end()); // XXX - copy!
  }

  template<class TAG, t_n_ N>
  template<typename O>
  inline
  t_segmented<TAG, N, t_overflow_grow>::
    t_segmented(t_segmented<TAG, 0, O>&& segmented) noexcept
      : store_{segmented.get_size()} {
    impl_.assign(store_, segmented.begin(), segmented.end()); // XXX - copy!
  }

  template<class TAG, t_n_ N>
  template<t_n_ N1, typename O>
  inline
  typename t_segmented<TAG, N, t_overflow_grow>::r_self_
    t_segmented<TAG, N, t_overflow_grow>::
      operator=(const t_segmented<TAG, N1, O>& segmented) noexcept {
    impl_.assign(store_, segmented.begin(), segmented.end());
    return *this;
  }

  template<class TAG, t_n_ N>
  template<t_n_ N1>
  inline
  typename t_segmented<TAG, N, t_overflow_grow>::r_self_
    t_segmented<TAG, N, t_overflow_grow>::
      operator=(t_segmented<TAG, N1, t_overflow_grow>&& segmented) noexcept {
    impl_.assign(store_, segmented.begin(), segmented.end()); // XXX - copy!
    return *this;
  }

  template<class TAG, t_n_ N>
  template<typename O>
  inline
  typename t_segmented<TAG, N, t_overflow_grow>::r_self_
    t_segmented<TAG, N, t_overflow_grow>::
      operator=(t_segmented<TAG, 0, O>&& segmented) noexcept {
    impl_.assign(store_, segmented.begin(), segmented.end()); // XXX - copy!
    return *this;
  }

  template<class TAG, t_n_ N>
  inline
  t_n t_segmented<TAG, N, t_overflow_grow>::get_segs_num() const noexcept {
    return impl_.get_segs_num();
  }

  template<class TAG, t_n_ N>
  inline
  t_n t_segmented<TAG, N, t_overflow_grow>::get_capacity() const noexcept {
    return store_.get_capacity();
  }

  template<class TAG, t_n_ N>
  inline
  t_n t_segmented<TAG, N, t_overflow_grow>::get_size() const noexcept {
    return impl_.get_size();
  }

  template<class TAG, t_n_ N>
  inline
  t_bool t_segmented<TAG, N, t_overflow_grow>::is_empty() const noexcept {
    return impl_.is_empty();
  }

  template<class TAG, t_n_ N>
  inline
  t_bool t_segmented<TAG, N, t_overflow_grow>::assign(t_char delimit,
                                                      t_crange range) noexcept {
    return impl_.assign(store_, delimit, range);
  }

  template<class TAG, t_n_ N>
  inline
  t_result t_segmented<TAG, N, t_overflow_grow>
      ::push_back(t_user user) noexcept {
    return impl_.push_back(store_, user);
  }

  template<class TAG, t_n_ N>
  inline
  t_result t_segmented<TAG, N, t_overflow_grow>
      ::push_back(t_crange range, t_user user) noexcept {
    return impl_.push_back(store_, range, user);
  }

  template<class TAG, t_n_ N>
  inline
  t_id t_segmented<TAG, N, t_overflow_grow>::insert(t_id id,
                                                    t_user user) noexcept {
    return impl_.insert(store_, id, user);
  }

  template<class TAG, t_n_ N>
  inline
  t_id t_segmented<TAG, N, t_overflow_grow>::insert(t_id id, t_crange range,
                                                    t_user user) noexcept {
    return impl_.insert(store_, id, range, user);
  }

  template<class TAG, t_n_ N>
  inline
  t_id t_segmented<TAG, N, t_overflow_grow>::insert(t_seg_no seg_no,
                                                    t_user user) noexcept {
    return impl_.insert(store_, seg_no, user);
  }

  template<class TAG, t_n_ N>
  inline
  t_id t_segmented<TAG, N, t_overflow_grow>::insert(t_seg_no seg_no,
                                                    t_crange range,
                                                    t_user user) noexcept {
    return impl_.insert(store_, seg_no, range, user);
  }

  template<class TAG, t_n_ N>
  inline
  t_bool t_segmented<TAG, N, t_overflow_grow>::change(t_id id,
                                                      t_user user) noexcept {
    return impl_.change(store_, id, user);
  }

  template<class TAG, t_n_ N>
  inline
  t_bool t_segmented<TAG, N, t_overflow_grow>::change(t_seg_no seg_no,
                                                      t_user user) noexcept {
    return impl_.change(store_, seg_no, user);
  }

  template<class TAG, t_n_ N>
  inline
  t_bool t_segmented<TAG, N, t_overflow_grow>::change(t_id id, t_crange range,
                                                      t_user user) noexcept {
    return impl_.change(store_, id, range, user);
  }

  template<class TAG, t_n_ N>
  inline
  t_bool t_segmented<TAG, N, t_overflow_grow>::change(t_seg_no seg_no,
                                                      t_crange range,
                                                      t_user user) noexcept {
    return impl_.change(store_, seg_no, range, user);
  }

  template<class TAG, t_n_ N>
  inline
  t_bool t_segmented<TAG, N, t_overflow_grow>::remove(t_id id, t_n n) noexcept {
    return impl_.remove(store_, id, n);
  }

  template<class TAG, t_n_ N>
  inline
  t_bool t_segmented<TAG, N, t_overflow_grow>
      ::remove(t_seg_no seg_no, t_n n) noexcept {
    return impl_.remove(store_, seg_no, n);
  }

  template<class TAG, t_n_ N>
  inline
  t_void t_segmented<TAG, N, t_overflow_grow>::clear() noexcept {
    return impl_.clear();
  }

  template<class TAG, t_n_ N>
  inline
  t_crange t_segmented<TAG, N, t_overflow_grow>::get(t_id id) const noexcept {
    return impl_.get(store_, id);
  }

  template<class TAG, t_n_ N>
  inline
  t_crange t_segmented<TAG, N, t_overflow_grow>
      ::get(t_seg_no seg_no) const noexcept {
    return impl_.get(store_, seg_no);
  }

  template<class TAG, t_n_ N>
  inline
  t_crange t_segmented<TAG, N, t_overflow_grow>::front() const noexcept {
    return impl_.front(store_);
  }

  template<class TAG, t_n_ N>
  inline
  t_crange t_segmented<TAG, N, t_overflow_grow>::back() const noexcept {
    return impl_.back(store_);
  }

  template<class TAG, t_n_ N>
  inline
  t_id t_segmented<TAG, N, t_overflow_grow>
      ::find(t_crange range) const noexcept {
    return impl_.find(store_, range);
  }

  template<class TAG, t_n_ N>
  inline
  t_id t_segmented<TAG, N, t_overflow_grow>
      ::find_next(t_crange range, t_id id) const noexcept {
    return impl_.find_next(store_, range, id);
  }

  template<class TAG, t_n_ N>
  inline
  t_crange t_segmented<TAG, N, t_overflow_grow>
      ::operator[](t_id id) const noexcept {
    return impl_.get(store_, id);
  }

  template<class TAG, t_n_ N>
  inline
  t_crange t_segmented<TAG, N, t_overflow_grow>
      ::operator[](t_seg_no seg_no) const noexcept {
    return impl_.get(store_, seg_no);
  }

  template<class TAG, t_n_ N>
  inline
  t_citr t_segmented<TAG, N, t_overflow_grow>::begin() const noexcept {
    return impl_.begin(store_);
  }

  template<class TAG, t_n_ N>
  inline
  t_citr t_segmented<TAG, N, t_overflow_grow>::end() const noexcept {
    return impl_.end(store_);
  }

  template<class TAG, t_n_ N>
  template<typename F>
  inline
  t_void t_segmented<TAG, N, t_overflow_grow>::each(F&& func) const noexcept {
    return impl_.each(store_, preserve<F>(func));
  }

  template<class TAG, t_n_ N>
  template<typename BY, typename TO>
  t_void t_segmented<TAG, N, t_overflow_grow>
      ::generate(BY&& by, TO&& to) const noexcept {
    return impl_.generate(store_, preserve<BY>(by), preserve<TO>(to));
  }

  template<class TAG,  t_n_ N>
  template<class TAG1, t_n_ N1, typename O1>
  inline
  t_bool t_segmented<TAG, N, t_overflow_grow>
      ::is_equal(const t_segmented<TAG1, N1, O1>& segs) const noexcept {
    return impl_.is_equal(store_, segs.store_, segs.impl_);
  }

  template<class TAG,  t_n_ N>
  template<class TAG1, t_n_ N1, typename O1>
  inline
  t_bool t_segmented<TAG, N, t_overflow_grow>
      ::is_less(const t_segmented<TAG1, N1, O1>& segs) const noexcept {
    return impl_.is_less(store_, segs.store_, segs.impl_);
  }

  template<class TAG,  t_n_ N>
  template<class TAG1, t_n_ N1, typename O1>
  inline
  t_bool t_segmented<TAG, N, t_overflow_grow>
      ::is_less_equal(const t_segmented<TAG1, N1, O1>& segs) const noexcept {
    return impl_.is_less_equal(store_, segs.store_, segs.impl_);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}
}

#endif
