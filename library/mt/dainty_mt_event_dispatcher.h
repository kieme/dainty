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

#include "dainty_named_ptr.h"
#include "dainty_named_string.h"
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

  using named::t_fd;
  using named::T_fd;
  using named::t_void;
  using named::t_bool;
  using named::t_n;
  using named::T_n;
  using named::t_usec;
  using named::t_ix;
  using named::string::t_string;
  using named::string::FMT;
  using named::t_validity;
  using named::t_errn;
  using named::t_prefix;
  using named::t_explicit;
  using named::VALID;
  using named::INVALID;
  using named::BAD_FD;
  using err::t_err;

  enum  t_event_id_tag_ {};
  using t_event_id_ = named::t_short;
  using t_event_id  = t_explicit<t_event_id_, t_event_id_tag_>;
  using T_event_id  = t_prefix<t_event_id>::T_;
  using t_event_ids = container::list::t_list<t_event_id, 200>;
  using r_event_ids = t_prefix<t_event_ids>::r_;

  enum  t_event_user_tag_ {};
  using t_event_user = named::t_user<t_event_user_tag_>;

  enum  t_service_name_tag_ {};
  using t_service_name = t_string<t_service_name_tag_, 20>;
  using T_service_name = t_prefix<t_service_name>::T_;
  using R_service_name = t_prefix<t_service_name>::R_;

  enum  t_name_tag_ {};
  using t_name = t_string<t_name_tag_, 20>;

  enum  t_event_type { RD_EVENT, WR_EVENT };
  using T_event_type = t_prefix<t_event_type>::T_;

  using t_event_prio = named::t_uchar;
  using T_event_prio = t_prefix<t_event_prio>::T_;

  enum  t_quit_tag_ {};
  using t_quit = t_explicit<t_bool, t_quit_tag_>;

  constexpr t_quit     QUIT        {true};
  constexpr t_quit     DONT_QUIT   {false};
  constexpr t_event_id BAD_EVENT_ID{-1};

  enum  t_cmd  { QUIT_EVENT_LOOP, REMOVE_EVENT, CONTINUE };
  using T_cmd = t_prefix<t_cmd>::T_;

///////////////////////////////////////////////////////////////////////////////

  class t_impl_;
  enum  t_impl_owner_tag_ { };
  using t_impl_owner_ = named::ptr::t_ptr<t_impl_, t_impl_owner_tag_,
                                          named::ptr::t_deleter>;

///////////////////////////////////////////////////////////////////////////////

  t_n            get_supported_services()     noexcept;
  t_service_name get_supported_service (t_ix) noexcept;

///////////////////////////////////////////////////////////////////////////////

  class t_event_params {
  public:
    T_fd         fd;
    T_event_type type;
    T_event_prio prio;
    t_event_user user;

    inline
    t_event_params(t_fd _fd, t_event_type _type,
                   t_event_prio _prio = 0,
                   t_event_user _user = t_event_user{0L}) noexcept
      : fd{_fd}, type(_type), prio(_prio), user(_user) {
    }
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

    T_cmd         cmd;
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
    using r_event_params = event_dispatcher::r_event_params;
    using t_action       = event_dispatcher::t_action;

    virtual ~t_event_logic() { }
    virtual t_action notify_event(r_event_params) noexcept = 0;
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
  using r_event_info = named::t_prefix<t_event_info>::r_;
  using p_event_info = named::t_prefix<t_event_info>::p_;
  using P_event_info = named::t_prefix<t_event_info>::P_;

  using t_event_infos = container::ptrlist::t_ptrlist<t_event_info>;
  using r_event_infos = named::t_prefix<t_event_infos>::r_;

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
  using T_params = named::t_prefix<t_params>::T_;
  using R_params = named::t_prefix<t_params>::R_;

///////////////////////////////////////////////////////////////////////////////

  class t_dispatcher;
  using r_dispatcher = t_prefix<t_dispatcher>::r_;
  using x_dispatcher = t_prefix<t_dispatcher>::x_;
  using R_dispatcher = t_prefix<t_dispatcher>::R_;

  class t_dispatcher {
  public:
    class t_logic {
    public:
      using r_event_info   = event_dispatcher::r_event_info;
      using r_event_infos  = event_dispatcher::r_event_infos;
      using t_usec         = event_dispatcher::t_usec;
      using t_errn         = event_dispatcher::t_errn;
      using t_quit         = event_dispatcher::t_quit;

      virtual ~t_logic() { }

      virtual t_void notify_may_reorder  (r_event_infos) noexcept = 0;
      virtual t_void notify_removed      (r_event_info)  noexcept = 0;
      virtual t_quit notify_timeout      (t_usec)        noexcept = 0;
      virtual t_quit notify_error        (t_errn)        noexcept = 0;
      virtual t_quit notify_all_processed()              noexcept = 0;
    };
    using p_logic = t_prefix<t_logic>::p_;

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
    t_fd          get_fd_impl_() const noexcept;

    t_event_id    add_event(       R_event_params, p_event_logic) noexcept;
    t_event_id    add_event(t_err, R_event_params, p_event_logic) noexcept;
    p_event_logic del_event(       t_event_id)                    noexcept;
    p_event_logic del_event(t_err, t_event_id)                    noexcept;

    t_void        clear_events()      noexcept;
    t_void        clear_events(t_err) noexcept;

    P_event_info  get_event(       t_event_id) const noexcept;
    P_event_info  get_event(t_err, t_event_id) const noexcept;

    t_bool        get_events(r_event_ids) const noexcept;

    t_n           event_loop(       p_logic) noexcept;
    t_n           event_loop(t_err, p_logic) noexcept;

    t_n           event_loop(       p_logic, t_usec) noexcept;
    t_n           event_loop(t_err, p_logic, t_usec) noexcept;

  private:
    t_impl_owner_ impl_;
  };

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
