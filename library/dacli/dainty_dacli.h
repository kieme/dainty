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

#ifndef _DAINTY_DACLI_H_
#define _DAINTY_DACLI_H_

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <map>

#include "dainty_base.h"
#include "dainty_base_string.h"
#include "dainty_oops.h"
#include "dainty_dacli_err.h"

/******************************************************************************/

namespace dainty
{
namespace dacli
{
  using base::t_void;
  using base::t_bool;
  using base::t_prefix;
  using base::string::t_string;
  using base::string::operator""_SL;
  using err::t_err;
  using err::r_err;

  using t_string_crange = base::string::t_crange;

/******************************************************************************/

namespace argn
{
  using base::t_ix;
  using base::t_n;

  using base::operator"" _ix;
  using base::operator"" _bix;
  using base::operator"" _eix;
  using base::operator"" _n;

  constexpr base::t_uint32 OPTIONAL = 0x10000000;

  enum t_type : base::t_uint32 {
    TYPE_Q   = 0x00000000,  // 1.
    TYPE_A   = 0x00000001,  // 1.
    TYPE_X   = 0x00000002,  // 2.
    TYPE_Z   = 0x00000004,  // 3.
    TYPE_S   = 0x00000008,  // 4.
    TYPE_C   = 0x00000010,  // 5.
    TYPE_L   = 0x00000020,  // 6.
    TYPE_MB  = 0x00000040,  // 7.
    TYPE_B   = 0x00100040,
    TYPE_H   = 0x00000080,  // 8.
    TYPE_K   = 0x00000100,  // 9.
    TYPE_XI  = 0x00000200,  // 10.
    TYPE_OA  = OPTIONAL | TYPE_A,
    TYPE_OX  = OPTIONAL | TYPE_X,
    TYPE_OZ  = OPTIONAL | TYPE_Z,
    TYPE_OS  = OPTIONAL | TYPE_S,
    TYPE_OC  = OPTIONAL | TYPE_C,
    TYPE_OL  = OPTIONAL | TYPE_L,
    TYPE_OB  = OPTIONAL | TYPE_MB, // looks wrong - XXX
    TYPE_OH  = OPTIONAL | TYPE_H,
    TYPE_OK  = OPTIONAL | TYPE_K
  };

  constexpr t_bool is_optional (t_type type) { return OPTIONAL & type; }
  constexpr t_bool is_simple   (t_type type) { return TYPE_S   & type; }
  constexpr t_bool is_array    (t_type type) { return TYPE_A   & type; }
  constexpr t_bool is_lookup   (t_type type) { return TYPE_X   & type; }
  constexpr t_bool is_compound (t_type type) { return TYPE_C   & type; }
  constexpr t_bool is_option   (t_type type) { return TYPE_Z   & type; }
  constexpr t_bool is_list     (t_type type) { return TYPE_L   & type; }
  constexpr t_bool is_boolean  (t_type type) { return TYPE_MB  & type; }
  constexpr t_bool is_selection(t_type type) { return TYPE_H   & type; }

  constexpr t_type get_base_type(t_type type) {
    return (t_type)(0xFFFF & type);
  }

  constexpr t_type get_path_type(t_type type) {
    switch (type) {
      case TYPE_H:
      case TYPE_OH:
      case TYPE_C:
      case TYPE_OC:
      case TYPE_S:
      case TYPE_OS:
        return TYPE_S;
      case TYPE_B:
      case TYPE_OB:
      case TYPE_MB:
        return TYPE_MB;
      case TYPE_A:
      case TYPE_OA:
        return TYPE_A;
      case TYPE_X:
      case TYPE_OX:
        return TYPE_X;
      case TYPE_XI:
        return TYPE_XI;
      case TYPE_Z:
      case TYPE_OZ:
        return TYPE_Z;
      case TYPE_K:
      case TYPE_OK:
      case TYPE_L:
      case TYPE_OL:
        return TYPE_L;
      default:
        break; // assert
    }
    return TYPE_S; // can you do better
  }

  constexpr t_string_crange str(t_type argype) {
    switch (argype) {
      case TYPE_Q:   return "Q"_SL;
      case TYPE_A:   return "A"_SL;
      case TYPE_OA:  return "OA"_SL;
      case TYPE_X:   return "X"_SL;
      case TYPE_OX:  return "OX"_SL;
      case TYPE_XI:  return "IX"_SL;
      case TYPE_Z:   return "Z"_SL;
      case TYPE_OZ:  return "OZ"_SL;
      case TYPE_S:   return "S"_SL;
      case TYPE_OS:  return "OS"_SL;
      case TYPE_C:   return "C"_SL;
      case TYPE_OC:  return "OC"_SL;
      case TYPE_L:   return "L"_SL;
      case TYPE_OL:  return "OL"_SL;
      case TYPE_B:   return "B"_SL;
      case TYPE_OB:  return "OB"_SL;
      case TYPE_MB:  return "MB"_SL;
      case TYPE_H:   return "H"_SL;
      case TYPE_OH:  return "OH"_SL;
      case TYPE_K:   return "K"_SL;
      case TYPE_OK:  return "OK"_SL;
    }
    return t_string_crange{};
  }

  constexpr t_string_crange str(t_bool state) {
    return state ? "<true>"_SL : "<false>"_SL;
  }

/******************************************************************************/

  enum  t_word_tag_ {};
  using t_word      = t_string<t_word_tag_>; // heap - 0
  using r_word      = t_prefix<t_word>::r_;
  using R_word      = t_prefix<t_word>::R_;

  using t_words     = std::vector<t_word>; // change to t_segmented_string
  using R_words     = t_prefix<t_words>::R_;

  using t_path      = t_words;

  using t_name     = t_word;
  using r_name     = t_prefix<t_name>::r_;
  using R_name     = t_prefix<t_name>::R_;
  using x_name     = t_prefix<t_name>::x_;

  using t_value    = t_word;
  using r_value    = t_prefix<t_value>::r_;
  using R_value    = t_prefix<t_value>::R_;
  using x_value    = t_prefix<t_value>::x_;

  using t_fullname = t_words;
  using R_fullname = t_prefix<t_fullname>::R_;

  using t_values   = t_words;
  using r_values   = t_prefix<t_values>::r_;
  using R_values   = t_prefix<t_values>::R_;
  using x_values   = t_prefix<t_values>::x_;

  t_void print(t_string_crange prefix, R_words);

  struct t_arginfo {
    t_arginfo(t_type type) : type_(type) {
      list_.options_ = 0;
    }

    t_arginfo(t_type type, t_n::t_value max, t_n::t_value min = 0,
              t_bool init = false) : type_(type) {
      range_.max_  = max;
      range_.min_  = min;
      range_.cnt_  = 0;
      range_.init_ = init;
    }

    t_type type_;
    union {
      struct {
        t_n::t_value max_  = 0;
        t_n::t_value min_  = 0;
        t_n::t_value cnt_  = 0;
        t_bool       init_ = false;
      } range_;
      struct {
        base::t_uint16 options_ = 0;
      } list_;
    };
    t_name                     ext_;
    std::vector<base::p_void> mem_; // XXX - placeholder for a freelistid
  };
  using r_arginfo = t_prefix<t_arginfo>::r_;
  using R_arginfo = t_prefix<t_arginfo>::R_;

  struct t_argvalue {
    t_argvalue(R_arginfo info) : info_(info) {
    }

    t_arginfo info_;
    t_values   values_;
    t_path     path_;
  };

  struct arg_compare {
    t_bool operator()(R_fullname lh, R_fullname rh) const;
  };

////////////////////////////////////////////////////////////////////////////////

  using t_table = std::map<t_fullname, t_argvalue, arg_compare>;
  using t_itr   = t_table::iterator;        //impl
  using t_citr  = t_table::const_iterator;  // impl
  using t_pair  = std::pair<t_itr, t_bool>; // impl
  using t_arg   = t_table::value_type;
  using r_arg   = t_prefix<t_arg>::r_;
  using R_arg   = t_prefix<t_arg>::R_;
  using p_arg   = t_prefix<t_arg>::p_;
  using P_arg   = t_prefix<t_arg>::P_;
  using x_arg   = t_prefix<t_arg>::x_;

////////////////////////////////////////////////////////////////////////////////

  // XXX must be cleaned up
  struct t_optional_params {
    t_bool optional_;
    constexpr t_optional_params(t_bool optional = false) noexcept
      : optional_(optional) {
    }
  };
  using t_oparams = t_prefix<t_optional_params>::t_;
  using R_oparams = t_prefix<t_oparams>::R_;

  constexpr t_oparams OPTIONAL_TRUE {true};  // XXX not really happy
  constexpr t_oparams OPTIONAL_FALSE{false}; // XXX use explicit

  struct t_range_params {
    t_n range_max_;
    t_n range_min_;
    constexpr t_range_params(t_n max = 0_n, t_n min = 0_n)
      : range_max_(max), range_min_(min) { }
  };
  using t_rparams = t_prefix<t_range_params>::t_;
  using r_rparams = t_prefix<t_rparams>::r_;
  using R_rparams = t_prefix<t_rparams>::R_;

  struct t_params : t_oparams, t_rparams {
    t_params() = default;
    t_params(R_oparams oparams, R_rparams rparams)
      : t_oparams(oparams), t_rparams(rparams) {
    }
  };
  using r_params = t_prefix<t_params>::r_;
  using R_params = t_prefix<t_params>::R_;

  // XXX - naming
  static const t_params unbound_range{};
  static const t_params optional_unbound_range{t_oparams{true}, t_rparams{}};

  inline t_bool operator==(R_rparams lh, R_rparams rh) {
    return lh.range_max_ == rh.range_max_ &&
           lh.range_min_ == rh.range_min_;
  }

  inline t_bool operator!=(R_rparams lh, R_rparams rh) {
    return !(lh == rh);
  }

////////////////////////////////////////////////////////////////////////////////

  class t_argn;
  using p_argn  = t_prefix<t_argn>::p_;
  using P_argn  = t_prefix<t_argn>::P_;
  using r_argn  = t_prefix<t_argn>::r_;
  using R_argn  = t_prefix<t_argn>::R_;
  using x_argn  = t_prefix<t_argn>::x_;

  class t_ref;
  using r_ref = t_prefix<t_ref>::r_;
  using R_ref = t_prefix<t_ref>::R_;

  class t_ref { // add maybe optional_params
    struct t_cid {
      P_argn argn_ = nullptr;
      P_arg  arg_  = nullptr;
      t_cid(P_argn argn, P_arg arg) : argn_(argn), arg_(arg) {}
    };
  public:
    inline t_ref() = default;
    inline t_ref(       R_ref) = default;
    inline t_ref(t_err, R_ref);

    inline r_ref operator=(R_ref) = default;

    t_type            get_type() const;
    t_type            get_base_type() const;
    R_name            get_name() const;
    R_fullname        get_fullname() const;
    t_optional_params get_optional_params() const;

    inline operator t_bool() const                 { return is_valid_(); }

  protected:
    inline t_ref(p_argn argn, p_arg arg)  : argn_(argn), arg_(arg)     { }

    inline t_bool is_valid_() const              { return argn_ && arg_; }
    inline t_void clear_()            { argn_ = nullptr; arg_ = nullptr; }
    inline r_arg  set_()                                { return *arg_;  }
    inline R_arg  get_() const                          { return *arg_;  }
    inline r_argn set_argn_()                           { return *argn_; }
    inline R_argn get_argn_() const                     { return *argn_; }

    inline t_ref make_ref_(p_arg arg)             { return {argn_, arg}; }
    inline t_cid make_id_ (P_arg arg) const       { return {argn_, arg}; }

  private:
    friend class t_argn;
    friend class t_cref;
    p_argn argn_ = nullptr;
    p_arg  arg_  = nullptr;
  };

  class t_cref;
  using r_cref = t_prefix<t_cref>::r_;
  using R_cref = t_prefix<t_cref>::R_;

  class t_cref {
  public:
    inline t_cref() = default;
    inline t_cref(       R_cref) = default;
    inline t_cref(t_err, R_cref);
    inline t_cref(R_ref ref) : argn_(ref.argn_), arg_(ref.arg_)        { }
    inline t_cref(t_ref::t_cid id)  : argn_(id.argn_), arg_(id.arg_)   { }
    inline r_cref operator=(R_cref) = default;

    t_type            get_type() const;
    t_type            get_base_type() const;
    R_name            get_name() const;
    R_fullname        get_fullname() const;
    t_optional_params get_optional_params() const;

    inline operator t_bool() const                 { return is_valid_(); }

  protected:
    inline t_cref(P_argn argn, P_arg arg) : argn_(argn), arg_(arg)     { }

    inline t_bool is_valid_() const              { return argn_ && arg_; }
    inline t_void clear_()            { argn_ = nullptr; arg_ = nullptr; }
    inline R_arg  get_() const                          { return *arg_;  }
    inline R_argn get_argn_() const                     { return *argn_; }

    inline t_cref make_ref_(P_arg arg) const      { return {argn_, arg}; }

  private:
    friend class t_argn;
    P_argn argn_ = nullptr;
    P_arg  arg_  = nullptr;
  };

////////////////////////////////////////////////////////////////////////////////

  class t_compound_ref;
  using r_compound_ref = t_prefix<t_compound_ref>::r_;

  class t_simple_ref : public t_ref {
  public:
    t_simple_ref(       t_ref);
    t_simple_ref(t_err, t_ref);

    t_bool transform_to(t_err, r_compound_ref, t_values values);

    R_value get_value() const;
    t_bool  set_value(t_err, t_value);
  };
  using r_simple_ref = t_prefix<t_simple_ref>::r_;

  class t_simple_cref : public t_cref {
  public:
    t_simple_cref(       t_cref);
    t_simple_cref(t_err, t_cref);

    R_value get_value() const;
  };

////////////////////////////////////////////////////////////////////////////////

  class t_boolean_ref : public t_ref {
  public:
    t_boolean_ref(       t_ref);
    t_boolean_ref(t_err, t_ref);

    t_bool get_value() const;
    t_bool set_value(t_err, bool); // if it is none modifyable?
  };

  class t_boolean_cref : public t_cref {
  public:
    t_boolean_cref(       t_cref);
    t_boolean_cref(t_err, t_cref);

    t_bool get_value() const;
  };

////////////////////////////////////////////////////////////////////////////////

  class t_array_ref : public t_ref {
  public:
    t_array_ref(       t_ref);
    t_array_ref(t_err, t_ref);

    t_range_params get_range_params() const;

    t_bool set_as_initialized();
    t_bool is_initialized() const;

    R_values get_values() const;
    t_bool   set_values(t_err, t_values);
  };

  class t_array_cref : public t_cref {
  public:
    t_array_cref(t_cref);
    t_array_cref(t_err, t_cref);

    t_range_params get_range_params() const;

    t_bool   is_initialized() const;
    R_values get_values() const;
  };

////////////////////////////////////////////////////////////////////////////////

  class t_compound_ref : public t_ref {
  public:
    t_compound_ref(       t_ref);
    t_compound_ref(t_err, t_ref);

    t_bool add_missing_part_values(t_err, R_values);

    R_values get_part_values() const;
    R_value  get_value() const;
    t_bool   set_value(t_err, t_value);
  };

  class t_compound_cref : public t_cref {
  public:
    t_compound_cref(       t_cref);
    t_compound_cref(t_err, t_cref);

    R_values get_part_values() const;
    R_value  get_value() const;
  };

////////////////////////////////////////////////////////////////////////////////

  class t_selection_ref : public t_ref {
  public:
    t_selection_ref(       t_ref);
    t_selection_ref(t_err, t_ref);

    R_values get_values() const;
    R_value  get_value()  const;
    t_bool   set_value (t_err, t_value);
    t_bool   test_value(t_err, t_value) const;
  };

  class t_selection_cref : public t_cref {
  public:
    t_selection_cref(       t_cref);
    t_selection_cref(t_err, t_cref);

    R_values get_values() const;
    R_value  get_value() const;
    t_bool   test_value(t_err, t_value) const;
  };

////////////////////////////////////////////////////////////////////////////////

  class t_openlist_ref : public t_ref {
  public:
    t_openlist_ref(       t_ref);
    t_openlist_ref(t_err, t_ref);
  };

  class t_openlist_cref : public t_cref {
  public:
    t_openlist_cref(       t_cref);
    t_openlist_cref(t_err, t_cref);
  };

////////////////////////////////////////////////////////////////////////////////

  class t_collection_ref : public t_ref {
  public:
    t_collection_ref(       t_ref);
    t_collection_ref(t_err, t_ref);

    t_ref  operator[](t_ix);
    t_cref operator[](t_ix) const;

    t_ref  operator[](t_string_crange name);
    t_cref operator[](t_string_crange name) const;

    t_n    get_size() const;
    t_bool is_empty() const;
  };

  class t_collection_cref : public t_cref {
  public:
    t_collection_cref(       t_cref);
    t_collection_cref(t_err, t_cref);

    t_cref operator[](t_ix idx) const;
    t_cref operator[](t_string_crange name) const;

    inline
    t_cref operator[](R_name name) const {
      return (*this)[name.get_cstr()];
    }

    t_n    get_size() const;
    t_bool is_empty() const;
  };

////////////////////////////////////////////////////////////////////////////////

  class t_list_ref : public t_collection_ref {
  public:
    t_list_ref(       t_ref);
    t_list_ref(t_err, t_ref);

    bool swap(t_err, t_ix, t_ix);

    t_ref add (t_err, t_string_crange);
    t_ref add (t_err, t_cref);

    t_ref add_simple   (t_err, t_name,                    R_oparams);
    t_ref add_simple   (t_err, t_name, t_value,           R_oparams);
    t_ref add_boolean  (t_err, t_name, t_bool,            R_oparams);
    t_ref add_compound (t_err, t_name, t_values,          R_oparams);
    t_ref add_compound (t_err, t_name, t_values, t_value, R_oparams);
    t_ref add_selection(t_err, t_name, t_values,          R_oparams);
    t_ref add_list     (t_err, t_name,                    R_oparams);
    t_ref add_openlist (t_err, t_name,                    R_oparams);
    t_ref add_option   (t_err, t_name,                    R_oparams);
    t_ref add_array    (t_err, t_name, t_bool,            R_params);
    t_ref add_array    (t_err, t_name, t_values,          R_params);
    t_ref add_lookup   (t_err, t_name,                    R_params);

    t_bool del(R_name);
  };

  class t_list_cref : public t_collection_cref {
  public:
    t_list_cref(t_cref);
    t_list_cref(t_err, t_cref);
  };

////////////////////////////////////////////////////////////////////////////////

  class t_options_ref : public t_collection_ref {
  public:
    t_options_ref(       t_ref);
    t_options_ref(t_err, t_ref);

    R_name get_extension() const; // nothing is allowed to be optional

    t_ref add (t_err, t_string_crange);
    t_ref add (t_err, t_cref);

    t_ref add_simple   (t_err, t_name);
    t_ref add_simple   (t_err, t_name, t_value);
    t_ref add_boolean  (t_err, t_name, t_bool);
    t_ref add_compound (t_err, t_name, t_values);
    t_ref add_compound (t_err, t_name, t_values, t_value);
    t_ref add_selection(t_err, t_name, t_values);
    t_ref add_list     (t_err, t_name);
    t_ref add_openlist (t_err, t_name);
    t_ref add_array    (t_err, t_name, t_bool,   R_rparams);
    t_ref add_array    (t_err, t_name, t_values, R_rparams);
    t_ref add_lookup   (t_err, t_name,           R_rparams);

    t_bool del(R_name);
  };

  class t_options_cref : public t_collection_cref {
  public:
    t_options_cref(       t_cref);
    t_options_cref(t_err, t_cref);

    R_name get_extension() const;
  };

////////////////////////////////////////////////////////////////////////////////

  class t_lookup_ref : public t_collection_ref {
  public:
    t_lookup_ref(       t_ref);
    t_lookup_ref(t_err, t_ref); // nothing is allowed to be optional

    t_range_params get_range_params() const;

    t_cref add_simple   (t_err, t_name);
    t_cref add_boolean  (t_err, t_name);
    t_cref add_compound (t_err, t_name, t_values);
    t_cref add_selection(t_err, t_name, t_values);
    t_cref add_array    (t_err, t_name, R_rparams);
     t_ref add_lookup   (t_err, t_name, R_rparams);

    t_bool set_as_initialized();
    t_bool is_initialized() const;

    t_ref  add_value(t_err, t_name);        // remove t_err from all calls
    t_bool del_value(R_name);

    t_ref  get_value(t_string_crange name);
    t_cref get_value(t_string_crange name) const;

    inline
    t_ref get_value(R_name name) {
      return get_value(name.get_cstr());
    }

    inline
    t_cref get_value(R_name name) const {
      return get_value(name.get_cstr());
    }

    t_void clear_value();
    t_n    size_value() const;

    t_ref   begin_value();
    t_cref  begin_value() const;
    t_cref cbegin_value() const;

    t_ref   next_value(t_ref);
    t_cref  next_value(t_ref) const;
    t_cref cnext_value(t_cref) const;
  };

  class t_lookup_cref : public t_collection_cref {
  public:
    t_lookup_cref(       t_cref);
    t_lookup_cref(t_err, t_cref);

    t_range_params get_range_params() const;

    t_bool is_initialized() const;

    t_cref get_value(t_string_crange name) const;

    inline
    t_cref get_value(R_name name) const {
      return get_value(name.get_cstr());
    }

    t_n     size_value() const;

    t_cref  begin_value() const;
    t_cref cbegin_value() const;

    t_cref  next_value(t_ref) const;
    t_cref cnext_value(t_cref) const;
  };

////////////////////////////////////////////////////////////////////////////////

  class t_lookup_value_ref : public t_collection_ref {
  public:
    t_lookup_value_ref(       t_ref);
    t_lookup_value_ref(t_err, t_ref);
  };

  class t_lookup_value_cref : public t_collection_cref {
  public:
    t_lookup_value_cref(       t_cref);
    t_lookup_value_cref(t_err, t_cref);
  };

////////////////////////////////////////////////////////////////////////////////

  class t_argn {
  public:
    t_argn(t_err);

    t_list_ref   get_root();
    t_list_cref  get_root() const;
    t_list_cref cget_root() const;

    t_ref  operator[](R_fullname);
    t_cref operator[](R_fullname) const;

    t_n    get_size() const;
    t_bool is_empty() const;
    t_void print() const;

    t_ref begin();
    t_ref next(t_ref);

    t_cref begin() const;
    t_cref next(t_cref) const;

    t_cref cbegin() const;
    t_cref cnext(t_cref) const;

  private:
    friend class t_simple_ref;
    friend class t_lookup_ref;
    friend class t_lookup_cref;
    friend class t_list_ref;
    friend class t_list_cref;
    friend class t_options_ref;
    friend class t_options_cref;

    t_ref  add_simple_   (r_err, t_ref, x_name, R_oparams);
    t_ref  add_simple_   (r_err, t_ref, x_name, x_value, R_oparams);
    t_ref  add_boolean_  (r_err, t_ref, x_name, t_bool, R_oparams);
    t_ref  add_compound_ (r_err, t_ref, x_name, x_values, R_oparams);
    t_ref  add_compound_ (r_err, t_ref, x_name, x_values, x_value, R_oparams);
    t_ref  add_selection_(r_err, t_ref, x_name, x_values, R_oparams);
    t_ref  add_list_     (r_err, t_ref, x_name, R_oparams);
    t_ref  add_openlist_ (r_err, t_ref, x_name, R_oparams);
    t_ref  add_option_   (r_err, t_ref, x_name, R_oparams);
    t_ref  add_array_    (r_err, t_ref, x_name, t_bool, R_params);
    t_ref  add_array_    (r_err, t_ref, x_name, x_values, R_params);
    t_ref  add_lookup_   (r_err, t_ref, x_name, R_params);
    t_bool del_          (r_ref, R_name);

    t_ref  add_lookup_value_(r_err, t_ref, x_name);
    t_bool del_lookup_value_(r_ref, R_name);
    t_void clr_lookup_value_(r_ref);
    t_ref  get_lookup_value_(r_ref,  t_string_crange);
    t_cref get_lookup_value_(t_cref, t_string_crange) const;

    t_compound_ref transform_(r_err, r_simple_ref, x_values);

   private:
    t_ref add_(r_err, t_ref, x_name, r_arginfo);
    t_ref add_(r_err, t_ref, x_name, r_arginfo, x_value);
    t_ref add_(r_err, t_ref, x_name, r_arginfo, x_values);

    t_table table_;
  };
  using r_argn = t_prefix<t_argn>::r_;
  using R_argn = t_prefix<t_argn>::R_;

  t_bool operator==(R_argn, R_argn);
  t_bool operator!=(R_argn, R_argn);

  t_bool operator==(t_cref, t_cref);
  t_bool operator!=(t_cref, t_cref);

  t_bool is_same_definition(t_cref, t_cref, t_bool ignore_optional = false);
  t_bool is_same(t_cref, t_cref);

  t_bool append(t_err, t_list_ref, t_cref);
}

////////////////////////////////////////////////////////////////////////////////

  enum  t_text_tag_ {};
  using t_text = t_string<t_text_tag_>; // heap - 0
  using r_text = t_prefix<t_text>::r_;
  using R_text = t_prefix<t_text>::R_;

  t_void parse_notation(t_err err, argn::r_argn argn, R_text);
  t_void build_notation(t_err err, r_text,       argn::R_argn);
  t_void merge_notation(t_err err, argn::r_argn, argn::R_argn);

  //bool parse_cmd  (r_text err_msg, r_argn argn, t_string_crange cmd_p);
  //bool process_cmd(r_text err_msg, r_argn use,  t_string_crange cmd_p);

  /*
  class t_dacli {
  public:
    // add help
    // add test
    // add config
    // add list
    t_dacli(t_err);

    bool add_cli(t_err, argn::t_name cmd_name, argn::t_argn&& cmd_in_args,
                                                argn::t_argn&& cmd_out_args);
    bool del_cli(t_err, const argn::t_name cmd_name);

    bool process_incoming(icomming, callback)
  };
  */
////////////////////////////////////////////////////////////////////////////////
}
}

#endif
