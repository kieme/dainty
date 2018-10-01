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

#ifndef _DAINTY_SANDBOX_H_
#define _DAINTY_SANDBOX_H_

#include "dainty_named.h"
#include "dainty_named_string.h"
#include "dainty_os_clock.h"
#include "dainty_os_threading_thread.h"
#include "dainty_tracing_tracer.h"
#include "dainty_messaging_messenger.h"
#include "dainty_sandbox_err.h"

///////////////////////////////////////////////////////////////////////////////

#define SB_DEBUG(...) \
  TRACE_0(get_trace(), trace::level_debug, ( __VA_ARGS__ ))
#define SB_NOTICE(...) \
  TRACE_0(get_trace(), trace::level_notice, ( __VA_ARGS__ ))
#define SB_WARNING(...) \
  TRACE_0(get_trace(), trace::level_warning, ( __VA_ARGS__ ))
#define SB_ERROR(...) \
  TRACE_0(get_trace(), trace::level_error, ( __VA_ARGS__ ))

///////////////////////////////////////////////////////////////////////////////

namespace dainty
{
namespace sandbox
{
  using named::t_explicit;
  using named::t_void;
  using named::t_bool;
  using named::t_n;
  using named::t_fd;
  using named::t_prefix;
  using named::t_errn;
  using named::t_fd;
  using named::t_nsec;
  using err::t_err;

  enum multiple_of_1ms_tag_t { };
  typedef strongtype_t<uint_t, multiple_of_1ms_tag_t> multiple_of_1ms_t;

  enum label_tag_t { };
  typedef string::nstring_t<16, label_tag_t> label_t;

  class t_logic_impl_;
  using p_logic_impl_ = t_prefix<t_logic_impl_>::p_;

///////////////////////////////////////////////////////////////////////////////

  class message_handler_t {
  public:
    typedef messaging::message_t message_t;

    message_handler_t(const label_t& label) : label_(label) { }

    virtual ~message_handler_t() { }
    virtual t_bool handle(message_t&) = 0;

    const label_t label_;
  };

///////////////////////////////////////////////////////////////////////////////

  class stats_t {
  public:
  };

///////////////////////////////////////////////////////////////////////////////

  class t_logic : public detached::thread:t_logic {
  public:
    typedef trace::source_t                      tracepoint_t;
    typedef sandbox::multiple_of_1ms_t           multiple_of_1ms_t;
    typedef sandbox::t_fd                        t_fd;
    typedef sandbox::stats_t                     stats_t;
    typedef sandbox::label_t                     label_t;
    typedef sandbox::nanoseconds_t               nanoseconds_t;
    typedef sandbox::message_handler_t           message_handler_t;
    typedef messaging::multiple_of_100ms_t       multiple_of_100ms_t;
    typedef messaging::password_t                password_t;
    typedef messaging::messenger_timer_params_t  messenger_timer_params_t;
    typedef messaging::messenger_key_t           messenger_key_t;
    typedef messaging::messenger_visibility_t    messenger_visibility_t;
    typedef messaging::messenger_state_t         messenger_state_t;
    typedef messaging::messenger_prio_t          messenger_prio_t;
    typedef messaging::messenger_user_t          messenger_user_t;
    typedef messaging::messenger_name_t          messenger_name_t;
    typedef messaging::messenger_create_params_t messenger_create_params_t;
    typedef messaging::messenger_params_t        messenger_params_t;
    typedef messaging::message_t                 message_t;
    typedef messaging::messages_t                messages_t;
    typedef messaging::message_id_t              message_id_t;
    typedef messaging::messenger_memberlist_t    messenger_memberlist_t;
    typedef messaging::messenger_memberkeylist_t messenger_memberkeylist_t;

    t_logic(R_name, R_create_params_);

    t_logic(R_logic) = delete;
    t_logic(x_logic) = delete;
    r_logic operator=(R_logic) = delete;
    r_logic operator=(x_logic) = delete;

    virtual ~t_logic();

    operator t_validity() const;

    t_tracer& get_trace(t_err) const; // make trace point
    t_name    get_name (t_err) const;
    t_key     get_key  () const;
    t_bool    get_params(t_err, r_messenger_params) const;
    t_void    get_stats (t_err, stats_t&, t_bool reset = false);

///////////////////////////////////////////////////////////////////////////////

    t_void post_message (t_err, R_messenger_key, r_message) const;

///////////////////////////////////////////////////////////////////////////////

    t_void update_visibility  (t_err, t_visibility);
    t_void update_alive_period(t_err, t_multiple_of_100ms);

///////////////////////////////////////////////////////////////////////////////

    t_void start_message_timer(t_errr, R_message_timer_params);
    t_void stop_message_timer (t_err);
    t_bool query_message_timer(t_err, r_message_timer_params) const;

///////////////////////////////////////////////////////////////////////////////

    t_bool add_monitor(const messenger_name_t&,
                       messenger_prio_t = messenger_prio_t(0),
                       messenger_user_t = messenger_user_t());
    t_bool remove_monitor(const messenger_name_t&,
                          messenger_user_t* = 0);
    messenger_key_t is_monitored(const messenger_name_t&,
                                 messenger_user_t* = 0) const;
    t_bool get_monitored(messenger_memberkeylist_t&) const;

///////////////////////////////////////////////////////////////////////////////

    t_bool add_to_group(const password_t&, const messenger_name_t& group,
                        messenger_prio_t = messenger_prio_t(0),
                        messenger_user_t = messenger_user_t());
    t_bool remove_from_group(const password_t&, const messenger_name_t&,
                             messenger_user_t* = 0);
    t_bool is_in_group(const messenger_name_t&,
                       messenger_user_t* = 0) const;
    t_bool get_groups(messenger_memberlist_t&) const;

///////////////////////////////////////////////////////////////////////////////

    t_bool create_group(const password_t&, const messenger_name_t&,
                        messenger_visibility_t);
    t_bool destroy_group(const password_t&, const messenger_name_t&);
    t_bool fetch_group(const messenger_name_t&,
                       messenger_visibility_t&, messenger_memberlist_t* = 0);

///////////////////////////////////////////////////////////////////////////////

    t_bool add_another_to_group(const password_t&,
                                const messenger_name_t& name,
                                const messenger_name_t& group,
                                messenger_prio_t = messenger_prio_t(0),
                                messenger_user_t = messenger_user_t());
    t_bool remove_another_from_group(const password_t&,
                                     const messenger_name_t& name,
                                     const messenger_name_t& group,
                                     messenger_user_t* = 0);
    t_bool is_another_in_group(const messenger_name_t& name,
                               const messenger_name_t& group,
                               messenger_user_t* = 0);

///////////////////////////////////////////////////////////////////////////////

    multiple_of_1ms_t get_max_wait() const;
    t_void            set_max_wait(multiple_of_1ms_t);

///////////////////////////////////////////////////////////////////////////////

    t_bool   register_message_handler     (R_message_id, message_handler_t*);
    t_bool unregister_message_handler     (R_message_id);
    t_bool   is_message_handler_registered(message_handler_t*) const;

///////////////////////////////////////////////////////////////////////////////

    // WR/RX
    t_bool add_event_monitor   (t_fd, const label_t&, messenger_user_t);
    t_bool remove_event_monitor(t_fd, label_t* = 0, messenger_user_t* = 0);
    t_bool is_event_monitored  (t_fd, label_t* = 0, messenger_user_t* = 0);

///////////////////////////////////////////////////////////////////////////////

   // timer services
   // name

///////////////////////////////////////////////////////////////////////////////

    virtual t_void handle_wakeup(nanoseconds_t elapsed, t_bool maxwait) = 0;
    virtual t_void handle_notify(messenger_state_t,
                                 const messenger_name_t&,
                                 const messenger_key_t&,
                                 messenger_prio_t,
                                 messenger_user_t) = 0;
    virtual t_void handle_user(message_t&) = 0;
    virtual t_void handle_bad(message_t&)  = 0;
    virtual t_void handle_timeout(t_bool periodic,
                                  multiple_of_100ms_t,
                                  const messenger_key_t&,
                                  messenger_prio_t,
                                  messenger_user_t) = 0;
    virtual t_void handle_failed(errorid_t, message_t&) = 0;
    virtual t_void handle_event(fd_t, const label_t&, messenger_user_t) = 0;

///////////////////////////////////////////////////////////////////////////////

  private:
    friend class t_logic_impl_;

    p_logic_impl_ impl_;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_sandbox;
  using r_sandbox = t_prefix<t_sandbox>::r_;
  using x_sandbox = t_prefix<t_sandbox>::x_;
  using R_sandbox = t_prefix<t_sandbox>::R_;

  class t_sandbox {
  public:
     t_sandbox(R_name, t_passable_ptr<t_logic>);
     t_sandbox(R_sandbox) = delete;
     t_sandbox(x_sandbox) = delete;
    ~t_sandbox();

     r_sandbox operator=(R_sandbox) = delete;
     r_sandbox operator=(x_sandbox) = delete;

  private:
    t_key    key_;
    t_thread thread_;
  };

///////////////////////////////////////////////////////////////////////////////
}
}

#endif
