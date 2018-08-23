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
  using library::types::void_t;

///////////////////////////////////////////////////////////////////////////////

  enum remote_visibility_t {
    remote_visibility_off,
    remote_visibility_local,
    remote_visibility_slave,
    remote_visibility_master
  };

  enum remotename_tag_t { };
  typedef nstring_t<16, remotename_tag_t> remotename_t;

  struct params_t {
    remote_visibility_t visibility_;
    remotename_t        name_;
    params_t() : visibility_(remote_visibility_off) { }
    params_t(remote_visibility_t visibility) : visibility_(visibility) { }
    params_t(remote_visibility_t visibility, const remotename_t& name)
      : visibility_(visibility), name_(name) { }
  };

///////////////////////////////////////////////////////////////////////////////

  struct messenger_stats_t {
    void_t reset() { }
  };

  struct messenger_info_t {
    messenger_key_t    key_;
    messenger_name_t   name_;
    messenger_params_t params_;
    messenger_stats_t  stats_;
    messenger_info_t() : key_(0) { }
  };

  typedef std::vector<messenger_info_t> messenger_infos_t;

///////////////////////////////////////////////////////////////////////////////

  bool_t is_running();

  void_t update_params(const params_t&);
  void_t fetch_params (params_t&);

///////////////////////////////////////////////////////////////////////////////

  messenger_create_params_t default_messenger_create_params();

///////////////////////////////////////////////////////////////////////////////

  t_messenger create_messenger(const messenger_name_t&,
                               const messenger_create_params_t&
                                   = default_messenger_create_params());

  t_messenger create_messenger(errorid_t&,
                               const messenger_name_t&,
                               const messenger_create_params_t&
                                   = default_messenger_create_params());

  bool_t fetch_messenger(const messenger_name_t&, messenger_params_t&);
  bool_t fetch_messenger(const messenger_name_t&, messenger_info_t&,
                         bool_t clearstats = false);

  bool_t fetch_messengers(messenger_infos_t&, bool_t clearstats = false);

  bool_t destroy_messenger(t_messenger);

///////////////////////////////////////////////////////////////////////////////

  // group - individual, chained, chained_revert
  bool_t create_group(const password_t&, const messenger_name_t&,
                      messenger_visibility_t);
  bool_t destroy_group(const password_t&, const messenger_name_t&);
  bool_t fetch_group(const messenger_name_t&, messenger_visibility_t&,
                     messenger_memberlist_t* = 0);

  bool_t add_messenger_to_group(const password_t&,
                                const messenger_name_t& name,
                                const messenger_name_t& group,
                                messenger_prio_t =
                                  messenger_prio_t(0),
                                messenger_user_t =
                                  messenger_user_t());
  bool_t remove_messenger_from_group(const password_t&,
                                     const messenger_name_t& name,
                                     const messenger_name_t& group,
                                     messenger_user_t* = 0);
  bool_t is_messenger_in_group (const messenger_name_t& name,
                                const messenger_name_t& group,
                                messenger_user_t* = 0);
  bool_t fetch_messenger_groups(const messenger_name_t& name,
                                messenger_memberlist_t&);

///////////////////////////////////////////////////////////////////////////////

  bool_t who_is(const messenger_key_t&, messenger_name_t&,
                bool_t* group = 0, bool_t* local = 0);

///////////////////////////////////////////////////////////////////////////////

  bool_t post_message(const messenger_key_t&, message_t&);

///////////////////////////////////////////////////////////////////////////////
}
}

#endif
