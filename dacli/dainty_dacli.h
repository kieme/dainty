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
#include "dainty_base_string_segmented.h"
#include "dainty_dacli_err.h"

// table should be called dictionary
// set_as_initialized and is_initialized should be renamed
// need to figure out how I want to return the content
//   name is the last segment of the fullname
//   fullname is not build until requested
//   value and values are not the same type. value could return t_string_range
// inline must be changed to constexpr in many cases

/******************************************************************************/

namespace dainty
{
namespace dacli
{
/******************************************************************************/

  using err::t_err;
  using err::r_err;

  using base::types::t_prefix;
  using base::types::t_void;
  using base::types::t_bool;
  using base::types::t_uint32;
  using base::types::t_uint16;
  using base::types::p_void;

  using base::specific::t_ix;
  using base::specific::t_n;
  using base::specific::t_validity;
  using base::specific::VALID;
  using base::specific::INVALID;
  using base::specific::operator"" _ix;
  using base::specific::operator"" _bix;
  using base::specific::operator"" _eix;
  using base::specific::operator"" _n;

  using base::string::t_string;
  using base::string::segmented::t_segmented;
  using base::string::segmented::t_seg_no;
  using base::string::operator""_SL;

/******************************************************************************/

  using t_string_crange = base::string::t_crange;

/******************************************************************************/

  constexpr t_uint32 OPTIONAL = 0x10000000;

  enum t_type : t_uint32 {
    TYPE_Q   = 0x00000000,  // 0. // INVALID TYPE     **
    TYPE_L   = 0x00000001,  // 1. // LIST TYPE        ** name@n=[1,2,3,4]
    TYPE_T   = 0x00000002,  // 2. // TABLE TYPE       ** name@2{a=, b=, ~c}=
    TYPE_Z   = 0x00000004,  // 3. // OPTIONS TYPE     ** prefix_(~opt | value=)
    TYPE_S   = 0x00000008,  // 4. // SIMPLE TYPE      ** name=value
    TYPE_C   = 0x00000010,  // 5. // COMPOUND TYPE    ** name=main:p1:p2
    TYPE_G   = 0x00000020,  // 6. // GROUP TYPE       ** name=()
    TYPE_B   = 0x00000040,  // 7. // BOOLEAN TYPE     ** ~name, name, !name
    TYPE_H   = 0x00000080,  // 8. // SELECTION TYPE   ** name=[1|2]
    TYPE_K   = 0x00000100,  // 9. // OPEN_GROUP TYPE  ** group=(*)
    TYPE_OL  = OPTIONAL | TYPE_L,
    TYPE_OT  = OPTIONAL | TYPE_T,
    TYPE_OZ  = OPTIONAL | TYPE_Z,
    TYPE_OS  = OPTIONAL | TYPE_S,
    TYPE_OC  = OPTIONAL | TYPE_C,
    TYPE_OG  = OPTIONAL | TYPE_G,
    TYPE_OB  = OPTIONAL | TYPE_B,
    TYPE_OH  = OPTIONAL | TYPE_H,
    TYPE_OK  = OPTIONAL | TYPE_K
  };
  using T_type = t_prefix<t_type>::T_;
  using r_type = t_prefix<t_type>::r_;

  constexpr t_bool is_optional  (t_type t) noexcept { return OPTIONAL & t; }
  constexpr t_bool is_simple    (t_type t) noexcept { return TYPE_S   & t; }
  constexpr t_bool is_list      (t_type t) noexcept { return TYPE_L   & t; }
  constexpr t_bool is_table     (t_type t) noexcept { return TYPE_T   & t; }
  constexpr t_bool is_compound  (t_type t) noexcept { return TYPE_C   & t; }
  constexpr t_bool is_options   (t_type t) noexcept { return TYPE_Z   & t; }
  constexpr t_bool is_group     (t_type t) noexcept { return TYPE_G   & t; }
  constexpr t_bool is_open_group(t_type t) noexcept { return TYPE_K   & t; }
  constexpr t_bool is_boolean   (t_type t) noexcept { return TYPE_B   & t; }
  constexpr t_bool is_selection (t_type t) noexcept { return TYPE_H   & t; }

  constexpr t_type get_base_type(t_type type) noexcept {
    return (t_type)(0xFFFF & type);
  }

  constexpr t_type get_path_type(t_type type) noexcept {
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
        return TYPE_B;
      case TYPE_L:
      case TYPE_OL:
        return TYPE_L;
      case TYPE_T:
      case TYPE_OT:
        return TYPE_T;
      case TYPE_Z:
      case TYPE_OZ:
        return TYPE_Z;
      case TYPE_K:
      case TYPE_OK:
      case TYPE_G:
      case TYPE_OG:
        return TYPE_G;
      default:
        break; // assert
    }
    return TYPE_S; // 'S', 'B', 'L', 'X', 'Z'
  }

  constexpr t_string_crange str(t_type argype) noexcept {
    switch (argype) {
      case TYPE_Q:   return "Q"_SL;
      case TYPE_L:   return "L"_SL;
      case TYPE_OL:  return "OL"_SL;
      case TYPE_T:   return "T"_SL;
      case TYPE_OT:  return "OT"_SL;
      case TYPE_Z:   return "Z"_SL;
      case TYPE_OZ:  return "OZ"_SL;
      case TYPE_S:   return "S"_SL;
      case TYPE_OS:  return "OS"_SL;
      case TYPE_C:   return "C"_SL;
      case TYPE_OC:  return "OC"_SL;
      case TYPE_G:   return "G"_SL;
      case TYPE_OG:  return "OG"_SL;
      case TYPE_B:   return "B"_SL;
      case TYPE_OB:  return "OB"_SL;
      case TYPE_H:   return "H"_SL;
      case TYPE_OH:  return "OH"_SL;
      case TYPE_K:   return "K"_SL;
      case TYPE_OK:  return "OK"_SL;
    }
    return t_string_crange{};
  }

  constexpr t_string_crange str(t_bool state) noexcept {
    return state ? "true"_SL : "false"_SL;
  }

/******************************************************************************/

  enum  t_word_tag_ {};
  using t_word     = t_string<t_word_tag_>;
  using r_word     = t_prefix<t_word>::r_;
  using R_word     = t_prefix<t_word>::R_;

  enum  t_words_tag_ { };
  using t_words    = t_segmented<t_words_tag_, 32>;
  using R_words    = t_prefix<t_words>::R_;

  enum  t_path_tag_ {};
  using t_path     = t_string<t_path_tag_, 10>;
  using r_path     = t_prefix<t_path>::r_;
  using R_path     = t_prefix<t_path>::R_;

  enum  t_name_tag_ {};
  using t_name     = t_string<t_name_tag_, 24>;
  using r_name     = t_prefix<t_name>::r_;
  using R_name     = t_prefix<t_name>::R_;

  enum  t_value_tag_ {};
  using t_value    = t_string<t_value_tag_, 24>;
  using r_value    = t_prefix<t_value>::r_;
  using R_value    = t_prefix<t_value>::R_;

  enum  t_fullname_tag_ { };
  using t_fullname = t_segmented<t_fullname_tag_, 32>;
  using R_fullname = t_prefix<t_fullname>::R_;

  enum  t_values_tag_ { };
  using t_values = t_segmented<t_values_tag_, 32>;
  using R_values = t_prefix<t_values>::R_;

  enum  t_optional_tag_ { };
  using t_optional_ = t_bool;
  using t_optional  = t_specific<t_optional_, t_optional_tag_>;

  constexpr t_optional IS_MANDATORY{false};
  constexpr t_optional IS_OPTIONAL {true};

/******************************************************************************/

  t_void print(t_string_crange prefix, R_words);

/******************************************************************************/

  struct t_arg_info_ {
    t_arg_info_(t_type type) noexcept : type_(type) {
      group_.options_ = 0;
    }

    t_arg_info_(t_type type,
                t_n::t_value max,
                t_n::t_value min = 0,
                t_bool init      = false) noexcept : type_(type) {
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
        t_uint16 options_ = 0;
      } group_;
    };
    t_name              ext_;
    std::vector<p_void> mem_; // XXX - placeholder for a freelistid
  };
  using r_arg_info_ = t_prefix<t_arg_info_>::r_;
  using R_arg_info_ = t_prefix<t_arg_info_>::R_;

  struct t_arg_value_ {
    t_arg_value_(R_arg_info_ info) noexcept : info_(info) {
    }

    t_arg_info_ info_;
    t_values    values_;
    t_path      path_; // do I really need path? XXX
  };
  using R_arg_value_ = t_prefix<t_arg_value_>::R_;
  using r_arg_value_ = t_prefix<t_arg_value_>::r_;

////////////////////////////////////////////////////////////////////////////////

  using t_table_ = std::map<t_fullname, t_arg_value_, t_arg_compare_>;
  using t_itr_   = t_table_::iterator;        // impl
  using t_citr_  = t_table_::const_iterator;  // impl
  using t_pair_  = std::pair<t_itr_, t_bool>; // impl
  using t_arg_   = t_table_::value_type;
  using r_arg_   = t_prefix<t_arg_>::r_;
  using R_arg_   = t_prefix<t_arg_>::R_;
  using p_arg_   = t_prefix<t_arg_>::p_;
  using P_arg_   = t_prefix<t_arg_>::P_;
  using x_arg_   = t_prefix<t_arg_>::x_;

////////////////////////////////////////////////////////////////////////////////

  struct t_range_params {
    t_n range_max = 0_n;
    t_n range_min = 0_n;

    constexpr t_range_params() noexcept = default;
    constexpr t_range_params(t_n max, t_n min = 0_n) noexcept
      : range_max(max), range_min(min) {
    }
  };
  using t_range_params = t_prefix<t_range_params>::t_;
  using r_range_params = t_prefix<t_range_params>::r_;
  using R_range_params = t_prefix<t_range_params>::R_;

  constexpr t_bool operator==(R_range_params lh, R_range_params rh) noexcept {
    return lh.range_max_ == rh.range_max_ &&
           lh.range_min_ == rh.range_min_;
  }

  constexpr t_bool operator!=(R_range_params lh, R_range_params rh) noexcept {
    return !(lh == rh);
  }

///////////////////////////////////////////////////////////////////////////////

  struct t_params {
    t_optional     optional;
    t_range_params range_params;

    constexpr t_params() noexcept = default;
    constexpr t_params(t_optional     _optional,
                       R_range_params _range_params) noexcept
      : optional(optional), range_params(range_params) {
    }
  };
  using r_params = t_prefix<t_params>::r_;
  using R_params = t_prefix<t_params>::R_;

  // XXX - naming - can loose the _RANGE
  constexpr t_params UNBOUND_RANGE{};
  constexpr t_params OPTIONAL_UNBOUND_RANGE{true, t_range_params{}};

////////////////////////////////////////////////////////////////////////////////

  class t_argn;
  using p_argn  = t_prefix<t_argn>::p_;
  using P_argn  = t_prefix<t_argn>::P_;
  using r_argn  = t_prefix<t_argn>::r_;
  using R_argn  = t_prefix<t_argn>::R_;
  using x_argn  = t_prefix<t_argn>::x_;

////////////////////////////////////////////////////////////////////////////////

  class t_ref;
  using r_ref = t_prefix<t_ref>::r_;
  using R_ref = t_prefix<t_ref>::R_;

  class t_ref { // add maybe optional_params
    struct t_cid {
      P_argn argn_ = nullptr;
      P_arg_ arg_  = nullptr;
      constexpr t_cid(P_argn argn, P_arg_ arg) noexcept
        : argn_(argn), arg_(arg) {
      }
    };
  public:
    inline t_ref()             noexcept = default;
    inline t_ref(       R_ref) noexcept = default;
    inline t_ref(t_err, R_ref) noexcept;

    inline r_ref operator=(R_ref) noexcept = default;

    t_type            get_type           () const noexcept;
    t_type            get_base_type      () const noexcept;
    R_name            get_name           () const noexcept;
    R_fullname        get_fullname       () const noexcept;
    t_optional_params get_optional_params() const noexcept;

    inline operator t_bool    () const noexcept        { return is_valid_(); }
    inline operator t_validity() const noexcept; // XXX

  protected:
    inline t_ref(p_argn argn, p_arg_ arg) noexcept : argn_(argn), arg_(arg) { }

    inline t_bool is_valid_() const noexcept          { return argn_ && arg_; }
    inline t_void clear_   ()       noexcept { argn_ = nullptr; arg_ = nullptr; }
    inline r_arg_ set_     ()       noexcept                 { return *arg_;  }
    inline R_arg_ get_     () const noexcept                 { return *arg_;  }
    inline r_argn set_argn_()       noexcept                 { return *argn_; }
    inline R_argn get_argn_() const noexcept                 { return *argn_; }

    inline t_ref make_ref_ (p_arg_ arg)       noexcept { return {argn_, arg}; }
    inline t_cid make_id_  (P_arg_ arg) const noexcept { return {argn_, arg}; }

  private:
    friend class t_argn;
    friend class t_cref;
    p_argn argn_ = nullptr;
    p_arg_ arg_  = nullptr;
  };

////////////////////////////////////////////////////////////////////////////////

  class t_cref;
  using r_cref = t_prefix<t_cref>::r_;
  using R_cref = t_prefix<t_cref>::R_;

  class t_cref {
  public:
    inline t_cref()                 noexcept = default;
    inline t_cref(       R_cref)    noexcept = default;
    inline t_cref(t_err, R_cref)    noexcept ;
    inline t_cref(R_ref ref)        noexcept : argn_(ref.argn_), arg_(ref.arg_) { }
    inline t_cref(t_ref::t_cid id)  noexcept : argn_(id.argn_), arg_(id.arg_) { }
    inline r_cref operator=(R_cref) noexcept = default;

    t_type            get_type           () const noexcept;
    t_type            get_base_type      () const noexcept;
    R_name            get_name           () const noexcept;
    R_fullname        get_fullname       () const noexcept;
    t_optional_params get_optional_params() const noexcept;

    inline operator t_bool() const noexcept            { return is_valid_(); }
    inline operator t_validity() const noexcept; // XXX

  protected:
    inline t_cref(P_argn argn, P_arg_ arg) noexcept : argn_(argn), arg_(arg)     { }

    inline t_bool is_valid_() const  noexcept { return argn_ && arg_; }
    inline t_void clear_   ()        noexcept { argn_ = nullptr; arg_ = nullptr; }
    inline R_arg_ get_     () const  noexcept                { return *arg_;  }
    inline R_argn get_argn_() const  noexcept                { return *argn_; }

    inline t_cref make_ref_(P_arg_ arg) const noexcept { return {argn_, arg}; }

  private:
    friend class t_argn;
    P_argn argn_ = nullptr;
    P_arg_ arg_  = nullptr;
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

    // XXX not bool but a text. empty is then ~
    t_bool get_value() const;
    t_bool set_value(t_err, t_bool); // if it is none modifyable?
  };

  class t_boolean_cref : public t_cref {
  public:
    t_boolean_cref(       t_cref);
    t_boolean_cref(t_err, t_cref);

    t_bool get_value() const;
  };

////////////////////////////////////////////////////////////////////////////////

  class t_list_ref : public t_ref {
  public:
    t_list_ref(       t_ref);
    t_list_ref(t_err, t_ref);

    t_range_params get_range_params() const;

    t_bool set_as_initialized(); // initial as empty XXX- read from arg_info
    t_bool is_initialized() const;

    R_values get_values() const;
    t_bool   set_values(t_err, t_values);
  };

  class t_list_cref : public t_cref {
  public:
    t_list_cref(t_cref);
    t_list_cref(t_err, t_cref);

    t_range_params get_range_params() const;

    t_bool   is_initialized() const;
    R_values get_values() const;
  };

////////////////////////////////////////////////////////////////////////////////

  // check if 
  class t_compound_ref : public t_ref {
  public:
    t_compound_ref(       t_ref) noexcept;
    t_compound_ref(t_err, t_ref) noexcept;

    t_bool add_missing_part_values(t_err, R_values) noexcept;

    R_values get_values() const noexcept;
    R_value  get_value() const;
  };

  class t_compound_cref : public t_cref {
  public:
    t_compound_cref(       t_cref) noexcept;
    t_compound_cref(t_err, t_cref) noexcept;

    R_values get_values() const noexcept;
  };

////////////////////////////////////////////////////////////////////////////////

  class t_selection_ref : public t_ref {
  public:
    t_selection_ref(       t_ref) noexcept;
    t_selection_ref(t_err, t_ref) noexcept;

    // default
    R_values get_values()               const noexcept;
    t_bool   test_value(t_err, t_value) const noexcept;
  };

  class t_selection_cref : public t_cref {
  public:
    t_selection_cref(       t_cref) noexcept;
    t_selection_cref(t_err, t_cref) noexcept;

    // default

    R_values get_values()               const noexcept;
    t_bool   test_value(t_err, t_value) const noexcept;
  };

////////////////////////////////////////////////////////////////////////////////

  class t_open_group_ref : public t_ref {
  public:
    t_open_group_ref(       t_ref) noexcept;
    t_open_group_ref(t_err, t_ref) noexcept;
  };

  class t_open_group_cref : public t_cref {
  public:
    t_open_group_cref(       t_cref) noexcept;
    t_open_group_cref(t_err, t_cref) noexcept;
  };

////////////////////////////////////////////////////////////////////////////////

  class t_collection_ref : public t_ref {
  public:
    t_collection_ref(       t_ref) noexcept;
    t_collection_ref(t_err, t_ref) noexcept;

    t_ref  operator[](t_ix) noexcept;
    t_cref operator[](t_ix) const noexcept;

    t_ref  operator[](t_string_crange name) noexcept;
    t_cref operator[](t_string_crange name) const noexcept;

    t_n    get_size() const noexcept;
    t_bool is_empty() const noexcept;
  };

  class t_collection_cref : public t_cref {
  public:
    t_collection_cref(       t_cref) noexcept;
    t_collection_cref(t_err, t_cref) noexcept;

    t_cref operator[](t_ix idx)             const noexcept;
    t_cref operator[](t_string_crange name) const noexcept;

    inline
    t_cref operator[](R_name name) const noexcept {
      return (*this)[name.get_cstr()];
    }

    t_n    get_size() const noexcept;
    t_bool is_empty() const noexcept;
  };

////////////////////////////////////////////////////////////////////////////////

  class t_group_ref : public t_collection_ref {
  public:
    t_group_ref(       t_ref);
    t_group_ref(t_err, t_ref);

    t_bool swap(t_err, t_ix, t_ix);

    t_ref add (t_err, t_string_crange);
    t_ref add (t_err, t_cref);

    t_simple_ref  add_simple(t_err, t_name,                    t_optional);
    t_simple_ref  add_simple(t_err, t_name, t_value,           t_optional);

    t_boolean_ref add_boolean(t_err, t_name,                    t_optional);
    t_boolean_ref add_boolean(t_err, t_name, t_bool,       t_optional);

    t_compound_ref add_compound(t_err, t_name, t_values,   t_optional);

    t_selection_ref  add_selection (t_err, t_name, t_values,  t_optional);
    t_group_ref      add_group     (t_err, t_name,            t_optional);
    t_open_group_ref add_open_group(t_err, t_name,            t_optional);

    t_list_ref       add_list      (t_err, t_name, t_init,    R_params);
    t_list_ref       add_list      (t_err, t_name, t_values,  R_params);

    t_table_ref   add_table    (t_err, t_name,                    R_params);
    t_options_ref add_options  (t_err, t_name,                    t_optional);

    t_bool del(R_name);
  };

  class t_group_cref : public t_collection_cref {
  public:
    t_group_cref(t_cref);
    t_group_cref(t_err, t_cref);
  };

////////////////////////////////////////////////////////////////////////////////

  class t_table_ref : public t_collection_ref {
  public:
    t_table_ref(       t_ref);
    t_table_ref(t_err, t_ref); // nothing is allowed to be optional

    t_range_params get_range_params() const;

    t_cref add_simple   (t_err, t_name);
    t_cref add_boolean  (t_err, t_name);
    t_cref add_compound (t_err, t_name, t_values);
    t_cref add_selection(t_err, t_name, t_values);
    t_cref add_list     (t_err, t_name, R_range_params);
     t_ref add_table   (t_err, t_name, R_range_params);

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

  class t_table_cref : public t_collection_cref {
  public:
    t_table_cref(       t_cref);
    t_table_cref(t_err, t_cref);

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

  class t_table_value_ref : public t_collection_ref {
  public:
    t_table_value_ref(       t_ref);
    t_table_value_ref(t_err, t_ref);
  };

  class t_table_value_cref : public t_collection_cref {
  public:
    t_table_value_cref(       t_cref);
    t_table__value_cref(t_err, t_cref);
  };

////////////////////////////////////////////////////////////////////////////////

  class t_options_ref : public t_collection_ref {
  public:
    t_options_ref(       t_ref);
    t_options_ref(t_err, t_ref);

    R_name get_extension() const; // nothing is allowed to be optional

    t_ref add (t_err, t_string_crange);
    t_ref add (t_err, t_cref);

    t_ref add_simple    (t_err, t_name);
    t_ref add_simple    (t_err, t_name, t_value);
    t_ref add_boolean   (t_err, t_name, t_bool);
    t_ref add_compound  (t_err, t_name, t_values);
    t_ref add_compound  (t_err, t_name, t_values, t_value);
    t_ref add_selection (t_err, t_name, t_values);
    t_ref add_group     (t_err, t_name);
    t_ref add_open_group(t_err, t_name);
    t_ref add_list      (t_err, t_name, t_bool,   R_range_params);
    t_ref add_list      (t_err, t_name, t_values, R_range_params);
    t_ref add_table     (t_err, t_name,           R_range_params);

    t_bool del(R_name);
  };

  class t_options_cref : public t_collection_cref {
  public:
    t_options_cref(       t_cref);
    t_options_cref(t_err, t_cref);

    R_name get_extension() const;
  };

////////////////////////////////////////////////////////////////////////////////

  class t_argn {
  public:
    t_argn(t_err);

    t_group_ref   get_root();
    t_group_cref  get_root() const;
    t_group_cref cget_root() const;

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
    friend class t_table_ref;
    friend class t_table_cref;
    friend class t_group_ref;
    friend class t_group_cref;
    friend class t_options_ref;
    friend class t_options_cref;

    t_ref  add_simple_    (r_err, t_ref, x_name, t_optional);
    t_ref  add_simple_    (r_err, t_ref, x_name, x_value, t_optional);
    t_ref  add_boolean_   (r_err, t_ref, x_name, t_bool, t_optional);
    t_ref  add_compound_  (r_err, t_ref, x_name, x_values, t_optional);
    t_ref  add_compound_  (r_err, t_ref, x_name, x_values, x_value, t_optional);
    t_ref  add_selection_ (r_err, t_ref, x_name, x_values, t_optional);
    t_ref  add_group_     (r_err, t_ref, x_name, t_optional);
    t_ref  add_open_group_(r_err, t_ref, x_name, t_optional);
    t_ref  add_options_   (r_err, t_ref, x_name, t_optional);
    t_ref  add_list_      (r_err, t_ref, x_name, t_bool, R_params);
    t_ref  add_list_      (r_err, t_ref, x_name, x_values, R_params);
    t_ref  add_table_    (r_err, t_ref, x_name, R_params);
    t_bool del_           (r_ref, R_name);

    t_ref  add_table_value_(r_err, t_ref, x_name);
    t_bool del_table_value_(r_ref, R_name);
    t_void clr_table_value_(r_ref);
    t_ref  get_table_value_(r_ref,  t_string_crange);
    t_cref get_table_value_(t_cref, t_string_crange) const;

    t_compound_ref transform_(r_err, r_simple_ref, x_values);

   private:
    t_ref add_(r_err, t_ref, x_name, r_arg_info_);
    t_ref add_(r_err, t_ref, x_name, r_arg_info_, x_value);
    t_ref add_(r_err, t_ref, x_name, r_arg_info_, x_values);

    t_table_ table_;
  };
  using r_argn = t_prefix<t_argn>::r_;
  using R_argn = t_prefix<t_argn>::R_;

  t_bool operator==(R_argn, R_argn);
  t_bool operator!=(R_argn, R_argn);

  t_bool operator==(t_cref, t_cref);
  t_bool operator!=(t_cref, t_cref);

  t_bool is_same_definition(t_cref, t_cref, t_bool ignore_optional = false);
  t_bool is_same(t_cref, t_cref);

  t_bool append(t_err, t_group_ref, t_cref);

////////////////////////////////////////////////////////////////////////////////

  enum  t_text_tag_ {};
  using t_text = t_string<t_text_tag_>; // heap - 0
  using r_text = t_prefix<t_text>::r_;
  using R_text = t_prefix<t_text>::R_;

  t_void parse_notation(t_err, r_argn, R_text);
  t_void build_notation(t_err, r_text, R_argn);
  t_void merge_notation(t_err, r_argn, R_argn);

  //bool parse_cmd  (r_text err_msg, r_argn argn, t_string_crange cmd_p);
  //bool process_cmd(r_text err_msg, r_argn use,  t_string_crange cmd_p);

  /*
  class t_dacli {
  public:
    // add help
    // add test
    // add config
    // add group
    t_dacli(t_err);

    bool add_cli(t_err, t_name cmd_name, t_argn&& cmd_in_args,
                                                t_argn&& cmd_out_args);
    bool del_cli(t_err, const t_name cmd_name);

    bool process_incoming(icomming, callback)
  };
  */
////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////
}
}

#endif
