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

#ifndef _DAINTY_BASE_STRING_SEGMENTED_4_H_
#define _DAINTY_BASE_STRING_SEGMENTED_4_H_

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

  template<class TAG>
  class t_segmented<TAG, 0, t_overflow_grow> {
    using t_impl_ = t_segmented_impl_<t_overflow_grow>;
  public:
    t_segmented() noexcept : impl_{store_} { }

    // copy? - why not

    t_n_     get_segs_num()               const noexcept;
    t_n_     get_capacity()               const noexcept;
    t_n_     get_size    ()               const noexcept;
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
    t_void generate(BY&&, TO&&) noexcept;

    template<class TAG1, t_n_ N1, typename O1>
    t_bool is_equal     (const t_segmented<TAG1, N1, O1>&) const noexcept;

    template<class TAG1, t_n_ N1, typename O1>
    t_bool is_less      (const t_segmented<TAG1, N1, O1>&) const noexcept;

    template<class TAG1, t_n_ N1, typename O1>
    t_bool is_less_equal(const t_segmented<TAG1, N1, O1>&) const noexcept;

  private:
    template<typename, t_n_, typename> friend class t_segmented;

    // XXX
    t_n_    max_  = 0;
    p_char  store_;
    t_impl_ impl_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<class TAG>
  inline
  t_n_ t_segmented<TAG, 0, t_overflow_grow>::get_segs_num() const noexcept {
    return impl_.get_segs_num();
  }

  template<class TAG>
  inline
  t_n_ t_segmented<TAG, 0, t_overflow_grow>::get_capacity() const noexcept {
    return max_;
  }

  template<class TAG>
  inline
  t_n_ t_segmented<TAG, 0, t_overflow_grow>::get_size() const noexcept {
    return impl_.get_size(store_);
  }

  template<class TAG>
  inline
  t_bool t_segmented<TAG, 0, t_overflow_grow>::is_empty() const noexcept {
    return impl_.is_empty();
  }

  template<class TAG>
  inline
  t_result t_segmented<TAG, 0, t_overflow_grow>
      ::push_back(t_user user) noexcept {
    return impl_.push_back(store_, store_ + max_, user);
  }

  template<class TAG>
  inline
  t_result t_segmented<TAG, 0, t_overflow_grow>::
      push_back(t_crange range, t_user user) noexcept {
    return impl_.push_back(store_, store_ + max_, range, user);
  }

  template<class TAG>
  inline
  t_id t_segmented<TAG, 0, t_overflow_grow>::insert(t_id id,
                                                    t_user user) noexcept {
    return impl_.insert(store_, store_ + max_, id, user);
  }

  template<class TAG>
  inline
  t_id t_segmented<TAG, 0, t_overflow_grow>::insert(t_id id, t_crange range,
                                                    t_user user) noexcept {
    return impl_.insert(store_, store_ + max_, id, range, user);
  }

  template<class TAG>
  inline
  t_id t_segmented<TAG, 0, t_overflow_grow>::insert(t_seg_no seg_no,
                                                    t_user user) noexcept {
    return impl_.insert(store_, store_ + max_, seg_no, user);
  }

  template<class TAG>
  inline
  t_id t_segmented<TAG, 0, t_overflow_grow>::insert(t_seg_no seg_no,
                                                    t_crange range,
                                                    t_user user) noexcept {
    return impl_.insert(store_, store_ + max_, seg_no, range, user);
  }

  template<class TAG>
  inline
  t_bool t_segmented<TAG, 0, t_overflow_grow>::change(t_id id,
                                                      t_user user) noexcept {
    return impl_.change(store_, id, user);
  }

  template<class TAG>
  inline
  t_bool t_segmented<TAG, 0, t_overflow_grow>::change(t_seg_no seg_no,
                                                      t_user user) noexcept {
    return impl_.change(store_, seg_no, user);
  }

  template<class TAG>
  inline
  t_bool t_segmented<TAG, 0, t_overflow_grow>::change(t_id id, t_crange range,
                                                      t_user user) noexcept {
    return impl_.change(store_, store_ + max_, id, range, user);
  }

  template<class TAG>
  inline
  t_bool t_segmented<TAG, 0, t_overflow_grow>::change(t_seg_no seg_no,
                                                      t_crange range,
                                                      t_user user) noexcept {
    return impl_.change(store_, store_ + max_, seg_no, range, user);
  }

  template<class TAG>
  inline
  t_bool t_segmented<TAG, 0, t_overflow_grow>::remove(t_id id, t_n n) noexcept {
    return impl_.remove(store_, id, n);
  }

  template<class TAG>
  inline
  t_bool t_segmented<TAG, 0, t_overflow_grow>
      ::remove(t_seg_no seg_no, t_n n) noexcept {
    return impl_.remove(store_, seg_no, n);
  }

  template<class TAG>
  inline
  t_void t_segmented<TAG, 0, t_overflow_grow>::clear() noexcept {
    return impl_.clear(store_);
  }

  template<class TAG>
  inline
  t_crange t_segmented<TAG, 0, t_overflow_grow>::get(t_id id) const noexcept {
    return impl_.get(store_, id);
  }

  template<class TAG>
  inline
  t_crange t_segmented<TAG, 0, t_overflow_grow>
      ::get(t_seg_no seg_no) const noexcept {
    return impl_.get(store_, seg_no);
  }

  template<class TAG>
  inline
  t_crange t_segmented<TAG, 0, t_overflow_grow>::front() const noexcept {
    return impl_.front(store_);
  }

  template<class TAG>
  inline
  t_crange t_segmented<TAG, 0, t_overflow_grow>::back() const noexcept {
    return impl_.back(store_);
  }

  template<class TAG>
  inline
  t_id t_segmented<TAG, 0, t_overflow_grow>
      ::find(t_crange range) const noexcept {
    return impl_.find(store_, range);
  }

  template<class TAG>
  inline
  t_id t_segmented<TAG, 0, t_overflow_grow>
      ::find_next(t_crange range, t_id id) const noexcept {
    return impl_.find_next(store_, range, id);
  }

  template<class TAG>
  inline
  t_crange t_segmented<TAG, 0, t_overflow_grow>
      ::operator[](t_id id) const noexcept {
    return impl_.get(store_, id);
  }

  template<class TAG>
  inline
  t_crange t_segmented<TAG, 0, t_overflow_grow>
      ::operator[](t_seg_no seg_no) const noexcept {
    return impl_.get(store_, seg_no);
  }

  template<class TAG>
  inline
  t_citr t_segmented<TAG, 0, t_overflow_grow>::begin() const noexcept {
    return impl_.begin(store_);
  }

  template<class TAG>
  inline
  t_citr t_segmented<TAG, 0, t_overflow_grow>::end() const noexcept {
    return impl_.end();
  }

  template<class TAG>
  template<typename F>
  inline
  t_void t_segmented<TAG, 0, t_overflow_grow>::each(F&& func) const noexcept {
    return impl_.each(store_, preserve<F>(func));
  }

  template<class TAG>
  template<typename BY, typename TO>
  t_void t_segmented<TAG, 0, t_overflow_grow>
      ::generate(BY&& by, TO&& to) noexcept {
    return impl_.generate(store_, preserve<BY>(by), preserve<TO>(to));
  }

  template<class TAG>
  template<class TAG1, t_n_ N1, typename O1>
  inline
  t_bool t_segmented<TAG, 0, t_overflow_grow>
      ::is_equal(const t_segmented<TAG1, N1, O1>& segs) const noexcept {
    return impl_.is_equal(store_, segs.store_, segs.impl_);
  }

  template<class TAG>
  template<class TAG1, t_n_ N1, typename O1>
  inline
  t_bool t_segmented<TAG, 0, t_overflow_grow>
      ::is_less(const t_segmented<TAG1, N1, O1>& segs) const noexcept {
    return impl_.is_less(store_, segs.store_, segs.impl_);
  }

  template<class TAG>
  template<class TAG1, t_n_ N1, typename O1>
  inline
  t_bool t_segmented<TAG, 0, t_overflow_grow>
      ::is_less_equal(const t_segmented<TAG1, N1, O1>& segs) const noexcept {
    return impl_.is_less_equal(store_, segs.store_, segs.impl_);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}
}

#endif
