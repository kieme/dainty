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

#ifndef _DAINTY_NAMED_STRING_SEGMENTED_H_
#define _DAINTY_NAMED_STRING_SEGMENTED_H_

#include "dainty_named_string.h"

namespace dainty
{
namespace named
{
namespace string
{
namespace segmented
{
///////////////////////////////////////////////////////////////////////////////

  // faster than vectors of strings, that have an index
  // /...../..../..../...../
  // densely packed
  // very little data structure overhead space wise
  // single heap allocation kept at a time.
  // comparison of whole string is cheap
  // copying of whole string is cheap
  // access the last element is O(1) -  might change that to store space
  // good when segment sizes vary.
  // if there must be escape for /, then they will be added.
  //  can be used to check if your range will be modified
  // works well when the segments are not too long
  // assume individual segments will be rarely compared individually.

  // might mean a number of copies due to a number of allocations
  // accessing a specific seg_no is O(N) and not O(1)
  // removing a string does not dealloc. just move bytes.

///////////////////////////////////////////////////////////////////////////////

  using string::t_string;
  using string::operator""_SL;
  using string::t_crange; // ranges should have types!

  using t_user = t_uchar; // only support values 1-63

  enum  t_seg_no_tag_ { };
  using t_seg_no_ = named::t_n_;
  using t_seg_no  = named::t_explicit<t_seg_no_, t_seq_no_tag_>;

  enum  t_id_tag_ { };
  using t_id_ = named::t_ix_;
  using t_id  = named::t_explicit<t_ix_, t_id_tag_>;

  constexpr t_id BAD_ID{0};

///////////////////////////////////////////////////////////////////////////////

  struct t_generator {
    t_n_   cnt = 0;
    t_char delimiter;

    t_generator(t_char _delimiter = ' ') noexcept : delimiter{_delimiter} { }

    template<typename TAG, t_n_ N>
    t_void operator()(t_string<TAG, N>& str, t_crange range, t_user) noexcept {
      if (cnt++)
        str << delimiter << range;
      else
        str << range;
    }

    t_void reset() noexcept {
      cnt = 0;
    }
  };

///////////////////////////////////////////////////////////////////////////////

  class t_citr {
  public:
    t_citr   operator++(t_int)  noexcept;
    t_citr&  operator++()       noexcept;
    t_crange operator* () const noexcept;
    t_user   get_user  () const noexcept;

  private:
    template<t_n_> friend class t_segmented;
    constexpr t_citr(P_char) noexcept;

    P_char buf_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<t_n_ MAX>
  class t_segmented {
  public:
    struct t_result {
      operator t_bool () const noexcept { return id != BAD_ID; }
      t_id     id;
      t_seg_no seg_no;
    };

    t_segmented() noexcept : next_{buf_} { }

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

    t_result insert(t_user)                         noexcept;
    t_result insert(t_crange, t_user = 0)           noexcept;
    t_bool   change(t_seg_no,           t_user)     noexcept;
    t_bool   change(t_seg_no, t_crange, t_user = 0) noexcept;
    t_bool   remove(t_seg_no)                       noexcept;
    t_bool   change(t_id,           t_user)         noexcept;
    t_bool   change(t_id, t_crange, t_user = 0)     noexcept;
    t_bool   remove(t_id)                           noexcept;
    t_void   clear()                                noexcept;

    t_citr begin() const noexcept;
    t_citr end  () const noexcept;

    template<typename F>
    t_void each(F&&) const noexcept;

    template<typename BY, typename TO>
    t_void generate(BY&&, TO&&) noexcept;

    template<t_n_ MAX1>
    t_bool is_equal     (const t_segmented<MAX1>&) const noexcept;

    template<t_n_ MAX1>
    t_bool is_less      (const t_segmented<MAX1>&) const noexcept;

    template<t_n_ MAX1>
    t_bool is_less_equal(const t_segmented<MAX1>&) const noexcept;

  private:
    template<t_n_> friend class t_segmented;

    t_n_   segs_ = 0;
    p_char next_ = nullptr;
    t_char buf_[MAX];
  };

///////////////////////////////////////////////////////////////////////////////

  inline
  t_bool operator==(const t_citr& lh, const t_citr& rh) {
    return true; // XXX-3
  }

  inline
  t_bool operator!=(const t_citr& lh, const t_citr& rh) {
    return !(lh == rh);
  }

///////////////////////////////////////////////////////////////////////////////

  template<t_n_ MAX1, t_n_ MAX2>
  inline
  t_bool operator==(const t_segmented<MAX1>& lh,
                    const t_segmented<MAX2>& rh) noexcept {
    return lh.is_equal(rh);
  }

  template<t_n_ MAX1, t_n_ MAX2>
  inline
  t_bool operator!=(const t_segmented<MAX1>& lh,
                    const t_segmented<MAX2>& rh) noexcept {
    return !lh.is_equal(rh);
  }

  template<t_n_ MAX1, t_n_ MAX2>
  inline
  t_bool operator<=(const t_segmented<MAX1>& lh,
                    const t_segmented<MAX2>& rh) noexcept {
    return rh.is_less_equal(lh);
  }

  template<t_n_ MAX1, t_n_ MAX2>
  inline
  t_bool operator>=(const t_segmented<MAX1>& lh,
                    const t_segmented<MAX2>& rh) noexcept {
    return !rh.is_less(lh);
  }

  template<t_n_ MAX1, t_n_ MAX2>
  inline
  t_bool operator<(const t_segmented<MAX1>& lh,
                   const t_segmented<MAX2>& rh) noexcept {
    return rh.is_less(lh);
  }

  template<t_n_ MAX1, t_n_ MAX2>
  inline
  t_bool operator>(const t_segmented<MAX1>& lh,
                   const t_segmented<MAX2>& rh) noexcept {
    return !rh.is_less_equal(lh);
  }

///////////////////////////////////////////////////////////////////////////////

  p_char insert_       (p_char,           t_user)                     noexcept;
  p_char insert_       (p_char, t_crange, t_user)                     noexcept;
  p_char change_       (p_char, p_char, t_seg_no,           t_user)   noexcept;
  p_char change_       (p_char, p_char, t_seg_no, t_crange, t_user)   noexcept;
  p_char remove_       (p_char, p_char, t_seg_no)                     noexcept;
  p_char change_       (p_char, p_char,           t_user)             noexcept;
  p_char change_       (p_char, p_char, t_crange, t_user)             noexcept;
  p_char remove_       (p_char, p_char)                               noexcept;
  P_char find_         (P_char, P_char, t_crange)                     noexcept;
  P_char find_next_    (P_char, P_char, t_crange)                     noexcept;
  t_bool is_equal_     (t_n_,   P_char, P_char, t_n_, P_char, P_char) noexcept;
  t_bool is_less_      (P_char, P_char, P_char, P_char)               noexcept;
  t_bool is_less_equal_(P_char, P_char, P_char, P_char)               noexcept;

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

  template<t_n_ MAX>
  inline
  t_n_ t_segmented<MAX>::get_segs_num() const noexcept {
    return segs_;
  }

  template<t_n_ MAX>
  inline
  t_n_ t_segmented<MAX>::get_capacity() const noexcept {
    return MAX;
  }

  template<t_n_ MAX>
  inline
  t_n_ t_segmented<MAX>::get_size() const noexcept {
    return next_ - buf_;
  }

  template<t_n_ MAX>
  inline
  t_bool t_segmented<MAX>::is_empty() const noexcept {
    return next_ - buf_;
  }

  template<t_n_ MAX>
  inline
  typename t_segmented<MAX>::t_result
      t_segmented<MAX>::insert(t_user usr) noexcept {
    if (usr && (next_ + HDR_MAX_ <= buf_ + MAX)) {
      t_ix_ ix = next_ - buf_;
      next_ = insert_(next_, usr);
      return {t_id{ix + 1}, t_seg_no{segs_++}};
    }
    return {BAD_ID, t_seg_no{0}};
  }

  template<t_n_ MAX>
  inline
  typename t_segmented<MAX>::t_result
      t_segmented<MAX>::insert(t_crange range, t_user usr) noexcept {
    if (next_ + HDR_MAX_ + named::get(range.n) <= buf_ + MAX) {
      t_ix_ ix = next_ - buf_;
      next_ = insert_(next_, range, usr);
      return {t_id{ix + 1}, t_seg_no{segs_++}};
    }
    return {BAD_ID, t_seg_no{0}};
  }

  template<t_n_ MAX>
  inline
  t_bool t_segmented<MAX>::change(t_id id, t_user usr) noexcept {
    p_char ptr = buf_ + named::get(id) - 1;
    if (ptr < next_) {
      next_ = change_(ptr, next_, usr);
      return true;
    }
    return false;
  }

  template<t_n_ MAX>
  inline
  t_bool t_segmented<MAX>::change(t_seg_no seg_no, t_user usr) noexcept {
    if (named::get(seg_no) < segs_) {
      p_char next = change_(buf_, next_, seg_no, usr);
      if (next) {
        next_ = next;
        return true;
      }
    }
    return false;
  }

  template<t_n_ MAX>
  inline
  t_bool t_segmented<MAX>::change(t_id id, t_crange range,
                                  t_user usr) noexcept {
    p_char ptr = buf_ + named::get(id) - 1;
    if (ptr < next_) {
      next_ = change_(ptr, next_, range, usr);
      return true;
    }
    return false;
  }

  template<t_n_ MAX>
  inline
  t_bool t_segmented<MAX>::change(t_seg_no seg_no, t_crange range,
                                  t_user usr) noexcept {
    if (named::get(seg_no) < segs_) {
      p_char next = change_(buf_, next_, seg_no, range, usr);
      if (next) {
        next_ = next;
        return true;
      }
    }
    return false;
  }

  template<t_n_ MAX>
  inline
  t_bool t_segmented<MAX>::remove(t_id id) noexcept {
    p_char ptr = buf_ + named::get(id) - 1;
    if (ptr < next_) {
      next_ = remove_(buf_, next_);
      --segs_;
      return true;
    }
    return false;
  }

  template<t_n_ MAX>
  inline
  t_bool t_segmented<MAX>::remove(t_seg_no seg_no) noexcept {
    if (named::get(seg_no) < segs_) {
      next_ = remove_(buf_, next_, seg_no);
      --segs_;
      return true;
    }
    return false;
  }

  template<t_n_ MAX>
  inline
  t_void t_segmented<MAX>::clear() noexcept {
    segs_ = 0;
    next_ = buf_;
  }

  template<t_n_ MAX>
  inline
  t_crange t_segmented<MAX>::get(t_id id) const noexcept {
    P_char ptr = buf_ + named::get(id) - 1;
    t_seg_hdr_ hdr{ptr[0], ptr[1]};
    return {ptr + HDR_MAX_, hdr.hdr.len};
  }

  template<t_n_ MAX>
  inline
  t_crange t_segmented<MAX>::get(t_seg_no seg_no) const noexcept {
    P_char ptr = buf_;
    for (t_n_ cnt = 0; ptr < next_; ++cnt) {
      t_seg_hdr_ hdr{ptr[0], ptr[1]};
      if (cnt == named::get(seg_no))
        return {ptr + HDR_MAX_, hdr.hdr.len};
      ptr += HDR_MAX_ + hdr.hdr.len;
    }
    return {nullptr, t_n{0}};
  }

  template<t_n_ MAX>
  inline
  t_crange t_segmented<MAX>::front() const noexcept {
    if (segs_)
      return get(t_seg_no{0});
    return {nullptr, t_n{0}};
  }

  template<t_n_ MAX>
  inline
  t_crange t_segmented<MAX>::back() const noexcept {
    if (segs_)
      return get(t_seg_no{segs_ - 1});
    return {nullptr, t_n{0}};
  }

  template<t_n_ MAX>
  inline
  t_id t_segmented<MAX>::find(t_crange range) const noexcept {
    P_char begin = buf_;
    if (begin < next_) {
      P_char entry = find_(begin, next_, range);
      if (entry != next_)
        return t_id((entry - buf_) + 1);
    }
    return BAD_ID;
  }

  template<t_n_ MAX>
  inline
  t_id t_segmented<MAX>::find_next(t_crange range, t_id _id) const noexcept {
    t_id_ id = named::get(_id);
    if (id) {
      P_char begin = buf_ + id - 1;
      if (begin < next_) {
        P_char entry = find_next_(begin, next_, range);
        if (entry != next_)
          return t_id((entry - buf_) + 1);
      }
    }
    return BAD_ID;
  }

  template<t_n_ MAX>
  inline
  t_crange t_segmented<MAX>::operator[](t_id id) const noexcept {
    return get(id);
  }

  template<t_n_ MAX>
  inline
  t_crange t_segmented<MAX>::operator[](t_seg_no seg_no) const noexcept {
    return get(seg_no);
  }

  template<t_n_ MAX>
  inline
  t_citr t_segmented<MAX>::begin() const noexcept {
    return {buf_};
  }

  template<t_n_ MAX>
  inline
  t_citr t_segmented<MAX>::end() const noexcept {
    return {next_};
  }

  template<t_n_ MAX>
  template<typename F>
  inline
  t_void t_segmented<MAX>::each(F&& func) const noexcept {
    P_char ptr = buf_;
    while (ptr < next_) {
      t_seg_hdr_ hdr{ptr[0], ptr[1]};
      func({ptr + HDR_MAX_, hdr.hdr.len}, hdr.hdr.usr);
      ptr += HDR_MAX_ + hdr.hdr.len;
    }
  }

  template<t_n_ MAX>
  template<typename BY, typename TO>
  __attribute__((noinline))
  t_void t_segmented<MAX>::generate(BY&& by, TO&& to) noexcept {
    P_char ptr = buf_;
    while (ptr < next_) {
      t_seg_hdr_ hdr{ptr[0], ptr[1]};
      by(std::forward<TO>(to), {ptr + HDR_MAX_, t_n{hdr.hdr.len}}, hdr.hdr.usr);
      ptr += HDR_MAX_ + hdr.hdr.len;
    }
  }

  template<t_n_ MAX>
  template<t_n_ MAX1>
  inline
  t_bool t_segmented<MAX>
      ::is_equal(const t_segmented<MAX1>& segs) const noexcept {
    return is_equal_(segs_, buf_, next_, segs.segs_, segs.buf_, segs.next_);
  }

  template<t_n_ MAX>
  template<t_n_ MAX1>
  inline
  t_bool t_segmented<MAX>
      ::is_less(const t_segmented<MAX1>& segs) const noexcept {
    return is_less_(buf_, next_, segs.buf_, segs.next_);
  }

  template<t_n_ MAX>
  template<t_n_ MAX1>
  inline
  t_bool t_segmented<MAX>
      ::is_less_equal(const t_segmented<MAX1>& segs) const noexcept {
    return is_less_equal_(buf_, next_, segs.buf_, segs.next_);
  }

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

///////////////////////////////////////////////////////////////////////////////
}
}
}
}

#endif
