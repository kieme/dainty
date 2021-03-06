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

#ifndef _DAINTY_SANDBOX_LOGIC_EXT_API_H_
#define _DAINTY_SANDBOX_LOGIC_EXT_API_H_

#include "dainty_base_string.h"
#include "dainty_sandbox_logic_api.h"

///////////////////////////////////////////////////////////////////////////////

namespace dainty
{
namespace sandbox
{
  using t_err = err::t_err;

  using base::types::t_prefix;
  using base::types::t_void;
  using base::types::t_bool;

  using base::specific::t_validity;
  using base::specific::VALID;
  using base::specific::INVALID;

  using base::string::t_string;

///////////////////////////////////////////////////////////////////////////////

  enum  t_extension_name_tag {};
  using t_extension_name = t_string<t_extension_name_tag>;
  using T_extension_name = t_prefix<t_extension_name>::T_;
  using R_extension_name = t_prefix<t_extension_name>::R_;

  class t_logic;
  using r_logic = t_prefix<t_logic>::r_;
  using R_logic = t_prefix<t_logic>::R_;
  using p_logic = t_prefix<t_logic>::p_;
  using x_logic = t_prefix<t_logic>::x_;

///////////////////////////////////////////////////////////////////////////////

  class t_logic_ext_api {
  public:
    virtual ~t_logic_ext_api() { }

///////////////////////////////////////////////////////////////////////////////

    virtual operator t_validity() const noexcept = 0;

///////////////////////////////////////////////////////////////////////////////

    virtual R_extension_name get_name       () const noexcept = 0;
    virtual R_logic_stats    get_logic_stats() const noexcept = 0;
    virtual R_logic_name     get_logic_name () const noexcept = 0;

///////////////////////////////////////////////////////////////////////////////

    virtual t_timer_id start_timer  (t_err, R_timer_name,
                                     R_timer_params)          noexcept = 0;
    virtual t_timer_id start_timer  (t_err, R_timer_name, R_timer_params,
                                     x_timer_notify_ptr)      noexcept = 0;
    virtual t_void     restart_timer(t_err, t_timer_id)       noexcept = 0;
    virtual t_void     restart_timer(t_err, t_timer_id,
                                     R_timer_params)          noexcept = 0;
    virtual t_bool             stop_timer (t_timer_id)        noexcept = 0;
    virtual t_timer_notify_ptr clear_timer(t_timer_id)        noexcept = 0;
    virtual P_timer_params     get_timer  (t_timer_id)  const noexcept = 0;

///////////////////////////////////////////////////////////////////////////////

    virtual t_fdevent_id add_fdevent(t_err, R_fdevent_name,
                                     R_fdevent_params)            noexcept = 0;
    virtual t_fdevent_id add_fdevent(t_err, R_fdevent_name,
                                     R_fdevent_params,
                                     x_fdevent_notify_ptr)        noexcept = 0;
    virtual t_fdevent_notify_ptr del_fdevent(t_fdevent_id)        noexcept = 0;
    virtual P_fdevent_params     get_fdevent(t_fdevent_id)  const noexcept = 0;
  };

///////////////////////////////////////////////////////////////////////////////
}
}

#endif
