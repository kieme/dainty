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

#ifndef _DAINTY_OOPS_CTXT_H_
#define _DAINTY_OOPS_CTXT_H_

#include "dainty_base.h"

namespace dainty
{
namespace oops
{
  using base::t_prefix;
  using base::T_bool;
  using base::t_bool;
  using base::t_void;
  using base::P_cstr;
  using base::P_void;
  using base::t_errn_;
  using base::t_errn;
  using base::t_validity;
  using base::NO_ERRN_;
  using base::NO_ERRN;
  using base::BAD_ERRN_;
  using base::BAD_ERRN;
  using base::VALID;
  using base::INVALID;

////////////////////////////////////////////////////////////////////////////////

  enum t_category {
    UNRECOVERABLE = 0,
    RECOVERABLE   = 1,
    IGNORE        = 2
  };
  using T_category = t_prefix<t_category>::T_;

  enum  t_user_tag_ { };
  using t_user = base::t_user<t_user_tag_>;

  using t_tagid    = base::t_uint16;
  using t_lineno   = base::t_uint16;
  using t_id       = base::t_uint32;
  using t_depth    = base::t_uint16;
  using T_depth    = base::T_uint16;
  using P_filename = P_cstr;

////////////////////////////////////////////////////////////////////////////////

  struct t_data1 {
    inline
    t_data1(t_bool _owner, t_bool _mem) noexcept
      : owner{_owner}, mem{_mem}, tag{0} {
    }

    T_bool  owner;
    T_bool  mem;
    t_tagid tag;
  };

  struct t_data2 {
    inline
    t_data2(t_bool _owner, t_bool _mem) noexcept
      : owner{_owner}, mem{_mem} {
    }

    inline
    t_data2(t_bool _owner, t_bool _mem, t_depth _depth) noexcept
      : owner{_owner}, mem{_mem}, depth{_depth} {
    }

    T_bool     owner;
    T_bool     mem;
    T_depth    depth = 0;
    t_tagid    tag   = 0;
    t_bool     set   = false;
    t_lineno   line  = 0;
    P_filename file  = P_cstr{nullptr};
  };

  struct t_def {
    inline
    t_def(t_category _category, P_cstr _string) noexcept
      : category{_category}, string{_string} {
    }

    inline
    t_def(t_category _category, P_cstr _string, t_id _next) noexcept
      : category{_category}, string{_string}, next{_next} {
    }

    inline
    t_def(t_category _category, P_cstr _string, t_id _next, t_user _user) noexcept
      : category{_category}, string{_string}, next{_next}, user{_user} {
    }

    t_category category;
    P_cstr     string;
    t_id       next     = 0;
    t_user     user     = t_user{0L};
  };

  using p_what = t_def (*)(t_id);

////////////////////////////////////////////////////////////////////////////////

  struct t_info;
  using  r_info = t_prefix<t_info>::r_;
  using  R_info = t_prefix<t_info>::R_;

  struct t_info {
    inline
    t_info(P_void _ctxt) noexcept : ctxt{_ctxt} {
    }

    inline
    r_info reset() {
      ctxt  = nullptr;
      id    = 0;
      what  = nullptr;
      depth = 0;
      tag   = 0;
      file  = P_filename{nullptr};
      line  = 0;
      return *this;
    }

    P_void     ctxt;
    t_id       id    = 0;
    p_what     what  = nullptr;
    t_depth    depth = 0;
    t_tagid    tag   = 0;
    P_filename file  = P_filename{nullptr};
    t_lineno   line  = 0;
  };

////////////////////////////////////////////////////////////////////////////////

  using r_data1 = base::t_prefix<t_data1>::r_;
  using R_data1 = base::t_prefix<t_data1>::R_;
  using r_data2 = base::t_prefix<t_data2>::r_;
  using R_data2 = base::t_prefix<t_data2>::R_;

////////////////////////////////////////////////////////////////////////////////

  using p_policy = t_void (*)(R_info);
  using p_print1 = t_void (*)(R_info, R_data1);
  using p_print2 = t_void (*)(R_info, R_data2);

////////////////////////////////////////////////////////////////////////////////

  t_void default_policy(R_info)          noexcept;
  t_def  default_what  (t_id)            noexcept;
  t_void default_print (R_info, R_data1) noexcept;
  t_void default_print (R_info, R_data2) noexcept;

  t_void trace_step_in_ (R_info, p_what, P_void ctxt, R_data1) noexcept;
  t_void trace_step_in_ (R_info, p_what, P_void ctxt, R_data2) noexcept;
  t_void trace_step_out_(R_info, p_what, P_void ctxt, R_data1) noexcept;
  t_void trace_step_out_(R_info, p_what, P_void ctxt, R_data2) noexcept;
  t_void trace_step_do_ (R_info, p_what, P_void ctxt, R_data1);
  t_void trace_step_do_ (R_info, p_what, P_void ctxt, R_data2);

////////////////////////////////////////////////////////////////////////////////

#ifdef OOPS_BASIC
  using t_data  = t_data1;
  using r_data  = r_data1;
  using R_data  = R_data1;
  using p_print = p_print1;
#else
  using t_data  = t_data2;
  using r_data  = r_data2;
  using R_data  = R_data2;
  using p_print = p_print2;
#endif

////////////////////////////////////////////////////////////////////////////////

  template<p_policy POLICY = default_policy, p_print PRINT = default_print>
  class t_ctxt {
  public:
    using r_ctxt = typename t_prefix<t_ctxt>::r_;
    using x_ctxt = typename t_prefix<t_ctxt>::x_;
    using R_ctxt = typename t_prefix<t_ctxt>::R_;

    t_ctxt() noexcept;

    t_ctxt(R_ctxt)           = delete;
    t_ctxt(x_ctxt)           = delete;
    r_ctxt operator=(R_ctxt) = delete;
    r_ctxt operator=(x_ctxt) = delete;

    t_void set(t_id, p_what, R_data1);
    t_void set(t_id, p_what, R_data2);
    t_void set(R_info);

    t_info clear() noexcept;

    t_id    get_id   () const noexcept;
    t_depth get_depth() const noexcept;
    p_what  get_what () const noexcept;
    t_info  get_info () const noexcept;

    t_void print(R_data) const noexcept;
    P_cstr what()        const noexcept;

    t_void step_in (R_data, p_what) noexcept;
    t_void step_out(R_data, p_what) noexcept;
    t_void step_do (R_data, p_what);

  private:
    t_info info_;
  };

////////////////////////////////////////////////////////////////////////////////

  template<p_policy POLICY, p_print PRINT>
  inline
  t_ctxt<POLICY, PRINT>::t_ctxt() noexcept : info_{this} {
  }

  template<p_policy POLICY, p_print PRINT>
  inline
  t_void t_ctxt<POLICY, PRINT>::set(t_id id, p_what what, R_data1 data) {
    info_.id    = id;
    info_.what  = what;
    info_.depth = 0;
    info_.tag   = data.tag;
    info_.file  = nullptr;
    info_.line  = 0;
    POLICY(info_);
  }

  template<p_policy POLICY, p_print PRINT>
  inline
  t_void t_ctxt<POLICY, PRINT>::set(t_id id, p_what what, R_data2 data) {
    info_.id    = id;
    info_.what  = what;
    info_.depth = data.depth;
    info_.tag   = data.tag;
    info_.file  = data.file;
    info_.line  = data.line;
    POLICY(info_);
  }

  template<p_policy POLICY, p_print PRINT>
  inline
  t_void t_ctxt<POLICY, PRINT>::set(R_info info) {
    info_ = info;
    POLICY(info_);
  }

  template<p_policy POLICY, p_print PRINT>
  inline
  t_id t_ctxt<POLICY, PRINT>::get_id() const noexcept {
    return info_.id;
  }

  template<p_policy POLICY, p_print PRINT>
  inline
  t_depth t_ctxt<POLICY, PRINT>::get_depth() const noexcept {
    return info_.depth;
  }

  template<p_policy POLICY, p_print PRINT>
  inline
  p_what t_ctxt<POLICY, PRINT>::get_what() const noexcept {
    return info_.what;
  }

  template<p_policy POLICY, p_print PRINT>
  inline
  t_info t_ctxt<POLICY, PRINT>::get_info() const noexcept {
    return info_;
  }

  template<p_policy POLICY, p_print PRINT>
  inline
  t_void t_ctxt<POLICY, PRINT>::print(R_data data) const noexcept {
    PRINT(info_, data);
  }

  template<p_policy POLICY, p_print PRINT>
  inline
  P_cstr t_ctxt<POLICY, PRINT>::what() const noexcept {
    return info_.what ? P_cstr{"no oops"} : info_.what(info_.id).string;
  }

  template<p_policy POLICY, p_print PRINT>
  inline
  t_info t_ctxt<POLICY, PRINT>::clear() noexcept {
    t_info tmp = info_;
    info_.reset();
    return tmp;
  }

  template<p_policy POLICY, p_print PRINT>
  inline
  t_void t_ctxt<POLICY, PRINT>::step_in(R_data data, p_what what) noexcept {
    trace_step_in_(info_, what, this, data);
  }

  template<p_policy POLICY, p_print PRINT>
  inline
  t_void t_ctxt<POLICY, PRINT>::step_out(R_data data, p_what what) noexcept {
    trace_step_out_(info_, what, this, data);
  }

  template<p_policy POLICY, p_print PRINT>
  inline
  t_void t_ctxt<POLICY, PRINT>::step_do(R_data data, p_what what) {
    trace_step_do_(info_, what, this, data);
  }
}
}

#endif
