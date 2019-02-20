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

#ifndef _DAINTY_MT_TIMERS_H_
#define _DAINTY_MT_TIMERS_H_

#include "dainty_named.h"
#include "dainty_named_ptr.h"
#include "dainty_named_utility.h"
#include "dainty_container_list.h"
#include "dainty_mt_err.h"

namespace dainty
{
namespace mt
{
namespace timers
{
  using named::t_fd;
  using named::t_void;
  using named::t_bool;
  using named::t_n;
  using named::t_msec;
  using named::t_errn;
  using named::t_validity;
  using named::t_prefix;
  using named::t_explicit;
  using named::VALID;
  using named::INVALID;
  using named::BAD_FD;
  using err::t_err;

///////////////////////////////////////////////////////////////////////////////

  enum  t_timer_id_tag_ {};
  using t_timer_id_ = named::t_int;
  using t_timer_id  = t_explicit<t_timer_id_, t_timer_id_tag_>;
  constexpr t_timer_id BAD_TIMER_ID{-1};

  using t_timer_ids = container::list::t_list<t_timer_id, 100>;
  using r_timer_ids = t_prefix<t_timer_ids>::r_;

  enum  t_timer_user_tag_ {};
  using t_timer_user = named::t_user<t_timer_user_tag_>;

  struct t_timer_params {
    t_bool       periodic = false;
    t_msec       interval = t_msec{0};
    t_msec       early    = t_msec{0};
    t_timer_user user;
  };
  using R_timer_params = t_prefix<t_timer_params>::R_;

  class t_timer_logic {
  public:
    virtual ~t_timer_logic() { };
    virtual t_void notify_timers_timeout(t_timer_id,
                                         R_timer_params) noexcept = 0;
  };
  using p_timer_logic = t_prefix<t_timer_logic>::p_;

  class t_timer_info {
  public:
    t_timer_id     id     = t_timer_id{0};
    p_timer_logic  logic  = nullptr;
    t_timer_params params;

    inline
    t_timer_info(p_timer_logic _logic, R_timer_params _params) noexcept
      : logic(_logic), params(_params) {
    }
  };
  using r_timer_info = t_prefix<t_timer_info>::r_;
  using P_timer_info = t_prefix<t_timer_info>::P_;

///////////////////////////////////////////////////////////////////////////////

  class t_impl_;
  enum  t_impl_owner_tag_ { };
  using t_impl_owner_ = named::ptr::t_ptr<t_impl_, t_impl_owner_tag_,
                                          named::ptr::t_deleter>;

///////////////////////////////////////////////////////////////////////////////

  class t_timers;
  using r_timers = t_prefix<t_timers>::r_;
  using x_timers = t_prefix<t_timers>::x_;
  using R_timers = t_prefix<t_timers>::R_;

  class t_timers {
  public:
    class t_logic : public t_timer_logic {
    public:
      // error
      virtual t_void notify_timers_error    (t_errn) noexcept = 0;
      virtual t_void notify_timers_processed()       noexcept = 0;
    };
    using p_logic = t_prefix<t_logic>::p_;

    t_timers(       t_n max) noexcept;
    t_timers(t_err, t_n max) noexcept;
    t_timers(x_timers)       noexcept;

    t_timers(R_timers)           = delete;
    r_timers operator=(R_timers) = delete;
    r_timers operator=(x_timers) = delete;

    operator t_validity() const noexcept;
    t_fd     get_fd    () const noexcept;

    t_timer_id start_timer(       R_timer_params,
                                  p_timer_logic = nullptr) noexcept;
    t_timer_id start_timer(t_err, R_timer_params,
                                  p_timer_logic = nullptr) noexcept;
    t_errn restart_timer  (       t_timer_id, R_timer_params) noexcept;
    t_void restart_timer  (t_err, t_timer_id, R_timer_params) noexcept;

    t_errn restart_timer  (       t_timer_id) noexcept;
    t_void restart_timer  (t_err, t_timer_id) noexcept;

    p_timer_logic stop_timer  (t_timer_id) noexcept;
    t_void        clear_timers()           noexcept;

    t_bool is_timer_running(t_timer_id) const noexcept;


    P_timer_info get_info(       t_timer_id) const noexcept;
    P_timer_info get_info(t_err, t_timer_id) const noexcept;

    t_void get_timer_ids(r_timer_ids)        const noexcept;

    t_errn notify_expiry(       p_logic) noexcept;
    t_void notify_expiry(t_err, p_logic) noexcept;

    t_errn event_loop(       p_logic) noexcept;
    t_void event_loop(t_err, p_logic) noexcept;

  private:
    t_impl_owner_ impl_;
  };

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
