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

#ifndef _DAINTY_BASE_STRING_SEGMENTED_H_
#define _DAINTY_BASE_STRING_SEGMENTED_H_

#include "dainty_base_string_segmented_1.h"
#include "dainty_base_string_segmented_2.h"
#include "dainty_base_string_segmented_3.h"
#include "dainty_base_string_segmented_4.h"

namespace dainty
{
namespace base
{
namespace string
{
namespace segmented
{
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

  template<typename TAG,  t_n_ N,  typename O,
           typename TAG1, t_n_ N1, typename O1>
  inline
  t_bool operator==(const t_segmented<TAG,  N,  O>&  lh,
                    const t_segmented<TAG1, N1, O1>& rh) noexcept {
    return lh.is_equal(rh);
  }

  template<typename TAG, t_n_ N,  typename O,
           typename TAG1,t_n_ N1, typename O1>
  inline
  t_bool operator!=(const t_segmented<TAG,  N,  O>&  lh,
                    const t_segmented<TAG1, N1, O1>& rh) noexcept {
    return !lh.is_equal(rh);
  }

  template<typename TAG,  t_n_ N,  typename O,
           typename TAG1, t_n_ N1, typename O1>
  inline
  t_bool operator<=(const t_segmented<TAG,  N,  O>&  lh,
                    const t_segmented<TAG1, N1, O1>& rh) noexcept {
    return rh.is_less_equal(lh);
  }

  template<typename TAG,  t_n_ N,  typename O,
           typename TAG1, t_n_ N1, typename O1>
  inline
  t_bool operator>=(const t_segmented<TAG,  N,  O>&  lh,
                    const t_segmented<TAG1, N1, O1>& rh) noexcept {
    return !rh.is_less(lh);
  }

  template<typename TAG,  t_n_ N,  typename O,
           typename TAG1, t_n_ N1, typename O1>
  inline
  t_bool operator<(const t_segmented<TAG,  N,  O>&  lh,
                   const t_segmented<TAG1, N1, O1>& rh) noexcept {
    return rh.is_less(lh);
  }

  template<typename TAG,  t_n_ N,  typename O,
           typename TAG1, t_n_ N1, typename O1>
  inline
  t_bool operator>(const t_segmented<TAG,  N,  O>&  lh,
                   const t_segmented<TAG1, N1, O1>& rh) noexcept {
    return !rh.is_less_equal(lh);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}
}

#endif
