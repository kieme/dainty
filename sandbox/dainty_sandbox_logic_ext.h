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

#ifndef _DAINTY_SANDBOX_LOGIC_EXT_H_
#define _DAINTY_SANDBOX_LOGIC_EXT_H_

#include "dainty_named.h"
#include "dainty_named_string.h"
#include "dainty_sandbox_logic_ext_api.h"
#include "dainty_sandbox_logic_ext_notify.h"

///////////////////////////////////////////////////////////////////////////////

namespace dainty
{
namespace sandbox
{
  using named::VALID;
  using named::INVALID;
  using named::t_void;
  using named::t_bool;
  using named::t_validity;
  using named::t_prefix;
  using named::string::t_string;

  class t_logic_ext;
  using r_logic_ext = t_prefix<t_logic_ext>::r_;
  using p_logic_ext = t_prefix<t_logic_ext>::p_;
  using x_logic_ext = t_prefix<t_logic_ext>::x_;

///////////////////////////////////////////////////////////////////////////////

  class t_logic_ext : public t_logic_ext_api, public t_logic_ext_notify {
  public:
    t_logic_ext(R_extension_name name, r_logic logic) noexcept
      : name_{name}, logic_{logic} {
    }

    t_logic_ext() = delete;
    t_logic_ext(r_logic_ext) = delete;
    t_logic_ext(x_logic_ext) = delete;
    r_logic_ext operator=(r_logic_ext) = delete;
    r_logic_ext operator=(x_logic_ext) = delete;

///////////////////////////////////////////////////////////////////////////////

    operator t_validity() const noexcept override;

///////////////////////////////////////////////////////////////////////////////

    R_extension_name get_name       () const noexcept override final;
    R_logic_stats    get_logic_stats() const noexcept override final;
    R_logic_name     get_logic_name () const noexcept override final;

///////////////////////////////////////////////////////////////////////////////

    t_timer_id start_timer  (t_err, R_timer_name,
                             R_timer_params)          noexcept override final;
    t_timer_id start_timer  (t_err, R_timer_name, R_timer_params,
                             x_timer_notify_ptr)      noexcept override final;
    t_void     restart_timer(t_err, t_timer_id)       noexcept override final;
    t_void     restart_timer(t_err, t_timer_id,
                             R_timer_params)          noexcept override final;
    t_bool             stop_timer (t_timer_id)        noexcept override final;
    t_timer_notify_ptr clear_timer(t_timer_id)        noexcept override final;
    P_timer_params     get_timer  (t_timer_id)  const noexcept override final;

///////////////////////////////////////////////////////////////////////////////

    t_fdevent_id add_fdevent(t_err, R_fdevent_name,
                             R_fdevent_params)            noexcept override final;
    t_fdevent_id add_fdevent(t_err, R_fdevent_name,
                             R_fdevent_params,
                             x_fdevent_notify_ptr)        noexcept override final;
    t_fdevent_notify_ptr del_fdevent(t_fdevent_id)        noexcept override final;
    P_fdevent_params     get_fdevent(t_fdevent_id)  const noexcept override final;

///////////////////////////////////////////////////////////////////////////////

  private:
    T_extension_name name_;
    r_logic          logic_;
  };
  using p_logic_ext = t_prefix<t_logic_ext>::p_;

  t_void register_(t_err, r_logic, p_logic_ext);

///////////////////////////////////////////////////////////////////////////////
}
}

#endif
