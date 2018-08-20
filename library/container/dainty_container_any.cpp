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
  t_bool same_type_(const t_it_& it1, const t_it_& it2) {
    return typeid(it1) == typeid(it2);
  }

  t_any& t_any::operator=(const t_any& any) {
    if (store_ && any.store_ && same_type_(*store_, *any.store_))
      store_->copy(*any.store_);
    else {
      if (store_)
        delete store_;
      user_  = any.user_;
      store_ = any.store_;
      if (store_)
        store_ = store_->clone();
    }
    return *this;
  }

  t_any& t_any::operator=(t_any&& any) {
    if (store_)
      delete store_;
    user_  = any.user_;
    store_ = any.store_;
    any.store_   = nullptr;
    any.user_.id = 0;
    return *this;
  }

  t_any::~t_any() {
    if (store_)
      delete store_;
  }

  t_bool t_any::same_type(const t_any& any) const {
    if (user_.id == any.user_.id) {
       if (store_ && any.store_)
         return same_type_(*store_, *any.store_);
       if (!store_ && !any.store_)
         return true;
    }
    return false;
  }

  t_bool t_any::is_equal(const t_any& any) const {
    if (same_type(any) && store_)
      return store_->is_equal(*any.store_);
    return false;
  }

  t_any& t_any::assign(t_user user) {
    if (store_) {
      delete store_;
      store_ = nullptr;
    }
    user_ = user;
    return *this;
  }

////////////////////////////////////////////////////////////////////////////////
}
}
}
