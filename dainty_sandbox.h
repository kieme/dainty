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
#include "dainty_named_ptr.h"
#include "dainty_named_string.h"
#include "dainty_named_terminal.h"
#include "dainty_os_clock.h"
#include "dainty_mt_detached_thread.h"
#include "dainty_tracing.h"
#include "dainty_messaging.h"
#include "dainty_sandbox_err.h"

///////////////////////////////////////////////////////////////////////////////

#define SB_EMERG(TEXT)    DAINTY_TR_EMERG(tracer_, TEXT)
#define SB_ALERT(TEXT)    DAINTY_TR_ALERT(tracer_, TEXT)
#define SB_CRITICAL(TEXT) DAINTY_TR_CRITICAL(tracer_, TEXT)
#define SB_ERROR(TEXT)    DAINTY_TR_ERROR(tracer_, TEXT)
#define SB_WARNING(TEXT)  DAINTY_TR_WARNING(tracer_, TEXT)
#define SB_NOTICE(TEXT)   DAINTY_TR_NOTICE(tracer_, TEXT)
#define SB_INFO(TEXT)     DAINTY_TR_INFO(tracer_, TEXT)
#define SB_DEBUG(TEXT)    DAINTY_TR_DEBUG(tracer_, TEXT)

///////////////////////////////////////////////////////////////////////////////

namespace dainty
{
namespace sandbox
{
  using named::t_explicit;
  using named::t_prefix;
  using named::t_validity;
  using named::t_void;
  using named::t_bool;
  using named::t_n;
  using named::t_fd;
  using named::t_msec;
  using named::ptr::t_ptr;
  using named::string::t_string;
  using named::VALID;
  using named::INVALID;
  using os::clock::t_time;
  using messaging::message::t_message;
  using namespace dainty::named::terminal;

  enum  t_label_tag_ { };
  using t_label = t_string<t_label_tag_>;
  using R_label = t_prefix<t_label>::R_;
  using p_label = t_prefix<t_label>::p_;

///////////////////////////////////////////////////////////////////////////////

  class t_message_logic {
  public:
    using r_message = messaging::message::r_message;

    virtual ~t_message_logic() { }
    virtual t_bool process(r_message) = 0;
  };
  using p_message_logic = t_prefix<t_message_logic>::p_;

///////////////////////////////////////////////////////////////////////////////

  class t_stats {
  public:
  };
  using r_stats = t_prefix<t_stats>::r_;

///////////////////////////////////////////////////////////////////////////////

  class t_impl_;
  using p_impl_ = t_prefix<t_impl_>::p_;

///////////////////////////////////////////////////////////////////////////////

  using t_thread_           = mt::detached_thread::t_thread;
  using t_thread_logic_     = t_thread_::t_logic;

///////////////////////////////////////////////////////////////////////////////

  class t_logic;
  using r_logic = t_prefix<t_logic>::r_;
  using x_logic = t_prefix<t_logic>::x_;
  using R_logic = t_prefix<t_logic>::R_;

  class t_logic : private t_thread_logic_ {
  public:
    using t_err                     = sandbox::err::t_err;
    using t_fd                      = sandbox::t_fd;
    using t_stats                   = sandbox::t_stats;
    using t_label                   = sandbox::t_label;
    using t_message_logic           = sandbox::t_message_logic;
    using t_tracer                  = tracing::tracer::t_tracer;
    using R_password                = messaging::R_password;
    using t_message                 = messaging::message::t_message;
    using x_message                 = messaging::message::x_message;
    using t_messages                = messaging::messenger::t_messages;
    using t_message_id              = messaging::message::t_id;
    using R_message_id              = messaging::message::R_id;
    using t_multiple_of_100ms       = messaging::t_multiple_of_100ms;
    using t_messenger_scope         = messaging::t_messenger_scope;
    using r_messenger_scope         = messaging::r_messenger_scope;
    using p_messenger_name_list     = messaging::p_messenger_name_list;
    using R_messenger_password      = messaging::R_messenger_password;
    using t_messenger_prio          = messaging::t_messenger_prio;
    using t_messenger_user          = messaging::t_messenger_user;
    using p_messenger_user          = messaging::p_messenger_user;
    using t_messenger_name          = messaging::t_messenger_name;
    using R_messenger_name          = messaging::R_messenger_name;
    using t_messenger_key           = messaging::t_messenger_key;
    using R_messenger_key           = messaging::R_messenger_key;
    using t_messenger_state         = messaging::message::t_messenger_state;
    using t_messenger_params        = messaging::t_messenger_params;
    using r_messenger_params        = messaging::messenger::r_params;
    using r_messenger_timer_params  = messaging::messenger::r_timer_params;
    using R_messenger_timer_params  = messaging::messenger::R_timer_params;
    using r_messenger_group_list    = messaging::messenger::r_group_list;
    using r_messenger_monitor_list  = messaging::messenger::r_monitor_list;
    using R_messenger_create_params = messaging::R_messenger_create_params;

    t_logic(t_err, R_messenger_name, R_messenger_create_params);
    virtual ~t_logic();

    t_logic(R_logic)           = delete;
    t_logic(x_logic)           = delete;
    r_logic operator=(R_logic) = delete;
    r_logic operator=(x_logic) = delete;

    operator t_validity() const;

    t_messenger_key  get_key   () const;
    t_messenger_name get_name  (t_err) const;
    t_void           get_params(t_err, r_messenger_params) const;
    t_void           get_stats (t_err, r_stats, t_bool reset = false);

///////////////////////////////////////////////////////////////////////////////

    t_void post_message(t_err, R_messenger_key, x_message);

///////////////////////////////////////////////////////////////////////////////

    t_void update_scope       (t_err, t_messenger_scope);
    t_void update_alive_period(t_err, t_multiple_of_100ms);

///////////////////////////////////////////////////////////////////////////////

    t_void start_message_timer(t_err, R_messenger_timer_params);
    t_void stop_message_timer (t_err);
    t_void query_message_timer(t_err, r_messenger_timer_params) const;

///////////////////////////////////////////////////////////////////////////////

    t_void add_monitor(t_err, R_messenger_name,
                              t_messenger_prio = t_messenger_prio(0),
                              t_messenger_user = t_messenger_user());
    t_void remove_monitor(t_err, R_messenger_name, p_messenger_user = nullptr);
    t_messenger_key is_monitored(t_err, R_messenger_name,
                                        p_messenger_user = nullptr) const;
    t_void get_monitored(t_err, r_messenger_monitor_list) const;

///////////////////////////////////////////////////////////////////////////////

    t_void add_to_group(t_err, R_messenger_password, R_messenger_name group,
                               t_messenger_prio = t_messenger_prio(0),
                               t_messenger_user = t_messenger_user());
    t_void remove_from_group(t_err, R_messenger_password, R_messenger_name,
                                    p_messenger_user = nullptr);
    t_bool is_in_group(t_err, R_messenger_name,
                              p_messenger_user = nullptr) const;
    t_void get_groups(t_err, r_messenger_group_list) const;

///////////////////////////////////////////////////////////////////////////////

    t_void create_group (t_err, R_password, R_messenger_name,
                                t_messenger_scope);
    t_void destroy_group(t_err, R_password, R_messenger_name);
    t_bool is_group     (t_err, R_messenger_name, r_messenger_scope,
                                p_messenger_name_list = nullptr);

///////////////////////////////////////////////////////////////////////////////

    t_void add_another_to_group(t_err, R_messenger_password,
                                       R_messenger_name name,
                                       R_messenger_name group,
                                       t_messenger_prio = t_messenger_prio(0),
                                       t_messenger_user = t_messenger_user());
    t_void remove_another_from_group(t_err, R_messenger_password,
                                            R_messenger_name,
                                            R_messenger_name group,
                                            p_messenger_user = nullptr);
    t_bool is_another_in_group(t_err, R_messenger_name,
                                      R_messenger_name group,
                                      p_messenger_user = nullptr);

///////////////////////////////////////////////////////////////////////////////

    t_msec get_max_wait() const;
    t_void set_max_wait(t_err, t_msec);

///////////////////////////////////////////////////////////////////////////////

    t_void register_message_logic(t_err, R_message_id, R_label,
                                         p_message_logic); // XXX - t_ptr?
    p_message_logic unregister_message_logic(t_err, R_label);
    p_message_logic is_message_logic_registered(t_err, R_label) const;

///////////////////////////////////////////////////////////////////////////////

    t_void add_event_monitor   (t_err, t_fd, R_label, t_messenger_user);
    t_void remove_event_monitor(t_err, t_fd, p_label = nullptr,
                                       p_messenger_user = nullptr);
    t_bool is_event_monitored  (t_err, t_fd, p_label = nullptr,
                                       p_messenger_user = nullptr);

///////////////////////////////////////////////////////////////////////////////

   // timer services

///////////////////////////////////////////////////////////////////////////////

    virtual t_void process_wakeup(t_time elapsed, t_bool maxwait) = 0;
    virtual t_void process_notify(t_messenger_state,
                                  R_messenger_name,
                                  R_messenger_key,
                                  t_messenger_prio,
                                  t_messenger_user) = 0;
    virtual t_void process_user(x_message) = 0;
    virtual t_void process_bad (x_message) = 0;
    virtual t_void process_timeout(t_bool periodic,
                                   t_multiple_of_100ms,
                                   R_messenger_key,
                                   t_messenger_prio,
                                   t_messenger_user) = 0;
    //virtual t_void process_failed(errorid_t, x_message) = 0;
    virtual t_void process_event(t_fd, R_label, t_messenger_user) = 0;

  protected:
    t_validity valid_ = INVALID;
    t_tracer   tracer_;

  private:
    friend class t_sandbox;
    friend class t_main;
    friend class t_impl_;

    using t_thread_err = t_thread_logic_::t_err;

    virtual t_void update(t_thread_err, os::r_pthread_attr) noexcept override;
    virtual t_void prepare(t_thread_err) noexcept override;
    virtual t_void run() noexcept override;

    using t_ptr_ = t_ptr<t_impl_, t_logic, named::ptr::t_deleter>;

    mutable t_ptr_ impl_;
  };
  using p_logic = t_prefix<t_logic>::p_;
  using r_logic = t_prefix<t_logic>::r_;

///////////////////////////////////////////////////////////////////////////////

  class t_sandbox;
  using r_sandbox = t_prefix<t_sandbox>::r_;
  using x_sandbox = t_prefix<t_sandbox>::x_;
  using R_sandbox = t_prefix<t_sandbox>::R_;

  class t_sandbox {
  public:
    using t_err  = t_logic::t_err;
    using R_name = t_logic::R_messenger_name; // thread name - XXX
    using t_ptr_ = t_ptr<t_logic, t_sandbox, named::ptr::t_deleter>;

    t_sandbox(t_err, R_name, t_ptr_);
    t_sandbox(x_sandbox);
    t_sandbox(R_sandbox) = delete;
    ~t_sandbox();

    r_sandbox operator=(x_sandbox);
    r_sandbox operator=(R_sandbox) = delete;

    operator t_validity() const;

  private:
    t_logic::t_messenger_key key_ = t_logic::t_messenger_key{0};
  };

///////////////////////////////////////////////////////////////////////////////

  class t_main;
  using r_main = t_prefix<t_main>::r_;
  using x_main = t_prefix<t_main>::x_;
  using R_main = t_prefix<t_main>::R_;

  class t_main {
  public:
    using t_err  = t_logic::t_err;
    using R_name = t_logic::R_messenger_name; // application name - XXX
    using t_ptr_ = t_ptr<t_logic, t_main, named::ptr::t_deleter>;

    t_main(t_err, R_name, t_ptr_);
    t_main(x_main) = delete;
    t_main(R_main) = delete;

    r_main operator=(x_main) = delete;
    r_main operator=(R_main) = delete;

    operator t_validity() const;

  private:
    t_logic::t_messenger_key key_ = t_logic::t_messenger_key{0};
  };

///////////////////////////////////////////////////////////////////////////////

  t_void start(err::t_err);

///////////////////////////////////////////////////////////////////////////////
}
}

#endif
