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

#ifndef _DAINTY_CONTAINER_BYTEBUF_IMPL_H_
#define _DAINTY_CONTAINER_BYTEBUF_IMPL_H_

#include "dainty_base.h"
#include "dainty_base_range.h"
#include "dainty_base_assert.h"
#include "dainty_container_err.h"

namespace dainty
{
namespace container
{
namespace bytebuf
{
//////////////////////////////////////////////////////////////////////////////

  using base::types::t_prefix;
  using base::types::t_void;
  using base::types::t_bool;
  using base::types::t_uchar;
  using base::types::t_ix_;
  using base::types::t_n_;

  using base::types::t_byte_;
  using base::types::p_byte_;
  using base::types::P_byte_;
  using base::types::r_byte_;

  using base::specific::P_cstr;
  using base::specific::t_ix;
  using base::specific::t_n;
  using base::specific::t_validity;
  using base::specific::INVALID;
  using base::specific::VALID;

  using base::assertion::assert_now;

///////////////////////////////////////////////////////////////////////////////

  using t_view  = base::range::t_byte_range;
  using t_cview = base::range::t_byte_crange;

///////////////////////////////////////////////////////////////////////////////

  p_byte_ alloc_   (t_n_);
  t_void  dealloc_ (p_byte_);
  t_void  copy_    (p_byte_, t_n_, P_byte_, t_n_);
  t_bool  is_equal_(P_byte_, P_byte_, t_n_);

  t_view  mk_view_ (p_byte_, t_n_);
  t_view  mk_view_ (p_byte_, t_n_, t_ix_);
  t_view  mk_view_ (p_byte_, t_n_, t_ix_, t_ix_);

  t_cview mk_cview_(P_byte_, t_n_);
  t_cview mk_cview_(P_byte_, t_n_, t_ix_);
  t_cview mk_cview_(P_byte_, t_n_, t_ix_, t_ix_);

///////////////////////////////////////////////////////////////////////////////

  inline
  t_bool operator==(const t_cview& lh, const t_cview& rh) {
    return lh.n == rh.n && is_equal_(begin(lh), begin(rh), get(rh.n));
  }

  inline
  t_bool operator!=(const t_cview& lh, const t_cview& rh) {
    return lh.n != rh.n || !is_equal_(begin(lh), begin(rh), get(rh.n));
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
