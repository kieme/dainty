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
#include "dainty_container_ptr.h"
#include "dainty_mt_event_dispatcher.h"
#include "dainty_mt_timers.h"
#include "dainty_sandbox_err.h"
#include "dainty_sandbox_logic_stats.h"

///////////////////////////////////////////////////////////////////////////////

namespace dainty
{
namespace sandbox
{
  using t_err = err::t_err;
  using named::t_fd;
  using named::t_msec;
  using named::t_void;
  using named::t_bool;
  using named::t_prefix;
  using named::t_explicit;
  using named::t_validity;
  using named::string::t_string;
  using container::ptr::t_passable_ptr;

///////////////////////////////////////////////////////////////////////////////

  enum  t_logic_name_tag {};
  using t_logic_name = t_string<t_logic_name_tag>;
  using T_logic_name = t_prefix<t_logic_name>::t_;
  using R_logic_name = t_prefix<t_logic_name>::R_;

///////////////////////////////////////////////////////////////////////////////

  enum  t_spin_cnt_tag_ {};
  using t_spin_cnt_ = named::t_int;
  using t_spin_cnt  = named::t_explicit<t_spin_cnt_, t_spin_cnt_tag_>;

///////////////////////////////////////////////////////////////////////////////

  enum  t_timer_name_tag_ {};
  using t_timer_name   = t_string<t_timer_name_tag_, 20>;
  using R_timer_name   = t_prefix<t_timer_name>::R_;
  using t_timer_id     = mt::timers::t_timer_id;
  using t_timer_prio   = mt::timers::t_timer_prio;
  using t_timer_user   = mt::timers::t_timer_user;
  using t_timer_params = mt::timers::t_timer_params;
  using R_timer_params = t_prefix<t_timer_params>::R_;
  using P_timer_params = t_prefix<t_timer_params>::P_;

  class t_timer_notify {
  public:
    virtual ~t_timer_notify() {}
    virtual t_void notify_timeout(t_timer_id, R_timer_params) noexcept = 0;
  };
  using p_timer_notify     = t_prefix<t_timer_notify>::p_;
  using t_timer_notify_ptr = t_passable_ptr<t_timer_notify>;
  using x_timer_notify_ptr = t_prefix<t_timer_notify_ptr>::x_;

  constexpr t_timer_id BAD_TIMER_ID{mt::timers::BAD_TIMER_ID};

///////////////////////////////////////////////////////////////////////////////

  enum  t_fdevent_name_tag_ {};
  using t_fdevent_name   = t_string<t_fdevent_name_tag_, 20>;
  using R_fdevent_name   = t_prefix<t_fdevent_name>::R_;
  using t_fdevent_type   = mt::event_dispatcher::t_event_type;
  using t_fdevent_id     = mt::event_dispatcher::t_event_id;
  using t_fdevent_user   = mt::event_dispatcher::t_event_user;
  using t_fdevent_prio   = mt::event_dispatcher::t_event_prio;
  using t_fdevent_params = mt::event_dispatcher::t_event_params;
  using R_fdevent_params = t_prefix<t_fdevent_params>::R_;
  using P_fdevent_params = t_prefix<t_fdevent_params>::P_;

  class t_fdevent_notify {
  public:
    virtual ~t_fdevent_notify() {}
    virtual t_void notify_fdevent(t_fdevent_id, R_fdevent_params) noexcept = 0;
  };
  using p_fdevent_notify     = t_prefix<t_fdevent_notify>::p_;
  using t_fdevent_notify_ptr = t_passable_ptr<t_fdevent_notify>;
  using x_fdevent_notify_ptr = t_prefix<t_fdevent_notify_ptr>::x_;

  constexpr t_fdevent_type FDEVENT_READ  {mt::event_dispatcher::RD_EVENT};
  constexpr t_fdevent_type FDEVENT_WRITE {mt::event_dispatcher::WR_EVENT};
  constexpr t_fdevent_id   BAD_FDEVENT_ID{mt::event_dispatcher::BAD_EVENT_ID};

///////////////////////////////////////////////////////////////////////////////

  class t_logic_api {
  public:
    // using XXX
    virtual ~t_logic_api() { }

///////////////////////////////////////////////////////////////////////////////

    virtual operator t_validity() const noexcept = 0;

///////////////////////////////////////////////////////////////////////////////

    virtual t_void quit() noexcept = 0;

///////////////////////////////////////////////////////////////////////////////

    virtual R_logic_stats get_logic_stats() const noexcept = 0;
    virtual R_logic_name  get_logic_name () const noexcept = 0;

///////////////////////////////////////////////////////////////////////////////

    virtual t_void     enable_spin    (t_err, t_spin_cnt) noexcept = 0;
    virtual t_void     disable_spin   ()                  noexcept = 0;
    virtual t_spin_cnt get_spin_cnt   ()            const noexcept = 0;
    virtual t_msec     get_spin_period()            const noexcept = 0;

///////////////////////////////////////////////////////////////////////////////

    virtual t_timer_id start_timer(t_err, R_timer_name,
                                   R_timer_params)     noexcept = 0;
    virtual t_timer_id start_timer(t_err, R_timer_name,
                                   R_timer_params,
                                   x_timer_notify_ptr) noexcept = 0;

    virtual t_void restart_timer(t_err, t_timer_id)                 noexcept = 0;
    virtual t_void restart_timer(t_err, t_timer_id, R_timer_params) noexcept = 0;

    virtual t_bool             stop_timer (t_timer_id)       noexcept = 0;
    virtual t_timer_notify_ptr clear_timer(t_timer_id)       noexcept = 0;
    virtual P_timer_params     get_timer  (t_timer_id) const noexcept = 0;

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
