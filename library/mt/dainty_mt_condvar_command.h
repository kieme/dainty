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

#ifndef _DAINTY_MT_CONDVAR_COMMAND_H_
#define _DAINTY_MT_CONDVAR_COMMAND_H_

#include "dainty_base_ptr.h"
#include "dainty_mt_err.h"

namespace dainty
{
namespace mt
{
namespace condvar_command
{
///////////////////////////////////////////////////////////////////////////////

  using err::t_err;

  using oops::t_oops;

  using base::types::t_prefix;
  using base::types::t_void;
  using base::types::t_uint;

  using base::specific::t_errn;
  using base::specific::t_validity;
  using base::specific::t_n;
  using base::specific::VALID;
  using base::specific::INVALID;
  using base::specific::NO_ERRN;

  using base::ptr::t_ptr;
  using base::ptr::t_no_deleter;
  using base::ptr::t_deleter;

///////////////////////////////////////////////////////////////////////////////

  enum  t_user_tag_ { };
  using t_user = base::t_user<t_user_tag_>;

///////////////////////////////////////////////////////////////////////////////

  using t_id = t_uint;

  class t_command {
  public:
    t_command(t_id _id) noexcept : id(_id) { }
    virtual ~t_command() { }

    const t_id id;
  };
  using r_command = t_prefix<t_command>::r_;
  using p_command = t_prefix<t_command>::p_;

///////////////////////////////////////////////////////////////////////////////

  class t_impl_;
  enum  t_impl_user_tag_ { };
  using t_impl_user_ = t_ptr<t_impl_, t_impl_user_tag_, t_no_deleter>;

  enum  t_impl_owner_tag_ { };
  using t_impl_owner_ = t_ptr<t_impl_, t_impl_owner_tag_, t_deleter>;

///////////////////////////////////////////////////////////////////////////////

  class t_client;
  using r_client = t_prefix<t_client>::r_;
  using x_client = t_prefix<t_client>::x_;
  using R_client = t_prefix<t_client>::R_;

  class t_client {
  public:
    t_client(x_client client) noexcept;

    r_client operator=(R_client) = delete;
    r_client operator=(x_client) = delete;

    operator t_validity() const noexcept;

    t_void       request(t_err, r_command) noexcept;
    t_errn async_request(       p_command) noexcept;
    t_void async_request(t_err, p_command) noexcept;

  private:
    friend class t_processor;
    friend class t_impl_;
    t_client() = default;
    t_client(t_impl_user_, t_user) noexcept;

    t_impl_user_ impl_;
    t_user       user_ = t_user{0L};
  };

///////////////////////////////////////////////////////////////////////////////

  class t_processor;
  using r_processor = t_prefix<t_processor>::r_;
  using x_processor = t_prefix<t_processor>::x_;
  using R_processor = t_prefix<t_processor>::R_;

  class t_processor {
  public:
    class t_logic {
    public:
      using t_err     = t_oops<>;
      using t_user    = condvar_command::t_user;
      using t_command = condvar_command::t_command;
      using p_command = condvar_command::p_command;
      using r_command = condvar_command::r_command;

      virtual ~t_logic() { }
      virtual t_void       process(t_err, t_user, r_command) noexcept = 0;
      virtual t_void async_process(       t_user, p_command) noexcept = 0;
    };

    using r_logic = t_prefix<t_logic>::r_;

     t_processor(t_err)       noexcept;
     t_processor(x_processor) noexcept;
    ~t_processor();

    t_processor(R_processor)            = delete;
    r_processor operator=(x_processor) = delete;
    r_processor operator=(R_processor) = delete;

    operator t_validity () const noexcept;

    t_void   process(t_err, r_logic, t_n max = t_n{1}) noexcept;

    t_client make_client(       t_user) noexcept;
    t_client make_client(t_err, t_user) noexcept;

  private:
    t_impl_owner_ impl_;
  };

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
