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

  using impl_::INIT_LIST;
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
    t_array(impl_::t_init_none_);                          // ARRAY_1_2_2

    template<typename U, typename... Us>
    t_array(impl_::t_init_list_, U&& u, Us&&... us);       // ARRAY_1_2_3

    template<typename C>
    t_array(impl_::t_init_custom_, C&&);                   // ARRAY_1_2_4

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
    r_array_ init(impl_::t_init_list_, U&&, Us&&...);      // ARRAY_1_5_1

    template<typename C>
    r_array_ init(impl_::t_init_custom_, C&&);             // ARRAY_1_5_2

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

    t_array(t_n_max, impl_::t_init_none_);                     // ARRAY_2_2_2

    template<typename C>
    t_array(t_n_max, impl_::t_init_custom_, C&&);              // ARRAY_2_2_3

    template<typename U, typename... Us>
    t_array(impl_::t_init_list_, U&& u, Us&&... us);           // ARRAY_2_2_4

    template<typename U, typename... Us>
    t_array(t_n_max, impl_::t_init_list_, U&& u, Us&&... us);  // ARRAY_2_2_5

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
    r_array_ init(impl_::t_init_list_, U&&, Us&&...);          // ARRAY_2_5_1

    template<typename U, typename... Us>
    r_array_ init(t_n_max, impl_::t_init_list_, U&&, Us&&...); // ARRAY_2_5_2

    template<typename C>
    r_array_ init(impl_::t_init_custom_, C&&);                 // ARRAY_2_5_3

    template<typename C>
    r_array_ init(t_n_max, impl_::t_init_custom_, C&&);        // ARRAY_2_5_4

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

  // ARRAY_1_1
  template<int N, typename T>
  constexpr
  int t_array<N, T>::capacity() {
    return N;
  }

  // ARRAY_1_2_1
  template<int N, typename T>
  inline
  t_array<N, T>::t_array() {
    impl_::init_(N, ptr());
  }

  // ARRAY_1_2_2
  template<int N, typename T>
  constexpr
  t_array<N, T>::t_array(impl_::t_init_none_) {
  }

  // ARRAY_1_2_3
  template<int N, typename T>
  template<typename U, typename... Us>
  inline
  t_array<N, T>::t_array(impl_::t_init_list_, U&& u, Us&&... us) {
    impl_::init_(N, ptr(), INIT_LIST, f_cast<U>(u), f_cast<Us>(us)...);
  }

  // ARRAY_1_2_4
  template<int N, typename T>
  template<typename C>
  inline
  t_array<N, T>::t_array(impl_::t_init_custom_, C&& c) {
    impl_::init_(N, ptr(), INIT_CUSTOM, f_cast<C>(c));
  }

  // ARRAY_1_2_5
  template<int N, typename T>
  inline
  t_array<N, T>::t_array(R_array_ arr) {
    impl_::init_(N, ptr(), arr.Ptr());
  }

  // ARRAY_1_2_6
  template<int N, typename T>
  template<typename U>
  inline
  t_array<N, T>::t_array(const t_array<N, U>& arr) {
    impl_::init_(N, ptr(), arr.Ptr());
  }

  // ARRAY_1_2_7
  template<int N, typename T>
  template<typename U>
  inline
  t_array<N, T>::t_array(const U (&arr)[N]) {
    impl_::init_(N, ptr(), arr);
  }

  // ARRAY_1_3_1
  template<int N, typename T>
  inline
  typename t_array<N, T>::r_array_ t_array<N, T>::operator=(R_array_ arr) {
    impl_::copy_(set(), arr.Get());
    return *this;
  }

  // ARRAY_1_3_2
  template<int N, typename T>
  template<typename U>
  inline
  typename t_array<N, T>::r_array_ t_array<N, T>::operator=(const U (&arr)[N]) {
    impl_::copy_(set(), arr);
  }

  // ARRAY_1_3_3
  template<int N, typename T>
  template<typename U>
  inline
  typename t_array<N, T>::r_array_ t_array<N, T>::operator=(const t_array<N, U>& arr) {
    impl_::copy_(set(), arr.Get());
    return *this;
  }

  // ARRAY_1_4
  template<int N, typename T>
  inline
  t_array<N, T>::~t_array() {
    impl_::destruct(t_n_max{N}, ptr());
  }

  // ARRAY_1_5_1
  template<int N, typename T>
  template<typename U, typename... Us>
  inline
  typename t_array<N, T>::r_array_ t_array<N, T>::init(impl_::t_init_list_, U&& u, Us&&... us) {
    impl_::init_(N, ptr(), INIT_LIST, f_cast<U>(u), f_cast<Us>(us)...);
    return *this;
  }

  // ARRAY_1_5_2
  template<int N, typename T>
  template<typename C>
  inline
  typename t_array<N, T>::r_array_ t_array<N, T>::init(impl_::t_init_custom_, C&& custom) {
    impl_::init_(N, ptr(), INIT_CUSTOM, f_cast<C>(custom));
    return *this;
  }

  // ARRAY_1_5_3
  template<int N, typename T>
  template<typename U>
  inline
  typename t_array<N, T>::r_array_ t_array<N, T>::init(const U (&arr)[N]) {
    impl_::init_(N, ptr(), arr);
  }

  // ARRAY_1_5_4
  template<int N, typename T>
  template<typename U>
  inline
  typename t_array<N, T>::r_array_ t_array<N, T>::init(const t_array<N, U>& arr) {
    impl_::init_(N, ptr(), arr.Ptr());
    return *this;
  }

  // ARRAY_1_6
  template<int N, typename T>
  inline
  typename t_array<N, T>::r_array_ t_array<N, T>::destruct() {
    impl_::destruct(t_n_max{N}, ptr());
    return *this;
  }

  // ARRAY_1_7
  template<int N, typename T>
  inline
  typename t_array<N, T>::p_value_ t_array<N, T>::ptr(int ix) {
    return store_[ix].ptr();
  }

  // ARRAY_1_8
  template<int N, typename T>
  inline
  typename t_array<N, T>::P_value_ t_array<N, T>::Ptr(int ix) const {
    return store_[ix].Ptr();
  }

  // ARRAY_1_9
  template<int N, typename T>
  inline
  typename t_array<N, T>::r_value_ t_array<N, T>::set(int ix) {
    return store_[ix].ref();
  }

  // ARRAY_1_10
  template<int N, typename T>
  inline
  typename t_array<N, T>::R_value_ t_array<N, T>::Get(int ix) const {
    return store_[ix].Ref();
  }

  // ARRAY_1_11_1
  template<int N, typename T>
  inline
  typename t_array<N, T>::r_value_ t_array<N, T>::operator[](int ix) {
    return set(ix);
  }

  // ARRAY_1_11_2
  template<int N, typename T>
  inline
  typename t_array<N, T>::R_value_ t_array<N, T>::operator[](int ix) const {
    return Get(ix);
  }

  // ARRAY_1_12_1
  template<int N, typename T>
  inline
  t_array<N, T>::operator p_value_ () {
    return ptr();
  }

  // ARRAY_1_12_2
  template<int N, typename T>
  inline
  t_array<N, T>::operator P_value_ () const {
    return Ptr();
  }

  // ARRAY_1_13
  template<int N, typename T>
  inline
  typename t_array<N, T>::r_arr_ t_array<N, T>::set() {
    return *reinterpret_cast<p_arr_>(store_);
  }

  // ARRAY_1_14
  template<int N, typename T>
  inline
  typename t_array<N, T>::R_arr_ t_array<N, T>::Get() const {
    return *reinterpret_cast<P_arr_>(store_);
  }

  // ARRAY_1_15_1
  template<int N, typename T>
  inline
  t_bool t_array<N, T>::is_equal(R_array_ arr) const {
    return impl_::is_equal_(Get(), arr.Get());
  }

  // ARRAY_1_15_2
  template<int N, typename T>
  inline
  t_bool t_array<N, T>::is_equal(R_arr_ arr) const {
    return impl_::is_equal_(Get(), arr);
  }

  // ARRAY_1_16
  template<int N, typename T>
  inline
  typename t_array<N, T>::p_value_ t_array<N, T>::begin() {
    return ptr();
  }

  // ARRAY_1_18
  template<int N, typename T>
  inline
  typename t_array<N, T>::p_value_ t_array<N, T>::end() {
    return ptr() + N;
  }

  // ARRAY_1_17
  template<int N, typename T>
  inline
  typename t_array<N, T>::P_value_ t_array<N, T>::Begin() const {
    return Ptr();
  }

  // ARRAY_1_19
  template<int N, typename T>
  inline
  typename t_array<N, T>::P_value_ t_array<N, T>::End() const {
    return Ptr() + N;
  }

  // ARRAY_1_20
  template<int N, typename T>
  inline
  typename t_array<N, T>::p_value_ t_array<N, T>::rbegin() {
    return ptr() + N - 1;
  }

  // ARRAY_1_21
  template<int N, typename T>
  inline
  typename t_array<N, T>::P_value_ t_array<N, T>::RBegin() const {
    return Ptr() + N - 1;
  }

  // ARRAY_1_22
  template<int N, typename T>
  inline
  typename t_array<N, T>::p_value_ t_array<N, T>::rend() {
    return ptr() - 1;
  }

  // ARRAY_1_23
  template<int N, typename T>
  inline
  typename t_array<N, T>::P_value_ t_array<N, T>::REnd() const {
    return Ptr() - 1;
  }

  // ARRAY_1_24
  template<int N, typename T>
  template<typename F>
  inline
  t_void t_array<N, T>::each(F&& func) {
    impl_::each_(set(), f_cast<F>(func));
  }

  // ARRAY_1_25
  template<int N, typename T>
  template<typename F>
  inline
  t_void t_array<N, T>::Each(F&& func) const {
    impl_::Each_(Get(), f_cast<F>(func));
  }

  /////////////////////////////////////////////////////////////////////////////

  // ARRAY_2_1
  template<typename T>
  inline
  int t_array<0, T>::capacity() {
    return max_;
  }

  // ARRAY_2_2_1
  template<typename T>
  inline
  t_array<0, T>::t_array() {
  }

  // ARRAY_2_2_2
  template<typename T>
  inline
  t_array<0, T>::t_array(t_n_max max, impl_::t_init_none_)
      : max_(max), store_(impl_::alloc<T>(max_)) {
  }

  // ARRAY_2_2_3
  template<typename T>
  template<typename C>
  inline
  t_array<0, T>::t_array(t_n_max max, impl_::t_init_custom_, C&& custom)
      : max_(max), store_(impl_::alloc<T>(max_)) {
    impl_::init_(max_, store_, f_cast<C>(custom));
  }

  // ARRAY_2_2_4
  template<typename T>
  template<typename U, typename... Us>
  inline
  t_array<0, T>::t_array(impl_::t_init_list_, U&& u, Us&&... us)
      : max_(sizeof...(us) + 1), store_(impl_::alloc<T>(max_)) {
    impl_::init_(max_, store_, INIT_LIST, f_cast<U>(u), f_cast<Us>(us)...);
  }

  // ARRAY_2_2_5
  template<typename T>
  template<typename U, typename... Us>
  inline
  t_array<0, T>::t_array(t_n_max max, impl_::t_init_list_, U&& u, Us&&... us)
      : max_(impl_::max_of(sizeof...(us) + 1, max)), store_(impl_::alloc<T>(max_)) {
    impl_::init_(max_, store_, INIT_LIST, f_cast<U>(u), f_cast<Us>(us)...);
  }

  // ARRAY_2_2_6
  template<typename T>
  inline
  t_array<0, T>::t_array(R_array_ arr)
      : max_(arr.max_), store_(impl_::alloc<T>(max_)) {
    impl_::init_(max_, store_, arr.Ptr());
  }

  // ARRAY_2_2_7
  template<typename T>
  template<int N, typename U>
  inline
  t_array<0, T>::t_array(const t_array<N, U>& arr)
      : max_(arr.capacity()), store_(impl_::alloc<T>(max_)) {
    impl_::init_(max_, store_, arr.Ptr());
  }

  // ARRAY_2_2_8
  template<typename T>
  template<int N, typename U>
  inline
  t_array<0, T>::t_array(t_n_max, const t_array<N, U>& arr) {
    impl_::init_(max_, store_, arr.capacity(), arr.Ptr());
  }

  // ARRAY_2_2_9
  template<typename T>
  template<int N, typename U>
  inline
  t_array<0, T>::t_array(const U (&arr)[N])
      : max_(N), store_(impl_::alloc<T>(max_)) {
    impl_::init_(max_, store_, &arr[0]);
  }

  // ARRAY_2_2_10
  template<typename T>
  template<int N, typename U>
  inline
  t_array<0, T>::t_array(t_n_max, const U (&arr)[N]) {
    impl_::init_(max_, store_, N, &arr[0]);
  }

  // ARRAY_2_3_1
  template<typename T>
  inline
  typename t_array<0, T>::r_array_ t_array<0, T>::operator=(R_array_) {
    // what happens to existing objects - it depends?
    // if nothing existing - easy - init
    // if existing - same lenght     - but not initialed - easy - copy
    // if existing - not same lenght - but not initialed - easy - copy

        // TODO_11
    return *this;
  }

  // ARRAY_2_3_2
  template<typename T>
  template<int N, typename U>
  inline
  typename t_array<0, T>::r_array_ t_array<0, T>::operator=(const U (&)[N]) {
        // TODO_10
    return *this;
  }

  // ARRAY_2_3_3
  template<typename T>
  template<int N, typename U>
  inline
  typename t_array<0, T>::r_array_ t_array<0, T>::operator=(const t_array<N, U>&) {
        // TODO_9
    return *this;
  }

  // ARRAY_2_4
  template<typename T>
  inline
  t_array<0, T>::~t_array() {
    impl_::destruct(t_n_max{max_}, store_);
  }

  // ARRAY_2_5_1
  template<typename T>
  template<typename U, typename... Us>
  inline
  typename t_array<0, T>::r_array_
      t_array<0, T>::init(impl_::t_init_list_, U&&, Us&&...) {
        // TODO_8
  }

  // ARRAY_2_5_2
  template<typename T>
  template<typename U, typename... Us>
  inline
  typename t_array<0, T>::r_array_
      t_array<0, T>::init(t_n_max, impl_::t_init_list_, U&&, Us&&...) {
        // TODO_7
  }

  // ARRAY_2_5_3
  template<typename T>
  template<typename C>
  inline
  typename t_array<0, T>::r_array_
      t_array<0, T>::init(impl_::t_init_custom_, C&&) {
        // TODO_6
  }

  // ARRAY_2_5_4
  template<typename T>
  template<typename C>
  inline
  typename t_array<0, T>::r_array_
      t_array<0, T>::init(t_n_max, impl_::t_init_custom_, C&&) {
        // TODO_5
  }

  // ARRAY_2_5_5
  template<typename T>
  template<int N, typename U>
  inline
  typename t_array<0, T>::r_array_
      t_array<0, T>::init(const U (&)[N]) {
        // TODO_4
  }

  // ARRAY_2_5_6
  template<typename T>
  template<int N, typename U>
  inline
  typename t_array<0, T>::r_array_
      t_array<0, T>::init(t_n_max, const U (&)[N]) {
        // TODO_3
  }

  // ARRAY_2_5_7
  template<typename T>
  template<int N, typename U>
  inline
  typename t_array<0, T>::r_array_
      t_array<0, T>::init(const t_array<N, U>&) {
        // TODO_2
  }

  // ARRAY_2_5_8
  template<typename T>
  template<int N, typename U>
  inline
  typename t_array<0, T>::r_array_
      t_array<0, T>::init(t_n_max, const t_array<N, U>&) {
        // TODO_1
  }

  // ARRAY_2_6
  template<typename T>
  inline
  typename t_array<0, T>::r_array_ t_array<0, T>::destruct() {
    impl_::destruct(t_n_max{max_}, store_);
  }

  // ARRAY_2_7
  template<typename T>
  inline
  typename t_array<0, T>::p_value_ t_array<0, T>::ptr(int ix) {
    return store_ + ix;
  }

  // ARRAY_2_8
  template<typename T>
  inline
  typename t_array<0, T>::P_value_ t_array<0, T>::Ptr(int ix) const {
    return store_ + ix;
  }

  // ARRAY_2_9
  template<typename T>
  inline
  typename t_array<0, T>::r_value_ t_array<0, T>::set(int ix) {
    return *ptr(ix);
  }

  // ARRAY_2_10
  template<typename T>
  inline
  typename t_array<0, T>::R_value_ t_array<0, T>::Get(int ix) const {
    return *Ptr(ix);
  }

  // ARRAY_2_11_1
  template<typename T>
  inline
  typename t_array<0, T>::r_value_ t_array<0, T>::operator[](int ix) {
    return set(ix);
  }

  // ARRAY_2_11_2
  template<typename T>
  inline
  typename t_array<0, T>::R_value_ t_array<0, T>::operator[](int ix) const {
    return Get(ix);
  }

  // ARRAY_2_12_1
  template<typename T>
  inline
  t_array<0, T>::operator p_value_ () {
    return ptr();
  }

  // ARRAY_2_12_2
  template<typename T>
  inline
  t_array<0, T>::operator P_value_ () const {
    return Ptr();
  }

  // ARRAY_2_12_3
  template<typename T>
  inline
  t_array<0, T>::operator t_bool () const {
    return max_;
  }

  // ARRAY_2_13
  template<typename T>
  inline
  typename t_array<0, T>::r_arr_ t_array<0, T>::set() {
    return *reinterpret_cast<p_arr_>(store_);
  }

  // ARRAY_2_14
  template<typename T>
  inline
  typename t_array<0, T>::R_arr_ t_array<0, T>::Get() const {
    return *reinterpret_cast<P_arr_>(store_);
  }

  // ARRAY_2_15_1
  template<typename T>
  template<int N>
  inline
  t_bool t_array<0, T>::is_equal(const t_array<N, T>& arr) const {
    return impl_::is_equal_(max_, store_, arr.capacity(), arr.Ptr());
  }

  // ARRAY_2_15_2
  template<typename T>
  template<int N>
  inline
  t_bool t_array<0, T>::is_equal(const T (&arr)[N]) const {
    return impl_::is_equal_(max_, store_, N, &arr[0]);
  }

  // ARRAY_2_16
  template<typename T>
  inline
  typename t_array<0, T>::p_value_ t_array<0, T>::begin() {
    return ptr();
  }

  // ARRAY_2_17
  template<typename T>
  inline
  typename t_array<0, T>::P_value_ t_array<0, T>::Begin() const {
    return Ptr();
  }

  // ARRAY_2_18
  template<typename T>
  inline
  typename t_array<0, T>::p_value_ t_array<0, T>::end() {
    return ptr() + max_;
  }

  // ARRAY_2_19
  template<typename T>
  inline
  typename t_array<0, T>::P_value_ t_array<0, T>::End() const {
    return Ptr() + max_;
  }

  // ARRAY_2_20
  template<typename T>
  inline
  typename t_array<0, T>::p_value_ t_array<0, T>::rbegin() {
    return ptr() + max_ - 1;
  }

  // ARRAY_2_21
  template<typename T>
  inline
  typename t_array<0, T>::P_value_ t_array<0, T>::RBegin() const {
    return Ptr() + max_ - 1;
  }

  // ARRAY_2_22
  template<typename T>
  inline
  typename t_array<0, T>::p_value_ t_array<0, T>::rend() {
    return ptr() - 1;
  }

  // ARRAY_2_23
  template<typename T>
  inline
  typename t_array<0, T>::P_value_ t_array<0, T>::REnd() const {
    return Ptr() - 1;
  }

  // ARRAY_2_24
  template<typename T>
  template<typename F>
  inline
  t_void t_array<0, T>::each(F&& func) {
    impl_::Each_(max_, store_, f_cast<F>(func));
  }

  // ARRAY_2_25
  template<typename T>
  template<typename F>
  inline
  t_void t_array<0, T>::Each(F&& func) const {
    impl_::Each_(max_, store_, f_cast<F>(func));
  }

  /////////////////////////////////////////////////////////////////////////////

  // MAKE_ARRAY_01
  template<typename T, typename... Ts>
  inline
  t_array<sizeof...(Ts) + 1, T>
      make_array(impl_::t_init_list_, T&& t, Ts&&... ts) {
    return t_array<sizeof...(Ts) + 1, T>(INIT_LIST, f_cast<T>(t), f_cast<Ts>(ts)...);
  }

  // MAKE_ARRAY_02
  template<int N, typename T, typename... Ts>
  inline
  t_array<N, T> make_array(impl_::t_init_list_, T&& t, Ts&&... ts) {
    return t_array<N, T>(INIT_LIST, f_cast<T>(t), f_cast<Ts>(ts)...);
  }

  // MAKE_ARRAY_03
  template<int N, typename T>
  inline
  t_array<N, T> make_array(T (&arr)[N]) {
    return t_array<N, T>(arr);
  }

  // MAKE_ARRAY_04
  template<int N, typename T>
  inline
  t_array<N, T> make_array(const T (&arr)[N]) {
    return t_array<N, T>(arr);
  }

  // MAKE_ARRAY_05
  template<int N, typename T>
  inline
  t_array<N, T> make_array() {
    return t_array<N, T>();
  }

  // MAKE_ARRAY_06
  template<int N, typename T>
  inline
  t_array<N, T> make_array(impl_::t_init_none_) {
    return t_array<N, T>(INIT_NONE);
  }

  // MAKE_ARRAY_07
  template<int N, typename T, typename C>
  inline
  t_array<N, T> make_array(impl_::t_init_custom_, C&& custom) {
    return t_array<N, T>(f_cast<C>(custom));
  }

  /////////////////////////////////////////////////////////////////////////////

  // operator==_01
  template<int N, typename T>
  inline
  t_bool operator==(const t_array<N, T>& lh, const t_array<N, T>& rh) {
    return lh.is_equal(rh);
  }

  // operator==_02
  template<int N, typename T>
  inline
  t_bool operator==(const t_array<N, T>& lh, const T (&rh)[N]) {
    return lh.is_equal(rh);
  }

  // operator==_03
  template<int N, typename T>
  inline
  t_bool operator==(const T (&lh)[N], const t_array<N, T>& rh) {
    return rh.is_equal(lh);
  }

  // operator==_04
  template<int N, typename T>
  inline
  t_bool operator==(const t_array<0, T>& lh, const t_array<N, T>& rh) {
    return lh.is_equal(rh);
  }

  // operator==_05
  template<int N, typename T>
  inline
  t_bool operator==(const t_array<N, T>& lh, const t_array<0, T>& rh) {
    return rh.is_equal(lh);
  }

  /////////////////////////////////////////////////////////////////////////////

  // operator!=_01
  template<int N, typename T>
  inline
  t_bool operator!=(const t_array<N, T>& lh, const t_array<N, T>& rh) {
    return !lh.is_equal(rh);
  }

  // operator!=_02
  template<int N, typename T>
  inline
  t_bool operator!=(const t_array<N, T>& lh, const T (&rh)[N]) {
    return !lh.is_equal(rh);
  }

  // operator!=_03
  template<int N, typename T>
  inline
  t_bool operator!=(const T (&lh)[N], const t_array<N, T>& rh) {
    return !rh.is_equal(lh);
  }

  // operator!=_04
  template<int N, typename T>
  inline
  t_bool operator!=(const t_array<0, T>& lh, const t_array<N, T>& rh) {
    return !lh.is_equal(rh);
  }

  // operator!=_05
  template<int N, typename T>
  inline
  t_bool operator!=(const t_array<N, T>& lh, const t_array<0, T>& rh) {
    return !rh.is_equal(lh);
  }

  /////////////////////////////////////////////////////////////////////////////
}
}
#endif