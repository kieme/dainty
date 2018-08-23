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

#include "dainty_named.h"

namespace dainty
{
namespace oops
{
////////////////////////////////////////////////////////////////////////////////

  enum t_category {
    UNRECOVERABLE = 0,
    RECOVERABLE   = 1,
    IGNORE        = 2
  };

  using named::t_bool;
  using named::t_void;
  using named::P_cstr;
  using named::P_void;

  enum  t_user_tag_ { };
  using t_user = named::t_user<t_user_tag_>;

  using t_tagid    = named::t_uint16;
  using t_lineno   = named::t_uint16;
  using t_id       = named::t_uint32;
  using t_depth    = named::t_uint16;
  using P_filename = P_cstr;

  struct t_data1 {
    t_data1(t_bool owner, t_bool mem) : owner_(owner), mem_(mem), tag_(0) { }

    const t_bool owner_;
    const t_bool mem_;
    t_tagid      tag_;
  };

  struct t_data2 {
    t_data2(t_bool owner, t_bool mem)
      : owner_(owner), mem_(mem), depth_(0), tag_(0),
        set_(false), line_(0), file_(nullptr) { }

    t_data2(t_bool owner, t_bool mem, t_depth depth)
      : owner_(owner), mem_(mem), depth_(depth), tag_(0),
        set_(false), line_(0), file_(nullptr) { }

    const t_bool  owner_;
    const t_bool  mem_;
    const t_depth depth_;
    t_tagid       tag_;
    t_bool        set_;
    t_lineno      line_;
    P_filename    file_;
  };

  struct t_def {
    t_category category_;
    P_cstr     string_;
    t_id       next_;
    t_user     user_;

    t_def(t_category category, P_cstr string, t_id next = 0,
          t_user user = t_user{0L})
      : category_(category), string_(string), next_(next), user_(user)
    { }
  };

  typedef t_def (*p_what)(t_id);

////////////////////////////////////////////////////////////////////////////////

  struct t_info {
    t_info(P_void ctxt)
      : ctxt_(ctxt), id_(0), what_(0), depth_(0), tag_(0), file_(0), line_(0)
    { }

    inline t_info& set(t_id id, p_what what, t_depth depth,
                       t_tagid tag, P_filename file, t_lineno line) {
      id_    = id;
      what_  = what;
      depth_ = depth;
      tag_   = tag;
      file_  = file;
      line_  = line;
      return *this;
    }

    inline t_info& reset() {
      return set(0, 0, 0, 0, P_cstr{nullptr}, 0);
    }

    P_void     ctxt_;
    t_id       id_;
    p_what     what_;
    t_depth    depth_;
    t_tagid    tag_;
    P_filename file_;
    t_lineno   line_;
  };

////////////////////////////////////////////////////////////////////////////////

  using r_data1 = named::t_prefix<t_data1>::r_;
  using R_data1 = named::t_prefix<t_data1>::R_;
  using r_data2 = named::t_prefix<t_data2>::r_;
  using R_data2 = named::t_prefix<t_data2>::R_;
  using r_info  = named::t_prefix<t_info>::r_;
  using R_info  = named::t_prefix<t_info>::R_;

////////////////////////////////////////////////////////////////////////////////

  typedef t_void (*p_policy)(R_info);
  typedef t_void (*p_print1)(R_info, R_data1);
  typedef t_void (*p_print2)(R_info, R_data2);

////////////////////////////////////////////////////////////////////////////////

  t_def  default_what  (t_id);
  t_void default_policy(R_info);
  t_void default_print (R_info, R_data1);
  t_void default_print (R_info, R_data2);

  t_void trace_step_in (R_info, p_what, P_void ctxt, R_data1);
  t_void trace_step_in (R_info, p_what, P_void ctxt, R_data2);
  t_void trace_step_out(R_info, p_what, P_void ctxt, R_data1);
  t_void trace_step_out(R_info, p_what, P_void ctxt, R_data2);
  t_void trace_step_do (R_info, p_what, P_void ctxt, R_data1);
  t_void trace_step_do (R_info, p_what, P_void ctxt, R_data2);

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

  template<p_policy A = default_policy, p_print P = default_print>
  class t_ctxt {
  public:
    t_ctxt();

    void set(t_id, p_what, R_data1);
    void set(t_id, p_what, R_data2);
    void set(R_info);

    t_id    get_id   () const;
    t_depth get_depth() const;
    p_what  get_what () const;
    t_info  get_info () const;

    t_info clear();

    void print(R_data) const;
    P_cstr what() const;

    void step_in (R_data, p_what);
    void step_out(R_data, p_what);
    void step_do (R_data, p_what);

  private:
    t_info info_;
  };

////////////////////////////////////////////////////////////////////////////////

  template<p_policy A, p_print P>
  inline
  t_ctxt<A, P>::t_ctxt() : info_(this) {
  }

  template<p_policy A, p_print P>
  inline
  t_void t_ctxt<A, P>::set(t_id id, p_what what, R_data1 data) {
    A(info_.set(id, what, 0, data.tag_, P_cstr{nullptr}, 0));
  }

  template<p_policy A, p_print P>
  inline
  t_void t_ctxt<A, P>::set(t_id id, p_what what, R_data2 data) {
    A(info_.set(id, what, data.depth_, data.tag_, data.file_, data.line_));
  }

  template<p_policy A, p_print P>
  inline
  t_void t_ctxt<A, P>::set(R_info info) {
    info_ = info;
    A(info_);
  }

  template<p_policy A, p_print P>
  inline
  t_id t_ctxt<A, P>::get_id() const {
    return info_.id_;
  }

  template<p_policy A, p_print P>
  inline
  t_depth t_ctxt<A, P>::get_depth() const {
    return info_.depth_;
  }

  template<p_policy A, p_print P>
  inline
  p_what t_ctxt<A, P>::get_what() const {
    return info_.what_;
  }

  template<p_policy A, p_print P>
  inline
  t_info t_ctxt<A, P>::get_info() const {
    return info_;
  }

  template<p_policy A, p_print P>
  inline
  t_void t_ctxt<A, P>::print(R_data data) const {
    P(info_, data);
  }

  template<p_policy A, p_print P>
  inline
  P_cstr t_ctxt<A, P>::what() const {
    return info_.what_ ? P_cstr{"no oops"} : info_.what_(info_.id_).string_;
  }

  template<p_policy A, p_print P>
  inline
  t_info t_ctxt<A, P>::clear() {
    t_info tmp = info_;
    info_.reset();
    return tmp;
  }

  template<p_policy A, p_print P>
  inline
  t_void t_ctxt<A, P>::step_in(R_data data, p_what what) {
    trace_step_in(info_, what, this, data);
  }

  template<p_policy A, p_print P>
  inline
  t_void t_ctxt<A, P>::step_out(R_data data, p_what what) {
    trace_step_out(info_, what, this, data);
  }

  template<p_policy A, p_print P>
  inline
  t_void t_ctxt<A, P>::step_do(R_data data, p_what what) {
    trace_step_do(info_, what, this, data);
  }
}
}

#endif
