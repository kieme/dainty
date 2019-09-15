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

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cstring>

#include "dainty_dacli.h"

// 1. have a defined argument notation as to define arguments
// 2. define the parsing behaviour of arguments
//
// ------------------------------------------------------------------
// the checking functions are required
// 2. X/A return params and not range_params
// 3. error messages must be made more useful
// 4. write test scrips
// 5. make it look like dacli commands
// 6. rules - e.g. table take no values in its definition, no list or select list
// 7. <>, '', or without?
// 8. spaces or noz spaces between arguments
//
// -------
//
// '(:cmd@n=[list], :input=(*), :output=(*), :settings=(A))'
//

namespace dainty
{
namespace dacli
{
////////////////////////////////////////////////////////////////////////////////

  using namespace base;
  using namespace base::types;
  using namespace base::range;
  using namespace base::string;

////////////////////////////////////////////////////////////////////////////////

  using err::r_err;

  using std::begin;
  using std::cbegin;
  using std::end;
  using std::cend;

  enum t_compare_ { }; // XXX

  const t_fullname EMPTY_FULLNAME;
  const t_name     EMPTY_NAME;
  const t_value    EMPTY_VALUE;
  const t_values   EMPTY_VALUES;

////////////////////////////////////////////////////////////////////////////////

namespace notation
{
namespace parse
{
  template<typename P>
  P_cstr_ parse_args(t_err, P, t_char delimit, P_cstr_);

  // >> skip_spaces()
  constexpr P_cstr_ strip_spaces(P_cstr_ p) noexcept {
    for (; *p == ' '; ++p);
    return p;
  }

  constexpr P_cstr_ find(P_cstr_ p, t_char C) noexcept {
    for (auto b = p; *p; ++p)
      if (*p == C && (b == p || p[-1] != '/')) // escape character
        break;
    return p;
  }

  constexpr P_cstr_ find_end_of_word(P_cstr_ p) noexcept {
    for (; *p && *p != ',' &&
                 *p != '=' &&
                 *p != '(' &&
                 *p != ')' &&
                 *p != ':' &&
                 *p != '[' &&
                 *p != ']' &&
                 *p != ' ' &&
                 *p != '|' &&
                 *p != '@' &&
                 *p != '{' &&
                 *p != '}' &&
                 *p != '>' &&
                 *p != '<'; ++p);
    return p;
  }

  P_cstr_ parse_word(r_err err, r_word word, P_cstr_ p) {
    ERR_GUARD(err) {
      P_cstr_ b   = p;
      P_cstr_ end = 0;
      if (*b == '<') {
        end = find(++b, '>');
        if (*end == '>')
          p = end + 1;
        else
          err = err::E_PARSE;
      } else
        p = end = find_end_of_word(b);
      if (!err && end - b) {
        word << '<';
        ///word.append(b, end - b); - XXX
        word << '>';
        p = strip_spaces(p);
      }
    }
    return p;
  }

  t_n to_num(P_cstr_ start, t_n_ num) {
    std::string tmp;
    tmp.assign(start, num);
    return t_n(std::stoul(tmp)); // will narrow
  }

  P_cstr_ parse_bound(r_err err, r_rparams params, P_cstr_ p) {
    ERR_GUARD(err) {
      if (*p == '@') {
        p = strip_spaces(p+1);
        if (*p == 'n') {
          params.range_max_ = params.range_min_ = 0_n;
          p = strip_spaces(p+1);
        } else {
          t_n min = 0_n, max = 0_n;
          t_bool stop = false, is_range = false;
          while (!err && !stop) {
            P_cstr_ b = p;
            for (; *p >= 48 && *p <= 57; ++p);
            switch (*p) {
              case '-':
                is_range = true;
                min = to_num(b, p - b);
                p = strip_spaces(p+1);
                if (*p == 'n') {
                  stop = true;
                  p = strip_spaces(p+1);
                  if (*p != '=' && *p != '{')
                    err = err::E_PARSE;
                }
                break;
              case '=':
              case '{':
                if (p - b)
                   max = to_num(b, p - b);
                else
                  err = err::E_PARSE;
                stop = true;
                break;
              default: err = err::E_PARSE;
                break;
            }
          }
          params.range_max_ = max;
          params.range_min_ = is_range ? min : max;
        }
      } else
        err = err::E_INTERNAL;
    }
    return p;
  }

  P_cstr_ parse_name_and_type(r_err err, r_name name, r_type type,
                              r_params params, P_cstr_ p) {
    ERR_GUARD(err) {
      t_bool modify = false;
      if (*p == '!') {
        modify = true;
        p = strip_spaces(p+1);
      } else if (*p == '~') { // XXX 
        modify = true;
        p = strip_spaces(p+1);
      }
      params.optional_ = false;
      if (*p == ':') {
        params.optional_ = true;
        p = strip_spaces(p+1);
      }
      t_word word;
      p = parse_word(err, word, p);
      ERR_GUARD(err) {
        switch (*p) {
          case '=': {
            P_cstr_ b = strip_spaces(p+1);
            switch (*b) {
              case '(': {
                if (b[1] == '*' && b[2] == ')') {
                  p = strip_spaces(b+3);
                  type = params.optional_ ? TYPE_OK : TYPE_K;
                } else
                  type = params.optional_ ? TYPE_OG : TYPE_G;
              } break;
              case '[': {
                type = params.optional_ ? TYPE_OH : TYPE_H;
                p = strip_spaces(p+1);
              } break;
              default: {
                if (*b == '<')
                  b = find(++b, '>');
                else
                  b = find_end_of_word(b);
                switch(*b) {
                  case ':': type = params.optional_ ? TYPE_OC : TYPE_C;
                    break;
                  default:  type = params.optional_ ? TYPE_OS : TYPE_S;
                    break;
                }
                p = strip_spaces(p+1);
              } break;
            }
          } break;
          case '(':
            type = params.optional_ ? TYPE_OZ : TYPE_Z;
            break;
          case '|':
          case ')':
          case '}':
          case ',': {
            type = params.optional_ ? TYPE_OB : B;
          } break;
          case '@': {
            P_cstr_ b = p;
            p = parse_bound(err, params, b);
            ERR_GUARD(err) {
              switch (*p) {
                case '=': type = params.optional_ ? TYPE_OL : TYPE_L;
                  p = strip_spaces(p+1);
                  break;
                case '{': type = params.optional_ ? TYPE_OT : TYPE_T;
                  break;
                default: err = err::E_PARSE;
                  break;
              }
            }
          } break;
          default: err = err::E_PARSE;
            break;
        }
        ERR_GUARD(err) {
          if (!modify || (type == TYPE_B))
            name = base::x_cast(word);
          else
            err = err::E_PARSE;
        }
      }
    }
    return p;
  }

  P_cstr_ parse_simple_value(r_err err, r_value value, P_cstr_ p) {
    ERR_GUARD(err) {
      return parse_word(err, value, p);
    }
    return p;
  }

  P_cstr_ parse_list_values(r_err err, r_values values, r_bool init,
                            r_rparams params, P_cstr_ p) {
    ERR_GUARD(err) {
      if (*p == '[') {
        p = strip_spaces(p+1);
        if (*p != ']') {
          auto max = get(params.range_max_);
          decltype(max) cnt = 0;
          while (*p && !err & (!max || cnt < max)) {
            t_word word;
            p = parse_word(err, word, p);
            ERR_GUARD(err) {
              values.push_back(base::x_cast(word));
              ++cnt;
              if (*p == ',')
                p = strip_spaces(p+1);
              else
                break;
            }
          }
          ERR_GUARD(err) {
            if (*p == ']') {
              if (cnt < get(params.range_min_))
                err = err::E_RANGE;
            } else
              err = err::E_RANGE;
          }
        } else
          init = true;
        p = strip_spaces(p+1);
      } else
        init = false;
    }
    return p;
  }

  P_cstr_ parse_selection_values(r_err err, r_values values, P_cstr_ p) {
    ERR_GUARD(err) {
      t_n::t_value cnt = 0;
      if (*p == '[') {
        p = strip_spaces(p+1);
        t_word word;
        p = parse_word(err, word, p);
        ERR_GUARD(err) {
          if (!word.is_empty()) {
            ++cnt;
            values.push_back(base::x_cast(word));
            while (*p == '|' && !err) {
              p = parse_word(err, word, strip_spaces(p+1));
              ERR_GUARD(err) {
                if (!word.is_empty()) {
                  for (auto&& i : values) {
                    if (word == i) {
                      err = err::E_SAME;
                      break;
                    }
                  }
                } else
                  err = err::E_PARSE;
                ERR_GUARD(err) {
                  ++cnt;
                  values.push_back(base::x_cast(word));
                }
              }
            }
          } else
            err = err::E_NO_NAME;
        }
      }
      if (*p == ']')
        p = strip_spaces(p+1);
      else
        err = err::E_PARSE;
    }
    return p;
  }

  P_cstr_ parse_compound_values(r_err err, r_values values, r_value value,
                                P_cstr_ p) {
    ERR_GUARD(err) {
      t_word word;
      p = parse_word(err, word, p);
      ERR_GUARD(err) {
        std::string main_value;
        if (!word.is_empty())
          value = base::x_cast(word);
        while (*p == ':' && !err) {
          p = parse_word(err, word, strip_spaces(p+1));
          if (!err) {
            if (!word.is_empty())
              values.push_back(base::x_cast(word));
            else
              err = err::E_EMPTY;
          }
        }
      }
    }
    return p;
  }

  P_cstr_ parse_table_values(t_err err, t_table_ref table, P_cstr_ p) {
    ERR_GUARD(err) {
      if (*p == '[') {
        p = strip_spaces(p+1);
        if (*p != ']') {
          do {
            t_word word;
            p = parse_word(err, word, p);
            ERR_GUARD(err) {
              if (*p == ':') {
                p = strip_spaces(p+1);
                t_table_value_ref value(table.add_value(err,
                                                          base::x_cast(word)));
                if (value) {
                  if (*p == '{') {
                    p = strip_spaces(p+1);
                    auto max = get(value.get_size());
                    for (decltype(max) cnt = 0; !err && cnt < max;) {
                      t_ref ref(err, value[t_ix{cnt}]);
                      switch (get_base_type(ref.get_type())) {
                        case TYPE_S: {
                          t_simple_ref simple_ref(ref);
                          t_value value;
                          p = parse_simple_value(err, value, p);
                          simple_ref.set_value(err, base::x_cast(value));
                        } break;
                        case TYPE_B: {
                          t_boolean_ref bool_ref(ref);
                          t_word word;
                          p = parse_word(err, word, p);
                          if (!word.is_empty()) {
                            t_bool state = true;
                            auto wrange = word.mk_range();
                            auto brange = bool_ref.get_name().mk_range(t_ix{1});
                            if (wrange[t_ix{1}] == '!')
                              state = false; // XXX - this is wrong
                            if (mk_crange<t_compare_>(wrange, t_ix(1 + !state)) !=
                                mk_crange<t_compare_>(brange, t_ix{1}))
                              bool_ref.set_value(err, state);
                            else
                              err = err::E_NO_MATCH;
                          } else
                            err = err::E_NO_NAME;
                        } break;
                        case TYPE_H: {
                          t_selection_ref selection_ref(ref);
                          t_value value;
                          p = parse_simple_value(err, value, p);
                          selection_ref.set_value(err, base::x_cast(value));
                        } break;
                        case TYPE_C: {
                          t_compound_ref compound_ref(ref);
                          t_value value;
                          p = parse_simple_value(err, value, p);
                          compound_ref.set_value(err, base::x_cast(value));
                        } break;
                        case TYPE_L: {
                          t_list_ref list_ref(ref);
                          t_values values;
                          t_bool init = false;
                          t_rparams params;
                          p = parse_list_values(err, values, init, params, p);
                          list_ref.set_values(err, base::x_cast(values));
                        } break;
                        case TYPE_T: {
                          t_table_ref table_ref(ref);
                          p = parse_table_values(err, table_ref, p);
                        } break;
                        default: err = err::E_INVALID_ARG;
                          break;
                      }

                      if (++cnt < max) {
                        if (*p == ',')
                          p = strip_spaces(p+1);
                        else
                          err = err::E_PARSE;
                      }
                    }
                  }
                  if (*p == '}') {
                    p = strip_spaces(p+1);
                    if (*p == ',')
                      p = strip_spaces(p+1);
                    else
                      break;
                  } else
                    err = err::E_PARSE;
                }
              } else
                err = err::E_PARSE;
            }
          } while(!err);
          if (*p == ']')
            p = strip_spaces(p+1);
          else
            err = err::E_PARSE;
        } else {
          table.set_as_initialized();
          p = strip_spaces(p+1);
        }
      } else
        err = err::E_PARSE;
    }
    return p;
  }

  P_cstr_ parse_table(t_err err, t_table_ref table, P_cstr_ p) {
    ERR_GUARD(err) {
      if (*p == '{') {
        p = strip_spaces(p+1);
        if (*p != '}') {
          p = parse_args(err, table, ',', p);
          ERR_GUARD(err) {
            if (*p == '}') {
              p = strip_spaces(p+1);
              if (*p == '=') {
                p = strip_spaces(p+1);
                if (*p == '[')
                  p = parse_table_values(err, table, p);
              } else
                err = err::E_PARSE;
            } else
              err = err::E_PARSE;
          }
        } else
          err = err::E_EMPTY;
      } else
        err = err::E_PARSE;
    }
    return p;
  }

  P_cstr_ parse_options(t_err err, t_options_ref options, P_cstr_ p) {
    ERR_GUARD(err) {
      if (*p == '(') {
        p = strip_spaces(p+1);
        if (*p != ')')
          p = parse_args(err, options, '|', p);
        else
          err = err::E_EMPTY;
        if (*p == ')')
           p = strip_spaces(p+1);
        else if (!err)
          err = err::E_PARSE;
      } else
        err = err::E_PARSE;
    }
    return p;
  }

  P_cstr_ parse_group(t_err err, t_group_ref group, P_cstr_ p) {
    ERR_GUARD(err) {
      if (*p == '(') {
        p = strip_spaces(p+1);
        if (*p != ')')
          p = parse_args(err, group, ',', p);
        if (*p == ')')
           p = strip_spaces(p+1);
        else if (!err)
          err = err::E_PARSE;
      } else
        err = err::E_PARSE;
    }
    return p;
  }

  P_cstr_ parse_arg(t_err err, t_group_ref group, P_cstr_ p) {
    ERR_GUARD(err) {
      t_name   name;
      t_type   type;
      t_params params;
      p = parse_name(err, name, type, params, p);
      ERR_GUARD(err) {
        switch (get_base_type(type)) {
          case TYPE_S: {
            t_value value;
            p = parse_simple_value(err, value, p);
            if (value.is_empty())
              group.add_simple(err, name, params);
            else
              group.add_simple(err, name, base::x_cast(value), params);
          } break;
          case TYPE_B: {
            group.add_boolean(err, name, type != TYPE_MB, params);
          } break;
          case TYPE_H: {
            t_values values;
            p = parse_selection_values(err, values, p);
            group.add_selection(err, name, values, params);
          } break;
          case TYPE_C: {
            t_values values;
            t_value value;
            p = parse_compound_values(err, values, value, p);
            if (value.is_empty())
              group.add_compound(err, name, values, params);
            else
              group.add_compound(err, name, values, value, params);
          } break;
          case TYPE_L: {
            t_values values;
            t_bool init = false;
            p = parse_list_values(err, values, init, params, p);
            if (!values.is_empty())
              group.add_list(err, name, values, params);
            else
              group.add_list(err, name, init, params);
          } break;
          case TYPE_G: {
            p = parse_group(err, group.add_group(err, name, params),
                            strip_spaces(p+1));
          } break;
          case TYPE_K: {
            group.add_open_group(err, name, params);
          } break;
          case TYPE_Z: {
            p = parse_options(err, group.add_options(err, name, params), p);
          } break;
          case TYPE_T: {
            p = parse_table(err, group.add_table(err, name, params), p);
          } break;
          default: err = err::E_INVALID_ARG;
            break;
        }
      }
    }
    return p;
  }

  P_cstr_ parse_arg(t_err err, t_options_ref options, P_cstr_ p) {
    ERR_GUARD(err) {
      t_name name;
      t_type type;
      t_params params;
      p = parse_name(err, name, type, params, p);
      ERR_GUARD(err) {
        if (!is_optional(type)) {
          switch (type) {
            case TYPE_S: {
              t_value value;
              p = parse_simple_value(err, value, p);
              if (value.is_empty())
                options.add_simple(err, name);
              else
                options.add_simple(err, name, base::x_cast(value));
            } break;
            case TYPE_B:
              options.add_boolean(err, name, type != TYPE_MB);
              break;
            case TYPE_H:{
              t_values values;
              p = parse_selection_values(err, values, p);
              options.add_selection(err, name, values);
            } break;
            case TYPE_C: {
              t_values values;
              t_value value;
              p = parse_compound_values(err, values, value, p);
              if (value.is_empty())
                options.add_compound(err, name, values);
              else
                options.add_compound(err, name, values, value);
            } break;
            case TYPE_L: {
              t_values values;
              t_bool init = false;
              p = parse_list_values(err, values, init, params, p);
              if (!values.is_empty())
                options.add_list(err, name, values, params);
              else
                options.add_list(err, name, init, params);
            } break;
            case TYPE_G: {
              p = parse_group(err, options.add_group(err, name),
                              strip_spaces(p+1));
            } break;
            case TYPE_K:
              options.add_open_group(err, name);
              break;
            case TYPE_T: {
              p = parse_table(err, options.add_table(err, name, params), p);
            } break;
            default: err = err::E_INVALID_ARG;
              break;
          }
        } else
          err = err::E_MANDATORY;
      }
    }
    return p;
  }

  P_cstr_ parse_arg(t_err err, t_table_ref table, P_cstr_ p) {
    ERR_GUARD(err) {
      t_name name;
      t_type type;
      t_params params;
      p = parse_name(err, name, type, params, p);
      ERR_GUARD(err) {
        if (!is_optional(type)) {
          switch (type) {
            case TYPE_S: {
              t_value value;
              p = parse_simple_value(err, value, p);
              if (value.is_empty())
                table.add_simple(err, name);
              else
                err = err::E_NOT_EMPTY;
            } break;
            case TYPE_B:
              table.add_boolean(err, name);
              break;
            case TYPE_H: {
              t_values values;
              p = parse_selection_values(err, values, p);
              table.add_selection(err, name, values);
            } break;
            case TYPE_C: {
              t_values values;
              t_value value;
              p = parse_compound_values(err, values, value, p);
              if (value.is_empty())
                table.add_compound(err, name, values);
              else
                err = err::E_NOT_EMPTY;
            } break;
            case TYPE_L:{
              t_values values;
              t_bool init = false;
              p = parse_list_values(err, values, init, params, p);
              ERR_GUARD(err) {
                if (!init && values.is_empty())
                  table.add_table(err, name, params);
                else
                  err = err::E_NOT_EMPTY;
              }
            } break;
            case TYPE_T: {
              p = parse_table(err, table.add_table(err, name, params), p);
            } break;
            default: err = err::E_INVALID_ARG;
              break;
          }
        } else
          err = err::E_MANDATORY;
      }
    }
    return p;
  }

  template<typename P>
  P_cstr_ parse_args(t_err err, P parent, const char delimit, P_cstr_ p) {
    ERR_GUARD(err) {
      for (; *p && !err; p = strip_spaces(p+1)) {
        p = parse_arg(err, parent, p);
        if (*p != delimit)
          break;
      }
    }
    return p;
  }
}

///////////////////////////////////////////////////////////////////////////////

namespace build
{
  t_void build_name(t_err err, r_text   text,
                               R_name   name,
                               t_type   type,
                               t_params params) {
    ERR_GUARD(err) {
      if (params.optional_)
        text << ':';
      text << name;
    }
  }

  t_void build_simple(t_err err, r_text        text,
                                 R_name        name,
                                 t_simple_cref ref) {
    ERR_GUARD(err) {
      text << name;
      text << '=';
      if (!ref.get_value().is_empty())
        text << ref.get_value();
    }
  }

  t_void build_boolean(t_err err, r_text         text,
                                  R_name         name,
                                  t_boolean_cref ref) {
    ERR_GUARD(err) {
      if (!ref.get_value())
        text << '!';
      text << name;
    }
  }

  t_void build_selection(t_err err, r_text           text,
                                    R_name           name,
                                    t_selection_cref ref) {
    ERR_GUARD(err) {
      text << name;
      text << "=["_SL;
      auto last = cend(ref.get_values());
      for (auto k = cbegin(ref.get_values());;) {
        text << *k++;
        if (k != last)
          text << '|';
        else
          break;
      }
      text << "]"_SL;
    }
  }

  t_void build_compound(t_err err, r_text          text,
                                   R_name          name,
                                   t_compound_cref ref) {
    ERR_GUARD(err) {
      text << name;
      text << '=';
      if (!ref.get_value().is_empty())
        text << ref.get_value();
      for (auto&& part: ref.get_part_values()) {
        text << ':';
        text << part;
      }
    }
  }

  t_void build_list(t_err err, r_text       text,
                               R_name       name,
                               t_list_cref ref) {
    ERR_GUARD(err) {
      t_rparams params(ref.get_range_params());
      text << name;
      text << '@';
      if (get(params.range_min_) != get(params.range_max_)) {
        text << integer(get(params.range_min_));
        text << '-';
      }
      if (!get(params.range_max_))
        text << "n="_SL;
      else {
        text << integer(get(params.range_max_));
        text << '=';
      }
      if (!ref.get_values().is_empty()) {
        text << '[';
        auto last = cend(ref.get_values());
        for (auto k = cbegin(ref.get_values());;) {
          text << *k++;
          if (k != last)
            text << ',';
          else
            break;
        }
        text << ']';
      } else if (ref.is_initialized())
        text << "[]"_SL;
    }
  }

  t_void build_table_values(t_err err, r_text        text,
                                        t_table_cref table) {
    ERR_GUARD(err) {
      if (table.is_initialized() || get(table.size_value())) { // not efficient
        text << '[';
        if (get(table.size_value())) { // empty -XXX
          t_table_value_cref value{table.cbegin_value()};
          do {
            text << value.get_name();
            text << ":{"_SL;
            auto max = get(value.get_size());
            for (decltype(max) ix = 0; ix < max;) {
              t_cref arg{value[t_ix{ix}]};
              switch (arg.get_base_type()) {
                case TYPE_S: {
                  t_simple_cref ref(arg);
                  text << ref.get_value();
                } break;
                case TYPE_B: {
                  t_boolean_cref ref(arg);
                  if (!ref.get_value())
                    text << '!';
                  text << ref.get_name();
                } break;
                case TYPE_H: {
                  t_selection_cref ref(arg);
                  text << ref.get_value();
                } break;
                case TYPE_T: {
                  build_table_values(err, text, arg);
                } break;
                default: err = err::E_UNKNOWN;
                  break;
              }
              ERR_GUARD(err) {
                if (++ix < max)
                  text << ',';
              } else
                break;
            }
            text << '}';
            value = table.cnext_value(value);
            if (value)
              text << ',';
          } while (!err && value);
        }
        text << ']';
      }
    }
  }

  template<typename P> t_void build_args(t_err, r_text, const P, char);

  t_void build_table(t_err err, r_text        text,
                                 R_name        name,
                                 t_table_cref table) {
    ERR_GUARD(err) {
      t_rparams params(table.get_range_params());
      text << name;
      text << '@';
      if (get(params.range_min_)) {
        text << integer(get(params.range_min_));
        text << '-';
      }
      if (!get(params.range_max_))
        text << "n{"_SL;
      else {
        text << integer(get(params.range_max_));
        text << '{';
      }
      build_args(err, text, table, ',');
      ERR_GUARD(err) {
        text << "}="_SL;
        build_table_values(err, text, table);
      }
    }
  }

  t_void build_open_group(t_err err, r_text          text,
                                     R_name          name,
                                     t_open_group_cref open_group) {
    ERR_GUARD(err) {
      text << name << "=(*)";
    }
  }

  t_void build_options(t_err err, r_text         text,
                                  R_name         name,
                                  t_options_cref options) {
    ERR_GUARD(err) {
      text << options.get_extension();
      text << '(';
      build_args(err, text, options, '|');
      ERR_GUARD(err) {
        text << ')';
      }
    }
  }

  t_void build_group(t_err err, r_text       text,
                                R_name       name,
                                t_group_cref group) {
    ERR_GUARD(err) {
      text << name;
      text << "=("_SL;
      build_args(err, text, group, ',');
      ERR_GUARD(err) {
        text << ')';
      }
    }
  }

  t_void build_group(t_err err, r_text       text,
                                t_group_cref group) {
    ERR_GUARD(err) {
      text << '(';
      build_args(err, text, group, ',');
      ERR_GUARD(err) {
        text << ')';
      }
    }
  }

  t_void build_arg(t_err err, r_text         text,
                              t_cref         ref,
                              t_options_cref options) {
    ERR_GUARD(err) {
      t_type type = ref.get_type();
      if (is_optional(type))
        text << ':';
      const t_name& extname = options.get_extension();
      const t_name& argname = ref.get_name();
      t_name name;
      if (extname.is_empty())
        name << argname;
      else {
        name << '<';
        ///name.append(argname.(extname.length() - 1); // XXX
      }
      switch (get_base_type(type)) {
        case TYPE_S:
          build_simple(err, text, name, ref);
          break;
        case TYPE_B:
          build_boolean(err, text, name, ref);
          break;
        case TYPE_L:
          build_list(err, text, name, ref);
          break;
        case TYPE_C:
          build_compound(err, text, name, ref);
          break;
        case TYPE_H:
          build_selection(err, text, name, ref);
          break;
        case TYPE_K:
          build_open_group(err, text, name, ref);
          break;
        case TYPE_G:
          build_group(err, text, name, ref);
          break;
        case TYPE_T:
          build_table(err, text, name, ref);
          break;
        default: err = err::E_UNKNOWN;
          break;
      }
    }
  }

  t_void build_arg(t_err err, r_text text,
                              t_cref ref,
                              t_table_cref) { // XXX
    ERR_GUARD(err) {
      t_type type = ref.get_type();
      if (is_optional(type))
        text << ':';
      const t_name& name = ref.get_name();
      switch (get_base_type(type)) {
        case TYPE_S:
          build_simple(err, text, name, ref);
          break;
        case TYPE_B:
          build_boolean(err, text, name, ref);
          break;
        case TYPE_L:
          build_list(err, text, name, ref);
          break;
        case TYPE_C:
          build_compound(err, text, name, ref);
          break;
        case TYPE_H:
          build_selection(err, text, name, ref);
          break;
        case TYPE_T:
          build_table(err, text, name, ref);
          break;
        default: err = err::E_UNKNOWN;
          break;
      }
    }
  }

  t_void build_arg(t_err err, r_text text,
                              t_cref ref,
                              t_group_cref) { // XXX
    ERR_GUARD(err) {
      t_type type = ref.get_type();
      if (is_optional(type))
        text << ':';
      const t_name& name = ref.get_name();
      switch (get_base_type(type)) {
        case TYPE_S:
          build_simple(err, text, name, ref);
          break;
        case TYPE_B:
          build_boolean(err, text, name, ref);
          break;
        case TYPE_L:
          build_list(err, text, name, ref);
          break;
        case TYPE_C:
          build_compound(err, text, name, ref);
          break;
        case TYPE_H:
          build_selection(err, text, name, ref);
          break;
        case TYPE_K:
          build_open_group(err, text, name, ref);
          break;
        case TYPE_G:
          build_group(err, text, name, ref);
          break;
        case TYPE_Z:
          build_options(err, text, name, ref);
          break;
        case TYPE_T:
          build_table(err, text, name, ref);
          break;
        default: err = err::E_UNKNOWN;
          break;
      }
    }
  }

  template<typename P>
  t_void build_args(t_err err, r_text text,
                               P      parent,
                               t_char delimit) {
    ERR_GUARD(err) {
      if (!parent.is_empty()) {
        auto max = get(parent.get_size());
        decltype(max) ix = 0;
        do {
          build_arg(err, text, parent[t_ix{ix}], parent);
          if (++ix < max)
            text << delimit;
        } while (!err && ix < max);
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

namespace merge
{
  t_bool check_use(t_err err, t_cref use_ref) {
    ERR_GUARD(err) {
      const t_type type = use_ref.get_type();
      if (!is_optional(type) &&
          type != TYPE_K && type != TYPE_Z) {
        switch (use_ref.get_base_type()) {
          case TYPE_G:
          case TYPE_B:
            break;
          case TYPE_S: {
            t_simple_cref simple(use_ref);
            if (simple.get_value().is_empty())
              err = err::E_EMPTY;
          } break;
          case TYPE_C: {
            t_compound_cref compound(use_ref);
            if (compound.get_value().is_empty())
              err = err::E_EMPTY;
          } break;
          case TYPE_L: {
            t_list_cref list(use_ref);
            if (list.get_values().is_empty())
              err = err::E_EMPTY;
          } break;
          case TYPE_T: {
            t_table_cref table(use_ref); /// XXX  - empty
            if (!get(table.size_value()) && !table.is_initialized())
              err = err::E_XXX;
          } break;
          default: err = err::E_INTERNAL;
            break;
        }
      } else
        err = err::E_XXX;
    }
    return !err;
  }

  t_bool found_and_swap_use(t_err err, t_group_ref   use, // don't need references
                                       t_ref         use_ref,
                                       t_ix::t_value use_ix,
                                       t_cref        def_ref) {
    ERR_GUARD(err) {
      if (use_ref.get_name() != def_ref.get_name()) {
        auto max = get(use.get_size());
        t_ix::t_value ix = 0;
        for (; ix < max; ++ix) {
          if (ix != use_ix) {
            t_ref ref = use[t_ix{ix}];
            if (def_ref.get_name() == ref.get_name()) {
              use.swap(err, t_ix{ix}, t_ix{use_ix});
              use_ref = ref;
              break;
            }
          }
        }
        if (ix == max)
          return false;
      }
      ERR_GUARD(err) {
        if (get_path_type(use_ref.get_type()) !=
            get_path_type(def_ref.get_type()))
          err = err::E_INCOMPATIBLE;
      }
    }
    return !err;
  }

  t_bool insert_default_use(t_err err, t_group_ref    use,
                                       t_ix::t_value& use_ix,
                                       t_group_cref   def,
                                       t_cref         def_ref) {
    ERR_GUARD(err) {
      const t_type def_type = def_ref.get_type();
      if (is_optional(def_type)) {
        switch (def_type) {
          case TYPE_OK:
          case TYPE_OZ:
          case TYPE_OB:
          case TYPE_OH:
          case TYPE_OG:
            break;
          case TYPE_OS: {
            t_simple_cref def_simple(def_ref);
            if (!def_simple.get_value().is_empty()) {
              auto max = get(use.get_size());
              use.add_simple(err, def_ref.get_name(), def_simple.get_value(),
                             t_oparams(true));
              if (use_ix < max)
                use.swap(err, t_ix{use_ix}, t_ix{max-1});
              if (!err)
                ++use_ix;
            }
          } break;
          case TYPE_OC: {
            t_compound_cref def_compound(def_ref);
            if (!def_compound.get_value().is_empty()) {
              auto max = get(use.get_size());
              use.add_compound(err, def_ref.get_name(),
                               def_compound.get_part_values(),
                               def_compound.get_value(),
                               t_oparams(true));
              if (use_ix < max)
                use.swap(err, t_ix{use_ix}, t_ix{max-1});
              if (!err)
                ++use_ix;
            }
          } break;
          case TYPE_OL: {
            t_list_cref def_list(def_ref);
            t_params params(t_oparams(true),
                                  def_list.get_range_params());
            t_bool insert = false;
            auto max = get(use.get_size());
            if (!def_list.get_values().is_empty()) {
              use.add_list(err, def_ref.get_name(),
                                 def_list.get_values(),
                                 params);
              insert = true;
            } else if (def_list.is_initialized()) {
              use.add_list(err, def_ref.get_name(), true, params);
              insert = true;
            }
            if (!err && insert) {
              if (use_ix < max)
                use.swap(err, t_ix{use_ix}, t_ix{max-1});
              ++use_ix;
            }
          } break;
          case TYPE_OT: {
            t_table_cref def_table(def_ref);
            auto max = get(use.get_size());
            if ((!def_table.is_empty() || def_table.is_initialized()) &&
                append(err, use, def_table)) {
              if (use_ix < max)
                use.swap(err, t_ix{use_ix}, t_ix{max-1});
              ++use_ix;
            }
          } break;
          default: err = err::E_INTERNAL;
            break;
        }
      } else
        err = err::E_NO_ARG;
    }
    return !err;
  }

  t_void check_mandatory(t_err, t_group_ref, t_ref, t_cref);

  t_bool merge_group(t_err err, t_group_ref  use, t_group_cref def) {
    ERR_GUARD(err) {
      auto def_max = get(def.get_size());
      auto use_max = get(use.get_size());
      if (use_max <= def_max) {
        if (def_max) {
          if (use_max) {
            t_ix::t_value ix = 0, use_ix = 0;
            do {
              t_cref def_ref  = def[t_ix{ix}];
              t_ref  use_ref  = use[t_ix{use_ix}];
              if (check_use(err, use_ref)) {
                if (found_and_swap_use(err, use, use_ref, use_ix, def_ref)) {
                  check_mandatory(err, use, use_ref, def_ref);
                  ++use_ix;
                } else {
                  insert_default_use(err, use, use_ix, def, def_ref);
                  use_max = get(use.get_size());
                  if (use_max > def_max)
                    err = err::E_INVALID_ARG;
                }
              }
            } while (!err && ++ix < use_max);
            if (use_ix == use_max) {
              if (ix < def_max) {
                for (; !err && ix < def_max; ++ix)
                  insert_default_use(err, use, use_ix, def, def[t_ix{ix}]);
              }
            } else if (!err)
              err = err::E_UNKNOWN;
          } else {
            decltype(use_max) use_ix = 0;
            for (decltype(def_max) ix = 0; !err && ix < def_max; ++ix)
              insert_default_use(err, use, use_ix, def, def[t_ix{ix}]);
          }
        }
      } else
        err = err::E_XXX;
    }
    return !err;
  }

  t_void check_mandatory(t_err err, t_group_ref use,
                                    t_ref       use_ref,
                                    t_cref      def_ref) {
    ERR_GUARD(err) {
      const t_type def_type = def_ref.get_type();
      switch (def_type) {
        case TYPE_OK:
        case TYPE_K:
        case TYPE_B:
          break;
        case TYPE_OB:
        case TYPE_B: {
          t_boolean_cref boolean(use_ref);
          if (!boolean.get_value())
            err = err::E_INVALID_ARG;
        } break;
        case TYPE_OS:
        case TYPE_S: {
          t_simple_cref def_simple(def_ref), use_simple(use_ref);
          if (!is_optional(def_type) &&
              !def_simple.get_value().is_empty() &&
               def_simple.get_value() != use_simple.get_value())
            err = err::E_NO_MATCH;
        } break;
        case TYPE_OC:
        case TYPE_C: {
          t_compound_cref def_compound(def_ref);
          const t_bool mandatory = !def_compound.get_value().is_empty() &&
                                   !is_optional(def_type);
          if (use_ref.get_type() == TYPE_S) {
            t_simple_ref use_simple(use_ref);
            if (!mandatory || use_simple.get_value() ==
                              def_compound.get_value()) {
              t_compound_ref use_compound(t_ref{});
              use_simple.transform_to(err, use_compound,
                                      def_compound.get_part_values());
            } else
              err = err::E_NO_MATCH;
          } else {
            t_compound_ref use_compound(use_ref);
            if (!mandatory || use_compound.get_value() ==
                              def_compound.get_value())
              use_compound.add_missing_part_values(err,
                                                   def_compound.
                                                     get_part_values());
            else
              err = err::E_NO_MATCH;
          }
        } break;
        case TYPE_H:
        case TYPE_OH: {
          t_selection_cref selection_ref(def_ref);
          t_simple_cref simple_ref(use_ref);
          if (simple_ref) {
            if (!selection_ref.test_value(err, simple_ref.get_value()))
              err = err::E_INVALID_ARG;
          } else
            err = err::E_INVALID_ARG;
        } break;
        case TYPE_OL:
        case TYPE_L: {
          t_list_cref def_list(def_ref), use_list(use_ref);
          if (def_list.get_range_params() ==
              use_list.get_range_params()) {
            if (!is_optional(def_type) &&
                !def_list.get_values().is_empty() &&
                 def_list.get_values() != use_list.get_values())
              err = err::E_NO_MATCH;
          } else
            err = err::E_INVALID_ARG;
        } break;
        case TYPE_OG:
        case TYPE_G: {
          merge_group(err, use_ref, def_ref);
        } break;
        case TYPE_OZ:
        case TYPE_Z: {
          t_options_cref options(def_ref);
          // don't know what to do. - XXX
          // can search through use for all options
          //check in use
          // type might need to know its an options
        } break;
        case TYPE_OT:
        case TYPE_T: {
          t_table_cref def_table(def_ref), use_table(use_ref);
          if (is_same_definition(def_table, use_table, true)) {
            if (!is_optional(def_type) &&
                !is_same(def_table, use_table))
              err = err::E_NO_MATCH;
          } else
            err = err::E_INVALID_ARG;
        } break;
        default: err = err::E_INTERNAL;
          break;
      }
    }
  }
}
}

///////////////////////////////////////////////////////////////////////////////

  t_bool check_empty(t_err err, t_string_crange word) {
    /* XXX
    ERR_GUARD(err) {
      if (!word.is_empty() &&
          !(word.size() == 2 && word[0] == '<' && word[1] == '>')) {
        if (word[0] != '<') {
          word.insert(word.begin(), '<');
          word.push_back('>');
        } else if (word.back() != '>')
          err = err::E_INVALID_ARG;
        if (!err)
          return false;
      } else
        err = err::EMPTY;
    }
    */
    return word == INVALID;
  }

  t_bool check_name(t_err err, t_string_crange name) {
    ERR_GUARD(err) {
      if (!check_empty(err, name)) {
        return true;
      }
    }
    return false;
  }

  t_bool check_value(t_err err, t_string_crange value) {
    ERR_GUARD(err) {
      if (!check_empty(err, value)) {
        return true;
      }
    }
    return false;
  }

  t_bool check_values(t_err err, const t_values& values) {
    ERR_GUARD(err) {
      if (!values.is_empty()) {
        for (auto value : values) {
          if (!check_value(err, value))
            return false;
        }
        return true;
      } else
        err = err::E_EMPTY;
    }
    return false;
  }

  t_bool check_selection_values(t_err err, t_values& values) {
    ERR_GUARD(err) {
      if (check_values(err, values)) {
        t_n_ segs = get(values.get_segs_num());
        for (t_ix_ seg = 0; seg < segs; ++seg) {
          auto name = values.get(t_seg_no{seg});
          auto id   = values.find(name);
          if (values.find_next(name, id) != base::string::segmented::BAD_ID)
            return false;
        }
        return true;
      }
    }
    return false;
  }

  t_bool check_selection_value(t_err err, t_value& value,
                               const t_values& values) {
    ERR_GUARD(err) {
      if (!check_empty(err, value))
        return values.find(value) != base::string::segmented::BAD_ID;
    }
    return false;
  }

  t_bool check_range(t_err err, const t_rparams& params) {
    if (get(params.range_max_) &&
       (get(params.range_max_) < get(params.range_min_)))
      err = err::E_RANGE;
    return true;
  }

  t_bool check_range(t_err err, const t_rparams& params, t_n size) {
    if (check_range(err, params)) {
      if (get(size) >= get(params.range_min_)) {
        if (!get(params.range_max_) || (get(size) <= get(params.range_max_)))
          return true;
        err = err::E_RANGE;
      } else
        err = err::E_RANGE;
    }
    return false;
  }

////////////////////////////////////////////////////////////////////////////////

 t_void print(P_cstr_ prefix, R_words words) {
    t_word word("[");
    auto itr = words.begin(), end = words.end();
    while (itr != end) {
      word << *itr;
      if (++itr != end)
        word << ',';
    }
    word << "]";
    std::cout << prefix << get(word.get_cstr()) << std::endl;
  }

  t_bool match(argn::R_fullname maybe, argn::R_fullname name, unsigned n) {
    auto m = get(maybe.get_segs_num());
    if (m >= n) {
      for (decltype(m) ix = 0; ix < n; ++ix)
        if (maybe[t_seg_no{ix}] != name[t_seg_no{ix}])
          return false;
      return true;
    }
    return false;
  }

  // not sure how to do this yet - efficiency - XXX
  t_bool t_arg_compare_::operator()(R_fullname lh,
                                    R_fullname rh) const noexcept {
    return lh < rh;
  }

////////////////////////////////////////////////////////////////////////////////

  t_ref::t_ref(t_err err, R_ref ref) noexcept
      : argn_(ref.argn_), arg_(ref.arg_) {
    if (err)
      clear_();
    else if (!is_valid_())
      err = err::E_INVALID_REF;
  }

  t_type t_ref::get_type() const noexcept {
    if (is_valid_())
      return get_().second.info_.type_;
    return TYPE_Q;
  }

  t_type t_ref::get_base_type() const noexcept {
    if (is_valid_())
      return argn::get_base_type(get_().second.info_.type_);
    return TYPE_Q;
  }

  R_fullname t_ref::get_fullname() const noexcept {
    if (is_valid_())
      return get_().first;
    return EMPTY_FULLNAME;
  }

  R_name t_ref::get_name() const noexcept {
    if (is_valid_())
      return get_().first.back(); // XXX
    return EMPTY_NAME;
  }

  t_oparams t_ref::get_optional_params() const noexcept {
    if (is_valid_())
      return {is_optional(get_().second.info_.type_)};
    return {false};
  }

////////////////////////////////////////////////////////////////////////////////

  t_cref::t_cref(t_err err, R_cref ref) noexcept
      : argn_(ref.argn_), arg_(ref.arg_) {
    if (err)
      clear_();
    else if (!is_valid_())
      err = err::E_INVALID_REF;
  }

  t_type t_cref::get_type() const noexcept {
    if (is_valid_())
      return get_().second.info_.type_;
    return TYPE_Q;
  }

  t_type t_cref::get_base_type() const noexcept {
    if (is_valid_())
      return argn::get_base_type(get_().second.info_.type_);
    return TYPE_Q;
  }

  R_fullname t_cref::get_fullname() const noexcept {
    if (is_valid_())
      return get_().first;
    return EMPTY_FULLNAME;
  }

  R_name t_cref::get_name() const noexcept {
    if (is_valid_())
      return get_().first.back();
    return EMPTY_NAME;
  }

  t_oparams t_cref::get_optional_params() const noexcept {
    if (is_valid_())
      return {is_optional(get_().second.info_.type_)};
    return {false};
  }

////////////////////////////////////////////////////////////////////////////////

  t_simple_ref::t_simple_ref(t_ref ref) : t_ref(ref) {
    if (is_valid_() && get_base_type() != TYPE_S)
      clear_();
  }

  t_simple_ref::t_simple_ref(t_err err, t_ref ref) : t_ref(err, ref) {
    if (!err && get_base_type() != TYPE_S) {
      clear_();
      //err.set(mk_cstr("id is not a simple argument"));
      err = err::E_XXX;
    }
  }

  t_bool t_simple_ref::transform_to(t_err err, t_compound_ref& compound,
                                               t_values values) { // XXX
    ERR_GUARD(err) {
      if (is_valid_())
        compound = set_argn_().transform_(err, *this, base::x_cast(values));
      else
        err = err::E_INVALID_REF;
    }
    return !err;
  }

  R_value t_simple_ref::get_value() const {
    if (is_valid_()) {
      if (!get_().second.values_.is_empty())
        return get_().second.values_.front(); // XXX
    }
    return EMPTY_VALUE;
  }

  t_bool t_simple_ref::set_value(t_err err, t_value value) {
    if (check_value(err, value)) {
      if (is_valid_()) {
        if (get_().second.values_.is_empty())
          set_().second.values_.push_back(base::x_cast(value));
        else
          set_().second.values_.change(t_seg_no{0}, base::x_cast(value));
      } else
        err = err::E_INVALID_REF;
    }
    return !err;
  }

////////////////////////////////////////////////////////////////////////////////

  t_simple_cref::t_simple_cref(t_cref ref) : t_cref(ref) {
    if (is_valid_() && get_base_type() != TYPE_S)
      clear_();
  }

  t_simple_cref::t_simple_cref(t_err err, t_cref ref)
    : t_cref(err, ref) {
    if (!err && get_base_type() != TYPE_S) {
      clear_();
      //err.set(mk_cstr("id is not a simple argument"));
      err = err::E_XXX;
    }
  }

  R_value t_simple_cref::get_value() const {
    if (is_valid_()) {
      if (!get_().second.values_.is_empty())
        return get_().second.values_.front();
    }
    return EMPTY_VALUE;
  }

////////////////////////////////////////////////////////////////////////////////

  t_boolean_ref::t_boolean_ref(t_ref ref) : t_ref(ref) {
    if (is_valid_() && get_base_type() != TYPE_MB)
      clear_();
  }

  t_boolean_ref::t_boolean_ref(t_err err, t_ref ref) : t_ref(err, ref) {
    if (!err && get_base_type() != TYPE_MB) {
      clear_();
      err = err::E_XXX;
    }
  }

  t_bool t_boolean_ref::get_value() const {
    if (is_valid_())
      return get_().second.values_.front() == str(true);
    return false;
  }

  t_bool t_boolean_ref::set_value(t_err err, t_bool state) {
    ERR_GUARD(err) {
     if (is_valid_()) {
       if (state || get_type() == TYPE_MB)
         set_().second.values_.change(t_seg_no{0}, state); //XXX
       else
        err = err::E_XXX;
      } else
        err = err::E_INVALID_REF;
    }
    return !err;
  }

////////////////////////////////////////////////////////////////////////////////

  t_boolean_cref::t_boolean_cref(t_cref ref) : t_cref(ref) {
    if (is_valid_() && get_base_type() != TYPE_MB)
      clear_();
  }

  t_boolean_cref::t_boolean_cref(t_err err, t_cref ref)
    : t_cref(err, ref) {
    if (!err && get_base_type() != TYPE_MB) {
      clear_();
      err = err::E_XXX;
    }
  }

  t_bool t_boolean_cref::get_value() const {
    if (is_valid_())
      return get_().second.values_.front() == str(true);
    return false;
  }

////////////////////////////////////////////////////////////////////////////////

  t_list_ref::t_list_ref(t_ref ref) : t_ref(ref) {
    if (is_valid_() && get_base_type() != TYPE_L)
      clear_();
  }

  t_list_ref::t_list_ref(t_err err, t_ref ref) : t_ref(err, ref) {
    if (!err && get_base_type() != TYPE_L) {
      clear_();
      err = err::E_XXX;
    }
  }

  t_rparams t_list_ref::get_range_params() const {
    if (is_valid_())
      return {t_n{get_().second.info_.range_.max_},
              t_n{get_().second.info_.range_.min_}};
    return {};
  }

  t_bool t_list_ref::set_as_initialized() {
    if (is_valid_()) {
      if (!get_().second.info_.range_.cnt_) {
        set_().second.info_.range_.init_ = true;
        return true;
      }
    }
    return false;
  }

  t_bool t_list_ref::is_initialized() const {
    if (is_valid_())
      return get_().second.info_.range_.init_;
    return false;
  }

  R_values t_list_ref::get_values() const {
    if (is_valid_())
      return get_().second.values_;
    return EMPTY_VALUES; // XXX
  }

  t_bool t_list_ref::set_values(t_err err, t_values values) {
    if (check_values(err, values)) {
      if (is_valid_())
        set_().second.values_ = base::x_cast(values);
      else
        err = err::E_INVALID_REF;
    }
    return !err;
  }

////////////////////////////////////////////////////////////////////////////////

  t_list_cref::t_list_cref(t_cref ref) : t_cref(ref) {
    if (is_valid_() && get_base_type() != TYPE_L)
      clear_();
  }

  t_list_cref::t_list_cref(t_err err, t_cref ref) : t_cref(err, ref) {
    if (!err && get_base_type() != TYPE_L) {
      clear_();
      err = err::E_XXX;
    }
  }

  t_rparams t_list_cref::get_range_params() const {
    if (is_valid_())
      return {t_n{get_().second.info_.range_.max_},
              t_n{get_().second.info_.range_.min_}};
    return {};
  }

  t_bool t_list_cref::is_initialized() const {
    if (is_valid_())
      return get_().second.info_.range_.init_;
    return false;
  }

  R_values t_list_cref::get_values() const {
    if (is_valid_())
      return get_().second.values_;
    return EMPTY_VALUES;
  }

////////////////////////////////////////////////////////////////////////////////

  t_compound_ref::t_compound_ref(t_ref ref) : t_ref(ref) {
    if (is_valid_() && get_base_type() != TYPE_C)
      clear_();
  }

  t_compound_ref::t_compound_ref(t_err err, t_ref ref) : t_ref(err, ref) {
    if (!err && get_base_type() != TYPE_C) {
      clear_();
      err = err::E_XXX;
    }
  }

  t_bool t_compound_ref::add_missing_part_values(t_err    err,
                                                 R_values values) {
    ERR_GUARD(err) {
      if (is_valid_()) {
        auto max  = get(get_().second.values_.get_size());
        auto nmax = get(values.get_size());
        if (nmax > max) {
          for (auto ix = max; ix < nmax; ++ix)
            set_().second.values_.push_back(values[t_seg_no{ix}]);
        } else if (nmax < max)
          err = err::E_XXX;
      } else
        err = err::E_INVALID_REF;
    }
    return false;
  }

  R_values t_compound_ref::get_part_values() const {
    if (is_valid_())
      return get_().second.values_;
    return EMPTY_VALUES;
  }

  R_value t_compound_ref::get_value() const {
    if (is_valid_())
      return get_().second.info_.ext_;
    return EMPTY_VALUE;
  }

  t_bool t_compound_ref::set_value(t_err err, t_value value) {
    if (check_value(err, value)) {
      if (is_valid_())
        set_().second.info_.ext_ = base::x_cast(value);
      else
        err = err::E_INVALID_REF;
    }
    return !err;
  }

////////////////////////////////////////////////////////////////////////////////

  t_compound_cref::t_compound_cref(t_cref ref) : t_cref(ref) {
    if (is_valid_() && get_base_type() != TYPE_C)
      clear_();
  }

  t_compound_cref::t_compound_cref(t_err err, t_cref ref) : t_cref(err, ref) {
    if (!err && get_base_type() != TYPE_C) {
      clear_();
      err = err::E_XXX;
    }
  }

  R_values t_compound_cref::get_part_values() const {
    if (is_valid_())
      return get_().second.values_;
    return EMPTY_VALUES;
  }

  R_value t_compound_cref::get_value() const {
    if (is_valid_())
      return get_().second.info_.ext_;
    return EMPTY_VALUE;
  }

////////////////////////////////////////////////////////////////////////////////

  t_selection_ref::t_selection_ref(t_ref ref) : t_ref(ref) {
    if (is_valid_() && get_base_type() != TYPE_H)
      clear_();
  }

  t_selection_ref::t_selection_ref(t_err err, t_ref ref) : t_ref(err, ref) {
    if (!err && get_base_type() != TYPE_H) {
      clear_();
      err = err::E_XXX;
    }
  }

  R_values t_selection_ref::get_values() const {
    if (is_valid_())
      return get_().second.values_;
    return EMPTY_VALUES;
  }

  R_value t_selection_ref::get_value() const {
    if (is_valid_())
      return get_().second.info_.ext_;
    return EMPTY_VALUE;
  }

  t_bool t_selection_ref::set_value(t_err err, t_value value) {
    if (is_valid_()) {
      if (check_selection_value(err, value, get_().second.values_))
        set_().second.info_.ext_ = base::x_cast(value);
    } else if (!err)
      err = err::E_INVALID_REF;
    return !err;
  }

  t_bool t_selection_ref::test_value(t_err err, t_value value) const {
    if (check_value(err, value)) {
      if (is_valid_())
        return get_().second.values_.find(value) !=
               base::string::segmented::BAD_ID;
      else
        err = err::E_INVALID_REF;
    }
    return !err;
  }

////////////////////////////////////////////////////////////////////////////////

  t_selection_cref::t_selection_cref(t_cref ref) : t_cref(ref) {
    if (is_valid_() && get_base_type() != TYPE_H)
      clear_();
  }

  t_selection_cref::t_selection_cref(t_err err, t_cref ref) : t_cref(err, ref) {
    if (!err && get_base_type() != TYPE_H) {
      clear_();
      err = err::E_XXX;
    }
  }

  R_values t_selection_cref::get_values() const {
    if (is_valid_())
      return get_().second.values_;
    return EMPTY_VALUES;
  }

  R_value t_selection_cref::get_value() const {
    if (is_valid_())
      return get_().second.info_.ext_;
    return EMPTY_VALUE;
  }

  t_bool t_selection_cref::test_value(t_err err, t_value value) const {
    if (check_value(err, value)) {
      if (is_valid_())
        return get_().second.values_.find(value) !=
               base::string::segmented::BAD_ID;
      err = err::E_INVALID_REF;
    }
    return !err;
  }

////////////////////////////////////////////////////////////////////////////////

  t_open_group_ref::t_open_group_ref(t_ref ref) : t_ref(ref) {
    if (is_valid_() && get_base_type() != TYPE_K)
      clear_();
  }

  t_open_group_ref::t_open_group_ref(t_err err, t_ref ref) : t_ref(err, ref) {
    if (!err && get_base_type() != TYPE_K) {
      clear_();
      err = err::E_XXX;
    }
  }

////////////////////////////////////////////////////////////////////////////////

  t_open_group_cref::t_open_group_cref(t_cref ref) : t_cref(ref) {
    if (is_valid_() && get_base_type() != TYPE_K)
      clear_();
  }

  t_open_group_cref::t_open_group_cref(t_err err, t_cref ref)
      : t_cref(err, ref) {
    if (!err && get_base_type() != TYPE_K) {
      clear_();
      err = err::E_XXX;
    }
  }

////////////////////////////////////////////////////////////////////////////////

  t_collection_ref::t_collection_ref(t_ref ref) : t_ref(ref) {
    if (is_valid_()) {
      const t_type base = get_base_type();
      if (base != TYPE_G && base != TYPE_T && base != TYPE_Z && base != TYPE_TI)
        clear_();
    }
  }

  t_collection_ref::t_collection_ref(t_err err, t_ref ref)
      : t_ref(err, ref) {
    ERR_GUARD(err) {
      const t_type base = get_base_type();
      if (base != TYPE_G && base != TYPE_T && base != TYPE_Z && base != TYPE_TI) {
        clear_();
        err = err::E_XXX;
      }
    }
  }

  t_ref t_collection_ref::operator[](t_ix ix) {
    if (is_valid_())
      return make_ref_((p_arg_)set_().second.info_.mem_[get(ix)]);
    return {};
  }

  t_cref t_collection_ref::operator[](t_ix ix) const {
    if (is_valid_())
      return make_id_((P_arg_)get_().second.info_.mem_[get(ix)]);
    return {};
  }

  t_ref t_collection_ref::operator[](t_string_crange name) {
    if (is_valid_()) {
      auto max = get_().second.info_.mem_.size();
      for (decltype(max) ix = 0; ix < max; ++ix) {
        p_arg_ arg = (p_arg_)set_().second.info_.mem_[ix];
        if (arg->first.back() == name)
          return make_ref_(arg);
      }
    }
    return {};
  }

  t_cref t_collection_ref::operator[](t_string_crange name) const {
    if (is_valid_()) {
      auto max = get_().second.info_.mem_.size();
      for (decltype(max) ix = 0; ix < max; ++ix) {
        P_arg_ arg = (P_arg_)get_().second.info_.mem_[ix];
        if (arg->first.back() == name)
          return make_id_(arg);
      }
    }
    return {};
  }

  t_n t_collection_ref::get_size() const {
    if (is_valid_())
      return t_n(get_().second.info_.mem_.size()); // narrow here
    return t_n{0};
  }

  t_bool t_collection_ref::is_empty() const {
    if (is_valid_())
      return get_().second.info_.mem_.empty();
    return false;
  }

////////////////////////////////////////////////////////////////////////////////

  t_collection_cref::t_collection_cref(t_cref ref) : t_cref(ref) {
    if (is_valid_()) {
      t_type base = get_base_type();
      if (base != TYPE_G && base != TYPE_T && base != TYPE_Z && base != TYPE_TI)
        clear_();
    }
  }

  t_collection_cref::t_collection_cref(t_err err, t_cref ref) : t_cref(err, ref) {
    ERR_GUARD(err) {
      t_type base = get_base_type();
      if (base != TYPE_G && base != TYPE_T && base != TYPE_Z && base != TYPE_TI) {
        clear_();
        err = err::E_XXX;
      }
    }
  }

  t_cref t_collection_cref::operator[](t_ix ix) const {
    if (is_valid_())
      return make_ref_((P_arg_)get_().second.info_.mem_[get(ix)]);
    return {};
  }

  t_cref t_collection_cref::operator[](t_string_crange name) const {
    if (is_valid_()) {
      auto max = get_().second.info_.mem_.size();
      for (decltype(max) ix = 0; ix < max; ++ix) {
        P_arg_ arg = (P_arg_)get_().second.info_.mem_[ix];
        if (arg->first.back() == name)
          return make_ref_(arg);
      }
    }
    return {};
  }

  t_n t_collection_cref::get_size() const {
    if (is_valid_())
      return t_n(get_().second.info_.mem_.size()); // narrow here
    return t_n{0};
  }

  t_bool t_collection_cref::is_empty() const {
    if (is_valid_())
      return get_().second.info_.mem_.empty();
    return false;
  }

////////////////////////////////////////////////////////////////////////////////

  t_group_ref::t_group_ref(t_ref ref) : t_collection_ref(ref) {
    if (is_valid_() && get_base_type() != TYPE_G)
      clear_();
  }

  t_group_ref::t_group_ref(t_err err, t_ref ref)
      : t_collection_ref(err, ref) {
    if (!err && get_base_type() != TYPE_G) {
      clear_();
      err = err::E_XXX;
    }
  }

  t_bool t_group_ref::swap(t_err err, t_ix first, t_ix second) {
    ERR_GUARD(err) {
      if (is_valid_()) {
        auto max = get(to_ix(get_size()));
        decltype(max) one = get(first), two = get(second);
        if (one < max && two < max) {
          p_void tmp = set_().second.info_.mem_[one];
          set_().second.info_.mem_[one] = set_().second.info_.mem_[two];
          set_().second.info_.mem_[two] = tmp;
        } else
          err = err::E_XXX;
      } else
        err = err::E_INVALID_REF;
    }
    return !err;
  }

  t_ref t_group_ref::add(t_err err, t_string_crange cstr) {
    ERR_GUARD(err) {
      if (is_valid_()) {
        P_cstr_ p = cstr.ptr;
        p = notation::parse::parse_arg(err, *this, p);
        ERR_GUARD(err) { // XXX
          if (*p == '\0')
            return {}; // can write the back
          err = err::E_XXX;
        }
      } else
        err = err::E_INVALID_REF;
    }
    return {};
  }

  t_ref t_group_ref::add_simple(t_err err, t_name    name,
                                          R_oparams params) {
    ERR_GUARD(err) {
      if (is_valid_())
        return set_argn_().add_simple_(err, *this, base::x_cast(name), params);
      err = err::E_INVALID_REF;
    }
    return {};
  }

  t_ref t_group_ref::add_simple(t_err err, t_name    name,
                                          t_value   value,
                                          R_oparams params) {
    ERR_GUARD(err) {
      if (is_valid_())
        return set_argn_().add_simple_(err, *this, base::x_cast(name),
                                       base::x_cast(value), params);
      err = err::E_INVALID_REF;
    }
    return {};
  }

  t_ref t_group_ref::add_boolean(t_err err, t_name    name,
                                            t_bool    state,
                                            R_oparams params) {
    ERR_GUARD(err) {
      if (is_valid_())
        return set_argn_().add_boolean_(err, *this, base::x_cast(name),
                                        state, params);
      err = err::E_INVALID_REF;
    }
    return {};
  }

  t_ref t_group_ref::add_compound(t_err err, t_name    name,
                                             t_values  values,
                                             R_oparams params) {
    ERR_GUARD(err) {
      if (is_valid_())
        return set_argn_().add_compound_(err, *this, base::x_cast(name),
                                         base::x_cast(values), params);
      err = err::E_INVALID_REF;
    }
    return {};
  }

  t_ref t_group_ref::add_compound(t_err err, t_name    name,
                                             t_values  values,
                                             t_value   value,
                                             R_oparams params) {
    ERR_GUARD(err) {
      if (is_valid_())
        return set_argn_().add_compound_(err, *this, base::x_cast(name),
                                         base::x_cast(values),
                                         base::x_cast(value), params);
      err = err::E_INVALID_REF;
    }
    return {};
  }

  t_ref t_group_ref::add_list(t_err err, t_name   name,
                                         t_bool   init,
                                         R_params params) {
    ERR_GUARD(err) {
      if (is_valid_())
        return set_argn_().add_list_(err, *this, base::x_cast(name), init,
                                      params);
      err = err::E_INVALID_REF;
    }
    return {};
  }

  t_ref t_group_ref::add_list(t_err err, t_name   name,
                                         t_values values,
                                         R_params params) {
    ERR_GUARD(err) {
      if (is_valid_())
        return set_argn_().add_list_(err, *this, base::x_cast(name),
                                      base::x_cast(values), params);
      err = err::E_INVALID_REF;
    }
    return {};
  }

  t_ref t_group_ref::add_selection(t_err err, t_name    name,
                                              t_values  values,
                                              R_oparams params) {
    ERR_GUARD(err) {
      if (is_valid_())
        return set_argn_().add_selection_(err, *this, base::x_cast(name),
                                          base::x_cast(values), params);
      err = err::E_INVALID_REF;
    }
    return {};
  }

  t_ref t_group_ref::add_group(t_err err, t_name    name,
                                          R_oparams params) {
    ERR_GUARD(err) {
      if (is_valid_())
        return set_argn_().add_group_(err, *this, base::x_cast(name), params);
      err = err::E_INVALID_REF;
    }
    return {};
  }

  t_ref t_group_ref::add_open_group(t_err err, t_name    name,
                                               R_oparams params) {
    ERR_GUARD(err) {
      if (is_valid_())
        return set_argn_().add_open_group_(err, *this, base::x_cast(name),
                                           params);
      err = err::E_INVALID_REF;
    }
    return {};
  }

  t_ref t_group_ref::add_options(t_err err, t_name    name,
                                            R_oparams params) {
    ERR_GUARD(err) {
      if (is_valid_())
        return set_argn_().add_options_(err, *this, base::x_cast(name),
                                        params);
      err = err::E_INVALID_REF;
    }
    return {};
  }

  t_ref t_group_ref::add_table(t_err err, t_name   name,
                                           R_params params) {
    ERR_GUARD(err) {
      if (is_valid_())
         return set_argn_().add_table_(err, *this, base::x_cast(name),
                                        params);
      err = err::E_INVALID_REF;
    }
    return {};
  }

  t_bool t_group_ref::del(R_name name) {
    if (is_valid_())
      return set_argn_().del_(*this, name);
    return false;
  }

////////////////////////////////////////////////////////////////////////////////

  t_group_cref::t_group_cref(t_cref ref) : t_collection_cref(ref) {
    if (is_valid_() && get_base_type() != TYPE_G)
      clear_();
  }

  t_group_cref::t_group_cref(t_err err, t_cref ref)
      : t_collection_cref(err, ref) {
    if (!err && get_base_type() != TYPE_G) {
      clear_();
      err = err::E_XXX;
    }
  }

////////////////////////////////////////////////////////////////////////////////

  t_options_ref::t_options_ref(t_ref ref) : t_collection_ref(ref) {
    if (is_valid_() && get_base_type() != TYPE_Z)
      clear_();
  }

  t_options_ref::t_options_ref(t_err err, t_ref ref)
    : t_collection_ref(err, ref) {
    if (!err && get_base_type() != TYPE_Z) {
      clear_();
      err = err::E_XXX;
    }
  }

  R_name t_options_ref::get_extension() const {
    if (is_valid_())
      return get_().second.info_.ext_;
    return EMPTY_NAME;
  }

  t_ref t_options_ref::add(t_err err, t_string_crange cstr) {
    ERR_GUARD(err) {
      if (is_valid_()) {
        if (is_empty()) { // looks wrong
          P_cstr_ p = cstr.ptr;
          p = notation::parse::parse_options(err, *this, p);
          ERR_GUARD(err) {
            if (*p == '\0')
              return {};
            err = err::E_XXX;
          }
        } else
          err = err::E_XXX;
      } else
        err = err::E_INVALID_REF;
    }
    return {};
  }

  t_ref t_options_ref::add_simple(t_err err, t_name name) {
    ERR_GUARD(err) {
      if (is_valid_()) {
        const t_oparams params{false};
        return set_argn_().add_simple_(err, *this, base::x_cast(name),
                                       params);
      }
      err = err::E_INVALID_REF;
    }
    return {};
  }

  t_ref t_options_ref::add_simple(t_err err, t_name name, t_value value) {
    ERR_GUARD(err) {
      if (is_valid_()) {
        const t_oparams params{false};
        return set_argn_().add_simple_(err, *this, base::x_cast(name),
                                       base::x_cast(value), params);
      }
      err = err::E_INVALID_REF;
    }
    return {};
  }

  t_ref t_options_ref::add_boolean(t_err err, t_name name, t_bool state) {
    ERR_GUARD(err) {
      if (is_valid_()) {
        const t_oparams params{false};
        return set_argn_().add_boolean_(err, *this, base::x_cast(name), state,
                                        params);
      }
      err = err::E_INVALID_REF;
    }
    return {};
  }

  t_ref t_options_ref::add_compound(t_err err, t_name name,
                                               t_values values) {
    ERR_GUARD(err) {
      if (is_valid_()) {
        const t_oparams params{false};
        return set_argn_().add_compound_(err, *this, base::x_cast(name),
                                         base::x_cast(values), params);
      }
      err = err::E_INVALID_REF;
    }
    return {};
  }

  t_ref t_options_ref::add_compound(t_err err, t_name   name,
                                               t_values values,
                                               t_value  value) {
    ERR_GUARD(err) {
      if (is_valid_()) {
        const t_oparams params{false};
        return set_argn_().add_compound_(err, *this, base::x_cast(name),
                                         base::x_cast(values),
                                         base::x_cast(value), params);
      }
      err = err::E_INVALID_REF;
    }
    return {};
  }

  t_ref t_options_ref::add_list(t_err err, t_name    name,
                                           t_bool    init,
                                           R_rparams rparams) {
    ERR_GUARD(err) {
      if (is_valid_()) {
        const t_params params{t_oparams{false}, rparams};
        return set_argn_().add_list_(err, *this, base::x_cast(name), init,
                                      params);
      }
      err = err::E_INVALID_REF;
    }
    return {};
  }

  t_ref t_options_ref::add_list(t_err err, t_name    name,
                                           t_values  values,
                                           R_rparams rparams) {
    ERR_GUARD(err) {
      if (is_valid_()) {
        const t_params params{t_oparams{false}, rparams};
        return set_argn_().add_list_(err, *this, base::x_cast(name),
                                     base::x_cast(values), params);
      }
      err = err::E_INVALID_REF;
    }
    return {};
  }

  t_ref t_options_ref::add_selection(t_err err, t_name name,
                                                t_values values) {
    ERR_GUARD(err) {
      if (is_valid_()) {
        const t_oparams params{false};
        return set_argn_().add_selection_(err, *this, base::x_cast(name),
                                          base::x_cast(values), params);
      }
      err = err::E_INVALID_REF;
    }
    return {};
  }

  t_ref t_options_ref::add_group(t_err err, t_name name) {
    ERR_GUARD(err) {
      if (is_valid_()) {
        const t_oparams params{false};
        return set_argn_().add_group_(err, *this, base::x_cast(name), params);
      }
      err = err::E_INVALID_REF;
    }
    return {};
  }

  t_ref t_options_ref::add_open_group(t_err err, t_name name) {
    ERR_GUARD(err) {
      if (is_valid_()) {
        const t_oparams params{false};
        return set_argn_().add_open_group_(err, *this, base::x_cast(name),
                                         params);
      }
      err = err::E_INVALID_REF;
    }
    return {};
  }

  t_ref t_options_ref::add_table(t_err err, t_name    name,
                                             R_rparams rparams) {
    ERR_GUARD(err) {
      if (is_valid_()) {
        const t_params params{t_oparams{false}, rparams};
        return set_argn_().add_table_(err, *this, base::x_cast(name), params);
      }
      err = err::E_INVALID_REF;
    }
    return {};
  }

  t_bool t_options_ref::del(R_name name) {
    if (is_valid_())
      return set_argn_().del_(*this, name);
    return false;
  }

////////////////////////////////////////////////////////////////////////////////

  t_options_cref::t_options_cref(t_cref ref) : t_collection_cref(ref) {
    if (is_valid_() && get_base_type() != TYPE_Z)
      clear_();
  }

  t_options_cref::t_options_cref(t_err err, t_cref ref)
    : t_collection_cref(err, ref) {
    if (!err && get_base_type() != TYPE_Z) {
      clear_();
      err = err::E_XXX;
    }
  }

  R_name t_options_cref::get_extension() const {
    if (is_valid_())
      return get_().second.info_.ext_;
    return EMPTY_NAME;
  }

////////////////////////////////////////////////////////////////////////////////

  t_table_ref::t_table_ref(t_ref ref) : t_collection_ref(ref) {
    if (is_valid_() && get_base_type() != TYPE_T)
      clear_();
  }

  t_table_ref::t_table_ref(t_err err, t_ref ref)
    : t_collection_ref(err, ref) {
    if (!err && get_base_type() != TYPE_T) {
      clear_();
      err = err::E_XXX;
    }
  }

  t_rparams t_table_ref::get_range_params() const {
    if (is_valid_())
      return {t_n{get_().second.info_.range_.max_},
              t_n{get_().second.info_.range_.min_}};
    return {};
  }

  t_cref t_table_ref::add_simple(t_err err, t_name name) {
    ERR_GUARD(err) {
      if (is_valid_()) {
        const t_oparams params{false};
        return set_argn_().add_simple_(err, *this, base::x_cast(name), params);
      }
      err = err::E_INVALID_REF;
    }
    return {};
  }

  t_cref t_table_ref::add_boolean(t_err err, t_name name) {
    ERR_GUARD(err) {
      if (is_valid_()) {
        const t_oparams params{false};
        return set_argn_().add_boolean_(err, *this, base::x_cast(name), false,
                                        params);
      }
      err = err::E_INVALID_REF;
    }
    return {};
  }

  t_cref t_table_ref::add_compound(t_err err, t_name   name,
                                               t_values values) {
    ERR_GUARD(err) {
      if (is_valid_()) {
        const t_oparams params{false};
        return set_argn_().add_compound_(err, *this, base::x_cast(name),
                                         base::x_cast(values), params);
      }
      err = err::E_INVALID_REF;
    }
    return {};
  }

  t_cref t_table_ref::add_selection(t_err err, t_name   name,
                                                t_values values) {
    ERR_GUARD(err) {
      if (is_valid_()) {
        const t_oparams params{false};
        return set_argn_().add_selection_(err, *this, base::x_cast(name),
                                         base::x_cast(values), params);
      }
      err = err::E_INVALID_REF;
    }
    return {};
  }

  t_cref t_table_ref::add_list(t_err err, t_name    name,
                                           R_rparams rparams) {
    ERR_GUARD(err) {
      if (is_valid_()) {
        const t_params params{t_oparams{false}, rparams};
        return set_argn_().add_list_(err, *this, base::x_cast(name), true,
                                     params);
      }
      err = err::E_INVALID_REF;
    }
    return {};
  }

  t_ref t_table_ref::add_table(t_err err, t_name    name,
                                            R_rparams rparams) {
    ERR_GUARD(err) {
      if (is_valid_()) {
        const t_params params{t_oparams{false}, rparams};
        return set_argn_().add_table_(err, *this, base::x_cast(name), params);
      }
      err = err::E_INVALID_REF;
    }
    return {};
  }

  t_bool t_table_ref::set_as_initialized() {
    if (is_valid_()) {
      if (!get_().second.info_.range_.cnt_) {
        set_().second.info_.range_.init_ = true;
        return true;
      }
    }
    return false;
  }

  t_bool t_table_ref::is_initialized() const {
    if (is_valid_())
      return get_().second.info_.range_.init_;
    return false;
  }

  t_ref t_table_ref::add_value(t_err err, t_name name) {
    ERR_GUARD(err) {
      if (is_valid_())
        return set_argn_().add_table_value_(err, *this, base::x_cast(name));
      err = err::E_INVALID_REF;
    }
    return {};
  }

  t_bool t_table_ref::del_value(R_name name) {
    if (is_valid_())
      return set_argn_().del_table_value_(*this, name);
    return false;
  }

  t_ref t_table_ref::get_value(t_string_crange name) {
    if (is_valid_())
      return set_argn_().get_table_value_(*this, name);
    return {};
  }

  t_cref t_table_ref::get_value(t_string_crange name) const {
    if (is_valid_())
      return get_argn_().get_table_value_(*this, name);
    return {};
  }

  t_void t_table_ref::clear_value() {
    if (is_valid_())
      set_argn_().clr_table_value_(*this);
  }

  t_n t_table_ref::size_value() const {
    if (is_valid_())
      return t_n{get_().second.info_.range_.cnt_};
    return t_n{0};
  }

  t_ref t_table_ref::begin_value() {
    if (get(size_value())) { // is_empty - XXX
      t_fullname endname(get_fullname());
      endname.push_back("."_SL);
      endname.push_back("/"_SL);
      t_itr_ itr = set_argn_().table_.upper_bound(endname);
      if (itr != std::end(get_argn_().table_))
        return make_ref_(&(*itr));
    }
    return {};
  }

  t_cref t_table_ref::begin_value() const {
    return cbegin_value();
  }

  t_cref t_table_ref::cbegin_value() const {
    return t_table_cref(*this).cbegin_value();
  }

  t_ref t_table_ref::next_value(t_ref ref) {
    if (ref) {
      R_fullname refname = ref.get_fullname();
      t_fullname endname(refname);
      endname.push_back("/"_SL);
      t_itr_ itr = set_argn_().table_.upper_bound(endname);
      if (itr != std::cend(get_argn_().table_) &&
          match(itr->first, refname, get(refname.get_size()) - 1)) // XXX
        return make_ref_(&(*itr));
    }
    return {};
  }

  t_cref t_table_ref::next_value(t_ref ref) const {
    return cnext_value(ref);
  }

  t_cref t_table_ref::cnext_value(t_cref ref) const {
    return t_table_cref(*this).cnext_value(ref);
  }

////////////////////////////////////////////////////////////////////////////////

  t_table_cref::t_table_cref(t_cref ref) : t_collection_cref(ref) {
    if (is_valid_() && get_base_type() != TYPE_T)
      clear_();
  }

  t_table_cref::t_table_cref(t_err err, t_cref ref)
    : t_collection_cref(err, ref) {
    if (!err && get_base_type() != TYPE_T) {
      clear_();
      err = err::E_XXX;
    }
  }

  t_rparams t_table_cref::get_range_params() const {
    if (is_valid_())
      return {t_n{get_().second.info_.range_.max_},
              t_n{get_().second.info_.range_.min_}};
    return {};
  }

  t_bool t_table_cref::is_initialized() const {
    if (is_valid_())
      return get_().second.info_.range_.init_;
    return false;
  }

  t_cref t_table_cref::get_value(t_string_crange name) const {
    if (is_valid_())
      return get_argn_().get_table_value_(*this, name);
    return {};
  }

  t_n t_table_cref::size_value() const {
    if (is_valid_())
      return t_n{get_().second.info_.range_.cnt_};
    return t_n{0};
  }


  t_cref t_table_cref::begin_value() const {
    return cbegin_value();
  }

  t_cref t_table_cref::cbegin_value() const {
    if (get(size_value())) { // is_empty - XXX
      t_fullname endname(get_fullname());
      endname.push_back("."_SL);
      endname.push_back("/"_SL);
      t_citr_ itr = get_argn_().table_.upper_bound(endname);
      if (itr != std::cend(get_argn_().table_))
        return make_ref_(&(*itr));
    }
    return {};
  }

  t_cref t_table_cref::next_value(t_ref ref) const {
    return cnext_value(ref);
  }

  t_cref t_table_cref::cnext_value(t_cref ref) const {
    if (ref) {
      R_fullname refname = ref.get_fullname();
      t_fullname endname(refname);
      endname.push_back("/"_SL);
      t_citr_ itr = get_argn_().table_.upper_bound(endname);
      if (itr != std::cend(get_argn_().table_) &&
          match(itr->first, refname, get(refname.get_size()) - 1)) // XXX
        return make_ref_(&(*itr));
    }
    return {};
  }

////////////////////////////////////////////////////////////////////////////////

  t_table_value_ref::t_table_value_ref(t_ref ref) : t_collection_ref(ref) {
    if (is_valid_() && get_type() != TYPE_TI)
      clear_();
  }

  t_table_value_ref::t_table_value_ref(t_err err, t_ref ref)
    : t_collection_ref(err, ref) {
    if (!err && get_type() != TYPE_TI) {
      clear_();
      err = err::E_XXX;
    }
  }

  t_table_value_cref::t_table_value_cref(t_cref ref)
    : t_collection_cref(ref) {
    if (is_valid_() && get_type() != TYPE_TI)
      clear_();
  }

  t_table_value_cref::t_table_value_cref(t_err err, t_cref ref)
    : t_collection_cref(err, ref) {
    if (!err && get_type() != TYPE_TI) {
      clear_();
      err = err::E_XXX;
    }
  }

////////////////////////////////////////////////////////////////////////////////

  t_argn::t_argn(t_err err) {
    ERR_GUARD(err) {
      auto p = table_.insert({t_fullname{'#', "/"_SL},
                              t_arg_info_(TYPE_G)}); // XXX
      if (p.second)
        p.first->second.path_.push_back(str(TYPE_G));
      else
        err = err::E_XXX;
    }
  }

  t_ref t_argn::add_(r_err err, t_ref       parent,
                                x_name      name,
                                r_arg_info_ info) {
    ERR_GUARD(err) {
      t_fullname fullname(parent.get_fullname());
      t_type base = parent.get_base_type();
      t_type type = info.type_;
      if (base == TYPE_Z) {
        // fullname.back() = name; // XXX
        //if (!parent.get_().second.info_.ext_.is_empty()) {
        //  fullname.back() = fullname.back().mk_range(t_ix{1});
          // XXX not sure
          //fullname.back().insert(0, parent.get_().second.info_.ext_.get_cstr(),
          //                       parent.get_().second.info_.ext_.length() -1);
        //}
      } else if (base == TYPE_T) {
        t_word attr{"."};
        fullname.push_back(attr);
        fullname.push_back(name);
      } else if (get_base_type(type) == TYPE_Z) {
        parent.set_().second.info_.ext_ = name;
        t_word opt{".opt"};
        opt << integer(++parent.set_().second.info_.group_.options_);
        fullname.push_back(base::x_cast(opt));
        //name = name.back(); not sure XXX
      } else
        fullname.push_back(name);
      ERR_GUARD(err) {
        auto p = table_.insert(t_arg_(fullname, t_arg_value_(base::x_cast(info))));
        if (p.second) {
          r_arg_value_ v = p.first->second;
          if (base != TYPE_Z)
            v.path_ = parent.get_().second.path_;
          //else XXX
          //  v.path_.assign(parent.get_().second.path_.begin(),
          //                 parent.get_().second.path_.end() - 1);
          v.path_.push_back(str(get_path_type(type)));
          parent.set_().second.values_.push_back(fullname.back()); // make a pointer?
          parent.set_().second.info_.mem_.push_back(&(*p.first));
          return {this , &(*p.first)};
        } else
          err = err::E_XXX;
      }
    }
    return {};
  }

  t_ref t_argn::add_(r_err err, t_ref       parent,
                                x_name      name,
                                r_arg_info_ info,
                                x_value     value) {
    t_ref ref = add_(err, parent, base::x_cast(name), info);
    if (ref)
      ref.set_().second.values_.push_back(base::x_cast(value));
    return ref;
  }

  t_ref t_argn::add_(r_err err, t_ref       parent,
                                x_name      name,
                                r_arg_info_ info,
                                x_values    values) {
    t_ref ref = add_(err, parent, base::x_cast(name), info);
    if (ref)
      ref.set_().second.values_ = base::x_cast(values);
    return ref;
  }

  t_bool t_argn::del_(r_ref ref, R_name name) {
    switch (ref.get_base_type()) {
      case TYPE_G:
      case TYPE_Z: {
        t_collection_ref collection(ref);
        t_ref found = collection[name];
        if (found) {
          t_fullname argname(ref.get_fullname());
          argname.push_back(name);
          t_itr_ first = table_.find(argname);
          if (first != std::end(table_)) {
            argname.push_back("/"_SL);
            table_.erase(first, table_.upper_bound(argname));
            return true;
          }
        }
      } break;
      default:
        break;
     }
    return false;
  }

  t_ref t_argn::begin() {
    if (!table_.empty()) {
      t_group_ref root(get_root());
      if (root && !root.is_empty())
        return root[t_ix{0}];
    }
    return {};
  }

  t_ref t_argn::next(t_ref ref) {
    if (ref) {
      switch (ref.get_base_type()) {
        case TYPE_B:
        case TYPE_S:
        case TYPE_L:
        case TYPE_H:
        case TYPE_C:
        case TYPE_K: {
          R_fullname refname = ref.get_fullname();
          t_fullname endname(refname);
          endname.push_back("/"_SL);
          t_itr_ itr = table_.upper_bound(endname);
          if (itr != std::end(table_))
            return {this, &(*itr)};
        } break;
        case TYPE_T: {
          t_table_ref table(ref);
          return table.begin_value();
        } break;
        case TYPE_TI:
        case TYPE_G:
        case TYPE_Z: {
          t_collection_ref collection(ref);
          if (collection && !collection.is_empty())
            return collection[0_ix];
        } break;
        default:
          break;
      }
    }
    return {};
  }

  t_cref t_argn::begin() const {
    return cbegin();
  }

  t_cref t_argn::next(t_cref ref) const {
    return cnext(ref);
  }

  t_cref t_argn::cbegin() const {
    if (!table_.empty()) {
      t_group_cref root(get_root());
      if (root && !root.is_empty())
        return root[0_ix];
    }
    return {};
  }

  t_cref t_argn::cnext(t_cref ref) const {
    if (ref) {
      switch (ref.get_base_type()) {
        case TYPE_B:
        case TYPE_S:
        case TYPE_L:
        case TYPE_H:
        case TYPE_C:
        case TYPE_K: {
          R_fullname refname = ref.get_fullname();
          t_fullname endname(refname);
          endname.push_back("/"_SL);
          t_citr_ itr = table_.upper_bound(endname);
          if (itr != std::cend(table_))
            return {this, &(*itr)};
        } break;
        case TYPE_T: {
          t_table_cref table(ref);
          return table.cbegin_value();
        } break;
        case TYPE_TI:
        case TYPE_G:
        case TYPE_Z: {
          t_collection_cref collection(ref);
          if (collection && !collection.is_empty())
            return collection[0_ix];
        } break;
        default:
          break;
      }
    }
    return {};
  }

////////////////////////////////////////////////////////////////////////////////

  t_ref t_argn::add_simple_(r_err err, t_ref     parent,
                                       x_name    name,
                                       R_oparams params) {
    if (check_name(err, name)) {
      t_arg_info_ info(params.optional_ ? TYPE_OS : TYPE_S);
      return add_(err, parent, base::x_cast(name), info);
    }
    return {};
  }

  t_ref t_argn::add_simple_(r_err err, t_ref     parent,
                                       x_name    name,
                                       x_value   value,
                                       R_oparams params) {
    if (check_name(err, name) && check_value(err, value)) {
      t_arg_info_ info(params.optional_ ? TYPE_OS : TYPE_S);
      return add_(err, parent, base::x_cast(name), info, base::x_cast(value));
    }
    return {};
  }

  t_ref t_argn::add_list_(r_err err, t_ref    parent,
                                     x_name   name,
                                     t_bool   init,
                                     R_params params) {
    if (check_name(err, name) && check_range(err, params)) {
      const t_type type = params.optional_ ? TYPE_OL : TYPE_L;
      t_arg_info_ info(type, get(params.range_max_), get(params.range_min_),
                     init);
      return add_(err, parent, base::x_cast(name), info);
    }
    return {};
  }

  t_ref t_argn::add_list_(r_err err, t_ref    parent,
                                     x_name   name,
                                     x_values values,
                                     R_params params) {
    if (check_name(err, name) && check_values(err, values) &&
        check_range(err, params, t_n(values.get_size()))) { // narrowing
      T_type type = params.optional_ ? TYPE_OL : TYPE_L;
      t_arg_info_ info(type, get(params.range_max_), get(params.range_min_),
                     true);
      info.range_.cnt_ = get(values.get_size()); // XXX
      return add_(err, parent, base::x_cast(name), info, base::x_cast(values));
    }
    return {};
  }

  t_ref t_argn::add_selection_(r_err err, t_ref     parent,
                                          x_name    name,
                                          x_values  values,
                                          R_oparams params) {
    if (check_name(err, name) && check_selection_values(err, values)) {
      t_arg_info_ info(params.optional_ ? TYPE_OH : TYPE_H);
      return add_(err, parent, base::x_cast(name), info, base::x_cast(values));
    }
    return {};
  }

  t_ref t_argn::add_compound_(r_err err, t_ref     parent,
                                         x_name    name,
                                         x_values  values,
                                         R_oparams params) {
    if (check_name(err, name) && check_values(err, values)) {
      t_arg_info_ info(params.optional_ ? TYPE_OC : TYPE_C);
      return add_(err, parent, base::x_cast(name), info, base::x_cast(values));
    }
    return {};
  }

  t_ref t_argn::add_compound_(r_err err, t_ref     parent,
                                         x_name    name,
                                         x_values  values,
                                         x_value   value,
                                         R_oparams params) {
    if (check_name(err, name) && check_values(err, values) &&
        check_value(err, value)) {
      t_arg_info_ info(params.optional_ ? TYPE_OC : TYPE_C);
      info.ext_ = base::x_cast(value);
      return add_(err, parent, base::x_cast(name), info, base::x_cast(values));
    }
    return {};
  }

  t_ref t_argn::add_boolean_(r_err err, t_ref     parent,
                                        x_name    name,
                                        t_bool    state,
                                        R_oparams params) {
    if (check_name(err, name)) {
      t_arg_info_ info(params.optional_ ? TYPE_OB : TYPE_B);
      t_name tmp{str(state)};
      return add_(err, parent, base::x_cast(name), info, base::x_cast(tmp));
    }
    return {};
  }

  t_ref t_argn::add_group_(r_err err, t_ref     parent,
                                      x_name    name,
                                      R_oparams params) {
    if (check_name(err, name)) {
      t_arg_info_ info(params.optional_ ? TYPE_OG : TYPE_G);
      return add_(err, parent, base::x_cast(name), info);
    }
    return {};
  }

  t_ref t_argn::add_open_group_(r_err err, t_ref     parent,
                                           x_name    name,
                                           R_oparams params) {
    if (check_name(err, name)) {
      t_arg_info_ info(params.optional_ ? TYPE_OK : TYPE_K);
      return add_(err, parent, base::x_cast(name), info);
    }
    return {};
  }

  t_ref t_argn::add_options_(r_err err, t_ref     parent,
                                        x_name    name,
                                        R_oparams params) {
    if (check_name(err, name)) {
      t_arg_info_ info(params.optional_ ? TYPE_OZ : TYPE_Z);
      info.ext_ = name;
      return add_(err, parent, base::x_cast(name), info);
    }
    return {};
  }

  t_ref t_argn::add_table_(r_err err, t_ref    parent,
                                      x_name   name,
                                      R_params params) {
    if (check_name(err, name) && check_range(err, params)) {
      T_type type = params.optional_ ? TYPE_OT : TYPE_T;
      t_arg_info_ info(type, get(params.range_max_), get(params.range_min_));
      return add_(err, parent, base::x_cast(name), info);
    }
    return {};
  }

  inline r_arg_ get_(t_itr_& itr) {
    return *itr;
  }

  inline r_arg_value_ get_value_(t_itr_& itr) {
    return itr->second;
  }

  inline R_arg_ get_(t_citr_& itr) {
    return *itr;
  }

  inline R_arg_value_ get_value_(t_citr_& itr) {
    return itr->second;
  }

  inline r_arg_ get_(t_pair_& pair) {
    return *pair.first;
  }

  inline r_arg_value_ get_value_(t_pair_& pair) {
    return pair.first->second;
  }

  t_ref t_argn::add_table_value_(r_err err, t_ref ref, x_name name) {
    t_table_ref table(err, ref);
    ERR_GUARD(err) {
      if (!name.is_empty()) {
        auto fullname = table.get_fullname();
        fullname.push_back(base::x_cast(name));
        auto pair = table_.insert(t_arg_(fullname, t_arg_info_(TYPE_TI)));
        if (pair.second) {
          pair.first->second.info_.range_ = table.get_().second.info_.range_;
          pair.first->second.info_.range_.init_ = true;
          for (auto member : table.set_().second.info_.mem_) {
            p_arg_ arg = (p_arg_)member;
            auto mem_name(fullname);
            mem_name.push_back(arg->first.back());
            auto mem_pair = table_.insert(t_arg_(base::x_cast(mem_name),
                                                t_arg_info_(arg->second.info_.type_)));
            if (mem_pair.second) {
              pair.first->second.info_.mem_.push_back(&(*mem_pair.first));
              switch (arg->second.info_.type_) {
                case argn::TYPE_S:
                  break;
                case argn::TYPE_B:
                  mem_pair.first->second.values_.push_back(str(false));
                  break;
                case argn::TYPE_H:
                  get_value_(mem_pair).values_ = arg->second.values_;
                  get_value_(mem_pair).info_.mem_.push_back(arg);
                  break;
                case argn::TYPE_C:
                  mem_pair.first->second.values_ = arg->second.values_;
                  break;
                case argn::TYPE_L:
                  mem_pair.first->second.info_.range_ = arg->second.info_.range_;
                  break;
                case argn::TYPE_T:
                  mem_pair.first->second.info_.range_ = arg->second.info_.range_;
                  mem_pair.first->second.info_.range_.init_ = true;
                  mem_pair.first->second.info_.mem_   = arg->second.info_.mem_;
                  break;
                default: err = err::E_INTERNAL;
                  break;
              }
            } else
              err = err::E_XXX;
            if (err)
              break;
          }
          ERR_GUARD(err) {
              table.set_().second.info_.range_.init_ = true;
            ++table.set_().second.info_.range_.cnt_;
            return {this, &(*pair.first)};
          }
        } else if (pair.first != std::end(table_))
          err = err::E_XXX;
        else
          err = err::E_XXX;
      } else
        err = err::E_NO_NAME;
    }
    return {};
  }

 t_bool t_argn::del_table_value_(r_ref ref,  R_name name) {
    if (ref) {
      t_fullname argname(ref.get_fullname());
      argname.push_back(name);
      t_itr_ first = table_.find(argname);
      if (first != std::end(table_)) {
        argname.push_back("/"_SL);
        table_.erase(first, table_.upper_bound(argname));
        return true;
      }
    }
    return false;
  }

  t_void t_argn::clr_table_value_(r_ref ref) {
    t_table_ref table(ref);
    if (table && !table.is_empty()) {
      t_fullname endname(table.get_fullname());
      endname.push_back("."_SL);
      endname.push_back("/"_SL);
      t_itr_ first = table_.upper_bound(endname);
      if (first != std::end(table_)) {
        //endname.erase(endname.end() - 2); XXX
        table_.erase(first, table_.upper_bound(endname));
      }
      ref.set_().second.info_.range_.cnt_ = 0;
    }
  }

  t_ref t_argn::get_table_value_(r_ref ref, t_string_crange name) {
    t_fullname fullname(ref.get_fullname());
    fullname.push_back(name);
    return operator[](fullname);
  }

  t_cref t_argn::get_table_value_(t_cref ref, t_string_crange name) const {
    t_fullname fullname(ref.get_fullname());
    fullname.push_back(name);
    return operator[](fullname);
  }

  t_group_ref t_argn::get_root() {
    if (!table_.empty())
      return t_ref{this, &(*std::begin(table_))};
    return t_ref{};
  }

  t_group_cref t_argn::get_root() const {
    return cget_root();
  }

  t_group_cref t_argn::cget_root() const {
    if (!table_.empty())
      return t_cref{this, &(*std::cbegin(table_))};
    return t_cref{};
  }

  t_ref t_argn::operator[](R_fullname fullname) {
    auto itr = table_.find(fullname);
    if (itr != std::end(table_))
      return {this, &(*itr)};
    return {};
  }

  t_cref t_argn::operator[](R_fullname fullname) const {
    auto citr = table_.find(fullname);
    if (citr != std::cend(table_))
      return {this, &(*citr)};
    return {};
  }

  t_void t_argn::print() const {
    t_words names, values, paths, types;
    unsigned name_max =0, value_max = 0, path_max = 0, type_max = 0;

    printf("(arg/n{name/n=,value/n=,path/n=,type/n=}=[\n");
    for (auto&& p : table_) {
      t_word type("["_SL);
      R_arg_info_ info = p.second.info_;
      type << '<';
      type << str(info.type_);
      type << '>';

      t_type base = get_base_type(info.type_);
      if (base == TYPE_T || base == TYPE_L) {
        type << '<';
        if (info.range_.max_)
           type << integer(info.range_.max_); // XXX
        else
           type << 'n';
        type << ">,<"_SL;
        type << integer(info.range_.min_);
        type << ">,<"_SL;
        type << integer(info.range_.cnt_);
        type << ">,<"_SL;
        type << (info.range_.init_ ? "init"_SL : "uninit"_SL);
        type << '>';
      }
      if (base == TYPE_G || base == TYPE_Z || base == TYPE_T) {
        // number of options
        auto max = info.mem_.size();
        for (decltype(max) ix = 0; ix < max; ++ix) {
          P_arg_ arg = (P_arg_)info.mem_[ix];
          type << ",";
          type << arg->first.back();
        }
      }
      type << ']';

      t_word path("["_SL);
      auto max_path = get(p.second.path_.get_size());
      for (decltype(max_path) i = 0; i < max_path; ++i) {
        path << p.second.path_[t_seg_no{i}];
        if (i < max_path - 1)
          path << ',';
      }
      path << ']';

      t_word name("["_SL);
      auto max_names = get(p.first.get_size());
      for (decltype(max_names) i = 0; i < max_names; ++i) {
        name << p.first[t_seg_no{i}];
        if (i < max_names - 1)
          name << ',';
      }
      name << ']';

      t_word value("["_SL);
      auto max_values = get(p.second.values_.get_size());
      if (max_values) {
        for (decltype(max_values) i = 0; i < max_values; ++i) {
          t_string_crange range = p.second.values_[t_seg_no{i}];
          if (range == VALID) {
            value << range;
          } else
            value << "-";
          if (i < max_values - 1)
            value << ',';
        }
      }
      value << ']';

      types.push_back(type);
      paths.push_back(path);
      names.push_back(name);
      values.push_back(value);

      if (type_max < get(type.get_length()))
        type_max = get(type.get_length());

      if (path_max < get(path.get_length()))
        path_max = get(path.get_length());

      if (name_max < get(name.get_length()))
        name_max = get(name.get_length());

      if (value_max < get(value.get_length()))
        value_max = get(value.get_length());
    }

    unsigned entries = get(names.get_size());
    for (unsigned i = 0; i < entries; ++i) {
      /* XXX
      std::cout << "  {" << std::left
                << std::setw(name_max)  << get(names[i].get_cstr())  << ","
                << std::setw(value_max) << get(values[i].get_cstr()) << ","
                << std::setw(path_max)  << get(paths[i].get_cstr())  << ","
                << std::setw(type_max)  << get(types[i].get_cstr())  << "}";
      if (i < entries - 1)
        std::cout << ",";
      */
      std::cout << std::endl;
    }

    printf("])\n");
  }

  t_bool t_argn::is_empty() const {
    return table_.size() < 2;
  }

  t_n t_argn::get_size() const {
    return t_n((table_.empty()) ? 0 : table_.size() - 1); // narrowing
  }

  t_compound_ref t_argn::transform_(r_err err, r_simple_ref ref,
                                               x_values values) {
    if (check_values(err, values)) {
      ref.set_().second.info_.ext_  = ref.set_().second.values_.front();
      ref.set_().second.values_     = base::x_cast(values);
      ref.set_().second.info_.type_ =
        is_optional(ref.get_().second.info_.type_) ? TYPE_OC : TYPE_C;
      t_compound_ref tmp = ref;
      ref.clear_();
      return tmp;
    }
    return t_ref{};
  }

  t_bool operator==(R_argn lh, R_argn rh) {
    if (lh.get_size() == rh.get_size())
      return lh.get_root() == rh.get_root();
    return false;
  }

  t_bool operator!=(R_argn lh, R_argn rh) {
    return !(lh == rh);
  }

  t_bool operator==(t_cref lh, t_cref rh) {
    return is_same(lh, rh);
  }

  t_bool operator!=(t_cref lh, t_cref rh) {
    return !(lh == rh);
  }

  t_bool is_same_definition(t_cref lh, t_cref rh, t_bool ignore_optional) {
    if (lh && rh &&
        (ignore_optional ? lh.get_base_type() == rh.get_base_type()
                         : lh.get_type() == rh.get_type()) &&
        lh.get_name() == rh.get_name()) {
      switch (lh.get_base_type()) {
        case TYPE_S:
        case TYPE_K:
        case TYPE_B:
          return true;
        case TYPE_C: {
          t_compound_cref lh_ref(lh), rh_ref(rh);
          return lh_ref.get_part_values() == rh_ref.get_part_values();
        }
        case TYPE_H: {
          t_selection_cref lh_ref(lh), rh_ref(rh);
          return lh_ref.get_values() == rh_ref.get_values();
        }
        case TYPE_L: {
          t_list_cref lh_ref(lh), rh_ref(rh);
          return lh_ref.get_range_params() == rh_ref.get_range_params();
        }
        case TYPE_T: {
          t_table_cref lh_ref(lh), rh_ref(rh);
          if (lh_ref.get_range_params() == rh_ref.get_range_params() &&
              get(lh_ref.get_size()) == get(rh_ref.get_size())) {
            auto max = get(lh_ref.get_size());
            for (decltype(max) ix = 0; ix < max; ++ix)
              if (!is_same_definition(lh_ref[t_ix{ix}], rh_ref[t_ix{ix}]))
                return false;
            return true;
          }
        } break;
        case TYPE_TI: {
          t_table_value_cref lh_ref(lh), rh_ref(rh);
          if (get(lh_ref.get_size()) == get(rh_ref.get_size())) {
            auto max = get(lh_ref.get_size());
            for (decltype(max) ix = 0; ix < max; ++ix)
              if (!is_same_definition(lh_ref[t_ix{ix}], rh_ref[t_ix{ix}]))
                return false;
            return true;
          }
        }
        default:
          break;
      }
    }
    return false;
  }

  t_bool is_same(t_cref lh, t_cref rh) {
    if (lh && rh) {
      if (is_same_definition(lh, rh, false)) {
        switch (lh.get_base_type()) {
          case TYPE_S: {
            t_simple_cref lh_ref(lh), rh_ref(rh);
            return lh_ref.get_value() == rh_ref.get_value();
          }
          case TYPE_K:
            return true;
          case TYPE_B: {
            t_boolean_cref lh_ref(lh), rh_ref(rh);
            return lh_ref.get_value() == rh_ref.get_value();
          }
          case TYPE_C: {
            t_compound_cref lh_ref(lh), rh_ref(rh);
            return lh_ref.get_value() == rh_ref.get_value();
          }
          case TYPE_H: {
            t_selection_cref lh_ref(lh), rh_ref(rh);
            return lh_ref.get_value() == rh_ref.get_value();
          }
          case TYPE_L: {
            t_list_cref lh_ref(lh), rh_ref(rh);
            return lh_ref.get_values() == rh_ref.get_values();
          }
          case TYPE_T: {
            t_table_cref lh_ref(lh), rh_ref(rh);
            if (get(lh_ref.size_value()) == get(rh_ref.size_value())) {
              if (!get(lh_ref.size_value())) // is_empty - XXX
                return true;
              else {
                t_table_value_cref lh_value_ref(lh_ref.cbegin_value()),
                                    rh_value_ref(rh_ref.cbegin_value());
                do {
                  if (!is_same(lh_value_ref, rh_value_ref))
                    return false;
                  lh_value_ref = lh_ref.cnext_value(lh_value_ref);
                  rh_value_ref = rh_ref.cnext_value(rh_value_ref);
                } while (lh_value_ref && rh_value_ref);
                return !lh_value_ref && !rh_value_ref;
              }
            }
          } break;
          case TYPE_TI: {
            t_table_value_cref lh_ref(lh), rh_ref(rh);
            if (get(lh_ref.get_size()) == get(rh_ref.get_size())) {
              auto max = get(lh_ref.get_size());
              for (decltype(max) ix = 0; ix < max; ++ix)
                if (!is_same(lh_ref[t_ix{ix}], rh_ref[t_ix{ix}]))
                  return false;
              return true;
            }
          }
          break;
          default:
            break;
        }
      } else
        return !lh && !rh;
    }
    return false;
  }

  t_bool append(t_err err, t_group_ref group, t_cref ref) {
    ERR_GUARD(err) {
      if (group && ref) {
        // make sure something is not inserted from itself.
        return true;
      } else
        err = err::E_INVALID_REF;
    }
    return false;
  }

  t_void parse_notation(t_err err, r_argn argn, R_text text) {
    ERR_GUARD(err) {
      t_group_ref group(err, argn.get_root());
      notation::parse::parse_group(err, group,
                                   notation::parse::strip_spaces(get(text.get_cstr())));
    }
  }

  t_void build_notation(t_err err, r_text text, R_argn argn) {
    ERR_GUARD(err) {
      t_group_cref group(err, argn.get_root());
      notation::build::build_group(err, text, group);
    }
  }

  t_void merge_notation(t_err err, r_argn use, R_argn def) {
    ERR_GUARD(err) {
      t_group_ref  use_group(err, use.get_root());
      t_group_cref def_group(err, def.get_root());
      notation::merge::merge_group(err, use_group, def_group);
    }
  }

////////////////////////////////////////////////////////////////////////////////
}
}
