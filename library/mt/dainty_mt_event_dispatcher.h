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

#ifndef _DAINTY_MT_EVENT_DISPATCHER_H_
#define _DAINTY_MT_EVENT_DISPATCHER_H_

#include "dainty_base_ptr.h"
#include "dainty_base_string.h"
#include "dainty_container_freelist.h"
#include "dainty_container_list.h"
#include "dainty_container_ptrlist.h"
#include "dainty_mt_err.h"

namespace dainty
{
namespace mt
{
namespace event_dispatcher
{
///////////////////////////////////////////////////////////////////////////////

  using err::t_err;

  using base::types::t_prefix;
  using base::types::t_void;
  using base::types::t_bool;
  using base::types::t_short;
  using base::types::t_uchar;

  using base::specific::t_specific;
  using base::specific::t_fd;
  using base::specific::t_n;
  using base::specific::T_n;
  using base::specific::t_msec;
  using base::specific::r_msec;
  using base::specific::t_ix;
  using base::specific::t_validity;
  using base::specific::t_errn;
  using base::specific::t_prefix;
  using base::specific::VALID;
  using base::specific::INVALID;
  using base::specific::BAD_FD;

  using base::ptr::t_ptr;
  using base::ptr::t_deleter;

  using base::string::t_string;
  using base::string::FMT;

///////////////////////////////////////////////////////////////////////////////

  enum  t_event_id_tag_ {};
  using t_event_id_ = t_short;
  using t_event_id  = t_specific<t_event_id_, t_event_id_tag_>;
  using t_event_ids = container::list::t_list<t_event_id, 200>;
  using r_event_ids = t_prefix<t_event_ids>::r_;

  enum  t_event_user_tag_ {};
  using t_event_user = base::t_user<t_event_user_tag_>;

  enum  t_service_name_tag_ {};
  using t_service_name = t_string<t_service_name_tag_, 20>;
  using T_service_name = t_prefix<t_service_name>::T_;
  using R_service_name = t_prefix<t_service_name>::R_;

  enum  t_name_tag_ {};
  using t_name = t_string<t_name_tag_, 20>;

  enum  t_event_type { RD_EVENT, WR_EVENT };

  using t_event_prio = t_uchar;

  enum  t_quit_tag_ {};
  using t_quit = t_specific<t_bool, t_quit_tag_>;

  constexpr t_quit     QUIT        {true};
  constexpr t_quit     DONT_QUIT   {false};
  constexpr t_event_id BAD_EVENT_ID{-1};

  enum t_cmd { QUIT_EVENT_LOOP, REMOVE_EVENT, CONTINUE };

///////////////////////////////////////////////////////////////////////////////

  class t_impl_;
  enum  t_impl_owner_tag_ { };
  using t_impl_owner_ = t_ptr<t_impl_, t_impl_owner_tag_, t_deleter>;

///////////////////////////////////////////////////////////////////////////////

  t_n            get_supported_services()     noexcept;
  t_service_name get_supported_service (t_ix) noexcept;

///////////////////////////////////////////////////////////////////////////////

  class t_event_params {
  public:
    t_fd         fd   = BAD_FD;
    t_event_type type = RD_EVENT;
    t_event_prio prio = t_event_prio{0};
    t_event_user user;
  };
  using r_event_params = t_prefix<t_event_params>::r_;
  using R_event_params = t_prefix<t_event_params>::R_;

///////////////////////////////////////////////////////////////////////////////

  class t_event_logic;
  using p_event_logic = t_prefix<t_event_logic>::p_;

  class t_action {
  public:
    using t_cmd         = event_dispatcher::t_cmd;
    using p_event_logic = event_dispatcher::p_event_logic;

    t_cmd         cmd;
    p_event_logic next;

    inline
    t_action(t_cmd         _cmd  = REMOVE_EVENT,
             p_event_logic _next = nullptr) noexcept
      : cmd(_cmd), next(_next) {
    }
  };

///////////////////////////////////////////////////////////////////////////////

  class t_event_logic {
  public:
    using t_event_id     = event_dispatcher::t_event_id;
    using r_event_params = event_dispatcher::r_event_params;
    using t_action       = event_dispatcher::t_action;

    virtual ~t_event_logic() { }
    virtual t_action notify_dispatcher_event(t_event_id,
                                             r_event_params) noexcept = 0;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_event_info {
  public:
    t_event_id     id     = t_event_id{0};
    p_event_logic  logic  = nullptr;
    t_event_params params;

    inline
    t_event_info(p_event_logic _logic, R_event_params _params) noexcept
      : logic(_logic), params(_params) {
    }
  };
  using r_event_info  = t_prefix<t_event_info>::r_;
  using p_event_info  = t_prefix<t_event_info>::p_;
  using P_event_info  = t_prefix<t_event_info>::P_;
  using t_event_infos = container::ptrlist::t_ptrlist<t_event_info>;
  using r_event_infos = t_prefix<t_event_infos>::r_;

///////////////////////////////////////////////////////////////////////////////

  class t_params {
  public:
    T_n            max;
    T_service_name service_name;

    inline
    t_params(t_n _max, R_service_name _name) noexcept
      : max(_max), service_name(_name) {
    }
  };
  using T_params = t_prefix<t_params>::T_;
  using R_params = t_prefix<t_params>::R_;

///////////////////////////////////////////////////////////////////////////////

  class t_dispatcher;
  using r_dispatcher = t_prefix<t_dispatcher>::r_;
  using x_dispatcher = t_prefix<t_dispatcher>::x_;
  using R_dispatcher = t_prefix<t_dispatcher>::R_;

  class t_dispatcher {
  public:
    class t_logic : public t_event_logic {
    public:
      using r_event_info   = event_dispatcher::r_event_info;
      using r_event_infos  = event_dispatcher::r_event_infos;
      using t_msec         = event_dispatcher::t_msec;
      using r_msec         = event_dispatcher::r_msec;
      using t_errn         = event_dispatcher::t_errn;
      using t_quit         = event_dispatcher::t_quit;

      virtual ~t_logic() { }

      virtual t_void notify_dispatcher_reorder  (r_event_infos) noexcept;
      virtual t_void notify_dispatcher_removed  (r_event_info)  noexcept;
      virtual t_quit notify_dispatcher_processed(r_msec)        noexcept;
      virtual t_quit notify_dispatcher_timeout  (t_msec)        noexcept = 0;
      virtual t_quit notify_dispatcher_error    (t_errn)        noexcept = 0;
    };
    using r_logic = t_prefix<t_logic>::r_;

    t_dispatcher(       R_params) noexcept;
    t_dispatcher(t_err, R_params) noexcept;
    t_dispatcher(x_dispatcher)    noexcept;
   ~t_dispatcher();

    t_dispatcher()                        = delete;
    t_dispatcher(R_dispatcher)            = delete;
    r_dispatcher operator=(x_dispatcher)  = delete;
    r_dispatcher operator=(R_dispatcher)  = delete;

    operator      t_validity  () const noexcept;
    t_params      get_params  () const noexcept;
    t_void        display     () const noexcept;

    t_event_id    add_event(       R_event_params,
                                   p_event_logic = nullptr) noexcept;
    t_event_id    add_event(t_err, R_event_params,
                                   p_event_logic = nullptr) noexcept;
    p_event_logic del_event(       t_event_id)              noexcept;
    p_event_logic del_event(t_err, t_event_id)              noexcept;

    t_void        clear_events()      noexcept;
    t_void        clear_events(t_err) noexcept;

    P_event_info  get_event(       t_event_id) const noexcept;
    P_event_info  get_event(t_err, t_event_id) const noexcept;

    t_bool        get_events(r_event_ids) const noexcept;

    t_n           event_loop(       r_logic) noexcept;
    t_n           event_loop(t_err, r_logic) noexcept;

    t_n           event_loop(       r_logic, t_msec) noexcept;
    t_n           event_loop(t_err, r_logic, t_msec) noexcept;

  private:
    t_impl_owner_ impl_;
  };

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
