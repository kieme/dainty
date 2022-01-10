#ifndef _DAINTY_HASHBUF_IMPL_H_
#define _DAINTY_HASHBUF_IMPL_H_

#define DAINTY_INTERNAL_INCLUDE_NEW
#define DAINTY_INTERNAL_INCLUDE_IOSTREAM
#define DAINTY_INTERNAL_INCLUDE_C_STDLIB
#define DAINTY_INTERNAL_INCLUDE_C_STRING
#include "dainty_internal_.h"
#include "dainty_types.h"
#include "dainty_alloc.h"
#include "dainty_util.h"

// NOTE-1 only key is taken care off and value is just memory
// NOTE-2 value memcopied. It does not know values as types.
// NOTE-3 consider just doing a memcmp with keys.
// NOTE-4 hash_id can act as keys. The hash_id will not change even after a rehash.

// MUST_DO break code down - see if real functions can be used
// MUST_DO implement N==1, will be a huge improvement.
// MUST_DO use dainty pattern

// TODO - t_hash_node_, t_hash_val_, t_hash_alloc
// TODO - automatic hash function for types?
// TODO - how many bytes are in use compared not
// TODO - how about triggering a rehash request
// TODO - think about a use count

namespace dainty
{
namespace hashbuf
{
namespace impl_
{
  /////////////////////////////////////////////////////////////////////////////

  using types::t_bool;
  using types::t_void;
  using types::t_double;
  using types::t_of;
  using types::T_of;
  using types::r_of;
  using types::R_of;
  using types::x_of;
  using types::X_of;
  using types::p_of;
  using types::P_of;
  using types::q_of;
  using types::Q_of;

  using util::reset;
  using util::f_cast;
  using util::x_cast;

  using alloc::t_alloc;

  using t_reuse      = t_bool;
  using t_must_add   = t_bool;

  using t_hash_id    = types::t_u4_; // TODO
  using t_u1         = types::t_u1_;
  using t_n          = types::t_n_;
  using r_n          = types::r_n_;
  using t_ix         = t_n;
  using t_n_max      = t_n;
  using t_n_bytes    = t_n;

  /////////////////////////////////////////////////////////////////////////////

  constexpr t_reuse    REUSE       = true;
  constexpr t_reuse    NO_REUSE    = false;

  constexpr t_must_add MUST_ADD    = true;
  constexpr t_must_add FIND_OR_ADD = false;

  constexpr t_hash_id HASH_USE_    = 1 << ((sizeof(t_hash_id) * 8) - 1);
  constexpr t_hash_id HASH_MASK_   = ~HASH_USE_;

  /////////////////////////////////////////////////////////////////////////////

  template<typename A, typename T>
  constexpr
  t_void raw_copy(T* dst, const T* src, t_n n = 1) {
    ::memcpy(dst, src, A::template bytes<T>(n));
  }

  /////////////////////////////////////////////////////////////////////////////

  /*
  template<typename T>
  constexpr
  T reset(T& obj, const T& value = T()) {
    T tmp = obj;
    obj = value;
    return tmp;
  }

  template<typename T>
  constexpr
  T* reset(T*& obj, T* value) {
    T* tmp = obj;
    obj = value;
    return tmp;
  }

  template<typename T>
  constexpr
  const T* reset(const T*& obj, T* value) {
    const T* tmp = obj;
    obj = value;
    return tmp;
  }
  */

  /*
  template<typename T>
  constexpr
  T* reset(T*& obj, nullptr_t) {
    T* tmp = obj;
    obj = nullptr;
    return tmp;
  }
  */

  /////////////////////////////////////////////////////////////////////////////

  constexpr
  t_n next_prime(t_n value) {
    for (value += !(value & 1); true; value += 2) {
      t_n n = 3;
      for (; n < value && value % n; n += 2);
      if (n >= value)
        return value;
    }
    return 1;
  }

  /////////////////////////////////////////////////////////////////////////////

  struct t_hash_stats {
    t_n_bytes  memory                 = 0;
    t_n_bytes  active_use             = 0;
    t_n_bytes  overhead               = 0;
    t_n_bytes  minimum                = 0;
    t_n_bytes  cached                 = 0;
    t_n        entries                = 0;
    t_n_max    slots                  = 0;
    t_n        slots_in_use           = 0;
    t_n_max    node_entries           = 0;
    t_n        cached_nodes           = 0;
    t_n        nodes                  = 0;
    t_n        node_avg_entries       = 0;
    t_n        slot_node_max_depth    = 0;
    t_n        slot_node_avg_depth    = 0;
    t_n        slot_entries_max_depth = 0;
    t_n        slot_entries_avg_depth = 0;
    t_n        entries_in_first       = 0;
    t_n        entries_in_second      = 0;
    t_n        entries_in_third       = 0;
    t_n        entries_in_fourth      = 0;
    t_n        entries_in_rest        = 0;
  };

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A>
  struct t_node_;

  template<typename K, typename V>
  struct t_val_;

  template<typename K, typename V, int N, typename A>
  struct t_Ident_;

  template<typename K, typename V, int N, typename A>
  struct t_ident_;

  /////////////////////////////////////////////////////////////////////////////

  template<typename A>
  struct t_traits_alloc_ {
    using t_alloc_ = A;
    using t_alloc  = t_of<t_alloc_>;
    using r_alloc  = r_of<t_alloc_>;
    using R_alloc  = R_of<t_alloc_>;
    using x_alloc  = x_of<t_alloc_>;
  };

  template<typename K>
  struct t_traits_key_ {
    using t_key_ = K;
    using t_key  = t_of<t_key_>;
    using R_key  = R_of<t_key_>;
    using x_key  = x_of<t_key_>;
  };

  template<typename V>
  struct t_traits_value_ {
    using t_value_ = V;
    using t_value  = t_of<t_value_>;
    using r_value  = r_of<t_value_>;
    using R_value  = R_of<t_value_>;
    using p_value  = p_of<t_value_>;
    using P_value  = P_of<t_value_>;
  };

  template<>
  struct t_traits_value_<t_void> {
    using t_value_ = t_void;
    using t_value  = t_of<t_value_>;
  };

  template<typename K, typename V>
  struct t_traits_val_ {
    using t_val_ = impl_::t_val_<K, V>;
    using t_val  = t_of<t_val_>;
    using p_val  = p_of<t_val_>;
    using P_val  = P_of<t_val_>;
  };

  template<typename K, typename V, int N, typename A>
  struct t_traits_node_ {
    using t_node_    = impl_::t_node_<K, V, N, A>;
    using t_node     = t_of<t_node_>;
    using p_node     = p_of<t_node_>;
    using P_node     = P_of<t_node_>;
    using q_node     = q_of<t_node_>;
    using Q_node     = Q_of<t_node_>;
  };

  template<typename K, typename V, int N, typename A>
  struct t_traits_ : public t_traits_value_<V> {
    using t_alloc    = typename t_traits_alloc_<A>::t_alloc;
    using r_alloc    = typename t_traits_alloc_<A>::r_alloc;
    using R_alloc    = typename t_traits_alloc_<A>::R_alloc;
    using x_alloc    = typename t_traits_alloc_<A>::x_alloc;
    using t_key      = typename t_traits_key_<K>::t_key;
    using R_key      = typename t_traits_key_<K>::R_key;
    using x_key      = typename t_traits_key_<K>::x_key;
    using t_val      = typename t_traits_val_<K, V>::t_val;
    using p_val      = typename t_traits_val_<K, V>::p_val;
    using P_val      = typename t_traits_val_<K, V>::P_val;
    using t_u1_val   = alignas(t_val) t_u1[t_alloc::template bytes<t_val>()];
    using t_node     = typename t_traits_node_<K, V, N, A>::t_node;
    using p_node     = typename t_traits_node_<K, V, N, A>::p_node;
    using P_node     = typename t_traits_node_<K, V, N, A>::P_node;
    using q_node     = typename t_traits_node_<K, V, N, A>::q_node;
    using Q_node     = typename t_traits_node_<K, V, N, A>::Q_node;
    using t_Ident    = impl_::t_Ident_<K, V, N, A>;
    using r_Ident    = t_Ident&;
    using R_Ident    = const t_Ident&;
    using t_ident    = impl_::t_ident_<K, V, N, A>;
    using R_ident    = const t_ident&;
    using t_out      = std::ostream;
    using r_out      = t_out&;
    using t_stats    = t_hash_stats;
  };

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V>
  struct t_val_ {
    using t_key   = typename t_traits_key_<K>::t_key;
    using t_value = typename t_traits_value_<V>::t_value;

    t_key   key;
    t_value value;
  };

  template<typename K>
  struct t_val_<K, t_void> {
    using t_key   = K;
    using t_value = typename t_traits_value_<t_void>::t_value;

    t_key key;
  };

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V>
  inline
  t_void construct_copy_(t_val_<K, V>* dst, const K& key) {
    new (&dst->key) K(key);
  }

  template<typename K, typename V>
  inline
  t_void construct_move_(t_val_<K, V>* dst, K&& key) {
    new (&dst->key) K(x_cast(key));
  }

  template<typename K, typename V>
  inline
  t_void construct_move_(t_val_<K, V>* dst, t_val_<K, V>* src) {
    new (&dst->key) K(x_cast(src->key));
    ::memcpy(&dst->value, &src->value, sizeof(V));
  }

  template<typename K, typename V>
  inline
  t_void construct_(t_val_<K, V>* dst, const t_val_<K, V>* src) {
    new (&dst->key) K(src->key);
    ::memcpy(&dst->value, &src->value, sizeof(V));
  }

  template<typename K>
  inline
  t_void construct_(t_val_<K, t_void>* dst, const t_val_<K, t_void>* src) {
    new (&dst->key) K(src->key);
  }

  template<typename K, typename V>
  inline
  t_void destruct_(t_val_<K, V>* dst) {
    dst->key.~K();
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A>
  struct t_node_ {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using t_alloc   = typename t_traits_::t_alloc;
    using p_node_   = typename t_traits_::p_node;
    using q_node_   = typename t_traits_::q_node;
    using t_val_    = typename t_traits_::t_val;
    using p_val_    = typename t_traits_::p_val;
    using P_val_    = typename t_traits_::P_val;
    using t_u1_val_ = typename t_traits_::t_u1_val;

    t_n       cnt  = 0;
    q_node_   slot = nullptr;
    p_node_   prv  = nullptr;
    p_node_   nxt  = nullptr;
    t_hash_id hsh[N];
    t_u1_val_ val[N];

    constexpr
    t_node_(q_node_ _slot, p_node_ _prv, p_node_ _nxt)
        : slot(_slot), prv(_prv), nxt(_nxt) {
      for (t_ix ix = 0; ix < N; ++ix)
        hsh[ix] = 0;
    }

    constexpr
    p_val_ val_(t_ix ix) {
      return reinterpret_cast<p_val_>(&val[ix]);
    }

    constexpr
    P_val_ Val_(t_ix ix) const {
      return reinterpret_cast<P_val_>(&val[ix]);
    }
  };

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, typename A>
  struct t_node_<K, V, 1, A> {
    using t_traits_ = impl_::t_traits_<K, V, 1, A>;
    using t_alloc   = typename t_traits_::t_alloc;
    using p_node_   = typename t_traits_::p_node;
    using q_node_   = typename t_traits_::q_node;
    using t_val_    = typename t_traits_::t_val;
    using p_val_    = typename t_traits_::p_val;
    using P_val_    = typename t_traits_::P_val;
    using t_u1_val_ = typename t_traits_::t_u1_val;

    q_node_   slot = nullptr;
    p_node_   prv  = nullptr;
    p_node_   nxt  = nullptr;
    t_hash_id hsh  = 0;
    t_u1_val_ val;

    constexpr
    t_node_(q_node_ _slot, p_node_ _prv, p_node_ _nxt)
        : slot(_slot), prv(_prv), nxt(_nxt) {
    }

    constexpr
    p_val_ val_() {
      return reinterpret_cast<p_val_>(&val);
    }

    constexpr
    P_val_ Val_() const {
      return reinterpret_cast<P_val_>(&val);
    }
  };

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A>
  class t_ident_ {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using t_Ident_  = typename t_traits_::t_Ident;
    using t_node_   = typename t_traits_::t_node;
    using q_node_   = typename t_traits_::q_node;
    using p_node_   = typename t_traits_::p_node;
    using Q_node_   = typename t_traits_::Q_node;
    using p_val_    = typename t_traits_::p_val;
    using P_val_    = typename t_traits_::P_val;

  public:
    t_ix    ix   = 0;
    p_node_ node = nullptr;

    constexpr
    t_ident_() = default;

    constexpr
    t_ident_(t_ix _ix, p_node_ _node) : ix(_ix), node(_node) {
    }

    constexpr p_val_ val() {
      return node->val_(ix);
    }

    constexpr P_val_ Val() const {
      return node->Val_(ix);
    }

    constexpr
    q_node_ slot() {
      return node->slot;
    }

    constexpr
    Q_node_ Slot() const {
      return node->slot;
    }

    constexpr
    t_hash_id hash() const {
      return node->hsh[ix];
    }

    constexpr
    operator t_bool() const {
      return node;
    }

    constexpr
    t_void clear() {
      ix   = 0;
      node = nullptr;
    }

    constexpr
    t_ident_ reset() {
      t_ident_ tmp{ix, node};
      clear();
      return tmp;
    }

    constexpr
    t_ident_ reset(t_ident_ ident) {
      t_ident_ tmp{ix, node};
      ix   = ident.ix;
      node = ident.node;
      return tmp;
    }
  };

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, typename A>
  class t_ident_<K, V, 1, A> {
    using t_traits_ = impl_::t_traits_<K, V, 1, A>;
    using t_Ident_  = typename t_traits_::t_Ident;
    using t_node_   = typename t_traits_::t_node;
    using q_node_   = typename t_traits_::q_node;
    using Q_node_   = typename t_traits_::Q_node;
    using p_node_   = typename t_traits_::p_node;
    using p_val_    = typename t_traits_::p_val;
    using P_val_    = typename t_traits_::P_val;

  public:
    p_node_ node = nullptr;

    constexpr
    t_ident_() = default;

    constexpr
    t_ident_(p_node_ _node) : node(_node) {
    }

    constexpr p_val_ val() {
      return node->val_();
    }

    constexpr P_val_ Val() const {
      return node->Val_();
    }

    constexpr
    q_node_ slot() {
      return node->slot;
    }

    constexpr
    Q_node_ Slot() const {
      return node->slot;
    }

    constexpr
    t_hash_id hash() const {
      return node->hsh;
    }

    constexpr
    operator t_bool() const {
      return node;
    }

    constexpr
    t_void clear() {
      node = nullptr;
    }

    constexpr
    t_ident_ reset() {
      t_ident_ tmp{node};
      clear();
      return tmp;
    }

    constexpr
    t_ident_ reset(t_ident_ ident) {
      t_ident_ tmp{node};
      node = ident.node;
      return tmp;
    }
  };

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A>
  class t_Ident_ {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using r_Ident_  = typename t_traits_::r_Ident;
    using R_Ident_  = typename t_traits_::R_Ident;
    using R_ident_  = typename t_traits_::R_ident;
    using t_node_   = typename t_traits_::t_node;
    using Q_node_   = typename t_traits_::Q_node;
    using P_node_   = typename t_traits_::P_node;
    using P_val_    = typename t_traits_::P_val;

  public:
    t_ix    ix   = 0;
    P_node_ node = nullptr;

    constexpr
    t_Ident_() = default;

    constexpr
    t_Ident_(t_ix _ix, P_node_ _node) : ix(_ix), node(_node) {
    }

    constexpr
    t_Ident_(R_ident_ ident) : ix(ident.ix), node(ident.node) {
    }

    constexpr
    r_Ident_ operator=(R_ident_ ident) {
      ix   = ident.ix;
      node = ident.node;
      return *this;
    }

    constexpr P_val_ Val() const {
      return node->Val_(ix);
    }

    constexpr
    t_hash_id hash() const {
      return node->hsh[ix];
    }

    constexpr
    Q_node_ Slot() const {
      return node->slot;
    }

    constexpr
    operator t_bool() const {
      return node;
    }

    constexpr
    t_void clear() {
      ix   = 0;
      node = nullptr;
    }

    constexpr
    t_Ident_ reset() {
      t_Ident_ tmp{ix, node};
      clear();
      return tmp;
    }

    constexpr
    t_Ident_ reset(R_Ident_ ident) {
      t_Ident_ tmp{ix, node};
      ix   = ident.ix;
      node = ident.node;
      return tmp;
    }
  };

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, typename A>
  class t_Ident_<K, V, 1, A> {
    using t_traits_ = impl_::t_traits_<K, V, 1, A>;
    using R_ident_  = typename t_traits_::R_ident;
    using R_Ident_  = typename t_traits_::R_Ident;
    using r_Ident_  = typename t_traits_::r_Ident;
    using t_node_   = typename t_traits_::t_node;
    using Q_node_   = typename t_traits_::Q_node;
    using P_node_   = typename t_traits_::P_node;
    using P_val_    = typename t_traits_::P_val;

  public:
    P_node_ node = nullptr;

    constexpr
    t_Ident_() = default;

    constexpr
    t_Ident_(P_node_ _node) : node(_node) {
    }

    constexpr
    t_Ident_(R_ident_ ident) : node(ident.node) {
    }

    constexpr
    r_Ident_ operator=(R_ident_ ident) {
      node = ident.node;
      return *this;
    }

    constexpr P_val_ Val() const {
      return node->Val_();
    }

    constexpr
    t_hash_id hash() const {
      return node->hsh;
    }

    constexpr
    Q_node_ Slot() const {
      return node->slot;
    }

    constexpr
    operator t_bool() const {
      return node;
    }

    constexpr
    t_void clear() {
      node = nullptr;
    }

    constexpr
    t_Ident_ reset() {
      t_Ident_ tmp{node};
      clear();
      return tmp;
    }

    constexpr
    t_Ident_ reset(t_Ident_ ident) {
      t_Ident_ tmp{node};
      node = ident.node;
      return tmp;
    }
  };

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A>
  constexpr
  t_Ident_<K, V, N, A> Const_(const t_ident_<K, V, N, A>& ident) {
    return ident;
  }

  template<typename K, typename V, int N, typename A>
  constexpr
  const t_Ident_<K, V, N, A>& Const_(const t_Ident_<K, V, N, A>& ident) {
    return ident;
  }

  /////////////////////////////////////////////////////////////////////////////

  template<template<typename, typename, int, typename> typename I,
           typename K, typename V, int N, typename A>
  constexpr
  t_ident_<K, V, N, A>& acc_(I<K, V, N, A>& i) {
    return i.ident_;
  }

  template<template<typename, typename, int, typename> typename I,
           typename K, typename V, int N, typename A>
  constexpr
  t_Ident_<K, V, N, A>& Acc_(I<K, V, N, A>& i) {
    return i.ident_;
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A>
  constexpr
  t_bool operator==(const t_Ident_<K, V, N, A>& lh,
                    const t_Ident_<K, V, N, A>& rh) {
    return lh.node == rh.node && lh.ix == rh.ix;
  }

  template<typename K, typename V, typename A>
  constexpr
  t_bool operator==(const t_Ident_<K, V, 1, A>& lh,
                    const t_Ident_<K, V, 1, A>& rh) {
    return lh.node == rh.node;
  }

  template<typename K, typename V, int N, typename A>
  constexpr
  t_bool operator==(const t_ident_<K, V, N, A>& lh,
                    const t_ident_<K, V, N, A>& rh) {
    return lh.node == rh.node && lh.ix == rh.ix;
  }

  template<typename K, typename V, typename A>
  constexpr
  t_bool operator==(const t_ident_<K, V, 1, A>& lh,
                    const t_ident_<K, V, 1, A>& rh) {
    return lh.node == rh.node;
  }

  template<typename K, typename V, int N, typename A>
  constexpr
  t_bool operator==(const t_ident_<K, V, N, A>& lh,
                    const t_Ident_<K, V, N, A>& rh) {
    return lh.node == rh.node && lh.ix == rh.ix;
  }

  template<typename K, typename V, typename A>
  constexpr
  t_bool operator==(const t_ident_<K, V, 1, A>& lh,
                    const t_Ident_<K, V, 1, A>& rh) {
    return lh.node == rh.node;
  }

  template<typename K, typename V, int N, typename A>
  constexpr
  t_bool operator==(const t_Ident_<K, V, N, A>& lh,
                    const t_ident_<K, V, N, A>& rh) {
    return lh.node == rh.node && lh.ix == rh.ix;
  }

  template<typename K, typename V, typename A>
  constexpr
  t_bool operator==(const t_Ident_<K, V, 1, A>& lh,
                    const t_ident_<K, V, 1, A>& rh) {
    return lh.node == rh.node;
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A>
  constexpr
  t_bool operator!=(const t_Ident_<K, V, N, A>& lh,
                    const t_Ident_<K, V, N, A>& rh) {
    return lh.node != rh.node || lh.ix != rh.ix;
  }

  template<typename K, typename V, typename A>
  constexpr
  t_bool operator!=(const t_Ident_<K, V, 1, A>& lh,
                    const t_Ident_<K, V, 1, A>& rh) {
    return lh.node != rh.node;
  }

  template<typename K, typename V, int N, typename A>
  constexpr
  t_bool operator!=(const t_ident_<K, V, N, A>& lh,
                    const t_ident_<K, V, N, A>& rh) {
    return lh.node != rh.node || lh.ix != rh.ix;
  }

  template<typename K, typename V, typename A>
  constexpr
  t_bool operator!=(const t_ident_<K, V, 1, A>& lh,
                    const t_ident_<K, V, 1, A>& rh) {
    return lh.node != rh.node;
  }

  template<typename K, typename V, int N, typename A>
  constexpr
  t_bool operator!=(const t_ident_<K, V, N, A>& lh,
                    const t_Ident_<K, V, N, A>& rh) {
    return lh.node != rh.node || lh.ix != rh.ix;
  }

  template<typename K, typename V, typename A>
  constexpr
  t_bool operator!=(const t_ident_<K, V, 1, A>& lh,
                    const t_Ident_<K, V, 1, A>& rh) {
    return lh.node != rh.node;
  }

  template<typename K, typename V, int N, typename A>
  constexpr
  t_bool operator!=(const t_Ident_<K, V, N, A>& lh,
                    const t_ident_<K, V, N, A>& rh) {
    return lh.node != rh.node || lh.ix != rh.ix;
  }

  template<typename K, typename V, typename A>
  constexpr
  t_bool operator!=(const t_Ident_<K, V, 1, A>& lh,
                    const t_ident_<K, V, 1, A>& rh) {
    return lh.node != rh.node;
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A>
  constexpr
  t_ident_<K, V, N, A> reset(t_ident_<K, V, N, A>& obj) {
    return obj.reset();
  }

  template<typename K, typename V, int N, typename A>
  constexpr
  t_ident_<K, V, N, A> reset(t_ident_<K, V, N, A>& obj,
                             const t_ident_<K, V, N, A>& value) {
    return obj.reset(value);
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A>
  class t_alloc_cache_ {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using t_node_   = typename t_traits_::t_node;
    using p_node_   = typename t_traits_::p_node;
    using q_node_   = typename t_traits_::q_node;
    using t_alloc_  = typename t_traits_::t_alloc;
    using R_alloc_  = typename t_traits_::R_alloc;
    using x_alloc_  = typename t_traits_::x_alloc;

  public:
    inline
    t_alloc_cache_() = default;

    inline
    t_alloc_cache_(R_alloc_ alloc) : alloc_(alloc) {
    }

    inline
    t_alloc_cache_(x_alloc_ alloc) : alloc_(x_cast(alloc)) {
    }

    inline
   ~t_alloc_cache_() {
      clear();
    }

    template<typename T>
    constexpr
    static t_n_bytes bytes(t_n n = 1) {
      return t_alloc_::template bytes<T>(n);
    }

    inline
    p_node_ acquire() {
      if (!avail_)
        return alloc_.template acquire<t_node_>();
      p_node_ node = avail_;
      avail_ = avail_->nxt;
      return node;
    }

    inline
    q_node_ acquire_fill(t_n_max max) {
      return alloc_.template acquire_fill<p_node_>(max);
    }

    inline
    t_void release(p_node_ node, t_reuse reuse = NO_REUSE) {
      if (reuse) {
        node->nxt = avail_;
        avail_    = node;
      } else
        alloc_.release(node);
    }

    inline
    t_void release(q_node_ slots) {
      alloc_.release(slots);
    }

    inline
    t_void clear() {
      p_node_ node = reset(avail_, nullptr);
      while (node) {
        p_node_ tmp = node;
        node = node->nxt;
        alloc_.release(tmp);
      }
    }

    constexpr
    t_n cached_nodes() const {
      t_n n = 0;
      for (p_node_ node = avail_; node; node = node->nxt)
        ++n;
      return n;
    }

  private:
    t_alloc_ alloc_;
    p_node_  avail_ = nullptr;
  };

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A>
  constexpr
  t_node_<K, V, N, A>**
      slot_(t_node_<K, V, N, A>** slots, t_n_max max, t_hash_id id) {
    return &slots[(HASH_MASK_ & id) % max];
  }

  template<typename K, typename V, int N, typename A>
  constexpr
  const t_node_<K, V, N, A>* const *
      Slot_(const t_node_<K, V, N, A>* const * slots, t_n_max max, t_hash_id id) {
    return &slots[(HASH_MASK_ & id) % max];
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename K>
  constexpr
  t_hash_id hash_(const K& key, t_hash_id (&hash)(const K&) noexcept) noexcept {
    return HASH_USE_ | hash(key);
  }

  /////////////////////////////////////////////////////////////////////////////

  template<template<typename, typename, int, typename> typename I,
           typename K, typename V, int N, typename A>
  constexpr
  t_void next_(I<K, V, N, A>& ident) {
    ++ident.ix;
    do {
      for (; ident.ix < N; ++ident.ix)
        if (ident.hash())
          return;
      ident.ix = 0;
    } while ((ident.node = ident.node->nxt));
  }

  template<template<typename, typename, int, typename> typename I,
           typename K, typename V, typename A>
  constexpr
  t_void next_(I<K, V, 1, A>& ident) {
    ident.node = ident.node->nxt;
  }

  /////////////////////////////////////////////////////////////////////////////

  template<template<typename, typename, int, typename> typename I,
           typename K, typename V, int N, typename A>
  constexpr
  t_void next_same_key_(I<K, V, N, A>& ident) {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using Q_node_   = typename t_traits_::Q_node;
    using P_val_    = typename t_traits_::P_val;

    Q_node_   slot = ident.Slot();
    P_val_    val  = ident.Val();
    t_hash_id id   = ident.hash();
    ++ident.ix;
    do {
      for (; ident.ix < N; ++ident.ix)
        if (ident.hash() == id && val->key == ident.Val()->key)
          return;
      ident.ix = 0;
    } while ((ident.node = ident.node->nxt) && ident.Slot() == slot);
    ident.reset();
  }

  template<template<typename, typename, int, typename> typename I,
           typename K, typename V, typename A>
  constexpr
  t_void next_same_key_(I<K, V, 1, A>& ident) {
    using t_traits_ = impl_::t_traits_<K, V, 1, A>;
    using Q_node_   = typename t_traits_::Q_node;
    using P_val_    = typename t_traits_::P_val;

    Q_node_   slot = ident.Slot();
    P_val_    val  = ident.Val();
    t_hash_id id   = ident.hash();
    while ((ident.node = ident.node->nxt) && ident.Slot() == slot) {
      if (ident.hash() == id && val->key == ident.Val()->key)
        return;
    }
    ident.node = nullptr;
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A>
  inline
  t_Ident_<K, V, N, A> Find_hash_(const t_node_<K, V, N, A>* const * slot,
                                  t_hash_id id) {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using P_node_   = typename t_traits_::P_node;

    for (P_node_ node = *slot; node && node->slot == slot; node = node->nxt) {
      for (t_ix ix = 0, cnt = 0; cnt < node->cnt; ++ix) {
        t_hash_id h = node->hsh[ix];
        if (h) {
          if (h == id)
            return {ix, node};
          ++cnt;
        }
      }
    }
    return {};
  }

  template<typename K, typename V, typename A>
  inline
  t_Ident_<K, V, 1, A> Find_hash_(const t_node_<K, V, 1, A>* const * slot,
                                  t_hash_id id) {
    using t_traits_ = impl_::t_traits_<K, V, 1, A>;
    using P_node_   = typename t_traits_::P_node;

    for (P_node_ node = *slot; node && node->slot == slot; node = node->nxt) {
      if (node->hsh == id)
        return {node};
    }
    return {};
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A>
  inline
  t_ident_<K, V, N, A> find_hash_(t_node_<K, V, N, A>** slot, t_hash_id id) {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using p_node_   = typename t_traits_::p_node;

    for (p_node_ node = *slot; node && node->slot == slot; node = node->nxt) {
      for (t_ix ix = 0, cnt = 0; cnt < node->cnt; ++ix) {
        t_hash_id h = node->hsh[ix];
        if (h) {
          if (h == id)
            return {ix, node};
          ++cnt;
        }
      }
    }
    return {};
  }

  template<typename K, typename V, typename A>
  inline
  t_ident_<K, V, 1, A> find_hash_(t_node_<K, V, 1, A>** slot, t_hash_id id) {
    using t_traits_ = impl_::t_traits_<K, V, 1, A>;
    using p_node_   = typename t_traits_::p_node;

    for (p_node_ node = *slot; node && node->slot == slot; node = node->nxt) {
      if (node->hsh == id)
        return {node};
    }
    return {};
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A>
  inline
  t_Ident_<K, V, N, A> Find_(const t_node_<K, V, N, A>* const * slot,
                             t_hash_id id, const K& key) {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using P_node_   = typename t_traits_::P_node;

    for (P_node_ node = *slot; node && node->slot == slot; node = node->nxt) {
      for (t_ix ix = 0, cnt = 0; cnt < node->cnt; ++ix) {
        t_hash_id h = node->hsh[ix];
        if (h) {
          if (h == id && key == node->Val_(ix)->key)
            return {ix, node};
          ++cnt;
        }
      }
    }
    return {};
  }

  template<typename K, typename V, typename A>
  inline
  t_Ident_<K, V, 1, A> Find_(const t_node_<K, V, 1, A>* const * slot,
                             t_hash_id id, const K& key) {
    using t_traits_ = impl_::t_traits_<K, V, 1, A>;
    using P_node_   = typename t_traits_::P_node;

    for (P_node_ node = *slot; node && node->slot == slot; node = node->nxt) {
      if (node->hsh == id && key == node->Val_()->key)
        return {node};
    }
    return {};
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A>
  inline
  t_ident_<K, V, N, A> find_(t_node_<K, V, N, A>** slot, t_hash_id id,
                             const K& key) {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using p_node_   = typename t_traits_::p_node;

    for (p_node_ node = *slot; node && node->slot == slot; node = node->nxt) {
      for (t_ix ix = 0, cnt = 0; cnt < node->cnt; ++ix) {
        t_hash_id h = node->hsh[ix];
        if (h) {
          if (h == id && key == node->val_(ix)->key)
            return {ix, node};
          ++cnt;
        }
      }
    }
    return {};
  }

  template<typename K, typename V, typename A>
  inline
  t_ident_<K, V, 1, A> find_(t_node_<K, V, 1, A>** slot,
                             t_hash_id id, const K& key) {
    using t_traits_ = impl_::t_traits_<K, V, 1, A>;
    using p_node_   = typename t_traits_::p_node;

    for (p_node_ node = *slot; node && node->slot == slot; node = node->nxt) {
      if (node->hsh == id && key == node->val_()->key)
        return {node};
    }
    return {};
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename HF, typename K, typename V, int N, typename A, typename F>
  inline
  t_void each_(t_ident_<K, V, N, A> head, F&& func) {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using p_node_   = typename t_traits_::p_node;
    using t_ident_  = typename t_traits_::t_ident;

    t_ix ix = head.ix;
    for (p_node_ node = head.node; node; node = node->nxt) {
      for (t_ix cnt = 0; cnt < node->cnt; ++ix) {
        if (node->hsh[ix]) {
          ++cnt;
          func(HF{t_ident_{ix, node}});
        }
      }
      ix = 0;
    }
  }

  template<typename HF, typename K, typename V, typename A, typename F>
  inline
  t_void each_(t_ident_<K, V, 1, A> head, F&& func) {
    using t_traits_ = impl_::t_traits_<K, V, 1, A>;
    using p_node_   = typename t_traits_::p_node;
    using t_ident_  = typename t_traits_::t_ident;

    for (p_node_ node = head.node; node; node = node->nxt)
      func(HF{t_ident_{node}});
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename HF, typename K, typename V, int N, typename A, typename F>
  inline
  t_void each_(t_Ident_<K, V, N, A> head, F&& func) {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using P_node_   = typename t_traits_::P_node;
    using t_Ident_  = typename t_traits_::t_Ident;

    t_ix ix = head.ix;
    for (P_node_ node = head.node; node; node = node->nxt) {
      for (t_ix cnt = 0; cnt < node->cnt; ++ix) {
        if (node->hsh[ix]) {
          ++cnt;
          func(HF{t_Ident_{ix, node}});
        }
      }
      ix = 0;
    }
  }

  template<typename HF, typename K, typename V, typename A, typename F>
  inline
  t_void each_(t_Ident_<K, V, 1, A> head, F&& func) {
    using t_traits_ = impl_::t_traits_<K, V, 1, A>;
    using P_node_   = typename t_traits_::P_node;
    using t_Ident_  = typename t_traits_::t_Ident;

    for (P_node_ node = head.node; node; node = node->nxt)
      func(HF{t_Ident_{node}});
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename HP, typename HF, typename K, typename V, int N, typename A,
           typename P, typename F>
  inline
  t_n each_if_(t_Ident_<K, V, N, A> head, P&& pred, F&& func) {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using P_node_   = typename t_traits_::P_node;
    using t_Ident_  = typename t_traits_::t_Ident;

    t_n  n  = 0;
    t_ix ix = head.ix;
    for (P_node_ node = head.node; node; node = node->nxt) {
      for (t_ix cnt = 0; cnt < node->cnt; ++ix) {
        if (node->hsh[ix]) {
          ++cnt;
          t_Ident_ ident{ix, node};
          if (pred(HP{ident})) {
            ++n;
            func(HF{ident});
          }
        }
      }
      ix = 0;
    }
    return n;
  }

  template<typename HP, typename HF, typename K, typename V, typename A,
           typename P, typename F>
  inline
  t_n each_if_(t_Ident_<K, V, 1, A> head, P&& pred, F&& func) {
    using t_traits_ = impl_::t_traits_<K, V, 1, A>;
    using P_node_   = typename t_traits_::P_node;
    using t_Ident_  = typename t_traits_::t_Ident;

    t_n n  = 0;
    for (P_node_ node = head.node; node; node = node->nxt) {
      t_Ident_ ident{node};
      if (pred(HP{ident})) {
        ++n;
        func(HF{ident});
      }
    }
    return n;
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename HP, typename HF, typename K, typename V, int N, typename A,
           typename P, typename F>
  inline
  t_n each_if_(t_ident_<K, V, N, A> head, P&& pred, F&& func) {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using p_node_   = typename t_traits_::p_node;
    using t_ident_  = typename t_traits_::t_ident;

    t_n  n  = 0;
    t_ix ix = head.ix;
    for (p_node_ node = head.node; node; node = node->nxt) {
      for (t_ix cnt = 0; cnt < node->cnt; ++ix) {
        if (node->hsh[ix]) {
          ++cnt;
          t_ident_ ident{ix, node};
          if (pred(HP{Const_(ident)})) {
            ++n;
            func(HF{ident});
          }
        }
      }
      ix = 0;
    }
    return n;
  }

  template<typename HP, typename HF, typename K, typename V, typename A,
           typename P, typename F>
  inline
  t_n each_if_(t_ident_<K, V, 1, A> head, P&& pred, F&& func) {
    using t_traits_ = impl_::t_traits_<K, V, 1, A>;
    using p_node_   = typename t_traits_::p_node;
    using t_ident_  = typename t_traits_::t_ident;

    t_n  n  = 0;
    for (p_node_ node = head.node; node; node = node->nxt) {
      t_ident_ ident{node};
      if (pred(HP{Const_(ident)})) {
        ++n;
        func(HF{ident});
      }
    }
    return n;
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A>
  t_void next_same_hash_(t_ident_<K, V, N, A>& ident) {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using Q_node_   = typename t_traits_::Q_node;

    Q_node_   slot = ident.Slot();
    t_hash_id id   = ident.hash();

    ++ident.ix;
    do {
      for (; ident.ix < N; ++ident.ix)
        if (ident.hash() == id)
          return;
      ident.ix = 0;
    } while ((ident.node = ident.node->nxt) && ident.Slot() == slot);
    ident.node = nullptr;
  }

  template<typename K, typename V, typename A>
  t_void next_same_hash_(t_ident_<K, V, 1, A>& ident) {
    using t_traits_ = impl_::t_traits_<K, V, 1, A>;
    using Q_node_   = typename t_traits_::Q_node;

    Q_node_   slot = ident.Slot();
    t_hash_id id   = ident.hash();
    while ((ident.node = ident.node->nxt) && ident.Slot() == slot) {
      if (ident.hash() == id)
        return;
    }
    ident.node = nullptr;
  }

  /////////////////////////////////////////////////////////////////////////////

  // impl_next_same_hash_1
  template<typename K, typename V, int N, typename A>
  t_void next_same_hash_(t_Ident_<K, V, N, A>& ident) {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using Q_node_   = typename t_traits_::Q_node;

    Q_node_   slot = ident.Slot();
    t_hash_id id   = ident.hash();

    ++ident.ix;
    do {
      for (; ident.ix < N; ++ident.ix)
        if (ident.hash() == id)
          return;
      ident.ix = 0;
    } while ((ident.node = ident.node->nxt) && ident.Slot() == slot);
    ident.node = nullptr;
  }

  // impl_next_same_hash_2
  template<typename K, typename V, typename A>
  t_void next_same_hash_(t_Ident_<K, V, 1, A>& ident) {
    using t_traits_ = impl_::t_traits_<K, V, 1, A>;
    using Q_node_   = typename t_traits_::Q_node;

    Q_node_   slot = ident.Slot();
    t_hash_id id   = ident.hash();
    while ((ident.node = ident.node->nxt) && ident.Slot() == slot) {
      if (ident.hash() == id)
        return;
    }
    ident.node = nullptr;
  }

  /////////////////////////////////////////////////////////////////////////////

  // impl_next_same_slot_1_1
  template<typename K, typename V, int N, typename A>
  inline
  t_void next_same_slot_(t_ident_<K, V, N, A>& ident) {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using Q_node_   = typename t_traits_::Q_node;

    Q_node_ slot = ident.Slot();
    ++ident.ix;
    do {
      for (; ident.ix < N; ++ident.ix)
        if (ident.hash())
          return;
      ident.ix = 0;
    } while ((ident.node = ident.node->nxt) && ident.Slot() == slot);
    ident.node = nullptr;
  }

  // impl_next_same_slot_1_2
  template<typename K, typename V, typename A>
  inline
  t_void next_same_slot_(t_ident_<K, V, 1, A>& ident) {
    using t_traits_ = impl_::t_traits_<K, V, 1, A>;
    using Q_node_   = typename t_traits_::Q_node;

    Q_node_ slot = ident.Slot();
    while ((ident.node = ident.node->nxt) && ident.Slot() == slot)
      return;
    ident.node = nullptr;
  }

  /////////////////////////////////////////////////////////////////////////////

  // impl_next_same_slot_2_1
  template<typename K, typename V, int N, typename A>
  inline
  t_void next_same_slot_(t_Ident_<K, V, N, A>& ident) {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using Q_node_   = typename t_traits_::Q_node;

    Q_node_ slot = ident.Slot();
    ++ident.ix;
    do {
      for (; ident.ix < N; ++ident.ix)
        if (ident.hash())
          return;
      ident.ix = 0;
    } while ((ident.node = ident.node->nxt) && ident.Slot() == slot);
    ident.node = nullptr;
  }

  // impl_next_same_slot_2_2
  template<typename K, typename V, typename A>
  inline
  t_void next_same_slot_(t_Ident_<K, V, 1, A>& ident) {
    using t_traits_ = impl_::t_traits_<K, V, 1, A>;
    using Q_node_   = typename t_traits_::Q_node;

    Q_node_ slot = ident.Slot();
    while ((ident.node = ident.node->nxt) && ident.Slot() == slot)
      return;
    ident.node = nullptr;
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A>
  inline
  t_void del_(t_ident_<K, V, N, A>& head, t_alloc_cache_<K, V, N, A>& cache,
              r_n size, t_ident_<K, V, N, A> ident, t_reuse reuse) {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using p_node_   = typename t_traits_::p_node;

    if (ident) {
      p_node_ node = ident.node;
      if (remove_(ident, size, head))
        unlink_(head, node, cache, reuse);
    }
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename HF, typename K, typename V, int N, typename A, typename F>
  inline
  t_void del_(t_ident_<K, V, N, A>& head, t_alloc_cache_<K, V, N, A>& cache,
              r_n size, t_ident_<K, V, N, A> ident, t_reuse reuse,
              F&& func) {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using p_node_   = typename t_traits_::p_node;

    if (ident) {
      func(HF{ident});
      p_node_ node = ident.node;
      if (remove_(ident, size, head))
        unlink_(head, node, cache, reuse);
    }
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A>
  inline
  t_bool del_(t_ident_<K, V, N, A>& head, t_alloc_cache_<K, V, N, A>& cache,
              t_hash_id (&hash)(const K&) noexcept,
              t_node_<K, V, N, A>** slots, r_n size, t_n max, const K& key,
              t_reuse reuse) {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using q_node_   = typename t_traits_::q_node;
    using p_node_   = typename t_traits_::p_node;
    using t_ident_  = typename t_traits_::t_ident;

    t_hash_id id   = hash_(key, hash);
    q_node_   slot = slot_(slots, max, id);
    for (p_node_ node = *slot; node && node->slot == slot; node = node->nxt) {
      for (t_ix ix = 0, cnt = 0; cnt < node->cnt; ++ix) {
        t_hash_id h = node->hsh[ix];
        if (h) {
          if (h == id && key == node->Val_(ix)->key) {
            if (remove_(t_ident_(ix, node), size, head))
              unlink_(head, node, cache, reuse);
            return true;
          }
          ++cnt;
        }
      }
    }
    return false;
  }

  template<typename K, typename V, typename A>
  inline
  t_bool del_(t_ident_<K, V, 1, A>& head, t_alloc_cache_<K, V, 1, A>& cache,
              t_hash_id (&hash)(const K&) noexcept,
              t_node_<K, V, 1, A>** slots, r_n size, t_n max, const K& key,
              t_reuse reuse) {
    using t_traits_ = impl_::t_traits_<K, V, 1, A>;
    using q_node_   = typename t_traits_::q_node;
    using p_node_   = typename t_traits_::p_node;
    using t_ident_  = typename t_traits_::t_ident;

    t_hash_id id   = hash_(key, hash);
    q_node_   slot = slot_(slots, max, id);
    for (p_node_ node = *slot; node && node->slot == slot; node = node->nxt) {
      t_hash_id h = node->hsh;
      if (h == id && key == node->Val_()->key) {
        if (remove_(t_ident_(node), size, head))
          unlink_(head, node, cache, reuse);
        return true;
      }
    }
    return false;
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename HF, typename K, typename V, int N, typename A, typename F>
  inline
  t_bool del_(t_ident_<K, V, N, A>& head, t_alloc_cache_<K, V, N, A>& cache,
              t_hash_id (&hash)(const K&) noexcept,
              t_node_<K, V, N, A>** slots, r_n size, t_n max, const K& key,
              t_reuse reuse, F&& func) {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using q_node_   = typename t_traits_::q_node;
    using p_node_   = typename t_traits_::p_node;
    using t_ident_  = typename t_traits_::t_ident;

    t_hash_id id   = hash_(key, hash);
    q_node_   slot = slot_(slots, max, id);
    for (p_node_ node = *slot; node && node->slot == slot; node = node->nxt) {
      for (t_ix ix = 0, cnt = 0; cnt < node->cnt; ++ix) {
        t_hash_id h = node->hsh[ix];
        if (h) {
          if (h == id && key == node->Val_(ix)->key) {
            t_ident_ ident(ix, node);
            func(HF{ident});
            if (remove_(ident, size, head))
              unlink_(head, node, cache, reuse);
            return true;
          }
          ++cnt;
        }
      }
    }
    return false;
  }

  template<typename HF, typename K, typename V, typename A, typename F>
  inline
  t_bool del_(t_ident_<K, V, 1, A>& head, t_alloc_cache_<K, V, 1, A>& cache,
              t_hash_id (&hash)(const K&) noexcept,
              t_node_<K, V, 1, A>** slots, r_n size, t_n max, const K& key,
              t_reuse reuse, F&& func) {
    using t_traits_ = impl_::t_traits_<K, V, 1, A>;
    using q_node_   = typename t_traits_::q_node;
    using p_node_   = typename t_traits_::p_node;
    using t_ident_  = typename t_traits_::t_ident;

    t_hash_id id   = hash_(key, hash);
    q_node_   slot = slot_(slots, max, id);
    for (p_node_ node = *slot; node && node->slot == slot; node = node->nxt) {
      t_hash_id h = node->hsh;
      if (h == id && key == node->Val_()->key) {
        t_ident_ ident(node);
        func(HF{ident});
        if (remove_(ident, size, head))
          unlink_(head, node, cache, reuse);
        return true;
      }
    }
    return false;
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename HP, typename K, typename V, int N, typename A, typename P>
  inline
  t_n del_if_(t_ident_<K, V, N, A>& head, t_alloc_cache_<K, V, N, A>& cache,
              r_n size, P&& pred, t_reuse reuse) {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using p_node_   = typename t_traits_::p_node;
    using q_node_   = typename t_traits_::q_node;
    using t_ident_  = typename t_traits_::t_ident;

    t_n n = 0;
    if (head) {
      t_ident_ ident = reset(head);
      do {
        t_n cnt_node = ident.node->cnt;
        for (t_ix cnt = 0; cnt < cnt_node; ++ident.ix) {
          if (ident.hash()) {
            ++cnt;
            if (pred(HP{Const_(ident)})) {
              ++n;
              undo_(ident, size);
            } else if (!head)
              head = ident;
          }
        }
        ident.ix = 0;
        p_node_ tmp = reset(ident.node, ident.node->nxt);
        if (!tmp->cnt) {
          q_node_ slot = tmp->slot;
          if (tmp == *slot)
            *slot = (ident.node && ident.node->slot == slot) ? ident.node : nullptr;
          if (ident)
            ident.node->prv = tmp->prv;
          if (tmp->prv)
            tmp->prv->nxt = ident.node;
          cache.release(tmp, reuse);
        }
      } while (ident.node);
    }
    return n;
  }

  template<typename HP, typename K, typename V, typename A, typename P>
  inline
  t_n del_if_(t_ident_<K, V, 1, A>& head, t_alloc_cache_<K, V, 1, A>& cache,
              r_n size, P&& pred, t_reuse reuse) {
    using t_traits_ = impl_::t_traits_<K, V, 1, A>;
    using p_node_   = typename t_traits_::p_node;
    using q_node_   = typename t_traits_::q_node;
    using t_ident_  = typename t_traits_::t_ident;

    t_n n = 0;
    if (head) {
      p_node_ node = reset(head.node, nullptr);
      do {
        t_ident_ ident(node);
        if (pred(HP{Const_(ident)})) {
          ++n;
          undo_(ident, size);
        } else if (!head.node)
          head.node = node;

        p_node_ tmp = reset(node, node->nxt);
        if (!tmp->cnt) {
          q_node_ slot = tmp->slot;
          if (tmp == *slot)
            *slot = (node && node->slot == slot) ? node : nullptr;
          if (node)
            node->prv = tmp->prv;
          if (tmp->prv)
            tmp->prv->nxt = node;
          cache.release(tmp, reuse);
        }
      } while (node);
    }
    return n;
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename HP, typename HF, typename K, typename V, int N, typename A,
           typename P, typename F>
  inline
  t_n del_if_(t_ident_<K, V, N, A>& head, t_alloc_cache_<K, V, N, A>& cache,
              r_n size, P&& pred, t_reuse reuse, F&& func) {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using p_node_   = typename t_traits_::p_node;
    using q_node_   = typename t_traits_::q_node;
    using t_ident_  = typename t_traits_::t_ident;

    t_n n = 0;
    if (head) {
      t_ix    ix   = reset(head.ix, 0U);
      p_node_ node = reset(head.node, nullptr);
      do {
        t_n cnt_node = node->cnt;
        for (t_ix cnt = 0; cnt < cnt_node; ++ix) {
          if (node->hsh[ix]) {
            ++cnt;
            t_ident_ ident(ix, node);
            if (pred(HP{Const_(ident)})) {
              ++n;
              func(HF{ident});
              undo_(ident, size);
            } else if (!head.node) {
              head.ix   = ix;
              head.node = node;
            }
          }
        }
        ix = 0;

        p_node_ tmp = reset(node, node->nxt);
        if (!tmp->cnt) {
          q_node_ slot = tmp->slot;
          if (tmp == *slot)
            *slot = (node && node->slot == slot) ? node : nullptr;
          if (node)
            node->prv = tmp->prv;
          if (tmp->prv)
            tmp->prv->nxt = node;
          cache.release(tmp, reuse);
        }
      } while (node);
    }
    return n;
  }

  template<typename HP, typename HF, typename K, typename V, typename A,
           typename P, typename F>
  inline
  t_n del_if_(t_ident_<K, V, 1, A>& head, t_alloc_cache_<K, V, 1, A>& cache,
              r_n size, P&& pred, t_reuse reuse, F&& func) {
    using t_traits_ = impl_::t_traits_<K, V, 1, A>;
    using p_node_   = typename t_traits_::p_node;
    using q_node_   = typename t_traits_::q_node;
    using t_ident_  = typename t_traits_::t_ident;

    t_n n = 0;
    if (head) {
      p_node_ node = reset(head.node, nullptr);
      do {
        t_ident_ ident(node);
        if (pred(HP{Const_(ident)})) {
          ++n;
          func(HF{ident});
          undo_(ident, size);
        } else if (!head.node)
          head.node = node;

        p_node_ tmp = reset(node, node->nxt);
        if (!tmp->cnt) {
          q_node_ slot = tmp->slot;
          if (tmp == *slot)
            *slot = (node && node->slot == slot) ? node : nullptr;
          if (node)
            node->prv = tmp->prv;
          if (tmp->prv)
            tmp->prv->nxt = node;
          cache.release(tmp, reuse);
        }
      } while (node);
    }
    return n;
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A>
  inline
  t_void clear_(t_ident_<K, V, N, A>& head, t_alloc_cache_<K, V, N, A>& cache,
                r_n size, t_reuse reuse) {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using p_node_   = typename t_traits_::p_node;
    using q_node_   = typename t_traits_::q_node;

    if (head) {
      t_ix    ix   = reset(head.ix, 0U);
      p_node_ node = reset(head.node, nullptr);
      q_node_ slot = nullptr;
      do {
        for (t_ix cnt = 0; cnt < node->cnt; ++ix) {
          if (node->hsh[ix]) {
            ++cnt;
            destruct_(node->val_(ix));
          }
        }
        if (slot != node->slot) {
          slot = node->slot;
         *slot = nullptr;
        }
        ix = 0;
        cache.release(reset(node, node->nxt), reuse);
      } while (node);
      size = 0;
    }
  }

  template<typename K, typename V, typename A>
  inline
  t_void clear_(t_ident_<K, V, 1, A>& head, t_alloc_cache_<K, V, 1, A>& cache,
                r_n size, t_reuse reuse) {
    using t_traits_ = impl_::t_traits_<K, V, 1, A>;
    using p_node_   = typename t_traits_::p_node;
    using q_node_   = typename t_traits_::q_node;

    if (head) {
      p_node_ node = reset(head.node, nullptr);
      q_node_ slot = nullptr;
      do {
        destruct_(node->val_());
        if (slot != node->slot) {
          slot = node->slot;
         *slot = nullptr;
        }
        cache.release(reset(node, node->nxt), reuse);
      } while (node);
      size = 0;
    }
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename HF, typename K, typename V, int N, typename A, typename F>
  inline
  t_void clear_(t_ident_<K, V, N, A>& head, t_alloc_cache_<K, V, N, A>& cache,
                r_n size, t_reuse reuse, F&& func) {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using p_node_   = typename t_traits_::p_node;
    using q_node_   = typename t_traits_::q_node;
    using t_ident_  = typename t_traits_::t_ident;

    if (head) {
      t_ix    ix   = reset(head.ix, 0U);
      p_node_ node = reset(head.node, nullptr);
      q_node_ slot = nullptr;
      do {
        for (t_ix cnt = 0; cnt < node->cnt; ++ix) {
          if (node->hsh[ix]) {
            ++cnt;
            func(HF{t_ident_{ix, node}});
            destruct_(node->val_(ix));
          }
        }
        if (slot != node->slot) {
          slot = node->slot;
         *slot = nullptr;
        }
        ix = 0;
        cache.release(reset(node, node->nxt), reuse);
      } while (node);
      size = 0;
    }
  }

  template<typename HF, typename K, typename V, typename A, typename F>
  inline
  t_void clear_(t_ident_<K, V, 1, A>& head, t_alloc_cache_<K, V, 1, A>& cache,
                r_n size, t_reuse reuse, F&& func) {
    using t_traits_ = impl_::t_traits_<K, V, 1, A>;
    using p_node_   = typename t_traits_::p_node;
    using q_node_   = typename t_traits_::q_node;
    using t_ident_  = typename t_traits_::t_ident;

    if (head) {
      p_node_ node = reset(head.node, nullptr);
      q_node_ slot = nullptr;
      do {
        func(HF{t_ident_{node}});
        destruct_(node->val_());
        if (slot != node->slot) {
          slot = node->slot;
         *slot = nullptr;
        }
        cache.release(reset(node, node->nxt), reuse);
      } while (node);
      size = 0;
    }
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A>
  constexpr
  t_n node_cnt_(const t_node_<K, V, N, A>* node) {
    return node->cnt;
  }

  template<typename K, typename V, typename A>
  constexpr
  t_n node_cnt_(const t_node_<K, V, 1, A>* node) {
    return 1;
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A>
  inline
  t_hash_stats stats_(t_Ident_<K, V, N, A> head, t_n max, t_n size,
                      const t_alloc_cache_<K, V, N, A>& alloc,
                      t_n_bytes buf_size) {
    using t_traits_   = impl_::t_traits_<K, V, N, A>;
    using t_val_      = typename t_traits_::t_val;
    using t_stats_    = typename t_traits_::t_stats;
    using t_node_     = typename t_traits_::t_node;
    using P_node_     = typename t_traits_::P_node;

    const t_n_bytes node_bytes = alloc.template bytes<t_node_>();

    t_stats_ stats;

    stats.memory       = buf_size + alloc.template bytes<P_node_>(max);
    stats.slots        = max;
    stats.entries      = size;
    stats.minimum      = size * alloc.template bytes<t_val_>();
    stats.node_entries = N;

    t_n slot_nodes   = 0;
    t_n slot_entries = 0;
    for (P_node_ node = head.node; node; ) {
      t_n node_cnt = node_cnt_(node);
      stats.nodes  += 1;
      slot_entries += node_cnt;
      switch (++slot_nodes) {
        case 1:  stats.entries_in_first  += node_cnt; break;
        case 2:  stats.entries_in_second += node_cnt; break;
        case 3:  stats.entries_in_third  += node_cnt; break;
        case 4:  stats.entries_in_fourth += node_cnt; break;
        default: stats.entries_in_rest   += node_cnt; break;
      }

      P_node_ tmp = reset(node, node->nxt);

      if (!node || tmp->slot != node->slot) {
        ++stats.slots_in_use;
        if (slot_nodes > stats.slot_node_max_depth)
          stats.slot_node_max_depth = slot_nodes;
        slot_nodes = 0;
        if (slot_entries > stats.slot_entries_max_depth)
          stats.slot_entries_max_depth = slot_entries;
        slot_entries = 0;
      }
    }

    stats.cached_nodes           = alloc.cached_nodes();
    stats.memory                += (node_bytes * stats.nodes);
    stats.cached                += (node_bytes * stats.cached_nodes);
    stats.memory                += stats.cached;
    stats.active_use             = stats.memory - stats.cached;
    stats.overhead               = stats.active_use - stats.minimum;
    stats.slot_node_avg_depth    = stats.nodes / stats.slots_in_use;
    stats.node_avg_entries       = stats.entries / stats.nodes;
    stats.slot_entries_avg_depth = stats.entries / stats.slots;
    return stats;
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename H, typename K, typename V, int N, typename A>
  inline
  t_void show_node_(std::ostream& out,
                    std::ostream& (*out_func)(std::ostream&, const H&) noexcept,
                    const t_node_<K, V, N, A>* node) {
    out << "." << node->cnt << "{";
    for (t_ix ix = 0, cnt = 0; cnt < node->cnt; ++ix) {
      t_hash_id id = node->hsh[ix];
      if (id) {
        ++cnt;
        out << "#" << ix << "." << id;
        if (out_func) {
          out << "[";
          out_func(out, H{t_Ident_<K, V, N, A>{ix, node}});
          out << "]";
        }
        if (cnt != node->cnt)
          out << ",";
      }
    }
    out << "}";
  }

  template<typename H, typename K, typename V, typename A>
  inline
  t_void show_node_(std::ostream& out,
                    std::ostream& (*out_func)(std::ostream&, const H&) noexcept,
                    const t_node_<K, V, 1, A>* node) {
    out << "{" << node->hsh;
    if (out_func) {
      out << "[";
      out_func(out, H{t_Ident_<K, V, 1, A>{node}});
      out << "]";
    }
    out << "}";
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename H, typename K, typename V, int N, typename A>
  inline
  t_void show_(std::ostream& out, t_bool detail,
               std::ostream& (*out_func)(std::ostream&, const H&) noexcept,
               t_Ident_<K, V, N, A> head, t_n max, t_n size,
               const t_alloc_cache_<K, V, N, A>& alloc,
               const t_node_<K, V, N, A>* const * slots,
               t_n_bytes buf_size) {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using t_stats_  = typename t_traits_::t_stats;
    using P_node_   = typename t_traits_::P_node;
    using Q_node_   = typename t_traits_::Q_node;

    t_stats_ stats = stats_(head, max, size, alloc, buf_size);
    out <<   "memory                 = " << stats.memory
        << "\nactive_use             = " << stats.active_use
        << "\noverhead               = " << stats.overhead
        << "\nminimum                = " << stats.minimum
        << "\ncached                 = " << stats.cached
        << "\nentries                = " << stats.entries
        << "\nslots                  = " << stats.slots
        << "\nslots_in_use           = " << stats.slots_in_use
        << "\nnode_entries           = " << stats.node_entries
        << "\ncached_nodes           = " << stats.cached_nodes
        << "\nnodes                  = " << stats.nodes
        << "\nnode_avg_entries       = " << stats.node_avg_entries
        << "\nslot_node_max_depth    = " << stats.slot_node_max_depth
        << "\nslot_node_avg_depth    = " << stats.slot_node_avg_depth
        << "\nslot_entries_max_depth = " << stats.slot_entries_max_depth
        << "\nslot_entries_avg_depth = " << stats.slot_entries_avg_depth
        << "\nentries_in_first       = " << stats.entries_in_first
        << "\nentries_in_second      = " << stats.entries_in_second
        << "\nentries_in_third       = " << stats.entries_in_third
        << "\nentries_in_fourth      = " << stats.entries_in_fourth
        << "\nentries_in_rest        = " << stats.entries_in_rest
        << std::endl;
    if (detail) {
      Q_node_ slot      = nullptr;
      t_n     slot_node = 0;
      for (P_node_ node = head.node; node; node = node->nxt) {
        if (slot != node->slot) {
          slot      = node->slot;
          slot_node = 0;
          out << "\n->slot#" << (slot - slots);
        }
        ++slot_node;
        out << " ->node#" << slot_node;
        show_node_<H>(out, out_func, node);
      }
      out << std::endl;
    }
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A>
  inline
  t_n_bytes bytes_(const t_Ident_<K, V, N, A>& head, t_n_max max,
                   const t_alloc_cache_<K, V, N, A>& alloc,
                   t_n_bytes buf_size) {
    using t_traits_   = impl_::t_traits_<K, V, N, A>;
    using t_node_     = typename t_traits_::t_node;
    using P_node_     = typename t_traits_::P_node;

    t_n_bytes bytes = buf_size + alloc.template bytes<P_node_>(max);
    t_n       nodes = alloc.cached_nodes();
    for (P_node_ node = head.node; node; node = node->nxt)
      ++nodes;
    bytes += nodes * alloc.template bytes<t_node_>();
    return bytes;
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A>
  inline
  t_ident_<K, V, N, A> add_(t_ident_<K, V, N, A>& head,
                            t_alloc_cache_<K, V, N, A>& cache,
                            r_n size, t_hash_id id,
                            t_node_<K, V, N, A>** slot) {
    using t_traits_   = impl_::t_traits_<K, V, N, A>;
    using t_node_     = typename t_traits_::t_node;
    using p_node_     = typename t_traits_::p_node;

    p_node_ node_add = nullptr;
    t_ix    ix_add   = 0;
    if (*slot) {
      p_node_ last = nullptr;
      for (p_node_ node = *slot; node && node->slot == slot; node = node->nxt) {
        for (t_ix cnt = 0, ix = 0; cnt < node->cnt; ++ix) {
          if (!node->hsh[ix]) {
            node_add = node;
            ix_add   = ix;
            break;
          }
          ++cnt;
        }
        if (!node_add && node->cnt < N) {
          node_add = node;
          ix_add   = node->cnt;
        }
        last = node;
      }
      if (!node_add) {
        node_add = new (cache.acquire()) t_node_(slot, last, last->nxt);
        if (last->nxt)
          last->nxt->prv = node_add;
        last->nxt        = node_add;
      }
    } else {
      node_add = new (cache.acquire()) t_node_(slot, nullptr, head.node);
      if (node_add->nxt)
        node_add->nxt->prv = node_add;
      head.ix   = 0;
      head.node = node_add;
      *slot     = node_add;
    }
    ++size;
    node_add->cnt        += 1;
    node_add->hsh[ix_add] = id;
    return {ix_add, node_add};
  }

  template<typename K, typename V, typename A>
  inline
  t_ident_<K, V, 1, A> add_(t_ident_<K, V, 1, A>& head,
                            t_alloc_cache_<K, V, 1, A>& cache,
                            r_n size, t_hash_id id,
                            t_node_<K, V, 1, A>** slot) {
    using t_traits_   = impl_::t_traits_<K, V, 1, A>;
    using t_node_     = typename t_traits_::t_node;
    using p_node_     = typename t_traits_::p_node;

    p_node_ node_add = nullptr;
    if (*slot) {
      p_node_ last = nullptr;
      for (p_node_ node = *slot; node && node->slot == slot; node = node->nxt)
        last = node;

      node_add = new (cache.acquire()) t_node_(slot, last, last->nxt);
      if (last->nxt)
        last->nxt->prv = node_add;
      last->nxt        = node_add;
    } else {
      node_add = new (cache.acquire()) t_node_(slot, nullptr, head.node);
      if (node_add->nxt)
        node_add->nxt->prv = node_add;
      head.node = node_add;
      *slot     = node_add;
    }
    ++size;
    node_add->hsh = id;
    return {node_add};
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A>
  inline
  t_ident_<K, V, N, A> add_cp_(t_ident_<K, V, N, A>& head,
                               t_alloc_cache_<K, V, N, A>& cache,
                               t_hash_id (&hash)(const K&) noexcept,
                               const K& key,
                               r_n size, t_n_max max,
                               t_node_<K, V, N, A>** slots) {
    using t_traits_   = impl_::t_traits_<K, V, N, A>;
    using q_node_     = typename t_traits_::q_node;
    using t_ident_    = typename t_traits_::t_ident;

    t_hash_id id    = hash_(key, hash);
    q_node_   slot  = slot_(slots, max, id);
    t_ident_  ident = add_(head, cache, size, id, slot);
    if (ident)
      construct_copy_(ident.val(), key);
    return ident;
  }

  template<typename K, typename V, int N, typename A>
  inline
  t_ident_<K, V, N, A> add_mv_(t_ident_<K, V, N, A>& head,
                               t_alloc_cache_<K, V, N, A>& cache,
                               t_hash_id (&hash)(const K&) noexcept,
                               K&& key,
                               r_n size, t_n_max max,
                               t_node_<K, V, N, A>** slots) {
    using t_traits_   = impl_::t_traits_<K, V, N, A>;
    using q_node_     = typename t_traits_::q_node;
    using t_ident_    = typename t_traits_::t_ident;

    t_hash_id id    = hash_(key, hash);
    q_node_   slot  = slot_(slots, max, id);
    t_ident_  ident = add_(head, cache, size, id, slot);
    if (ident)
      construct_move_(ident.val(), x_cast(key));
    return ident;
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A>
  inline
  t_bool rehash_(t_ident_<K, V, N, A>& head, t_alloc_cache_<K, V, N, A>& cache,
                 r_n _size, t_n_max& max, t_n_max new_max,
                 t_node_<K, V, N, A>**& _slots) {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using t_ident_  = typename t_traits_::t_ident;
    using p_node_   = typename t_traits_::p_node;
    using q_node_   = typename t_traits_::q_node;
    using p_val_    = typename t_traits_::p_val;

    if (new_max) {
      new_max = next_prime(new_max);
      if (max != new_max) {
        max        = new_max;
        t_n     size  = reset(_size, 0U);
        q_node_ slots = reset(_slots, cache.acquire_fill(max));
        if (size) {
          p_node_ node = reset(head.node, nullptr);
          t_ix    ix   = reset(head.ix, 0U);
          do {
            for (t_ix cnt = 0; cnt < node->cnt; ++ix) {
              t_hash_id id = node->hsh[ix];
              if (id) {
                ++cnt;
                p_val_   val   = node->val_(ix);
                q_node_  slot  = slot_(_slots, max, id);
                t_ident_ ident = add_(head, cache, _size, id, slot);
                construct_(ident.val(), val);
              }
            }
            ix = 0;
            p_node_ tmp = node;
            node = node->nxt;
            cache.release(tmp, REUSE);
          } while (node);
        }
        cache.release(slots);
      }
      return true;
    }
    return false;
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A>
  inline
  t_void unlink_(t_ident_<K, V, N, A>& head, t_node_<K, V, N, A>* node,
                 t_alloc_cache_<K, V, N, A>& cache, t_reuse reuse) {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using q_node_   = typename t_traits_::q_node;

    q_node_ slot = node->slot;
    if (node == *slot) {
     *slot = (node->nxt && node->nxt->slot == slot) ? node->nxt : nullptr;
      if (head.node == node) {
        head.node = node->nxt;
        if (head.node)
          for (head.ix = 0; !head.hash(); ++head.ix);
      }
    }
    if (node->nxt)
      node->nxt->prv = node->prv;
    if (node->prv)
      node->prv->nxt = node->nxt;
    cache.release(node, reuse);
  }

  template<typename K, typename V, typename A>
  inline
  t_void unlink_(t_ident_<K, V, 1, A>& head, t_node_<K, V, 1, A>* node,
                 t_alloc_cache_<K, V, 1, A>& cache, t_reuse reuse) {
    using t_traits_ = impl_::t_traits_<K, V, 1, A>;
    using q_node_   = typename t_traits_::q_node;

    q_node_ slot = node->slot;
    if (node == *slot) {
     *slot = (node->nxt && node->nxt->slot == slot) ? node->nxt : nullptr;
      if (head.node == node)
        head.node = node->nxt;
    }
    if (node->nxt)
      node->nxt->prv = node->prv;
    if (node->prv)
      node->prv->nxt = node->nxt;
    cache.release(node, reuse);
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A>
  inline
  t_void undo_(t_ident_<K, V, N, A> ident, r_n size) {
    destruct_(ident.val());
    --size;
    ident.node->hsh[ident.ix] = 0;
    ident.node->cnt          -= 1;
  }

  template<typename K, typename V, typename A>
  inline
  t_void undo_(t_ident_<K, V, 1, A> ident, r_n size) {
    destruct_(ident.val());
    --size;
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A>
  inline
  t_bool remove_(t_ident_<K, V, N, A> ident, r_n size,
                 t_ident_<K, V, N, A>& head) {
    undo_(ident, size);
    t_bool unlink = !ident.node->cnt;
    if (ident == head && !unlink)
      for (++head.ix; !ident.hash(); ++head.ix);
    return unlink;
  }

  template<typename K, typename V, typename A>
  inline
  t_bool remove_(t_ident_<K, V, 1, A> ident, r_n size,
                 t_ident_<K, V, 1, A>& head) {
    undo_(ident, size);
    return true;
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A>
  inline
  t_ident_<K, V, N, A> add_unique_(t_ident_<K, V, N, A>& head,
                                   t_alloc_cache_<K, V, N, A>& cache,
                                   r_n size, t_hash_id id, const K& key,
                                   t_must_add must_add,
                                   t_node_<K, V, N, A>** slot) {
    using t_traits_   = impl_::t_traits_<K, V, N, A>;
    using t_ident_    = typename t_traits_::t_ident;
    using t_node_     = typename t_traits_::t_node;
    using p_node_     = typename t_traits_::p_node;

    p_node_ node_add = nullptr;
    t_ix    ix_add   = 0;
    if (*slot) {
      p_node_ last = nullptr;
      for (p_node_ node = *slot; node && node->slot == slot; node = node->nxt) {
        for (t_ix cnt = 0, ix = 0; cnt < node->cnt; ++ix) {
          if (node->hsh[ix]) {
            if (node->hsh[ix] == id && node->Val_(ix)->key == key)
              return must_add ? t_ident_() : t_ident_{ix, node};
            ++cnt;
          } else if (!node_add) {
            node_add = node;
            ix_add   = ix;
          }
        }
        if (!node_add && node->cnt < N) {
          node_add = node;
          ix_add   = node->cnt;
        }
        last = node;
      }
      if (!node_add) {
        node_add = new (cache.acquire()) t_node_(slot, last, last->nxt);
        if (last->nxt)
          last->nxt->prv = node_add;
        last->nxt        = node_add;
      }
    } else {
      node_add = new (cache.acquire()) t_node_(slot, nullptr, head.node);
      if (node_add->nxt)
        node_add->nxt->prv = node_add;
      head.ix   = 0;
      head.node = node_add;
      *slot     = node_add;
    }
    ++size;
    node_add->cnt        += 1;
    node_add->hsh[ix_add] = id;
    return {ix_add, node_add};
  }

  template<typename K, typename V, typename A>
  inline
  t_ident_<K, V, 1, A> add_unique_(t_ident_<K, V, 1, A>& head,
                                   t_alloc_cache_<K, V, 1, A>& cache,
                                   r_n size, t_hash_id id, const K& key,
                                   t_must_add must_add,
                                   t_node_<K, V, 1, A>** slot) {
    using t_traits_   = impl_::t_traits_<K, V, 1, A>;
    using t_ident_    = typename t_traits_::t_ident;
    using t_node_     = typename t_traits_::t_node;
    using p_node_     = typename t_traits_::p_node;

    p_node_ node_add = nullptr;
    if (*slot) {
      p_node_ last = nullptr;
      for (p_node_ node = *slot; node && node->slot == slot; node = node->nxt) {
        if (node->hsh == id && node->Val_()->key == key)
          return must_add ? t_ident_() : t_ident_{node};
        last = node;
      }
      node_add = new (cache.acquire()) t_node_(slot, last, last->nxt);
      if (last->nxt)
        last->nxt->prv = node_add;
      last->nxt        = node_add;
    } else {
      node_add = new (cache.acquire()) t_node_(slot, nullptr, head.node);
      if (node_add->nxt)
        node_add->nxt->prv = node_add;
      head.node = node_add;
      *slot     = node_add;
    }
    ++size;
    node_add->hsh = id;
    return {node_add};
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A>
  inline
  t_ident_<K, V, N, A> add_unique_cp_(t_ident_<K, V, N, A>& head,
                                      t_node_<K, V, N, A>** slots,
                                      t_hash_id (&hash)(const K&) noexcept,
                                      t_alloc_cache_<K, V, N, A>& cache,
                                      r_n size, t_n max,
                                      const K& key, t_must_add must_add) {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using q_node_   = typename t_traits_::q_node;
    using t_ident_  = typename t_traits_::t_ident;

    t_hash_id id    = hash_(key, hash);
    q_node_   slot  = slot_(slots, max, id);
    t_ident_  ident = add_unique_(head, cache, size, id, key, must_add, slot);
    if (ident)
      construct_copy_(ident.val(), key);
    return ident;
  }

  template<typename K, typename V, int N, typename A>
  inline
  t_ident_<K, V, N, A> add_unique_mv_(t_ident_<K, V, N, A>& head,
                                      t_node_<K, V, N, A>** slots,
                                      t_hash_id (&hash)(const K&) noexcept,
                                      t_alloc_cache_<K, V, N, A>& cache,
                                      r_n size, t_n max,
                                      K&& key, t_must_add must_add) {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using q_node_   = typename t_traits_::q_node;
    using t_ident_  = typename t_traits_::t_ident;

    t_hash_id id    = hash_(const_cast<const K&>(key), hash);
    q_node_   slot  = slot_(slots, max, id);
    t_ident_  ident = add_unique_(head, cache, size, id, key, must_add, slot);
    if (ident)
      construct_move_(ident.val(), x_cast(key));
    return ident;
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A, int N1, typename A1>
  inline
  t_void copy_(t_n_max                     max,
               t_ident_<K, V, N, A>&       head,
               t_node_<K, V, N, A>**       slots,
               t_alloc_cache_<K, V, N, A>& cache,
               t_hash_id                 (&hash)(const K&) noexcept,
               r_n                         size,
               t_Ident_<K, V, N1, A1>      cp_head) {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using P_node_   = typename t_traits_::P_node;
    using q_node_   = typename t_traits_::q_node;
    using t_ident_  = typename t_traits_::t_ident;

    t_ix ix = cp_head.ix;
    for (P_node_ node = cp_head.node; node; node = node->nxt) {
      for (t_ix cnt = 0; cnt < node->cnt; ++ix) {
        t_hash_id id = node->hsh[ix];
        if (id) {
          ++cnt;
          q_node_  slot  = slot_(slots, max, id);
          t_ident_ ident = add_(head, cache, size, id, slot);
          construct_(ident.val(), node->Val_(ix));
        }
      }
      ix = 0;
    }
  }

  template<typename K, typename V, int N, typename A, typename A1>
  inline
  t_void copy_(t_n_max                     max,
               t_ident_<K, V, N, A>&       head,
               t_node_<K, V, N, A>**       slots,
               t_alloc_cache_<K, V, N, A>& cache,
               t_hash_id                 (&hash)(const K&) noexcept,
               r_n                         size,
               t_Ident_<K, V, 1, A1>       cp_head) {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using P_node_   = typename t_traits_::P_node;
    using q_node_   = typename t_traits_::q_node;
    using t_ident_  = typename t_traits_::t_ident;

    for (P_node_ node = cp_head.node; node; node = node->nxt) {
      t_hash_id id    = node->hsh;
      q_node_   slot  = slot_(slots, max, id);
      t_ident_  ident = add_(head, cache, size, id, slot);
      construct_(ident.val(), node->Val_());
    }
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V, int N, typename A>
  inline
  t_void copy_(t_n_max                     max,
               t_ident_<K, V, N, A>&       head,
               t_node_<K, V, N, A>**       slots,
               t_alloc_cache_<K, V, N, A>& cache,
               t_hash_id                 (&hash)(const K&) noexcept,
               r_n                         size,
               t_Ident_<K, V, N, A>        cp_head) {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using P_node_   = typename t_traits_::P_node;
    using q_node_   = typename t_traits_::q_node;
    using t_ident_  = typename t_traits_::t_ident;

    t_ix ix = cp_head.ix;
    for (P_node_ node = cp_head.node; node; node = node->nxt) {
      for (t_ix cnt = 0; cnt < node->cnt; ++ix) {
        t_hash_id id = node->hsh[ix];
        if (id) {
          ++cnt;
          q_node_  slot  = slot_(slots, max, id);
          t_ident_ ident = add_(head, cache, size, id, slot);
          construct_(ident.val(), node->Val_(ix));
        }
      }
      ix = 0;
    }
  }

  template<typename K, typename V, int N, typename A>
  inline
  t_void copy_(t_n_max                     max,
               t_ident_<K, V, N, A>&       head,
               t_node_<K, V, N, A>**       slots,
               t_alloc_cache_<K, V, N, A>& cache,
               t_hash_id                 (&hash)(const K&) noexcept,
               r_n                         size,
               t_Ident_<K, V, 1, A>        cp_head) {
    using t_traits_ = impl_::t_traits_<K, V, N, A>;
    using P_node_   = typename t_traits_::P_node;
    using q_node_   = typename t_traits_::q_node;
    using t_ident_  = typename t_traits_::t_ident;

    for (P_node_ node = cp_head.node; node; node = node->nxt) {
      t_hash_id id    = node->hsh;
      q_node_   slot  = slot_(slots, max, id);
      t_ident_  ident = add_(head, cache, size, id, slot);
      construct_(ident.val(), node->Val_());
    }
  }

  /////////////////////////////////////////////////////////////////////////////
}
}
}
#endif
