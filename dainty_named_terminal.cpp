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

#include "dainty_named_terminal.h"

namespace dainty
{
namespace named
{
namespace terminal
{
  t_out::t_out(t_fmt, P_cstr_ fmt, ...) {
    va_list vars;
    va_start(vars, fmt);
    va_assign(fmt, vars);
    va_end(vars);
  }

  t_out::~t_out() {
    if (!is_empty()) {
      if (get_back() != '\n') {
        if (get_length() == get_capacity())
          mod_(t_ix{get(get_length()) - 1}, '\n');
        else
          append("\n");
      }
      display();
    }
  }

  r_out_string operator+=(r_out_string out, t_flush) {
    if (!out.is_empty()) {
      if (out.get_back() != '\n') {
        if (out.get_length() == out.get_capacity())
          out.mod_(t_ix{get(out.get_length()) - 1}, '\n');
        else
          out.append("\n");
      }
      out.display();
      out.clear();
    }
    return out;
  }
}
}
}
