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

#include "dainty_base.h"
#include "dainty_base_ptr.h"
#include "dainty_base_string.h"
#include "dainty_container_list.h"
#include "dainty_container_ptrlist.h"
#include "dainty_mt_err.h"

namespace dainty
{
namespace mt
{
namespace timers
{
///////////////////////////////////////////////////////////////////////////////

  using err::t_err;

  using base::types::t_prefix;
  using base::types::t_void;
  using base::types::t_uchar;
  using base::types::t_bool;
  using base::types::t_int;

  using base::specific::t_specific;
  using base::specific::t_fd;
  using base::specific::t_ix;
  using base::specific::t_n;
  using base::specific::T_n;
  using base::specific::t_msec;
  using base::specific::t_errn;
  using base::specific::t_validity;
  using base::specific::t_prefix;
  using base::specific::t_percentage;
  using base::specific::T_percentage;
  using base::specific::VALID;
  using base::specific::INVALID;
  using base::specific::BAD_FD;

  using base::string::t_string;

  using base::ptr::t_ptr;
  using base::ptr::t_deleter;

///////////////////////////////////////////////////////////////////////////////

  enum  t_timer_id_tag_ {};
  using t_timer_id_ = t_int;
  using t_timer_id  = t_specific<t_timer_id_, t_timer_id_tag_>;
  constexpr t_timer_id BAD_TIMER_ID{-1};

  using t_timer_ids = container::list::t_list<t_timer_id, 100>;
  using r_timer_ids = t_prefix<t_timer_ids>::r_;

  enum  t_timer_user_tag_ {};
  using t_timer_user = base::t_user<t_timer_user_tag_>;

  using t_timer_prio = t_uchar;
  using T_timer_prio = t_prefix<t_timer_prio>::T_;

  enum  t_service_name_tag_ {};
  using t_service_name = t_string<t_service_name_tag_, 20>;
  using T_service_name = t_prefix<t_service_name>::T_;
  using R_service_name = t_prefix<t_service_name>::R_;

  struct t_timer_params {
    t_bool       periodic = false;
    t_msec       timeout  = t_msec{0};
    t_msec       early    = t_msec{0};
    t_timer_prio prio     = t_timer_prio{0};
    t_timer_user user;
  };
  using R_timer_params = t_prefix<t_timer_params>::R_;

  class t_timer_logic {
  public:
    using t_timer_id     = timers::t_timer_id;
    using t_timer_params = timers::t_timer_params;
    using R_timer_params = timers::R_timer_params;

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
  };
  using r_timer_info  = t_prefix<t_timer_info>::r_;
  using P_timer_info  = t_prefix<t_timer_info>::P_;
  using t_timer_infos = container::ptrlist::t_ptrlist<t_timer_info>;
  using r_timer_infos = t_prefix<t_timer_infos>::r_;

///////////////////////////////////////////////////////////////////////////////

  class t_impl_;
  enum  t_impl_owner_tag_ { };
  using t_impl_owner_ = t_ptr<t_impl_, t_impl_owner_tag_, t_deleter>;

///////////////////////////////////////////////////////////////////////////////

  t_n            get_supported_services()     noexcept;
  t_service_name get_supported_service (t_ix) noexcept;

///////////////////////////////////////////////////////////////////////////////

  class t_params {
  public:
    T_n            max;
    T_percentage   overrun; // zero disable testing
    T_service_name service_name;

    inline
    t_params(t_n _max, t_percentage _overrun, R_service_name _name) noexcept
      : max(_max), overrun{_overrun}, service_name(_name) {
    }
  };
  using T_params = t_prefix<t_params>::T_;
  using R_params = t_prefix<t_params>::R_;

///////////////////////////////////////////////////////////////////////////////

  class t_timers;
  using r_timers = t_prefix<t_timers>::r_;
  using x_timers = t_prefix<t_timers>::x_;
  using R_timers = t_prefix<t_timers>::R_;

  class t_timers {
  public:
    class t_logic : public t_timer_logic {
    public:
      using t_errn        = timers::t_errn;
      using r_timer_infos = timers::r_timer_infos;

      virtual ~t_logic() { }

      virtual t_void notify_timers_reorder  (r_timer_infos)      noexcept;
      virtual t_void notify_timers_overrun  (t_timer_id, t_msec) noexcept;
      virtual t_void notify_timers_processed()                   noexcept;
    };
    using r_logic = t_prefix<t_logic>::r_;

    t_timers(       R_params) noexcept;
    t_timers(t_err, R_params) noexcept;
    t_timers(x_timers)        noexcept;
   ~t_timers();

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

    t_bool        stop_timer (t_timer_id) noexcept;
    p_timer_logic clear_timer(t_timer_id) noexcept;

    t_void        clear_timers()           noexcept;

    t_bool is_timer_running(t_timer_id) const noexcept;


    P_timer_info get_info(       t_timer_id) const noexcept;
    P_timer_info get_info(t_err, t_timer_id) const noexcept;

    t_void get_timer_ids(r_timer_ids)        const noexcept;

    t_errn process(       r_logic) noexcept;
    t_void process(t_err, r_logic) noexcept;

  private:
    t_impl_owner_ impl_;
  };

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
