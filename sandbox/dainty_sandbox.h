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

#ifndef FRAMEWORK_SANDBOX_H
#define FRAMEWORK_SANDBOX_H

#include "framework_library_types.h"
#include "framework_library_time.h"
#include "framework_library_thread.h"
#include "framework_library_string_nstring.h"
#include "framework_service_trace_source.h"
#include "framework_service_messaging_messenger.h"

////////////////////////////////////////////////////////////////////////////////

#define SB_DEBUG(...) \
  TRACE_0(get_trace(), trace::level_debug, ( __VA_ARGS__ ))
#define SB_NOTICE(...) \
  TRACE_0(get_trace(), trace::level_notice, ( __VA_ARGS__ ))
#define SB_WARNING(...) \
  TRACE_0(get_trace(), trace::level_warning, ( __VA_ARGS__ ))
#define SB_ERROR(...) \
  TRACE_0(get_trace(), trace::level_error, ( __VA_ARGS__ ))

////////////////////////////////////////////////////////////////////////////////

namespace dainty
{
namespace sandbox
{
  using namespace service;
  using namespace library;

  using types::strongtype_t;
  using types::errorid_t;
  using types::void_t;
  using types::bool_t;
  using types::uint_t;
  using types::fd_t;
  using time::nanoseconds_t;

  enum multiple_of_1ms_tag_t { };
  typedef strongtype_t<uint_t, multiple_of_1ms_tag_t> multiple_of_1ms_t;

  enum label_tag_t { };
  typedef string::nstring_t<16, label_tag_t> label_t;

////////////////////////////////////////////////////////////////////////////////

  class message_handler_t {
  public:
    typedef messaging::message_t message_t;

    message_handler_t(const label_t& label) : label_(label) { }

    virtual ~message_handler_t() { }
    virtual bool_t handle(message_t&) = 0;

    const label_t label_;
  };

  class stats_t {
  public:
  };

////////////////////////////////////////////////////////////////////////////////

  class logic_impl_t;

  class logic_t {
  public:
    typedef trace::source_t                      tracepoint_t;
    typedef sandbox::multiple_of_1ms_t           multiple_of_1ms_t;
    typedef sandbox::fd_t                        fd_t;
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

    logic_t(const messenger_name_t&);
    logic_t(const messenger_name_t&, const messenger_create_params_t&);
    virtual ~logic_t();

    bool_t is_valid() const;

    tracepoint_t&     get_trace() const;
    messenger_name_t  get_name()  const;
    messenger_key_t   get_key ()  const;
    bool_t            get_params(messenger_params_t&) const;
    void_t            get_stats(stats_t&, bool_t reset = false);

////////////////////////////////////////////////////////////////////////////////

    bool_t post_message (const messenger_key_t&, message_t&) const;

////////////////////////////////////////////////////////////////////////////////

    bool_t update_visibility(messenger_visibility_t);
    bool_t update_alive_period(multiple_of_100ms_t);

////////////////////////////////////////////////////////////////////////////////

    bool_t start_message_timer(const messenger_timer_params_t&);
    bool_t stop_message_timer ();
    bool_t query_message_timer(messenger_timer_params_t&) const;

////////////////////////////////////////////////////////////////////////////////

    bool_t add_monitor(const messenger_name_t&,
                       messenger_prio_t = messenger_prio_t(0),
                       messenger_user_t = messenger_user_t());
    bool_t remove_monitor(const messenger_name_t&,
                          messenger_user_t* = 0);
    messenger_key_t is_monitored(const messenger_name_t&,
                                 messenger_user_t* = 0) const;
    bool_t get_monitored(messenger_memberkeylist_t&) const;

////////////////////////////////////////////////////////////////////////////////

    bool_t add_to_group(const password_t&, const messenger_name_t& group,
                        messenger_prio_t = messenger_prio_t(0),
                        messenger_user_t = messenger_user_t());
    bool_t remove_from_group(const password_t&, const messenger_name_t&,
                             messenger_user_t* = 0);
    bool_t is_in_group(const messenger_name_t&,
                       messenger_user_t* = 0) const;
    bool_t get_groups(messenger_memberlist_t&) const;

////////////////////////////////////////////////////////////////////////////////

    bool_t create_group(const password_t&, const messenger_name_t&,
                        messenger_visibility_t);
    bool_t destroy_group(const password_t&, const messenger_name_t&);
    bool_t fetch_group(const messenger_name_t&,
                       messenger_visibility_t&, messenger_memberlist_t* = 0);

////////////////////////////////////////////////////////////////////////////////

    bool_t add_another_to_group(const password_t&,
                                const messenger_name_t& name,
                                const messenger_name_t& group,
                                messenger_prio_t = messenger_prio_t(0),
                                messenger_user_t = messenger_user_t());
    bool_t remove_another_from_group(const password_t&,
                                     const messenger_name_t& name,
                                     const messenger_name_t& group,
                                     messenger_user_t* = 0);
    bool_t is_another_in_group(const messenger_name_t& name,
                               const messenger_name_t& group,
                               messenger_user_t* = 0);

////////////////////////////////////////////////////////////////////////////////

    multiple_of_1ms_t get_max_wait() const;
    void_t            set_max_wait(multiple_of_1ms_t);

////////////////////////////////////////////////////////////////////////////////

    bool_t   register_message_handler     (message_handler_t*);
    bool_t unregister_message_handler     (message_handler_t*);
    bool_t   is_message_handler_registered(message_handler_t*) const;

////////////////////////////////////////////////////////////////////////////////

    bool_t add_rxevent_monitor   (fd_t, const label_t&, messenger_user_t);
    bool_t remove_rxevent_monitor(fd_t, label_t* = 0, messenger_user_t* = 0);
    bool_t is_rxevent_monitored  (fd_t, label_t* = 0, messenger_user_t* = 0);

////////////////////////////////////////////////////////////////////////////////

   // timer services
   // name

////////////////////////////////////////////////////////////////////////////////

  protected:
    errorid_t errorid_;

  private:
    friend class sandbox_threadhandler_t;
    friend class logic_impl_t;

    errorid_t event_loop();

////////////////////////////////////////////////////////////////////////////////

    virtual void_t handle_wakeup(nanoseconds_t elapsed, bool_t maxwait) = 0;
    virtual void_t handle_notify(messenger_state_t,
                                 const messenger_name_t&,
                                 const messenger_key_t&,
                                 messenger_prio_t,
                                 messenger_user_t) = 0;
    virtual void_t handle_user(message_t&) = 0;
    virtual void_t handle_bad(message_t&)  = 0;
    virtual void_t handle_timeout(bool_t periodic,
                                  multiple_of_100ms_t,
                                  const messenger_key_t&,
                                  messenger_prio_t,
                                  messenger_user_t) = 0;
    virtual void_t handle_failed(errorid_t, message_t&) = 0;
    virtual void_t handle_rxevent(fd_t, const label_t&, messenger_user_t) = 0;

////////////////////////////////////////////////////////////////////////////////

    logic_t();
    logic_t(const logic_t&);
    logic_t& operator=(const logic_t&);

    logic_impl_t* impl_;
  };

////////////////////////////////////////////////////////////////////////////////

  class sandbox_threadhandler_t :
    private library::thread::detachedthread_t::handler_t {
    friend class sandbox_t;

    sandbox_threadhandler_t(logic_t*);
    ~sandbox_threadhandler_t();

    bool_t prepare();
    void_t process();

    logic_t* logic_;
  };

////////////////////////////////////////////////////////////////////////////////

  class sandbox_t {
    typedef library::thread::detachedthread_t thread_t;
    typedef sandbox_threadhandler_t           threadhandler_t;

    static threadhandler_t* make_threadhandler(logic_t*);

  public:
    typedef thread_t::name_t                     name_t;
    typedef messaging::messenger_name_t          messenger_name_t;
    typedef messaging::messenger_create_params_t messenger_create_params_t;

    template<typename L>
    static threadhandler_t* make() {
      return make_threadhandler(new L);
    }

    template<typename L>
    static threadhandler_t*
      make(const messenger_name_t& name,
           const messenger_create_params_t& params
             = messenger_create_params_t()) {
      return make_threadhandler(new L(name, params));
    }

    sandbox_t(threadhandler_t*);
    sandbox_t(const name_t&, threadhandler_t*);
    ~sandbox_t();

  private:
    friend class sandbox_threadhandler_t;
    messaging::messenger_key_t key_;
    thread_t                   thread_;
  };

////////////////////////////////////////////////////////////////////////////////
}
}

#endif
