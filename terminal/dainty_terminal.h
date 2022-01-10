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

#ifndef _DAINTY_TERMINAL_H_
#define _DAINTY_TERMINAL_H_

#include "dainty_string.h"

namespace dainty
{
namespace terminal
{
  using namespace base::string;

  enum t_flush { FLUSH };
  enum t_clear { CLEAR };

////////////////////////////////////////////////////////////////////////////////

  enum  t_out_string_tag_ { };
  using t_out_string = t_string<t_out_string_tag_, 120, t_overflow_grow>;
  using r_out_string = t_prefix<t_out_string>::r_;
  using R_out_string = t_prefix<t_out_string>::R_;

  class t_out : public t_out_string {
  public:
    using t_out_string::t_out_string;
    using t_out_string::operator=;

    t_out(P_cstr_ fmt, ...) noexcept
      __attribute__((format(printf, 2, 3)));

    ~t_out();
  };

///////////////////////////////////////////////////////////////////////////////

  r_out_string operator<<(r_out_string, t_flush) noexcept;

  inline
  r_out_string operator<<(r_out_string out, t_clear) noexcept {
    out.clear();
    return out;
  }

///////////////////////////////////////////////////////////////////////////////

}
}

#endif
