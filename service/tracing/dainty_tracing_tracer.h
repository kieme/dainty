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

#ifndef _DAINTY_TRACING_TRACER_H_
#define _DAINTY_TRACING_TRACER_H_

#include "dainty_container_freelist.h"
#include "dainty_named_string.h"
#include "dainty_tracing_err.h"

#define DAINTY_TR_(TR, LEVEL, TEXT)                                         \
  dainty::tracing::tracer::ref(TR).post((LEVEL), (TEXT));

#define DAINTY_TR_EMERG(TR, TEXT)                                           \
  DAINTY_TR_(TR, dainty::tracing::tracer::EMERG, TEXT)
#define DAINTY_TR_ALERT(TR, TEXT)                                           \
  DAINTY_TR_(TR, dainty::tracing::tracer::ALERT, TEXT)
#define DAINTY_TR_CRITICAL(TR, TEXT)                                        \
  DAINTY_TR_(TR, dainty::tracing::tracer::CRITICAL, TEXT)
#define DAINTY_TR_ERROR(TR, TEXT)                                           \
  DAINTY_TR_(TR, dainty::tracing::tracer::ERROR, TEXT)
#define DAINTY_TR_WARNING(TR, TEXT)                                         \
  DAINTY_TR_(TR, dainty::tracing::tracer::WARNING, TEXT)
#define DAINTY_TR_NOTICE(TR, TEXT)                                          \
  DAINTY_TR_(TR, dainty::tracing::tracer::NOTICE, TEXT)
#define DAINTY_TR_INFO(TR, TEXT)                                            \
  DAINTY_TR_(TR, dainty::tracing::tracer::INFO, TEXT)
#define DAINTY_TR_DEBUG(TR, TEXT)                                           \
  DAINTY_TR_(TR, dainty::tracing::tracer::DEBUG, TEXT)

///////////////////////////////////////////////////////////////////////////////

namespace dainty
{
namespace tracing
{
namespace tracer
{
  using named::string::t_string;
  using named::t_void;
  using named::t_bool;
  using named::t_validity;
  using named::VALID;
  using named::INVALID;
  using named::string::FMT;

///////////////////////////////////////////////////////////////////////////////

  using t_impl_id_ = container::freelist::t_id;
  using t_credit   = named::t_uint32;

  enum  t_name_tag_ {};
  using t_name = t_string<t_name_tag_, 32>;
  using R_name = named::t_prefix<t_name>::R_;

  enum  t_text_tag_ {};
  using t_text = t_string<t_text_tag_, 80>;
  using R_text = named::t_prefix<t_text>::R_;

///////////////////////////////////////////////////////////////////////////////

  enum t_level {
    EMERG    = 0,
    ALERT    = 1,
    CRITICAL = 2,
    ERROR    = 3,
    WARNING  = 4,
    NOTICE   = 5,
    INFO     = 6,
    DEBUG    = 7
  };

  enum  t_levelname_tag_ { };
  using t_levelname = t_string<t_levelname_tag_, 10>;
  using R_levelname = named::t_prefix<t_levelname>::R_;

  t_levelname to_name(t_level);

///////////////////////////////////////////////////////////////////////////////

  t_level  default_level ();
  t_credit default_credit();

  class t_params {
  public:
    t_level  level;
    t_credit credit;

    t_params(t_level  _level  = default_level(),
             t_credit _credit = default_credit())
      : level (_level),
        credit(_credit) {
    }
  };

  using r_params = named::t_prefix<t_params>::r_;
  using R_params = named::t_prefix<t_params>::R_;

///////////////////////////////////////////////////////////////////////////////

  class t_id {
  public:
    using t_seq = named::t_int32;

    t_id();
    t_id(const t_id&);

    operator t_validity() const;

    t_id release();

  private:
    friend t_void set_(t_id&, t_seq, t_impl_id_);
    friend t_impl_id_ get_(const t_id&);
    friend t_bool operator==(const t_id&, const t_id&);
    friend class t_point;
    t_id(t_seq, t_impl_id_);

    t_seq      seq_;
    t_impl_id_ id_;
  };

  using R_id = named::t_prefix<t_id>::R_;

///////////////////////////////////////////////////////////////////////////////

  class t_point {
  public:
    t_point() = default;
    t_point(t_point&&);
    t_point& operator=(t_point&&);

    t_point& operator=(const t_point&) = delete;
    t_point(const t_point&)            = delete;

    t_validity post(       t_level, R_text) const;
    t_validity post(t_err, t_level, R_text) const;

    t_validity waitable_post(       t_level, R_text) const;
    t_validity waitable_post(t_err, t_level, R_text) const;

    t_name  get_name () const;
    t_level get_level() const;

    operator t_validity() const;

  private:
    friend class t_tracer;
    t_point(R_id, R_name);
    t_point release();

    t_id   id_;
    t_name name_;
  };

  using r_point = named::t_prefix<t_point>::r_;
  using R_point = named::t_prefix<t_point>::R_;
  using p_point = named::t_prefix<t_point>::p_;
  using P_point = named::t_prefix<t_point>::P_;
  using x_point = named::t_prefix<t_point>::x_;

////////////////////////////////////////////////////////////////////////////////

  class t_tracer {
  public:
    t_tracer() = default;
    t_tracer(t_tracer&&);
    t_tracer& operator=(t_tracer&&);
    ~t_tracer();

    t_tracer(const t_tracer&)            = delete;
    t_tracer& operator=(const t_tracer&) = delete;

    operator t_validity() const;

    p_point operator->();
    P_point operator->() const;

    r_point operator*();
    R_point operator*() const;

    t_point make_point(R_name);

  private:
    friend t_tracer mk_(R_id, R_name);
    t_tracer(R_id, R_name);

    t_point point_;
  };

  using r_tracer = named::t_prefix<t_tracer>::r_;
  using R_tracer = named::t_prefix<t_tracer>::R_;
  using x_tracer = named::t_prefix<t_tracer>::x_;

///////////////////////////////////////////////////////////////////////////////

  t_validity shared_trace(       t_level, R_text);
  t_validity shared_trace(t_err, t_level, R_text);

////////////////////////////////////////////////////////////////////////////////

  inline r_point ref(r_point point)   { return point; }
  inline R_point ref(R_point point)   { return point; }

  inline r_point ref(r_tracer tracer) { return *tracer;}
  inline R_point ref(R_tracer tracer) { return *tracer;}

////////////////////////////////////////////////////////////////////////////////

  inline
  t_id::t_id() : seq_(-1), id_(0) {
  }

  inline
  t_id::t_id(R_id id) : seq_(id.seq_), id_(id.id_){
  }

  inline
  t_id::operator t_validity() const {
    return seq_ != -1 ? VALID : INVALID;
  }

  inline
  t_id t_id::release() {
    t_id tmp(*this);
    seq_ = -1;
    return tmp;
  }

  inline
  t_id::t_id(t_seq seq, t_impl_id_ id) : seq_(seq), id_(id) {
  }

///////////////////////////////////////////////////////////////////////////////

  inline
  t_point::t_point(x_point point)
    : id_(point.id_.release()), name_(std::move(point.name_)) {
  }

  inline
  t_point& t_point::operator=(x_point point) { // note: nothing to delete
    id_   = point.id_.release();
    name_ = std::move(point.name_);
    return *this;
  }

  inline
  t_point::operator t_validity() const {
    return id_;
  }

  inline
  t_point::t_point(R_id id, R_name name) : id_(id), name_{name} {
  }

  inline
  t_point t_point::release() {
    return t_point(id_.release(), name_);
  }

////////////////////////////////////////////////////////////////////////////////

  inline
  t_tracer::operator t_validity() const {
    return point_;
  }

  inline
  p_point t_tracer::operator->() {
    return &point_;
  }

  inline
  P_point t_tracer::operator->() const {
    return &point_;
  }

  inline
  r_point t_tracer::operator*() {
    return point_;
  }

  inline
  R_point t_tracer::operator*() const {
    return point_;
  }

  inline
  t_tracer::t_tracer(R_id id, R_name name) : point_(id, name) {
  }
}
}
}

#endif
