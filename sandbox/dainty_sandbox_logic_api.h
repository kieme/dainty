/******************************************************************************

MIT License

Copyright (c) 2018 frits.germs@gmx.net

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

#ifndef _DAINTY_SANDBOX_LOGIC_API_H_
#define _DAINTY_SANDBOX_LOGIC_API_H_

#include "dainty_named.h"
#include "dainty_named_string.h"
#include "dainty_sandbox_logic_stats.h"
#include "dainty_sandbox_err.h"

///////////////////////////////////////////////////////////////////////////////

namespace dainty
{
namespace sandbox
{
  using t_err = err::t_err;
  using named::string::t_string;

  enum  t_messenger_name_tag {};
  using t_messenger_name = t_string<t_messenger_name_tag>;
  using T_messenger_name = t_prefix<t_messenger_name>::t_;
  using R_messenger_name = t_prefix<t_messenger_name>::R_;

///////////////////////////////////////////////////////////////////////////////

  class t_logic_api {
  public:
    virtual ~t_logic_api() { }

    virtual R_logic_stats    get_logic_stats   () const noexcept = 0;
    virtual R_messenger_name get_messenger_name() const noexcept = 0;
  };

///////////////////////////////////////////////////////////////////////////////

}
}

#endif
