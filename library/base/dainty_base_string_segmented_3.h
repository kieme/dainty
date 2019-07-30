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

#ifndef _DAINTY_BASE_STRING_SEGMENTED_3_H_
#define _DAINTY_BASE_STRING_SEGMENTED_3_H_

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

  template<class TAG, typename O>
  class t_segmented<TAG, 0, O> {
    using t_impl_ = t_segmented_impl_<O>;
    using t_self_ = t_segmented<TAG, 0, O>;
    using r_self_ = typename t_prefix<t_self_>::r_;
    using R_self_ = typename t_prefix<t_self_>::R_;
    using x_self_ = typename t_prefix<t_self_>::x_;

  public:
    t_segmented(t_n max)                   noexcept : store_{max} { }
    t_segmented(t_n max, t_char, t_crange) noexcept;
    t_segmented(const t_segmented&)      = delete;

    template<t_n_ N, typename O1>
    t_segmented(t_n max, const t_segmented<TAG, N, O1>&) noexcept;
    template<t_n_ N>
    t_segmented(t_n max, t_segmented<TAG, N, t_overflow_grow>&&) noexcept;
    template<typename O1>
    t_segmented(t_n max, t_segmented<TAG, 0, O1>&&) noexcept;

    r_self_ operator=(R_self_) noexcept;
    r_self_ operator=(x_self_) noexcept;

    template<t_n_ N, typename O1>
    r_self_ operator=(const t_segmented<TAG, N, O1>&) noexcept;
    template<t_n_ N>
    r_self_ operator=(t_segmented<TAG, N, t_overflow_grow>&&) noexcept;

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

    using t_store_ = buf::t_buf<t_char, 0, buf::t_size_dynamic>;

    t_store_ store_;
    t_impl_  impl_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<class TAG, typename O>
  inline
  t_segmented<TAG, 0, O>::
    t_segmented(t_n max, t_char, t_crange) noexcept {
    // call impl XXX
  }

  template<class TAG, typename O>
  template<t_n_ N, typename O1>
  inline
  t_segmented<TAG, 0, O>::
      t_segmented(t_n max, const t_segmented<TAG, N, O1>& segmented) noexcept
    : store_{max} {
    impl_.assign(store_, segmented.begin(), segmented.end());
  }

  template<class TAG, typename O>
  template<t_n_ N>
  inline
  t_segmented<TAG, 0, O>::
      t_segmented(t_n max,
                  t_segmented<TAG, N, t_overflow_grow>&& segmented) noexcept
    : store_{max} {
    impl_.assign(store_, segmented.begin(), segmented.end()); // XXX - copy!
  }

  template<class TAG, typename O>
  template<typename O1>
  inline
  t_segmented<TAG, 0, O>::
      t_segmented(t_n max, t_segmented<TAG, 0, O1>&& segmented) noexcept
    : store_{max} {
    impl_.assign(store_, segmented.begin(), segmented.end()); // XXX - copy!
  }

  template<class TAG, typename O>
  inline
  typename t_segmented<TAG, 0, O>::r_self_ t_segmented<TAG, 0, O>::
      operator=(R_self_ segmented) noexcept {
    impl_.assign(store_, segmented.begin(), segmented.end());
    return *this;
  }

  template<class TAG, typename O>
  inline
  typename t_segmented<TAG, 0, O>::r_self_ t_segmented<TAG, 0, O>::
      operator=(x_self_ segmented) noexcept {
    impl_.assign(store_, segmented.begin(), segmented.end()); // XXX - copy!
    return *this;
  }

  template<class TAG, typename O>
  template<t_n_ N, typename O1>
  inline
  typename t_segmented<TAG, 0, O>::r_self_ t_segmented<TAG, 0, O>::
      operator=(const t_segmented<TAG, N, O1>& segmented) noexcept {
    impl_.assign(store_, segmented.begin(), segmented.end());
    return *this;
  }

  template<class TAG, typename O>
  template<t_n_ N>
  inline
  typename t_segmented<TAG, 0, O>::r_self_ t_segmented<TAG, 0, O>::
      operator=(t_segmented<TAG, N, t_overflow_grow>&& segmented) noexcept {
    impl_.assign(store_, segmented.begin(), segmented.end()); // XXX - copy!
    return *this;
  }

  template<class TAG, typename O>
  inline
  t_n t_segmented<TAG, 0, O>::get_segs_num() const noexcept {
    return impl_.get_segs_num();
  }

  template<class TAG, typename O>
  inline
  t_n t_segmented<TAG, 0, O>::get_capacity() const noexcept {
    return store_.get_capacity();
  }

  template<class TAG, typename O>
  inline
  t_n t_segmented<TAG, 0, O>::get_size() const noexcept {
    return impl_.get_size();
  }

  template<class TAG, typename O>
  inline
  t_bool t_segmented<TAG, 0, O>::is_empty() const noexcept {
    return impl_.is_empty();
  }

  template<class TAG, typename O>
  inline
  t_result t_segmented<TAG, 0, O>::push_back(t_user user) noexcept {
    return impl_.push_back(store_, user);
  }

  template<class TAG, typename O>
  inline
  t_result t_segmented<TAG, 0, O>::push_back(t_crange range,
                                             t_user user) noexcept {
    return impl_.push_back(store_, range, user);
  }

  template<class TAG, typename O>
  inline
  t_id t_segmented<TAG, 0, O>::insert(t_id id, t_user user) noexcept {
    return impl_.insert(store_, id, user);
  }

  template<class TAG, typename O>
  inline
  t_id t_segmented<TAG, 0, O>::insert(t_id id, t_crange range,
                                      t_user user) noexcept {
    return impl_.insert(store_, id, range, user);
  }

  template<class TAG, typename O>
  inline
  t_id t_segmented<TAG, 0, O>::insert(t_seg_no seg_no, t_user user) noexcept {
    return impl_.insert(store_, seg_no, user);
  }

  template<class TAG, typename O>
  inline
  t_id t_segmented<TAG, 0, O>::insert(t_seg_no seg_no, t_crange range,
                                      t_user user) noexcept {
    return impl_.insert(store_, seg_no, range, user);
  }

  template<class TAG, typename O>
  inline
  t_bool t_segmented<TAG, 0, O>::change(t_id id, t_user user) noexcept {
    return impl_.change(store_, id, user);
  }

  template<class TAG, typename O>
  inline
  t_bool t_segmented<TAG, 0, O>::change(t_seg_no seg_no, t_user user) noexcept {
    return impl_.change(store_, seg_no, user);
  }

  template<class TAG, typename O>
  inline
  t_bool t_segmented<TAG, 0, O>::change(t_id id, t_crange range,
                                        t_user user) noexcept {
    return impl_.change(store_, id, range, user);
  }

  template<class TAG, typename O>
  inline
  t_bool t_segmented<TAG, 0, O>::change(t_seg_no seg_no, t_crange range,
                                        t_user user) noexcept {
    return impl_.change(store_, seg_no, range, user);
  }

  template<class TAG, typename O>
  inline
  t_bool t_segmented<TAG, 0, O>::remove(t_id id, t_n n) noexcept {
    return impl_.remove(store_, id, n);
  }

  template<class TAG, typename O>
  inline
  t_bool t_segmented<TAG, 0, O>::remove(t_seg_no seg_no, t_n n) noexcept {
    return impl_.remove(store_, seg_no, n);
  }

  template<class TAG, typename O>
  inline
  t_void t_segmented<TAG, 0, O>::clear() noexcept {
    return impl_.clear();
  }

  template<class TAG, typename O>
  inline
  t_crange t_segmented<TAG, 0, O>::get(t_id id) const noexcept {
    return impl_.get(store_, id);
  }

  template<class TAG, typename O>
  inline
  t_crange t_segmented<TAG, 0, O>::get(t_seg_no seg_no) const noexcept {
    return impl_.get(store_, seg_no);
  }

  template<class TAG, typename O>
  inline
  t_crange t_segmented<TAG, 0, O>::front() const noexcept {
    return impl_.front(store_);
  }

  template<class TAG, typename O>
  inline
  t_crange t_segmented<TAG, 0, O>::back() const noexcept {
    return impl_.back(store_);
  }

  template<class TAG, typename O>
  inline
  t_id t_segmented<TAG, 0, O>::find(t_crange range) const noexcept {
    return impl_.find(store_, range);
  }

  template<class TAG, typename O>
  inline
  t_id t_segmented<TAG, 0, O>::find_next(t_crange range,
                                         t_id id) const noexcept {
    return impl_.find_next(store_, range, id);
  }

  template<class TAG, typename O>
  inline
  t_crange t_segmented<TAG, 0, O>::operator[](t_id id) const noexcept {
    return impl_.get(store_, id);
  }

  template<class TAG, typename O>
  inline
  t_crange t_segmented<TAG, 0, O>::operator[](t_seg_no seg_no) const noexcept {
    return impl_.get(store_, seg_no);
  }

  template<class TAG, typename O>
  inline
  t_citr t_segmented<TAG, 0, O>::begin() const noexcept {
    return impl_.begin(store_);
  }

  template<class TAG, typename O>
  inline
  t_citr t_segmented<TAG, 0, O>::end() const noexcept {
    return impl_.end(store_);
  }

  template<class TAG, typename O>
  template<typename F>
  inline
  t_void t_segmented<TAG, 0, O>::each(F&& func) const noexcept {
    return impl_.each(store_, preserve<F>(func));
  }

  template<class TAG, typename O>
  template<typename BY, typename TO>
  t_void t_segmented<TAG, 0, O>::generate(BY&& by, TO&& to) const noexcept {
    return impl_.generate(store_, preserve<BY>(by), preserve<TO>(to));
  }

  template<class TAG, typename O>
  template<class TAG1, t_n_ N1, typename O1>
  inline
  t_bool t_segmented<TAG, 0, O>
      ::is_equal(const t_segmented<TAG1, N1, O1>& segs) const noexcept {
    return impl_.is_equal(store_, segs.store_, segs.impl_);
  }

  template<class TAG, typename O>
  template<class TAG1, t_n_ N1, typename O1>
  inline
  t_bool t_segmented<TAG, 0, O>
      ::is_less(const t_segmented<TAG1, N1, O1>& segs) const noexcept {
    return impl_.is_less(store_, segs.store_, segs.impl_);
  }

  template<class TAG, typename O>
  template<class TAG1, t_n_ N1, typename O1>
  inline
  t_bool t_segmented<TAG, 0, O>
      ::is_less_equal(const t_segmented<TAG1, N1, O1>& segs) const noexcept {
    return impl_.is_less_equal(store_, segs.store_, segs.impl_);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}
}

#endif
