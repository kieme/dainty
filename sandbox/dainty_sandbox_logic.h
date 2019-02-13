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

#ifndef _DAINTY_SANDBOX_LOGIC_H_
#define _DAINTY_SANDBOX_LOGIC_H_

#include "dainty_container_ptrlist.h"
#include "dainty_sandbox_err.h"
#include "dainty_sandbox_logic_api.h"
#include "dainty_sandbox_logic_ext.h"
#include "dainty_sandbox_logic_notify.h"

///////////////////////////////////////////////////////////////////////////////

namespace dainty
{
namespace sandbox
{
  using named::t_ix;
  using container::ptrlist::t_ptrlist;
  using t_logic_exts = t_ptrlist<t_logic_ext, 10>;

///////////////////////////////////////////////////////////////////////////////

  class t_impl_;
  using p_impl_ = t_prefix<t_impl_>::p_;

///////////////////////////////////////////////////////////////////////////////

  class t_logic : public t_logic_api, public t_logic_notify {
  public:
    t_logic(t_err, R_messenger_name) noexcept;
    virtual ~t_logic() { }

    t_logic() = delete;
    t_logic(r_logic) = delete;
    t_logic(x_logic) = delete;
    r_logic operator=(r_logic) = delete;
    r_logic operator=(x_logic) = delete;

    operator t_validity() const;

    R_logic_stats    get_logic_stats   () const noexcept override final;
    R_messenger_name get_messenger_name() const noexcept override final;

    t_void     enable_spin (t_err, t_spin_cnt) noexcept override final;
    t_void     disable_spin()                  noexcept override final;
    t_spin_cnt get_spin_cnt()            const noexcept override final;
    t_msec     get_spin_period()         const noexcept override final;

    t_timer_id   start_timer  (t_err, R_timer_name,
                                      R_timer_params) noexcept override final;
    t_void       restart_timer(t_err, t_timer_id,
                                      R_timer_params) noexcept override final;
    t_bool       stop_timer(t_timer_id)        noexcept override final;
    P_timer_info get_timer (t_timer_id)  const noexcept override final;

  private:
    friend t_void register_(t_err, r_logic, p_logic_ext);
    friend class t_thread;
    friend class t_main;
    friend class t_impl_;

    p_impl_          impl_ = nullptr;
    t_ix             ix_   = t_ix{0};
    T_messenger_name name_;
    t_logic_exts     exts_;
    t_logic_stats    stats_;
  };

///////////////////////////////////////////////////////////////////////////////

}
}

#endif
