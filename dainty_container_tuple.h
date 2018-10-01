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

#ifndef _DAINTY_CONTAINER_TUPLE_H_
#define _DAINTY_CONTAINER_TUPLE_H_

#include "dainty_container_any.h"
#include "dainty_container_list.h"

namespace dainty
{
namespace container
{
namespace tuple
{
  using named::t_n_;
  using named::t_n;
  using named::t_ix;
  using named::t_prefix;
  using named::t_validity;
  using named::t_validity;
  using named::VALID;
  using named::INVALID;
  using any::t_any;
  using any::t_user;

  template<t_n_ N>
  using t_store_ = list::t_list<t_any, N>;

  template<t_n_ N, typename T, typename... Ts>
  struct t_tuple_ final {
    using r_store_ = typename t_prefix<t_store_<N>>::r_;

    t_tuple_<N, T>     tuple_;
    t_tuple_<N, Ts...> tuples_;

    t_tuple_(r_store_ store) : tuple_{store},  tuples_{store} {
    }
    template<typename Arg, typename... Args>
    t_tuple_(r_store_ store, Arg&& arg, Args&&... args)
      : tuple_ {store, std::forward<Arg> (arg)},
        tuples_{store, std::forward<Args>(args)...} {
    }
  };

  template<t_n_ N, typename T>
  struct t_tuple_<N, T> final {
    using r_store_ = typename t_prefix<t_store_<N>>::r_;

    t_tuple_(r_store_ store) {
      (store.push_back())->template emplace<T>(t_user{0L});
    }
    template<typename Arg>
    t_tuple_(r_store_ store, Arg&& arg) {
      (store.push_back())->template emplace<T>(t_user{0L},
                                               std::forward<Arg>(arg));
    }
  };

  template<typename... Ts>
  class t_tuple final {
  public:
    inline
    t_tuple() : tuple_{store_} {
    }

    template<typename... Args>
    t_tuple(Args&&... args) : tuple_{store_, std::forward<Args>(args)...} {
    }

    inline
    operator t_validity() const {
      return store_;
    }

    inline
    t_any& operator[](t_ix ix) {
      return *store_.get(ix);
    }

    inline
    const t_any& operator[](t_ix ix) const {
      return *store_.get(ix);
    }

    inline
    t_n get_size() const {
      return store_.get_capacity();
    }

  private:
    t_store_<sizeof...(Ts)> store_;
    t_tuple_<sizeof...(Ts), Ts...> tuple_;
  };
}
}
}

#endif
