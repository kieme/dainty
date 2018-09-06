#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <unistd.h>
#include "framework_library_types_nfreelist.h"
#include "framework_library_types_nlist.h"
#include "framework_library_types_nmap.h"
#include "framework_service_trace.h"
#include "framework_service_messaging.h"
#include "framework_sandbox.h"

namespace framework
{
  namespace sandbox
  {
    namespace
    {
      const messaging::message_domain_t domain(1);

      enum sandbox_message_t {
        sandbox_message_kill = 1
      };

      struct kill_message_t : messaging::message_t {
        kill_message_t() : messaging::message_t(0) {
          set(id_t(domain, user_t(sandbox_message_kill), version_t(1)), 0);
        }
      };
    }

    // get stats message

    // get info message - properties

////////////////////////////////////////////////////////////////////////////////

    class logic_impl_t {
    public:
      typedef logic_t::tracepoint_t              tracepoint_t;
      typedef logic_t::multiple_of_1ms_t         multiple_of_1ms_t;
      typedef logic_t::fd_t                      fd_t;
      typedef logic_t::label_t                   label_t;
      typedef logic_t::stats_t                   stats_t;
      typedef logic_t::nanoseconds_t             nanoseconds_t;
      typedef logic_t::message_handler_t         message_handler_t;
      typedef logic_t::multiple_of_100ms_t       multiple_of_100ms_t;
      typedef logic_t::password_t                password_t;
      typedef logic_t::messenger_timer_params_t  messenger_timer_params_t;
      typedef logic_t::messenger_key_t           messenger_key_t;
      typedef logic_t::messenger_visibility_t    messenger_visibility_t;
      typedef logic_t::messenger_state_t         messenger_state_t;
      typedef logic_t::messenger_prio_t          messenger_prio_t;
      typedef logic_t::messenger_user_t          messenger_user_t;
      typedef logic_t::messenger_name_t          messenger_name_t;
      typedef logic_t::messenger_create_params_t messenger_create_params_t;
      typedef logic_t::messenger_params_t        messenger_params_t;
      typedef logic_t::message_t                 message_t;
      typedef logic_t::messages_t                messages_t;
      typedef logic_t::message_id_t              message_id_t;
      typedef logic_t::messenger_memberlist_t    messenger_memberlist_t;
      typedef logic_t::messenger_memberkeylist_t messenger_memberkeylist_t;

      logic_impl_t(logic_t&, const messenger_name_t&);
      logic_impl_t(logic_t&, const messenger_name_t&,
                             const messenger_create_params_t&);
      ~logic_impl_t();

      bool_t is_valid() const;

      tracepoint_t&    get_trace() const;
      messenger_name_t get_name()  const;
      messenger_key_t  get_key ()  const;
      bool_t           get_params(messenger_params_t&) const;
      void_t           get_stats(stats_t&, bool_t reset);

      bool_t post_message (const messenger_key_t&, message_t&) const;

      bool_t update_visibility(messenger_visibility_t);
      bool_t update_alive_period(multiple_of_100ms_t);

      bool_t start_message_timer(const messenger_timer_params_t&);
      bool_t stop_message_timer ();
      bool_t query_message_timer(messenger_timer_params_t&) const;

      bool_t add_monitor(const messenger_name_t&, messenger_prio_t,
                         messenger_user_t);
      bool_t remove_monitor(const messenger_name_t&, messenger_user_t*);
      messenger_key_t is_monitored(const messenger_name_t&,
                                   messenger_user_t*) const;
      bool_t get_monitored(messenger_memberkeylist_t&) const;

      bool_t add_to_group(const password_t&, const messenger_name_t& group,
                          messenger_prio_t, messenger_user_t);
      bool_t remove_from_group(const password_t&, const messenger_name_t&,
                               messenger_user_t*);
      bool_t is_in_group(const messenger_name_t&, messenger_user_t*) const;
      bool_t get_groups(messenger_memberlist_t&) const;

      bool_t create_group(const password_t&, const messenger_name_t&,
                          messenger_visibility_t);
      bool_t destroy_group(const password_t&, const messenger_name_t&);
      bool_t fetch_group(const messenger_name_t&, messenger_visibility_t&,
                         messenger_memberlist_t*);

      bool_t add_another_to_group(const password_t&,
                                  const messenger_name_t& name,
                                  const messenger_name_t& group,
                                  messenger_prio_t, messenger_user_t);
      bool_t remove_another_from_group(const password_t&,
                                       const messenger_name_t& name,
                                       const messenger_name_t& group,
                                       messenger_user_t*);
      bool_t is_another_in_group(const messenger_name_t& name,
                                 const messenger_name_t& group,
                                 messenger_user_t*);

      multiple_of_1ms_t get_max_wait() const;
      void_t            set_max_wait(multiple_of_1ms_t);

      bool_t   register_message_handler     (message_handler_t*);
      bool_t unregister_message_handler     (message_handler_t*);
      bool_t   is_message_handler_registered(message_handler_t*) const;

      bool_t add_rxevent_monitor   (fd_t, const label_t&, messenger_user_t);
      bool_t remove_rxevent_monitor(fd_t, label_t*, messenger_user_t*);
      bool_t is_rxevent_monitored  (fd_t, label_t*, messenger_user_t*);

      errorid_t event_loop();

    private:
      logic_impl_t();
      logic_impl_t(const logic_impl_t&);
      logic_impl_t& operator=(const logic_impl_t&);

      enum { FD_EVENT_TIMER      = 1,
             FD_EVENT_MESSENGER  = 2,
             FD_EVENT_OTHERS     = 3 };

      errorid_t handle_messenger_events_();
      uint_t    is_message_handler_registered_(message_handler_t*) const;

      struct fdentry_t {
        fd_t             fd_;
        label_t          label_;
        messenger_user_t user_;
        fdentry_t() : fd_(-1), user_(0) { }
        fdentry_t(fd_t fd, const label_t& label, messenger_user_t user)
          : fd_(fd), label_(label), user_(user) { }
      };

      struct fdcompare_t {
        inline bool_t operator()(const fd_t& lh, const fd_t& rh) const {
          return get(lh) < get(rh);
         }
      };

      typedef types::nmap_t<64, fd_t, fdentry_t, fdcompare_t> fd_list_t;
      typedef types::nlist_t<message_handler_t*, 10>          msghdl_list_t;

      logic_t&                     logic_;
      errorid_t&                   errorid_;
      fd_t                         waitfd_;
      fd_t                         timerfd_;
      messenger_name_t             name_;
      multiple_of_1ms_t            maxwait_;
      time::timestamp_t            last_ts_;
      fd_list_t                    fd_list_;
      msghdl_list_t                msghdl_list_;
      mutable trace::sourceowner_t trace_;
      messaging::messenger_owner_t messenger_;
    };

////////////////////////////////////////////////////////////////////////////////

    logic_impl_t::logic_impl_t(logic_t& logic, const messenger_name_t& name)
      : logic_(logic), errorid_(logic_.errorid_),
        waitfd_(::epoll_create(100)),
        timerfd_(::timerfd_create(CLOCK_MONOTONIC, 0)),
        name_(name), maxwait_(0), last_ts_(0),
        trace_(create_source(errorid_, name_.c_str(),
                             trace::sourceparams_t(trace::level_debug))),
        messenger_(messaging::create_messenger(errorid_, name_)) {
      if (get(waitfd_) == -1)
        errorid_ = 5;
      if (get(timerfd_) == -1)
        errorid_ = 6;
      // add timer to epoll_ctl
      if (errorid_)
        SB_ERROR("instance not started");
      else
        SB_DEBUG("instance started with default messenger params");
    }

    logic_impl_t::logic_impl_t(logic_t& logic, const messenger_name_t& name,
                               const messenger_create_params_t& params)
      : logic_(logic), errorid_(logic_.errorid_),
        waitfd_(::epoll_create(100)),
        timerfd_(::timerfd_create(CLOCK_MONOTONIC, 0)),
        name_(name), maxwait_(0), last_ts_(0),
        trace_(create_source(errorid_, name_.c_str(),
                             trace::sourceparams_t(trace::level_debug))),
        messenger_(messaging::create_messenger(errorid_, name_, params)) {
      if (get(waitfd_) == -1)
        errorid_ = 5;
      if (get(timerfd_) == -1)
        errorid_ = 6;
      // add timer to epoll_ctl
      if (errorid_)
        SB_ERROR("instance not started");
      else
        SB_DEBUG("instance started with specified messenger params");
    }

    logic_impl_t::~logic_impl_t() {
      SB_DEBUG("instance has been stopped");
    }

    bool_t logic_impl_t::is_valid() const {
      return !errorid_ && messenger_;
    }

    logic_impl_t::tracepoint_t& logic_impl_t::get_trace() const {
      return *trace_;
    }

    logic_impl_t::messenger_name_t logic_impl_t::get_name() const {
      return name_;
    }

    logic_impl_t::messenger_key_t logic_impl_t::get_key() const {
      return messenger_.get_key();
    }

    void_t logic_impl_t::get_stats(stats_t& stats, bool_t reset) {
    }

    errorid_t logic_impl_t::handle_messenger_events_() {
      messages_t messages;
      SB_DEBUG("check & read messages");
      if (messenger_.check_message(messages)) {
        using types::uint_t;
        uint_t n = messages.size();
        SB_DEBUG("received %u messages", n);

        for (uint_t i = 0; i < n; ++i ) {
          message_t tmp(messages[i].release());

          bool_t handled = false;
          for (msghdl_list_t::offset_t i = 0; i < msghdl_list_.size(); ++i) {
            message_handler_t* handler = msghdl_list_[i];
            if (handler->handle(tmp)) {
              SB_DEBUG("message handled in message handler(%s)",
                       handler->label_.c_str());
              handled = true;
              break;
            }
          }

          if (!handled) {
            message_id_t id(read_id(tmp));
            switch (get(id.domain_)) {
              case 0: { // messaging
                switch (get(id.user_)) {
                  case messaging::message_type_notify: {
                    messaging::notify_message_t message(tmp.release());
                    messenger_state_t state;
                    messenger_name_t name;
                    messenger_key_t key(0);
                    messenger_prio_t prio(0);
                    messenger_user_t user;
                    if (message.get(state, name, key, prio, user)) {
                      SB_DEBUG("notification details");
                      logic_.handle_notify(state, name, key, prio, user);
                    } else {
                      SB_ERROR("bad notification message");
                      logic_.handle_bad(message);
                    }
                  } break;

                  case messaging::message_type_timeout: {
                    messaging::timeout_message_t message(tmp.release());
                    bool_t periodic = false;
                    multiple_of_100ms_t factor(0);
                    messenger_key_t key(0);
                    messenger_prio_t prio(0);
                    messenger_user_t user;
                    if (message.get(periodic, factor, key, prio, user)) {
                      SB_DEBUG("timeout details");
                      logic_.handle_timeout(periodic, factor, key, prio, user);
                    } else {
                      SB_ERROR("bad timeout message");
                      logic_.handle_bad(message);
                    }
                  } break;

                  case messaging::message_type_alive: {
                    SB_DEBUG("receive alive msg and return");
                    messenger_.post_message(messenger_key_t(0), tmp);
                  } break;

                  case messaging::message_type_failed: {
                    messaging::failed_message_t message(tmp.release());
                    messaging::failed_message_t::reason_t reason;
                    message_t send_message;
                    if (message.get(reason, send_message)) {
                      SB_DEBUG("send failed msg");
                      logic_.handle_failed(reason, send_message);
                    } else {
                      SB_ERROR("bad send failed msg");
                      logic_.handle_bad(message);
                    }
                  } break;

                  default: {
                    SB_ERROR("unexpected message received");
                  } break;
                }
              } break;

              case 1: { // sandbox
                switch (get(id.user_)) {
                  case sandbox_message_kill:
                    SB_DEBUG("sandbox is requested to be killed");
                    return 1;

                  default:
                    break;
                }
              } break;

              default: { // all unhandled lands here
                SB_DEBUG("user message received");
                logic_.handle_user(tmp);
                break;
              }
            }
          }
        }
      }
      return 0;
    }

    errorid_t logic_impl_t::event_loop() {
      SB_DEBUG("enter event_loop");
      if (is_valid()) {
        epoll_event event; // for 64 entries
        event.events   = EPOLLIN;
        event.data.u32 = FD_EVENT_MESSENGER;
        if (::epoll_ctl(get(waitfd_), EPOLL_CTL_ADD,
                        get(messenger_.get_fd()), &event) == 0) {
          last_ts_ = time::timestamp_t::now();
          while (is_valid()) {
            SB_DEBUG("enter wait(fd-%d)", get(waitfd_));
            epoll_event happend_events[80];
            int n = ::epoll_wait(get(waitfd_), &happend_events[0], 80,
                                  (get(maxwait_) ? get(maxwait_) : -1));
            if (n == -1) {
              SB_ERROR("epoll wait, error-%d", errno);
              continue;
            }

            time::timestamp_t tmp(last_ts_);
            last_ts_ = time::timestamp_t::now();
            nanoseconds_t elapsed = (last_ts_ - tmp).nanoseconds();

            SB_DEBUG("wakeup with %d events: elapsed %lld ns)", n, elapsed);
            logic_.handle_wakeup(elapsed, n==0);

            for (int i = 0; i < n; ++i) {
              epoll_event& event = happend_events[i];
              switch (event.data.u32) {
                case FD_EVENT_TIMER: {
                  //handle_timeout
                } break;

                case FD_EVENT_MESSENGER: {
                  errorid_t errorid = handle_messenger_events_();
                  switch (errorid) {
                    case 1: {
                      SB_DEBUG("thread will terminate now");
                      return 0;
                    } break;

                    default:
                      errorid_ = errorid;
                      break;
                  }
                } break;

                default: {
                  uint_t offset = event.data.u32 - FD_EVENT_OTHERS;
                  fd_list_t::keyvalue_t* kv = fd_list_.get(offset);
                  if (kv) {
                    logic_.handle_rxevent(kv->value_.fd_, kv->value_.label_,
                                          kv->value_.user_);
                  }

                } break;
              }
            }
          }
        } else
          errorid_ = 1;
      }

      if (is_valid())
        SB_ERROR("event_loop exit, error-%d", errno);
      else
        SB_DEBUG("event_loop exit");

      return errorid_;
    }

    bool_t logic_impl_t::get_params(messenger_params_t& params) const {
      SB_DEBUG("get params request");
      bool_t success = messenger_.get_params(params);
      if (!success)
        SB_ERROR("get params failed");
      return success;
    }

    bool_t logic_impl_t::update_visibility(messenger_visibility_t visibility) {
      SB_DEBUG("visibility change request to %s",
               to_string(visibility).c_str());
      bool_t success = messenger_.update_visibility(visibility);
      if (!success)
        SB_ERROR("visibility change request failed");
      return success;
    }

    bool_t logic_impl_t::update_alive_period(multiple_of_100ms_t factor) {
      SB_DEBUG("alive period change request to %ux100ms", get(factor));
      bool_t success = messenger_.update_alive_period(factor);
      if (!success)
        SB_ERROR("alive period change failed");
      return success;
    }

    bool_t logic_impl_t::post_message(const messenger_key_t& key,
                                      message_t& message) const {
      SB_DEBUG("post message(size=%u) request", read_len(message));
      bool_t success = messenger_.post_message(key, message);
      if (!success)
        SB_ERROR("post message failed");
      return success;
    }

    bool_t logic_impl_t::start_message_timer(
      const messenger_timer_params_t& params) {
      SB_DEBUG("start message timer request");
      bool_t success = messenger_.start_timer(params);
      if (!success)
        SB_ERROR("start message timer failed");
      return success;
    }

    bool_t logic_impl_t::stop_message_timer() {
      SB_DEBUG("stop message timer request");
      bool_t success = messenger_.stop_timer();
      if (!success)
        SB_ERROR("stop message timer failed");
      return success;
    }

    bool_t logic_impl_t::query_message_timer(
      messenger_timer_params_t& params) const {
      SB_DEBUG("request query message timer");
      bool_t success = messenger_.query_timer(params);
      if (!success)
        SB_ERROR("request query message timer");
      return success;
    }

    bool_t logic_impl_t::add_to_group(const password_t& password,
                                      const messenger_name_t& name,
                                      messenger_prio_t prio,
                                      messenger_user_t user) {
      SB_DEBUG("add messenger to group(%s) request", name.c_str());
      bool_t success = messenger_.add_to_group(password, name, prio, user);
      if (!success)
        SB_ERROR("add messenger to group failed");
      return success;
    }

    bool_t logic_impl_t::remove_from_group(const password_t& password,
                                           const messenger_name_t& name,
                                           messenger_user_t* user) {
      SB_DEBUG("remove messenger from group(%s) request", name.c_str());
      bool_t success = messenger_.remove_from_group(password, name, user);
      if (!success)
        SB_ERROR("remove messenger from group failed");
      return success;
    }

    bool_t logic_impl_t::is_in_group(const messenger_name_t& name,
                                     messenger_user_t* user) const {
      SB_DEBUG("is messenger in group(%s) request", name.c_str());
      bool_t success = messenger_.is_in_group(name, user);
      if (!success)
        SB_ERROR("is messenger in group failed");
      return success;
    }

    bool_t logic_impl_t::get_groups(messenger_memberlist_t& list) const {
      SB_DEBUG("get messenger groups request");
      bool_t success = messenger_.get_groups(list);
      if (!success)
        SB_ERROR("get messenger groups failed");
      return success;
    }

    bool_t logic_impl_t::add_monitor(const messenger_name_t& name,
                                     messenger_prio_t prio,
                                     messenger_user_t user) {
      SB_DEBUG("add messenger(%s) monitor request", name.c_str());
      bool_t success = messenger_.add_monitor(name, prio, user);
      if (!success)
        SB_ERROR("add messenger monitor failed");
      return success;
    }

    bool_t logic_impl_t::remove_monitor(const messenger_name_t& name,
                                        messenger_user_t* user) {
      SB_DEBUG("remove messenger(%s) monitor request", name.c_str());
      bool_t success = messenger_.remove_monitor(name, user);
      if (!success)
        SB_DEBUG("remove messenger monitor failed");
      return success;
    }

    logic_impl_t::messenger_key_t
      logic_impl_t::is_monitored(const messenger_name_t& name,
                                 messenger_user_t* user) const {
      SB_DEBUG("is messenger(%s) monitored request", name.c_str());
      messenger_key_t key(messenger_.is_monitored(name, user));
      if (!get(key))
        SB_DEBUG("messenger(%) is not monitored", name.c_str());
      return key;
    }

    bool_t logic_impl_t::get_monitored(messenger_memberkeylist_t& list) const {
      SB_DEBUG("get messenger monitored request");
      bool_t success = messenger_.get_monitored(list);
      if (!success)
        SB_DEBUG("get messenger monitored failed");
      return success;
    }

    bool_t logic_impl_t::create_group(const password_t& password,
                                      const messenger_name_t& group,
                                      messenger_visibility_t visibility) {
      SB_DEBUG("create group(%s) with visibility(%s) request",
               group.c_str(), to_string(visibility).c_str());
      bool_t success = messaging::create_group(password, group, visibility);
      if (!success)
        SB_DEBUG("create group request failed");
      return success;
    }

    bool_t logic_impl_t::destroy_group(const password_t& password,
                                       const messenger_name_t& group) {
      SB_DEBUG("destroy group(%s) request", group.c_str());
      bool_t success = messaging::destroy_group(password, group);
      if (!success)
        SB_DEBUG("destroy group request failed");
      return success;
    }

    bool_t logic_impl_t::fetch_group(const messenger_name_t& group,
                                     messenger_visibility_t& visibility,
                                     messenger_memberlist_t* list) {
      SB_DEBUG("fetch group(%s) request", group.c_str());
      bool_t success =  messaging::fetch_group(group, visibility, list);
      if (!success)
        SB_DEBUG("fetch group request failed");
      return success;
    }

    bool_t logic_impl_t::add_another_to_group(const password_t& password,
                                              const messenger_name_t& name,
                                              const messenger_name_t& group,
                                              messenger_prio_t prio,
                                              messenger_user_t user) {
      SB_DEBUG("add another(%s) to group(%s) request", name.c_str(),
               group.c_str());
      bool_t success = false;
      if (name != name_)
        success = messaging::add_messenger_to_group(password, name, group, prio,
                                                    user);
      if (!success)
        SB_DEBUG("add another to group request failed");
      return success;
    }

    bool_t logic_impl_t::remove_another_from_group(const password_t& password,
                                                   const messenger_name_t& name,
                                                   const messenger_name_t& group,
                                                   messenger_user_t* user) {
      SB_DEBUG("remove another(%s) from group(%s) request", name.c_str(),
               group.c_str());
      bool_t success = messaging::remove_messenger_from_group(password, name,
                                                              group, user);
      if (!success)
        SB_DEBUG("remove another from group request failed");
      return success;
    }

    bool_t logic_impl_t::is_another_in_group(const messenger_name_t& name,
                                             const messenger_name_t& group,
                                             messenger_user_t* user) {
      SB_DEBUG("is another(%s) in group(%s) request", name.c_str(),
               group.c_str());
      bool_t success = messaging::is_messenger_in_group(name, group, user);
      if (!success)
        SB_DEBUG("is another in group request failed");
      return success;
    }

    bool_t logic_impl_t::add_rxevent_monitor(fd_t fd, const label_t& label,
                                             messenger_user_t user) {
      SB_DEBUG("add_rxevent_monitor(fd=%u) request", get(fd));
      fd_list_t::result_t r(fd_list_.insert(fd));
      if (r) {
        r.keyvalue_->value_.fd_    = fd;
        r.keyvalue_->value_.label_ = label;
        r.keyvalue_->value_.user_  = user;

        epoll_event event;
        event.events   = EPOLLIN;
        event.data.u32 = FD_EVENT_OTHERS + r.idx_;
        if (::epoll_ctl(get(waitfd_), EPOLL_CTL_ADD, get(fd), &event) == 0)
          return true;

        fd_list_.erase(r.idx_);
      }
      SB_DEBUG("add_rxevent_monitor request failed");
      return false;
    }

    bool_t logic_impl_t::remove_rxevent_monitor(fd_t fd, label_t* label,
                                                messenger_user_t* user) {
      SB_DEBUG("remove_rxevent_monitor(fd=%u) request", get(fd));
      fd_list_t::result_t r(fd_list_.find(fd));
      if (r) {
        if (::epoll_ctl(get(waitfd_), EPOLL_CTL_DEL, get(fd), NULL) != 0)
          SB_DEBUG("remove_rxevent_monitor epoll request failed");
        if (label)
          *label = r.keyvalue_->value_.label_;
        if (user)
          *user  = r.keyvalue_->value_.user_;
        fd_list_.erase(r.idx_);
        return true;
      }
      SB_DEBUG("remove_rxevent_monitor request failed");
      return false;
    }

    bool_t logic_impl_t::is_rxevent_monitored(fd_t fd, label_t* label,
                                              messenger_user_t* user) {
      fd_list_t::result_t r(fd_list_.find(fd));
      if (r) {
        if (label)
          *label = r.keyvalue_->value_.label_;
        if (user)
          *user  = r.keyvalue_->value_.user_;
      }
      return r;
    }

    bool_t logic_impl_t::register_message_handler(message_handler_t* handler) {
      SB_DEBUG("register message handler(%s) request",
               handler->label_.c_str());
      bool_t success = is_message_handler_registered_(handler)
                         == msghdl_list_.size() &&
                       msghdl_list_.push_back(handler);
      if (!success)
        SB_DEBUG("register message handler request failed");
      return success;
    }

    bool_t logic_impl_t::unregister_message_handler(
      message_handler_t* handler) {
      SB_DEBUG("unregister message handler(%s) request",
               handler->label_.c_str());
      bool_t success = msghdl_list_.erase(is_message_handler_registered_(
                                            handler));
      if (!success)
        SB_DEBUG("unregister message handler request failed");
      return success;
    }

    bool_t logic_impl_t::is_message_handler_registered(
      message_handler_t* handler) const {
      return is_message_handler_registered_(handler) < msghdl_list_.size();
    }

    uint_t logic_impl_t::is_message_handler_registered_(
      message_handler_t* handler) const {
      uint_t i = 0, max = msghdl_list_.size();
      for (; i < max; ++i)
        if (msghdl_list_[i] == handler)
          break;
      return i;
    }

    multiple_of_1ms_t logic_impl_t::get_max_wait() const {
      return maxwait_;
    }

    void_t logic_impl_t::set_max_wait(multiple_of_1ms_t time) {
      SB_DEBUG("set max wait time to %u", get(time));
      maxwait_ = time;
    }

////////////////////////////////////////////////////////////////////////////////

    logic_t::logic_t(const messenger_name_t& name)
      : errorid_(0), impl_(new logic_impl_t(*this, name)) {
    }

    logic_t::logic_t(const messenger_name_t& name,
                     const messenger_create_params_t& params)
      : errorid_(0), impl_(new logic_impl_t(*this, name, params)) {
    }

    logic_t::~logic_t() {
      delete impl_;
    }

    bool_t logic_t::is_valid() const {
      return impl_->is_valid();
    }

    logic_t::tracepoint_t& logic_t::get_trace() const {
      return impl_->get_trace();
    }

    logic_t::messenger_name_t logic_t::get_name() const {
      return impl_->get_name();
    }

    logic_t::messenger_key_t logic_t::get_key () const {
      return impl_->get_key();
    }

    bool_t logic_t::get_params(messenger_params_t& params) const {
      return impl_->get_params(params);
    }

    void_t logic_t::get_stats(stats_t& stats, bool_t reset) {
      impl_->get_stats(stats, reset);
    }

    bool_t logic_t::post_message(const messenger_key_t& key,
                                 message_t& message) const {
      return impl_->post_message(key, message);
    }

    bool_t logic_t::update_visibility(messenger_visibility_t visibility) {
      return impl_->update_visibility(visibility);
    }

    bool_t logic_t::update_alive_period(multiple_of_100ms_t factor) {
      return impl_->update_alive_period(factor);
    }

    bool_t logic_t::start_message_timer(
      const messenger_timer_params_t& params) {
      return impl_->start_message_timer(params);
    }

    bool_t logic_t::stop_message_timer() {
      return impl_->stop_message_timer();
    }

    bool_t logic_t::query_message_timer(
      messenger_timer_params_t& params) const {
      return impl_->query_message_timer(params);
    }

    bool_t logic_t::add_monitor(const messenger_name_t& name,
                                messenger_prio_t prio,
                                messenger_user_t user) {
      return impl_->add_monitor(name, prio, user);
    }

    bool_t logic_t::remove_monitor(const messenger_name_t& name,
                                   messenger_user_t* user) {
      return impl_->remove_monitor(name, user);
    }

    logic_t::messenger_key_t logic_t::is_monitored(
      const messenger_name_t& name, messenger_user_t* user) const {
      return impl_->is_monitored(name, user);
    }

    bool_t logic_t::get_monitored(messenger_memberkeylist_t& list) const {
      return impl_->get_monitored(list);
    }

    bool_t logic_t::add_to_group(const password_t& password,
                                 const messenger_name_t& group,
                                 messenger_prio_t prio,
                                 messenger_user_t user) {
      return impl_->add_to_group(password, group, prio, user);
    }

    bool_t logic_t::remove_from_group(const password_t& password,
                                      const messenger_name_t& name,
                                      messenger_user_t* user) {
      return impl_->remove_from_group(password, name, user);
    }

    bool_t logic_t::is_in_group(const messenger_name_t& name,
                                messenger_user_t* user) const {
      return impl_->is_in_group(name, user);
    }

    bool_t logic_t::get_groups(messenger_memberlist_t& list) const {
      return impl_->get_groups(list);
    }

    bool_t logic_t::create_group(const password_t& password,
                                 const messenger_name_t& name,
                                 messenger_visibility_t visibility) {
      return impl_->create_group(password, name, visibility);
    }

    bool_t logic_t::destroy_group(const password_t& password,
                                  const messenger_name_t& name) {
      return impl_->destroy_group(password, name);
    }

    bool_t logic_t::fetch_group(const messenger_name_t& name,
                                messenger_visibility_t& visibility,
                                messenger_memberlist_t* list) {
      return impl_->fetch_group(name, visibility, list);
    }

    bool_t logic_t::add_another_to_group(const password_t& password,
                                         const messenger_name_t& name,
                                         const messenger_name_t& group,
                                         messenger_prio_t prio,
                                         messenger_user_t user) {
      return impl_->add_another_to_group(password, name, group, prio, user);
    }

    bool_t logic_t::remove_another_from_group(const password_t& password,
                                              const messenger_name_t& name,
                                              const messenger_name_t& group,
                                              messenger_user_t* user) {
      return impl_->remove_another_from_group(password, name, group, user);
    }

    bool_t logic_t::is_another_in_group(const messenger_name_t& name,
                                        const messenger_name_t& group,
                                        messenger_user_t* user) {
      return impl_->is_another_in_group(name, group, user);
    }

    multiple_of_1ms_t logic_t::get_max_wait() const {
      return impl_->get_max_wait();
    }

    void_t logic_t::set_max_wait(multiple_of_1ms_t factor) {
      return impl_->set_max_wait(factor);
    }

    bool_t logic_t::register_message_handler(message_handler_t* handler) {
      return impl_->register_message_handler(handler);
    }

    bool_t logic_t::unregister_message_handler(message_handler_t* handler) {
      return impl_->unregister_message_handler(handler);
    }

    bool_t logic_t::is_message_handler_registered(
      message_handler_t* handler) const {
      return impl_->is_message_handler_registered(handler);
    }

    bool_t logic_t::add_rxevent_monitor(fd_t fd, const label_t& label,
                                        messenger_user_t user) {
      return impl_->add_rxevent_monitor(fd, label, user);
    }

    bool_t logic_t::remove_rxevent_monitor(fd_t fd, label_t* label,
                                           messenger_user_t* user) {
      return impl_->remove_rxevent_monitor(fd, label, user);
    }

    bool_t logic_t::is_rxevent_monitored(fd_t fd, label_t* label,
                                         messenger_user_t* user) {
      return impl_->is_rxevent_monitored(fd, label, user);
    }

    errorid_t logic_t::event_loop() {
      return impl_->event_loop();
    }

////////////////////////////////////////////////////////////////////////////////

    sandbox_threadhandler_t::sandbox_threadhandler_t(logic_t* logic)
      : logic_(logic) {
    }

    sandbox_threadhandler_t::~sandbox_threadhandler_t() {
      delete logic_;
    }

    bool_t sandbox_threadhandler_t::prepare() {
      return logic_ && logic_->is_valid();
    }

    void_t sandbox_threadhandler_t::process() {
      logic_->event_loop();
    }

    sandbox_threadhandler_t* sandbox_t::make_threadhandler(logic_t* logic) {
      return new sandbox_threadhandler_t(logic); // unique_ptr
    }

////////////////////////////////////////////////////////////////////////////////

    sandbox_t::sandbox_t(threadhandler_t* handler)
      : key_   (handler->logic_->get_key()),
        thread_(handler->logic_->get_name().c_str(), handler, true) {
    }

    sandbox_t::sandbox_t(const name_t& name, threadhandler_t* handler)
      : key_   (handler->logic_->get_key()),
        thread_(name, handler, true) {
    }

    sandbox_t::~sandbox_t() {
      kill_message_t message;
      messaging::post_message(key_, message);
    }

////////////////////////////////////////////////////////////////////////////////
  }
}
