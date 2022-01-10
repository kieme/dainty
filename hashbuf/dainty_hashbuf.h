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

#ifndef _DAINTY_HASHBUF_H_
#define _DAINTY_HASHBUF_H_

#include "impl_/dainty_hashbuf_impl_.h"

namespace dainty
{
namespace hashbuf
{
  using impl_::t_bool;
  using impl_::t_reuse;
  using impl_::t_must_add;
  using impl_::t_void;
  using impl_::t_double;
  using impl_::t_u1;
  using impl_::t_n;
  using impl_::t_ix;
  using impl_::t_n_max;
  using impl_::t_n_bytes;
  using impl_::t_hash_id;

  /////////////////////////////////////////////////////////////////////////////

  using impl_::REUSE;
  using impl_::NO_REUSE;

  using impl_::MUST_ADD;
  using impl_::FIND_OR_ADD;

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A> class t_Itr;

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A>
  class t_itr {
    using t_Itr_    = impl_::t_of<t_Itr<K, V, N, A>>;
    using r_itr_    = impl_::r_of<t_itr>;
    using R_itr_    = impl_::R_of<t_itr>;
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using t_ident_  = typename t_traits_::t_ident;
    using p_node_   = typename t_traits_::p_node;
    using q_node_   = typename t_traits_::q_node;
    using p_val_    = typename t_traits_::p_val;
    using P_val_    = typename t_traits_::P_val;
    using R_key_    = typename t_traits_::R_key;
    using r_value_  = typename t_traits_::r_value;
    using R_value_  = typename t_traits_::R_value;
    using p_value_  = typename t_traits_::p_value;
    using P_value_  = typename t_traits_::P_value;

  public:
    using t_key     = typename t_traits_::t_key;
    using t_value   = typename t_traits_::t_value;

    t_itr() = default;
    t_itr(t_ident_);             // itr_1_01

    R_key_    Key  () const;     // itr_1_02
    t_hash_id hash () const;     // itr_1_03

    t_bool   is_valid() const;   // itr_1_04
    operator t_bool  () const;   // itr_1_05

    r_value_ ref();              // itr_1_06
    R_value_ Ref() const;        // itr_1_07

    p_value_ ptr();              // itr_1_08
    P_value_ Ptr() const;        // itr_1_09

    p_value_ value();            // itr_1_10
    P_value_ Value() const;      // itr_1_11

    r_itr_   next();             // itr_1_12
    t_Itr_   Next();             // itr_1_13

    r_itr_   next_same_key();    // itr_1_14
    t_Itr_   Next_same_key();    // itr_1_15

    r_itr_   next_same_hash();   // itr_1_16
    t_Itr_   Next_same_hash();   // itr_1_17

    r_itr_   next_same_slot();   // itr_1_18
    t_Itr_   Next_same_slot();   // itr_1_19

    r_itr_   operator++();       // itr_1_20
    t_itr    operator++(int);    // itr_1_21

    p_value_ operator->();       // itr_1_22
    P_value_ operator->() const; // itr_1_23

    r_value_ operator* ();       // itr_1_24
    R_value_ operator* () const; // itr_1_25

  private:
    template<template<typename, typename, int, typename> typename I,
             typename K1, typename V1, int N1, typename A1>
    friend constexpr impl_::t_ident_<K1, V1, N1, A1>&
                       impl_::acc_(I<K1, V1, N1, A1>&);

    constexpr r_itr_ self_()       { return *this; }
    constexpr R_itr_ Self_() const { return *this; }

    t_ident_ ident_;
  };

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, int N, typename A>
  class t_itr<K, t_void, N, A> {
    using t_Itr_    = impl_::t_of<t_Itr<K, t_void, N, A>>;
    using r_itr_    = impl_::r_of<t_itr>;
    using R_itr_    = impl_::R_of<t_itr>;
    using t_traits_ = impl_::t_traits_<K, t_void, N, A>;
    using t_ident_  = typename t_traits_::t_ident;
    using p_node_   = typename t_traits_::p_node;
    using q_node_   = typename t_traits_::q_node;
    using p_val_    = typename t_traits_::p_val;
    using P_val_    = typename t_traits_::P_val;
    using R_key_    = typename t_traits_::R_key;

  public:
    using t_key     = typename t_traits_::t_key;

    t_itr() = default;
    t_itr(t_ident_);              // itr_2_01

    R_key_    Key  () const;      // itr_2_02
    t_hash_id hash () const;      // itr_2_03

    t_bool    is_valid() const;   // itr_2_04
    operator  t_bool  () const;   // itr_2_05

    r_itr_    next();             // itr_2_06
    t_Itr_    Next();             // itr_2_07

    r_itr_    next_same_key();    // itr_2_08
    t_Itr_    Next_same_key();    // itr_2_09

    r_itr_    next_same_hash();   // itr_2_10
    t_Itr_    Next_same_hash();   // itr_2_11

    r_itr_    next_same_slot();   // itr_2_12
    t_Itr_    Next_same_slot();   // itr_2_13

    r_itr_    operator++();       // itr_2_14
    t_itr     operator++(int);    // itr_2_15

  private:
    template<template<typename, typename, int, typename> typename I,
             typename K1, typename V1, int N1, typename A1>
    friend constexpr impl_::t_ident_<K1, V1, N1, A1>&
                       impl_::acc_(I<K1, V1, N1, A1>&);

    constexpr r_itr_ self_()       { return *this; }
    constexpr R_itr_ Self_() const { return *this; }

    t_ident_ ident_;
  };

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A>
  class t_Itr {
    using t_itr_    = impl_::t_of<t_itr<K, V, N, A>>;
    using r_Itr_    = impl_::r_of<t_Itr>;
    using R_Itr_    = impl_::R_of<t_Itr>;
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using t_node_   = typename t_traits_::t_node;
    using P_node_   = typename t_traits_::P_node;
    using Q_node_   = typename t_traits_::Q_node;
    using P_val_    = typename t_traits_::P_val;
    using t_Ident_  = typename t_traits_::t_Ident;
    using t_ident_  = typename t_traits_::t_ident;
    using R_key_    = typename t_traits_::R_key;
    using R_value_  = typename t_traits_::R_value;
    using P_value_  = typename t_traits_::P_value;

  public:
    using t_key     = typename t_traits_::t_key;
    using t_value   = typename t_traits_::t_value;

    t_Itr() = default;
    t_Itr(t_itr_);                // Itr_1_01
    t_Itr(t_ident_);              // Itr_1_02
    t_Itr(t_Ident_);              // Itr_1_03

    R_value_  ref() const;        // Itr_1_04
    R_value_  Ref() const;        // Itr_1_05

    P_value_  ptr() const;        // Itr_1_06
    P_value_  Ptr() const;        // Itr_1_07

    R_key_    Key  () const;      // Itr_1_08
    P_value_  value() const;      // Itr_1_09
    P_value_  Value() const;      // Itr_1_10
    t_hash_id hash () const;      // Itr_1_11

    t_bool    is_valid() const;   // Itr_1_12
    operator  t_bool  () const;   // Itr_1_13

    r_Itr_    Next();             // Itr_1_14
    r_Itr_    Next_same_key();    // Itr_1_15
    r_Itr_    Next_same_hash();   // Itr_1_16
    r_Itr_    Next_same_slot();   // Itr_1_17

    t_Itr     operator++(int);    // Itr_1_18
    r_Itr_    operator++();       // Itr_1_19

    P_value_  operator->() const; // Itr_1_20
    R_value_  operator* () const; // Itr_1_21

  private:
    template<template<typename, typename, int, typename> typename I,
             typename K1, typename V1, int N1, typename A1>
    friend constexpr impl_::t_Ident_<K1, V1, N1, A1>&
                       impl_::Acc_(I<K1, V1, N1, A1>&);

    constexpr r_Itr_ self_()       { return *this; }
    constexpr R_Itr_ Self_() const { return *this; }

    t_Ident_ ident_;
  };

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, int N, typename A>
  class t_Itr<K, t_void, N, A> {
    using t_itr_    = impl_::t_of<t_itr<K, t_void, N, A>>;
    using r_Itr_    = impl_::r_of<t_Itr>;
    using R_Itr_    = impl_::R_of<t_Itr>;
    using t_traits_ = impl_::t_traits_<K, t_void, N, A>;
    using t_node_   = typename t_traits_::t_node;
    using P_node_   = typename t_traits_::P_node;
    using Q_node_   = typename t_traits_::Q_node;
    using P_val_    = typename t_traits_::P_val;
    using R_key_    = typename t_traits_::R_key;
    using t_Ident_  = typename t_traits_::t_Ident;
    using t_ident_  = typename t_traits_::t_ident;

  public:
    using t_key     = typename t_traits_::t_key;

    t_Itr() = default;
    t_Itr(t_itr_);                // Itr_2_01
    t_Itr(t_ident_);              // Itr_2_02
    t_Itr(t_Ident_);              // Itr_2_03

    R_key_    Key  () const;      // Itr_2_04
    t_hash_id hash () const;      // Itr_2_05

    t_bool    is_valid() const;   // Itr_2_06
    operator  t_bool  () const;   // Itr_2_07

    r_Itr_    Next();             // Itr_2_08
    r_Itr_    Next_same_key();    // Itr_2_09
    r_Itr_    Next_same_hash();   // Itr_2_10
    r_Itr_    Next_same_slot();   // Itr_2_11

    t_Itr     operator++(int);    // Itr_2_12
    r_Itr_    operator++();       // Itr_2_13

  private:
    template<template<typename, typename, int, typename> typename I,
             typename K1, typename V1, int N1, typename A1>
    friend constexpr impl_::t_Ident_<K1, V1, N1, A1>&
                       impl_::Acc_(I<K1, V1, N1, A1>&);

    constexpr r_Itr_ self_()       { return *this; }
    constexpr R_Itr_ Self_() const { return *this; }

    t_Ident_ ident_;
  };

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A = impl_::t_alloc>
  class t_hashbuf {
    using t_itr_     = impl_::t_of<hashbuf::t_itr<K, V, N, A>>;
    using t_Itr_     = impl_::t_of<hashbuf::t_Itr<K, V, N, A>>;
    using R_Itr_     = impl_::R_of<t_Itr_>;
    using r_hashbuf_ = impl_::r_of<t_hashbuf>;
    using R_hashbuf_ = impl_::R_of<t_hashbuf>;
    using x_hashbuf_ = impl_::x_of<t_hashbuf>;
    using t_cache_   = impl_::t_alloc_cache_<K, V, N, A>;
    using t_traits_  = impl_::t_traits_<K, V, N, A>;
    using t_ident_   = typename t_traits_::t_ident;
    using t_Ident_   = typename t_traits_::t_Ident;
    using t_node_    = typename t_traits_::t_node;
    using p_node_    = typename t_traits_::p_node;
    using P_node_    = typename t_traits_::P_node;
    using q_node_    = typename t_traits_::q_node;
    using Q_node_    = typename t_traits_::Q_node;
    using t_val_     = typename t_traits_::t_val;
    using p_val_     = typename t_traits_::p_val;
    using P_val_     = typename t_traits_::P_val;
    using t_alloc_   = typename t_traits_::t_alloc;
    using r_alloc_   = typename t_traits_::r_alloc;
    using R_alloc_   = typename t_traits_::R_alloc;
    using x_alloc_   = typename t_traits_::x_alloc; // MUST be used
    using t_out_     = typename t_traits_::t_out;
    using r_out_     = typename t_traits_::r_out;
    using t_key_     = typename t_traits_::t_key;
    using R_key_     = typename t_traits_::R_key;
    using x_key_     = typename t_traits_::x_key;
    using t_stats_   = typename t_traits_::t_stats;

  public:
    using t_stats     = t_stats_;
    using t_out       = t_out_;
    using t_key       = t_key_;
    using t_itr       = t_itr_;
    using t_Itr       = t_Itr_;
    using t_alloc     = t_alloc_;

    // callback functions

    using r_hash_func = t_hash_id (&)(R_key_)         noexcept;
    using p_out_func  = r_out_    (*)(r_out_, R_Itr_) noexcept;

    // constructors

    t_hashbuf(t_n_max slots, r_hash_func);                           // H_1_01
    template<int N1, typename A1>
    t_hashbuf(t_n_max slots, const t_hashbuf<K, V, N1, A1>&);        // H_1_02

    t_hashbuf(R_hashbuf_);                                           // H_1_03
    t_hashbuf(x_hashbuf_);                                           // H_1_04

    template<int N1, typename A1>
    t_hashbuf(const t_hashbuf<K, V, N1, A1>&);                       // H_1_05

    // assignment

    r_hashbuf_ operator=(R_hashbuf_);                                // H_2_01
    r_hashbuf_ operator=(x_hashbuf_);                                // H_2_02

    template<int N1, typename A1>
    r_hashbuf_ operator=(const t_hashbuf<K, V, N1, A1>&);            // H_2_03

    // destructor

   ~t_hashbuf();                                                     // H_3_01

    // modifiers

    t_itr add_unique(R_key_, t_must_add = MUST_ADD);                 // H_4_01
    t_itr add_unique(x_key_, t_must_add = MUST_ADD);                 // H_4_02

    t_itr add(R_key_);                                               // H_5_01
    t_itr add(x_key_);                                               // H_5_02

                         t_bool del(R_key_, t_reuse = NO_REUSE);     // H_6_01
    template<typename F> t_bool del(R_key_, t_reuse, F&&);           // H_6_02

                         t_n    del_all(R_key_, t_reuse = NO_REUSE); // H_7_01
    template<typename F> t_n    del_all(R_key_, t_reuse, F&&);       // H_7_02

                         t_void del(t_itr, t_reuse = NO_REUSE);      // H_8_01
    template<typename F> t_void del(t_itr, t_reuse, F&&);            // H_8_02

    template<typename P> t_n    del_if(P&&, t_reuse = NO_REUSE);     // H_9_01
    template<typename P,
             typename F> t_n    del_if(P&&, t_reuse, F&&);           // H_9_02

                         t_void clear(t_reuse = NO_REUSE);           // H_10_01
    template<typename F> t_void clear(t_reuse, F&&);                 // H_10_02

    // info and stats

    r_alloc_  alloc   ();                                            // H_11_01
    R_alloc_  alloc   () const;                                      // H_11_02
    t_bool    is_empty() const;                                      // H_12
    t_n       size    () const;                                      // H_13
    t_n_max   slots   () const;                                      // H_14
    t_n_bytes bytes   () const;                                      // H_15
    t_stats   stats   () const;                                      // H_16
    t_void    show    (r_out_, t_bool detail = true,
                               p_out_func = nullptr) const;          // H_17

    // lookup

    t_itr find(R_key_);                                              // H_18
    t_Itr Find(R_key_) const;                                        // H_19

    t_itr find_hash(t_hash_id);                                      // H_20
    t_Itr Find_hash(t_hash_id) const;                                // H_21

    // rebuild internals

    t_bool rehash  (t_n_max slots);                                  // H_22
    t_void compact (t_reuse);                                        // H_23
    t_void optimize();                                               // H_24
    t_void gcollect();                                               // H_25

    // iteration

    t_itr begin();                                                   // H_26
    t_Itr Begin() const;                                             // H_27

    t_itr end  ();                                                   // H_28
    t_Itr End  () const;                                             // H_29

    // each iterator
    template<typename F> t_void each(F&&);                           // H_30
    template<typename F> t_void Each(F&&) const;                     // H_31

    template<typename P, typename F> t_n each_if(P&&, F&&);          // H_32
    template<typename P, typename F> t_n Each_if(P&&, F&&) const;    // H_33

  private:
    constexpr static t_n SIZE_OF_ = types::SIZEOF<t_hashbuf>;

    template<typename K1, typename V1, int N1, typename A1>
    friend class t_hashbuf;

    constexpr r_hashbuf_ self_()       { return *this; }
    constexpr R_hashbuf_ Self_() const { return *this; }

    t_cache_    cache_;
    t_n_max     max_;
    r_hash_func hash_;
    t_ident_    head_;
    t_n         size_  = 0;
    q_node_     slots_ = nullptr;
  };

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, typename A>
  class t_hashbuf<K, V, 0, A>;  //makes no sense

  /////////////////////////////////////////////////////////////////////////////

  // H_1_01
  template<typename K, typename V, int N, typename A>
  inline
  t_hashbuf<K, V, N, A>::t_hashbuf(t_n_max slots, r_hash_func hash)
      : max_(impl_::next_prime(slots)), hash_(hash),
        slots_(cache_.acquire_fill(max_)) {
  }

  // H_1_02
  template<typename K, typename V, int N, typename A>
  template<int N1, typename A1>
  inline
  t_hashbuf<K, V, N, A>::t_hashbuf(t_n_max slots,
                                   const t_hashbuf<K, V, N1, A1>& value)
      : max_(impl_::next_prime(slots)), hash_(value.hash_),
        slots_(cache_.acquire_fill(max_)) {
    impl_::copy_(max_, head_, slots_, cache_, hash_, size_,
                 impl_::Const_(value.head_));
  }

  // H_1_03
  template<typename K, typename V, int N, typename A>
  inline
  t_hashbuf<K, V, N, A>::t_hashbuf(R_hashbuf_ value)
      : max_(value.max_), hash_(value.hash_),
        slots_(cache_.acquire_fill(max_)) {
    impl_::copy_(max_, head_, slots_, cache_, hash_, size_,
                 impl_::Const_(value.head_));
  }

  // H_1_04
  template<typename K, typename V, int N, typename A>
  inline
  t_hashbuf<K, V, N, A>::t_hashbuf(x_hashbuf_ value)
      : max_  (impl_::reset(value.max_,   0U)),
        hash_ (value.hash_),
        head_ (impl_::reset(value.head_)),
        size_ (impl_::reset(value.size_,  0U)),
        slots_(impl_::reset(value.slots_, nullptr)) {
  }

  // H_1_05
  template<typename K, typename V, int N, typename A>
  template<int N1, typename A1>
  inline
  t_hashbuf<K, V, N, A>::t_hashbuf(const t_hashbuf<K, V, N1, A1>& value)
      : max_(value.max_), hash_(value.hash_),
        slots_(cache_.template acquire<p_node_>(max_)) {
    impl_::copy_(max_, head_, slots_, cache_, hash_, size_,
                 impl_::Const_(value.head_));
  }

  /////////////////////////////////////////////////////////////////////////////

  // H_3_0
  template<typename K, typename V, int N, typename A>
  inline
  t_hashbuf<K, V, N, A>::~t_hashbuf() {
    clear(NO_REUSE);
    cache_.release(slots_);
  }

  /////////////////////////////////////////////////////////////////////////////

  // H_2_01
  template<typename K, typename V, int N, typename A>
  inline
  typename t_hashbuf<K, V, N, A>::r_hashbuf_
      t_hashbuf<K, V, N, A>::operator=(R_hashbuf_ value) {
    if (&value != this) {
      clear(REUSE);
      impl_::copy_(max_, head_, slots_, cache_, hash_, size_,
                   impl_::Const_(value.head_));
    }
    return self_();
  }

  // H_2_02
  template<typename K, typename V, int N, typename A>
  inline
  typename t_hashbuf<K, V, N, A>::r_hashbuf_
      t_hashbuf<K, V, N, A>::operator=(x_hashbuf_ value) {
    if (&value != this) {
      cache_.release(slots_);
      hash_  = value.hash_;
      max_   = impl_::reset(value.max_, 0U);
      size_  = impl_::reset(value.size_, 0U);
      slots_ = impl_::reset(value.slots_, nullptr); // TODO
      head_  = impl_::reset(value.head_);
    }
    return self_();
  }

  // H_2_03
  template<typename K, typename V, int N, typename A>
  template<int N1, typename A1>
  inline
  typename t_hashbuf<K, V, N, A>::r_hashbuf_
      t_hashbuf<K, V, N, A>::operator=(const t_hashbuf<K, V, N1, A1>& value) {
    clear(REUSE);
    impl_::copy_(max_, head_, slots_, cache_, hash_, size_,
                 impl_::Const_(value.head_));
    return self_();
  }

  /////////////////////////////////////////////////////////////////////////////

  // H_4_01
  template<typename K, typename V, int N, typename A>
  inline
  typename t_hashbuf<K, V, N, A>::t_itr
      t_hashbuf<K, V, N, A>::add_unique(R_key_ key, t_must_add must_add) {
    return add_unique_cp_(head_, slots_, hash_, cache_, size_, max_, key, must_add);
  }

  // H_4_02
  template<typename K, typename V, int N, typename A>
  inline
  typename t_hashbuf<K, V, N, A>::t_itr
      t_hashbuf<K, V, N, A>::add_unique(x_key_ key, t_must_add must_add) {
    return add_unique_mv_(head_, slots_, hash_, cache_, size_, max_,
                          std::move(key), must_add);
  }

  // H_5_01
  template<typename K, typename V, int N, typename A>
  inline
  typename t_hashbuf<K, V, N, A>::t_itr
      t_hashbuf<K, V, N, A>::add(R_key_ key) {
    return impl_::add_cp_(head_, cache_, hash_, key, size_, max_, slots_);
  }

 // H_5_02
  template<typename K, typename V, int N, typename A>
  inline
  typename t_hashbuf<K, V, N, A>::t_itr
      t_hashbuf<K, V, N, A>::add(x_key_ key) {
    return impl_::add_mv_(head_, cache_, hash_, std::move(key), size_, max_,
                          slots_);
  }

  /////////////////////////////////////////////////////////////////////////////

  // H_6_01
  template<typename K, typename V, int N, typename A>
  inline
  t_bool t_hashbuf<K, V, N, A>::del(R_key_ key, t_reuse reuse) {
    return impl_::del_(head_, cache_, hash_, slots_, size_, max_, key, reuse);
  }

  // H_6_02
  template<typename K, typename V, int N, typename A>
  template<typename F>
  inline
  t_bool t_hashbuf<K, V, N, A>::del(R_key_ key, t_reuse reuse, F&& func) {
    return impl_::del_<t_itr>(head_, cache_, hash_, slots_, size_, max_, key,
                              reuse, std::forward<F>(func));
  }

  // H_7_01
  template<typename K, typename V, int N, typename A>
  inline
  t_n t_hashbuf<K, V, N, A>::del_all(R_key_ key, t_reuse reuse) {
    // TODO_3
    return 0;
  }

  // H_7_02
  template<typename K, typename V, int N, typename A>
  template<typename F>
  inline
  t_n t_hashbuf<K, V, N, A>::del_all(R_key_ key, t_reuse reuse, F&& func) {
    // TODO_4
    return 0;
  }

  // H_8_01
  template<typename K, typename V, int N, typename A>
  inline
  t_void t_hashbuf<K, V, N, A>::del(t_itr itr, t_reuse reuse) {
    impl_::del_(head_, cache_, size_, impl_::acc_(itr), reuse);
  }

  // H_8_02
  template<typename K, typename V, int N, typename A>
  template<typename F>
  inline
  t_void t_hashbuf<K, V, N, A>::del(t_itr itr, t_reuse reuse, F&& func) {
    impl_::del_<t_itr>(head_, cache_, size_, impl_::acc_(itr), reuse,
                       std::forward<F>(func));
  }

  /////////////////////////////////////////////////////////////////////////////

  // H_9_01
  template<typename K, typename V, int N, typename A>
  template<typename P>
  inline
  t_n t_hashbuf<K, V, N, A>::del_if(P&& pred, t_reuse reuse) {
    return impl_::del_if_<t_Itr>(head_, cache_, size_, pred, reuse);
  }

  // H_9_02
  template<typename K, typename V, int N, typename A>
  template<typename P, typename F>
  inline
  t_n t_hashbuf<K, V, N, A>::del_if(P&& pred, t_reuse reuse, F&& func) {
    return impl_::del_if_<t_Itr, t_itr>(head_, cache_, size_, pred, reuse,
                                        std::forward<F>(func));
  }

  /////////////////////////////////////////////////////////////////////////////

  // H_10_01
  template<typename K, typename V, int N, typename A>
  inline
  t_void t_hashbuf<K, V, N, A>::clear(t_reuse reuse) {
    impl_::clear_(head_, cache_, size_, reuse);
  }

  // H_10_02
  template<typename K, typename V, int N, typename A>
  template<typename F>
  inline
  t_void t_hashbuf<K, V, N, A>::clear(t_reuse reuse, F&& func) {
    impl_::clear_<t_itr>(head_, cache_, size_, reuse, std::forward<F>(func));
  }

  /////////////////////////////////////////////////////////////////////////////

  // H_11_01
  template<typename K, typename V, int N, typename A>
  inline
  typename t_hashbuf<K, V, N, A>::r_alloc_ t_hashbuf<K, V, N, A>::alloc() {
    return cache_.alloc;
  }

  // H_11_02
  template<typename K, typename V, int N, typename A>
  inline
  typename t_hashbuf<K, V, N, A>::R_alloc_
      t_hashbuf<K, V, N, A>::alloc() const {
    return cache_.alloc;
  }

  // H_12
  template<typename K, typename V, int N, typename A>
  inline
  t_bool t_hashbuf<K, V, N, A>::is_empty() const {
    return !size_;
  }

  // H_13
  template<typename K, typename V, int N, typename A>
  inline
  t_n t_hashbuf<K, V, N, A>::size() const {
    return size_;
  }

  // H_14
  template<typename K, typename V, int N, typename A>
  inline
  t_n_max t_hashbuf<K, V, N, A>::slots() const {
    return max_;
  }

  // H_15
  template<typename K, typename V, int N, typename A>
  inline
  t_n_bytes t_hashbuf<K, V, N, A>::bytes() const {
    return impl_::bytes_(impl_::Const_(head_), max_, cache_, SIZE_OF_);
  }

  /////////////////////////////////////////////////////////////////////////////

  // H_18
  template<typename K, typename V, int N, typename A>
  inline
  typename t_hashbuf<K, V, N, A>::t_itr
      t_hashbuf<K, V, N, A>::find(R_key_ key) {
    t_hash_id id = impl_::hash_(key, hash_);
    return t_itr(impl_::find_(slot_(slots_, max_, id), id, key));
  }

  // H_19
  template<typename K, typename V, int N, typename A>
  inline
  typename t_hashbuf<K, V, N, A>::t_Itr
      t_hashbuf<K, V, N, A>::Find(R_key_ key) const {
    t_hash_id id = impl_::hash_(key, hash_);
    return t_Itr(impl_::Find_(Slot_(slots_, max_, id), id, key));
  }

  /////////////////////////////////////////////////////////////////////////////

  // H_20
  template<typename K, typename V, int N, typename A>
  inline
  typename t_hashbuf<K, V, N, A>::t_itr
      t_hashbuf<K, V, N, A>::find_hash(t_hash_id id) {
    return t_itr(impl_::find_hash_(slot_(slots_, max_, impl_::HASH_USE_ | id), id));
  }

  // H_21
  template<typename K, typename V, int N, typename A>
  inline
  typename t_hashbuf<K, V, N, A>::t_Itr
      t_hashbuf<K, V, N, A>::Find_hash(t_hash_id id) const {
    return t_Itr(impl_::Find_hash_(Slot_(slots_, max_, impl_::HASH_USE_ | id), id));
  }

  /////////////////////////////////////////////////////////////////////////////

  // H_23
  template<typename K, typename V, int N, typename A>
  inline
  t_void t_hashbuf<K, V, N, A>::compact(t_reuse reuse) {
    // TODO_2
  }

  // H_25
  template<typename K, typename V, int N, typename A>
  inline
  t_void t_hashbuf<K, V, N, A>::gcollect() {
    cache_.clear();
  }

  // H_24
  template<typename K, typename V, int N, typename A>
  inline
  t_void t_hashbuf<K, V, N, A>::optimize() {
    compact(NO_REUSE);
    gcollect();
  }

  // H_16
  template<typename K, typename V, int N, typename A>
  inline
  typename t_hashbuf<K, V, N, A>::t_stats
      t_hashbuf<K, V, N, A>::stats() const {
    return impl_::stats_(head_.Const(), max_, size_, cache_, SIZE_OF_);
  }

  // H_17
  template<typename K, typename V, int N, typename A>
  inline
  t_void t_hashbuf<K, V, N, A>::show(r_out_ out, t_bool detail,
                                      p_out_func out_func) const {
    impl_::show_<t_Itr>(out, detail, out_func, impl_::Const_(head_), max_,
                        size_, cache_, slots_, SIZE_OF_);
  }

  // H_22
  template<typename K, typename V, int N, typename A>
  inline
  t_bool t_hashbuf<K, V, N, A>::rehash(t_n_max max) {
    return impl_::rehash_(head_, cache_, size_, max_, max, slots_);
  }

  /////////////////////////////////////////////////////////////////////////////

  // H_26
  template<typename K, typename V, int N, typename A>
  inline
  typename t_hashbuf<K, V, N, A>::t_itr t_hashbuf<K, V, N, A>::begin() {
    return t_itr(head_);
  }

  // H_28
  template<typename K, typename V, int N, typename A>
  inline
  typename t_hashbuf<K, V, N, A>::t_itr t_hashbuf<K, V, N, A>::end() {
    return t_Itr();
  }

  /////////////////////////////////////////////////////////////////////////////

  // H_27
  template<typename K, typename V, int N, typename A>
  inline
  typename t_hashbuf<K, V, N, A>::t_Itr t_hashbuf<K, V, N, A>::Begin() const {
    return t_Itr{head_};
  }

  // H_29
  template<typename K, typename V, int N, typename A>
  inline
  typename t_hashbuf<K, V, N, A>::t_Itr t_hashbuf<K, V, N, A>::End() const {
    return t_Itr();
  }

  /////////////////////////////////////////////////////////////////////////////

  // H_30
  template<typename K, typename V, int N, typename A>
  template<typename F>
  inline
  t_void t_hashbuf<K, V, N, A>::each(F&& func) {
    impl_::each_<t_itr>(head_, func);
  }

  // H_31
  template<typename K, typename V, int N, typename A>
  template<typename F>
  inline
  t_void t_hashbuf<K, V, N, A>::Each(F&& func) const {
    impl_::each_<t_Itr>(impl_::Const_(head_), func);
  }

  /////////////////////////////////////////////////////////////////////////////

  // H_32
  template<typename K, typename V, int N, typename A>
  template<typename P, typename F>
  inline
  t_n t_hashbuf<K, V, N, A>::each_if(P&& pred, F&& func) {
    return impl_::each_if_<t_Itr, t_itr>(head_, pred, func);
  }

  // H_33
  template<typename K, typename V, int N, typename A>
  template<typename P, typename F>
  inline
  t_n t_hashbuf<K, V, N, A>::Each_if(P&& pred, F&& func) const {
    return impl_::each_if_<t_Itr, t_Itr>(impl_::Const_(head_), pred, func);
  }

  /////////////////////////////////////////////////////////////////////////////

  // Itr_1_04
  template<typename K, typename V, int N, typename A>
  inline
  typename t_Itr<K, V, N, A>::R_value_ t_Itr<K, V, N, A>::ref() const {
    return *Value();
  }

  // Itr_1_05
  template<typename K, typename V, int N, typename A>
  inline
  typename t_Itr<K, V, N, A>::R_value_ t_Itr<K, V, N, A>::Ref() const {
    return *Value();
  }

  /////////////////////////////////////////////////////////////////////////////

  // Itr_1_06
  template<typename K, typename V, int N, typename A>
  inline
  typename t_Itr<K, V, N, A>::P_value_ t_Itr<K, V, N, A>::ptr() const {
    return Value();
  }

  // Itr_1_07
  template<typename K, typename V, int N, typename A>
  inline
  typename t_Itr<K, V, N, A>::P_value_ t_Itr<K, V, N, A>::Ptr() const {
    return Value();
  }

  /////////////////////////////////////////////////////////////////////////////

  // Itr_1_03
  template<typename K, typename V, int N, typename A>
  inline
  t_Itr<K, V, N, A>::t_Itr(t_Ident_ ident) : ident_(ident) {
  }

  // Itr_2_03
  template<typename K, int N, typename A>
  inline
  t_Itr<K, t_void, N, A>::t_Itr(t_Ident_ ident) : ident_(ident) {
  }

  /////////////////////////////////////////////////////////////////////////////

  // Itr_1_01
  template<typename K, typename V, int N, typename A>
  inline
  t_Itr<K, V, N, A>::t_Itr(t_itr_ itr) : ident_{impl_::acc_(itr)} {
  }

  // Itr_2_01
  template<typename K, int N, typename A>
  inline
  t_Itr<K, t_void, N, A>::t_Itr(t_itr_ itr) : ident_{impl_::acc_(itr)} {
  }

  /////////////////////////////////////////////////////////////////////////////

  // Itr_1_02
  template<typename K, typename V, int N, typename A>
  inline
  t_Itr<K, V, N, A>::t_Itr(t_ident_ ident) : ident_{ident} {
  }

  // Itr_2_02
  template<typename K, int N, typename A>
  inline
  t_Itr<K, t_void, N, A>::t_Itr(t_ident_ ident) : ident_{ident} {
  }

  /////////////////////////////////////////////////////////////////////////////

  // Itr_1_14
  template<typename K, typename V, int N, typename A>
  inline
  typename t_Itr<K, V, N, A>::r_Itr_ t_Itr<K, V, N, A>::Next() {
    impl_::next_(ident_);
    return self_();
  }

  // Itr_2_08
  template<typename K, int N, typename A>
  inline
  typename t_Itr<K, t_void, N, A>::r_Itr_ t_Itr<K, t_void, N, A>::Next() {
    impl_::next_(ident_);
    return self_();
  }

  /////////////////////////////////////////////////////////////////////////////

  // Itr_1_15
  template<typename K, typename V, int N, typename A>
  inline
  typename t_Itr<K, V, N, A>::r_Itr_ t_Itr<K, V, N, A>::Next_same_key() {
    impl_::next_same_key_(ident_);
    return self_();
  }

  // Itr_2_09
  template<typename K, int N, typename A>
  inline
  typename t_Itr<K, t_void, N, A>::r_Itr_ t_Itr<K, t_void, N, A>::Next_same_key() {
    impl_::next_same_key_(ident_);
    return self_();
  }

  /////////////////////////////////////////////////////////////////////////////

  // Itr_1_16
  template<typename K, typename V, int N, typename A>
  inline
  typename t_Itr<K, V, N, A>::r_Itr_ t_Itr<K, V, N, A>::Next_same_hash() {
    impl_::next_same_hash_(ident_);
    return self_();
  }

  // Itr_2_10
  template<typename K, int N, typename A>
  inline
  typename t_Itr<K, t_void, N, A>::r_Itr_ t_Itr<K, t_void, N, A>::Next_same_hash() {
    impl_::next_same_hash_(ident_);
    return self_();
  }

  /////////////////////////////////////////////////////////////////////////////

  // Itr_1_17
  template<typename K, typename V, int N, typename A>
  inline
  typename t_Itr<K, V, N, A>::r_Itr_ t_Itr<K, V, N, A>::Next_same_slot() {
    // impl_next_same_slot_2
    impl_::next_same_slot_(ident_);
    return self_();
  }

  // Itr_2_11
  template<typename K, int N, typename A>
  inline
  typename t_Itr<K, t_void, N, A>::r_Itr_ t_Itr<K, t_void, N, A>::Next_same_slot() {
    // impl_next_same_slot_2
    impl_::next_same_slot_(ident_);
    return self_();
  }

  /////////////////////////////////////////////////////////////////////////////

  // Itr_1_08
  template<typename K, typename V, int N, typename A>
  inline
  typename t_Itr<K, V, N, A>::R_key_ t_Itr<K, V, N, A>::Key() const {
    return ident_.Val()->key;
  }

  // Itr_2_04
  template<typename K, int N, typename A>
  inline
  typename t_Itr<K, t_void, N, A>::R_key_ t_Itr<K, t_void, N, A>::Key() const {
    return ident_.Val()->key;
  }

  /////////////////////////////////////////////////////////////////////////////

  // Itr_1_09
  template<typename K, typename V, int N, typename A>
  inline
  typename t_Itr<K, V, N, A>::P_value_ t_Itr<K, V, N, A>::value() const {
    return Value();
  }

  // Itr_1_10
  template<typename K, typename V, int N, typename A>
  inline
  typename t_Itr<K, V, N, A>::P_value_ t_Itr<K, V, N, A>::Value() const {
    return &ident_.Val()->value;
  }

  /////////////////////////////////////////////////////////////////////////////

  // Itr_1_11
  template<typename K, typename V, int N, typename A>
  inline
  t_hash_id t_Itr<K, V, N, A>::hash() const {
    return ident_.hash();
  }

  // Itr_2_05
  template<typename K, int N, typename A>
  inline
  t_hash_id t_Itr<K, t_void, N, A>::hash() const {
    return ident_.hash();
  }

  /////////////////////////////////////////////////////////////////////////////

  // Itr_1_12
  template<typename K, typename V, int N, typename A>
  inline
  t_bool t_Itr<K, V, N, A>::is_valid() const {
    return ident_;
  }

  // Itr_2_06
  template<typename K, int N, typename A>
  inline
  t_bool t_Itr<K, t_void, N, A>::is_valid() const {
    return ident_;
  }

  /////////////////////////////////////////////////////////////////////////////

  // Itr_1_18
  template<typename K, typename V, int N, typename A>
  inline
  typename t_Itr<K, V, N, A>::t_Itr t_Itr<K, V, N, A>::operator++(int) {
    t_Itr tmp = Self_();
    Next();
    return tmp;
  }

  // Itr_2_12
  template<typename K, int N, typename A>
  inline
  typename t_Itr<K, t_void, N, A>::t_Itr t_Itr<K, t_void, N, A>::operator++(int) {
    t_Itr tmp = Self_();
    Next();
    return tmp;
  }

  /////////////////////////////////////////////////////////////////////////////

  // Itr_1_19
  template<typename K, typename V, int N, typename A>
  inline
  typename t_Itr<K, V, N, A>::r_Itr_ t_Itr<K, V, N, A>::operator++() {
    return Next();
  }

  // Itr_2_13
  template<typename K, int N, typename A>
  inline
  typename t_Itr<K, t_void, N, A>::r_Itr_ t_Itr<K, t_void, N, A>::operator++() {
    return Next();
  }

  /////////////////////////////////////////////////////////////////////////////

  // Itr_1_13
  template<typename K, typename V, int N, typename A>
  inline
  t_Itr<K, V, N, A>::operator t_bool() const {
    return ident_;
  }

  // Itr_2_07
  template<typename K, int N, typename A>
  inline
  t_Itr<K, t_void, N, A>::operator t_bool() const {
    return ident_;
  }

  /////////////////////////////////////////////////////////////////////////////

  // Itr_1_20
  template<typename K, typename V, int N, typename A>
  inline
  typename t_Itr<K, V, N, A>::P_value_ t_Itr<K, V, N, A>::operator->() const {
    return Ptr();
  }

  // Itr_1_21
  template<typename K, typename V, int N, typename A>
  inline
  typename t_Itr<K, V, N, A>::R_value_ t_Itr<K, V, N, A>::operator*() const {
    return Ref();
  }

  /////////////////////////////////////////////////////////////////////////////

  // itr_1_01
  template<typename K, typename V, int N, typename A>
  inline
  t_itr<K, V, N, A>::t_itr(t_ident_ ident) : ident_(ident) {
  }

  // itr_2_01
  template<typename K, int N, typename A>
  inline
  t_itr<K, t_void, N, A>::t_itr(t_ident_ ident) : ident_(ident) {
  }

  /////////////////////////////////////////////////////////////////////////////

  // itr_1_02
  template<typename K, typename V, int N, typename A>
  inline
  typename t_itr<K, V, N, A>::R_key_ t_itr<K, V, N, A>::Key() const {
    return ident_.Val()->key;
  }

  // itr_2_02
  template<typename K, int N, typename A>
  inline
  typename t_itr<K, t_void, N, A>::R_key_ t_itr<K, t_void, N, A>::Key() const {
    return ident_.Val()->key;
  }

  /////////////////////////////////////////////////////////////////////////////

  // itr_1_03
  template<typename K, typename V, int N, typename A>
  inline
  t_hash_id t_itr<K, V, N, A>::hash() const {
    return ident_.hash();
  }

  // itr_2_03
  template<typename K, int N, typename A>
  inline
  t_hash_id t_itr<K, t_void, N, A>::hash() const {
    return ident_.hash();
  }

  /////////////////////////////////////////////////////////////////////////////

  // itr_1_04
  template<typename K, typename V, int N, typename A>
  inline
  t_bool t_itr<K, V, N, A>::is_valid() const {
    return ident_;
  }

  // itr_2_04
  template<typename K, int N, typename A>
  inline
  t_bool t_itr<K, t_void, N, A>::is_valid() const {
    return ident_;
  }

  /////////////////////////////////////////////////////////////////////////////

  // itr_1_05
  template<typename K, typename V, int N, typename A>
  inline
  t_itr<K, V, N, A>::operator t_bool() const {
    return ident_;
  }

  // itr_2_05
  template<typename K, int N, typename A>
  inline
  t_itr<K, t_void, N, A>::operator t_bool() const {
    return ident_;
  }

  /////////////////////////////////////////////////////////////////////////////

  // itr_1_10
  template<typename K, typename V, int N, typename A>
  inline
  typename t_itr<K, V, N, A>::p_value_ t_itr<K, V, N, A>::value() {
    return &ident_.val()->value;
  }

  // itr_1_11
  template<typename K, typename V, int N, typename A>
  inline
  typename t_itr<K, V, N, A>::P_value_ t_itr<K, V, N, A>::Value() const {
    return &ident_.Val()->value;
  }

  // itr_1_06
  template<typename K, typename V, int N, typename A>
  inline
  typename t_itr<K, V, N, A>::r_value_ t_itr<K, V, N, A>::ref() {
    return *value();
  }

  // itr_1_07
  template<typename K, typename V, int N, typename A>
  inline
  typename t_itr<K, V, N, A>::R_value_ t_itr<K, V, N, A>::Ref() const {
    return *Value();
  }

  // itr_1_08
  template<typename K, typename V, int N, typename A>
  inline
  typename t_itr<K, V, N, A>::p_value_ t_itr<K, V, N, A>::ptr() {
    return value();
  }

  // itr_1_09
  template<typename K, typename V, int N, typename A>
  inline
  typename t_itr<K, V, N, A>::P_value_ t_itr<K, V, N, A>::Ptr() const {
    return Value();
  }

  /////////////////////////////////////////////////////////////////////////////

  // itr_1_22
  template<typename K, typename V, int N, typename A>
  inline
  typename t_itr<K, V, N, A>::p_value_ t_itr<K, V, N, A>::operator->() {
    return ptr();
  }

  /////////////////////////////////////////////////////////////////////////////

  // itr_1_23
  template<typename K, typename V, int N, typename A>
  inline
  typename t_itr<K, V, N, A>::P_value_ t_itr<K, V, N, A>::operator->() const {
    return Ptr();
  }

  /////////////////////////////////////////////////////////////////////////////

  // itr_1_24
  template<typename K, typename V, int N, typename A>
  inline
  typename t_itr<K, V, N, A>::r_value_ t_itr<K, V, N, A>::operator*() {
    return ref();
  }

  /////////////////////////////////////////////////////////////////////////////

  // itr_1_25
  template<typename K, typename V, int N, typename A>
  inline
  typename t_itr<K, V, N, A>::R_value_ t_itr<K, V, N, A>::operator*() const {
    return Ref();
  }

  /////////////////////////////////////////////////////////////////////////////

  // itr_1_12
  template<typename K, typename V, int N, typename A>
  inline
  typename t_itr<K, V, N, A>::r_itr_ t_itr<K, V, N, A>::next() {
    impl_::next_(ident_);
    return self_();
  }

  // itr_2_06
  template<typename K, int N, typename A>
  inline
  typename t_itr<K, t_void, N, A>::r_itr_ t_itr<K, t_void, N, A>::next() {
    impl_::next_(ident_);
    return self_();
  }

  /////////////////////////////////////////////////////////////////////////////

  // itr_1_13
  template<typename K, typename V, int N, typename A>
  inline
  typename t_itr<K, V, N, A>::t_Itr_ t_itr<K, V, N, A>::Next() {
    impl_::next_(ident_);
    return Self_();
  }

  // itr_2_07
  template<typename K, int N, typename A>
  inline
  typename t_itr<K, t_void, N, A>::t_Itr_ t_itr<K, t_void, N, A>::Next() {
    return Self_();
  }

  /////////////////////////////////////////////////////////////////////////////

  // itr_1_14
  template<typename K, typename V, int N, typename A>
  inline
  typename t_itr<K, V, N, A>::r_itr_ t_itr<K, V, N, A>::next_same_key() {
    impl_::next_same_key_(ident_);
    return self_();
  }

  // itr_2_08
  template<typename K, int N, typename A>
  inline
  typename t_itr<K, t_void, N, A>::r_itr_ t_itr<K, t_void, N, A>::next_same_key() {
    impl_::next_same_key_(ident_);
    return self_();
  }

  /////////////////////////////////////////////////////////////////////////////

  // itr_1_15
  template<typename K, typename V, int N, typename A>
  inline
  typename t_itr<K, V, N, A>::t_Itr_ t_itr<K, V, N, A>::Next_same_key() {
    impl_::next_same_key_(ident_);
    return Self_();
  }

  // itr_2_09
  template<typename K, int N, typename A>
  inline
  typename t_itr<K, t_void, N, A>::t_Itr_ t_itr<K, t_void, N, A>::Next_same_key() {
    impl_::next_same_key_(ident_);
    return Self_();
  }

  /////////////////////////////////////////////////////////////////////////////

  // itr_1_16
  template<typename K, typename V, int N, typename A>
  inline
  typename t_itr<K, V, N, A>::r_itr_ t_itr<K, V, N, A>::next_same_hash() {
    impl_::next_same_hash_(ident_);
    return self_();
  }

  // itr_2_10
  template<typename K, int N, typename A>
  inline
  typename t_itr<K, t_void, N, A>::r_itr_ t_itr<K, t_void, N, A>::next_same_hash() {
    impl_::next_same_hash_(ident_);
    return self_();
  }

  /////////////////////////////////////////////////////////////////////////////

  // itr_1_17
  template<typename K, typename V, int N, typename A>
  inline
  typename t_itr<K, V, N, A>::t_Itr_ t_itr<K, V, N, A>::Next_same_hash() {
    // impl_next_same_hash
    impl_::next_same_hash_(ident_);
    return Self_();
  }

  // itr_2_11
  template<typename K, int N, typename A>
  inline
  typename t_itr<K, t_void, N, A>::t_Itr_ t_itr<K, t_void, N, A>::Next_same_hash() {
    // impl_next_same_hash
    impl_::next_same_hash_(ident_);
    return Self_();
  }

  /////////////////////////////////////////////////////////////////////////////

  // itr_1_18
  template<typename K, typename V, int N, typename A>
  inline
  typename t_itr<K, V, N, A>::r_itr_ t_itr<K, V, N, A>::next_same_slot() {
    // impl_next_same_slot_1
    impl_::next_same_slot_(ident_);
    return self_();
  }

  // itr_2_12
  template<typename K, int N, typename A>
  inline
  typename t_itr<K, t_void, N, A>::r_itr_ t_itr<K, t_void, N, A>::next_same_slot() {
    // impl_next_same_slot_1
    impl_::next_same_slot_(ident_);
    return self_();
  }

  /////////////////////////////////////////////////////////////////////////////

  // itr_1_19
  template<typename K, typename V, int N, typename A>
  inline
  typename t_itr<K, V, N, A>::t_Itr_ t_itr<K, V, N, A>::Next_same_slot() {
    // impl_next_same_slot_1
    impl_::next_same_slot_(ident_);
    return Self_();
  }

  // itr_2_13
  template<typename K, int N, typename A>
  inline
  typename t_itr<K, t_void, N, A>::t_Itr_ t_itr<K, t_void, N, A>::Next_same_slot() {
    // impl_next_same_slot_1
    impl_::next_same_slot_(ident_);
    return Self_();
  }

  /////////////////////////////////////////////////////////////////////////////

  // itr_1_21
  template<typename K, typename V, int N, typename A>
  inline
  typename t_itr<K, V, N, A>::t_itr t_itr<K, V, N, A>::operator++(int) {
    t_itr tmp = Self_();
    next();
    return tmp;
  }

  // itr_2_15
  template<typename K, int N, typename A>
  inline
  typename t_itr<K, t_void, N, A>::t_itr t_itr<K, t_void, N, A>::operator++(int) {
    t_itr tmp = Self_();
    next();
    return tmp;
  }

  /////////////////////////////////////////////////////////////////////////////

  // itr_1_20
  template<typename K, typename V, int N, typename A>
  inline
  typename t_itr<K, V, N, A>::r_itr_ t_itr<K, V, N, A>::operator++() {
    return next();
  }

  // itr_2_14
  template<typename K, int N, typename A>
  inline
  typename t_itr<K, t_void, N, A>::r_itr_ t_itr<K, t_void, N, A>::operator++() {
    return next();
  }

  /////////////////////////////////////////////////////////////////////////////
}
}
#endif
