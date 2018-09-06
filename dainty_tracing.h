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

#ifndef _DAINTY_TRACING_H_
#define _DAINTY_TRACING_H_

#include <vector>
#include "dainty_tracing_tracer.h"

namespace dainty
{
namespace tracing
{
  using named::t_n;
  using tracer::t_err;
  using tracer::t_string;
  using tracer::t_void;
  using tracer::t_bool;
  using tracer::t_validity;
  using tracer::t_level;
  using tracer::t_credit;
  using tracer::t_impl_id_;
  using tracer::VALID;
  using tracer::INVALID;
  using tracer::FMT;
  using tracer::EMERG;
  using tracer::ALERT;
  using tracer::CRITICAL;
  using tracer::ERROR;
  using tracer::WARNING;
  using tracer::NOTICE;
  using tracer::INFO;
  using tracer::DEBUG;
  using tracer::to_name;

///////////////////////////////////////////////////////////////////////////////

  using t_tracer        = tracer::t_tracer;
  using t_point         = tracer::t_point;
  using t_tracer_id     = tracer::t_id;
  using t_tracer_name   = tracer::t_name;
  using R_tracer_name   = tracer::R_name;
  using t_tracer_params = tracer::t_params;
  using r_tracer_params = tracer::r_params;
  using R_tracer_params = tracer::R_params;

  enum  t_wildcard_name_tag_ { };
  using t_wildcard_name = t_string<t_wildcard_name_tag_, 32>;
  using R_wildcard_name = named::t_prefix<t_wildcard_name>::R_;

  enum  t_observer_name_tag_ { };
  using t_observer_name = t_string<t_observer_name_tag_, 32>;
  using R_observer_name = named::t_prefix<t_observer_name>::R_;

///////////////////////////////////////////////////////////////////////////////

  enum t_output {
    LOGGER = 0,
    FTRACE,
    SHM
  };

  enum  t_output_name_tag_ { };
  using t_output_name = t_string<t_output_name_tag_, 20>;

  t_output_name to_name(t_output);

///////////////////////////////////////////////////////////////////////////////

  enum t_time_mode {
    NS = 0,
    NS_DIFF,
    DATE
  };

  enum  t_time_mode_name_tag_ { };
  using t_time_mode_name = t_string<t_time_mode_name_tag_, 20>;

  t_time_mode_name to_name(t_time_mode);

///////////////////////////////////////////////////////////////////////////////

  enum t_mode {
    ALL = 0,
    OFF,
    CONFIG
  };

  enum  t_mode_name_tag_ { };
  using t_mode_name = t_string<t_mode_name_tag_, 20>;

  t_mode_name to_name(t_mode);

///////////////////////////////////////////////////////////////////////////////

  class t_stats {
  public:
    using t_n = tracing::t_n;

    t_stats();

    t_n get      (t_level) const;
    t_n increment(t_level);

    t_n reset();
    t_n total() const;

  private:
    using t_n_ = named::t_n_;
    t_n_ used_[DEBUG + 1];
  };

///////////////////////////////////////////////////////////////////////////////

  t_level default_observer_level();

  class t_observer_params {
  public:
    t_level  level;
    t_bool   bind_to_all;         // not allowed to change after creation
    t_output output;              // not allowed to change after creation

    t_observer_params(t_level  _level       = default_observer_level(),
                      t_bool   _bind_to_all = false,
                      t_output _output      = LOGGER)
      : level(_level), bind_to_all(_bind_to_all), output(_output) {
    }
  };

  using r_observer_params = named::t_prefix<t_observer_params>::r_;
  using R_observer_params = named::t_prefix<t_observer_params>::R_;

///////////////////////////////////////////////////////////////////////////////

  using t_tracer_names   = std::vector<t_tracer_name>;
  using r_tracer_names   = named::t_prefix<t_tracer_names>::r_;
  using t_observer_names = std::vector<t_observer_name>;
  using r_observer_names = named::t_prefix<t_observer_names>::r_;

  class t_observer_info {
  public:
    t_observer_info() {
    }

    t_observer_info(R_observer_name _name, R_observer_params _params)
      : name(_name), params(_params) {
    }

    t_observer_name   name;
    t_observer_params params;
    t_tracer_names    tracers;
    t_stats           stats;
  };

  class t_tracer_info {
  public:
    t_tracer_name   name;
    t_tracer_params params;
    t_stats         stats;
  };

  using r_tracer_info    = named::t_prefix<t_tracer_info>::r_;
  using p_tracer_info    = named::t_prefix<t_tracer_info>::p_;
  using r_observer_info  = named::t_prefix<t_observer_info>::r_;
  using t_observer_infos = std::vector<t_observer_info>;
  using r_observer_infos = named::t_prefix<t_observer_infos>::r_;
  using t_tracer_infos   = std::vector<t_tracer_info>;
  using r_tracer_infos   = named::t_prefix<t_tracer_infos>::r_;

///////////////////////////////////////////////////////////////////////////////

  class t_params {
  public:
    t_n         queuesize;
    t_n         max_tracers;
    t_bool      to_terminal;
    t_bool      to_observers;
    t_time_mode time_mode;
    t_mode      mode;
    t_n         line_max;

    t_params() : queuesize   (4000),
                 max_tracers (100),
                 to_terminal (true),
                 to_observers(true),
                 time_mode   (NS_DIFF),
                 mode        (ALL),
                 line_max    (100) {
    }

    t_params(t_n         _queuesize,
             t_n         _max_tracers,
             t_bool      _to_terminal,
             t_bool      _to_observers,
             t_time_mode _time_mode,
             t_mode      _mode,
             t_n         _line_max)
      : queuesize   (_queuesize),
        max_tracers (_max_tracers),
        to_terminal (_to_terminal),
        to_observers(_to_observers),
        time_mode   (_time_mode),
        mode        (_mode),
        line_max    (_line_max) {
    }
  };
  using P_params = named::t_prefix<t_params>::P_;
  using r_params = named::t_prefix<t_params>::r_;
  using R_params = named::t_prefix<t_params>::R_;

///////////////////////////////////////////////////////////////////////////////

  t_bool is_running();

  t_void start (t_err, P_params = nullptr);
  t_void update(t_err, R_params);
  t_void fetch (t_err, r_params);

///////////////////////////////////////////////////////////////////////////////

  t_tracer make_tracer  (t_err, R_tracer_name);
  t_tracer make_tracer  (t_err, R_tracer_name, R_tracer_params);
  t_void   update_tracer(t_err, R_tracer_name, R_tracer_params);
  t_void   update_tracer(t_err, R_wildcard_name, t_level);
  t_bool   is_tracer    (t_err, R_tracer_name, r_tracer_params);
  t_bool   is_tracer    (t_err, R_tracer_name, r_tracer_info,
                                t_bool clearstats = false);
  t_void   fetch_tracers(t_err, r_tracer_infos, t_bool clearstats = false);

///////////////////////////////////////////////////////////////////////////////

  t_void create_observer (t_err, R_observer_name);
  t_void create_observer (t_err, R_observer_name, R_observer_params);
  t_void destroy_observer(t_err, R_observer_name);
  t_void update_observer (t_err, R_observer_name, R_observer_params);
  t_bool is_observer     (t_err, R_observer_name, r_observer_params);
  t_bool is_observer     (t_err, R_observer_name, r_observer_info,
                                 t_bool clearstats = false);
  t_void fetch_observers (t_err, r_observer_infos,
                                 t_bool clearstats = false);

///////////////////////////////////////////////////////////////////////////////

  t_void   bind_tracer        (t_err, R_observer_name, R_tracer_name);
  t_void   bind_tracers       (t_err, R_observer_name, R_wildcard_name);
  t_void unbind_tracers       (t_err, R_observer_name, R_wildcard_name);
  t_bool is_tracer_bound      (t_err, R_observer_name, R_tracer_name);
  t_void fetch_bound_tracers  (t_err, R_observer_name, r_tracer_names);
  t_void fetch_bound_observers(t_err, R_tracer_name,   r_observer_names);

///////////////////////////////////////////////////////////////////////////////

  inline
  t_stats::t_stats() {
    reset();
  }

  inline
  t_n t_stats::get(t_level level) const {
    return t_n{used_[level]};
  }

  inline
  t_n t_stats::increment(t_level level) {
    return t_n{++used_[level]};
  }

  inline
  t_n t_stats::reset() {
    t_n_ sum = 0;
    for (t_n_ i = 0; i < sizeof(used_)/sizeof(t_n_); ++i) {
      sum += used_[i];
      used_[i] = 0;
    }
    return t_n{sum};
  }

  inline
  t_n t_stats::total() const {
    t_n_ sum = 0;
    for (t_n_ i = 0; i < sizeof(used_)/sizeof(t_n_); ++i)
      sum += used_[i];
    return t_n{sum};
  }

///////////////////////////////////////////////////////////////////////////////
}
}

#endif
