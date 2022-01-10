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

#include <stdlib.h>
#include <string.h>
#include "dainty_container_bytebuf_impl.h"

namespace dainty
{
namespace container
{
namespace bytebuf
{
//////////////////////////////////////////////////////////////////////////////

  p_byte_ alloc_(t_n_ n) {
    p_byte_ byte = nullptr;
    if (n) {
      byte = (p_byte_)::malloc(n);
      if (!byte)
        assert_now(P_cstr("bytebuf: allocation failed"));
    } else
      assert_now(P_cstr("bytebuf: zero byte allocation is invalid"));
    return byte;
  }

  t_void dealloc_(p_byte_ byte) {
    if (byte)
      ::free(byte);
    else
      assert_now(P_cstr("bytebuf: trying to free an nullptr"));
  }

  t_void copy_(p_byte_ dst, t_n_ dst_n, P_byte_ src, t_n_ src_n) {
    if (src_n <= dst_n)
      ::memcpy(dst, src, src_n);
    else
      assert_now(P_cstr("bytebuf: copy would be truncated"));
  }

  t_bool is_equal_(P_byte_ buf1, P_byte_ buf2, t_n_ n) {
    return ::memcmp(buf1, buf2, n) == 0;
  }

  t_view mk_view_(p_byte_ byte, t_n_ max) {
    return {byte, t_n{max}};
  }

  t_view mk_view_(p_byte_ byte, t_n_ max, t_ix_ begin) {
    if (max <= begin)
      assert_now(P_cstr("bytebuf: begin is bigger than max"));
    return {byte + begin, t_n{max - begin}};
  }

  t_view mk_view_(p_byte_ byte, t_n_ max, t_ix_ begin, t_ix_ end) {
    if (max <= begin || end <= begin)
      assert_now(P_cstr("bytebuf: begin/end/max ranges are incorrect "));
    return {byte + begin, t_n{end - begin}};
  }

  t_cview mk_cview_(P_byte_ byte, t_n_ max) {
    return {byte, t_n{max}};
  }

  t_cview mk_cview_(P_byte_ byte, t_n_ max, t_ix_ begin) {
    if (max <= begin)
      assert_now(P_cstr("bytebuf: begin is bigger than max"));
    return {byte + begin, t_n{max - begin}};
  }

  t_cview mk_cview_(P_byte_ byte, t_n_ max, t_ix_ begin, t_ix_ end) {
    if (max <= begin || end <= begin)
      assert_now(P_cstr("bytebuf: begin/end/max ranges are incorrect "));
    return {byte + begin, t_n{end - begin}};
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}
