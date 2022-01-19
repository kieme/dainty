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

#ifndef _DAINTY_ARRAY_H_
#define _DAINTY_ARRAY_H_

#include "impl_/dainty_array_impl_.h"

namespace dainty
{
namespace array
{
  /////////////////////////////////////////////////////////////////////////////

  using types::t_void;
  using types::t_bool;
  using types::t_n_max_;

  using t_n_max = impl_::t_n_max;

  using util::f_cast;

  using impl_::INIT_FARGS;
  using impl_::INIT_NONE;
  using impl_::INIT_CUSTOM;

  /////////////////////////////////////////////////////////////////////////////

  template<int N, typename T>
  class t_array {
    using r_array_ = types::r_of<t_array>;
    using R_array_ = types::R_of<t_array>;

    using t_value_ = types::t_of<T>;
    using r_value_ = types::r_of<t_value_>;
    using R_value_ = types::R_of<t_value_>;
    using p_value_ = types::p_of<t_value_>;
    using P_value_ = types::P_of<t_value_>;

    using t_arr_   = t_value_[N];
    using p_arr_   = types::p_of<t_arr_>;
    using P_arr_   = types::P_of<t_arr_>;
    using r_arr_   = types::r_of<t_arr_>;
    using R_arr_   = types::R_of<t_arr_>;

  public:
    using t_value  = t_value_;
    using t_arr    = t_arr_;

    constexpr
    static int capacity();                                 // ARRAY_1_1

    t_array();                                             // ARRAY_1_2_1

    constexpr
    t_array(impl_::t_init_none);                           // ARRAY_1_2_2

    template<typename U, typename... Us>
    t_array(impl_::t_init_fargs, U&& u, Us&&... us);       // ARRAY_1_2_3

    template<typename C>
    t_array(impl_::t_init_custom, C&&);                    // ARRAY_1_2_4

    t_array(R_array_);                                     // ARRAY_1_2_5

    template<typename U>
    t_array(const t_array<N, U>&);                         // ARRAY_1_2_6

    template<typename U>
    t_array(const U (&)[N]);                               // ARRAY_1_2_7

    r_array_ operator=(R_array_);                          // ARRAY_1_3_1

    template<typename U>
    r_array_ operator=(const U (&)[N]);                    // ARRAY_1_3_2

    template<typename U>
    r_array_ operator=(const t_array<N, U>&);              // ARRAY_1_3_3

    ~t_array();                                            // ARRAY_1_4

    template<typename U, typename... Us>
    r_array_ init(impl_::t_init_fargs, U&&, Us&&...);      // ARRAY_1_5_1

    template<typename C>
    r_array_ init(impl_::t_init_custom, C&&);              // ARRAY_1_5_2

    template<typename U>
    r_array_ init(const U (&)[N]);                         // ARRAY_1_5_3

    template<typename U>
    r_array_ init(const t_array<N, U>&);                   // ARRAY_1_5_4

    r_array_ destruct();                                   // ARRAY_1_6

    p_value_ ptr(int ix = 0);                              // ARRAY_1_7
    P_value_ Ptr(int ix = 0) const;                        // ARRAY_1_8

    r_value_ set(int ix);                                  // ARRAY_1_9
    R_value_ Get(int ix) const;                            // ARRAY_1_10

    r_value_ operator[](int ix);                           // ARRAY_1_11_1
    R_value_ operator[](int ix) const;                     // ARRAY_1_11_2

    operator p_value_ ();                                  // ARRAY_1_12_1
    operator P_value_ () const;                            // ARRAY_1_12_2

    r_arr_ set();                                          // ARRAY_1_13
    R_arr_ Get() const;                                    // ARRAY_1_14

    t_bool is_equal(R_array_) const;                       // ARRAY_1_15_1
    t_bool is_equal(R_arr_)   const;                       // ARRAY_1_15_2

    p_value_ begin();                                      // ARRAY_1_16
    P_value_ Begin() const;                                // ARRAY_1_17

    p_value_ end();                                        // ARRAY_1_18
    P_value_ End() const;                                  // ARRAY_1_19

    p_value_ rbegin();                                     // ARRAY_1_20
    P_value_ RBegin() const;                               // ARRAY_1_21

    p_value_ rend();                                       // ARRAY_1_22
    P_value_ REnd() const;                                 // ARRAY_1_23

    template<typename F>
    t_void each(F&&);                                      // ARRAY_1_24

    template<typename F>
    t_void Each(F&&) const;                                // ARRAY_1_25

  private:
    impl_::t_valuestore<t_value> store_[N];
  };

  /////////////////////////////////////////////////////////////////////////////

  template<typename T>
  class t_array<0, T> {
    using r_array_ = types::r_of<t_array>;
    using R_array_ = types::R_of<t_array>;

    using t_value_ = types::t_of<T>;
    using r_value_ = types::r_of<t_value_>;
    using R_value_ = types::R_of<t_value_>;
    using p_value_ = types::p_of<t_value_>;
    using P_value_ = types::P_of<t_value_>;

    using t_arr_   = t_value_[];
    using p_arr_   = types::p_of<t_arr_>;
    using P_arr_   = types::P_of<t_arr_>;
    using r_arr_   = types::r_of<t_arr_>;
    using R_arr_   = types::R_of<t_arr_>;

  public:
    using t_value  = t_value_;
    using t_arr    = t_arr_;

    int capacity();                                            // ARRAY_2_1

    t_array();                                                 // ARRAY_2_2_1

    t_array(t_n_max, impl_::t_init_none);                      // ARRAY_2_2_2

    template<typename C>
    t_array(t_n_max, impl_::t_init_custom, C&&);               // ARRAY_2_2_3

    template<typename U, typename... Us>
    t_array(impl_::t_init_fargs, U&& u, Us&&... us);           // ARRAY_2_2_4

    template<typename U, typename... Us>
    t_array(t_n_max, impl_::t_init_fargs, U&& u, Us&&... us);  // ARRAY_2_2_5

    t_array(R_array_);                                         // ARRAY_2_2_6

    template<int N, typename U>
    t_array(const t_array<N, U>&);                             // ARRAY_2_2_7

    template<int N, typename U>
    t_array(t_n_max, const t_array<N, U>&);                    // ARRAY_2_2_8

    template<int N, typename U>
    t_array(const U (&)[N]);                                   // ARRAY_2_2_9

    template<int N, typename U>
    t_array(t_n_max, const U (&)[N]);                          // ARRAY_2_2_10

    r_array_ operator=(R_array_);                              // ARRAY_2_3_1

    template<int N, typename U>
    r_array_ operator=(const U (&)[N]);                        // ARRAY_2_3_2

    template<int N, typename U>
    r_array_ operator=(const t_array<N, U>&);                  // ARRAY_2_3_3

    ~t_array();                                                // ARRAY_2_4

    template<typename U, typename... Us>
    r_array_ init(impl_::t_init_fargs, U&&, Us&&...);          // ARRAY_2_5_1

    template<typename U, typename... Us>
    r_array_ init(t_n_max, impl_::t_init_fargs, U&&, Us&&...); // ARRAY_2_5_2

    template<typename C>
    r_array_ init(impl_::t_init_custom, C&&);                  // ARRAY_2_5_3

    template<typename C>
    r_array_ init(t_n_max, impl_::t_init_custom, C&&);         // ARRAY_2_5_4

    template<int N, typename U>
    r_array_ init(const U (&)[N]);                             // ARRAY_2_5_5

    template<int N, typename U>
    r_array_ init(t_n_max, const U (&)[N]);                    // ARRAY_2_5_6

    template<int N, typename U>
    r_array_ init(const t_array<N, U>&);                       // ARRAY_2_5_7

    template<int N, typename U>
    r_array_ init(t_n_max, const t_array<N, U>&);              // ARRAY_2_5_8

    r_array_ destruct();                                       // ARRAY_2_6

    p_value_ ptr(int ix = 0);                                  // ARRAY_2_7
    P_value_ Ptr(int ix = 0) const;                            // ARRAY_2_8

    r_value_ set(int ix);                                      // ARRAY_2_9
    R_value_ Get(int ix) const;                                // ARRAY_2_10

    r_value_ operator[](int ix);                               // ARRAY_2_11_1
    R_value_ operator[](int ix) const;                         // ARRAY_2_11_2

    operator p_value_ ();                                      // ARRAY_2_12_1
    operator P_value_ () const;                                // ARRAY_2_12_2
    operator t_bool   () const;                                // ARRAY_2_12_3

    r_arr_ set();                                              // ARRAY_2_13
    R_arr_ Get() const;                                        // ARRAY_2_14

    template<int N>
    t_bool is_equal(const t_array<N, T>&) const;               // ARRAY_2_15_1
    template<int N>
    t_bool is_equal(const T (&)[N]) const;                     // ARRAY_2_15_2

    p_value_ begin();                                          // ARRAY_2_16
    P_value_ Begin() const;                                    // ARRAY_2_17

    p_value_ end();                                            // ARRAY_2_18
    P_value_ End() const;                                      // ARRAY_2_19

    p_value_ rbegin();                                         // ARRAY_2_20
    P_value_ RBegin() const;                                   // ARRAY_2_21

    p_value_ rend();                                           // ARRAY_2_22
    P_value_ REnd() const;                                     // ARRAY_2_23

    template<typename F>
    t_void each(F&&);                                          // ARRAY_2_24

    template<typename F>
    t_void Each(F&&) const;                                    // ARRAY_2_25

  private:
    t_n_max  max_   = 0;
    p_value_ store_ = nullptr;
  };

  /////////////////////////////////////////////////////////////////////////////

  // MAKE_ARRAY_01
  template<typename T, typename... Ts>
  t_array<sizeof...(Ts) + 1, T>
      make_array(impl_::t_init_fargs, T&&, Ts&&...);

  // MAKE_ARRAY_02
  template<int N, typename T, typename... Ts>
  t_array<N, T> make_array(impl_::t_init_fargs, T&&, Ts&&...);

  // MAKE_ARRAY_03
  template<int N, typename T>
  t_array<N, T> make_array(T (&arr)[N]);

  // MAKE_ARRAY_04
  template<int N, typename T>
  t_array<N, T> make_array(const T (&arr)[N]);

  // MAKE_ARRAY_05
  template<int N, typename T>
  t_array<N, T> make_array();

  // MAKE_ARRAY_06
  template<int N, typename T>
  t_array<N, T> make_array(impl_::t_init_none);

  // MAKE_ARRAY_07
  template<int N, typename T, typename C>
  t_array<N, T> make_array(impl_::t_init_custom, C&&);

  /////////////////////////////////////////////////////////////////////////////

  // operator==_01
  template<int N, typename T>
  t_bool operator==(const t_array<N, T>&, const t_array<N, T>&);

  // operator==_02
  template<int N, typename T>
  t_bool operator==(const t_array<N, T>&, const T (&)[N]);

  // operator==_03
  template<int N, typename T>
  t_bool operator==(const T (&)[N], const t_array<N, T>&);

  // operator==_04
  template<int N, typename T>
  t_bool operator==(const t_array<0, T>&, const t_array<N, T>&);

  // operator==_05
  template<int N, typename T>
  t_bool operator==(const t_array<N, T>&, const t_array<0, T>&);

  /////////////////////////////////////////////////////////////////////////////

  // operator!=_01
  template<int N, typename T>
  t_bool operator!=(const t_array<N, T>&, const t_array<N, T>&);

  // operator!=_02
  template<int N, typename T>
  t_bool operator!=(const t_array<N, T>&, const T (&)[N]);

  // operator!=_03
  template<int N, typename T>
  t_bool operator!=(const T (&)[N], const t_array<N, T>&);

  // operator!=_04
  template<int N, typename T>
  t_bool operator!=(const t_array<0, T>&, const t_array<N, T>&);

  // operator!=_05
  template<int N, typename T>
  t_bool operator!=(const t_array<N, T>&, const t_array<0, T>&);

  /////////////////////////////////////////////////////////////////////////////
}
}

#include "impl_/dainty_array_.icpp"

#endif
