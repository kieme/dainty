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

#include <typeinfo>
#include "dainty_container_any.h"

namespace dainty
{
namespace container
{
namespace any
{
  t_bool same_type_(R_type_ lh, R_type_ rh) {
    return typeid(lh) == typeid(rh);
  }

  r_any t_any::operator=(R_any any) {
    if (store_ == VALID && any.store_ == VALID &&
        same_type_(*store_, *any.store_))
      store_->copy(*any.store_);
    else {
      store_.clear();
      if (any.store_ == VALID) {
        user_  = any.user_;
        store_ = any.store_->clone();
      }
    }
    return *this;
  }

  r_any t_any::operator=(x_any any) {
    store_.clear();
    user_  = any.user_;
    store_ = std::move(any.store_);
    any.user_.id = 0;
    return *this;
  }

  t_bool t_any::same_type(R_any any) const {
    if (user_.id == any.user_.id) {
       if (store_ == VALID && any.store_ == VALID)
         return same_type_(*store_, *any.store_);
       if (store_ == INVALID && any.store_ == INVALID)
         return true;
    }
    return false;
  }

  t_bool t_any::is_equal(R_any any) const {
    if (same_type(any) && store_ == VALID)
      return store_->is_equal(*any.store_);
    return false;
  }

  r_any t_any::assign(t_user user) {
    store_.clear();
    user_ = user;
    return *this;
  }

////////////////////////////////////////////////////////////////////////////////
}
}
}
