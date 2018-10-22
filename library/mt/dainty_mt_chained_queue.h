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

#ifndef _DAINTY_MT_CHAINED_QUEUE_H_
#define _DAINTY_MT_CHAINED_QUEUE_H_

#include "dainty_named_ptr.h"
#include "dainty_named_utility.h"
#include "dainty_container_any.h"
#include "dainty_container_chained_queue.h"
#include "dainty_mt_err.h"

namespace dainty
{
namespace mt
{
namespace chained_queue
{
  using named::t_fd;
  using named::t_n;
  using named::t_void;
  using named::t_validity;
  using named::VALID;
  using named::INVALID;
  using named::t_errn;
  using named::t_prefix;
  using named::ptr::t_deleter;
  using err::t_err;

  enum  t_user_tag_ { };
  using t_user = named::t_user<t_user_tag_>;

  using t_any   = container::any::t_any;
  using t_chain = container::chained_queue::t_chain<t_any>;

///////////////////////////////////////////////////////////////////////////////

  class t_impl_;
  enum  t_impl_user_tag_ { };
  using t_impl_user_ = named::ptr::t_ptr<t_impl_, t_impl_user_tag_,
                                         named::ptr::t_no_deleter>;
  enum  t_impl_owner_tag_ { };
  using t_impl_owner_ = named::ptr::t_ptr<t_impl_, t_impl_owner_tag_,
                                          named::ptr::t_deleter>;

///////////////////////////////////////////////////////////////////////////////

  class t_client;
  using r_client = t_prefix<t_client>::r_;
  using x_client = t_prefix<t_client>::x_;
  using R_client = t_prefix<t_client>::R_;

  class t_client {
  public:
    using t_chain = chained_queue::t_chain;

    t_client(x_client) noexcept;

    r_client operator=(R_client) = delete;
    r_client operator=(x_client) = delete;

    operator t_validity() const noexcept;

    t_chain acquire(       t_n = t_n{1}) noexcept;
    t_chain acquire(t_err, t_n = t_n{1}) noexcept;

    t_errn  insert (       t_chain)      noexcept;
    t_void  insert (t_err, t_chain)      noexcept;

  private:
    friend class t_processor;
    friend class t_impl_;
    t_client() = default;
    t_client(t_impl_user_ impl, t_user user) noexcept
      : impl_(impl), user_(user) {
    }

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
      using t_chain = chained_queue::t_chain;

      virtual ~t_logic() { }
      virtual t_void async_process(t_chain) noexcept = 0;
    };

    using r_logic = t_logic&;

     t_processor(t_err, t_n max) noexcept;
     t_processor(x_processor)    noexcept;
    ~t_processor();

    t_processor(R_processor)           = delete;
    r_processor operator=(x_processor) = delete;
    r_processor operator=(R_processor) = delete;

    operator t_validity () const noexcept;

    t_fd get_fd() const noexcept;

    t_void process          (t_err, r_logic, t_n max = t_n{1}) noexcept;
    t_void process_available(t_err, r_logic) noexcept;

    t_client make_client(       t_user) noexcept;
    t_client make_client(t_err, t_user) noexcept;

  private:
    t_impl_owner_ impl_;
  };

///////////////////////////////////////////////////////////////////////////////

  inline
  t_client::t_client(x_client client) noexcept
      : impl_{client.impl_.release()},
        user_{named::utility::reset(client.user_)} {
  }

  inline
  t_client::operator t_validity() const noexcept {
    return impl_;
  }

///////////////////////////////////////////////////////////////////////////////

  inline
  t_processor::t_processor(x_processor processor) noexcept
    : impl_(processor.impl_.release()) {
  }

  inline
  t_processor::operator t_validity() const noexcept {
    return impl_;
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
