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

  enum  t_seg_no_tag_ { };
  using t_seg_no_ = named::t_ix_;
  using t_seg_no  = named::t_explicit<t_seg_no_, t_seq_no_tag_>;
//  t_segment mk_segment(t_crange) noexcept;

///////////////////////////////////////////////////////////////////////////////

  template<typename TAG, t_n_ N>
  class t_segmented {
  public:
    t_segmented()    noexcept = default;
    t_segmented(t_n) noexcept;

    operator t_validity () const noexcept;
    operator t_range    () const noexcept;

    t_n get_capacity    () const noexcept;
    t_n get_size        () const noexcept;

    P_cstr_ c_str() const noexcept;

    t_seg_no append(t_crange) noexcept;
    t_bool   remove(t_seg_no) noexcept;
    t_void   clear()          noexcept;

    t_void shrink_when_more(t_n) noexcept;

    t_crange operator[](t_seg_no) const noexcept;

  private:
    using t_string_ = t_string<TAG, N, t_overflow_grow>;

    t_ix_ find_seg_begin_(t_crange, t_seg_no_) const noexcept;
    t_ix_ find_seg_end_  (t_crange, t_ix_)     const noexcept;

    t_n_      segs_ = 0;
    t_ix_     last_ = 0;
    t_string_ string_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename TAG, t_n_ N>
  inline
  t_segmented<TAG, N>::t_segmented(t_n) noexcept {
    // XXX
  }

  template<typename TAG, t_n_ N>
  inline
  t_segmented<TAG, N>::operator t_validity() const noexcept {
    return string_;
  }

  template<typename TAG, t_n_ N>
  inline
  t_segmented<TAG, N>::operator t_range() const noexcept {
    return string_.mk_range();
  }

  template<typename TAG, t_n_ N>
  inline
  t_n t_segmented<TAG, N>::get_capacity() const noexcept {
    return string_.get_capacity();
  }

  template<typename TAG, t_n_ N>
  inline
  t_n t_segmented<TAG, N>::get_size() const noexcept {
    return string_.get_size();
  }

  template<typename TAG, t_n_ N>
  inline
  P_cstr_ t_segmented<TAG, N>::c_str() const noexcept {
    return get(string_.get_cstr());
  }

  template<typename TAG, t_n_ N>
  inline
  t_seg_no t_segmented<TAG, N>::append(t_crange range) noexcept {
    return t_seq_no{1};
  }

  template<typename TAG, t_n_ N>
  inline
  t_bool t_segmented<TAG, N>::remove(t_seg_no no) noexcept {
    t_crange range = string_;
    t_ix_ bix = find_seg_begin_(range, get(no));
    if (bix < end) {
      t_ix_ eix  = find_seg_end_(range, bix);
      if (eix != get(range.n))
        string_.shift_left(t_ix{bix}, t_ix{eix});
      return true;
    }
    return false;
  }

  template<typename TAG, t_n_ N>
  inline
  t_void t_segmented<TAG, N>::clear() noexcept {
    last_ = segs_ = 0;
    string_.clear();
  }

  template<typename TAG, t_n_ N>
  inline
  t_void t_segmented<TAG, N>::shrink_when_more(t_n) noexcept {
  }

  template<typename TAG, t_n_ N>
  inline
  t_ix_ t_segmented<TAG, N>::find_seg_begin_(t_crange range,
                                             t_seg_no_ no)  const noexcept {
    t_n_ cnt = 0, num = get(no);
    t_ix_ ix = 0;
    for (; range.ptr[ix]; ++ix)
      if (range.ptr[ix] == '/') {
        if (ix && range.ptr[ix-1] == '\\')
          continue;
        if (++cnt == num)
          return ix;
      }
    return get(range.n);
  }

  template<typename TAG, t_n_ N>
  inline
  t_ix_ t_segmented<TAG, N>::find_seg_end_(t_ix_, t_ix end) const noexcept {
    return 0;
  }

  template<typename TAG, t_n_ N>
  inline
  t_crange t_segmented<TAG, N>::operator[](t_seg_no no) const noexcept {
    t_crange range = string_;
    t_ix_ bix = find_seg_begin_(range, get(no));
    if (bix < end)
      return string_.mk_range(bix, find_seg_end_(bix, end));
    return t_crange{};
  }

///////////////////////////////////////////////////////////////////////////////

  /*
  t_ix t_segmented::append(R_segment segment) noexcept {
    if (is_valid(segment)) { // maybe only place when I need to change a string
      t_ix_ ix = segs_++;
      string << '/' << segment;
      return t_ix{ix};
    }
  }
  */
}
}
}
}

#endif
