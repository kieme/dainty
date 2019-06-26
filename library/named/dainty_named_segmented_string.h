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

#ifndef _DAINTY_NAMED_SEGMENTED_STRING_H_
#define _DAINTY_NAMED_SEGMENTED_STRING_H_

#include "dainty_named_string.h"

namespace dainty
{
namespace named
{
namespace string
{
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

  template<typename TAG, t_n_ N>
  class t_segmented_string {
  public:
    t_segmented_string()    noexcept;
    t_segmented_string(t_n) noexcept;

    static t_segment mk_segment(t_crange) noexcept;

    operator t_validity () const noexcept;
    operator t_range    () const noexcept;

    t_n get_capacity    () const noexcept;
    t_n get_size        () const noexcept;
    t_n get_segment_size() const noexcept;

    P_cstr_ c_str() const noexcept;

    t_seg_no append(t_crange) noexcept;
    t_bool   remove(t_seg_no) noexcept;
    t_void   clear()          noexcept;

    t_void realloc_when_more(t_n) noexcept; // so that strings don't just grow and grow

    t_crange operator[](t_seg_no) const noexcept;

  private:
    using t_string_ = t_string<TAG, N, t_overflow_grow>;
    t_ix_ find_(t_seg_no_) const noexcept;
    t_ix_ find_(t_ix_)     const noexcept;

    t_n_      segs_ = 0;
    t_n_      last_ = 0;
    t_string_ string_;
  };

  // comparison
  // lh.seqs_ == rh.seqs && lh.size_ == rh.size_ && compare bytes

///////////////////////////////////////////////////////////////////////////////

  t_bool is_valid() {
  }

  t_ix t_segmented_string::append(R_segment segment) noexcept {
    if (is_valid(segment)) { // maybe only place when I need to change a string
      t_ix_ ix = segs_++;
      string << '/' << segment;
      return t_ix{ix};
    }
}
}
}
}

#endif
