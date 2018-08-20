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
#include <string>
#include <algorithm>
#include <vector>
#include <map>

#include "dainty_named.h"
#include "dainty_oops.h"

/******************************************************************************/

namespace dainty
{
namespace dacli
{
  using named::p_cstr;
  using named::t_void;
  using named::t_bool;
  using named::mk_cstr;

/******************************************************************************/

  const std::string TRUE_s ("<true>");
  const std::string FALSE_s("<false>");
  const std::string EMPTY_s("");

/******************************************************************************/

  struct t_err {
    t_err() : position_(nullptr) { }

    operator t_bool () const { return reason_.length(); }
    t_err set(named::p_cstr_ p, p_cstr reason);
    t_err set(p_cstr reason);

    std::string    reason_;
    named::p_cstr_ position_;
  };

/******************************************************************************/

  namespace argn
  {
    using named::t_ix;
    using named::t_n;

    using named::operator"" _ix;
    using named::operator"" _bix;
    using named::operator"" _eix;
    using named::operator"" _n;

    constexpr named::t_uint32 v_optional = 0x10000000;

    enum t_type : named::t_uint32 {
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
      TYPE_OA  = v_optional | TYPE_A,
      TYPE_OX  = v_optional | TYPE_X,
      TYPE_OZ  = v_optional | TYPE_Z,
      TYPE_OS  = v_optional | TYPE_S,
      TYPE_OC  = v_optional | TYPE_C,
      TYPE_OL  = v_optional | TYPE_L,
      TYPE_OB  = v_optional | TYPE_MB, // looks wrong - XXX
      TYPE_OH  = v_optional | TYPE_H,
      TYPE_OK  = v_optional | TYPE_K
    };

    constexpr t_bool is_optional (t_type type) { return v_optional & type; }
    constexpr t_bool is_simple   (t_type type) { return TYPE_S & type;     }
    constexpr t_bool is_array    (t_type type) { return TYPE_A & type;     }
    constexpr t_bool is_lookup   (t_type type) { return TYPE_X & type;     }
    constexpr t_bool is_compound (t_type type) { return TYPE_C & type;     }
    constexpr t_bool is_option   (t_type type) { return TYPE_Z & type;     }
    constexpr t_bool is_list     (t_type type) { return TYPE_L & type;     }
    constexpr t_bool is_boolean  (t_type type) { return TYPE_MB & type;    }
    constexpr t_bool is_selection(t_type type) { return TYPE_H & type;     }

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

    constexpr p_cstr c_str(t_type argype) {
      p_cstr::t_value str = "undefined";
      switch (argype) {
        case TYPE_Q:   str = "Q";
        case TYPE_A:   str = "A";
        case TYPE_OA:  str = "OA";
        case TYPE_X:   str = "X";
        case TYPE_OX:  str = "OX";
        case TYPE_XI:  str = "IX";
        case TYPE_Z:   str = "Z";
        case TYPE_OZ:  str = "OZ";
        case TYPE_S:   str = "S";
        case TYPE_OS:  str = "OS";
        case TYPE_C:   str = "C";
        case TYPE_OC:  str = "OC";
        case TYPE_L:   str = "L";
        case TYPE_OL:  str = "OL";
        case TYPE_B:   str = "B";
        case TYPE_OB:  str = "OB";
        case TYPE_MB:  str = "MB";
        case TYPE_H:   str = "H";
        case TYPE_OH:  str = "OH";
        case TYPE_K:   str = "K";
        case TYPE_OK:  str = "OK";
      }
      return named::mk_cstr(str);
    }

    inline const std::string& str(t_bool state) {
      return state ? TRUE_s : FALSE_s;
    }

/******************************************************************************/

    using t_word     = std::string;
    using t_words    = std::vector<t_word>;
    using t_name     = t_word;
    using t_fullname = t_words;
    using t_value    = t_word;
    using t_values   = t_words;
    using t_path     = t_words;

    static const t_word     empty_word_v;
    static const t_words    empty_words_v;
    static const t_value    empty_value_v;
    static const t_values   empty_values_v;
    static const t_fullname empty_fullname_v;
    static const t_name     empty_name_v;

    t_void print(p_cstr prefix, const t_words& words);

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
          named::t_uint16 options_ = 0;
        } list_;
      };
      t_name                     ext_;
      std::vector<named::p_void> mem_; // XXX - placeholder for a freelistid
    };

    struct t_argvalue {
      t_argvalue(const t_arginfo& info) : info_(info) { }

      t_arginfo info_;
      t_values     values_;
      t_path       path_;
    };

    struct arg_compare {
      t_bool operator()(const t_fullname& lh, const t_fullname& rh) const;
    };

////////////////////////////////////////////////////////////////////////////////

    using t_table   = std::map<t_fullname, t_argvalue, arg_compare>;
    using t_itr     = t_table::iterator;
    using t_citr    = t_table::const_iterator;
    using t_range   = std::pair<t_itr, t_itr>;
    using t_crange  = std::pair<t_citr, t_citr>;
    using t_arg     = t_table::value_type;
    using t_pair    = std::pair<t_itr, bool>;
    using p_arg     = t_arg*;
    using p_carg    = const t_arg*;

////////////////////////////////////////////////////////////////////////////////

    struct t_optional_params {
      t_bool optional_;
      t_optional_params(t_bool optional = false) : optional_(optional) { }
    };
    using t_oparams = t_optional_params;

    static const t_oparams optional  {true};
    static const t_oparams mandatory {false};

    struct t_range_params {
      t_n range_max_;
      t_n range_min_;
      t_range_params(t_n max = 0_n, t_n min = 0_n)
        : range_max_(max), range_min_(min) { }
    };
    using t_rparams = t_range_params;

    struct t_params : t_oparams, t_rparams {
      t_params() = default;
      t_params(const t_oparams& oparams, const t_rparams& rparams)
        : t_oparams(oparams), t_rparams(rparams) { }
    };

    static const t_params unbound_range{};
    static const t_params optional_unbound_range{t_oparams{true}, t_rparams{}};

    inline bool operator==(const t_rparams& lh, const t_rparams& rh) {
      return lh.range_max_ == rh.range_max_ &&
             lh.range_min_ == rh.range_min_;
    }

    inline bool operator!=(const t_rparams& lh, const t_rparams& rh) {
      return !(lh == rh);
    }

////////////////////////////////////////////////////////////////////////////////

    class t_argn;
    using p_argn  = t_argn*;
    using p_cargn = const t_argn*;

    class t_ref { // add maybe optional_params
      struct t_cid {
        p_cargn argn_ = nullptr;
        p_carg  arg_  = nullptr;
        t_cid(p_cargn argn, p_carg arg) : argn_(argn), arg_(arg) {}
      };
    public:
      inline t_ref() = default;
      inline t_ref(const t_ref&) = default;
      inline t_ref(t_err, const t_ref&);

      inline t_ref& operator=(const t_ref&) = default;

      t_type            get_type() const;
      t_type            get_base_type() const;
      const t_name&     get_name() const;
      const t_fullname& get_fullname() const;
      t_optional_params get_optional_params() const;

      inline operator t_bool() const                 { return is_valid_(); }

    protected:
      inline t_ref(p_argn argn, p_arg arg)  : argn_(argn), arg_(arg)     { }

      inline       t_bool  is_valid_() const       { return argn_ && arg_; }
      inline       t_void  clear_()     { argn_ = nullptr; arg_ = nullptr; }
      inline       t_arg&  set_()                         { return *arg_;  }
      inline const t_arg&  get_() const                   { return *arg_;  }
      inline       t_argn& set_argn_()                    { return *argn_; }
      inline const t_argn& get_argn_() const              { return *argn_; }

      inline t_ref make_ref_(p_arg arg)            { return {argn_, arg}; }
      inline t_cid make_id_(p_carg arg) const      { return {argn_, arg}; }

    private:
      friend class t_argn;
      friend class t_cref;
      p_argn argn_ = nullptr;
      p_arg  arg_  = nullptr;
    };

    class t_cref {
    public:
      inline t_cref() = default;
      inline t_cref(const t_cref&) = default;
      inline t_cref(const t_ref& ref) : argn_(ref.argn_), arg_(ref.arg_) { }
      inline t_cref(t_ref::t_cid id)  : argn_(id.argn_), arg_(id.arg_)   { }
      inline t_cref(t_err, const t_cref&);
      inline t_cref& operator=(const t_cref&) = default;

      t_type            get_type() const;
      t_type            get_base_type() const;
      const t_name&     get_name() const;
      const t_fullname& get_fullname() const;
      t_optional_params get_optional_params() const;

      inline operator t_bool() const                 { return is_valid_(); }

    protected:
      inline t_cref(p_cargn argn, p_carg arg) : argn_(argn), arg_(arg)   { }

      inline       t_bool  is_valid_() const       { return argn_ && arg_; }
      inline       t_void  clear_()     { argn_ = nullptr; arg_ = nullptr; }
      inline const t_arg&  get_() const                   { return *arg_;  }
      inline const t_argn& get_argn_() const              { return *argn_; }

      inline t_cref make_ref_(p_carg arg) const     { return {argn_, arg}; }

    private:
      friend class t_argn;
      p_cargn argn_ = nullptr;
      p_carg  arg_  = nullptr;
    };

////////////////////////////////////////////////////////////////////////////////

    class t_compound_ref;

    class t_simple_ref : public t_ref {
    public:
      t_simple_ref(t_ref);
      t_simple_ref(t_err, t_ref);

      t_bool transform_to(t_err, t_compound_ref&, t_values values);

      const t_value& get_value() const;
            t_bool   set_value(t_err, t_value);
    };

    class t_simple_cref : public t_cref {
    public:
      t_simple_cref(t_cref);
      t_simple_cref(t_err, t_cref);

      const t_value& get_value() const;
    };

////////////////////////////////////////////////////////////////////////////////

    class t_boolean_ref : public t_ref {
    public:
      t_boolean_ref(t_ref);
      t_boolean_ref(t_err, t_ref);

      t_bool get_value() const;
      t_bool set_value(t_err, bool); // if it is none modifyable?
    };

    class t_boolean_cref : public t_cref {
    public:
      t_boolean_cref(t_cref);
      t_boolean_cref(t_err, t_cref);

      t_bool get_value() const;
    };

////////////////////////////////////////////////////////////////////////////////

    class t_array_ref : public t_ref {
    public:
      t_array_ref(t_ref);
      t_array_ref(t_err, t_ref);

      t_range_params get_range_params() const;

      t_bool set_as_initialized();
      t_bool is_initialized() const;

      const t_values& get_values() const;
            t_bool    set_values(t_err, t_values);
    };

    class t_array_cref : public t_cref {
    public:
      t_array_cref(t_cref);
      t_array_cref(t_err, t_cref);

      t_range_params get_range_params() const;

            t_bool    is_initialized() const;
      const t_values& get_values() const;
    };

////////////////////////////////////////////////////////////////////////////////

    class t_compound_ref : public t_ref {
    public:
      t_compound_ref(t_ref);
      t_compound_ref(t_err, t_ref);

      t_bool add_missing_part_values(t_err, const t_values&);

      const t_values& get_part_values() const;
      const t_value&  get_value() const;
            t_bool    set_value(t_err, t_value);
    };

    class t_compound_cref : public t_cref {
    public:
      t_compound_cref(t_cref);
      t_compound_cref(t_err, t_cref);

      const t_values& get_part_values() const;
      const t_value&  get_value() const;
    };

////////////////////////////////////////////////////////////////////////////////

    class t_selection_ref : public t_ref {
    public:
      t_selection_ref(t_ref);
      t_selection_ref(t_err, t_ref);

      const t_values& get_values() const;
      const t_value&  get_value()  const;
            t_bool    set_value (t_err, t_value);
            t_bool    test_value(t_err, t_value) const;
    };

    class t_selection_cref : public t_cref {
    public:
      t_selection_cref(t_cref);
      t_selection_cref(t_err, t_cref);

      const t_values& get_values() const;
      const t_value&  get_value() const;
            t_bool    test_value(t_err, t_value) const;
    };

////////////////////////////////////////////////////////////////////////////////

    class t_openlist_ref : public t_ref {
    public:
      t_openlist_ref(t_ref);
      t_openlist_ref(t_err, t_ref);
    };

    class t_openlist_cref : public t_cref {
    public:
      t_openlist_cref(t_cref);
      t_openlist_cref(t_err, t_cref);
    };

////////////////////////////////////////////////////////////////////////////////

    class t_collection_ref : public t_ref {
    public:
      t_collection_ref(t_ref);
      t_collection_ref(t_err, t_ref);

      t_ref  operator[](t_ix);
      t_cref operator[](t_ix) const;

      t_ref  operator[](p_cstr name);
      t_cref operator[](p_cstr name) const;

      inline
      t_ref  operator[](const t_name& name) {
        return (*this)[mk_cstr(name.c_str())];
      }

      inline
      t_cref operator[](const t_name& name) const {
        return (*this)[mk_cstr(name.c_str())];
      }

      t_n    get_size() const;
      t_bool is_empty() const;
    };

    class t_collection_cref : public t_cref {
    public:
      t_collection_cref(t_cref);
      t_collection_cref(t_err, t_cref);

      t_cref operator[](t_ix idx) const;
      t_cref operator[](p_cstr name) const;

      inline
      t_cref operator[](const t_name& name) const {
        return (*this)[mk_cstr(name.c_str())];
      }

      t_n    get_size() const;
      t_bool is_empty() const;
    };

////////////////////////////////////////////////////////////////////////////////

    class t_list_ref : public t_collection_ref {
    public:
      t_list_ref(t_ref);
      t_list_ref(t_err, t_ref);

      bool swap(t_err, t_ix, t_ix);

      t_ref add (t_err, p_cstr);
      t_ref add (t_err, t_cref);

      t_ref add_simple    (t_err, t_name,                    const t_oparams&);
      t_ref add_simple    (t_err, t_name, t_value,           const t_oparams&);
      t_ref add_boolean   (t_err, t_name, bool,              const t_oparams&);
      t_ref add_compound  (t_err, t_name, t_values,          const t_oparams&);
      t_ref add_compound  (t_err, t_name, t_values, t_value, const t_oparams&);
      t_ref add_selection (t_err, t_name, t_values,          const t_oparams&);
      t_ref add_list      (t_err, t_name,                    const t_oparams&);
      t_ref add_openlist  (t_err, t_name,                    const t_oparams&);
      t_ref add_option    (t_err, t_name,                    const t_oparams&);
      t_ref add_array     (t_err, t_name, bool,              const t_params&);
      t_ref add_array     (t_err, t_name, t_values,          const t_params&);
      t_ref add_lookup    (t_err, t_name,                    const t_params&);

      bool del(const t_name&);
    };

    class t_list_cref : public t_collection_cref {
    public:
      t_list_cref(t_cref);
      t_list_cref(t_err, t_cref);
    };

////////////////////////////////////////////////////////////////////////////////

    class t_options_ref : public t_collection_ref {
    public:
      t_options_ref(t_ref);
      t_options_ref(t_err, t_ref);

      const t_name& get_extension() const; // nothing is allowed to be optional

      t_ref add (t_err, p_cstr);
      t_ref add (t_err, t_cref);

      t_ref add_simple   (t_err, t_name);
      t_ref add_simple   (t_err, t_name, t_value);
      t_ref add_boolean  (t_err, t_name, bool);
      t_ref add_compound (t_err, t_name, t_values);
      t_ref add_compound (t_err, t_name, t_values, t_value);
      t_ref add_selection(t_err, t_name, t_values);
      t_ref add_list     (t_err, t_name);
      t_ref add_openlist (t_err, t_name);
      t_ref add_array    (t_err, t_name, bool,     const t_rparams&);
      t_ref add_array    (t_err, t_name, t_values, const t_rparams&);
      t_ref add_lookup   (t_err, t_name,           const t_rparams&);

      bool del(const t_name&);
    };

    class t_options_cref : public t_collection_cref {
    public:
      t_options_cref(t_cref);
      t_options_cref(t_err, t_cref);
      const t_name& get_extension() const;
    };

////////////////////////////////////////////////////////////////////////////////

    class t_lookup_ref : public t_collection_ref {
    public:
      t_lookup_ref(t_ref);
      t_lookup_ref(t_err, t_ref); // nothing is allowed to be optional

      t_range_params get_range_params() const;

      t_cref add_simple   (t_err, t_name);
      t_cref add_boolean  (t_err, t_name);
      t_cref add_compound (t_err, t_name, t_values);
      t_cref add_selection(t_err, t_name, t_values);
      t_cref add_array    (t_err, t_name, const t_rparams&);
       t_ref add_lookup   (t_err, t_name, const t_rparams&);

      t_bool set_as_initialized();
      t_bool is_initialized() const;

      t_ref  add_value(t_err, t_name);        // remove t_err from all calls
      t_bool del_value(const t_name&);

      t_ref  get_value(p_cstr name);
      t_cref get_value(p_cstr name) const;

      inline
      t_ref get_value(const t_name& name) {
        return get_value(mk_cstr(name.c_str()));
      }

      inline
      t_cref get_value(const t_name& name) const {
        return get_value(mk_cstr(name.c_str()));
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
      t_lookup_cref(t_cref);
      t_lookup_cref(t_err, t_cref);

      t_range_params get_range_params() const;

      t_bool is_initialized() const;

      t_cref get_value(p_cstr name) const;

      inline
      t_cref get_value(const t_name& name) const {
        return get_value(mk_cstr(name.c_str()));
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
      t_lookup_value_ref(t_ref);
      t_lookup_value_ref(t_err, t_ref);
    };

    class t_lookup_value_cref : public t_collection_cref {
    public:
      t_lookup_value_cref(t_cref);
      t_lookup_value_cref(t_err, t_cref);
    };

////////////////////////////////////////////////////////////////////////////////

    class t_argn {
    public:
      t_argn(t_err);

      t_list_ref   get_root();
      t_list_cref  get_root() const;
      t_list_cref cget_root() const;

      t_ref  operator[](const t_fullname&);
      t_cref operator[](const t_fullname&) const;

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

      t_ref  add_simple_   (t_err, t_ref, t_name&&, const t_oparams&);
      t_ref  add_simple_   (t_err, t_ref, t_name&&, t_value&&,
                           const t_oparams&);
      t_ref  add_boolean_  (t_err, t_ref, t_name&&, bool, const t_oparams&);
      t_ref  add_compound_ (t_err, t_ref, t_name&&, t_values&&,
                           const t_oparams&);
      t_ref  add_compound_ (t_err, t_ref, t_name&&, t_values&&, t_value&&,
                           const t_oparams&);
      t_ref  add_selection_(t_err, t_ref, t_name&&, t_values&&,
                           const t_oparams&);
      t_ref  add_list_     (t_err, t_ref, t_name&&, const t_oparams&);
      t_ref  add_openlist_ (t_err, t_ref, t_name&&, const t_oparams&);
      t_ref  add_option_   (t_err, t_ref, t_name&&, const t_oparams&);
      t_ref  add_array_    (t_err, t_ref, t_name&&, bool, const t_params&);
      t_ref  add_array_    (t_err, t_ref, t_name&&, t_values&&,
                           const t_params&);
      t_ref  add_lookup_   (t_err, t_ref, t_name&&, const t_params&);
      t_bool del_          (t_ref, const t_name&);

      t_ref  add_lookup_value_(t_err, t_ref, t_name&&);
      t_bool del_lookup_value_(t_ref,  const t_name&);
      t_void clr_lookup_value_(t_ref);
      t_ref  get_lookup_value_(t_ref,  p_cstr);
      t_cref get_lookup_value_(t_cref, p_cstr) const;

      t_compound_ref transform_(t_err, t_simple_ref&, t_values&&);

     private:
      t_ref add_(t_err, t_ref, t_name&&, t_arginfo&);
      t_ref add_(t_err, t_ref, t_name&&, t_arginfo&, t_value&&);
      t_ref add_(t_err, t_ref, t_name&&, t_arginfo&, t_values&&);

      t_table table_;
    };

    t_bool operator==(const t_argn&, const t_argn&);
    t_bool operator!=(const t_argn&, const t_argn&);

    t_bool operator==(t_cref, t_cref);
    t_bool operator!=(t_cref, t_cref);

    t_bool is_same_definition(t_cref, t_cref, t_bool ignore_optional = false);
    t_bool is_same(t_cref, t_cref);

    t_bool append(t_err, t_list_ref, t_cref);
  }

////////////////////////////////////////////////////////////////////////////////

  using t_text = std::string;

  t_bool parse_syntax(t_err err, argn::t_argn& argn, p_cstr text);
  t_bool build_syntax(t_err err, t_text&,       const argn::t_argn&);
  t_bool merge_syntax(t_err err, argn::t_argn&, const argn::t_argn&);

  //bool parse_cmd  (t_text& err_msg, t_argn& argn, p_cstr cmd_p);
  //bool process_cmd(t_text& err_msg, t_argn& use,  p_cstr cmd_p);

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
