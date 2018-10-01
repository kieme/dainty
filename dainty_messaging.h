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

#ifndef _DAINTY_MESSAGING_H_
#define _DAINTY_MESSAGING_H_

#include <vector>
#include "dainty_messaging_messenger.h"

namespace dainty
{
namespace messaging
{
  using named::t_bool;
  using named::p_bool;
  using named::t_void;
  using named::t_n_;
  using named::t_n;
  using named::P_cstr;
  using named::t_validity;
  using named::string::t_string;
  using named::t_prefix;
  using named::VALID;
  using named::INVALID;
  using err::t_err;
  using messenger::t_multiple_of_100ms;
  using messenger::t_messenger;

  using t_messenger_params       = messenger::t_params;
  using r_messenger_params       = messenger::r_params;
  using t_messenger_visibility   = messenger::t_visibility;
  using t_messenger_prio         = messenger::t_prio;
  using t_messenger_key          = messenger::t_key;
  using R_messenger_key          = messenger::R_key;
  using t_messenger_user         = messenger::t_user;
  using t_messenger_name         = messenger::t_name;
  using r_messenger_name         = messenger::r_name;
  using R_messenger_name         = messenger::R_name;
  using R_messenger_password     = messenger::R_password;
  using r_messenger_monitor_list = messenger::r_monitor_list;
  using p_messenger_monitor_list = messenger::p_monitor_list;
  using p_messenger_group_list   = messenger::p_group_list;
  using r_messenger_group_list   = messenger::r_group_list;
  using t_messenger_timer_params = messenger::t_timer_params;
  using R_messenger_timer_params = messenger::R_timer_params;
  using r_messenger_visibility   = t_prefix<t_messenger_visibility>::r_;
  using p_messenger_user         = t_prefix<t_messenger_user>::p_;

///////////////////////////////////////////////////////////////////////////////

  class t_messenger_create_params {
  public:
    t_messenger_visibility   visibility;
    t_multiple_of_100ms      alive_factor;
    t_messenger_timer_params timer_params;

    inline t_messenger_create_params()
      : visibility(messenger::VISIBILITY_PROCESS), alive_factor(0) {
    }

    inline t_messenger_create_params(t_messenger_visibility _visibility)
      : visibility(_visibility), alive_factor(0) {
    }

    inline t_messenger_create_params(t_messenger_visibility   _visibility,
                                     t_multiple_of_100ms      _alive_factor,
                                     R_messenger_timer_params _timer_params)
      : visibility(_visibility), alive_factor(_alive_factor),
        timer_params(_timer_params) {
    }
  };
  using R_messenger_create_params = t_prefix<t_messenger_create_params>::R_;

///////////////////////////////////////////////////////////////////////////////

  enum  t_password_tag_ { };
  using t_password = t_string<t_password_tag_, 16>;
  using R_password = t_prefix<t_password>::R_;

///////////////////////////////////////////////////////////////////////////////

  enum t_visibility {
    VISIBILITY_OFF,
    VISIBILITY_LOCAL,
    VISIBILITY_SLAVE,
    VISIBILITY_MASTER
  };

  enum  t_visibility_name_tag_ { };
  using t_visibility_name = t_string<t_visibility_name_tag_, 14>;

  t_visibility_name to_name(t_visibility);

///////////////////////////////////////////////////////////////////////////////

  enum  t_name_tag_ { };
  using t_name = t_string<t_name_tag_, 16>;
  using R_name = t_prefix<t_name>::R_;

///////////////////////////////////////////////////////////////////////////////

  class t_params {
  public:
    t_visibility visibility;
    t_n          queuesize = t_n{4000};

    inline t_params() : visibility{VISIBILITY_OFF} {
    }

    inline t_params(t_visibility _visibility) : visibility{_visibility} {
    }

    inline t_params(t_visibility _visibility, t_n _queuesize)
      : visibility{_visibility}, queuesize{_queuesize} {
    }
  };
  using r_params = t_prefix<t_params>::r_;
  using R_params = t_prefix<t_params>::R_;
  using P_params = t_prefix<t_params>::P_;

///////////////////////////////////////////////////////////////////////////////

  class t_messenger_stats {
  public:
    t_void reset() {
      // XXX
    }
  };

///////////////////////////////////////////////////////////////////////////////

  class t_messenger_info {
  public:
    t_messenger_key    key;
    t_messenger_name   name;
    t_messenger_params params;
    t_messenger_stats  stats;

    inline t_messenger_info() : key{0} {
    }
  };
  using r_messenger_info = t_prefix<t_messenger_info>::r_;

  using t_messenger_infos = std::vector<t_messenger_info>;
  using r_messenger_infos = t_prefix<t_messenger_infos>::r_;

///////////////////////////////////////////////////////////////////////////////

  t_bool is_running();

  t_void start (t_err, R_name, P_params = nullptr);
  t_void update(t_err, R_params);
  t_void fetch (t_err, r_params);

///////////////////////////////////////////////////////////////////////////////

  t_messenger_create_params default_messenger_create_params();

///////////////////////////////////////////////////////////////////////////////

  t_messenger create_messenger(t_err, R_messenger_name,
                                      R_messenger_create_params
                                        = default_messenger_create_params());
  t_bool is_messenger(t_err, R_messenger_name, r_messenger_params);
  t_bool is_messenger(t_err, R_messenger_name, r_messenger_info,
                             t_bool clearstats = false);
  t_void fetch_messengers(t_err, r_messenger_infos, t_bool clearstats = false);

///////////////////////////////////////////////////////////////////////////////

  t_void create_group (t_err, R_password, R_messenger_name,
                              t_messenger_visibility);
  t_void destroy_group(t_err, R_password, R_messenger_name);
  t_bool is_group     (t_err, R_messenger_name, r_messenger_visibility,
                              p_messenger_group_list = nullptr);

  t_void add_messenger_to_group(t_err, R_messenger_password,
                                       R_messenger_name name,
                                       R_messenger_name group,
                                       t_messenger_prio = t_messenger_prio(0),
                                       t_messenger_user = t_messenger_user());
  t_void remove_messenger_from_group(t_err, R_messenger_password,
                                            R_messenger_name name,
                                            R_messenger_name group,
                                            p_messenger_user = nullptr);
  t_bool is_messenger_in_group (t_err, R_messenger_name name,
                                       R_messenger_name group,
                                       p_messenger_user = nullptr);
  t_void fetch_messenger_groups(t_err, R_messenger_name name,
                                       r_messenger_group_list);

///////////////////////////////////////////////////////////////////////////////

  t_bool who_is(t_err, R_messenger_key, r_messenger_name,
                       p_bool group = nullptr, p_bool local = nullptr);

///////////////////////////////////////////////////////////////////////////////
}
}

#endif
