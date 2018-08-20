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

#ifndef _DAINTY_MT_COMMAND_H_
#define _DAINTY_MT_COMMAND_H_

#include "dainty_os_fdbased.h"
#include "dainty_mt_err.h"

namespace dainty
{
namespace mt
{
namespace command
{
  using os::fdbased::t_fd;
  using named::t_n;
  using named::t_void;
  using named::t_validity;
  using named::VALID;
  using named::INVALID;

  enum  t_user_tag_ { };
  using t_user = named::t_user<t_user_tag_>;

///////////////////////////////////////////////////////////////////////////////

  using t_id = named::t_uint;

  class t_command {
  public:
    t_command(t_id _id) noexcept : id(_id) { }
    virtual ~t_command() { }

    const t_id id;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_impl_;

  class t_client {
  public:
    t_client(t_client&& client) noexcept;

    t_client& operator=(const t_client&) = delete;
    t_client& operator=(t_client&&)      = delete;

    operator t_validity() const noexcept;

    t_validity       request(       t_command&) noexcept;
    t_validity       request(t_err, t_command&) noexcept;

    t_validity async_request(       t_command*) noexcept;
    t_validity async_request(t_err, t_command*) noexcept;

  private:
    friend class t_processor;
    friend class t_impl_;
    t_client() = default;
    t_client(t_impl_* impl, t_user user) noexcept : impl_(impl), user_(user) {
    }

    t_impl_* impl_ = nullptr;
    t_user   user_ = t_user{0L};
  };

///////////////////////////////////////////////////////////////////////////////

  class t_processor {
  public:
    class t_logic {
    public:
      using t_err     = oops::t_oops<>;
      using t_user    = command::t_user;
      using t_command = named::t_prefix<command::t_command>::t_;
      using p_command = named::t_prefix<command::t_command>::p_;
      using r_command = named::t_prefix<command::t_command>::r_;

      virtual ~t_logic() { }
      virtual t_void       process(t_err, t_user, r_command) noexcept = 0;
      virtual t_void async_process(       t_user, p_command) noexcept = 0;
    };

    using r_logic = named::t_prefix<t_logic>::r_;

     t_processor(t_err)         noexcept;
     t_processor(t_processor&&) noexcept;
    ~t_processor();

    t_processor(const t_processor&)            = delete;
    t_processor& operator=(t_processor&&)      = delete;
    t_processor& operator=(const t_processor&) = delete;

    operator t_validity () const noexcept;

    t_fd get_fd() const noexcept;

    t_validity process(t_err, r_logic, t_n max = t_n{1}) noexcept;

    t_client make_client(       t_user) noexcept;
    t_client make_client(t_err, t_user) noexcept;

  private:
    t_impl_* impl_ = nullptr;
  };

///////////////////////////////////////////////////////////////////////////////

  inline
  t_client::t_client(t_client&& client) noexcept
      : impl_(client.impl_), user_(client.user_) {
    client.impl_ = nullptr;
    client.user_ = t_user{0L};
  }

  inline
  t_client::operator t_validity() const noexcept {
    return impl_ ? VALID : INVALID;
  }

///////////////////////////////////////////////////////////////////////////////

  inline
  t_processor::t_processor(t_processor&& processor) noexcept
      : impl_(processor.impl_) {
    processor.impl_ = nullptr;
  }

  inline
  t_processor::operator t_validity() const noexcept {
    return impl_ ? VALID : INVALID;
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
