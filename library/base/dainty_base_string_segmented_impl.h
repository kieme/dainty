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

#ifndef _DAINTY_BASE_STRING_SEGMENTED_IMPL_H_
#define _DAINTY_BASE_STRING_SEGMENTED_IMPL_H_

#include "dainty_base_string.h"

namespace dainty
{
namespace base
{
namespace string
{
namespace segmented
{
// proper assert
// memory buf

///////////////////////////////////////////////////////////////////////////////

  using string::t_string;
  using string::operator""_SL;
  using string::t_crange; // ranges should have types!
  using string::t_overflow_grow;
  using string::t_overflow_truncate;
  using string::t_overflow_assert;

  using t_user = t_uchar; // only support values 1-63

  enum  t_seg_no_tag_ { };
  using t_seg_no_ = base::t_n_;
  using t_seg_no  = base::t_explicit<t_seg_no_, t_seq_no_tag_>;

  enum  t_id_tag_ { };
  using t_id_ = base::t_ix_;
  using t_id  = base::t_explicit<t_ix_, t_id_tag_>;

  constexpr t_id     BAD_ID   {0};
  constexpr t_crange BAD_RANGE;

///////////////////////////////////////////////////////////////////////////////

  union t_seg_hdr_ {
    t_uint16 value;
    t_char   raw[sizeof(t_uint16)];
    struct t_hdr_ {
      t_uint16 len : 10; // use 10 bits for 1023 max length per segment
      t_uint16 usr :  6; // use 6  bits for 1-63 user codes, 0 is default
    } hdr;

    inline t_seg_hdr_() noexcept { value = 0; }
    inline t_seg_hdr_(t_char ch1, t_char ch2) noexcept {
      raw[0] = ch1; raw[1] = ch2;
    }
    inline t_seg_hdr_(t_n_ len, t_user usr) noexcept {
      hdr.len = len; hdr.usr = usr;
    }
  };
  constexpr t_n_ HDR_MAX_ = sizeof(t_seg_hdr_);

  static_assert(HDR_MAX_ == 2, "expect t_seg_hdr_ to be 2 characters large");

///////////////////////////////////////////////////////////////////////////////

  class t_citr {
  public:
              t_citr   operator++(t_int)        noexcept;
              t_citr&  operator++()             noexcept;
              t_crange operator* ()       const noexcept;
              t_user   get_user  ()       const noexcept;
    constexpr t_bool   is_equal  (t_citr) const noexcept;

  private:
    friend class t_segmented_impl_base_;
    constexpr t_citr(P_char) noexcept;

    P_char buf_;
  };

///////////////////////////////////////////////////////////////////////////////

  constexpr
  t_citr::t_citr(P_char buf) noexcept : buf_{buf} {
  }

  inline
  t_citr t_citr::operator++(t_int) noexcept {
    t_citr tmp{buf_};
    t_seg_hdr_ hdr{buf_[0], buf_[1]};
    buf_ += HDR_MAX_ + hdr.hdr.len;
    return tmp;
  }

  inline
  t_citr& t_citr::operator++() noexcept {
    t_seg_hdr_ hdr{buf_[0], buf_[1]};
    buf_ += HDR_MAX_ + hdr.hdr.len;
    return *this;
  }

  inline
  t_crange t_citr::operator*() const noexcept {
    t_seg_hdr_ hdr{buf_[0], buf_[1]};
    return {buf_ + HDR_MAX_, t_n{hdr.hdr.len}};
  }

  inline
  t_user t_citr::get_user() const noexcept {
    t_seg_hdr_ hdr{buf_[0], buf_[1]};
    return hdr.hdr.usr;
  }

  constexpr
  t_bool t_citr::is_equal(t_citr itr) const noexcept {
    return buf_ == itr.buf_;
  }

///////////////////////////////////////////////////////////////////////////////

  inline
  t_bool operator==(const t_citr& lh, const t_citr& rh) {
    return lh.is_equal(rh);
  }

  inline
  t_bool operator!=(const t_citr& lh, const t_citr& rh) {
    return !(lh == rh);
  }

///////////////////////////////////////////////////////////////////////////////

  struct t_result {
    constexpr operator t_bool () const noexcept { return id != BAD_ID; }
    t_id     id;
    t_seg_no seg_no;
  };
  constexpr t_result BAD_RESULT{BAD_ID, t_seg_no{0}};

///////////////////////////////////////////////////////////////////////////////

  struct t_seg_info_ {
    constexpr operator t_bool () const noexcept { return ptr; }
    p_char     ptr = nullptr;
    t_seg_hdr_ hdr;
  };
  const t_seg_info_ BAD_INFO;

  struct t_seg_cinfo_ {
    constexpr operator t_bool () const noexcept { return ptr; }
    P_char     ptr = nullptr;
    t_seg_hdr_ hdr;
  };
  const t_seg_cinfo_ BAD_CINFO;

///////////////////////////////////////////////////////////////////////////////

  class t_segmented_impl_base_;
  using R_segmented_impl_base_ = base::t_prefix<t_segmented_impl_base_>::R_;

  class t_segmented_impl_base_ {
  public:
    t_segmented_impl_base_(p_char store) noexcept;

    t_n_     get_segs_num ()       const noexcept;
    t_n_     get_size     (P_char) const noexcept;
    t_bool   is_empty     ()       const noexcept;

    t_crange get          (P_char, t_seg_no) const noexcept;
    t_crange get          (P_char, t_id)     const noexcept;

    t_crange front        (P_char) const noexcept;
    t_crange back         (P_char) const noexcept;

    t_id     find         (P_char, t_crange)       const noexcept;
    t_id     find_next    (P_char, t_crange, t_id) const noexcept;

    t_result push_back    (p_char, t_user)           noexcept;
    t_result push_back    (p_char, t_crange, t_user) noexcept;

    t_id     insert       (p_char, p_char, t_seg_no, t_user)           noexcept;
    t_id     insert       (p_char, p_char, t_seg_no, t_crange, t_user) noexcept;

    t_id     insert       (p_char, p_char, t_id, t_user)           noexcept;
    t_id     insert       (p_char, p_char, t_id, t_crange, t_user) noexcept;

    t_bool   change       (p_char, t_seg_no, t_user) noexcept;
    t_bool   change       (p_char, t_id,     t_user) noexcept;

    t_bool   change       (p_char, p_char, t_seg_no, t_crange, t_user) noexcept;
    t_bool   change       (p_char, p_char, t_id,     t_crange, t_user) noexcept;

    t_bool   remove       (p_char, t_seg_no, t_n)  noexcept;
    t_bool   remove       (p_char, t_id,     t_n)  noexcept;

    t_void   clear        (p_char)       noexcept;

    t_citr   begin        (P_char) const noexcept;
    t_citr   end          ()       const noexcept;

    t_bool   is_equal     (P_char, P_char, R_segmented_impl_base_) const noexcept;
    t_bool   is_less      (P_char, P_char, R_segmented_impl_base_) const noexcept;
    t_bool   is_less_equal(P_char, P_char, R_segmented_impl_base_) const noexcept;

    template<typename F>
    t_void each           (P_char, F&&) const noexcept;

    template<typename BY, typename TO>
    t_void generate       (P_char, BY&&, TO&&) noexcept;

  protected:
    t_seg_info_  get_seg (p_char, t_seg_no)       noexcept;
    t_seg_info_  get_seg (p_char, t_id)           noexcept;
    t_seg_cinfo_ get_cseg(P_char, t_seg_no) const noexcept;
    t_seg_cinfo_ get_cseg(P_char, t_id)     const noexcept;

  protected:
    t_n_   segs_ = 0;
    p_char next_ = nullptr;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename> class t_segmented_impl_;

///////////////////////////////////////////////////////////////////////////////

  template<>
  class t_segmented_impl_<t_overflow_assert> : public t_segmented_impl_base_ {
  public:
    using t_segmented_impl_base_::t_segmented_impl_base_;

    t_result push_back(p_char, p_char, t_user)           noexcept;
    t_result push_back(p_char, p_char, t_crange, t_user) noexcept;

    t_id     insert   (p_char, p_char, t_seg_no, t_user)           noexcept;
    t_id     insert   (p_char, p_char, t_seg_no, t_crange, t_user) noexcept;

    t_id     insert   (p_char, p_char, t_id, t_user)           noexcept;
    t_id     insert   (p_char, p_char, t_id, t_crange, t_user) noexcept;

    t_bool   change   (p_char, t_seg_no, t_user) noexcept;
    t_bool   change   (p_char, t_id,     t_user) noexcept;

    t_bool   change   (p_char, p_char, t_seg_no, t_crange, t_user) noexcept;
    t_bool   change   (p_char, p_char, t_id,     t_crange, t_user) noexcept;
  };

///////////////////////////////////////////////////////////////////////////////

  template<>
  class t_segmented_impl_<t_overflow_truncate> : public t_segmented_impl_base_ {
  public:
    using t_segmented_impl_base_::t_segmented_impl_base_;

    t_result push_back(p_char, p_char, t_user)           noexcept;
    t_result push_back(p_char, p_char, t_crange, t_user) noexcept;

    t_id     insert   (p_char, p_char, t_seg_no, t_user)           noexcept;
    t_id     insert   (p_char, p_char, t_seg_no, t_crange, t_user) noexcept;

    t_id     insert   (p_char, p_char, t_id, t_user)           noexcept;
    t_id     insert   (p_char, p_char, t_id, t_crange, t_user) noexcept;

    t_bool   change   (p_char, t_seg_no, t_user) noexcept;
    t_bool   change   (p_char, t_id,     t_user) noexcept;

    t_bool   change   (p_char, p_char, t_seg_no, t_crange, t_user) noexcept;
    t_bool   change   (p_char, p_char, t_id,     t_crange, t_user) noexcept;
  };

///////////////////////////////////////////////////////////////////////////////

  template<>
  class t_segmented_impl_<t_overflow_grow> : public t_segmented_impl_base_ {
  public:
    using t_segmented_impl_base_::t_segmented_impl_base_;

    t_result push_back(p_char, p_char, t_user)           noexcept;
    t_result push_back(p_char, p_char, t_crange, t_user) noexcept;

    t_id     insert   (p_char, p_char, t_seg_no, t_user)           noexcept;
    t_id     insert   (p_char, p_char, t_seg_no, t_crange, t_user) noexcept;

    t_id     insert   (p_char, p_char, t_id, t_user)           noexcept;
    t_id     insert   (p_char, p_char, t_id, t_crange, t_user) noexcept;

    t_bool   change   (p_char, t_seg_no, t_user) noexcept;
    t_bool   change   (p_char, t_id,     t_user) noexcept;

    t_bool   change   (p_char, p_char, t_seg_no, t_crange, t_user) noexcept;
    t_bool   change   (p_char, p_char, t_id,     t_crange, t_user) noexcept;
  };

///////////////////////////////////////////////////////////////////////////////

  p_char push_back_    (p_char,           t_user)                     noexcept;
  p_char push_back_    (p_char, t_crange, t_user)                     noexcept;
  p_char insert_       (p_char, p_char,   t_user)                     noexcept;
  p_char insert_       (p_char, p_char,   t_crange, t_user)           noexcept;
  p_char change_       (p_char, p_char,           t_user)             noexcept;
  p_char change_       (p_char, p_char, t_crange, t_user)             noexcept;
  p_char remove_       (p_char, p_char, t_n)                          noexcept;
  P_char find_         (P_char, P_char, t_crange)                     noexcept;
  P_char find_next_    (P_char, P_char, t_crange)                     noexcept;
  t_bool is_equal_     (t_n_,   P_char, P_char, t_n_, P_char, P_char) noexcept;
  t_bool is_less_      (P_char, P_char, P_char, P_char)               noexcept;
  t_bool is_less_equal_(P_char, P_char, P_char, P_char)               noexcept;

///////////////////////////////////////////////////////////////////////////////

  inline
  t_segmented_impl_base_::t_segmented_impl_base_(p_char store) noexcept
    : next_{store} {
  }

  inline
  t_n_ t_segmented_impl_base_::get_segs_num() const noexcept {
    return segs_;
  }

  inline
  t_n_ t_segmented_impl_base_::get_size(P_char store) const noexcept {
    return next_ - store;
  }

  inline
  t_bool t_segmented_impl_base_::is_empty() const noexcept {
    return !segs_;
  }

  inline
  t_seg_info_ t_segmented_impl_base_::get_seg(p_char store,
                                              t_seg_no _seg_no) noexcept {
    t_n_ seg_no = base::get(_seg_no);
    p_char ptr  = store;
    for (t_n_ cnt = 0; ptr < next_; ++cnt) {
      t_seg_hdr_ hdr{ptr[0], ptr[1]};
      if (cnt == seg_no)
        return t_seg_info_{ptr, hdr};
      ptr += HDR_MAX_ + hdr.hdr.len;
    }
    return BAD_INFO;
  }

  inline
  t_seg_cinfo_ t_segmented_impl_base_::
      get_cseg(P_char store, t_seg_no _seg_no) const noexcept {
    t_n_ seg_no = base::get(_seg_no);
    P_char ptr  = store;
    for (t_n_ cnt = 0; ptr < next_; ++cnt) {
      t_seg_hdr_ hdr{ptr[0], ptr[1]};
      if (cnt == seg_no)
        return t_seg_cinfo_{ptr, hdr};
      ptr += HDR_MAX_ + hdr.hdr.len;
    }
    return BAD_CINFO;
  }

  inline
  t_seg_info_ t_segmented_impl_base_::get_seg(p_char store, t_id id) noexcept {
    p_char ptr = store + base::get(id) - 1;
    t_seg_hdr_ hdr{ptr[0], ptr[1]};
    return t_seg_info_{ptr, hdr};
  }

  inline
  t_seg_cinfo_ t_segmented_impl_base_::get_cseg(P_char store,
                                                t_id id) const noexcept {
    P_char ptr = store + base::get(id) - 1;
    t_seg_hdr_ hdr{ptr[0], ptr[1]};
    return t_seg_cinfo_{ptr, hdr};
  }

  inline
  t_crange t_segmented_impl_base_::get(P_char store,
                                       t_seg_no seg_no) const noexcept {
    auto info = get_cseg(store, seg_no);
    return info ? t_crange{info.ptr + HDR_MAX_, t_n{info.hdr.hdr.len}}
                : BAD_RANGE;
  }

  inline
  t_crange t_segmented_impl_base_::get(P_char store, t_id id) const noexcept {
    auto info = get_cseg(store, id);
    return info ? t_crange{info.ptr + HDR_MAX_, t_n{info.hdr.hdr.len}}
                : BAD_RANGE;
  }

  inline
  t_crange t_segmented_impl_base_::front(P_char store) const noexcept {
    return segs_ ? get(store, t_seg_no{0}) : BAD_RANGE;
  }

  inline
  t_crange t_segmented_impl_base_::back(P_char store) const noexcept {
    return segs_ ? get(store, t_seg_no{segs_ - 1}) : BAD_RANGE;
  }

  inline
  t_id t_segmented_impl_base_::find(P_char store,
                                    t_crange range) const noexcept {
    if (store < next_) {
      P_char entry = find_(store, next_, range);
      if (entry != next_)
        return t_id((entry - store) + 1);
    }
    return BAD_ID;
  }

  inline
  t_id t_segmented_impl_base_::find_next(P_char store, t_crange range,
                                         t_id _id) const noexcept {
    t_id_ id = base::get(_id);
    if (id) {
      P_char ptr = store + id - 1;
      if (ptr < next_) {
        P_char entry = find_next_(ptr, next_, range);
        if (entry != next_)
          return t_id((entry - store) + 1);
      }
    }
    return BAD_ID;
  }

  inline
  t_void t_segmented_impl_base_::clear(p_char store) noexcept {
    segs_ = 0;
    next_ = store;
  }

  inline
  t_citr t_segmented_impl_base_::begin(P_char store) const noexcept {
    return { store };
  }

  inline
  t_citr t_segmented_impl_base_::end() const noexcept {
    return { next_ };
  }

  inline
  t_bool t_segmented_impl_base_
      ::is_equal(P_char store, P_char other_store,
                 R_segmented_impl_base_ other_impl) const noexcept {
    return is_equal_(segs_,            store,       next_,
                     other_impl.segs_, other_store, other_impl.next_);
  }

  inline
  t_bool t_segmented_impl_base_
      ::is_less(P_char store, P_char other_store,
                R_segmented_impl_base_ other_impl) const noexcept {
    return is_less_(store, next_, other_store, other_impl.next_);
  }

  inline
  t_bool t_segmented_impl_base_
      ::is_less_equal(P_char store, P_char other_store,
                      R_segmented_impl_base_ other_impl) const noexcept {
    return is_less_equal_(store, next_, other_store, other_impl.next_);
  }

  template<typename F>
  inline
  t_void t_segmented_impl_base_::each(P_char store, F&& func) const noexcept {
    P_char ptr = store;
    while (ptr < next_) {
      t_seg_hdr_ hdr{ptr[0], ptr[1]};
      func({ptr + HDR_MAX_, hdr.hdr.len}, hdr.hdr.usr);
      ptr += HDR_MAX_ + hdr.hdr.len;
    }
  }

  template<typename BY, typename TO>
  inline
  t_void t_segmented_impl_base_::generate(P_char store, BY&& by,
                                          TO&& to) noexcept {
    P_char ptr = store;
    while (ptr < next_) {
      t_seg_hdr_ hdr{ptr[0], ptr[1]};
      by(preserve<TO>(to), {ptr + HDR_MAX_, t_n{hdr.hdr.len}}, hdr.hdr.usr);
      ptr += HDR_MAX_ + hdr.hdr.len;
    }
  }

  inline
  t_result t_segmented_impl_base_::push_back(p_char store,
                                             t_user user) noexcept {
    t_ix_ ix = next_ - store;
    next_    = push_back_(next_, user);
    return {t_id{ix + 1}, t_seg_no{segs_++}};
  }

  inline
  t_result t_segmented_impl_base_::push_back(p_char store, t_crange range,
                                             t_user user) noexcept {
    t_ix_ ix = next_ - store;
    next_ = push_back_(next_, range, user);
    return {t_id{ix + 1}, t_seg_no{segs_++}};
  }

  inline
  t_id t_segmented_impl_base_::insert(p_char store, p_char end,
                                      t_seg_no seg_no, t_user user) noexcept {
    auto info = get_seg(store, seg_no);
    if (info && (next_ + HDR_MAX_ <= end)) {
      next_ = insert_(info.ptr, next_, user);
      ++segs_;
      return t_id((info.ptr - store) + 1);
    }
    return BAD_ID;
  }

  inline
  t_id t_segmented_impl_base_::insert(p_char store, p_char end,
                                      t_seg_no seg_no, t_crange range,
                                      t_user user) noexcept {
    auto info = get_seg(store, seg_no);
    if (info && (next_ + HDR_MAX_ + base::get(range.n) <= end)) {
      next_ = insert_(info.ptr, next_, range, user);
      ++segs_;
      return t_id((info.ptr - store) + 1);
    }
    return BAD_ID;
  }

  inline
  t_id t_segmented_impl_base_::insert(p_char store, p_char end, t_id id,
                                      t_user user) noexcept {
    auto info = get_seg(store, id);
    if (info && (next_ + HDR_MAX_ <= end)) {
      next_ = insert_(info.ptr, next_, user);
      ++segs_;
      return t_id((info.ptr - store) + 1);
    }
    return BAD_ID;
  }

  inline
  t_id t_segmented_impl_base_::insert(p_char store, p_char end, t_id id,
                                      t_crange range, t_user user) noexcept {
    auto info = get_seg(store, id);
    if (info && (next_ + HDR_MAX_ + base::get(range.n) <= end)) {
      next_ = insert_(info.ptr, next_, range, user);
      ++segs_;
      return t_id((info.ptr - store) + 1);
    }
    return BAD_ID;
  }

  inline
  t_bool t_segmented_impl_base_::change(p_char store, t_seg_no seg_no,
                                        t_user user) noexcept {
    auto info = get_seg(store, seg_no);
    if (info) {
      next_ = change_(info.ptr, next_, user);
      return true;
    }
    return false;
  }

  inline
  t_bool t_segmented_impl_base_::change(p_char store, p_char end,
                                        t_seg_no seg_no,
                                        t_crange range, t_user user) noexcept {
    auto info = get_seg(store, seg_no);
    if (info) {
      t_n_ available = (end - next_) + info.hdr.hdr.len;
      if (available >= base::get(range.n))
        next_ = change_(info.ptr, next_, range, user);
      return true;
    }
    return false;
  }

  inline
  t_bool t_segmented_impl_base_::change(p_char store, t_id id,
                                        t_user user) noexcept {
    auto info = get_seg(store, id);
    if (info) {
      next_ = change_(info.ptr, next_, user);
      return true;
    }
    return false;
  }

  inline
  t_bool t_segmented_impl_base_::change(p_char store, p_char end, t_id id,
                                        t_crange range, t_user user) noexcept {
    auto info = get_seg(store, id);
    if (info) {
      t_n_ available = (end - next_) + info.hdr.hdr.len;
      if (available >= base::get(range.n))
        next_ = change_(info.ptr, next_, range, user);
      return true;
    }
    return false;
  }

  inline
  t_bool t_segmented_impl_base_::remove(p_char store, t_seg_no seg_no,
                                        t_n _n) noexcept {
    t_n_ n = base::get(_n);
    if (base::get(seg_no) + n <= segs_) {
      auto info = get_seg(store, seg_no);
      next_ = remove_(info.ptr, next_, _n);
      segs_ -= n;
      return true;
    }
    return false;
  }

  inline
  t_bool t_segmented_impl_base_::remove(p_char store, t_id id,
                                        t_n n) noexcept {
    auto info = get_seg(store, id);
    if (info) {
      next_ = remove_(info.ptr, next_, n);
      segs_ -= base::get(n);
      return true;
    }
    return false;
  }

///////////////////////////////////////////////////////////////////////////////

  inline
  t_result t_segmented_impl_<t_overflow_assert>::
      push_back(p_char store, p_char end, t_user user) noexcept {
    if (user && (next_ + HDR_MAX_ <= end))
      return t_segmented_impl_base_::push_back(store, user);

    assert_now(FMT, "t_segmented: push_back failed");
    return BAD_RESULT;
  }

  inline
  t_result t_segmented_impl_<t_overflow_assert>::
      push_back(p_char store, p_char end, t_crange range,
                t_user user) noexcept {
    if (next_ + HDR_MAX_ + base::get(range.n) <= end)
      return t_segmented_impl_base_::push_back(store, range, user);

    assert_now(FMT, "t_segmented: push_back failed");
    return BAD_RESULT;
  }

  inline
  t_id t_segmented_impl_<t_overflow_assert>::
      insert(p_char, p_char, t_seg_no, t_user) noexcept {
    return BAD_ID; // XXX - 6
  }

  inline
  t_id t_segmented_impl_<t_overflow_assert>::
      insert(p_char, p_char, t_seg_no, t_crange, t_user) noexcept {
    return BAD_ID; // XXX - 7
  }

  inline
  t_id t_segmented_impl_<t_overflow_assert>::
      insert(p_char, p_char, t_id, t_user) noexcept {
    return BAD_ID; // XXX - 8
  }

  inline
  t_id t_segmented_impl_<t_overflow_assert>::
      insert(p_char, p_char, t_id, t_crange, t_user) noexcept {
    return BAD_ID; // XXX - 9
  }

  inline
  t_bool t_segmented_impl_<t_overflow_assert>::
      change(p_char store, t_seg_no seg_no, t_user user) noexcept {
    if (base::get(seg_no) < segs_) {
      if (t_segmented_impl_base_::change(store, seg_no, user))
        return true;

      assert_now(FMT, "t_segmented: change failed");
    }
    return false;
  }

  inline
  t_bool t_segmented_impl_<t_overflow_assert>::
      change(p_char store, t_id id, t_user user) noexcept {
    if (t_segmented_impl_base_::change(store, id, user))
      return true;

    assert_now(FMT, "t_segmented: change failed");
    return false;
  }

  inline
  t_bool t_segmented_impl_<t_overflow_assert>::
      change(p_char store, p_char end, t_seg_no seg_no, t_crange range,
             t_user user) noexcept {
    if (base::get(seg_no) < segs_) {
      if (t_segmented_impl_base_::change(store, end, seg_no, range, user))
        return true;
      assert_now(FMT, "t_segmented: change failed");
    }
    return false;
  }

  inline
  t_bool t_segmented_impl_<t_overflow_assert>::
      change(p_char store, p_char end, t_id id, t_crange range,
             t_user user) noexcept {
    if (t_segmented_impl_base_::change(store, end, id, range, user))
      return true;

    assert_now(FMT, "t_segmented: change failed");
    return false;
  }

///////////////////////////////////////////////////////////////////////////////

  inline
  t_result t_segmented_impl_<t_overflow_truncate>::
      push_back(p_char store, p_char end, t_user user) noexcept {
    if (user && (next_ + HDR_MAX_ <= end))
      return t_segmented_impl_base_::push_back(store, user);

    return BAD_RESULT;
  }

  inline
  t_result t_segmented_impl_<t_overflow_truncate>::
      push_back(p_char store, p_char end, t_crange range,
                t_user user) noexcept {
    if (next_ + HDR_MAX_ + base::get(range.n) <= end)
      return t_segmented_impl_base_::push_back(store, range, user);

    return BAD_RESULT;
  }

  inline
  t_id t_segmented_impl_<t_overflow_truncate>::
      insert(p_char, p_char, t_seg_no, t_user) noexcept {
    return BAD_ID; // XXX - 14
  }

  inline
  t_id t_segmented_impl_<t_overflow_truncate>::
      insert(p_char, p_char, t_seg_no, t_crange, t_user) noexcept {
    return BAD_ID; // XXX - 15
  }

  inline
  t_id t_segmented_impl_<t_overflow_truncate>::
      insert(p_char, p_char, t_id, t_user) noexcept {
    return BAD_ID; // XXX - 16
  }

  inline
  t_id t_segmented_impl_<t_overflow_truncate>::
      insert(p_char, p_char, t_id, t_crange, t_user) noexcept {
    return BAD_ID; // XXX - 17
  }

  inline
  t_bool t_segmented_impl_<t_overflow_truncate>::
      change(p_char store, t_seg_no seg_no, t_user user) noexcept {
    return t_segmented_impl_base_::change(store, seg_no, user);
  }

  inline
  t_bool t_segmented_impl_<t_overflow_truncate>::
      change(p_char store, t_id id, t_user user) noexcept {
    return t_segmented_impl_base_::change(store, id, user);
  }

  inline
  t_bool t_segmented_impl_<t_overflow_truncate>::
      change(p_char store, p_char end, t_seg_no seg_no, t_crange range,
             t_user user) noexcept {
    return t_segmented_impl_base_::change(store, end, seg_no, range, user);
  }

  inline
  t_bool t_segmented_impl_<t_overflow_truncate>::
      change(p_char store, p_char end, t_id id, t_crange range,
             t_user user) noexcept {
    return t_segmented_impl_base_::change(store, end, id, range, user);
  }

///////////////////////////////////////////////////////////////////////////////

  inline
  t_result t_segmented_impl_<t_overflow_grow>::
      push_back(p_char store, p_char end, t_user user) noexcept {
    if (user && (next_ + HDR_MAX_ <= end))
      return t_segmented_impl_base_::push_back(store, user);

    // must grow string XXX - 20
    return t_segmented_impl_base_::push_back(store, user);
  }

  inline
  t_result t_segmented_impl_<t_overflow_grow>::
      push_back(p_char store, p_char end, t_crange range,
                t_user user) noexcept {
    if (next_ + HDR_MAX_ + base::get(range.n) <= end)
      return t_segmented_impl_base_::push_back(store, range, user);

    // must grow string XXX - 21
    return t_segmented_impl_base_::push_back(store, range, user);
  }

  inline
  t_id t_segmented_impl_<t_overflow_grow>::
      insert(p_char store, p_char end, t_seg_no seg_no,
             t_user user) noexcept {
    return BAD_ID; // XXX - 22
  }

  inline
  t_id t_segmented_impl_<t_overflow_grow>::
      insert(p_char, p_char, t_seg_no, t_crange, t_user) noexcept {
    return BAD_ID; // XXX - 23
  }

  inline
  t_id t_segmented_impl_<t_overflow_grow>::
      insert(p_char, p_char, t_id, t_user) noexcept {
    return BAD_ID; // XXX - 24
  }

  inline
  t_id t_segmented_impl_<t_overflow_grow>::
      insert(p_char, p_char, t_id, t_crange, t_user) noexcept {
    return BAD_ID; // XXX - 25
  }

  inline
  t_bool t_segmented_impl_<t_overflow_grow>::
      change(p_char store, t_seg_no seg_no, t_user user) noexcept {
    if (base::get(seg_no) < segs_)
      return t_segmented_impl_base_::change(store, seg_no, user);
    return false;
  }

  inline
  t_bool t_segmented_impl_<t_overflow_grow>::
      change(p_char store, t_id id, t_user user) noexcept {
    return t_segmented_impl_base_::change(store, id, user);
  }

  inline
  t_bool t_segmented_impl_<t_overflow_grow>::
      change(p_char store, p_char end, t_seg_no seg_no, t_crange range,
             t_user user) noexcept {
    if (base::get(seg_no) < segs_) {
      if (!t_segmented_impl_base_::change(store, end, seg_no, range, user)) {
        // must grow - retry - XXX - 28
        t_segmented_impl_base_::change(store, end, seg_no, range, user);
      }
      return true;
    }
    return false;
  }

  inline
  t_bool t_segmented_impl_<t_overflow_grow>::
      change(p_char store, p_char end, t_id id, t_crange range,
             t_user user) noexcept {
    if (!t_segmented_impl_base_::change(store, end, id, range, user)) {
      // grow - store/end XXX
      t_segmented_impl_base_::change(store, end, id, range, user);
    }
    return true;
  }

///////////////////////////////////////////////////////////////////////////////

  inline
  p_char push_back_(p_char buf, t_user usr) noexcept {
    t_seg_hdr_ hdr{0, usr};
    *buf++ = hdr.raw[0];
    *buf++ = hdr.raw[1];
    return buf;
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}
}

#endif
