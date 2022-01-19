#ifndef _DAINTY_HASHBUF_TYPES_H_
#define _DAINTY_HASHBUF_TYPES_H_

#include "dainty_types.h"

namespace dainty
{
namespace hashbuf
{
  /////////////////////////////////////////////////////////////////////////////

  using types::t_bool;
  using types::t_void;
  using types::t_double;

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
}
}
#endif
