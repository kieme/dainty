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

#include <stdio.h>
#include <syslog.h>
#include <memory>
#include <map>
#include <algorithm>
#include <limits>
#include "dainty_container_ptr.h"
#include "dainty_mt_event_dispatcher.h"
#include "dainty_mt_waitable_chained_queue.h"
#include "dainty_mt_command.h"
#include "dainty_mt_thread.h"
#include "dainty_os_threading.h"
#include "dainty_os_clock.h"
#include "dainty_tracing.h"

using namespace dainty::container;
using namespace dainty::named;
using namespace dainty::mt;
using namespace dainty::os;
using namespace dainty::tracing::tracer;

using string::FMT;
using dainty::container::any::t_any;
using dainty::os::threading::t_mutex_lock;
using dainty::os::clock::t_time;
using dainty::mt::thread::t_thread;
using dainty::mt::event_dispatcher::t_dispatcher;
using dainty::mt::event_dispatcher::t_event_logic;
using dainty::mt::event_dispatcher::t_action;
using dainty::mt::event_dispatcher::CONTINUE;
using dainty::mt::event_dispatcher::QUIT_EVENT_LOOP;
using dainty::mt::event_dispatcher::RD;

using t_thd_err       = t_thread::t_logic::t_err;
using t_cmd_err       = command::t_processor::t_logic::t_err;
using t_cmd_client    = command::t_client;
using t_cmd_processor = command::t_processor;
using t_cmd           = command::t_command;
using t_any_user      = any::t_user;
using t_que_chain     = waitable_chained_queue::t_chain;
using t_que_client    = waitable_chained_queue::t_client;
using t_que_processor = waitable_chained_queue::t_processor;

namespace dainty
{
namespace tracing
{
namespace tracer
{
  inline
  t_tracer mk_(R_id id, R_name name) {
    return {id, name};
  }

  inline
  t_impl_id_ get_(R_id id) {
    return id.id_;
  }

  inline
  t_void set_(t_id& id, t_id::t_seq seq, t_impl_id_ impl_id) {
    id.seq_ = seq;
    id.id_  = impl_id;
  }
}

///////////////////////////////////////////////////////////////////////////////

  struct t_item_ { // add point name
    t_id          id;
    t_level       level;
    t_text        text;
    t_time        time;
    t_tracer_name point;
  };

  using R_item_ = t_prefix<t_item_>::R_;

  inline
  t_bool operator==(R_item_ lh, R_item_ rh) {
    return lh.text == rh.text && lh.point == rh.point;
  }

///////////////////////////////////////////////////////////////////////////////

  enum  t_date_tag_ {};
  using t_date = string::t_string<t_date_tag_, 24>;
  using R_date = t_prefix<t_date>::R_;

  inline t_date make_date(t_time_mode time_mode, const t_time& time) {
    static t_time prev;
    t_date date;
    switch (time_mode) {
      case NS:
        date.assign(FMT, "%lld.%.9ld", (long long)to_(time).tv_sec,
                                        to_(time).tv_nsec);
        break;
      case NS_DIFF: {
        if (to_(prev).tv_sec || to_(prev).tv_nsec) {
          t_time diff(time);
          diff -= prev;
          date.assign(FMT, "%lld.%.9ld", (long long)to_(diff).tv_sec,
                                          to_(diff).tv_nsec);
        } else
          date.assign(FMT, "%lld.%.9ld", 0LL, 0L);
      } break;
      case DATE:
        date += "date:...."; //XXX
        break;
    }
    prev = time;
    return date;
  }

///////////////////////////////////////////////////////////////////////////////

  enum  t_line_tag_ { };
  using t_line = string::t_string<t_line_tag_, 120>;

  t_line make_line(t_n cnt, R_tracer_name& name, R_tracer_name point,
                   R_levelname level, R_text text) {
    t_line line;
    line += name;
    line += " ";
    if (point == name)
      line += "(*) ";
    else {
      line += "(";
      line += point;
      line += ") ";
    }
    if (get(cnt) != 1)
      line.append(FMT, "cnt=%u ", get(cnt));
    line += text;
    return line;
  }
  using R_line = t_prefix<t_line>::R_;

///////////////////////////////////////////////////////////////////////////////

  class t_output_impl_ {
  public:
    virtual ~t_output_impl_() { };
    virtual t_void notify(R_observer_name, t_level, R_date, R_line) = 0;
  };

  using p_output_impl_     = t_prefix<t_output_impl_>::p_;
  using t_output_impls_    = std::vector<p_output_impl_>;
  using p_output_impls_    = t_prefix<t_output_impls_>::p_;
  using t_output_impl_ptr_ = ptr::t_passable_ptr<t_output_impl_>;

///////////////////////////////////////////////////////////////////////////////

  class t_logger_output_ : public t_output_impl_ {
  public:
    virtual t_void notify(R_observer_name name, t_level level, R_date,
                          R_line line) override {
      P_cstr_ who = get(name.c_str()), msg = get(line.c_str());
      switch (level) {
        case EMERG:    ::syslog(LOG_EMERG,   "[%s] %s", who, msg); break;
        case ALERT:    ::syslog(LOG_ALERT,   "[%s] %s", who, msg); break;
        case CRITICAL: ::syslog(LOG_CRIT,    "[%s] %s", who, msg); break;
        case ERROR:    ::syslog(LOG_ERR,     "[%s] %s", who, msg); break;
        case WARNING:  ::syslog(LOG_WARNING, "[%s] %s", who, msg); break;
        case NOTICE:   ::syslog(LOG_NOTICE,  "[%s] %s", who, msg); break;
        case INFO:     ::syslog(LOG_INFO,    "[%s] %s", who, msg); break;
        case DEBUG:    ::syslog(LOG_DEBUG,   "[%s] %s", who, msg); break;
      }
    }
  };

///////////////////////////////////////////////////////////////////////////////

  class t_ftrace_output_ : public t_output_impl_ {
  public:
    t_ftrace_output_(t_err err) {
    }

    ~t_ftrace_output_() {
    }

    virtual t_void notify(R_observer_name name, t_level level, R_date,
                          R_line line) override {
    }
  private:
  };

///////////////////////////////////////////////////////////////////////////////

  struct t_update_params_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 1;
    R_params params;

    inline
    t_update_params_cmd_(R_params _params) : t_cmd{cmd_id}, params(_params) {
    };
  };
  using r_update_params_cmd_ = t_prefix<t_update_params_cmd_>::r_;

  struct t_fetch_params_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 2;
    r_params params;

    inline
    t_fetch_params_cmd_(r_params _params) : t_cmd{cmd_id}, params(_params) {
    };
  };
  using r_fetch_params_cmd_ = t_prefix<t_fetch_params_cmd_>::r_;

  struct t_get_point_name_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 3;
    R_id           id;
    t_tracer_name& name;

    inline
    t_get_point_name_cmd_(R_id _id, t_tracer_name& _name)
      : t_cmd{cmd_id}, id(_id), name(_name) {
    };
  };
  using r_get_point_name_cmd_ = t_prefix<t_get_point_name_cmd_>::r_;

  struct t_get_point_level_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 4;
    R_id     id;
    t_level& level;

    inline
    t_get_point_level_cmd_(R_id _id, t_level& _level)
      : t_cmd{cmd_id}, id(_id), level(_level) {
    };
  };
  using r_get_point_level_cmd_ = t_prefix<t_get_point_level_cmd_>::r_;

  struct t_make_tracer_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 5;
    R_tracer_name   name;
    R_tracer_params params;
    t_id&           id;

    inline
    t_make_tracer_cmd_(R_tracer_name _name, R_tracer_params _params, t_id& _id)
      : t_cmd{cmd_id}, name(_name), params(_params), id(_id) {
    };
  };
  using r_make_tracer_cmd_ = t_prefix<t_make_tracer_cmd_>::r_;

  struct t_update_tracer_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 6;
    R_wildcard_name name;
    t_level         level;
    t_bool&         updated;

    inline
    t_update_tracer_cmd_(R_wildcard_name _name, t_level _level,
                         t_bool& _updated)
      : t_cmd{cmd_id}, name(_name), level(_level), updated(_updated) {
    };
  };
  using r_update_tracer_cmd_ = t_prefix<t_update_tracer_cmd_>::r_;

  struct t_update_tracer_params_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 7;
    R_tracer_name   name;
    R_tracer_params params;

    inline
    t_update_tracer_params_cmd_(R_tracer_name _name, R_tracer_params _params)
      : t_cmd{cmd_id}, name(_name), params(_params) {
    };
  };
  using r_update_tracer_params_cmd_ =
    t_prefix<t_update_tracer_params_cmd_>::r_;

  struct t_fetch_tracer_params_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 8;
    R_tracer_name    name;
    t_tracer_params& params;
    t_bool&          found;

    inline
    t_fetch_tracer_params_cmd_(R_tracer_name _name, t_tracer_params& _params,
                               t_bool& _found)
      : t_cmd{cmd_id}, name(_name), params(_params), found(_found) {
    };
  };
  using r_fetch_tracer_params_cmd_ =
    t_prefix<t_fetch_tracer_params_cmd_>::r_;

  struct t_fetch_tracer_info_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 9;
    R_tracer_name  name;
    r_tracer_info& info;
    t_bool         clear_stats;
    t_bool&        found;

    inline
    t_fetch_tracer_info_cmd_(R_tracer_name _name, r_tracer_info& _info,
                             t_bool _clear_stats, t_bool _found)
      : t_cmd{cmd_id}, name(_name), info(_info), clear_stats(_clear_stats),
        found(_found) {
    };
  };
  using r_fetch_tracer_info_cmd_ =
    t_prefix<t_fetch_tracer_info_cmd_>::r_;

  struct t_fetch_tracers_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 10;
    t_tracer_infos& infos;
    t_bool          clear_stats;
    t_bool&         found;

    inline
    t_fetch_tracers_cmd_(t_tracer_infos& _infos, t_bool _clear_stats,
                         t_bool& _found)
      : t_cmd{cmd_id}, infos(_infos), clear_stats(_clear_stats),
        found(_found) {
    };
  };
  using r_fetch_tracers_cmd_ = t_prefix<t_fetch_tracers_cmd_>::r_;

  struct t_create_observer_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 11;
    R_observer_name   name;
    R_observer_params params;

    inline
    t_create_observer_cmd_(R_observer_name _name, R_observer_params _params)
      : t_cmd{cmd_id}, name(_name), params(_params) {
    };
  };
  using r_create_observer_cmd_ = t_prefix<t_create_observer_cmd_>::r_;

  struct t_destroy_observer_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 12;
    R_observer_name name;

    inline
    t_destroy_observer_cmd_(R_observer_name _name)
      : t_cmd{cmd_id}, name(_name) {
    };
  };
  using r_destroy_observer_cmd_ = t_prefix<t_destroy_observer_cmd_>::r_;

  struct t_update_observer_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 13;
    R_observer_name   name;
    R_observer_params params;

    inline
    t_update_observer_cmd_(R_observer_name _name, R_observer_params _params)
      : t_cmd{cmd_id}, name(_name), params(_params) {
    };
  };
  using r_update_observer_cmd_ = t_prefix<t_update_observer_cmd_>::r_;

  struct t_fetch_observer_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 14;
    R_observer_name   name;
    r_observer_params params;
    t_bool&           found;

    inline
    t_fetch_observer_cmd_(R_observer_name _name,
                          r_observer_params _params,
                          t_bool& _found)
      : t_cmd{cmd_id}, name(_name), params(_params), found(_found) {
    };
  };
  using r_fetch_observer_cmd_ = t_prefix<t_fetch_observer_cmd_>::r_;

  struct t_fetch_observer_info_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 15;
    R_observer_name  name;
    r_observer_info  info;
    t_bool           clear_stats;
    t_bool&          found;

    inline
    t_fetch_observer_info_cmd_(R_observer_name _name,
                               r_observer_info _info,
                               t_bool _clear_stats,
                               t_bool& _found)
      : t_cmd{cmd_id}, name(_name), info(_info), clear_stats(_clear_stats),
        found(_found) {
    };
  };
  using r_fetch_observer_info_cmd_ =
    t_prefix<t_fetch_observer_info_cmd_>::r_;

  struct t_fetch_observers_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 16;
    t_observer_infos& infos;
    t_bool            clear_stats;
    t_bool&           found;

    inline
    t_fetch_observers_cmd_(t_observer_infos& _infos, t_bool _clear_stats,
                           t_bool& _found)
      : t_cmd{cmd_id}, infos(_infos), clear_stats(_clear_stats),
        found(_found) {
    };
  };
  using r_fetch_observers_cmd_ = t_prefix<t_fetch_observers_cmd_>::r_;

  struct t_bind_tracer_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 17;
    R_observer_name name;
    R_tracer_name   tracer_name;
    t_bool&         found;

    inline
    t_bind_tracer_cmd_(R_observer_name _name, R_tracer_name _tracer_name,
                       t_bool& _found)
      : t_cmd{cmd_id}, name(_name), tracer_name(_tracer_name), found(_found) {
    };
  };
  using r_bind_tracer_cmd_ = t_prefix<t_bind_tracer_cmd_>::r_;

  struct t_bind_tracers_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 18;
    R_observer_name name;
    R_wildcard_name wildcard_name;
    t_bool&         found;

    inline
    t_bind_tracers_cmd_(R_observer_name _name, R_wildcard_name _wildcard_name,
                        t_bool& _found)
      : t_cmd{cmd_id}, name(_name), wildcard_name(_wildcard_name),
        found(_found) {
    };
  };
  using r_bind_tracers_cmd_ = t_prefix<t_bind_tracers_cmd_>::r_;

  struct t_unbind_tracers_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 19;
    R_observer_name name;
    R_wildcard_name wildcard_name;
    t_bool&         found;

    inline
    t_unbind_tracers_cmd_(R_observer_name _name,
                          R_wildcard_name _wildcard_name, t_bool& _found)
      : t_cmd{cmd_id}, name(_name), wildcard_name(_wildcard_name),
        found(_found) {
    };
  };
  using r_unbind_tracers_cmd_ = t_prefix<t_unbind_tracers_cmd_>::r_;

  struct t_is_tracer_bound_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 20;
    R_observer_name name;
    R_tracer_name   tracer_name;
    t_bool&         found;

    inline
    t_is_tracer_bound_cmd_(R_observer_name _name, R_tracer_name _tracer_name,
                           t_bool& _found)
      : t_cmd{cmd_id}, name(_name), tracer_name(_tracer_name),
        found(_found) {
    };
  };
  using r_is_tracer_bound_cmd_ = t_prefix<t_is_tracer_bound_cmd_>::r_;

  struct t_fetch_bound_tracers_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 21;
    R_observer_name name;
    r_tracer_names  tracer_names;
    t_bool&         found;

    inline
    t_fetch_bound_tracers_cmd_(R_observer_name _name,
                               r_tracer_names _tracer_names, t_bool& _found)
      : t_cmd{cmd_id}, name(_name), tracer_names(_tracer_names),
        found(_found) {
    };
  };
  using r_fetch_bound_tracers_cmd_ =
    t_prefix<t_fetch_bound_tracers_cmd_>::r_;

  struct t_fetch_bound_observers_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 22;
    R_tracer_name    name;
    r_observer_names observer_names;
    t_bool&          found;

    inline
    t_fetch_bound_observers_cmd_(R_tracer_name _name,
                                 r_observer_names _observer_names,
                                 t_bool& _found)
      : t_cmd{cmd_id}, name(_name), observer_names(_observer_names),
        found(_found) {
    };
  };
  using r_fetch_bound_observers_cmd_ =
    t_prefix<t_fetch_bound_observers_cmd_>::r_;

  struct t_destroy_tracer_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 23;
    R_id    id;
    t_bool& done;

    inline
    t_destroy_tracer_cmd_(R_id _id, t_bool& _done)
      : t_cmd{cmd_id}, id(_id), done(_done) {
    };
  };
  using r_destroy_tracer_cmd_ = t_prefix<t_destroy_tracer_cmd_>::r_;

  struct t_do_chain_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 24;
    t_que_chain& chain;

    inline
    t_do_chain_cmd_(t_que_chain& _chain)
      : t_cmd{cmd_id}, chain(_chain) {
    };
  };
  using r_do_chain_cmd_ = t_prefix<t_do_chain_cmd_>::r_;

  struct t_clean_death_cmd_ : t_cmd {
    constexpr static command::t_id cmd_id = 25;

    inline
    t_clean_death_cmd_() : t_cmd{cmd_id} {
    };
  };
  using r_clean_death_cmd_ = t_prefix<t_clean_death_cmd_>::r_;

///////////////////////////////////////////////////////////////////////////////

  class t_data_ { // t_err should be r_err
  public:
    using r_err = t_prefix<tracing::t_err>::r_;

    struct t_observer_data_ {
      t_observer_info      info;
      t_output_impl_ptr_ impl;
    };

    using p_observer_data_  = t_prefix<t_observer_data_>::p_;
    using t_observers_data_ = std::vector<p_observer_data_>;
    using p_observers_data_ = t_prefix<t_observers_data_>::p_;

    struct t_tracer_data_ {
      t_tracer_info     info;
      p_observers_data_ observers = nullptr;
      t_tracer_id       id;
    };

    using p_tracer_data_ = t_prefix<t_tracer_data_>::p_;

    struct t_tracer_lk_data_ {
      p_tracer_data_    data = nullptr;
      t_observers_data_ observers;
    };

    using t_freelist_        = freelist::t_freelist<t_tracer_data_>;
    using t_tracers_         = std::map<t_tracer_name, t_tracer_lk_data_>;
    using t_observers_       = std::map<t_observer_name, t_observer_data_>;
    using t_tracers_value_   = t_tracers_::value_type;
    using t_observers_value_ = t_observers_::value_type;

///////////////////////////////////////////////////////////////////////////////

    t_params params;

    t_data_(R_params _params)
      : params{_params}, freelist_{params.max_tracers} {
    }

    t_tracer_id add_tracer(r_err err, R_tracer_name name,
                           R_tracer_params params) {
      t_tracer_id id;
      if (!freelist_.is_full()) {
        static t_tracer_id::t_seq seq = 0;
        auto tracer = tracers_.insert(t_tracers_value_(name,
                                                       t_tracer_lk_data_()));
        if (tracer.second) {
          auto result = freelist_.insert();
          set_(result.ptr->id, ++seq, result.id);
          result.ptr->info.name   = name;
          result.ptr->info.params = params;
          result.ptr->observers   = &tracer.first->second.observers;
          tracer.first->second.data = result.ptr;
          id = result.ptr->id;
          // bound to all log books with XXX-0
        } else if (tracer.first != tracers_.end()) {
          if (!tracer.first->second.data) {
            auto result = freelist_.insert();
            set_(result.ptr->id, ++seq, result.id);
            result.ptr->info.name   = name;
            result.ptr->info.params = params;
            result.ptr->observers   = &tracer.first->second.observers;
            tracer.first->second.data = result.ptr;
            id = result.ptr->id;
            // bound to all log books with XXX-0
          } else
            err = E_XXX;
        } else
          err = E_XXX;
        if (seq == std::numeric_limits<t_tracer_id::t_seq>::max())
          seq = 0;
      } else
        err = E_XXX;
      return id;
    }

    t_validity update_tracer(r_err err, R_tracer_name name,
                             R_tracer_params params) {
      auto tracer = is_tracer(name);
      if (tracer) {
        tracer->info.params = params;
        return VALID;
      } else
        err = E_XXX;
      return INVALID;
    }

    t_bool update_tracer(r_err err, R_wildcard_name wildcard_name,
                         t_level level) {
      t_bool found = false;
      for (auto&& tracer : tracers_) {
        if (tracer.first.match(wildcard_name)) {
          if (tracer.second.data) {
            tracer.second.data->info.params.level = level;
            found = true;
          }
        }
      }
      return found;
    }

    t_validity del_tracer(t_tracer_id id) {
      auto tracer = is_tracer(id);
      if (tracer) {
        auto tracer_lk = tracers_.find(tracer->info.name);
        if (tracer_lk != tracers_.end()) {
          tracers_.erase(tracer_lk);
          freelist_.erase(get_(id));
          return VALID;
        }
      }
      return INVALID;
    }

    p_tracer_data_ is_tracer(R_tracer_name name) {
      auto tracer = tracers_.find(name);
      if (tracer != tracers_.end())
        return tracer->second.data;
      return nullptr;
    }

    p_tracer_data_ is_tracer(t_tracer_id id) {
      auto tracer = freelist_.get(get_(id));
      if (tracer && tracer->id == id) {
        return tracer;
      }
      return nullptr;
    }

    p_observer_data_ add_observer(r_err err, R_observer_name name,
                                  R_observer_params params,
                                  t_output_impl_ptr_ impl) {
      auto entry = observers_.insert(t_observers_value_(name,
                                                        t_observer_data_()));
      if (entry.second) {
        entry.first->second.info.name   = name;
        entry.first->second.info.params = params; //XXX impl must be added
        entry.first->second.impl        = std::move(impl);
        return &entry.first->second;
      }
      return nullptr;
    }

    t_validity update_observer(r_err err, R_observer_name name,
                               R_observer_params params,
                               t_output_impl_ptr_ impl) {
      auto entry = observers_.find(name);
      if (entry != std::cend(observers_)) {
        entry->second.info.name   = name;
        entry->second.info.params = params;
        entry->second.impl        = std::move(impl);
        // bound to all - XXX-0
        return VALID;
      } else
        err = E_XXX;
      return INVALID;
    }

    t_output_impl_ptr_ del_observer(r_err err, R_observer_name name) {
      t_output_impl_ptr_ impl;
      auto entry = observers_.find(name);
      if (entry != std::cend(observers_)) {
        auto observer = &entry->second;
        for (auto&& tracer_name : entry->second.info.tracers) {
          auto tracer = tracers_.find(tracer_name);
          if (tracer != std::cend(tracers_)) {
            tracer->second.observers.erase(
              std::find(std::begin(tracer->second.observers),
                        std::end  (tracer->second.observers), observer));
            if (tracer->second.observers.empty() && !tracer->second.data)
              tracers_.erase(tracer);
          }
        }
        impl = std::move(entry->second.impl);
        observers_.erase(entry);
      } else
        err = E_XXX;
      return impl;
    }

    p_observer_data_ is_observer(R_observer_name name) {
      auto entry = observers_.find(name);
      if (entry != std::cend(observers_))
        return &entry->second;
      return nullptr;
    }

    t_bool bind_tracer(r_err err, R_observer_name name,
                       R_tracer_name tracer_name) {
      auto observer = is_observer(name);
      if (observer) {
         auto tracer = is_tracer(tracer_name);
         if (tracer) {
           if (std::find(std::cbegin(*tracer->observers),
                         std::cend  (*tracer->observers), observer) ==
                std::end(*tracer->observers)) {
             tracer->observers->push_back(observer);
             return true;
           }
         } else {
           auto tracer = tracers_.insert(t_tracers_value_(tracer_name,
                                                          t_tracer_lk_data_()));
           if (tracer.second) {
             tracer.first->second.observers.push_back(observer);
             return true;
           } else
             err = E_XXX;
         }
      } else
        err = E_XXX;
      return false;
    }

    t_bool is_tracer_bound(R_observer_name name, R_tracer_name tracer_name) {
      auto observer = is_observer(name);
      if (observer) {
         auto tracer = is_tracer(tracer_name);
         if (tracer)
           return std::find(std::cbegin(*tracer->observers),
                            std::cend  (*tracer->observers), observer) !=
                    std::end(*tracer->observers);
      }
      return false;
    }

    t_bool bind_tracers(r_err err, R_observer_name name,
                        R_wildcard_name wildcard_name) {
      auto observer = is_observer(name);
      if (observer) {
        t_bool bound = false;
        for (auto&& tracer : tracers_) {
          if (tracer.first.match(wildcard_name)) {
            if (std::find(std::cbegin(tracer.second.observers),
                          std::cend  (tracer.second.observers), observer) ==
                std::end(tracer.second.observers)) {
              observer->info.tracers.push_back(tracer.first);
              tracer.second.observers.push_back(observer);
              bound = true;
            }
          }
        }
        return bound;
      } else
        err = E_XXX;
      return false;
    }

    t_bool unbind_tracers(r_err err, R_observer_name name,
                          R_wildcard_name wildcard_name) {
      auto observer = is_observer(name);
      if (observer) {
        t_bool unbound = false;
        for (auto i = observer->info.tracers.begin();
             i != observer->info.tracers.end(); ) {
          t_tracer_name& tracer_name = *i;
          if (tracer_name.match(wildcard_name)) {
            auto tracer = tracers_.find(tracer_name);
            tracer->second.observers.erase(
              std::find(std::cbegin(tracer->second.observers),
                        std::cend  (tracer->second.observers), observer));
            observer->info.tracers.erase(i);
            unbound = true;
          } else
            ++i;
        }
        return unbound;
      } else
        err = E_XXX;
      return false;
    }

    t_bool fetch_bound_tracers(r_err, R_observer_name name,
                               r_tracer_names tracer_names) {
      auto observer = is_observer(name);
      if (observer) {
        tracer_names = observer->info.tracers;
        return !tracer_names.empty();
      }
      return false;
    }

    t_bool fetch_bound_observers(r_err err, R_tracer_name name,
                                 r_observer_names observer_names) {
      auto tracer = is_tracer(name);
      if (tracer) {
        for (auto observer : *tracer->observers)
          observer_names.push_back(observer->info.name);
        return !observer_names.empty();
      }
      return false;
    }

  private:
    t_freelist_  freelist_;
    t_tracers_   tracers_;
    t_observers_ observers_;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_logic_ : public t_thread::t_logic,
                   public t_cmd_processor::t_logic,
                   public t_que_processor::t_logic,
                   public t_dispatcher::t_logic {
  public:
    t_logic_(tracing::t_err err, R_params params)
      : data_         {params},
        ftrace_       {err},
        cmd_processor_{err},
        que_processor_{err, data_.params.queuesize},
        dispatcher_   {err, {t_n{2}, "epoll_service"}} {
    }

    t_cmd_client make_cmd_client() {
      return cmd_processor_.make_client(command::t_user{0L});
    }

    t_que_client make_que_client() {
      return que_processor_.make_client(waitable_chained_queue::t_user{0L});
    }

///////////////////////////////////////////////////////////////////////////////

    virtual t_validity update(t_thd_err err,
                              r_pthread_attr) noexcept override {
      printf("thread update - before thread is created\n");
      return VALID;
    }

    virtual t_validity prepare(t_thd_err err) noexcept override {
      printf("thread prepare - after thread is created\n");
      return VALID;
    }

    virtual p_void run() noexcept override {
      printf("thread run - its main loop\n");

      tracing::t_err err;
      {
        t_cmd_proxy_ cmd_proxy{err, action_, cmd_processor_, *this};
        dispatcher_.add_event (err, {cmd_processor_.get_fd(), RD}, &cmd_proxy);

        t_que_proxy_ que_proxy{err, action_, que_processor_, *this};
        dispatcher_.add_event (err, {que_processor_.get_fd(), RD}, &que_proxy);

        dispatcher_.event_loop(err, this);
      }
      if (err) {
        err.print();
        err.clear();
      }

      return nullptr;
    }

///////////////////////////////////////////////////////////////////////////////

    virtual t_void may_reorder_events (r_event_infos infos) override {
      //printf("events received = %lu\n", infos.size());
    }

    virtual t_void notify_event_remove(r_event_info) override {
      printf("remove event which should never happen!!\n");
      // not required
    }

    virtual t_quit notify_timeout(t_usec) override {
      printf("timeout that should never happen!!\n");
      return true; // not required
    }

    virtual t_quit notify_error(t_errn)  override {
      printf("error in event handling!!\n");
      return true; // die
    }

///////////////////////////////////////////////////////////////////////////////

    t_void process_item(waitable_chained_queue::t_entry& entry) {
      auto& item = entry.any.ref<t_item_>();
      auto  data = data_.is_tracer(item.id);
      if (data) {
        t_level level = item.level;
        t_date  date  = make_date(data_.params.time_mode, item.time);
        t_line  line  = make_line(entry.cnt, data->info.name, item.point,
                                  to_name(level), item.text);
        switch (data_.params.mode) {
          case OFF: {
          } break;

          case ALL: {
            if (data_.params.to_terminal)
              printf("%s %s\n", get(date.c_str()), get(line.c_str()));
            if (data_.params.to_observers)
              for (auto observer : *data->observers)
                if (level <= observer->info.params.level)
                  observer->impl->notify(observer->info.name, level, date,
                                         line);
          } break;

          case CONFIG: {
            if (level <= data->info.params.level) {
              if (data_.params.to_terminal)
                printf("%s %s\n", get(date.c_str()), get(line.c_str()));
              if (data_.params.to_observers)
                for (auto observer : *data->observers)
                  if (level <= observer->info.params.level)
                    observer->impl->notify(observer->info.name, level, date,
                                           line);
            }
          } break;
        }
      } else {
        printf("tracer(%s) died already\n", get(item.point.c_str()));
      }
    }

    t_void process_chain(t_chain& chain) {
      for (auto item = chain.head; item; item = item->next())
        process_item(item->ref());
    }

    virtual t_void async_process(t_chain chain) noexcept override {
      printf("recv a trace\n");
      process_chain(chain);
    }

    virtual t_void async_process(t_user, p_command cmd) noexcept override {
      printf("thread async command - none is used at this point\n");
      // not used
    }

///////////////////////////////////////////////////////////////////////////////

    t_void process(tracing::t_err err, r_do_chain_cmd_ cmd) noexcept {
      process_chain(cmd.chain);
    }

    t_void process(tracing::t_err err, r_update_params_cmd_ cmd) noexcept {
      printf("thread update_params_cmd received\n");
      if (get(data_.params.queuesize)   == get(cmd.params.queuesize) &&
          get(data_.params.max_tracers) == get(cmd.params.max_tracers)) {
        data_.params.to_terminal  = cmd.params.to_terminal;
        data_.params.to_observers = cmd.params.to_observers;
        data_.params.time_mode    = cmd.params.time_mode;
        data_.params.mode         = cmd.params.mode;
        data_.params.line_max     = cmd.params.line_max;
      } else
        err = E_XXX;
    }

    t_void process(tracing::t_err err, r_fetch_params_cmd_ cmd) noexcept {
      printf("thread fetch_params_cmd received\n");
      cmd.params = data_.params;
    }

    t_void process(tracing::t_err err, r_get_point_name_cmd_ cmd) noexcept {
      printf("thread get_point_name_cmd received\n");
      auto tracer = data_.is_tracer(cmd.id);
      if (tracer) {
        cmd.name = tracer->info.name;
      } else
        err = E_XXX;
    }

    t_void process(tracing::t_err err, r_get_point_level_cmd_ cmd) noexcept {
      printf("thread get_point_level_cmd received\n");
      auto tracer = data_.is_tracer(cmd.id);
      if (tracer) {
        cmd.level = tracer->info.params.level;
      } else
        err = E_XXX;
    }

    t_void process(tracing::t_err err, r_make_tracer_cmd_ cmd) noexcept {
      printf("thread make_tracer_cmd received\n");
      cmd.id = data_.add_tracer(err, cmd.name, cmd.params);
    }

    t_void process(tracing::t_err err, r_update_tracer_cmd_ cmd) noexcept {
      printf("thread update_tracer_cmd received\n");
      cmd.updated = data_.update_tracer(err, cmd.name, cmd.level);
    }

    t_void process(tracing::t_err err,
                   r_update_tracer_params_cmd_ cmd) noexcept {
      printf("thread update_tracer_params_cmd received\n");
      data_.update_tracer(err, cmd.name, cmd.params);
    }

    t_void process(tracing::t_err err,
                   r_fetch_tracer_params_cmd_ cmd) noexcept {
      printf("thread fetch_tracer_params_cmd received\n");
      cmd.found = false;
      auto tracer = data_.is_tracer(cmd.name);
      if (tracer) {
        cmd.params = tracer->info.params;
        cmd.found  = true;
      } else
        err = E_XXX;
    }

    t_void process(tracing::t_err err,
                   r_fetch_tracer_info_cmd_ cmd) noexcept {
      printf("thread fetch_tracer_info_cmd received\n");
      cmd.found = false;
      auto tracer = data_.is_tracer(cmd.name);
      if (tracer) {
        cmd.info  = tracer->info;
        cmd.found = true;
        if (cmd.clear_stats)
          tracer->info.stats.reset();
      } else
        err = E_XXX;
    }

    t_void process(tracing::t_err err, r_fetch_tracers_cmd_ cmd) noexcept {
      printf("thread fetch_tracers_cmd received\n");
      // XXX-1
    }

    t_void process(tracing::t_err err, r_create_observer_cmd_ cmd) noexcept {
      printf("thread create_observer_cmd received\n");
      t_output_impl_ptr_ impl;
      switch (cmd.params.output) {
        case LOGGER:
          impl = t_output_impl_ptr_{&logger_, nullptr};
          break;
        case FTRACE:
          impl = t_output_impl_ptr_{&ftrace_, nullptr};
          break;
        case SHM:
          err = E_XXX; // not implemented
          break;
      }
      data_.add_observer(err, cmd.name, cmd.params, std::move(impl));
    }

    t_void process(tracing::t_err err, r_destroy_observer_cmd_ cmd) noexcept {
      printf("thread destroy_observer_cmd received\n");
      data_.del_observer(err, cmd.name);
    }

    t_void process(tracing::t_err err, r_update_observer_cmd_ cmd) noexcept {
      printf("thread update_observer_cmd received\n");
      // check if new logger is required
      // XXX-4
    }

    t_void process(tracing::t_err err, r_fetch_observer_cmd_ cmd) noexcept {
      printf("thread fetch_observer_cmd received\n");
      auto observer = data_.is_observer(cmd.name);
      if (observer) {
        cmd.params = observer->info.params;
        cmd.found = true;
      } else
        cmd.found = false;
    }

    t_void process(tracing::t_err err,
                   r_fetch_observer_info_cmd_ cmd) noexcept {
      printf("thread fetch_observer_info_cmd received\n");
      auto observer = data_.is_observer(cmd.name);
      if (observer) {
        cmd.info = observer->info;
        cmd.found = true;
      } else
        cmd.found = false;
    }

    t_void process(tracing::t_err err, r_fetch_observers_cmd_ cmd) noexcept {
      printf("thread fetch_observers_cmd received\n");
      // XXX-6
    }

    t_void process(tracing::t_err err, r_bind_tracer_cmd_ cmd) noexcept {
      printf("thread bind_tracer_cmd received\n");
      cmd.found = data_.bind_tracer(err, cmd.name, cmd.tracer_name);
    }

    t_void process(tracing::t_err err, r_bind_tracers_cmd_ cmd) noexcept {
      printf("thread bind_tracers_cmd received\n");
      cmd.found = data_.bind_tracers(err, cmd.name, cmd.wildcard_name);
    }

    t_void process(tracing::t_err err, r_unbind_tracers_cmd_ cmd) noexcept {
      printf("thread unbind_tracers_cmd received\n");
      cmd.found = data_.unbind_tracers(err, cmd.name, cmd.wildcard_name);
    }

    t_void process(tracing::t_err err, r_is_tracer_bound_cmd_ cmd) noexcept {
      printf("thread is_tracer_bound_cmd received\n");
      cmd.found = data_.is_tracer_bound(cmd.name, cmd.tracer_name);
    }

    t_void process(tracing::t_err err,
                   r_fetch_bound_tracers_cmd_ cmd) noexcept {
      printf("thread fetch_bound_tracers_cmd received\n");
      cmd.found = data_.fetch_bound_tracers(err, cmd.name, cmd.tracer_names);
    }

    t_void process(tracing::t_err err,
                   r_fetch_bound_observers_cmd_ cmd) noexcept {
      printf("thread fetch_bound_observers_cmd received\n");
      cmd.found = data_.fetch_bound_observers(err, cmd.name, cmd.observer_names);
    }

    t_void process(tracing::t_err err, r_destroy_tracer_cmd_ cmd) noexcept {
      printf("thread destroy_tracer_cmd received\n");
      cmd.done = data_.del_tracer(cmd.id) == VALID;
    }

    t_void process(tracing::t_err err, r_clean_death_cmd_ cmd) noexcept {
      printf("thread clean_death_cmd received\n");
      action_.cmd = QUIT_EVENT_LOOP;
    }

    virtual t_void process(t_cmd_err err, t_user,
                           r_command cmd) noexcept override {
      T_ERR_GUARD(err) {
        switch (cmd.id) {
          case t_update_params_cmd_::cmd_id:
            process(err, static_cast<r_update_params_cmd_>(cmd));
            break;
          case t_fetch_params_cmd_::cmd_id:
            process(err, static_cast<r_fetch_params_cmd_>(cmd));
            break;
          case t_get_point_name_cmd_::cmd_id:
            process(err, static_cast<r_get_point_name_cmd_>(cmd));
            break;
          case t_get_point_level_cmd_::cmd_id:
            process(err, static_cast<r_get_point_level_cmd_>(cmd));
            break;
          case t_make_tracer_cmd_::cmd_id:
            process(err, static_cast<r_make_tracer_cmd_>(cmd));
            break;
          case t_update_tracer_cmd_::cmd_id:
            process(err, static_cast<r_update_tracer_cmd_>(cmd));
            break;
          case t_update_tracer_params_cmd_::cmd_id:
            process(err, static_cast<r_update_tracer_params_cmd_>(cmd));
            break;
          case t_fetch_tracer_params_cmd_::cmd_id:
            process(err, static_cast<r_fetch_tracer_params_cmd_>(cmd));
            break;
          case t_fetch_tracer_info_cmd_::cmd_id:
            process(err, static_cast<r_fetch_tracer_info_cmd_>(cmd));
            break;
          case t_fetch_tracers_cmd_::cmd_id:
            process(err, static_cast<r_fetch_tracers_cmd_>(cmd));
            break;
          case t_create_observer_cmd_::cmd_id:
            process(err, static_cast<r_create_observer_cmd_>(cmd));
            break;
          case t_destroy_observer_cmd_::cmd_id:
            process(err, static_cast<r_destroy_observer_cmd_>(cmd));
            break;
          case t_update_observer_cmd_::cmd_id:
            process(err, static_cast<r_update_observer_cmd_>(cmd));
            break;
          case t_fetch_observer_cmd_::cmd_id:
            process(err, static_cast<r_fetch_observer_cmd_>(cmd));
            break;
          case t_fetch_observer_info_cmd_::cmd_id:
            process(err, static_cast<r_fetch_observer_info_cmd_>(cmd));
            break;
          case t_fetch_observers_cmd_::cmd_id:
            process(err, static_cast<r_fetch_observers_cmd_>(cmd));
            break;
          case t_bind_tracer_cmd_::cmd_id:
            process(err, static_cast<r_bind_tracer_cmd_>(cmd));
            break;
          case t_bind_tracers_cmd_::cmd_id:
            process(err, static_cast<r_bind_tracers_cmd_>(cmd));
            break;
          case t_unbind_tracers_cmd_::cmd_id:
            process(err, static_cast<r_unbind_tracers_cmd_>(cmd));
            break;
          case t_is_tracer_bound_cmd_::cmd_id:
            process(err, static_cast<r_is_tracer_bound_cmd_>(cmd));
            break;
          case t_fetch_bound_tracers_cmd_::cmd_id:
            process(err, static_cast<r_fetch_bound_tracers_cmd_>(cmd));
            break;
          case t_fetch_bound_observers_cmd_::cmd_id:
            process(err, static_cast<r_fetch_bound_observers_cmd_>(cmd));
            break;
          case t_destroy_tracer_cmd_::cmd_id:
            process(err, static_cast<r_destroy_tracer_cmd_>(cmd));
            break;
          case t_do_chain_cmd_::cmd_id:
            process(err, static_cast<r_do_chain_cmd_>(cmd));
            break;
          case t_clean_death_cmd_::cmd_id:
            process(err, static_cast<r_clean_death_cmd_>(cmd));
            break;
          default:
            // made a mess
            // XXX- 16
            break;
        }
      }
    }

///////////////////////////////////////////////////////////////////////////////

  private:
    class t_cmd_proxy_ : public t_event_logic {
    public:
      t_cmd_proxy_(tracing::t_err& err, t_action& action,
                   t_cmd_processor& processor, t_cmd_processor::t_logic& logic)
        : err_(err), action_(action), processor_(processor), logic_{logic} {
      }

      virtual t_name get_name() const override {
        return {"cmd logic"};
      }

      virtual t_action notify_event(r_event_params params) override {
        action_.cmd = CONTINUE;
        processor_.process(err_, logic_);
        return action_;
      }

    private:
      tracing::t_err&           err_;
      t_action&                 action_;
      t_cmd_processor&          processor_;
      t_cmd_processor::t_logic& logic_;
    };

    class t_que_proxy_ : public t_event_logic {
    public:
      t_que_proxy_(tracing::t_err& err, t_action& action,
                   t_que_processor& processor, t_que_processor::t_logic& logic)
        : err_(err), action_(action), processor_(processor), logic_{logic} {
      }

      virtual t_name get_name() const override {
        return {"queue logic"};
      }

      virtual t_action notify_event(r_event_params params) override {
        action_.cmd = CONTINUE;
        processor_.process_available(err_, logic_);
        return action_;
      }

    private:
      tracing::t_err&           err_;
      t_action&                 action_;
      t_que_processor&          processor_;
      t_que_processor::t_logic& logic_;
    };

    t_action         action_;
    t_data_          data_;
    t_logger_output_ logger_;
    t_ftrace_output_ ftrace_;
    t_cmd_processor  cmd_processor_;
    t_que_processor  que_processor_;
    t_dispatcher     dispatcher_;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_tracing_ {
  public:
    t_tracing_(t_err& err, R_params params)
      : logic_     {err, params},
        cmd_client_{logic_.make_cmd_client()},
        que_client_{logic_.make_que_client()},
        thread_    {err, P_cstr{"tracing"}, &logic_, false},
        shared_tr_ {make_tracer(err, P_cstr{"shared"}, t_tracer_params())} {
    }

    t_tracer_name get_point_name(R_id id) {
      t_tracer_name name;
      t_get_point_name_cmd_ cmd(id, name);
      if (cmd_client_.request(cmd) == VALID)
        return name;
      return {};
    }

    t_level get_point_level(R_id id) {
      t_level level;
      t_get_point_level_cmd_ cmd(id, level);
      if (cmd_client_.request(cmd) == VALID)
        return level;
      return {};
    }

    t_validity update(t_err& err, R_params params) {
      t_update_params_cmd_ cmd(params);
      cmd_client_.request(err, cmd);
      return !err ? VALID : INVALID;
    }

    t_validity fetch(t_err& err, t_params& params) {
      t_fetch_params_cmd_ cmd(params);
      cmd_client_.request(err, cmd);
      return !err ? VALID : INVALID;
    }

    t_tracer make_tracer(t_err& err, R_tracer_name name,
                         R_tracer_params params) {
      t_id id{};
      t_make_tracer_cmd_ cmd(name, params, id);
      cmd_client_.request(err, cmd);
      return tracer::mk_(!err ? id : t_id{}, name);
    }

    t_bool update_tracer(t_err& err, R_wildcard_name name, t_level level) {
      t_bool updated = false;
      t_update_tracer_cmd_ cmd(name, level, updated);
      cmd_client_.request(err, cmd);
      return !err ? updated : false;
    }

    t_validity update_tracer(t_err& err, R_tracer_name name,
                             R_tracer_params params) {
      t_update_tracer_params_cmd_ cmd(name, params);
      cmd_client_.request(err, cmd);
      return !err ? VALID : INVALID;
    }

    t_bool fetch_tracer(t_err& err, R_tracer_name name,
                        r_tracer_params params) {
      t_bool found = false;
      t_fetch_tracer_params_cmd_ cmd(name, params, found);
      cmd_client_.request(err, cmd);
      return !err ? found : false;
    }

    t_bool fetch_tracer(t_err& err, R_tracer_name name,
                        r_tracer_info info, t_bool clear_stats) {
      t_bool found = false;
      t_fetch_tracer_info_cmd_ cmd(name, info, clear_stats, found);
      cmd_client_.request(err, cmd);
      return !err ? found : false;
    }

    t_bool fetch_tracers(t_err& err, t_tracer_infos& infos,
                         t_bool clear_stats) {
      t_bool found = false;
      t_fetch_tracers_cmd_ cmd(infos, clear_stats, found);
      cmd_client_.request(err, cmd);
      return !err ? found : false;
    }

    t_validity create_observer(t_err& err, R_observer_name name,
                               R_observer_params params) {
      t_create_observer_cmd_ cmd(name, params);
      cmd_client_.request(err, cmd);
      return !err ? VALID : INVALID;
    }

    t_validity destroy_observer(t_err& err, R_observer_name name) {
      t_destroy_observer_cmd_ cmd(name);
      cmd_client_.request(err, cmd);
      return !err ? VALID : INVALID;
    }

    t_validity update_observer(t_err& err, R_observer_name name,
                               R_observer_params params) {
      t_update_observer_cmd_ cmd(name, params);
      cmd_client_.request(err, cmd);
      return !err ? VALID : INVALID;
    }

    t_bool fetch_observer(t_err& err, R_observer_name name,
                          r_observer_params params) {
      t_bool found = false;
      t_fetch_observer_cmd_ cmd(name, params, found);
      cmd_client_.request(err, cmd);
      return !err ? found : false;
    }

    t_bool fetch_observer(t_err& err, R_observer_name name,
                          r_observer_info info, t_bool clear_stats) {
      t_bool found = false;
      t_fetch_observer_info_cmd_ cmd(name, info, clear_stats, found);
      cmd_client_.request(err, cmd);
      return !err ? found : false;
    }

    t_bool fetch_observers(t_err& err, r_observer_infos infos,
                           t_bool clear_stats) {
      t_bool found = false;
      t_fetch_observers_cmd_ cmd(infos, clear_stats, found);
      cmd_client_.request(err, cmd);
      return !err ? found : false;
    }

    t_bool bind_tracer(t_err& err, R_observer_name name,
                       R_tracer_name tracer_name) {
      t_bool found = false;
      t_bind_tracer_cmd_ cmd(name, tracer_name, found);
      cmd_client_.request(err, cmd);
      return !err ? found : false;
    }

    t_bool bind_tracers (t_err& err, R_observer_name name,
                         R_wildcard_name wildcard_name) {
      t_bool found = false;
      t_bind_tracers_cmd_ cmd(name, wildcard_name, found);
      cmd_client_.request(err, cmd);
      return !err ? found : false;
    }

    t_bool unbind_tracers(t_err& err, R_observer_name name,
                          R_wildcard_name wildcard_name) {
      t_bool found = false;
      t_unbind_tracers_cmd_ cmd(name, wildcard_name, found);
      cmd_client_.request(err, cmd);
      return !err ? found : false;
    }

    t_bool is_tracer_bound(t_err& err, R_observer_name name,
                           R_tracer_name tracer_name) {
      t_bool found = false;
      t_is_tracer_bound_cmd_ cmd(name, tracer_name, found);
      cmd_client_.request(err, cmd);
      return !err ? found : false;
    }

    t_bool fetch_bound_tracers(t_err& err, R_observer_name name,
                               r_tracer_names tracer_names) {
      t_bool found = false;
      t_fetch_bound_tracers_cmd_ cmd(name, tracer_names, found);
      cmd_client_.request(err, cmd);
      return !err ? found : false;
    }

    t_bool fetch_bound_observers(t_err& err, R_tracer_name name,
                                 r_observer_names observer_names) {
      t_bool found = false;
      t_fetch_bound_observers_cmd_ cmd(name, observer_names, found);
      cmd_client_.request(err, cmd);
      return !err ? found : false;
    }

    t_bool destroy_tracer(tracer::t_id id) {
      t_bool done = false;
      t_destroy_tracer_cmd_ cmd(id, done);
      return cmd_client_.request(cmd) == VALID ? done : false;
    }

    t_validity do_chain(t_err& err, t_que_chain& chain) {
      t_do_chain_cmd_ cmd(chain);
      cmd_client_.request(err, cmd);
      return !err ? VALID : INVALID;
    }

    t_validity do_chain(t_que_chain& chain) {
      t_do_chain_cmd_ cmd(chain);
      return cmd_client_.request(cmd);
    }

    t_validity clean_death() {
      t_clean_death_cmd_ cmd;
      return cmd_client_.request(cmd);
    }

///////////////////////////////////////////////////////////////////////////////

    t_validity shared_trace(t_level level, R_text text) {
      return ref(shared_tr_).post(level, text);
    }

    t_validity shared_trace(t_err err, t_level level, R_text text) {
      return ref(shared_tr_).post(err, level, text);
    }

    t_validity post(R_id id, t_level level, R_tracer_name name, R_text text) {
      t_que_chain chain = que_client_.acquire();
      if (get(chain.cnt)) {
        t_item_& item = chain.head->ref().any.emplace<t_item_>(t_any_user{0L});
        item.point = name;
        item.text  = text;
        item.level = level;
        item.id    = id;
        item.time  = clock::realtime_now();
        return level > CRITICAL ? que_client_.compared_insert(chain) :
                                  do_chain(chain);
      }
      return INVALID;
    }

    t_validity post(t_err& err, R_id id, t_level level,
                    R_tracer_name name, R_text text) {
      t_que_chain chain = que_client_.acquire(err);
      if (!err) {
        t_item_& item = chain.head->ref().any.emplace<t_item_>(t_any_user{0L});
        item.point = name;
        item.text  = text;
        item.level = level;
        item.id    = id;
        item.time  = clock::realtime_now();
        return level > CRITICAL ? que_client_.compared_insert(err, chain) :
                                  do_chain(err, chain);
      }
      return INVALID;
    }

    t_validity waitable_post(R_id id, t_level level,
                            R_tracer_name name, R_text text) {
      t_que_chain chain = que_client_.waitable_acquire();
      if (get(chain.cnt)) {
        t_item_& item = chain.head->ref().any.emplace<t_item_>(t_any_user{0L});
        item.point = name;
        item.text  = text;
        item.level = level;
        item.id    = id;
        item.time  = clock::realtime_now();
        return level > CRITICAL ? que_client_.compared_insert(chain) :
                                  do_chain(chain);
      }
      return INVALID;
    }

    t_validity waitable_post(t_err& err, R_id id, t_level level,
                             R_tracer_name name, R_text text) {
      t_que_chain chain = que_client_.waitable_acquire(err);
      if (!err) {
        t_item_& item = chain.head->ref().any.emplace<t_item_>(t_any_user{0L});
        item.point = name;
        item.text  = text;
        item.level = level;
        item.id    = id;
        item.time  = clock::realtime_now();
        return level > CRITICAL ?  que_client_.compared_insert(err, chain) :
                                   do_chain(err, chain);
      }
      return INVALID;
    }

///////////////////////////////////////////////////////////////////////////////

  private:
    t_logic_     logic_;
    t_cmd_client cmd_client_;
    t_que_client que_client_;
    t_thread     thread_;
    t_tracer     shared_tr_;
  };

  std::unique_ptr<t_tracing_> tr_ = nullptr; // shared_ptr thread safe

///////////////////////////////////////////////////////////////////////////////

namespace tracer
{
  t_levelname to_name(t_level level) {
    const char* tbl[] = { "emerg", "alert", "critical", "error", "warning",
                          "notice", "info", "debug" };
    return P_cstr(tbl[level]);
  }

  inline t_level default_level() {
    return NOTICE;
  }

  inline t_credit default_credit() {
    return 50;
  }

  inline t_bool operator==(R_id lh, R_id rh) {
    return lh.seq_ == rh.seq_ && get(lh.id_)  == get(rh.id_);
  }

  inline t_bool operator!=(R_id lh, R_id rh) {
    return !(lh == rh);
  }

///////////////////////////////////////////////////////////////////////////////

  t_validity t_point::post(t_level level, R_text text) const {
    if (tracing::tr_)
      return level < NOTICE ?
               tracing::tr_->waitable_post(id_, level, name_, text) :
               tracing::tr_->post         (id_, level, name_, text);
    return INVALID;
  }

  t_validity t_point::post(t_err err, t_level level, R_text text) const {
    T_ERR_GUARD(err) {
      if (tracing::tr_)
        return level < NOTICE ?
          tracing::tr_->waitable_post(err, id_, level, name_, text) :
          tracing::tr_->post         (err, id_, level, name_, text);
      err = E_XXX;
    }
    return INVALID;
  }

  t_validity t_point::waitable_post(t_level level, R_text text) const {
    if (tracing::tr_)
      return tracing::tr_->waitable_post(id_, level, name_, text);
    return INVALID;
  }

  t_validity t_point::waitable_post(t_err err, t_level level,
                                    R_text text) const {
    T_ERR_GUARD(err) {
      if (tracing::tr_)
        return tracing::tr_->waitable_post(err, id_, level, name_, text);
      err = E_XXX;
    }
    return INVALID;
  }

  t_name t_point::get_name() const {
    if (tracing::tr_)
      return tracing::tr_->get_point_name(id_);
    return t_name();
  }

  t_level t_point::get_level() const {
    if (tracing::tr_)
      return tracing::tr_->get_point_level(id_);
    return EMERG;
  }

///////////////////////////////////////////////////////////////////////////////

  t_tracer::t_tracer(x_tracer tracer) : point_{tracer.point_.release()} {
  }

  t_tracer& t_tracer::operator=(x_tracer tracer) {
    if (point_ == VALID && tracing::tr_)
      tracing::tr_->destroy_tracer(point_.id_.release());
    point_.id_ = tracer.point_.id_.release();
    return *this;
  }

  t_tracer::~t_tracer() {
    if (point_ == VALID && tracing::tr_)
      tracing::tr_->destroy_tracer(point_.id_.release());
  }

  t_point t_tracer::make_point(R_name name) {
    // could do more with it but not now
    return t_point{point_.id_, name};
  }

///////////////////////////////////////////////////////////////////////////////

  t_validity shared_trace(t_level level, R_text text) {
    if (tracing::tr_)
      return tracing::tr_->shared_trace(level, text);
    return INVALID;
  }

  t_validity shared_trace(t_err err, t_level level, R_text text) {
    T_ERR_GUARD(err) {
      if (tracing::tr_)
        return tracing::tr_->shared_trace(err, level, text);
      err = E_XXX;
    }
    return INVALID;
  }
}

///////////////////////////////////////////////////////////////////////////////

  t_output_name to_name(t_output output) {
    const char* tbl[] = { "logger", "ftrace", "shm" };
    return P_cstr(tbl[output]);
  }

  t_time_mode_name to_name(t_time_mode mode) {
    const char* tbl[] = { "ns", "ns_diff", "date" };
    return P_cstr(tbl[mode]);
  }

  t_mode_name to_name(t_mode mode) {
    const char* tbl[] = { "all", "off", "config" };
    return P_cstr(tbl[mode]);
  }

  t_level default_observer_level() {
    return DEBUG;
  }

///////////////////////////////////////////////////////////////////////////////

  t_validity start(t_err err, P_params params) {
    T_ERR_GUARD(err) {
      static t_mutex_lock lock(err);
      <% auto scope = lock.make_locked_scope(err);
        if (scope == VALID) {
          if (!tr_) {
            tr_.reset(new t_tracing_{err, params ? *params : t_params{}});
            if (err)
              delete tr_.release();
          }
          return VALID;
        }
      %>
    }
    return INVALID;
  }

  t_void destroy() {
    if (tr_) {
      tr_->clean_death();
      delete tr_.release();
    }
  }

  t_bool is_running() {
    if (tr_)
      return true;
    return false;
  }

  t_validity update(t_err err, R_params params) {
    T_ERR_GUARD(err) {
      if (tracing::tr_)
        return tracing::tr_->update(err, params);
      err = E_XXX;
    }
    return INVALID;
  }

  t_validity fetch(t_err err, r_params params) {
    T_ERR_GUARD(err) {
      if (tracing::tr_)
        return tracing::tr_->fetch(err, params);
      err = E_XXX;
    }
    return INVALID;
  }

  t_tracer make_tracer(t_err err, R_tracer_name name) {
    T_ERR_GUARD(err) {
      if (tracing::tr_)
        return tracing::tr_->make_tracer(err, name, t_tracer_params());
      err = E_XXX;
    }
    return mk_(tracer::t_id{}, tracer::t_name{});
  }

  t_tracer make_tracer(t_err err, R_tracer_name name, R_tracer_params params) {
    T_ERR_GUARD(err) {
      if (tracing::tr_)
        return tracing::tr_->make_tracer(err, name, params);
      err = E_XXX;
    }
    return mk_(tracer::t_id{}, tracer::t_name{});
  }

  t_bool update_tracer(t_err err, R_wildcard_name name, t_level level) {
    T_ERR_GUARD(err) {
      if (tracing::tr_)
        return tracing::tr_->update_tracer(err, name, level);
      err = E_XXX;
    }
    return false;
  }

  t_validity update_tracer(t_err err, R_tracer_name name,
                           R_tracer_params params) {
    T_ERR_GUARD(err) {
      if (tracing::tr_)
        return tracing::tr_->update_tracer(err, name, params);
      err = E_XXX;
    }
    return INVALID;
  }

  t_bool fetch_tracer(t_err err, R_tracer_name name, r_tracer_params params) {
    T_ERR_GUARD(err) {
      if (tracing::tr_)
        return tracing::tr_->fetch_tracer(err, name, params);
      err = E_XXX;
    }
    return false;
  }

  t_bool fetch_tracer(t_err err, R_tracer_name name, r_tracer_info info,
                      t_bool clear_stats) {
    T_ERR_GUARD(err) {
      if (tracing::tr_)
        return tracing::tr_->fetch_tracer(err, name, info, clear_stats);
      err = E_XXX;
    }
    return false;
  }

  t_bool fetch_tracers(t_err err, r_tracer_infos infos, t_bool clear_stats) {
    T_ERR_GUARD(err) {
      if (tracing::tr_)
        return tracing::tr_->fetch_tracers(err, infos, clear_stats);
      err = E_XXX;
    }
    return false;
  }

///////////////////////////////////////////////////////////////////////////////

  t_validity create_observer(t_err err, R_observer_name name) {
    T_ERR_GUARD(err) {
      if (tracing::tr_)
        return tracing::tr_->create_observer(err, name, t_observer_params());
      err = E_XXX;
    }
    return INVALID;
  }

  t_validity create_observer(t_err err, R_observer_name name,
                             R_observer_params params) {
    T_ERR_GUARD(err) {
      if (tracing::tr_)
        return tracing::tr_->create_observer(err, name, params);
      err = E_XXX;
    }
    return INVALID;
  }

  t_validity destroy_observer(t_err err, R_observer_name name) {
    T_ERR_GUARD(err) {
      if (tracing::tr_)
        return tracing::tr_->destroy_observer(err, name);
      err = E_XXX;
    }
    return INVALID;
  }

  t_validity update_observer(t_err err, R_observer_name name,
                             R_observer_params params) {
    T_ERR_GUARD(err) {
      if (tracing::tr_)
        return tracing::tr_->update_observer(err, name, params);
      err = E_XXX;
    }
    return INVALID;
  }

  t_bool fetch_observer(t_err err, R_observer_name name,
                        r_observer_params params) {
    T_ERR_GUARD(err) {
      if (tracing::tr_)
        return tracing::tr_->fetch_observer(err, name, params);
    }
    return false;
  }

  t_bool fetch_observer(t_err err, R_observer_name name,
                        r_observer_info info, t_bool clear_stats) {
    T_ERR_GUARD(err) {
      if (tracing::tr_)
        return tracing::tr_->fetch_observer(err, name, info, clear_stats);
    }
    return false;
  }

  t_bool fetch_observers(t_err err, r_observer_infos infos,
                         t_bool clear_stats) {
    T_ERR_GUARD(err) {
      if (tracing::tr_)
        return tracing::tr_->fetch_observers(err, infos, clear_stats);
    }
    return false;
  }

///////////////////////////////////////////////////////////////////////////////

  t_bool bind_tracer(t_err err, R_observer_name name,
                     R_tracer_name tracer_name) {
    T_ERR_GUARD(err) {
      if (tracing::tr_)
        return tracing::tr_->bind_tracer(err, name, tracer_name);
    }
    return false;
  }

  t_bool bind_tracers (t_err err, R_observer_name name,
                       R_wildcard_name wildcard) {
    T_ERR_GUARD(err) {
      if (tracing::tr_)
        return tracing::tr_->bind_tracers(err, name, wildcard);
    }
    return false;
  }

  t_bool unbind_tracers(t_err err, R_observer_name name,
                        R_wildcard_name wildcard) {
    T_ERR_GUARD(err) {
      if (tracing::tr_)
        return tracing::tr_->unbind_tracers(err, name, wildcard);
    }
    return false;
  }

  t_bool is_tracer_bound(t_err err, R_observer_name name,
                         R_tracer_name tracer_name) {
    T_ERR_GUARD(err) {
      if (tracing::tr_)
        return tracing::tr_->is_tracer_bound(err, name, tracer_name);
    }
    return false;
  }

  t_bool fetch_bound_tracers(t_err err, R_observer_name name,
                             r_tracer_names tracer_names) {
    T_ERR_GUARD(err) {
      if (tracing::tr_)
        return tracing::tr_->fetch_bound_tracers(err, name, tracer_names);
    }
    return false;
  }

  t_bool fetch_bound_observers(t_err err, R_tracer_name name,
                               r_observer_names observer_names) {
    T_ERR_GUARD(err) {
      if (tracing::tr_)
        return tracing::tr_->fetch_bound_observers(err, name, observer_names);
    }
    return false;
  }

///////////////////////////////////////////////////////////////////////////////

  struct automatic_start_ {
    automatic_start_() {
      t_err err;
      if (start(err) == INVALID) {
        // what to do
        // XXX - 11
      }
    }
    ~automatic_start_() {
      destroy();
    }
  };

  automatic_start_ start_;

///////////////////////////////////////////////////////////////////////////////
}
}
