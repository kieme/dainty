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

#include "dainty_named.h"
#include "dainty_named_range.h"
#include "dainty_container_err.h"

namespace dainty
{
namespace container
{
namespace bytebuf
{
//////////////////////////////////////////////////////////////////////////////

  using named::t_validity;
  using named::t_void;
  using named::t_bool;
  using named::t_ix_;
  using named::t_ix;
  using named::t_n_;
  using named::t_n;

  using named::INVALID;
  using named::VALID;

  using t_byte = named::t_uchar;
  using p_byte = named::t_prefix<t_byte>::p_;
  using P_byte = named::t_prefix<t_byte>::P_;
  using r_byte = named::t_prefix<t_byte>::r_;
  using R_byte = named::t_prefix<t_byte>::R_;

  enum t_view_tag_ { };
  using t_view  = named::range::t_range <t_byte, t_view_tag_>;
  using t_cview = named::range::t_crange<t_byte, t_view_tag_>;

///////////////////////////////////////////////////////////////////////////////

  p_byte  alloc_   (t_n_);
  t_void  dealloc_ (p_byte);
  t_void  copy_    (p_byte, t_n_, P_byte, t_n_);
  t_bool  is_equal_(P_byte, P_byte, t_n_);

  t_view  mk_view_ (p_byte, t_n_);
  t_view  mk_view_ (p_byte, t_n_, t_ix_);
  t_view  mk_view_ (p_byte, t_n_, t_ix_, t_ix_);

  t_cview mk_cview_(P_byte, t_n_);
  t_cview mk_cview_(P_byte, t_n_, t_ix_);
  t_cview mk_cview_(P_byte, t_n_, t_ix_, t_ix_);

///////////////////////////////////////////////////////////////////////////////

  inline
  t_bool operator==(const t_cview& lh, const t_cview& rh) {
    return lh.n == rh.n && is_equal_(lh.item, rh.item, get(rh.n));
  }

  inline
  t_bool operator!=(const t_cview& lh, const t_cview& rh) {
    return lh.n != rh.n || !is_equal_(lh.item, rh.item, get(rh.n));
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
