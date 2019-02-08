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

#ifndef _DAINTY_SANDBOX_LOGIC_EXT_CLI_H_
#define _DAINTY_SANDBOX_LOGIC_EXT_CLI_H_

#include "dainty_sandbox_logic_ext.h"

namespace dainty
{
namespace sandbox
{
///////////////////////////////////////////////////////////////////////////////

  class t_logic_cli_ext {
  public:
    t_logic_cli_ext(t_err, r_logic) noexcept;

    // add use API

  private:
    // add required callback methods
    virtual t_void cli_start(t_err) noexcept = 0;
    virtual t_void cli_cleanup()    noexcept = 0;

  private:
    using r_callback = t_prefix<t_logic_cli_ext>::r_;

    class t_ext_ : public t_logic_ext_ {
    public:
      t_ext_(t_err, r_logic, r_callback) noexcept; // r_err

      virtual t_void start(t_err) noexcept override final;
      virtual t_void cleanup()    noexcept override final;

    private:
      r_callback callback_;
    };

    t_ext_ ext_;
  };

///////////////////////////////////////////////////////////////////////////////

}
}

#endif
