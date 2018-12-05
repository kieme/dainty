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

///////////////////////////////////////////////////////////////////////////////

/* todo
 * 1. prevent remotes with the same name. it breaks the system.
 * 2. remote-cli, one terminal is enough to manage many. code is setup for it.
 */

///////////////////////////////////////////////////////////////////////////////

#include <linux/tipc.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/prctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <endian.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <search.h>

///////////////////////////////////////////////////////////////////////////////

#ifndef MSG_BUF_SIZE
#define MSG_BUF_SIZE         4096
#endif

#define MAX_TEST_HOPS        10
#define MAX_MSG_LEN          64
#define MAX_NAME_LEN         32
#define MAX_PROCESSNAME_LEN  (MAX_NAME_LEN + 4) /* prx-<name> */
#define MAX_REMOTES          20
#define MAX_LOCAL_MSNGRS     10
#define MAX_LOCAL_GRPS       20
#define MAX_MSNGRS           (MAX_LOCAL_MSNGRS + MAX_LOCAL_GRPS)
#define MAX_GRP_MEMBERS      5
#define MAX_CLI_REQ_LEN      128
#define MAX_CLI_OUT_LEN      32768
#define MAX_CLI_ARGS         10
#define MAX_LKUPS            ((MAX_REMOTES + 1) * MAX_MSNGRS)
#define MAX_FULLNAME_LEN     ((2*MAX_NAME_LEN) + 1)
#define CLI_OPTIONS(opts)    (sizeof(opts)/sizeof(t_cli_option))
#define INVALID_KEY          0
#define FREE_INUSE           -1
#define NANO2SEC(nano)       (1.0e-9*nano)

///////////////////////////////////////////////////////////////////////////////

typedef double                 t_double;
typedef int                    t_int;
typedef int*                   p_int;
typedef int                    t_n;
typedef int                    t_ix;
typedef int                    t_fd;
typedef void                   t_void;
typedef void*                  p_void;
typedef const void*            P_void;
typedef char                   t_char;
typedef unsigned char          t_uchar;
typedef t_uchar*               p_uchar;
typedef t_char                 t_name[MAX_NAME_LEN];
typedef t_char                 t_fullname[MAX_FULLNAME_LEN];
typedef t_char                 t_processname[MAX_PROCESSNAME_LEN];
typedef t_char                 t_msg[MAX_MSG_LEN];
typedef t_char                 t_cli_req[MAX_CLI_REQ_LEN];
typedef t_char                 t_cli_out[MAX_CLI_OUT_LEN];
typedef char*                  p_cstr;
typedef const char*            P_cstr;
typedef p_cstr                 t_cli_argv[MAX_CLI_ARGS];
typedef uint64_t               t_uint64;
typedef t_uint64               t_key;
typedef struct sockaddr*       p_sockaddr;
typedef struct tipc_portid     t_tipc_portid;
typedef t_tipc_portid*         p_tipc_portid;
typedef const t_tipc_portid*   P_tipc_portid;
typedef struct sockaddr_tipc   t_sockaddr_tipc;
typedef const t_sockaddr_tipc* P_sockaddr_tipc;
typedef struct tipc_subscr     t_tipc_subscr;
typedef struct tipc_event      t_tipc_event;
typedef struct epoll_event     t_epoll_event;
typedef struct timespec        t_timespec;
typedef t_timespec*            p_timespec;
typedef const t_timespec*      P_timespec;

///////////////////////////////////////////////////////////////////////////////

typedef enum bool { false, true } t_bool;
typedef t_bool* p_bool;

typedef enum cli_indent { I0 = 0, I1, I2, I3, I4 } t_cli_indent;
typedef t_void (*p_cli_out_flush)(p_void ctxt);

typedef enum prx_tipc_scope { PRX_TIPC_SYSTEM  = 11167,
                              PRX_TIPC_NETWORK = 31188 } t_prx_tipc_scope;

typedef enum prx_scope { PRX_SCOPE_NONE = 0,
                         PRX_SCOPE_PROCESS,
                         PRX_SCOPE_SYSTEM,
                         PRX_SCOPE_NETWORK } t_prx_scope;

typedef enum msngr_scope { MSNGR_SCOPE_NONE = 0,
                           MSNGR_SCOPE_OFF,
                           MSNGR_SCOPE_PROCESS,
                           MSNGR_SCOPE_SYSTEM,
                           MSNGR_SCOPE_NETWORK } t_msngr_scope;

typedef enum msg_type { MSG_NONE = 0, MSG_USER, MSG_TEST, MSG_CLI } t_msg_type;

typedef enum cli_type { CLI_REQ, CLI_RESP, CLI_QUIT } t_cli_type;

typedef enum msngr_req { MSNGR_REQ_NONE = 0,
                         MSNGR_REQ_ADD,
                         MSNGR_REQ_DEL } t_msngr_req;
typedef const t_msngr_req* P_msngr_req;

typedef t_msngr_req t_msngr_reqs[MAX_MSNGRS];

typedef struct cli_option {
  P_cstr name;
  t_int  value;
} t_cli_option;
typedef const t_cli_option* P_cli_option;

typedef struct msngr_ctxt {
  t_msngr_scope scope;
  t_name        name;
  t_key         key;
} t_msngr_ctxt;
typedef t_msngr_ctxt*       p_msngr_ctxt;
typedef const t_msngr_ctxt* P_msngr_ctxt;

typedef struct lk_ctxt {
  t_ix       free;
  t_key      key;
  t_fullname fullname;
  t_n        mons_cnt;
  t_n        members_cnt;
  t_bool     mons[MAX_LOCAL_MSNGRS];
  t_key      members[MAX_GRP_MEMBERS];
  p_void     ctxt; // required for twalk - to access cli_event
} t_lk_ctxt;
typedef t_lk_ctxt*       p_lk_ctxt;
typedef const t_lk_ctxt* P_lk_ctxt;

typedef struct remote_ctxt {
  t_prx_scope   scope;
  t_bool        verified;
  t_bool        advertice;
  t_name        name;
  t_tipc_portid portid;
  t_tipc_portid msg_portid;
  t_msngr_ctxt  msngrs[MAX_MSNGRS];
} t_remote_ctxt;

typedef struct test_ctxt {
  t_ix       remote;
  t_ix       fwd_remote;
  t_bool     running;
  t_n        echos;
  t_n        rx;
  t_n        tx;
  t_timespec start;
  t_timespec stop;
} t_test_ctxt;

typedef struct cli_ctxt {
  t_bool          enable;
  t_cli_req       req;
  t_cli_out       out;
  t_n             out_len;
  t_ix            req_remote;
  t_ix            ans_remote;
  p_cli_out_flush flush;
  t_sockaddr_tipc to_addr;
} t_cli_ctxt;

typedef struct ctxt {
  t_prx_scope     scope;
  t_name          name;
  t_fd            epoll_fd;
  t_fd            system_fd;
  t_fd            network_fd;
  t_fd            service_fd;
  t_fd            msg_fd;
  t_sockaddr_tipc system_addr;
  t_sockaddr_tipc network_addr;
  t_tipc_portid   service_portid;
  t_tipc_portid   system_portid;
  t_tipc_portid   network_portid;
  t_tipc_portid   msg_portid;
  t_tipc_subscr   system_subscr;
  t_tipc_subscr   network_subscr;
  t_remote_ctxt   remotes[MAX_REMOTES];
  t_msngr_ctxt    msngrs[MAX_MSNGRS];
  t_lk_ctxt       lkups[MAX_LKUPS];
  t_ix            free_lkup;
  p_void          lk;
  t_test_ctxt     test;
  t_cli_ctxt      cli;
} t_ctxt;
typedef t_ctxt* p_ctxt;

typedef struct msngr_req_ctxt {
  t_msngr_req  req;
  t_msngr_ctxt msngr;
} t_msngr_req_ctxt;

typedef struct protocol_pkt {
  t_prx_scope      scope;
  t_name           name;
  t_tipc_portid    msg_portid;
  t_n              cnt;
  t_msngr_req_ctxt msngrs[MAX_MSNGRS];
} t_protocol_pkt;
typedef t_protocol_pkt* p_protocol_pkt;

typedef struct cli_msg {
  t_cli_type type;
  t_n        len;
  char data[MSG_BUF_SIZE];
} t_cli_msg;

typedef struct test_msg {
  t_n           hop;
  t_n           hop_depth;
  t_tipc_portid portids[MAX_TEST_HOPS];
} t_test_msg;

typedef struct user_msg {
  t_key      src; // not sure if src should be included
  t_key      dst;
  t_n        len;
  t_msg      msg;
} t_user_msg;
typedef t_user_msg* p_user_msg;

typedef struct msg_pkt {
  t_msg_type type;
  union {
    t_user_msg user;
    t_test_msg test;
    t_cli_msg  cli;
    char       dummy_[MSG_BUF_SIZE];
  };
} t_msg_pkt;
typedef t_msg_pkt* p_msg_pkt;

///////////////////////////////////////////////////////////////////////////////

t_void exit_program(P_cstr msg) {
  perror(msg);
  fflush(stderr);
  exit(1);
}

t_void cli_out_append_string(p_ctxt ctxt, P_cstr string) {
  ctxt->cli.out_len += snprintf(ctxt->cli.out + ctxt->cli.out_len,
                                sizeof(ctxt->cli.out) - ctxt->cli.out_len,
                                "%s", string); // very inefficient.
}

t_void cli_out_append_format_v(p_ctxt ctxt, P_cstr fmt, va_list ap) {
  ctxt->cli.out_len += vsnprintf(ctxt->cli.out + ctxt->cli.out_len,
                                 sizeof(ctxt->cli.out) - ctxt->cli.out_len,
                                 fmt, ap);
}

t_void cli_out_append_format(p_ctxt ctxt, P_cstr fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  cli_out_append_format_v(ctxt, fmt, ap);
  va_end(ap);
}

t_void cli_out_flush_stdout(p_void _ctxt) {
  p_ctxt ctxt = (p_ctxt)_ctxt;
  if (ctxt->cli.enable && ctxt->cli.out_len) {
    printf("%s", ctxt->cli.out);
    fflush(stdout);
  }
  ctxt->cli.out[0]  = '\0';
  ctxt->cli.out_len = 0;
}

t_void cli_prompt(p_ctxt ctxt, t_bool prompt) {
  if (prompt && !ctxt->cli.req_remote) {
    if (!ctxt->cli.ans_remote) {
      if (ctxt->cli.enable)
        cli_out_append_format(ctxt, "%s $> ", ctxt->name);
    } else
      cli_out_append_format(ctxt, "remote:%s $> ", ctxt->name);
  }
  ctxt->cli.flush(ctxt);
}

t_void cli_event(p_ctxt ctxt, t_cli_indent indent, P_cstr fmt, ...) {
  switch (indent) {
    case I0: cli_out_append_format(ctxt, "%s ", ctxt->name);         break;
    case I1: cli_out_append_format(ctxt, "%s   ", ctxt->name);       break;
    case I2: cli_out_append_format(ctxt, "%s     ", ctxt->name);     break;
    case I3: cli_out_append_format(ctxt, "%s       ", ctxt->name);   break;
    case I4: cli_out_append_format(ctxt, "%s         ", ctxt->name); break;
  }
  va_list ap;
  va_start(ap, fmt);
  cli_out_append_format_v(ctxt, fmt, ap);
  va_end(ap);
  cli_out_append_format(ctxt, "\n");
}

t_void prx_event(p_ctxt ctxt, P_cstr fmt, ...) {
  cli_out_append_format(ctxt, "%s <prx event>   : ", ctxt->name);
  va_list ap;
  va_start(ap, fmt);
  cli_out_append_format_v(ctxt, fmt, ap);
  va_end(ap);
  cli_out_append_format(ctxt, "\n");
}

t_void msngr_event(p_ctxt ctxt, P_cstr fmt, ...) {
  cli_out_append_format(ctxt, "%s <msngr event> : ", ctxt->name);
  va_list ap;
  va_start(ap, fmt);
  cli_out_append_format_v(ctxt, fmt, ap);
  va_end(ap);
  cli_out_append_format(ctxt, "\n");
}

t_void test_event(p_ctxt ctxt, P_cstr fmt, ...) {
  cli_out_append_format(ctxt, "%s <test event>  : ", ctxt->name);
  va_list ap;
  va_start(ap, fmt);
  cli_out_append_format_v(ctxt, fmt, ap);
  va_end(ap);
  cli_out_append_format(ctxt, "\n");
}

///////////////////////////////////////////////////////////////////////////////

t_double delta_secs(P_timespec start, P_timespec stop) {
  return (t_double)stop->tv_sec - (t_double)start->tv_sec +
          NANO2SEC(stop->tv_nsec) - NANO2SEC(start->tv_nsec);
}

t_key htonk(t_key key) {
  return htobe64(key);
}

t_key ntohk(t_key key) {
  return be64toh(key);
}

t_prx_scope transform(t_prx_tipc_scope scope) {
  switch (scope) {
    case PRX_TIPC_SYSTEM:  return PRX_SCOPE_SYSTEM;
    case PRX_TIPC_NETWORK: return PRX_SCOPE_NETWORK;
  }
  exit_program("tipc scope to internal scope failed\n");
  return PRX_SCOPE_NONE;
}

P_cstr prx_tipc_scope_cstr(t_prx_tipc_scope scope) {
  if (scope == PRX_TIPC_SYSTEM)
    return "system";
  if (scope == PRX_TIPC_NETWORK)
    return "network";
  return "unknown";
}

P_cstr msngr_scope_cstr(t_msngr_scope scope) {
  P_cstr tbl[] = { "none", "off", "process", "system", "network" };
  return tbl[scope];
}

P_cstr prx_scope_cstr(t_prx_scope scope) {
  P_cstr tbl[] = { "none", "process", "system", "network"};
  return tbl[scope];
}

///////////////////////////////////////////////////////////////////////////////

t_bool is_remote_key(t_key key) {
  return (0xFFFF & (key >> 16)) ? true : false;
}

t_bool is_local_key(t_key key) {
  return is_remote_key(key) ? false : true;
}

t_bool is_group_key(t_key key) {
  return (0x1 & key) ? true : false;
}

t_ix get_key_remote(t_key key) {
  return (0xFFFF & (key >> 16)) - 1;
}

t_ix get_key_msngr(t_key key) {
  return (0x7FFF & (key >> 1)) - 1;
}

t_ix get_key_lkup(t_key key) {
  return (0xFFFF & (key >> 32)) - 1;
}

t_key get_key_hash(t_key key) {
  return 0xFFFF & (key >> 48);
}

t_key make_key_hash() {
  return (0xFFFF & (t_key)rand()) << 48;
}

t_key make_key(t_ix lkup, t_ix msngr, t_bool grp) {
  return make_key_hash() |
         ((0xFFFF & ((t_key)lkup + 1))  << 32) |
         ((0x7FFF & ((t_key)msngr + 1)) << 1)  |
         (grp ? 1 : 0);
}

t_key make_remote_key(t_key key, t_ix lkup, t_ix remote, t_ix msngr) {
  return (0xFFFF000000000000 & key) |
         ((0xFFFF & ((t_key)lkup   + 1)) << 32) |
         ((0xFFFF & ((t_key)remote + 1)) << 16) |
         ((0x7FFF & ((t_key)msngr  + 1)) << 1)  |
         (0x1 & key);
}

///////////////////////////////////////////////////////////////////////////////

t_void mk_processname(p_cstr dst, P_cstr prx) {
  strcpy(dst, "prx-");
  strcat(dst, prx);
}

t_void mk_fullname(p_cstr dst, P_cstr prx, P_cstr msngr) {
  strcpy(dst, prx);
  strcat(dst, "/");
  strcat(dst, msngr);
}

t_void cp_fullname(p_cstr dst, P_cstr src) {
  strcpy(dst, src);
}

t_void cp_name(p_cstr dst, P_cstr src) {
  strcpy(dst, src);
}

t_void cp_msg(p_cstr dst, P_cstr src) {
  strcpy(dst, src);
}

t_void cp_portid(p_tipc_portid dst, P_tipc_portid src) {
  memcpy(dst, src, sizeof(t_tipc_portid));
}

t_void cp_msngr(p_msngr_ctxt dst, P_msngr_ctxt src) {
  memcpy(dst, src, sizeof(t_msngr_ctxt));
}

t_bool eq_name(P_cstr lh, P_cstr rh) {
  return strcmp(lh, rh) == 0 ? true : false;
}

t_bool eq_str(P_cstr lh, P_cstr rh) {
  return strcmp(lh, rh) == 0 ? true : false;
}

t_bool eq_portid(P_tipc_portid dst, P_tipc_portid src) {
  return memcmp(dst, src, sizeof(t_tipc_portid)) == 0 ? true : false;
}

t_void init_ctxt(p_ctxt ctxt) {
  memset(ctxt, 0, sizeof(t_ctxt));
  for (t_ix lkup = 0; lkup < MAX_LKUPS; ++lkup) {
    ctxt->lkups[lkup].free = 1 + lkup;
    ctxt->lkups[lkup].ctxt = ctxt;
  }
  ctxt->cli.flush = cli_out_flush_stdout;
}

t_void init_lkup(p_lk_ctxt lk_ctxt, p_ctxt ctxt) {
  memset(lk_ctxt, 0, sizeof(t_lk_ctxt));
  lk_ctxt->ctxt = ctxt;
}

///////////////////////////////////////////////////////////////////////////////

t_bool is_name_valid(P_cstr name) {
  t_n n = 0;
  for (; *name; ++name) {
    if (*name == '/')
      return false;
    else
      ++n;
  }
  return n;
}

t_bool is_fullname_valid(P_cstr name) {
  t_n n1 = 0, n2 = 0;
  for (; *name; ++name) {
    if (*name == '/') {
      for (++name; *name; ++name) {
        if (*name == '/')
          return false;
        else
          ++n2;
      }
    } else
      ++n1;
  }
  return n1 && n2;
}

///////////////////////////////////////////////////////////////////////////////

p_cstr cli_strip_space(p_cstr input) {
  for (; *input && *input == ' '; ++input);
  return input;
}

t_bool cli_parse_args(p_cstr input, p_int argc, p_cstr argv[]) {
  *argc = 0;
  for (p_cstr p = cli_strip_space(input); *p; p = cli_strip_space(p)) {
    input = p;
    if (*p != '<') {
      for (; *p && *p != ' '; ++p);
      if (*p)
        *p++ = '\0';
    } else {
      input = ++p;
      for (; *p && *p != '>'; ++p);
      if (*p)
        *p++ = '\0';
    }
    if (*argc + 1 > MAX_CLI_ARGS)
      return false;
    argv[(*argc)++] = input;
  }
  return true;
}

t_int cli_find(P_cstr arg, t_int init, P_cli_option opts, t_n opts_max) {
  for (t_n option = 0; option < opts_max; ++option)
    if (eq_str(opts[option].name, arg))
      return opts[option].value;
  return init;
}

///////////////////////////////////////////////////////////////////////////////

t_int lk_compare(P_void key1,  P_void key2) {
  P_lk_ctxt ctxt1 = key1;
  P_lk_ctxt ctxt2 = key2;
  return strcmp(ctxt1->fullname, ctxt2->fullname);
}

t_void lk_action(P_void node, const VISIT which, const int depth) {
  p_lk_ctxt lk_ctxt = NULL;
  switch (which) {
    case postorder:
    case leaf:
      lk_ctxt = *(t_lk_ctxt**)node;
      break;
    default:
      break;
  }
  if (lk_ctxt) {
    p_ctxt ctxt = (p_ctxt)lk_ctxt->ctxt; // ugly - no alternative
    cli_event(ctxt, I2, "lkup: %s, key = 0x%" PRIx64, lk_ctxt->fullname,
              lk_ctxt->key);
    t_n cnt = 0;
    for (t_ix msngr = 0; msngr < MAX_LOCAL_MSNGRS; ++msngr)
      if (lk_ctxt->mons[msngr])
         cli_event(ctxt, I3, "(%d) monitored by msngr id:%d", ++cnt, msngr);
  }
}

t_ix insert_lkup(p_ctxt ctxt, P_cstr fullname) {
  t_ix free_lkup = ctxt->free_lkup;
  if (free_lkup < MAX_LKUPS) {
    cp_fullname(ctxt->lkups[free_lkup].fullname, fullname);
    p_void result = tsearch(&ctxt->lkups[free_lkup], &ctxt->lk, lk_compare);
    if (*(t_lk_ctxt**)result == &ctxt->lkups[free_lkup]) {
      ctxt->free_lkup = ctxt->lkups[free_lkup].free;
      ctxt->lkups[free_lkup].free = FREE_INUSE;
      return free_lkup;
    } else if (result) {
      ctxt->lkups[free_lkup].fullname[0] = '\0';
      p_lk_ctxt lk_ctxt = *(t_lk_ctxt**)result;
      return lk_ctxt - &ctxt->lkups[0];
    } else
      prx_event(ctxt, "could not enter messenger in lk");
  } else
    prx_event(ctxt, "max lookups installed already");
  return MAX_LKUPS;
}

t_ix find_lkup(p_ctxt ctxt, P_cstr fullname) {
  t_lk_ctxt lk_ctxt;
  cp_fullname(lk_ctxt.fullname, fullname);

  p_void result = tfind(&lk_ctxt, &ctxt->lk, lk_compare);
  if (result) {
    p_lk_ctxt lk_ctxt = *(t_lk_ctxt**)result;
    return lk_ctxt - &ctxt->lkups[0];
  }
  return MAX_LKUPS;
}

t_void remove_lkup(p_ctxt ctxt, t_ix lkup) {
  tdelete(&ctxt->lkups[lkup], &ctxt->lk, lk_compare);
  init_lkup(&ctxt->lkups[lkup], ctxt);

  ctxt->lkups[lkup].free = ctxt->free_lkup;
  ctxt->free_lkup = lkup;
}

t_void maybe_cleanup_lkup(p_ctxt ctxt, t_ix lkup) {
  if (ctxt->lkups[lkup].key == INVALID_KEY &&
      !ctxt->lkups[lkup].mons_cnt && !ctxt->lkups[lkup].members_cnt)
    remove_lkup(ctxt, lkup);
}

///////////////////////////////////////////////////////////////////////////////

t_void notify_messenger(p_ctxt ctxt, t_ix lkup, t_ix msngr) {
  if (ctxt->lkups[lkup].key != INVALID_KEY) {
    if (ctxt->msngrs[msngr].scope > MSNGR_SCOPE_OFF)
      msngr_event(ctxt, "notify %s that %s(0x%" PRIx64 ") became available",
                  ctxt->msngrs[msngr].name,
                  ctxt->lkups[lkup].fullname,
                  ctxt->lkups[lkup].key);
  } else if (ctxt->msngrs[msngr].scope > MSNGR_SCOPE_OFF)
    msngr_event(ctxt, "notify %s that %s became unavailable",
                ctxt->msngrs[msngr].name,
                ctxt->lkups[lkup].fullname);
}

t_void notify_messengers(p_ctxt ctxt, t_ix lkup) {
  if (ctxt->lkups[lkup].mons_cnt) {
    for (t_ix n = 0; n < MAX_LOCAL_MSNGRS; ++n)
      if (ctxt->lkups[lkup].mons[n])
        notify_messenger(ctxt, lkup, n);
  }
}

///////////////////////////////////////////////////////////////////////////////

t_bool insert_monitor(p_ctxt ctxt, t_ix msngr, P_cstr fullname) {
  t_ix lkup = insert_lkup(ctxt, fullname);
  if (lkup < MAX_LKUPS) {
    if (!ctxt->lkups[lkup].mons[msngr]) {
      ctxt->lkups[lkup].mons[msngr] = true;
      ctxt->lkups[lkup].mons_cnt   += 1;
      if (ctxt->lkups[lkup].key != INVALID_KEY)
        notify_messenger(ctxt, lkup, msngr);
    }
    return true;
  } else
    prx_event(ctxt, "couldn't insert lk");
  return false;
}

t_bool find_monitor(p_ctxt ctxt, t_ix msngr, P_cstr fullname) {
  t_ix lkup = find_lkup(ctxt, fullname);
  if (lkup < MAX_LKUPS) {
    if (ctxt->lkups[lkup].mons[msngr])
      return true;
  }
  return false;
}

t_bool remove_monitor(p_ctxt ctxt, t_ix msngr, P_cstr fullname) {
  t_ix lkup = find_lkup(ctxt, fullname);
  if (lkup < MAX_LKUPS) {
    if (ctxt->lkups[lkup].mons[msngr]) {
      ctxt->lkups[lkup].mons[msngr] = false;
      ctxt->lkups[lkup].mons_cnt   -= 1;
      maybe_cleanup_lkup(ctxt, lkup);
      return true;
    } else
      prx_event(ctxt, "no monitor to remove");
  } else
    prx_event(ctxt, "unknow fullname");
  return false;
}

t_void remove_monitors(p_ctxt ctxt, t_ix msngr) {
  for (t_ix lkup = 0; lkup < MAX_LKUPS; ++lkup) {
    if (ctxt->lkups[lkup].free == FREE_INUSE) {
      if (ctxt->lkups[lkup].mons[msngr]) {
        ctxt->lkups[lkup].mons[msngr] = false;
        ctxt->lkups[lkup].mons_cnt   -= 1;
        maybe_cleanup_lkup(ctxt, lkup);
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////

t_bool attach_grp_member(p_ctxt ctxt, P_cstr name, t_key key) {
  t_fullname fullname;
  mk_fullname(fullname, ctxt->name, name);

  t_ix lkup = insert_lkup(ctxt, fullname);
  if (lkup < MAX_LKUPS) {
    if (ctxt->lkups[lkup].members_cnt < MAX_GRP_MEMBERS) {
      for (t_ix member = 0; member < MAX_GRP_MEMBERS; ++member)
        if (ctxt->lkups[lkup].members[member] == key)
          return true;

      for (t_ix member = 0; member < MAX_GRP_MEMBERS; ++member) {
        if (ctxt->lkups[lkup].members[member] == INVALID_KEY) {
          ctxt->lkups[lkup].members[member] = key;
          ctxt->lkups[lkup].members_cnt    += 1;
          return true;
        }
      }
    }
  }
  return false;
}

t_void detach_grp_member(p_ctxt ctxt, P_cstr name, t_key key) {
  t_fullname fullname;
  mk_fullname(fullname, ctxt->name, name);

  t_ix lkup = find_lkup(ctxt, fullname);
  if (lkup < MAX_LKUPS) {
    if (ctxt->lkups[lkup].members_cnt) {
      for (t_ix member = 0; member < MAX_GRP_MEMBERS; ++member) {
        if (ctxt->lkups[lkup].members[member] == key) {
          ctxt->lkups[lkup].members[member] = INVALID_KEY;
          ctxt->lkups[lkup].members_cnt    -= 1;
          maybe_cleanup_lkup(ctxt, lkup);
          break;
        }
      }
    }
  }
}

t_void remove_members(p_ctxt ctxt, t_key key) {
  for (t_ix lkup = 0; lkup < MAX_LKUPS; ++lkup) {
    if (ctxt->lkups[lkup].free == FREE_INUSE) {
      if (ctxt->lkups[lkup].members_cnt) {
        for (t_ix member = 0; member < MAX_GRP_MEMBERS; ++member) {
          if (ctxt->lkups[lkup].members[member] == key) {
            ctxt->lkups[lkup].members[member] = INVALID_KEY;
            ctxt->lkups[lkup].members_cnt    -= 1;
            maybe_cleanup_lkup(ctxt, lkup);
            break;
          }
        }
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////

t_ix insert_msngr(p_ctxt ctxt, P_cstr name, t_msngr_scope scope,
                  t_bool is_grp) {
  t_fullname fullname;
  mk_fullname(fullname, ctxt->name, name);

  t_ix lkup = find_lkup(ctxt, fullname);
  if (lkup == MAX_LKUPS)
    lkup = insert_lkup(ctxt, fullname);
  else if (ctxt->lkups[lkup].key != INVALID_KEY)
    lkup = MAX_LKUPS;

  if (lkup < MAX_LKUPS) {
    t_ix begin = is_grp ? MAX_LOCAL_MSNGRS : 0;
    t_ix end   = is_grp ? MAX_MSNGRS       : MAX_LOCAL_MSNGRS;

    if (!ctxt->lkups[lkup].members_cnt || is_grp) {
      for (t_ix msngr = begin; msngr < end; ++msngr) {
        if (ctxt->msngrs[msngr].scope == MSNGR_SCOPE_NONE) {
          ctxt->msngrs[msngr].scope = scope;
          ctxt->msngrs[msngr].key   = make_key(lkup, msngr, is_grp);
          cp_name(ctxt->msngrs[msngr].name, name);
          ctxt->lkups[lkup].key = ctxt->msngrs[msngr].key;

          prx_event(ctxt, "learn messenger %s(%s) - key : 0x%" PRIx64, name,
                    msngr_scope_cstr(ctxt->msngrs[msngr].scope),
                    ctxt->msngrs[msngr].key);

          notify_messengers(ctxt, lkup);
          return msngr;
        }
      }
      maybe_cleanup_lkup(ctxt, lkup);
    }
  }
  return MAX_MSNGRS;
}

t_ix find_msngr(p_ctxt ctxt, P_cstr name) {
  t_fullname fullname;
  mk_fullname(fullname, ctxt->name, name);

  t_ix lkup = find_lkup(ctxt, fullname);
  t_key key = ctxt->lkups[lkup].key;
  if (lkup < MAX_LKUPS && key != INVALID_KEY && is_local_key(key))
    return get_key_msngr(key);
  return MAX_MSNGRS;
}

t_void remove_msngr(p_ctxt ctxt, t_ix msngr) {
  prx_event(ctxt, "unlearn messenger %s(%s) - key : 0x%" PRIx64,
            ctxt->msngrs[msngr].name,
            msngr_scope_cstr(ctxt->msngrs[msngr].scope),
            ctxt->msngrs[msngr].key);

  remove_monitors(ctxt, msngr);
  remove_members(ctxt,  ctxt->msngrs[msngr].key);

  t_ix lkup = get_key_lkup(ctxt->msngrs[msngr].key);

  ctxt->lkups[lkup].key       = INVALID_KEY;
  ctxt->msngrs[msngr].scope   = MSNGR_SCOPE_NONE;
  ctxt->msngrs[msngr].name[0] = '\0';
  ctxt->msngrs[msngr].key     = INVALID_KEY;

  notify_messengers(ctxt, lkup);
  maybe_cleanup_lkup(ctxt, lkup);
}

t_void remove_msngrs(p_ctxt ctxt) {
  for (t_ix msngr = 0; msngr < MAX_LOCAL_MSNGRS; ++msngr)
    if (ctxt->msngrs[msngr].scope != MSNGR_SCOPE_NONE)
      remove_msngr(ctxt, msngr);
}

///////////////////////////////////////////////////////////////////////////////

t_ix insert_remote_msngr(p_ctxt ctxt, t_ix remote,
                         p_msngr_ctxt msngr_ctxt) {
  t_fullname fullname;
  mk_fullname(fullname, ctxt->remotes[remote].name, msngr_ctxt->name);

  t_ix lkup = find_lkup(ctxt, fullname);
  if (lkup == MAX_LKUPS)
    lkup = insert_lkup(ctxt, fullname);
  else if (ctxt->lkups[lkup].key != INVALID_KEY)
    lkup = MAX_LKUPS;

  if (lkup < MAX_LKUPS && !ctxt->lkups[lkup].members_cnt) {
    for (t_ix msngr = 0; msngr < MAX_MSNGRS; ++msngr) {
      if (ctxt->remotes[remote].msngrs[msngr].scope == MSNGR_SCOPE_NONE) {
        cp_msngr(&ctxt->remotes[remote].msngrs[msngr], msngr_ctxt);
        ctxt->lkups[lkup].key = make_remote_key(
          ctxt->remotes[remote].msngrs[msngr].key, lkup, remote, msngr);

        prx_event(ctxt, "learn remote messenger %s(%s) - key : 0x%" PRIx64,
                  fullname,
                   msngr_scope_cstr(ctxt->remotes[remote].msngrs[msngr].scope),
                  ctxt->remotes[remote].msngrs[msngr].key);

        notify_messengers(ctxt, lkup);
        return msngr;
      }
    }
    maybe_cleanup_lkup(ctxt, lkup);
  }
  return MAX_MSNGRS;
}

t_void insert_verified_remote_msngrs(p_ctxt ctxt, t_ix remote) {
  prx_event(ctxt, "remote %s verified", ctxt->remotes[remote].name);

  for (t_ix msngr = 0; msngr < MAX_MSNGRS; ++msngr) {
    if (ctxt->remotes[remote].msngrs[msngr].scope != MSNGR_SCOPE_NONE) {
      t_fullname fullname;
      mk_fullname(fullname, ctxt->remotes[remote].name,
                  ctxt->remotes[remote].msngrs[msngr].name);

      t_ix lkup = find_lkup(ctxt, fullname);
      if (lkup == MAX_LKUPS)
        lkup = insert_lkup(ctxt, fullname);
      else if (ctxt->lkups[lkup].key != INVALID_KEY)
        lkup = MAX_LKUPS;

      if (lkup < MAX_LKUPS && !ctxt->lkups[lkup].members_cnt) {
        ctxt->lkups[lkup].key = make_remote_key(
          ctxt->remotes[remote].msngrs[msngr].key, lkup, remote, msngr);

        prx_event(ctxt, "learn remote messenger %s(%s) - key : 0x%" PRIx64,
                  fullname,
                  msngr_scope_cstr(ctxt->remotes[remote].msngrs[msngr].scope),
                  ctxt->remotes[remote].msngrs[msngr].key);

        notify_messengers(ctxt, lkup);
      } else {
        prx_event(ctxt, "discard remote messenger %s. name already used",
                  fullname);

        ctxt->remotes[remote].msngrs[msngr].scope   = MSNGR_SCOPE_NONE;
        ctxt->remotes[remote].msngrs[msngr].key     = INVALID_KEY;
        ctxt->remotes[remote].msngrs[msngr].name[0] = '\0';
      }
    }
  }
}

t_ix find_remote_msngr(p_ctxt ctxt, t_ix remote, p_msngr_ctxt msngr_ctxt) {
  for (t_ix msngr = 0; msngr < MAX_MSNGRS; ++msngr)
    if (ctxt->remotes[remote].msngrs[msngr].scope != MSNGR_SCOPE_NONE &&
        eq_name(ctxt->remotes[remote].msngrs[msngr].name, msngr_ctxt->name))
      return msngr;
  return MAX_MSNGRS;
}

t_void remove_remote_msngr(p_ctxt ctxt, t_ix remote, t_ix msngr) {
  t_fullname fullname;
  mk_fullname(fullname, ctxt->remotes[remote].name,
              ctxt->remotes[remote].msngrs[msngr].name);

  t_ix lkup = find_lkup(ctxt, fullname);
  if (lkup < MAX_LKUPS) {
    prx_event(ctxt, "unlearn remote messenger %s(%s) - key : 0x%" PRIx64,
              fullname,
              msngr_scope_cstr(ctxt->remotes[remote].msngrs[msngr].scope),
              ctxt->remotes[remote].msngrs[msngr].key);

    remove_members(ctxt, ctxt->lkups[lkup].key);
    ctxt->lkups[lkup].key = INVALID_KEY;
    notify_messengers(ctxt, lkup);

    ctxt->remotes[remote].msngrs[msngr].scope   = MSNGR_SCOPE_NONE;
    ctxt->remotes[remote].msngrs[msngr].key     = INVALID_KEY;
    ctxt->remotes[remote].msngrs[msngr].name[0] = '\0';

    maybe_cleanup_lkup(ctxt, lkup);
  }
}

///////////////////////////////////////////////////////////////////////////////

t_ix insert_remote(p_ctxt ctxt, t_prx_scope scope, P_tipc_portid portid) {
  for (t_ix remote = 0; remote < MAX_REMOTES; ++remote) {
    if (ctxt->remotes[remote].scope == PRX_SCOPE_NONE) {
      ctxt->remotes[remote].name[0] = '\0';
      ctxt->remotes[remote].scope = scope;
      cp_portid(&ctxt->remotes[remote].portid, portid);
      return remote;
    }
  }
  return MAX_REMOTES;
}

t_ix find_remote(p_ctxt ctxt, P_tipc_portid portid) {
  for (t_ix remote = 0; remote < MAX_REMOTES; ++remote)
    if (ctxt->remotes[remote].scope != PRX_SCOPE_NONE &&
        eq_portid(&ctxt->remotes[remote].portid, portid))
      return remote;
  return MAX_REMOTES;
}

t_ix find_remote_by_name(p_ctxt ctxt, P_cstr name) {
  for (t_ix remote = 0; remote < MAX_REMOTES; ++remote)
    if (ctxt->remotes[remote].scope != PRX_SCOPE_NONE &&
        eq_name(ctxt->remotes[remote].name, name))
      return remote;
  return MAX_REMOTES;
}

t_void remove_remote(p_ctxt ctxt, t_ix remote) {
  if (ctxt->remotes[remote].verified) {
    for (t_ix msngr = 0; msngr < MAX_MSNGRS; ++msngr) {
      if (ctxt->remotes[remote].msngrs[msngr].scope != MSNGR_SCOPE_NONE)
        remove_remote_msngr(ctxt, remote, msngr);
    }
  } else {
    for (t_ix msngr = 0; msngr < MAX_MSNGRS; ++msngr) {
      if (ctxt->remotes[remote].msngrs[msngr].scope != MSNGR_SCOPE_NONE) {
        ctxt->remotes[remote].msngrs[msngr].scope   = MSNGR_SCOPE_NONE;
        ctxt->remotes[remote].msngrs[msngr].key     = INVALID_KEY;
        ctxt->remotes[remote].msngrs[msngr].name[0] = '\0';
      }
    }
  }

  if (ctxt->test.remote && (ctxt->test.remote - 1 == remote)) {
    if (ctxt->test.running) {
      ctxt->test.running = false;
      clock_gettime(CLOCK_MONOTONIC, &ctxt->test.stop);
      test_event(ctxt, "test remote has left");
    }
    ctxt->test.remote = 0;
  }

  if (ctxt->test.fwd_remote && (ctxt->test.fwd_remote - 1 == remote)) {
    ctxt->test.fwd_remote = 0;
    test_event(ctxt, "test fwd remote has left");
  }

  if (ctxt->cli.req_remote && (ctxt->cli.req_remote - 1 == remote)) {
    ctxt->cli.req_remote = 0;
    ctxt->cli.ans_remote = 0;
    prx_event(ctxt, "remote cli connection broken");
  }

  if (ctxt->cli.ans_remote && (ctxt->cli.ans_remote - 1 == remote)) {
    ctxt->cli.req_remote = 0;
    ctxt->cli.ans_remote = 0;
    ctxt->cli.flush = cli_out_flush_stdout;
    prx_event(ctxt, "remote cli connection broken");
  }

  ctxt->remotes[remote].verified  = false;
  ctxt->remotes[remote].advertice = false;
  ctxt->remotes[remote].scope     = PRX_SCOPE_NONE;
  ctxt->remotes[remote].name[0]   = '\0';
}

t_void remove_remotes(p_ctxt ctxt, t_prx_scope scope) {
  for (t_ix remote = 0; remote < MAX_REMOTES; ++remote)
    if (ctxt->remotes[remote].scope == scope)
      remove_remote(ctxt, remote);
}

///////////////////////////////////////////////////////////////////////////////

t_void do_protocol_encode(p_protocol_pkt pkt, t_prx_scope scope,
                          P_cstr name, P_tipc_portid msg_portid,
                          t_n max_reqs, P_msngr_req reqs,
                          P_msngr_ctxt msngrs) {
  pkt->scope = htonl(scope);
  cp_name(pkt->name, name);

  pkt->msg_portid.ref  = htonl(msg_portid->ref);
  pkt->msg_portid.node = htonl(msg_portid->node);

  pkt->cnt = 0;
  for (t_ix msngr = 0; msngr < max_reqs; ++msngr) {
    if (reqs[msngr] != MSNGR_REQ_NONE) {
      pkt->msngrs[pkt->cnt].req = htonl(reqs[msngr]);
      cp_name(pkt->msngrs[pkt->cnt].msngr.name, msngrs[msngr].name);
      pkt->msngrs[pkt->cnt].msngr.key   = htonk(msngrs[msngr].key);
      pkt->msngrs[pkt->cnt].msngr.scope = htonl(msngrs[msngr].scope);
      ++pkt->cnt;
    }
  }
  pkt->cnt = htonl(pkt->cnt);
}

t_void do_protocol_decode(p_protocol_pkt pkt) {
  pkt->scope = ntohl(pkt->scope);
  pkt->cnt   = ntohl(pkt->cnt);

  pkt->msg_portid.ref  = htonl(pkt->msg_portid.ref);
  pkt->msg_portid.node = htonl(pkt->msg_portid.node);

  for (t_ix msngr = 0; msngr < pkt->cnt; ++msngr) {
    pkt->msngrs[msngr].req         = ntohl(pkt->msngrs[msngr].req);
    pkt->msngrs[msngr].msngr.key   = ntohk(pkt->msngrs[msngr].msngr.key);
    pkt->msngrs[msngr].msngr.scope = ntohl(pkt->msngrs[msngr].msngr.scope);
  }
}

t_void do_protocol_send(p_ctxt ctxt, t_ix remote, t_n max_reqs,
                        P_msngr_req reqs, P_msngr_ctxt msngrs) {
  t_fd fd = -1;
  t_prx_scope scope = PRX_SCOPE_SYSTEM;
  if (ctxt->remotes[remote].scope == PRX_SCOPE_SYSTEM)
    fd    = ctxt->system_fd;
  else {
    fd    = ctxt->network_fd;
    scope = ctxt->scope;
  }

  t_protocol_pkt pkt;
  do_protocol_encode(&pkt, scope, ctxt->name, &ctxt->msg_portid, max_reqs,
                     reqs, msngrs);

  t_sockaddr_tipc dest;
  dest.family   = AF_TIPC;
  dest.addrtype = TIPC_ADDR_ID;
  dest.scope    = ctxt->remotes[remote].scope == PRX_SCOPE_SYSTEM ?
                    TIPC_NODE_SCOPE : TIPC_CLUSTER_SCOPE;

  cp_portid(&dest.addr.id, &ctxt->remotes[remote].portid); // hton

  if (sendto(fd, &pkt, sizeof(pkt), 0, (p_sockaddr)&dest, sizeof(dest))
      != sizeof(pkt))
    exit_program("failed to initiate messaging");
}

t_void do_protocol_advertice_system(p_ctxt ctxt, t_ix remote) {
  t_msngr_reqs reqs;
  for (t_ix msngr = 0; msngr < MAX_MSNGRS; ++msngr) {
    if (ctxt->msngrs[msngr].scope == MSNGR_SCOPE_SYSTEM)
      reqs[msngr] = MSNGR_REQ_ADD;
    else
      reqs[msngr] = MSNGR_REQ_NONE;
  }
  do_protocol_send(ctxt, remote, MAX_MSNGRS, reqs, ctxt->msngrs);
  ctxt->remotes[remote].advertice = true;

  prx_event(ctxt, "send system advertice to %u.%u",
            ctxt->remotes[remote].portid.node,
            ctxt->remotes[remote].portid.ref);
}

t_void do_protocol_advertice_network(p_ctxt ctxt, t_ix remote) {
  t_msngr_reqs reqs;
  for (t_ix msngr = 0; msngr < MAX_MSNGRS; ++msngr) {
    if (ctxt->msngrs[msngr].scope == MSNGR_SCOPE_NETWORK)
      reqs[msngr] = MSNGR_REQ_ADD;
    else
      reqs[msngr] = MSNGR_REQ_NONE;
  }
  do_protocol_send(ctxt, remote, MAX_MSNGRS, reqs, ctxt->msngrs);
  ctxt->remotes[remote].advertice = true;

  prx_event(ctxt, "send network advertice to %u.%u",
            ctxt->remotes[remote].portid.node,
            ctxt->remotes[remote].portid.ref);
}

t_void do_protocol_advertice(p_ctxt ctxt, t_ix remote) {
  switch (ctxt->remotes[remote].scope) {
    case PRX_SCOPE_SYSTEM:
      do_protocol_advertice_system(ctxt, remote);
      break;
    case PRX_SCOPE_NETWORK:
      do_protocol_advertice_network(ctxt, remote);
      break;
    default:
      prx_event(ctxt, "unexpected request to advertice");
      break;
  }
}

t_void do_protocol_recv(p_ctxt ctxt, t_fd fd) {
  t_sockaddr_tipc src;
  t_protocol_pkt  pkt;

  socklen_t len = sizeof(src);
  if (recvfrom(fd, &pkt, sizeof(pkt), 0, (p_sockaddr)&src, &len)
      != sizeof(pkt))
    exit_program("failed to handle messaging");

  do_protocol_decode(&pkt);

  t_ix remote = find_remote(ctxt, &src.addr.id);
  if (remote < MAX_REMOTES) { //XXX - names should be verified
    cp_name(ctxt->remotes[remote].name, pkt.name);
    cp_portid(&ctxt->remotes[remote].msg_portid, &pkt.msg_portid);
    if (ctxt->remotes[remote].verified) {
      if (!ctxt->remotes[remote].advertice) {
        prx_event(ctxt, "receive advertice from vertified remote %s",
                  pkt.name);
        do_protocol_advertice(ctxt, remote);
      } else
        prx_event(ctxt, "receive msngr update from vertified remote %s",
                        pkt.name);

      for (t_ix msngr = 0; msngr < pkt.cnt; ++msngr) {
        switch (pkt.msngrs[msngr].req) {
          case MSNGR_REQ_ADD: {
            t_ix rmsngr = insert_remote_msngr(ctxt, remote,
                                              &pkt.msngrs[msngr].msngr);
            if (rmsngr == MAX_MSNGRS)
              prx_event(ctxt, "can't add messengr. drop. ignore");
          } break;
          case MSNGR_REQ_DEL: {
            t_ix rmsngr = find_remote_msngr(ctxt, remote,
                                            &pkt.msngrs[msngr].msngr);
            if (rmsngr < MAX_MSNGRS)
              remove_remote_msngr(ctxt, remote, rmsngr);
            else
              prx_event(ctxt, "can't del messengr - not there. drop. ignore");
          } break;
          default:
            prx_event(ctxt, "don't understand how this could happen");
            break;
        }
      }
    } else {
      /* very small time-window - highly unlikely - atleast in the demo
       * must update remote add/remove entries in its table - but its not
       * internally considered. once the remote is verified, then
       * its entries are placed in the lk, and its messengers become
       * internally visible.
       */
      prx_event(ctxt, "highly unlikely - production system require handling");
      prx_event(ctxt, "if this happens, in demo - it must be explained");
    }
  } else {
    prx_event(ctxt, "receive advertice from unvertified remote %s", pkt.name);

    remote = insert_remote(ctxt, pkt.scope, &src.addr.id);
    if (remote < MAX_REMOTES) {
      cp_name(ctxt->remotes[remote].name, pkt.name);
      cp_portid(&ctxt->remotes[remote].msg_portid, &pkt.msg_portid);
      for (t_ix msngr = 0; msngr < pkt.cnt; ++msngr) {
        if (pkt.msngrs[msngr].req == MSNGR_REQ_ADD) {
          cp_msngr(&ctxt->remotes[remote].msngrs[msngr],
                   &pkt.msngrs[msngr].msngr);
        } else
          prx_event(ctxt, "unexpected: don't understand how this could happen");
      }
    } else
      prx_event(ctxt, "max connected PRx has been reached");
  }
}

///////////////////////////////////////////////////////////////////////////////

t_void do_test_msg_encode(p_msg_pkt pkt, P_tipc_portid portid) {
  pkt->type = htonl(MSG_TEST);
  if (portid) {
    pkt->test.portids[pkt->test.hop].ref  = htonl(portid->ref);
    pkt->test.portids[pkt->test.hop].node = htonl(portid->node);

    pkt->test.hop += 1;
    pkt->test.hop       = htonl(pkt->test.hop);
    pkt->test.hop_depth = pkt->test.hop;
  } else {
    pkt->test.hop -= 1;
    pkt->test.hop       = htonl(pkt->test.hop);
    pkt->test.hop_depth = htonl(pkt->test.hop_depth);
  }
}

t_void do_test_msg_decode(p_msg_pkt pkt, p_tipc_portid portid) {
  pkt->test.hop       = ntohl(pkt->test.hop);
  pkt->test.hop_depth = ntohl(pkt->test.hop_depth);

  portid->node = ntohl(pkt->test.portids[pkt->test.hop-1].node);
  portid->ref  = ntohl(pkt->test.portids[pkt->test.hop-1].ref);
}

t_void do_test_msg_send(p_ctxt ctxt, p_msg_pkt pkt, P_tipc_portid portid,
                        t_bool add) {
  do_test_msg_encode(pkt, add ? &ctxt->msg_portid : NULL);

  t_sockaddr_tipc dest;
  dest.family   = AF_TIPC;
  dest.addrtype = TIPC_ADDR_ID;
  dest.scope    = TIPC_CLUSTER_SCOPE;

  cp_portid(&dest.addr.id, portid);

  if (sendto(ctxt->msg_fd, pkt, sizeof(t_msg_pkt), 0, (p_sockaddr)&dest,
             sizeof(dest)) != sizeof(t_msg_pkt))
    exit_program("failed to test message");
}

t_bool do_test_msg_1st_send(p_ctxt ctxt) {
  t_ix remote = ctxt->test.remote;
  if (remote) {
    t_msg_pkt pkt;
    pkt.test.hop       = 0;
    pkt.test.hop_depth = 0;

    ctxt->test.tx = 1;
    do_test_msg_send(ctxt, &pkt, &ctxt->remotes[remote-1].msg_portid, true);
    return true;
  }
  return false;
}

t_bool do_test_msg_recv(p_ctxt ctxt, p_msg_pkt pkt, P_sockaddr_tipc src) {
  t_tipc_portid last;
  do_test_msg_decode(pkt, &last);

  t_ix fwd_remote = ctxt->test.fwd_remote;
  if (!pkt->test.hop) {
    ctxt->test.rx += 1;
    if (ctxt->test.running) {
      if (ctxt->test.echos == 0 || ctxt->test.rx < ctxt->test.echos) {
        ctxt->test.tx += 1;
        do_test_msg_send(ctxt, pkt, &src->addr.id, true);
      } else {
        ctxt->test.running = false;
        clock_gettime(CLOCK_MONOTONIC, &ctxt->test.stop);
        test_event(ctxt, "test run completed");
        return true;
      }
    }
  } else if (pkt->test.hop < pkt->test.hop_depth || !fwd_remote)
    do_test_msg_send(ctxt, pkt, &last, false);
  else
    do_test_msg_send(ctxt, pkt, &ctxt->remotes[fwd_remote-1].msg_portid, true);
  return false;
}

///////////////////////////////////////////////////////////////////////////////

t_void do_cli_msg_encode(p_msg_pkt pkt, p_ctxt ctxt, t_cli_type type,
                         P_cstr msg,
                         t_n len) {
  pkt->type     = htonl(MSG_CLI);
  pkt->cli.type = htonl(type);
  pkt->cli.len  = htonl(len);
  cp_msg(pkt->cli.data, msg);
}

t_void do_cli_msg_decode(p_msg_pkt pkt) {
  pkt->cli.type = ntohl(pkt->cli.type);
  pkt->cli.len  = ntohl(pkt->cli.len);
}

t_bool do_cli_msg_send(p_ctxt ctxt, t_ix remote, t_cli_type type, P_cstr msg,
                       t_n len) {
  t_msg_pkt pkt;
  do_cli_msg_encode(&pkt, ctxt, type, msg, len);

  t_sockaddr_tipc dest;
  dest.family   = AF_TIPC;
  dest.addrtype = TIPC_ADDR_ID;
  dest.scope    = TIPC_CLUSTER_SCOPE;
  cp_portid(&dest.addr.id, &ctxt->remotes[remote].msg_portid);

  if (sendto(ctxt->msg_fd, &pkt, sizeof(pkt), 0, (p_sockaddr)&dest,
      sizeof(dest)) != sizeof(pkt)) {
    exit_program("failed to snd message");
    return true;
  }
  return false;
}

t_void cli_out_flush_msg(p_void _ctxt) {
  p_ctxt ctxt = (p_ctxt)_ctxt;
  if (ctxt->cli.out_len) {
    do_cli_msg_send(ctxt, ctxt->cli.ans_remote - 1, CLI_RESP, ctxt->cli.out,
                    ctxt->cli.out_len);
    ctxt->cli.out[0]  = '\0';
    ctxt->cli.out_len = 0;
  }
}

t_bool do_cli(p_ctxt);

t_bool do_cli_msg_recv(p_ctxt ctxt, p_msg_pkt pkt, P_sockaddr_tipc src) {
  do_cli_msg_decode(pkt);

  switch (pkt->cli.type) {
    case CLI_REQ: {
      for (t_ix remote = 0; remote < MAX_REMOTES; ++remote) {
        if (eq_portid(&src->addr.id, &ctxt->remotes[remote].msg_portid)) {
          ctxt->cli.ans_remote = remote + 1;
          ctxt->cli.flush  = cli_out_flush_msg;
          memcpy(ctxt->cli.req, pkt->cli.data, pkt->cli.len);
          ctxt->cli.req[pkt->cli.len] = '\0';
          return do_cli(ctxt);
        }
      }
    } break;
    case CLI_RESP:
      cli_out_append_string(ctxt, pkt->cli.data);
      break;
    case CLI_QUIT: {
      ctxt->cli.req_remote = 0;
    } break;
  }
  return false;
}

///////////////////////////////////////////////////////////////////////////////

t_void do_user_msg_encode(p_msg_pkt pkt, t_key src, t_key dst, P_cstr msg) {
  pkt->type      = htonl(MSG_USER);
  pkt->user.src  = htonk(src);
  pkt->user.dst  = htonk(dst);
  pkt->user.len  = htonl(strlen(msg) + 1);
  cp_msg(pkt->user.msg, msg);
}

t_void do_user_msg_decode(p_msg_pkt pkt) {
  pkt->user.src  = ntohk(pkt->user.src);
  pkt->user.dst  = ntohk(pkt->user.dst);
  pkt->user.len  = ntohl(pkt->user.len);
}

t_bool do_user_msg_send(p_ctxt ctxt, t_key src, t_key dst, P_cstr msg) {
  if (src != dst) {
    t_ix msngr = get_key_msngr(dst);
    if (msngr < MAX_MSNGRS) {
      if (is_local_key(dst)) {
        if (msngr < MAX_MSNGRS &&
            ctxt->msngrs[msngr].scope > MSNGR_SCOPE_OFF &&
            ctxt->msngrs[msngr].key == dst) {
          t_msg_pkt pkt;
          do_user_msg_encode(&pkt, src, dst, msg);

          t_sockaddr_tipc dest;
          dest.family   = AF_TIPC;
          dest.addrtype = TIPC_ADDR_ID;
          dest.scope    = TIPC_CLUSTER_SCOPE;
          cp_portid(&dest.addr.id, &ctxt->msg_portid);

          if (sendto(ctxt->msg_fd, &pkt, sizeof(pkt), 0, (p_sockaddr)&dest,
                     sizeof(dest)) != sizeof(pkt))
            exit_program("failed to snd message");
          return true;
        } else
          prx_event(ctxt, "unexpected sent packet. can't sent.");
      } else {
        t_ix remote = get_key_remote(dst);
        if (remote < MAX_REMOTES) {
          if (ctxt->remotes[remote].scope != PRX_SCOPE_NONE &&
              ctxt->remotes[remote].msngrs[msngr].scope != MSNGR_SCOPE_NONE &&
              get_key_hash(ctxt->remotes[remote].msngrs[msngr].key) ==
              get_key_hash(dst)) {
            t_msg_pkt pkt;
            do_user_msg_encode(&pkt, src,
                               ctxt->remotes[remote].msngrs[msngr].key, msg);

            t_sockaddr_tipc dest;
            dest.family   = AF_TIPC;
            dest.addrtype = TIPC_ADDR_ID;
            dest.scope    = TIPC_CLUSTER_SCOPE;
            cp_portid(&dest.addr.id, &ctxt->remotes[remote].msg_portid);

            if (sendto(ctxt->msg_fd, &pkt, sizeof(pkt), 0, (p_sockaddr)&dest,
                       sizeof(dest)) != sizeof(pkt))
              exit_program("failed to snd message");
            return true;
          }
        }
      }
    }
  }
  prx_event(ctxt, "invalid key:0x%" PRIx64 " & drop message<%s>", dst, msg);
  return false;
}

t_void do_user_msg_deliver(p_ctxt ctxt, t_key src, t_key dst, P_cstr msg) {
  if (src != dst) {
    t_ix msngr = get_key_msngr(dst);
    if (msngr < MAX_MSNGRS) {
      if (is_local_key(dst)) {
        if (is_group_key(dst)) {
          if (msngr >= MAX_LOCAL_MSNGRS && msngr < MAX_MSNGRS) {
            prx_event(ctxt, "grp %s received message(len=%d, <%s>)",
                      ctxt->msngrs[msngr].name, strlen(msg) + 1, msg);

            t_ix lkup = get_key_lkup(dst);
            if (ctxt->lkups[lkup].members_cnt) {
              for (t_ix member = 0; member < MAX_GRP_MEMBERS; ++member) {
                t_key member_key = ctxt->lkups[lkup].members[member];
                if (member_key != INVALID_KEY)
                  do_user_msg_send(ctxt, src, member_key, msg);
              }
            }
          }
        } else if (msngr < MAX_LOCAL_MSNGRS &&
                   ctxt->msngrs[msngr].scope > MSNGR_SCOPE_OFF &&
                   ctxt->msngrs[msngr].key == dst) {
          msngr_event(ctxt, "notify %s received message(len=%d, <%s>)",
                     ctxt->msngrs[msngr].name, strlen(msg) + 1, msg);
        } else
          prx_event(ctxt, "unexpected sent packet. can't sent.");
      } else
        do_user_msg_send(ctxt, src, dst, msg);
    } else
      prx_event(ctxt, "invalid key:0x%" PRIx64 " & drop msg <%s>", dst, msg);
  } else
    prx_event(ctxt, "don't allow to forward directly. infinite loop. drop msg");
}

t_void do_user_msg_recv(p_ctxt ctxt, p_msg_pkt pkt) {
  do_user_msg_decode(pkt);
  do_user_msg_deliver(ctxt, pkt->user.src, pkt->user.dst, pkt->user.msg);
}

///////////////////////////////////////////////////////////////////////////////

t_bool do_msg_recv(p_ctxt ctxt, p_bool prompt) {
  t_sockaddr_tipc src;
  t_msg_pkt       pkt;

  socklen_t len = sizeof(src);
  if (recvfrom(ctxt->msg_fd, &pkt, sizeof(pkt), 0, (p_sockaddr)&src, &len)
      != sizeof(pkt))
    exit_program("failed to handle msg messaging");

  pkt.type = ntohl(pkt.type);
  switch (pkt.type) {
    case MSG_USER:
      do_user_msg_recv(ctxt, &pkt);
      *prompt = false;
      break;
    case MSG_TEST:
      *prompt = do_test_msg_recv(ctxt, &pkt, &src);
      break;
    case MSG_CLI:
      *prompt = true;
      return do_cli_msg_recv(ctxt, &pkt, &src);
    default:
      prx_event(ctxt, "received msg with unknown type - %hhd", pkt.type);
  }
  return false;
}

///////////////////////////////////////////////////////////////////////////////

t_void do_tracking(p_ctxt ctxt) {
  t_tipc_event event;
  if (recv(ctxt->service_fd, &event, sizeof(event), 0) != sizeof(event))
    exit_program("TIPC service send event that is not the right size");

  t_prx_scope scope = transform(event.s.seq.type);

  switch (event.event) {
    case TIPC_PUBLISHED: {
      if (eq_portid(&ctxt->system_portid, &event.port) ||
          eq_portid(&ctxt->network_portid, &event.port))
        prx_event(ctxt, "prx(%s:%s) published", ctxt->name,
                  prx_scope_cstr(scope));
      else {
        prx_event(ctxt, "prx(%u.%u:%s) published", event.port.node,
                  event.port.ref, prx_scope_cstr(scope));

        t_ix remote = find_remote(ctxt, &event.port);
        if (remote < MAX_REMOTES) {
          ctxt->remotes[remote].verified = true;
          do_protocol_advertice(ctxt, remote);
          insert_verified_remote_msngrs(ctxt, remote);
        } else {
          remote = insert_remote(ctxt, scope, &event.port);
          if (remote < MAX_REMOTES) {
            ctxt->remotes[remote].verified = true;
            switch (scope) {
              case PRX_SCOPE_SYSTEM:
                if (ctxt->system_portid.ref < ctxt->remotes[remote].portid.ref)
                  do_protocol_advertice_system(ctxt, remote);
                break;
              case PRX_SCOPE_NETWORK:
                if (ctxt->network_portid.ref < ctxt->remotes[remote].portid.ref)
                  do_protocol_advertice_network(ctxt, remote);
                break;
              default:
                prx_event(ctxt, "unexpected type - logic error");
            }
          } else
            prx_event(ctxt, "max connected PRx entries reached");
        }
      }
    } break;

    case TIPC_WITHDRAWN: {
      if (eq_portid(&ctxt->system_portid, &event.port) ||
          eq_portid(&ctxt->network_portid, &event.port))
        prx_event(ctxt, "prx(%s:%s) withdrawn", ctxt->name,
                  prx_scope_cstr(scope));
      else {
        prx_event(ctxt, "prx(%u.%u:%s) withdrawn", event.port.node,
                  event.port.ref, prx_scope_cstr(scope));

        t_ix remote = find_remote(ctxt, &event.port);
        if (remote < MAX_REMOTES)
          remove_remote(ctxt, remote);
        else
          prx_event(ctxt, "unexpected: don't find anything to flush");
      }
    } break;
  }
}

///////////////////////////////////////////////////////////////////////////////

t_void add_system_scope(p_ctxt ctxt) {
  ctxt->system_addr.family             = AF_TIPC;               /* local */
  ctxt->system_addr.addrtype           = TIPC_ADDR_NAMESEQ;     /* local */
  ctxt->system_addr.scope              = TIPC_NODE_SCOPE;       /* local */
  ctxt->system_addr.addr.nameseq.type  = PRX_TIPC_SYSTEM;
  ctxt->system_addr.addr.nameseq.lower = 1;
  ctxt->system_addr.addr.nameseq.upper = 100;

  if (bind(ctxt->system_fd, (p_sockaddr)&ctxt->system_addr,
           sizeof(ctxt->system_addr)) != 0)
    exit_program("TIPC RDM socket could not be bound to SYSTEM");

  ctxt->system_subscr.seq.type  = PRX_TIPC_SYSTEM;
  ctxt->system_subscr.seq.lower = 1;
  ctxt->system_subscr.seq.upper = 100;
  ctxt->system_subscr.timeout   = TIPC_WAIT_FOREVER; /* local */
  ctxt->system_subscr.filter    = TIPC_SUB_PORTS;    /* local */

  if (send(ctxt->service_fd, &ctxt->system_subscr,
           sizeof(ctxt->system_subscr), 0) != sizeof(ctxt->system_subscr))
    exit_program("failed to send SYSTEM subscription to TIPC");

  ctxt->scope = PRX_SCOPE_SYSTEM;
}

t_void del_system_scope(p_ctxt ctxt) {
  if (bind(ctxt->system_fd, (p_sockaddr)&ctxt->system_addr, 0) != 0)
    exit_program("TIPC RDM socket could not be unbound from SYSTEM");

  ctxt->system_subscr.filter |= TIPC_SUB_CANCEL;

  if (send(ctxt->service_fd, &ctxt->system_subscr,
           sizeof(ctxt->system_subscr), 0) != sizeof(ctxt->system_subscr))
    exit_program("failed to send SYSTEM cancel subscription to TIPC");

  remove_remotes(ctxt, PRX_SCOPE_SYSTEM);
  ctxt->scope = PRX_SCOPE_PROCESS;
}

t_void add_network_scope(p_ctxt ctxt) {
  ctxt->network_addr.family             = AF_TIPC;            /* local */
  ctxt->network_addr.addrtype           = TIPC_ADDR_NAMESEQ;  /* local */
  ctxt->network_addr.scope              = TIPC_CLUSTER_SCOPE; /* local */
  ctxt->network_addr.addr.nameseq.type  = PRX_TIPC_NETWORK;
  ctxt->network_addr.addr.nameseq.lower = 1;
  ctxt->network_addr.addr.nameseq.upper = 100;

  if (bind(ctxt->network_fd, (p_sockaddr)&ctxt->network_addr,
           sizeof(ctxt->network_addr)) != 0)
    exit_program("TIPC RDM socket could not be bound to NETWORK");

  ctxt->network_subscr.seq.type  = PRX_TIPC_NETWORK;
  ctxt->network_subscr.seq.lower = 1;
  ctxt->network_subscr.seq.upper = 100;
  ctxt->network_subscr.timeout   = TIPC_WAIT_FOREVER; /* local */
  ctxt->network_subscr.filter    = TIPC_SUB_PORTS;    /* local */

  if (send(ctxt->service_fd, &ctxt->network_subscr,
      sizeof(ctxt->network_subscr), 0) != sizeof(ctxt->network_subscr))
    exit_program("failed to send NETWORK subscription to TIPC");

  ctxt->scope = PRX_SCOPE_NETWORK;
}

t_void del_network_scope(p_ctxt ctxt) {
  if (bind(ctxt->network_fd, (p_sockaddr)&ctxt->network_addr, 0) != 0)
    exit_program("TIPC RDM socket could not be unbound from NETWORK");

  ctxt->network_subscr.filter |= TIPC_SUB_CANCEL;

  if (send(ctxt->service_fd, &ctxt->network_subscr,
      sizeof(ctxt->network_subscr), 0) != sizeof(ctxt->network_subscr))
    exit_program("failed to send NETWORK cancel subscription to TIPC");

  remove_remotes(ctxt, PRX_SCOPE_NETWORK);
  ctxt->scope = PRX_SCOPE_SYSTEM;
}

///////////////////////////////////////////////////////////////////////////////

t_void do_change(p_ctxt ctxt, t_prx_scope scope) {
  switch (ctxt->scope) {
    case PRX_SCOPE_PROCESS: {
      switch (scope) {
        case PRX_SCOPE_SYSTEM: {
          add_system_scope(ctxt);
        } break;
        case PRX_SCOPE_NETWORK: {
          add_system_scope(ctxt);
          add_network_scope(ctxt);
        } break;
        default:
          cli_event(ctxt, I0, "do nothing");
          break;
      }
    } break;
    case PRX_SCOPE_SYSTEM: {
      switch (scope) {
        case PRX_SCOPE_PROCESS: {
          del_system_scope(ctxt);
        } break;
        case PRX_SCOPE_NETWORK: {
          add_network_scope(ctxt);
        } break;
        default:
          cli_event(ctxt, I0, "do nothing");
          break;
      }
    } break;
    case PRX_SCOPE_NETWORK: {
      switch (scope) {
        case PRX_SCOPE_PROCESS: {
          del_network_scope(ctxt);
          del_system_scope(ctxt);
        } break;
        case PRX_SCOPE_SYSTEM: {
          del_network_scope(ctxt);
        } break;
        default:
          cli_event(ctxt, I0, "do nothing");
          break;
      }
    } break;
    default:
      cli_event(ctxt, I0, "unexpected scope value");
      break;
  }
}

///////////////////////////////////////////////////////////////////////////////

t_void notify_system_remotes(p_ctxt ctxt, t_ix msngr, t_msngr_req req) {
  for (t_ix remote = 0; remote < MAX_REMOTES; ++remote)
    if (ctxt->remotes[remote].scope == PRX_SCOPE_SYSTEM)
      do_protocol_send(ctxt, remote, 1, &req, &ctxt->msngrs[msngr]);
}

t_void notify_network_remotes(p_ctxt ctxt, t_ix msngr, t_msngr_req req) {
  for (t_ix remote = 0; remote < MAX_REMOTES; ++remote)
    if (ctxt->remotes[remote].scope == PRX_SCOPE_NETWORK)
      do_protocol_send(ctxt, remote, 1, &req, &ctxt->msngrs[msngr]);
}

t_void do_msngr_chg(p_ctxt ctxt, t_ix msngr, t_msngr_scope scope) {
  switch (ctxt->msngrs[msngr].scope) {
    case MSNGR_SCOPE_OFF: {
      switch (scope) {
        case MSNGR_SCOPE_OFF: {
          cli_event(ctxt, I0, "no scope change. ignore");
        } break;
        case MSNGR_SCOPE_PROCESS: {
          ctxt->msngrs[msngr].scope = MSNGR_SCOPE_PROCESS;
          notify_messengers(ctxt, get_key_lkup(ctxt->msngrs[msngr].key));
        } break;
        case MSNGR_SCOPE_SYSTEM: {
          ctxt->msngrs[msngr].scope = MSNGR_SCOPE_SYSTEM;
          notify_messengers(ctxt, get_key_lkup(ctxt->msngrs[msngr].key));
          notify_system_remotes(ctxt, msngr, MSNGR_REQ_ADD);
        } break;
        case MSNGR_SCOPE_NETWORK: {
          ctxt->msngrs[msngr].scope = MSNGR_SCOPE_NETWORK;
          notify_messengers(ctxt, get_key_lkup(ctxt->msngrs[msngr].key));
          notify_network_remotes(ctxt, msngr, MSNGR_REQ_ADD);
        } break;
        default:
          cli_event(ctxt, I0, "unexpected scope value");
          break;
      }
    } break;
    case MSNGR_SCOPE_PROCESS: {
      switch (scope) {
        case MSNGR_SCOPE_OFF: {
          ctxt->msngrs[msngr].scope = MSNGR_SCOPE_OFF;
          notify_messengers(ctxt, get_key_lkup(ctxt->msngrs[msngr].key));
        } break;
        case MSNGR_SCOPE_PROCESS: {
          cli_event(ctxt, I0, "no scope change. ignore");
        } break;
        case MSNGR_SCOPE_SYSTEM: {
          ctxt->msngrs[msngr].scope = MSNGR_SCOPE_SYSTEM;
          notify_system_remotes(ctxt, msngr, MSNGR_REQ_ADD);
        } break;
        case MSNGR_SCOPE_NETWORK: {
          ctxt->msngrs[msngr].scope = MSNGR_SCOPE_NETWORK;
          notify_network_remotes(ctxt, msngr, MSNGR_REQ_ADD);
        } break;
        default:
          cli_event(ctxt, I0, "unexpected scope value");
          break;
      }
    } break;
    case MSNGR_SCOPE_SYSTEM: {
      switch (scope) {
        case MSNGR_SCOPE_OFF: {
          notify_system_remotes(ctxt, msngr, MSNGR_REQ_DEL);
          ctxt->msngrs[msngr].scope = MSNGR_SCOPE_OFF;
          notify_messengers(ctxt, get_key_lkup(ctxt->msngrs[msngr].key));
        } break;
        case MSNGR_SCOPE_PROCESS: {
          notify_system_remotes(ctxt, msngr, MSNGR_REQ_DEL);
          ctxt->msngrs[msngr].scope = MSNGR_SCOPE_PROCESS;
        } break;
        case MSNGR_SCOPE_SYSTEM: {
          cli_event(ctxt, I0, "no scope change. ignore");
        } break;
        case MSNGR_SCOPE_NETWORK: {
          notify_system_remotes(ctxt, msngr, MSNGR_REQ_DEL);
          ctxt->msngrs[msngr].scope = MSNGR_SCOPE_NETWORK;
          notify_network_remotes(ctxt, msngr, MSNGR_REQ_ADD);
        } break;
        default:
          cli_event(ctxt, I0, "unexpected scope value");
          break;
      }
    } break;
    case MSNGR_SCOPE_NETWORK: {
      switch (scope) {
        case MSNGR_SCOPE_OFF: {
          notify_network_remotes(ctxt, msngr, MSNGR_REQ_DEL);
          ctxt->msngrs[msngr].scope = MSNGR_SCOPE_OFF;
          notify_messengers(ctxt, get_key_lkup(ctxt->msngrs[msngr].key));
        } break;
        case MSNGR_SCOPE_PROCESS: {
          notify_network_remotes(ctxt, msngr, MSNGR_REQ_DEL);
          ctxt->msngrs[msngr].scope = MSNGR_SCOPE_PROCESS;
        } break;
        case MSNGR_SCOPE_SYSTEM: {
          notify_network_remotes(ctxt, msngr, MSNGR_REQ_DEL);
          ctxt->msngrs[msngr].scope = MSNGR_SCOPE_SYSTEM;
          notify_system_remotes(ctxt, msngr, MSNGR_REQ_ADD);
        } break;
        case MSNGR_SCOPE_NETWORK: {
          cli_event(ctxt, I0, "no scope change. ignore");
        } break;
        default:
          cli_event(ctxt, I0, "unexpected scope value");
          break;
      }
    } break;
    default:
      cli_event(ctxt, I0, "unexpected scope value");
      break;
  }
}

///////////////////////////////////////////////////////////////////////////////

t_bool do_cli_info(p_ctxt ctxt, t_n argc, p_cstr argv[]) {
  if (argc == 0) {
    cli_event(ctxt, I0, "dainty messaging instance (prx)");
    cli_event(ctxt, I1, "boundaries");
    cli_event(ctxt, I2, "data size        : %d", sizeof(t_ctxt));
    cli_event(ctxt, I2, "max name length  : %d", MAX_NAME_LEN);
    cli_event(ctxt, I2, "max local msngrs : %d", MAX_LOCAL_MSNGRS);
    cli_event(ctxt, I2, "max local grps   : %d", MAX_LOCAL_GRPS);
    cli_event(ctxt, I2, "max grp members  : %d", MAX_GRP_MEMBERS);
    cli_event(ctxt, I2, "max remotes      : %d", MAX_REMOTES);
    cli_event(ctxt, I2, "max known msngrs : %d", MAX_LKUPS);
    cli_event(ctxt, I2, "max cli request  : %d", MAX_CLI_REQ_LEN);
    cli_event(ctxt, I2, "max cli output   : %d", MAX_CLI_OUT_LEN);
    cli_event(ctxt, I1, "indentiy");
    cli_event(ctxt, I2, "name             : %s", ctxt->name);
    cli_event(ctxt, I2, "scope            : %s", prx_scope_cstr(ctxt->scope));
    cli_event(ctxt, I2, "network");
    cli_event(ctxt, I3, "service portid : fd:%d, tipc:%u.%u", ctxt->service_fd,
              ctxt->service_portid.node,
              ctxt->service_portid.ref);
    cli_event(ctxt, I3, "system portid  : fd:%d, tipc:%u.%u", ctxt->system_fd,
              ctxt->system_portid.node,
              ctxt->system_portid.ref);
    cli_event(ctxt, I3, "network portid : fd:%d, tipc:%u.%u", ctxt->network_fd,
              ctxt->network_portid.node,
              ctxt->network_portid.ref);
    cli_event(ctxt, I3, "msg portid     : fd:%d, tipc:%u.%u", ctxt->msg_fd,
              ctxt->msg_portid.node,
              ctxt->msg_portid.ref);

    cli_event(ctxt, I1, "remotes");
    for (t_ix remote = 0; remote < MAX_REMOTES; ++remote) {
      if (ctxt->remotes[remote].scope != PRX_SCOPE_NONE) {
        cli_event(ctxt, I2, "(id:%d) remote: %s(%s)", remote,
                  ctxt->remotes[remote].name,
                  prx_scope_cstr(ctxt->remotes[remote].scope));
        cli_event(ctxt, I3, "protocol: %u.%u",
                  ctxt->remotes[remote].portid.node,
                  ctxt->remotes[remote].portid.ref);
        cli_event(ctxt, I3, "msg     : %u.%u",
                  ctxt->remotes[remote].msg_portid.node,
                  ctxt->remotes[remote].msg_portid.ref);
      }
    }
    return false;
  }
  return true;
}

t_bool do_cli_change(p_ctxt ctxt, t_n argc, p_cstr argv[]) {
  if (argc == 1) {
    t_cli_option opts[] = {{"process", PRX_SCOPE_PROCESS},
                                {"system",  PRX_SCOPE_SYSTEM},
                                {"network", PRX_SCOPE_NETWORK}};
    t_prx_scope scope = cli_find(argv[0], PRX_SCOPE_NONE, opts,
                                    CLI_OPTIONS(opts));
    if (scope != PRX_SCOPE_NONE) {
      do_change(ctxt, scope);
      return false;
    }
  }
  return true;
}

t_bool do_cli_msngr_add(p_ctxt ctxt, t_n argc, p_cstr argv[]) {
  if (argc >= 2) {
    t_cli_option opts[] = {{"off",     MSNGR_SCOPE_OFF},
                                {"process", MSNGR_SCOPE_PROCESS},
                                {"system",  MSNGR_SCOPE_SYSTEM},
                                {"network", MSNGR_SCOPE_NETWORK}};
    t_msngr_scope scope = cli_find(argv[1], MSNGR_SCOPE_NONE, opts,
                                      CLI_OPTIONS(opts));
    if (scope != MSNGR_SCOPE_NONE) {
      if (is_name_valid(argv[0])) {
        t_bool is_group = false;
        if (argc == 3) {
          t_cli_option opts[] = {{"grp", 1}};
          if (cli_find(argv[2], 0, opts, CLI_OPTIONS(opts)))
             is_group = true;
        }

        t_ix msngr = find_msngr(ctxt, argv[0]);
        if (msngr == MAX_MSNGRS) {
          msngr = insert_msngr(ctxt, argv[0], scope, is_group);
          if (msngr < MAX_MSNGRS) {
            switch (scope) {
              case MSNGR_SCOPE_SYSTEM:
                notify_system_remotes(ctxt, msngr, MSNGR_REQ_ADD);
                break;
              case MSNGR_SCOPE_NETWORK:
                notify_network_remotes(ctxt, msngr, MSNGR_REQ_ADD);
                break;
              default:
                break;
            }
          } else
            cli_event(ctxt, I0, "max messneger reached. ignore request");
        } else
          cli_event(ctxt, I0, "msngr known. ignore request");
      } else
        cli_event(ctxt, I0, "msngr name is not valid (maybe container '/')");
      return false;
    }
  }
  return true;
}

t_bool do_cli_msngr_del(p_ctxt ctxt, t_n argc, p_cstr argv[]) {
  if (argc == 1) {
    t_ix msngr = find_msngr(ctxt, argv[0]);
    if (msngr < MAX_MSNGRS) {
      switch (ctxt->msngrs[msngr].scope) {
        case MSNGR_SCOPE_SYSTEM:
          notify_system_remotes(ctxt, msngr, MSNGR_REQ_DEL);
          break;
        case MSNGR_SCOPE_NETWORK:
          notify_network_remotes(ctxt, msngr, MSNGR_REQ_DEL);
          break;
        default:
          break;
      }
      remove_msngr(ctxt, msngr);
    } else
      cli_event(ctxt, I0, "msngr unknown. ignore request");
  } else
    return true;
  return false;
}

t_bool do_cli_msngr_chg(p_ctxt ctxt, t_n argc, p_cstr argv[]) {
  if (argc == 2) {
    t_cli_option opts[] = {{"off",     MSNGR_SCOPE_OFF},
                                {"process", MSNGR_SCOPE_PROCESS},
                                {"system",  MSNGR_SCOPE_SYSTEM},
                                {"network", MSNGR_SCOPE_NETWORK}};
    t_msngr_scope scope = cli_find(argv[1], MSNGR_SCOPE_NONE, opts,
                                      CLI_OPTIONS(opts));
    if (scope != MSNGR_SCOPE_NONE) {
      t_ix msngr = find_msngr(ctxt, argv[0]);
      if (msngr < MAX_MSNGRS)
        do_msngr_chg(ctxt, msngr, scope);
      else
        cli_event(ctxt, I0, "msngr unknown (%s). ignore request", argv[0]);
      return false;
    }
  }
  return true;
}

t_bool do_cli_msngr_snd(p_ctxt ctxt, t_n argc, p_cstr argv[]) {
  if (argc == 3) {
    if (argv[1][0] == '0' && argv[1][1] == 'x') {
      t_key dst = INVALID_KEY;
      if (sscanf(argv[1], "0x%" SCNx64, &dst) == 1) {
        t_ix msngr = find_msngr(ctxt, argv[0]);
        if (msngr < MAX_MSNGRS)
          do_user_msg_send(ctxt, ctxt->msngrs[msngr].key, dst, argv[2]);
        else
          cli_event(ctxt, I0, "could not find %s msngr", argv[0]);
        return false;
      }
    }
  }
  return true;
}

t_bool do_cli_msngr_list(p_ctxt ctxt, t_n argc, p_cstr argv[]) {
  if (argc == 0) {
    cli_event(ctxt, I1, "(1) msngr table:");
    t_n cnt = 0;
    for (t_ix n = 0; n < MAX_MSNGRS; ++n)
      if (ctxt->msngrs[n].scope != MSNGR_SCOPE_NONE)
         cli_event(ctxt, I2, "(%d) id:%d %s/%s (%s) : key = 0x%" PRIx64,
                   ++cnt, n, ctxt->name, ctxt->msngrs[n].name,
                   msngr_scope_cstr(ctxt->msngrs[n].scope),
                   ctxt->msngrs[n].key);

    cli_event(ctxt, I1, "(2) remote msngr table:");
    cnt = 0;
    for (t_ix n = 0; n < MAX_REMOTES; ++n) {
      if (ctxt->remotes[n].scope != PRX_SCOPE_NONE) {
        for (t_ix j = 0; j < MAX_MSNGRS; ++j) {
          if (ctxt->remotes[n].msngrs[j].scope != MSNGR_SCOPE_NONE) {
            cli_event(ctxt, I2, "(%d) id:%d:%d %s/%s (%s) : key = 0x%" PRIx64,
                      ++cnt, n, j, ctxt->remotes[n].name,
                      ctxt->remotes[n].msngrs[j].name,
                      msngr_scope_cstr(ctxt->remotes[n].msngrs[j].scope),
                      ctxt->remotes[n].msngrs[j].key);
          }
        }
      }
    }

    cli_event(ctxt, I1, "(3) lookup table:");
    twalk(ctxt->lk, lk_action);
    return false;
  }
  return true;
}

t_bool do_cli_msngr_key(p_ctxt ctxt, t_n argc, p_cstr argv[]) {
  if (argc == 1) {
    t_key key = INVALID_KEY;
    if (sscanf(argv[0], "0x%" SCNx64, &key) == 1) {
      if (key != INVALID_KEY) {
        t_bool is_grp   = is_group_key(key);
        t_bool is_local = is_local_key(key);

        t_int hash   = get_key_hash  (key);
        t_ix  lkup   = get_key_lkup  (key);
        t_ix  msngr  = get_key_msngr (key);
        t_ix  remote = get_key_remote(key);

        cli_event(ctxt, I0, "key info:");
        if (is_local) {
          cli_event(ctxt, I1, "local %s", is_grp ? "msngr grp" : "msngr");
          cli_event(ctxt, I1, "msngr id:%d", msngr);
          cli_event(ctxt, I1, "lkup id:%d",  lkup);
          cli_event(ctxt, I1, "hash %d",     hash);

          if (msngr < MAX_MSNGRS && lkup < MAX_LKUPS &&
              ctxt->msngrs[msngr].scope != MSNGR_SCOPE_NONE &&
              ctxt->msngrs[msngr].key   == key &&
              ctxt->lkups[lkup].free == -1 &&
              ctxt->lkups[lkup].key == key) {
              cli_event(ctxt, I1, "valid key (%s)", ctxt->lkups[lkup].fullname);
          } else
            cli_event(ctxt, I1, "invalid key");
        } else {
          cli_event(ctxt, I1, "remote %s", is_grp ? "msngr grp" : "msngr");
          cli_event(ctxt, I1, "remote id:%d", remote);
          cli_event(ctxt, I1, "msngr id:%d",  msngr);
          cli_event(ctxt, I1, "lkup id:%d",   lkup);
          cli_event(ctxt, I1, "hash %d",      hash);

          if (remote < MAX_REMOTES && msngr < MAX_MSNGRS &&
              lkup < MAX_LKUPS &&
              ctxt->remotes[remote].scope != PRX_SCOPE_NONE &&
              ctxt->remotes[remote].msngrs[msngr].scope != MSNGR_SCOPE_NONE &&
              ctxt->lkups[lkup].free == FREE_INUSE &&
              ctxt->lkups[lkup].key == key) {
              cli_event(ctxt, I1, "valid key (%s)",
                        ctxt->lkups[lkup].fullname);
          } else
            cli_event(ctxt, I1, "invalid key");
        }
      } else
        cli_event(ctxt, I0, "invalid key - can't be used");
      return false;
    }
  }
  return true;
}

t_bool do_cli_msngr_mon_add(p_ctxt ctxt, t_n argc, p_cstr argv[]) {
  if (argc == 2) {
    if (is_fullname_valid(argv[1])) {
      t_ix msngr = find_msngr(ctxt, argv[0]);
      if (msngr < MAX_MSNGRS) {
        if (insert_monitor(ctxt, msngr, argv[1]))
          cli_event(ctxt, I0, "msngr %s monitor(%s)", argv[0], argv[1]);
        else
          cli_event(ctxt, I0, "msngr %s could not monitor(%s)", argv[0],
                    argv[1]);
      } else
        cli_event(ctxt, I0, "didn't find msngr(%s)", argv[0]);
    } else
      cli_event(ctxt, I0, "the monitored fullnname:%s is not valid", argv[1]);
  } else
    return true;
  return false;
}

t_bool do_cli_msngr_mon_del(p_ctxt ctxt, t_n argc, p_cstr argv[]) {
  if (argc == 2) {
    t_ix msngr = find_msngr(ctxt, argv[0]);
    if (msngr < MAX_MSNGRS) {
      if (remove_monitor(ctxt, msngr, argv[1]))
        cli_event(ctxt, I0, "msngr %s does not monitor(%s)", argv[0], argv[1]);
      else
        cli_event(ctxt, I0, "msngr %s did not monitor(%s)", argv[0], argv[1]);
    } else
      cli_event(ctxt, I0, "didn't find msngr(%s)", argv[0]);
  } else
    return true;
  return false;
}

t_bool do_cli_msngr_grp_attach(p_ctxt ctxt, t_n argc, p_cstr argv[]) {
  if (argc == 2) {
    t_key key = INVALID_KEY;
    if (sscanf(argv[0], "0x%" SCNx64, &key) == 1) {
      if (key != INVALID_KEY) {
        t_ix lkup = get_key_lkup(key);
        if (lkup < MAX_LKUPS && ctxt->lkups[lkup].key == key) {
          if (!attach_grp_member(ctxt, argv[1], key))
            cli_event(ctxt, I0, "could not attach key to grp %s", argv[1]);
          return false;
        }
      }
      cli_event(ctxt, I0, "invalid key value");
      return false;
    } else
      cli_event(ctxt, I0, "invalid key");
  }
  return true;
}

t_bool do_cli_msngr_grp_detach(p_ctxt ctxt, t_n argc, p_cstr argv[]) {
  if (argc == 2) {
    t_key key = INVALID_KEY;
    if (sscanf(argv[0], "0x%" SCNx64, &key) == 1) {
      if (key != INVALID_KEY) {
        t_ix lkup = get_key_lkup(key);
        if (lkup < MAX_LKUPS && ctxt->lkups[lkup].key == key) {
          detach_grp_member(ctxt, argv[1], key);
          return false;
        }
      }
      cli_event(ctxt, I0, "invalid key value");
      return false;
    } else
      cli_event(ctxt, I0, "invalid key");
  }
  return true;
}

t_bool do_cli_msngr_grp_list(p_ctxt ctxt, t_n argc, p_cstr argv[]) {
  if (argc == 0) {
    for (t_ix lkup = 0; lkup < MAX_LKUPS; ++lkup) {
      if (ctxt->lkups[lkup].free == FREE_INUSE &&
          (ctxt->lkups[lkup].members_cnt ||
           (ctxt->lkups[lkup].key != INVALID_KEY &&
            is_group_key(ctxt->lkups[lkup].key) &&
            is_local_key(ctxt->lkups[lkup].key)))) {
        cli_event(ctxt, I1, "grp %s with %d members",
                  ctxt->lkups[lkup].fullname,
                  ctxt->lkups[lkup].members_cnt);
        if (ctxt->lkups[lkup].members_cnt) {
          for (t_ix member = 0; member < MAX_GRP_MEMBERS; ++member)
            if (ctxt->lkups[lkup].members[member] != INVALID_KEY)
              cli_event(ctxt, I2, "member: 0x%" PRIx64,
                        ctxt->lkups[lkup].members[member]);
        }
      }
    }
  }
  return false;
}

t_bool do_cli_test_fwd(p_ctxt ctxt, t_n argc, p_cstr argv[]) {
  if (argc == 1) {
    t_ix remote = find_remote_by_name(ctxt, argv[0]);
    if (remote < MAX_REMOTES)
      ctxt->test.fwd_remote = remote + 1;
    else
      cli_event(ctxt, I0, "%s is not a known remote", argv[0]);
    return false;
  }
  return true;
}

t_bool do_cli_test_start(p_ctxt ctxt, t_n argc, p_cstr argv[]) {
  if (argc == 1 || argc == 2) {
    t_ix remote = find_remote_by_name(ctxt, argv[0]);
    if (remote < MAX_REMOTES) {
      t_n echos = 0;
      if (argc == 2 && sscanf(argv[1], "%d", &echos) != 1)
        return false;
      clock_gettime(CLOCK_MONOTONIC, &ctxt->test.start);
      ctxt->test.remote  = remote + 1;
      ctxt->test.running = true;
      ctxt->test.echos   = echos;
      ctxt->test.rx      = 0;
      ctxt->test.tx      = 0;
      do_test_msg_1st_send(ctxt);
    } else
      cli_event(ctxt, I0, "%s is not a known remote", argv[0]);
    return false;
  }
  return true;
}

t_bool do_cli_test_stop(p_ctxt ctxt, t_n argc, p_cstr argv[]) {
  if (argc == 0) {
    if (ctxt->test.running) {
      ctxt->test.running = false;
      clock_gettime(CLOCK_MONOTONIC, &ctxt->test.stop);
    }
    return false;
  }
  return true;
}

t_bool do_cli_test_info(p_ctxt ctxt, t_n argc, p_cstr argv[]) {
  if (argc == 0) {
    P_cstr remote =
      ctxt->test.remote ? ctxt->remotes[ctxt->test.remote-1].name : "none";
    P_cstr fwd = ctxt->test.fwd_remote ?
      ctxt->remotes[ctxt->test.fwd_remote-1].name : "none";

    cli_event(ctxt, I1, "test info");
    cli_event(ctxt, I2, "remote    : %s", remote);
    cli_event(ctxt, I2, "fwd       : %s", fwd);
    cli_event(ctxt, I2, "running   : %s", ctxt->test.running ? "yes" : "no");
    cli_event(ctxt, I2, "echos     : %d", ctxt->test.echos);
    cli_event(ctxt, I2, "rx        : %d", ctxt->test.rx);
    cli_event(ctxt, I2, "tx        : %d", ctxt->test.tx);
    cli_event(ctxt, I2, "pkt_size  : %u", sizeof(t_msg_pkt));

    t_timespec timespec;
    if (ctxt->test.running)
      clock_gettime(CLOCK_MONOTONIC, &timespec);
    else if (ctxt->test.tx) {
      timespec.tv_sec  = ctxt->test.stop.tv_sec;
      timespec.tv_nsec = ctxt->test.stop.tv_nsec;
    }
    if (ctxt->test.start.tv_sec || ctxt->test.start.tv_nsec) {
      t_double secs = delta_secs(&ctxt->test.start, &timespec);
      cli_event(ctxt, I2, "duration  : %.6f sec", secs);
      cli_event(ctxt, I2, "pkt rate  : %.3f pkt/sec",
                (t_double)(ctxt->test.tx + ctxt->test.rx)/secs);
      cli_event(ctxt, I2, "throughput: %.3f byte/sec",
                (((t_double)ctxt->test.tx * sizeof(t_msg_pkt))/secs) +
                (((t_double)ctxt->test.rx * sizeof(t_msg_pkt))/secs));
    }
    return false;
  }
  return true;
}

t_bool do_cli_remote_cli(p_ctxt ctxt, t_n argc, p_cstr argv[]) {
  if (argc == 1) {
    t_ix remote = find_remote_by_name(ctxt, argv[0]);
    if (remote < MAX_REMOTES) {
      ctxt->cli.req_remote = remote + 1;
    } else
      cli_event(ctxt, I0, "could not connec to remote %s", argv[0]);
    return false;
  }
  return true;
}

t_bool do_cli_help(p_ctxt ctxt, t_n argc, p_cstr argv[]) {
  if (argc == 0) {
    cli_event(ctxt, I0, "");
    cli_event(ctxt, I0, "supported commands:");
    cli_event(ctxt, I1, "help                                : "
                        "print this message");
    cli_event(ctxt, I1, "info                                : "
                        "messaging instance information");
    cli_event(ctxt, I1, "quit                                : "
                        "quit by leaving the event loop");
    cli_event(ctxt, I1, "change <scope>                      : "
                        "change scope=[process|system|network]");
    cli_event(ctxt, I1, "msngr-add <name> <scope> [grp]      : "
                        "add messenger(or group), scope=[off|process|system|network]");
    cli_event(ctxt, I1, "msngr-del <name>                    : "
                        "remove messenger(or group)");
    cli_event(ctxt, I1, "msngr-chg <name> <scope>            : "
                        "change messenger scope=[off|process|system|network]");
    cli_event(ctxt, I1, "msngr-snd <name> <key> <...>        : "
                        " msngr <name> sends to <key> a message within <>");
    cli_event(ctxt, I1, "msngr-key <key>                     : "
                        "explain key and its association");
    cli_event(ctxt, I1, "msngr-list                          : "
                        "list all messengers learned");
    cli_event(ctxt, I1, "msngr-mon-add <name> <fullname>     : "
                        "<name> monitor <fullname>'s reachability");
    cli_event(ctxt, I1, "msngr-mon-del <name> <fullname>     : "
                        "<name> stops to monitor <fullname>'s reachability");
    cli_event(ctxt, I1, "msngr-grp-attach <key> <name>       : "
                        "<key> is attached to group <name>");
    cli_event(ctxt, I1, "msngr-grp-detach <key> <name>       : "
                        "<key> is detached from group <name>");
    cli_event(ctxt, I1, "msngr-grp-list                      : "
                        "list all groups with their member keys");
    cli_event(ctxt, I1, "test-start <name> [<echos>]         : "
                        "send test frame(s) to <name> and echo until stop");
    cli_event(ctxt, I1, "test-stop                           : "
                        "stop to echo (send frames)");
    cli_event(ctxt, I1, "test-fwd <name>                     : "
                        "set PRx name that can be forwarded to");
    cli_event(ctxt, I1, "test-info                           : "
                        "show test info");
    cli_event(ctxt, I1, "remote-cli <name>                   : "
                        "connect to remote cli (exit to quit)");
    return false;
  }
  return true;
}

t_bool do_cli(p_ctxt ctxt) {
  t_bool (*cli_func)(p_ctxt, t_n, p_cstr argv[]) = NULL;
  t_cli_argv argv;
  t_n        argc = 0;

  if (cli_parse_args(ctxt->cli.req, &argc, argv) && argc > 0) {
    t_cli_option opts[] = {{"quit",              1},
                           {"help",              2},
                           {"info",              3},
                           {"change",            4},
                           {"msngr-add",         5},
                           {"msngr-del",         6},
                           {"msngr-chg",         7},
                           {"msngr-snd",         8},
                           {"msngr-list",        9},
                           {"msngr-mon-add",    10},
                           {"msngr-mon-del",    11},
                           {"msngr-grp-attach", 12},
                           {"msngr-grp-detach", 13},
                           {"msngr-grp-list",   14},
                           {"msngr-key",        15},
                           {"test-fwd",         16},
                           {"test-start",       17},
                           {"test-stop",        18},
                           {"test-info",        19},
                           {"remote-cli",       20}};
    switch (cli_find(argv[0], -1, opts, CLI_OPTIONS(opts))) {
      case 1: return true;
      case 2:  cli_func = do_cli_help;             break;
      case 3:  cli_func = do_cli_info;             break;
      case 4:  cli_func = do_cli_change;           break;
      case 5:  cli_func = do_cli_msngr_add;        break;
      case 6:  cli_func = do_cli_msngr_del;        break;
      case 7:  cli_func = do_cli_msngr_chg;        break;
      case 8:  cli_func = do_cli_msngr_snd;        break;
      case 9:  cli_func = do_cli_msngr_list;       break;
      case 10: cli_func = do_cli_msngr_mon_add;    break;
      case 11: cli_func = do_cli_msngr_mon_del;    break;
      case 12: cli_func = do_cli_msngr_grp_attach; break;
      case 13: cli_func = do_cli_msngr_grp_detach; break;
      case 14: cli_func = do_cli_msngr_grp_list;   break;
      case 15: cli_func = do_cli_msngr_key;        break;
      case 16: cli_func = do_cli_test_fwd;         break;
      case 17: cli_func = do_cli_test_start;       break;
      case 18: cli_func = do_cli_test_stop;        break;
      case 19: cli_func = do_cli_test_info;        break;
      case 20: cli_func = do_cli_remote_cli;       break;
      default:
        if (argv[0][0] != '\0')
          cli_event(ctxt, I0, "unknown (%s). do \"help\" to see commands",
                    argv[0]);
        break;
    }
    if (cli_func && cli_func(ctxt, argc -1, argv + 1))
      cli_event(ctxt, I0, "incorrect use. see help");
  }
  return false;
}

///////////////////////////////////////////////////////////////////////////////

t_bool do_cli_stdin(p_ctxt ctxt, p_bool prompt) {
  t_n n = read(STDIN_FILENO, ctxt->cli.req, sizeof(ctxt->cli.req));
  ctxt->cli.req[n-1] = '\0';
  *prompt = true;
  if (ctxt->cli.req_remote) {
    if (strcmp(ctxt->cli.req, "exit"))
      do_cli_msg_send(ctxt, ctxt->cli.req_remote - 1, CLI_REQ, ctxt->cli.req,
                      n-1);
    else {
      do_cli_msg_send(ctxt, ctxt->cli.req_remote - 1, CLI_QUIT, ctxt->cli.req,
                      0);
      ctxt->cli.req_remote = 0;
      ctxt->cli.req[0] = 0;
    }
    return false;
  }
  return do_cli(ctxt);
}

///////////////////////////////////////////////////////////////////////////////

t_void do_setup(p_ctxt ctxt) {
  srand(time(NULL));

  t_processname processname;
  mk_processname(processname, ctxt->name);

  if (prctl(PR_SET_NAME, processname, NULL, NULL, NULL) < 0)
    exit_program("could not set process name");

  ctxt->system_fd = socket(AF_TIPC, SOCK_RDM, 0);
  if (ctxt->system_fd < 0)
    exit_program("TIPC RDM socket could not be opened");

  t_sockaddr_tipc own;
  unsigned own_len = sizeof(own);
  if (getsockname(ctxt->system_fd, (p_sockaddr)&own, &own_len) != 0)
    exit_program("can not get own socket address");

  cp_portid(&ctxt->system_portid, &own.addr.id); /* think this is local */

  ctxt->network_fd = socket(AF_TIPC, SOCK_RDM, 0);
  if (ctxt->network_fd < 0)
    exit_program("TIPC RDM socket could not be opened");

  own_len = sizeof(own);
  if (getsockname(ctxt->network_fd, (p_sockaddr)&own, &own_len) != 0)
    exit_program("can not get own socket address");

  cp_portid(&ctxt->network_portid, &own.addr.id); /* think this is local */

  ctxt->msg_fd = socket(AF_TIPC, SOCK_RDM, 0);
  if (ctxt->msg_fd < 0)
    exit_program("TIPC RDM socket for receiving messages could not be opened");

  own_len = sizeof(own);
  if (getsockname(ctxt->msg_fd, (p_sockaddr)&own, &own_len) != 0)
    exit_program("can not get own socket address");

  cp_portid(&ctxt->msg_portid, &own.addr.id);

  ctxt->service_fd = socket(AF_TIPC, SOCK_SEQPACKET, 0);
  if (ctxt->service_fd < 0)
    exit_program("TIPC SEQPACKET socket could not be opened");

  own_len = sizeof(own);
  if (getsockname(ctxt->service_fd, (p_sockaddr)&own, &own_len) != 0)
    exit_program("can not get own socket address");

  cp_portid(&ctxt->service_portid, &own.addr.id);

  t_sockaddr_tipc topsrv;
  topsrv.family                  = AF_TIPC;
  topsrv.addrtype                = TIPC_ADDR_NAME;
  topsrv.addr.name.name.type     = TIPC_TOP_SRV; /* local */
  topsrv.addr.name.name.instance = TIPC_TOP_SRV; /* local */
  topsrv.addr.name.domain        = 0;            /* local */

  if (connect(ctxt->service_fd, (p_sockaddr)&topsrv, sizeof(topsrv)) != 0)
    exit_program("failed to connect to the TIPC tracking service");

  ctxt->epoll_fd = epoll_create1(0);
  if (ctxt->epoll_fd == -1)
    exit_program("could not create epoll fd");

  t_epoll_event event;
  event.events  = EPOLLIN;

  if (ctxt->cli.enable) {
    event.data.fd = STDIN_FILENO;
    if (epoll_ctl(ctxt->epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &event) == -1)
      exit_program("could not add stdin fd to epoll");
  }

  event.data.fd = ctxt->system_fd;
  if (epoll_ctl(ctxt->epoll_fd, EPOLL_CTL_ADD, ctxt->system_fd, &event) == -1)
    exit_program("could not add system rdm socket fd (messages) to epoll");

  event.data.fd = ctxt->network_fd;
  if (epoll_ctl(ctxt->epoll_fd, EPOLL_CTL_ADD, ctxt->network_fd, &event) == -1)
    exit_program("could not add network rdm socket fd (messages) to epoll");

  event.data.fd = ctxt->msg_fd;
  if (epoll_ctl(ctxt->epoll_fd, EPOLL_CTL_ADD, ctxt->msg_fd, &event) == -1)
    exit_program("could not add msg rdm socket fd (messages) to epoll");

  event.data.fd = ctxt->service_fd;
  if (epoll_ctl(ctxt->epoll_fd, EPOLL_CTL_ADD, ctxt->service_fd, &event) == -1)
    exit_program("could not add seqpacket socket fd (events) to epoll");
}

t_void do_cleanup(p_ctxt ctxt) {
  close(ctxt->epoll_fd);
  close(ctxt->service_fd);
  close(ctxt->msg_fd);
  close(ctxt->network_fd);
  close(ctxt->system_fd);
}

t_void event_loop(p_ctxt ctxt) {
  t_bool quit   = false;
  t_bool prompt = true;
  cli_event(ctxt, I0, "dainty concept cli: type \"help\" to see commands");
  do {
    t_fd prio_fd[] = { -1, -1, -1, -1};
    t_epoll_event events[3];

    cli_prompt(ctxt, prompt);

    t_n num = epoll_wait(ctxt->epoll_fd, events, 3, -1);
    if (num == -1)
      exit_program("epoll wait failed");

    for (t_ix n = 0; n < num; ++n) {
      t_fd fd = events[n].data.fd;
           if (fd == ctxt->service_fd)
        prio_fd[0] = fd;
      else if (fd == ctxt->network_fd || fd == ctxt->system_fd)
        prio_fd[1] = fd;
      else if (fd == ctxt->msg_fd)
        prio_fd[2] = fd;
      else if (fd == STDIN_FILENO)
        prio_fd[3] = STDIN_FILENO;
    }

    for (t_ix prio = 0; prio < sizeof(prio_fd)/sizeof(t_fd); ++prio) {
      if (prio_fd[prio] != -1) {
        switch (prio) {
          case 0: do_tracking(ctxt);                  break;
          case 1: do_protocol_recv(ctxt, prio_fd[1]); break;
          case 2: quit = do_msg_recv(ctxt, &prompt);  break;
          case 3: quit = do_cli_stdin(ctxt, &prompt); break;
        }
      }
    }
  } while (!quit);
}

t_void do_init(p_ctxt ctxt, t_prx_scope scope, P_cstr name, t_bool cli) {
  init_ctxt(ctxt);
  ctxt->scope      = scope;
  cp_name(ctxt->name, name);
  ctxt->cli.enable = cli;
}

t_void do_start(t_prx_scope scope, P_cstr name, t_bool cli) {
  p_ctxt ctxt = (p_ctxt)malloc(sizeof(t_ctxt));
  if (ctxt) {
    do_init(ctxt, scope, name, cli);
    do_setup(ctxt);

    switch (scope) {
      case PRX_SCOPE_PROCESS:
        break;
      case PRX_SCOPE_SYSTEM:
        add_system_scope(ctxt);
        break;
      case PRX_SCOPE_NETWORK:
        add_system_scope(ctxt);
        add_network_scope(ctxt);
        break;
      default:
       exit_program("unsupported");
       break;
    }

    event_loop(ctxt);
    do_cleanup(ctxt);

    free(ctxt);
  } else
    exit_program("malloc failed for t_ctxt.");
}

///////////////////////////////////////////////////////////////////////////////

t_int main(t_n argc, P_cstr argv[]) {
  if (argc == 3 || argc == 4) {
    t_cli_option opts[] = {{"process", PRX_SCOPE_PROCESS},
                           {"system",  PRX_SCOPE_SYSTEM},
                           {"network", PRX_SCOPE_NETWORK}};
    t_prx_scope scope = cli_find(argv[2], PRX_SCOPE_NONE, opts,
                                 CLI_OPTIONS(opts));
    if (scope != PRX_SCOPE_NONE) {
      t_bool cli = (argc == 4 && strcmp("cli", argv[3]) == 0);
      do_start(scope, argv[1], cli);
      return 0;
    }
  }
  exit_program("usage: PRx <name> <[process|system|network]> [cli]");
  return -1;
}

///////////////////////////////////////////////////////////////////////////////
