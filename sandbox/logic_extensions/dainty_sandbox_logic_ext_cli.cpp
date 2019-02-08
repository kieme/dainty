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

#include "dainty_sandbox_logic_ext_cli.h"

namespace dainty
{
namespace sandbox
{
///////////////////////////////////////////////////////////////////////////////

  t_logic_cli_ext::t_logic_cli_ext(t_err err, r_logic logic) noexcept
    : ext_{err, logic, *this} {
  }

///////////////////////////////////////////////////////////////////////////////

  t_logic_cli_ext::t_ext_::t_ext_(t_err err, r_logic logic,
                                  r_callback callback) noexcept
      : t_logic_ext_{"cli", logic}, callback_{callback} {
    ERR_GUARD(err) {
      register_(err, get_logic(), this);
    }
  }

  t_void t_logic_cli_ext::t_ext_::start(t_err err) noexcept {
    ERR_GUARD(err) {
      callback_.cli_start(err);
    }
  }

  t_void t_logic_cli_ext::t_ext_::cleanup() noexcept {
      callback_.cli_cleanup();
  }

///////////////////////////////////////////////////////////////////////////////

}
}
