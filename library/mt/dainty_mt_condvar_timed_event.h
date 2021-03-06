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

#ifndef _DAINTY_MT_TIMED_CONDVAR_EVENT_H_
#define _DAINTY_MT_TIMED_CONDVAR_EVENT_H_

#include "dainty_base_ptr.h"
#include "dainty_os_clock.h"
#include "dainty_mt_err.h"

namespace dainty
{
namespace mt
{
namespace condvar_timed_event
{
///////////////////////////////////////////////////////////////////////////////

  using err::t_err;
  using oops::t_oops;

  using base::types::t_prefix;
  using base::types::t_void;
  using base::types::t_uint64;

  using base::specific::t_specific;
  using base::specific::t_n;
  using base::specific::t_validity;
  using base::specific::t_errn;
  using base::specific::VALID;
  using base::specific::INVALID;

  using base::ptr::t_ptr;
  using base::ptr::t_no_deleter;
  using base::ptr::t_deleter;

  using os::clock::t_time;

///////////////////////////////////////////////////////////////////////////////

  enum  t_user_tag_ { };
  using t_user = base::t_user<t_user_tag_>;

  enum  t_cnt_tag_ {};
  using t_cnt_ = t_uint64;
  using t_cnt  = t_specific<t_cnt_, t_cnt_tag_>;

///////////////////////////////////////////////////////////////////////////////

  class t_impl_;
  enum  t_impl_user_tag_ { };
  using t_impl_user_ = t_ptr<t_impl_, t_impl_user_tag_, t_no_deleter>;

  enum  t_impl_owner_tag_ { };
  using t_impl_owner_ = t_ptr<t_impl_, t_impl_owner_tag_, t_deleter>;

///////////////////////////////////////////////////////////////////////////////

  class t_client;
  using r_client = t_prefix<t_client>::r_;
  using x_client = t_prefix<t_client>::x_;
  using R_client = t_prefix<t_client>::R_;

  class t_client {
  public:
    t_client(x_client) noexcept;

    r_client operator=(R_client) = delete;
    r_client operator=(x_client) = delete;

    operator t_validity() const noexcept;

    t_errn post(       t_cnt = t_cnt{1}) noexcept;
    t_void post(t_err, t_cnt = t_cnt{1}) noexcept;

  private:
    friend class t_processor;
    friend class t_impl_;
    t_client() = default;
    t_client(t_impl_user_, t_user) noexcept;

    t_impl_user_ impl_;
    t_user       user_ = t_user{0L};
  };

///////////////////////////////////////////////////////////////////////////////

  class t_processor;
  using r_processor = t_prefix<t_processor>::r_;
  using x_processor = t_prefix<t_processor>::x_;
  using R_processor = t_prefix<t_processor>::R_;

  class t_processor {
  public:
    class t_logic {
    public:
      using t_err  = t_oops<>;
      using t_cnt  = condvar_timed_event::t_cnt;
      using t_time = t_prefix<condvar_timed_event::t_time>::t_;
      using R_time = t_prefix<condvar_timed_event::t_time>::R_;

      virtual ~t_logic() { }
      virtual t_void async_process  (t_cnt)  noexcept = 0;
      virtual t_void timeout_process(R_time) noexcept = 0;
    };

    using r_logic = t_prefix<t_logic>::r_;
    using R_time  = t_logic::R_time;

     t_processor(t_err)       noexcept;
     t_processor(x_processor) noexcept;
    ~t_processor();

    t_processor(R_processor)           = delete;
    r_processor operator=(x_processor) = delete;
    r_processor operator=(R_processor) = delete;

    operator t_validity () const noexcept;
    t_cnt    get_cnt(t_err);

    t_void            process(t_err, r_logic, R_time,
                              t_n max = t_n{1}) noexcept;
    t_void reset_then_process(t_err, r_logic, R_time,
                              t_n max = t_n{1}) noexcept;

    t_client make_client(       t_user) noexcept;
    t_client make_client(t_err, t_user) noexcept;

  private:
    t_impl_owner_ impl_;
  };

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
