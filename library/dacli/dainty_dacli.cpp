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

// 1. have a defined argument syntax as to define arguments
// 2. define the parsing behaviour of arguments
//
// ------------------------------------------------------------------
// the checking functions are required
// 2. X/A return params and not range_params
// 3. error messages must be made more useful
// 4. write test scrips
// 5. make it look like dacli commands
// 6. rules - e.g. lookup take no values in its definition, no list or select list
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

  using namespace named;
  using namespace argn;
  using err::r_err;

  using std::begin;
  using std::cbegin;
  using std::end;
  using std::cend;

  const t_fullname empty_fullname;
  const t_name     empty_name;
  const t_value    empty_value;

////////////////////////////////////////////////////////////////////////////////

  namespace syntax
  {
    namespace parse
    {
      template<typename P>
      P_cstr_ parse_args(t_err, P, t_char delimit, P_cstr_);

      inline P_cstr_ strip_space(P_cstr_ p) {
        for (; *p == ' '; ++p);
        return p;
      }

      template<char C>
      inline P_cstr_ find(P_cstr_ p) {
        for (auto b = p; *p; ++p)
          if (*p == C && (b == p || p[-1] != '/'))
            break;
        return p;
      }

      inline P_cstr_ find_end_of_word(P_cstr_ p) {
        for (; *p && *p != ',' && *p != '=' && *p != '(' && *p != ')' &&
               *p != (':') && *p != '[' && *p != ']' && *p != ' ' &&
               *p != '|' && *p != '@' && *p != '{' && *p != '}' && *p != '>' &&
               *p != '<'; ++p);
        return p;
      }

      P_cstr_ parse_word(r_err err, r_word word, P_cstr_ p) {
        ERR_GUARD(err) {
          P_cstr_ b   = p;
          P_cstr_ end = 0;
          if (*b == '<') {
            end = find<'>'>(++b);
            if (*end == '>')
              p = end + 1;
            else
              err = err::E_PARSE;
          } else
            p = end = find_end_of_word(b);
          if (!err && end - b) {
            word += '<';
            ///word.append(b, end - b); - XXX
            word += '>';
            p = strip_space(p);
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
            p = strip_space(p+1);
            if (*p == 'n') {
              params.range_max_ = params.range_min_ = 0_n;
              p = strip_space(p+1);
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
                    p = strip_space(p+1);
                    if (*p == 'n') {
                      stop = true;
                      p = strip_space(p+1);
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

      P_cstr_ parse_name(r_err err, r_name name, t_type& type, r_params params,
                                    P_cstr_ p) {
        ERR_GUARD(err) {
          t_bool modify = false;
          if (*p == '!') {
            modify = true;
            p = strip_space(p+1);
          }
          params.optional_ = false;
          if (*p == ':') {
            params.optional_ = true;
            p = strip_space(p+1);
          }
          t_word word;
          p = parse_word(err, word, p);
          ERR_GUARD(err) {
            switch (*p) {
              case '=': {
                P_cstr_ b = strip_space(p+1);
                switch (*b) {
                  case '(': {
                    if (b[1] == '*' && b[2] == ')') {
                      p = strip_space(b+3);
                      type = params.optional_ ? TYPE_OK : TYPE_K;
                    } else
                      type = params.optional_ ? TYPE_OL : TYPE_L;
                  } break;
                  case '[': {
                    type = params.optional_ ? TYPE_OH : TYPE_H;
                    p = strip_space(p+1);
                  } break;
                  default: {
                    if (*b == '<')
                      b = find<'>'>(++b);
                    else
                      b = find_end_of_word(b);
                    switch(*b) {
                      case ':': type = params.optional_ ? TYPE_OC : TYPE_C;
                        break;
                      default:  type = params.optional_ ? TYPE_OS : TYPE_S;
                        break;
                    }
                    p = strip_space(p+1);
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
                type = params.optional_ ? TYPE_OB : (modify ? TYPE_MB : TYPE_B);
              } break;
              case '@': {
                P_cstr_ b = p;
                p = parse_bound(err, params, b);
                ERR_GUARD(err) {
                  switch (*p) {
                    case '=': type = params.optional_ ? TYPE_OA : TYPE_A;
                      p = strip_space(p+1);
                      break;
                    case '{': type = params.optional_ ? TYPE_OX : TYPE_X;
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
              if (!modify || (type == TYPE_MB))
                name = std::move(word);
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

      P_cstr_ parse_array_values(r_err err, r_values values, r_bool init,
                                 r_rparams params, P_cstr_ p) {
        ERR_GUARD(err) {
          if (*p == '[') {
            p = strip_space(p+1);
            if (*p != ']') {
              auto max = get(params.range_max_);
              decltype(max) cnt = 0;
              while (*p && !err & (!max || cnt < max)) {
                t_word word;
                p = parse_word(err, word, p);
                ERR_GUARD(err) {
                  values.push_back(std::move(word));
                  ++cnt;
                  if (*p == ',')
                    p = strip_space(p+1);
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
            p = strip_space(p+1);
          } else
            init = false;
        }
        return p;
      }

      P_cstr_ parse_selection_values(r_err err, r_values values, P_cstr_ p) {
        ERR_GUARD(err) {
          t_n::t_value cnt = 0;
          if (*p == '[') {
            p = strip_space(p+1);
            t_word word;
            p = parse_word(err, word, p);
            ERR_GUARD(err) {
              if (!word.is_empty()) {
                ++cnt;
                values.push_back(std::move(word));
                while (*p == '|' && !err) {
                  p = parse_word(err, word, strip_space(p+1));
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
                      values.push_back(std::move(word));
                    }
                  }
                }
              } else
                err = err::E_NO_NAME;
            }
          }
          if (*p == ']')
            p = strip_space(p+1);
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
              value = std::move(word);
            while (*p == ':' && !err) {
              p = parse_word(err, word, strip_space(p+1));
              if (!err) {
                if (!word.is_empty())
                  values.push_back(std::move(word));
                else
                  err = err::E_EMPTY;
              }
            }
          }
        }
        return p;
      }

      P_cstr_ parse_lookup_values(t_err err, t_lookup_ref lookup, P_cstr_ p) {
        ERR_GUARD(err) {
          if (*p == '[') {
            p = strip_space(p+1);
            if (*p != ']') {
              do {
                t_word word;
                p = parse_word(err, word, p);
                ERR_GUARD(err) {
                  if (*p == ':') {
                    p = strip_space(p+1);
                    t_lookup_value_ref value(lookup.add_value(err,
                                                              std::move(word)));
                    if (value) {
                      if (*p == '{') {
                        p = strip_space(p+1);
                        auto max = get(value.get_size());
                        for (decltype(max) cnt = 0; !err && cnt < max;) {
                          t_ref ref(err, value[t_ix{cnt}]);
                          switch (get_base_type(ref.get_type())) {
                            case TYPE_S: {
                              t_simple_ref simple_ref(ref);
                              t_value value;
                              p = parse_simple_value(err, value, p);
                              simple_ref.set_value(err, std::move(value));
                            } break;
                            case TYPE_MB: {
                              t_boolean_ref bool_ref(ref);
                              t_word word;
                              p = parse_word(err, word, p);
                              if (!word.is_empty()) {
                                t_bool state = true;
                                auto wrange = word.mk_range();
                                auto brange = bool_ref.get_name().
                                                mk_range(t_ix{1});
                                if (wrange[t_ix{1}] == '!')
                                  state = false; // XXX - this is wrong
                                if (std::strcmp(wrange.get(t_ix{1 + !state}),
                                                brange.get(t_ix{1})))
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
                              selection_ref.set_value(err, std::move(value));
                            } break;
                            case TYPE_C: {
                              t_compound_ref compound_ref(ref);
                              t_value value;
                              p = parse_simple_value(err, value, p);
                              compound_ref.set_value(err, std::move(value));
                            } break;
                            case TYPE_A: {
                              t_array_ref array_ref(ref);
                              t_values values;
                              t_bool init = false;
                              t_rparams params;
                              p = parse_array_values(err, values, init, params, p);
                              array_ref.set_values(err, std::move(values));
                            } break;
                            case TYPE_X: {
                              t_lookup_ref lookup_ref(ref);
                              p = parse_lookup_values(err, lookup_ref, p);
                            } break;
                            default: err = err::E_INVALID_ARG;
                              break;
                          }

                          if (++cnt < max) {
                            if (*p == ',')
                              p = strip_space(p+1);
                            else
                              err = err::E_PARSE;
                          }
                        }
                      }
                      if (*p == '}') {
                        p = strip_space(p+1);
                        if (*p == ',')
                          p = strip_space(p+1);
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
                p = strip_space(p+1);
              else
                err = err::E_PARSE;
            } else {
              lookup.set_as_initialized();
              p = strip_space(p+1);
            }
          } else
            err = err::E_PARSE;
        }
        return p;
      }

      P_cstr_ parse_lookup(t_err err, t_lookup_ref lookup, P_cstr_ p) {
        ERR_GUARD(err) {
          if (*p == '{') {
            p = strip_space(p+1);
            if (*p != '}') {
              p = parse_args(err, lookup, ',', p);
              ERR_GUARD(err) {
                if (*p == '}') {
                  p = strip_space(p+1);
                  if (*p == '=') {
                    p = strip_space(p+1);
                    if (*p == '[')
                      p = parse_lookup_values(err, lookup, p);
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

      P_cstr_ parse_option(t_err err, t_options_ref option, P_cstr_ p) {
        ERR_GUARD(err) {
          if (*p == '(') {
            p = strip_space(p+1);
            if (*p != ')')
              p = parse_args(err, option, '|', p);
            else
              err = err::E_EMPTY;
            if (*p == ')')
               p = strip_space(p+1);
            else if (!err)
              err = err::E_PARSE;
          } else
            err = err::E_PARSE;
        }
        return p;
      }

      P_cstr_ parse_list(t_err err, t_list_ref list, P_cstr_ p) {
        ERR_GUARD(err) {
          if (*p == '(') {
            p = strip_space(p+1);
            if (*p != ')')
              p = parse_args(err, list, ',', p);
            if (*p == ')')
               p = strip_space(p+1);
            else if (!err)
              err = err::E_PARSE;
          } else
            err = err::E_PARSE;
        }
        return p;
      }

      P_cstr_ parse_arg(t_err err, t_list_ref list, P_cstr_ p) {
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
                  list.add_simple(err, name, params);
                else
                  list.add_simple(err, name, std::move(value), params);
              } break;
              case TYPE_MB: {
                list.add_boolean(err, name, type != TYPE_MB, params);
              } break;
              case TYPE_H: {
                t_values values;
                p = parse_selection_values(err, values, p);
                list.add_selection(err, name, values, params);
              } break;
              case TYPE_C: {
                t_values values;
                t_value value;
                p = parse_compound_values(err, values, value, p);
                if (value.is_empty())
                  list.add_compound(err, name, values, params);
                else
                  list.add_compound(err, name, values, value, params);
              } break;
              case TYPE_A: {
                t_values values;
                t_bool init = false;
                p = parse_array_values(err, values, init, params, p);
                if (!values.empty())
                  list.add_array(err, name, values, params);
                else
                  list.add_array(err, name, init, params);
              } break;
              case TYPE_L: {
                p = parse_list(err, list.add_list(err, name, params),
                               strip_space(p+1));
              } break;
              case TYPE_K: {
                list.add_openlist(err, name, params);
              } break;
              case TYPE_Z: {
                p = parse_option(err, list.add_option(err, name, params), p);
              } break;
              case TYPE_X: {
                p = parse_lookup(err, list.add_lookup(err, name, params), p);
              } break;
              default: err = err::E_INVALID_ARG;
                break;
            }
          }
        }
        return p;
      }

      P_cstr_ parse_arg(t_err err, t_options_ref option, P_cstr_ p) {
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
                    option.add_simple(err, name);
                  else
                    option.add_simple(err, name, std::move(value));
                } break;
                case TYPE_B:
                case TYPE_MB:
                  option.add_boolean(err, name, type != TYPE_MB);
                  break;
                case TYPE_H:{
                  t_values values;
                  p = parse_selection_values(err, values, p);
                  option.add_selection(err, name, values);
                } break;
                case TYPE_C: {
                  t_values values;
                  t_value value;
                  p = parse_compound_values(err, values, value, p);
                  if (value.is_empty())
                    option.add_compound(err, name, values);
                  else
                    option.add_compound(err, name, values, value);
                } break;
                case TYPE_A: {
                  t_values values;
                  t_bool init = false;
                  p = parse_array_values(err, values, init, params, p);
                  if (!values.empty())
                    option.add_array(err, name, values, params);
                  else
                    option.add_array(err, name, init, params);
                } break;
                case TYPE_L: {
                  p = parse_list(err, option.add_list(err, name),
                                 strip_space(p+1));
                } break;
                case TYPE_K:
                  option.add_openlist(err, name);
                  break;
                case TYPE_X: {
                  p = parse_lookup(err, option.add_lookup(err, name, params), p);
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

      P_cstr_ parse_arg(t_err err, t_lookup_ref lookup, P_cstr_ p) {
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
                    lookup.add_simple(err, name);
                  else
                    err = err::E_NOT_EMPTY;
                } break;
                case TYPE_MB:
                  lookup.add_boolean(err, name);
                  break;
                case TYPE_H: {
                  t_values values;
                  p = parse_selection_values(err, values, p);
                  lookup.add_selection(err, name, values);
                } break;
                case TYPE_C: {
                  t_values values;
                  t_value value;
                  p = parse_compound_values(err, values, value, p);
                  if (value.is_empty())
                    lookup.add_compound(err, name, values);
                  else
                    err = err::E_NOT_EMPTY;
                } break;
                case TYPE_A:{
                  t_values values;
                  t_bool init = false;
                  p = parse_array_values(err, values, init, params, p);
                  ERR_GUARD(err) {
                    if (!init && values.empty())
                      lookup.add_lookup(err, name, params);
                    else
                      err = err::E_NOT_EMPTY;
                  }
                } break;
                case TYPE_X: {
                  p = parse_lookup(err, lookup.add_lookup(err, name, params), p);
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
          for (; *p && !err; p = strip_space(p+1)) {
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
      t_void build_name(t_err err, t_text&         text,
                                    const t_name&   name,
                                    const t_type    type,
                                    const t_params& params) {
        ERR_GUARD(err) {
          if (params.optional_)
            text += ':';
          text += name;
        }
      }

      t_void build_simple(t_err err, t_text&       text,
                                      const t_name& name,
                                      t_simple_cref ref) {
        ERR_GUARD(err) {
          text += name;
          text += '=';
          if (!ref.get_value().is_empty())
            text += ref.get_value();
        }
      }

      t_void build_boolean(t_err err, t_text&        text,
                                       const t_name&  name,
                                       t_boolean_cref ref) {
        ERR_GUARD(err) {
          if (!ref.get_value())
            text += '!';
          text += name;
        }
      }

      t_void build_selection(t_err err, t_text&          text,
                                         const t_name&    name,
                                         t_selection_cref ref) {
        ERR_GUARD(err) {
          text += name;
          text += "=[";
          auto last = cend(ref.get_values());
          for (auto k = cbegin(ref.get_values());;) {
            text += *k++;
            if (k != last)
              text += '|';
            else
              break;
          }
          text += "]";
        }
      }

      t_void build_compound(t_err err, t_text&         text,
                                        const t_name&   name,
                                        t_compound_cref ref) {
        ERR_GUARD(err) {
          text += name;
          text += '=';
          if (!ref.get_value().is_empty())
            text += ref.get_value();
          for (auto&& part: ref.get_part_values()) {
            text += ':';
            text += part;
          }
        }
      }

      t_void build_array(t_err err, t_text&       text,
                                     const t_name& name,
                                     t_array_cref  ref) {
        ERR_GUARD(err) {
          t_rparams params(ref.get_range_params());
          text += name;
          text += '@';
          if (get(params.range_min_) != get(params.range_max_)) {
            text += std::to_string(get(params.range_min_));
            text += '-';
          }
          if (!get(params.range_max_))
            text += "n=";
          else {
            text += std::to_string(get(params.range_max_));
            text += "=";
          }
          if (!ref.get_values().empty()) {
            text += '[';
            auto last = cend(ref.get_values());
            for (auto k = cbegin(ref.get_values());;) {
              text += *k++;
              if (k != last)
                text += ',';
              else
                break;
            }
            text += ']';
          } else if (ref.is_initialized())
            text += "[]";
        }
      }

      t_void build_lookup_values(t_err err, t_text&       text,
                                             t_lookup_cref lookup) {
        ERR_GUARD(err) {
          if (lookup.is_initialized() || get(lookup.size_value())) { // not efficient
            text += '[';
            if (get(lookup.size_value())) { // empty -XXX
              t_lookup_value_cref value{lookup.cbegin_value()};
              do {
                text += value.get_name();
                text += ":{";
                auto max = get(value.get_size());
                for (decltype(max) ix = 0; ix < max;) {
                  t_cref arg{value[t_ix{ix}]};
                  switch (arg.get_base_type()) {
                    case TYPE_S: {
                      t_simple_cref ref(arg);
                      text += ref.get_value();
                    } break;
                    case TYPE_MB: {
                      t_boolean_cref ref(arg);
                      if (!ref.get_value())
                        text += '!';
                      text += ref.get_name();
                    } break;
                    case TYPE_H: {
                      t_selection_cref ref(arg);
                      text += ref.get_value();
                    } break;
                    case TYPE_X: {
                      build_lookup_values(err, text, arg);
                    } break;
                    default: err = err::E_UNKNOWN;
                      break;
                  }
                  ERR_GUARD(err) {
                    if (++ix < max)
                      text += ',';
                  } else
                    break;
                }
                text += '}';
                value = lookup.cnext_value(value);
                if (value)
                  text += ',';
              } while (!err && value);
            }
            text += ']';
          }
        }
      }

      template<typename P> t_void build_args(t_err, t_text&, const P, char);

      t_void build_lookup(t_err err, r_text text, R_name name,
                          t_lookup_cref lookup) {
        ERR_GUARD(err) {
          t_rparams params(lookup.get_range_params());
          text += name;
          text += '@';
          if (get(params.range_min_)) {
            text += std::to_string(get(params.range_min_));
            text += '-';
          }
          if (!get(params.range_max_))
            text += "n{";
          else {
            text += std::to_string(get(params.range_max_));
            text += "{";
          }
          build_args(err, text, lookup, ',');
          ERR_GUARD(err) {
            text += "}=";
            build_lookup_values(err, text, lookup);
          }
        }
      }

      t_void build_openlist(t_err err, t_text&         text,
                                        const t_name&   name,
                                        t_openlist_cref openlist) {
        ERR_GUARD(err) {
          text += name;
          text += "=(*)";
        }
      }

      t_void build_option(t_err err, t_text&       text,
                                      const t_name& name,
                                      t_options_cref option) {
        ERR_GUARD(err) {
          text += option.get_extension();
          text += "(";
          build_args(err, text, option, '|');
          ERR_GUARD(err) {
            text += ')';
          }
        }
      }

      t_void build_list(t_err err, t_text&       text,
                                    const t_name& name,
                                    t_list_cref   list) {
        ERR_GUARD(err) {
          text += name;
          text += "=(";
          build_args(err, text, list, ',');
          ERR_GUARD(err) {
            text += ')';
          }
        }
      }

      t_void build_list(t_err err, t_text& text, t_list_cref list) {
        ERR_GUARD(err) {
          text += '(';
          build_args(err, text, list, ',');
          ERR_GUARD(err) {
            text += ')';
          }
        }
      }

      t_void build_arg(t_err err, t_text&       text,
                                   t_cref        ref,
                                   t_options_cref option) {
        ERR_GUARD(err) {
          t_type type = ref.get_type();
          if (is_optional(type))
            text += ':';
          const t_name& extname = option.get_extension();
          const t_name& argname = ref.get_name();
          t_name name;
          if (extname.is_empty())
            name += argname;
          else {
            name += '<';
            ///name.append(argname.(extname.length() - 1); // XXX
          }
          switch (get_base_type(type)) {
            case TYPE_S:
              build_simple(err, text, name, ref);
              break;
            case TYPE_MB:
              build_boolean(err, text, name, ref);
              break;
            case TYPE_A:
              build_array(err, text, name, ref);
              break;
            case TYPE_C:
              build_compound(err, text, name, ref);
              break;
            case TYPE_H:
              build_selection(err, text, name, ref);
              break;
            case TYPE_K:
              build_openlist(err, text, name, ref);
              break;
            case TYPE_L:
              build_list(err, text, name, ref);
              break;
            case TYPE_X:
              build_lookup(err, text, name, ref);
              break;
            default: err = err::E_UNKNOWN;
              break;
          }
        }
      }

      t_void build_arg(t_err err, t_text& text, t_cref ref, t_lookup_cref) {
        ERR_GUARD(err) {
          t_type type = ref.get_type();
          if (is_optional(type))
            text += ':';
          const t_name& name = ref.get_name();
          switch (get_base_type(type)) {
            case TYPE_S:
              build_simple(err, text, name, ref);
              break;
            case TYPE_MB:
              build_boolean(err, text, name, ref);
              break;
            case TYPE_A:
              build_array(err, text, name, ref);
              break;
            case TYPE_C:
              build_compound(err, text, name, ref);
              break;
            case TYPE_H:
              build_selection(err, text, name, ref);
              break;
            case TYPE_X:
              build_lookup(err, text, name, ref);
              break;
            default: err = err::E_UNKNOWN;
              break;
          }
        }
      }

      t_void build_arg(t_err err, t_text& text, t_cref ref, t_list_cref) {
        ERR_GUARD(err) {
          t_type type = ref.get_type();
          if (is_optional(type))
            text += ':';
          const t_name& name = ref.get_name();
          switch (get_base_type(type)) {
            case TYPE_S:
              build_simple(err, text, name, ref);
              break;
            case TYPE_MB:
              build_boolean(err, text, name, ref);
              break;
            case TYPE_A:
              build_array(err, text, name, ref);
              break;
            case TYPE_C:
              build_compound(err, text, name, ref);
              break;
            case TYPE_H:
              build_selection(err, text, name, ref);
              break;
            case TYPE_K:
              build_openlist(err, text, name, ref);
              break;
            case TYPE_L:
              build_list(err, text, name, ref);
              break;
            case TYPE_Z:
              build_option(err, text, name, ref);
              break;
            case TYPE_X:
              build_lookup(err, text, name, ref);
              break;
            default: err = err::E_UNKNOWN;
              break;
          }
        }
      }

      template<typename P>
      t_void build_args(t_err err, t_text& text, P parent, char delimit) {
        ERR_GUARD(err) {
          if (!parent.is_empty()) {
            auto max = get(parent.get_size());
            decltype(max) ix = 0;
            do {
              build_arg(err, text, parent[t_ix{ix}], parent);
              if (++ix < max)
                text += delimit;
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
              case TYPE_L:
              case TYPE_MB:
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
              case TYPE_A: {
                t_array_cref array(use_ref);
                if (array.get_values().empty())
                  err = err::E_EMPTY;
              } break;
              case TYPE_X: {
                t_lookup_cref lookup(use_ref); /// XXX  - empty
                if (!get(lookup.size_value()) && !lookup.is_initialized())
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

      t_bool found_and_swap_use(t_err err, t_list_ref use, // don't need references
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

      t_bool insert_default_use(t_err err, t_list_ref&    use,
                                            t_ix::t_value& use_ix,
                                            t_list_cref    def,
                                            t_cref         def_ref) {
        ERR_GUARD(err) {
          const t_type def_type = def_ref.get_type();
          if (is_optional(def_type)) {
            switch (def_type) {
              case TYPE_OK:
              case TYPE_OZ:
              case TYPE_OB:
              case TYPE_OH:
              case TYPE_OL:
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
              case TYPE_OA: {
                t_array_cref def_array(def_ref);
                t_params params(t_oparams(true),
                                      def_array.get_range_params());
                t_bool insert = false;
                auto max = get(use.get_size());
                if (!def_array.get_values().empty()) {
                  use.add_array(err, def_ref.get_name(),
                                     def_array.get_values(),
                                     params);
                  insert = true;
                } else if (def_array.is_initialized()) {
                  use.add_array(err, def_ref.get_name(), true, params);
                  insert = true;
                }
                if (!err && insert) {
                  if (use_ix < max)
                    use.swap(err, t_ix{use_ix}, t_ix{max-1});
                  ++use_ix;
                }
              } break;
              case TYPE_OX: {
                t_lookup_cref def_lookup(def_ref);
                auto max = get(use.get_size());
                if ((!def_lookup.is_empty() || def_lookup.is_initialized()) &&
                    append(err, use, def_lookup)) {
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

      t_void check_mandatory(t_err, t_list_ref, t_ref, t_cref);

      t_bool merge_list(t_err err, t_list_ref use, t_list_cref def) {
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

      t_void check_mandatory(t_err err, t_list_ref use, t_ref use_ref,
                                        t_cref def_ref) {
        ERR_GUARD(err) {
          const t_type def_type = def_ref.get_type();
          switch (def_type) {
            case TYPE_OK:
            case TYPE_K:
            case TYPE_MB:
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
            case TYPE_OA:
            case TYPE_A: {
              t_array_cref def_array(def_ref), use_array(use_ref);
              if (def_array.get_range_params() ==
                  use_array.get_range_params()) {
                if (!is_optional(def_type) &&
                    !def_array.get_values().empty() &&
                     def_array.get_values() != use_array.get_values())
                  err = err::E_NO_MATCH;
              } else
                err = err::E_INVALID_ARG;
            } break;
            case TYPE_OL:
            case TYPE_L: {
              merge_list(err, use_ref, def_ref);
            } break;
            case TYPE_OZ:
            case TYPE_Z: {
              t_options_cref option(def_ref);
              // don't know what to do. - XXX
              // can search through use for all options
              //check in use
              // type might need to know its an option
            } break;
            case TYPE_OX:
            case TYPE_X: {
              t_lookup_cref def_lookup(def_ref), use_lookup(use_ref);
              if (is_same_definition(def_lookup, use_lookup, true)) {
                if (!is_optional(def_type) &&
                    !is_same(def_lookup, use_lookup))
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

  namespace argn
  {
    t_bool check_empty(t_err err, t_word& word) {
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
      return true;
    }

    t_bool check_name(t_err err, t_name& name) {
      ERR_GUARD(err) {
        if (!check_empty(err, name)) {
          return true;
        }
      }
      return false;
    }

    t_bool check_value(t_err err, t_value& value) {
      ERR_GUARD(err) {
        if (!check_empty(err, value)) {
          return true;
        }
      }
      return false;
    }

    t_bool check_values(t_err err, t_values& values) {
      ERR_GUARD(err) {
        if (!values.empty()) {
          for (auto&& value : values) {
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
          auto last = cend(values);
          for (auto&& value : values)
            if (std::find_if(cbegin(values), last,
                             [&value](const auto& v) {
                               return &value != &v && value == v;}) != last)
              return false;
          return true;
        }
      }
      return false;
    }

    t_bool check_selection_value(t_err err, t_value& value,
                                 const t_values& values) {
      ERR_GUARD(err) {
        if (!check_empty(err, value)) {
          auto end = std::cend(values);
          return std::find(std::cbegin(values), end, value) != end;
        }
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

   t_void print(named::P_cstr_ prefix, const t_words& words) {
      t_word word("[");
      auto max = words.size();
      for (decltype(max) i = 0; i < max; ++i) {
        word += words[i];
        if (i < max - 1)
          word += ',';
      }
      word += "]";
      std::cout << prefix << get(word.get_cstr()) << std::endl;
    }

    t_bool match(const argn::t_fullname& maybe, const argn::t_fullname& name,
                 unsigned n) {
      auto m = maybe.size();
      if (m >= n) {
        for (decltype(m) ix = 0; ix < n; ++ix)
          if (maybe[ix] != name[ix])
            return false;
        return true;
      }
      return false;
    }

    // not sure how to do this yet - efficiency
    t_bool arg_compare::operator()(const t_fullname& lh,
                                   const t_fullname& rh) const {
      auto lhs = lh.size();
      auto rhs = rh.size();
      auto min = lhs < rhs ? lhs : rhs;
      for (decltype(min) i = 0; i < min; ++i) { // comparson missing because of <>
        auto lh_range = lh[i].mk_range();
        auto rh_range = rh[i].mk_range();

        if (lh_range[t_ix{0}] == '/')
          return false;
        if (rh_range[t_ix{0}] == '/')
          return true;
        if (lh_range[t_ix{0}] == '.' && rh_range[t_ix{0}] != '.')
          return true;
        if (lh_range[t_ix{0}] != '.' && rh_range[t_ix{0}] == '.')
          return false;

        t_name l;
        if (lh_range[t_ix{0}] == '<')
          l = mk_range(lh_range, t_ix{1}, t_ix{get(lh_range.n) - 2});
        else
          l = lh_range;

        t_name r;
        if (rh_range[t_ix{0}] == '<')
          l = mk_range(rh_range, t_ix{1}, t_ix{get(rh_range.n) - 2});
        else
          l = rh_range;

        if (l < r)
          return true;
        if (l > r)
          return false;
      }
      return lhs < rhs;
    }

////////////////////////////////////////////////////////////////////////////////

    t_ref::t_ref(t_err err, const t_ref& ref)
      : argn_(ref.argn_), arg_(ref.arg_) {
      if (err)
        clear_();
      else if (!is_valid_())
        err = err::E_INVALID_REF;
    }

    t_type t_ref::get_type() const {
      if (is_valid_())
        return get_().second.info_.type_;
      return TYPE_Q;
    }

    t_type t_ref::get_base_type() const {
      if (is_valid_())
        return argn::get_base_type(get_().second.info_.type_);
      return TYPE_Q;
    }

    const t_fullname& t_ref::get_fullname() const {
      if (is_valid_())
        return get_().first;
      return empty_fullname;
    }

    const t_name& t_ref::get_name() const {
      if (is_valid_())
        return get_().first.back();
      return empty_name;
    }

    t_oparams t_ref::get_optional_params() const {
      if (is_valid_())
        return {is_optional(get_().second.info_.type_)};
      return {false};
    }

////////////////////////////////////////////////////////////////////////////////

    t_cref::t_cref(t_err err, const t_cref& ref)
      : argn_(ref.argn_), arg_(ref.arg_) {
      if (err)
        clear_();
      else if (!is_valid_())
        err = err::E_INVALID_REF;
    }

    t_type t_cref::get_type() const {
      if (is_valid_())
        return get_().second.info_.type_;
      return TYPE_Q;
    }

    t_type t_cref::get_base_type() const {
      if (is_valid_())
        return argn::get_base_type(get_().second.info_.type_);
      return TYPE_Q;
    }

    const t_fullname& t_cref::get_fullname() const {
      if (is_valid_())
        return get_().first;
      return empty_fullname;
    }

    const t_name& t_cref::get_name() const {
      if (is_valid_())
        return get_().first.back();
      return empty_name;
    }

    t_oparams t_cref::get_optional_params() const {
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
                                                  t_values values) {
      ERR_GUARD(err) {
        if (is_valid_())
          compound = set_argn_().transform_(err, *this, std::move(values));
        else
          err = err::E_INVALID_REF;
      }
      return !err;
    }

    const t_value& t_simple_ref::get_value() const {
      if (is_valid_()) {
        if (!get_().second.values_.empty())
          return get_().second.values_.front();
      }
      return empty_value;
    }

    t_bool t_simple_ref::set_value(t_err err, t_value value) {
      if (check_value(err, value)) {
        if (is_valid_()) {
          if (get_().second.values_.empty())
            set_().second.values_.push_back(std::move(value));
          else
            set_().second.values_[0] = std::move(value);
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

    const t_value& t_simple_cref::get_value() const {
      if (is_valid_()) {
        if (!get_().second.values_.empty())
          return get_().second.values_.front();
      }
      return empty_value;
    }

////////////////////////////////////////////////////////////////////////////////

    t_boolean_ref::t_boolean_ref(t_ref ref) : t_ref(ref) {
      if (is_valid_() && get_base_type() != TYPE_MB)
        clear_();
    }

    t_boolean_ref::t_boolean_ref(t_err err, t_ref ref) : t_ref(err, ref) {
      if (!err && get_base_type() != TYPE_MB) {
        clear_();
        err.set(mk_cstr("id is not a boolean argument"));
      }
    }

    t_bool t_boolean_ref::get_value() const {
      if (is_valid_())
        return get_().second.values_.front() == TRUE_s;
      return false;
    }

    t_bool t_boolean_ref::set_value(t_err err, t_bool state) {
      ERR_GUARD(err) {
       if (is_valid_()) {
         if (state || get_type() == TYPE_MB)
           set_().second.values_[0] = state ? TRUE_s : FALSE_s;
         else
           err.set(mk_cstr("boolean value doesn't match type"));
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
        err.set(mk_cstr("id is not a boolean argument"));
      }
    }

    t_bool t_boolean_cref::get_value() const {
      if (is_valid_())
        return get_().second.values_.front() == TRUE_s;
      return false;
    }

////////////////////////////////////////////////////////////////////////////////

    t_array_ref::t_array_ref(t_ref ref) : t_ref(ref) {
      if (is_valid_() && get_base_type() != TYPE_A)
        clear_();
    }

    t_array_ref::t_array_ref(t_err err, t_ref ref) : t_ref(err, ref) {
      if (!err && get_base_type() != TYPE_A) {
        clear_();
        err.set(mk_cstr("id is not an array argument"));
      }
    }

    t_rparams t_array_ref::get_range_params() const {
      if (is_valid_())
        return {t_n{get_().second.info_.range_.max_},
                t_n{get_().second.info_.range_.min_}};
      return {};
    }

    t_bool t_array_ref::set_as_initialized() {
      if (is_valid_()) {
        if (!get_().second.info_.range_.cnt_) {
          set_().second.info_.range_.init_ = true;
          return true;
        }
      }
      return false;
    }

    t_bool t_array_ref::is_initialized() const {
      if (is_valid_())
        return get_().second.info_.range_.init_;
      return false;
    }

    const t_values& t_array_ref::get_values() const {
      if (is_valid_())
        return get_().second.values_;
      return empty_values_v;
    }

    t_bool t_array_ref::set_values(t_err err, t_values values) {
      if (check_values(err, values)) {
        if (is_valid_())
          set_().second.values_ = std::move(values);
        else
          err = err::E_INVALID_REF;
      }
      return !err;
    }

////////////////////////////////////////////////////////////////////////////////

    t_array_cref::t_array_cref(t_cref ref) : t_cref(ref) {
      if (is_valid_() && get_base_type() != TYPE_A)
        clear_();
    }

    t_array_cref::t_array_cref(t_err err, t_cref ref) : t_cref(err, ref) {
      if (!err && get_base_type() != TYPE_A) {
        clear_();
        err.set(mk_cstr("id is not an array argument"));
      }
    }

    t_rparams t_array_cref::get_range_params() const {
      if (is_valid_())
        return {t_n{get_().second.info_.range_.max_},
                t_n{get_().second.info_.range_.min_}};
      return {};
    }

    t_bool t_array_cref::is_initialized() const {
      if (is_valid_())
        return get_().second.info_.range_.init_;
      return false;
    }

    const t_values& t_array_cref::get_values() const {
      if (is_valid_())
        return get_().second.values_;
      return empty_values_v;
    }

////////////////////////////////////////////////////////////////////////////////

    t_compound_ref::t_compound_ref(t_ref ref) : t_ref(ref) {
      if (is_valid_() && get_base_type() != TYPE_C)
        clear_();
    }

    t_compound_ref::t_compound_ref(t_err err, t_ref ref) : t_ref(err, ref) {
      if (!err && get_base_type() != TYPE_C) {
        clear_();
        err.set(mk_cstr("id is not a compound argument"));
      }
    }

    t_bool t_compound_ref::add_missing_part_values(t_err err,
                                                   const t_values& values) {
      ERR_GUARD(err) {
        if (is_valid_()) {
          auto max = get_().second.values_.size();
          auto nmax = values.size();
          if (nmax > max) {
            for (auto ix = max; ix < nmax; ++ix)
              set_().second.values_.push_back(values[ix]);
          } else if (nmax < max)
            err.set(mk_cstr("invalid number of parts"));
        } else
          err = err::E_INVALID_REF;
      }
      return false;
    }

    const t_values& t_compound_ref::get_part_values() const {
      if (is_valid_())
        return get_().second.values_;
      return empty_values_v;
    }

    const t_value& t_compound_ref::get_value() const {
      if (is_valid_())
        return get_().second.info_.ext_;
      return empty_value_v;
    }

    t_bool t_compound_ref::set_value(t_err err, t_value value) {
      if (check_value(err, value)) {
        if (is_valid_())
          set_().second.info_.ext_ = std::move(value);
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
        err.set(mk_cstr("id is not a compound argument"));
      }
    }

    const t_values& t_compound_cref::get_part_values() const {
      if (is_valid_())
        return get_().second.values_;
      return empty_values_v;
    }

    const t_value& t_compound_cref::get_value() const {
      if (is_valid_())
        return get_().second.info_.ext_;
      return empty_value_v;
    }

////////////////////////////////////////////////////////////////////////////////

    t_selection_ref::t_selection_ref(t_ref ref) : t_ref(ref) {
      if (is_valid_() && get_base_type() != TYPE_H)
        clear_();
    }

    t_selection_ref::t_selection_ref(t_err err, t_ref ref) : t_ref(err, ref) {
      if (!err && get_base_type() != TYPE_H) {
        clear_();
        err.set(mk_cstr("id is not a selection argument"));
      }
    }

    const t_values& t_selection_ref::get_values() const {
      if (is_valid_())
        return get_().second.values_;
      return empty_values_v;
    }

    const t_value& t_selection_ref::get_value() const {
      if (is_valid_())
        return get_().second.info_.ext_;
      return empty_value_v;
    }

    t_bool t_selection_ref::set_value(t_err err, t_value value) {
      if (is_valid_()) {
        if (check_selection_value(err, value, get_().second.values_))
          set_().second.info_.ext_ = std::move(value);
      } else if (!err)
        err = err::E_INVALID_REF;
      return !err;
    }

    t_bool t_selection_ref::test_value(t_err err, t_value value) const {
      if (check_value(err, value)) {
        if (is_valid_())
          return std::find(std::cbegin(get_().second.values_),
                           std::cend(get_().second.values_), value) !=
                 std::cend(get_().second.values_);
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
        err.set(mk_cstr("id is not a selection argument"));
      }
    }

    const t_values& t_selection_cref::get_values() const {
      if (is_valid_())
        return get_().second.values_;
      return empty_values_v;
    }

    const t_value& t_selection_cref::get_value() const {
      if (is_valid_())
        return get_().second.info_.ext_;
      return empty_value_v;
    }

    t_bool t_selection_cref::test_value(t_err err, t_value value) const {
      if (check_value(err, value)) {
        if (is_valid_())
          return std::find(std::cbegin(get_().second.values_),
                           std::cend(get_().second.values_), value) !=
                 std::cend(get_().second.values_);
        err = err::E_INVALID_REF;
      }
      return !err;
    }

////////////////////////////////////////////////////////////////////////////////

    t_openlist_ref::t_openlist_ref(t_ref ref) : t_ref(ref) {
      if (is_valid_() && get_base_type() != TYPE_K)
        clear_();
    }

    t_openlist_ref::t_openlist_ref(t_err err, t_ref ref) : t_ref(err, ref) {
      if (!err && get_base_type() != TYPE_K) {
        clear_();
        err.set(mk_cstr("id is not a openlist argument"));
      }
    }

////////////////////////////////////////////////////////////////////////////////

    t_openlist_cref::t_openlist_cref(t_cref ref) : t_cref(ref) {
      if (is_valid_() && get_base_type() != TYPE_K)
        clear_();
    }

    t_openlist_cref::t_openlist_cref(t_err err, t_cref ref) : t_cref(err, ref) {
      if (!err && get_base_type() != TYPE_K) {
        clear_();
        err.set(mk_cstr("id is not a openlist argument"));
      }
    }

////////////////////////////////////////////////////////////////////////////////

    t_collection_ref::t_collection_ref(t_ref ref) : t_ref(ref) {
      if (is_valid_()) {
        const t_type base = get_base_type();
        if (base != TYPE_L && base != TYPE_X && base != TYPE_Z && base != TYPE_XI)
          clear_();
      }
    }

    t_collection_ref::t_collection_ref(t_err err, t_ref ref)
        : t_ref(err, ref) {
      ERR_GUARD(err) {
        const t_type base = get_base_type();
        if (base != TYPE_L && base != TYPE_X && base != TYPE_Z && base != TYPE_XI) {
          clear_();
          err.set(mk_cstr("not a collection argument"));
        }
      }
    }

    t_ref t_collection_ref::operator[](t_ix ix) {
      if (is_valid_())
        return make_ref_((p_arg)set_().second.info_.mem_[get(ix)]);
      return {};
    }

    t_cref t_collection_ref::operator[](t_ix ix) const {
      if (is_valid_())
        return make_id_((p_carg)get_().second.info_.mem_[get(ix)]);
      return {};
    }

    t_ref t_collection_ref::operator[](P_cstr name) {
      if (is_valid_()) {
        auto max = get_().second.info_.mem_.size();
        for (decltype(max) ix = 0; ix < max; ++ix) {
          p_arg arg = (p_arg)set_().second.info_.mem_[ix];
          if (!std::strcmp(arg->first.back().get_cstr(), get(name)))
            return make_ref_(arg);
        }
      }
      return {};
    }

    t_cref t_collection_ref::operator[](P_cstr name) const {
      if (is_valid_()) {
        auto max = get_().second.info_.mem_.size();
        for (decltype(max) ix = 0; ix < max; ++ix) {
          p_carg arg = (p_carg)get_().second.info_.mem_[ix];
          if (!std::strcmp(arg->first.back().get_cstr(), get(name)))
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
        if (base != TYPE_L && base != TYPE_X && base != TYPE_Z && base != TYPE_XI)
          clear_();
      }
    }

    t_collection_cref::t_collection_cref(t_err err, t_cref ref) : t_cref(err, ref) {
      ERR_GUARD(err) {
        t_type base = get_base_type();
        if (base != TYPE_L && base != TYPE_X && base != TYPE_Z && base != TYPE_XI) {
          clear_();
          err.set(mk_cstr("not a collection argument"));
        }
      }
    }

    t_cref t_collection_cref::operator[](t_ix ix) const {
      if (is_valid_())
        return make_ref_((p_carg)get_().second.info_.mem_[get(ix)]);
      return {};
    }

    t_cref t_collection_cref::operator[](P_cstr name) const {
      if (is_valid_()) {
        auto max = get_().second.info_.mem_.size();
        for (decltype(max) ix = 0; ix < max; ++ix) {
          p_carg arg = (p_carg)get_().second.info_.mem_[ix];
          if (!std::strcmp(arg->first.back().get_cstr(), get(name)))
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

    t_list_ref::t_list_ref(t_ref ref) : t_collection_ref(ref) {
      if (is_valid_() && get_base_type() != TYPE_L)
        clear_();
    }

    t_list_ref::t_list_ref(t_err err, t_ref ref) : t_collection_ref(err, ref) {
      if (!err && get_base_type() != TYPE_L) {
        clear_();
        err.set(mk_cstr("id is not a list argument"));
      }
    }

    t_bool t_list_ref::swap(t_err err, t_ix first, t_ix second) {
      ERR_GUARD(err) {
        if (is_valid_()) {
          auto max = get(to_ix(get_size()));
          decltype(max) one = get(first), two = get(second);
          if (one < max && two < max) {
            named::p_void tmp = set_().second.info_.mem_[one];
            set_().second.info_.mem_[one] = set_().second.info_.mem_[two];
            set_().second.info_.mem_[two] = tmp;
          } else
            err.set(mk_cstr("not in scope"));
        } else
          err = err::E_INVALID_REF;
      }
      return !err;
    }

    t_ref t_list_ref::add(t_err err, P_cstr cstr) {
      ERR_GUARD(err) {
        if (is_valid_()) {
          named::P_cstr_ p = get(cstr);
          p = syntax::parse::parse_arg(err, *this, p);
          ERR_GUARD(err) { // XXX
            if (*p == '\0')
              return {}; // can write the back
            err.set(mk_cstr("not terminated correctly"));
          }
        } else
          err = err::E_INVALID_REF;
      }
      return {};
    }

    t_ref t_list_ref::add_simple(t_err err, t_name name,
                                             const t_oparams& params) {
      ERR_GUARD(err) {
        if (is_valid_())
          return set_argn_().add_simple_(err, *this, std::move(name), params);
        err = err::E_INVALID_REF;
      }
      return {};
    }

    t_ref t_list_ref::add_simple(t_err err, t_name name, t_value value,
                                             const t_oparams& params) {
      ERR_GUARD(err) {
        if (is_valid_())
          return set_argn_().add_simple_(err, *this, std::move(name),
                                         std::move(value), params);
        err = err::E_INVALID_REF;
      }
      return {};
    }

    t_ref t_list_ref::add_boolean(t_err err, t_name name, t_bool state,
                                              const t_oparams& params) {
      ERR_GUARD(err) {
        if (is_valid_())
          return set_argn_().add_boolean_(err, *this, std::move(name),
                                          state, params);
        err = err::E_INVALID_REF;
      }
      return {};
    }

    t_ref t_list_ref::add_compound(t_err err, t_name name, t_values values,
                                               const t_oparams& params) {
      ERR_GUARD(err) {
        if (is_valid_())
          return set_argn_().add_compound_(err, *this, std::move(name),
                                           std::move(values), params);
        err = err::E_INVALID_REF;
      }
      return {};
    }

    t_ref t_list_ref::add_compound(t_err err, t_name name, t_values values,
                                               t_value value,
                                               const t_oparams& params) {
      ERR_GUARD(err) {
        if (is_valid_())
          return set_argn_().add_compound_(err, *this, std::move(name),
                                           std::move(values),
                                           std::move(value), params);
        err = err::E_INVALID_REF;
      }
      return {};
    }

    t_ref t_list_ref::add_array(t_err err, t_name name, t_bool init,
                                            const t_params& params) {
      ERR_GUARD(err) {
        if (is_valid_())
          return set_argn_().add_array_(err, *this, std::move(name), init,
                                        params);
        err = err::E_INVALID_REF;
      }
      return {};
    }

    t_ref t_list_ref::add_array(t_err err, t_name name, t_values values,
                                            const t_params& params) {
      ERR_GUARD(err) {
        if (is_valid_())
          return set_argn_().add_array_(err, *this, std::move(name),
                                        std::move(values), params);
        err = err::E_INVALID_REF;
      }
      return {};
    }

    t_ref t_list_ref::add_selection(t_err err, t_name name, t_values values,
                                                const t_oparams& params) {
      ERR_GUARD(err) {
        if (is_valid_())
          return set_argn_().add_selection_(err, *this, std::move(name),
                                            std::move(values), params);
        err = err::E_INVALID_REF;
      }
      return {};
    }

    t_ref t_list_ref::add_list(t_err err, t_name name,
                                           const t_oparams& params) {
      ERR_GUARD(err) {
        if (is_valid_())
          return set_argn_().add_list_(err, *this, std::move(name), params);
        err = err::E_INVALID_REF;
      }
      return {};
    }

    t_ref t_list_ref::add_openlist(t_err err, t_name name,
                                               const t_oparams& params) {
      ERR_GUARD(err) {
        if (is_valid_())
          return set_argn_().add_openlist_(err, *this, std::move(name),
                                           params);
        err = err::E_INVALID_REF;
      }
      return {};
    }

    t_ref t_list_ref::add_option(t_err err, t_name name,
                                             const t_oparams& params) {
      ERR_GUARD(err) {
        if (is_valid_())
          return set_argn_().add_option_(err, *this, std::move(name),
                                         params);
        err = err::E_INVALID_REF;
      }
      return {};
    }

    t_ref t_list_ref::add_lookup(t_err err, t_name name,
                                             const t_params& params) {
      ERR_GUARD(err) {
        if (is_valid_())
           return set_argn_().add_lookup_(err, *this, std::move(name),
                                          params);
        err = err::E_INVALID_REF;
      }
      return {};
    }

    t_bool t_list_ref::del(const t_name& name) {
      if (is_valid_())
        return set_argn_().del_(*this, name);
      return false;
    }

////////////////////////////////////////////////////////////////////////////////

    t_list_cref::t_list_cref(t_cref ref) : t_collection_cref(ref) {
      if (is_valid_() && get_base_type() != TYPE_L)
        clear_();
    }

    t_list_cref::t_list_cref(t_err err, t_cref ref) : t_collection_cref(err, ref) {
      if (!err && get_base_type() != TYPE_L) {
        clear_();
        err.set(mk_cstr("id is not a list argument"));
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
        err.set(mk_cstr("id is not a option argument"));
      }
    }

    const t_name& t_options_ref::get_extension() const {
      if (is_valid_())
        return get_().second.info_.ext_;
      return EMPTY_s;
    }

    t_ref t_options_ref::add(t_err err, P_cstr cstr) {
      ERR_GUARD(err) {
        if (is_valid_()) {
          if (is_empty()) { // looks wrong
            named::P_cstr_ p = get(cstr);
            p = syntax::parse::parse_option(err, *this, p);
            ERR_GUARD(err) {
              if (*p == '\0')
                return {};
              err.set(mk_cstr("not terminated correctly"));
            }
          } else
            err.set(mk_cstr("list must be empty"));
        } else
          err = err::E_INVALID_REF;
      }
      return {};
    }

    t_ref t_options_ref::add_simple(t_err err, t_name name) {
      ERR_GUARD(err) {
        if (is_valid_()) {
          const t_oparams params{false};
          return set_argn_().add_simple_(err, *this, std::move(name),
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
          return set_argn_().add_simple_(err, *this, std::move(name),
                                         std::move(value), params);
        }
        err = err::E_INVALID_REF;
      }
      return {};
    }

    t_ref t_options_ref::add_boolean(t_err err, t_name name, t_bool state) {
      ERR_GUARD(err) {
        if (is_valid_()) {
          const t_oparams params{false};
          return set_argn_().add_boolean_(err, *this, std::move(name), state,
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
          return set_argn_().add_compound_(err, *this, std::move(name),
                                           std::move(values), params);
        }
        err = err::E_INVALID_REF;
      }
      return {};
    }

    t_ref t_options_ref::add_compound(t_err err, t_name name,
                                                 t_values values,
                                                 t_value value) {
      ERR_GUARD(err) {
        if (is_valid_()) {
          const t_oparams params{false};
          return set_argn_().add_compound_(err, *this, std::move(name),
                                           std::move(values),
                                           std::move(value), params);
        }
        err = err::E_INVALID_REF;
      }
      return {};
    }

    t_ref t_options_ref::add_array(t_err err, t_name name, t_bool init,
                                              const t_rparams& rparams) {
      ERR_GUARD(err) {
        if (is_valid_()) {
          const t_params params{t_oparams{false}, rparams};
          return set_argn_().add_array_(err, *this, std::move(name), init,
                                        params);
        }
        err = err::E_INVALID_REF;
      }
      return {};
    }

    t_ref t_options_ref::add_array(t_err err, t_name name, t_values values,
                                              const t_rparams& rparams) {
      ERR_GUARD(err) {
        if (is_valid_()) {
          const t_params params{t_oparams{false}, rparams};
          return set_argn_().add_array_(err, *this, std::move(name),
                                        std::move(values), params);
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
          return set_argn_().add_selection_(err, *this, std::move(name),
                                            std::move(values), params);
        }
        err = err::E_INVALID_REF;
      }
      return {};
    }

    t_ref t_options_ref::add_list(t_err err, t_name name) {
      ERR_GUARD(err) {
        if (is_valid_()) {
          const t_oparams params{false};
          return set_argn_().add_list_(err, *this, std::move(name), params);
        }
        err = err::E_INVALID_REF;
      }
      return {};
    }

    t_ref t_options_ref::add_openlist(t_err err, t_name name) {
      ERR_GUARD(err) {
        if (is_valid_()) {
          const t_oparams params{false};
          return set_argn_().add_openlist_(err, *this, std::move(name),
                                           params);
        }
        err = err::E_INVALID_REF;
      }
      return {};
    }

    t_ref t_options_ref::add_lookup(t_err err, t_name name,
                                               const t_rparams& rparams) {
      ERR_GUARD(err) {
        if (is_valid_()) {
          const t_params params{t_oparams{false}, rparams};
          return set_argn_().add_lookup_(err, *this, std::move(name), params);
        }
        err = err::E_INVALID_REF;
      }
      return {};
    }

    t_bool t_options_ref::del(const t_name& name) {
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
        err.set(mk_cstr("id is not a option argument"));
      }
    }

    const t_name& t_options_cref::get_extension() const {
      if (is_valid_())
        return get_().second.info_.ext_;
      return EMPTY_s;
    }

////////////////////////////////////////////////////////////////////////////////

    t_lookup_ref::t_lookup_ref(t_ref ref) : t_collection_ref(ref) {
      if (is_valid_() && get_base_type() != TYPE_X)
        clear_();
    }

    t_lookup_ref::t_lookup_ref(t_err err, t_ref ref)
      : t_collection_ref(err, ref) {
      if (!err && get_base_type() != TYPE_X) {
        clear_();
        err.set(mk_cstr("id is not a lookup argument"));
      }
    }

    t_rparams t_lookup_ref::get_range_params() const {
      if (is_valid_())
        return {t_n{get_().second.info_.range_.max_},
                t_n{get_().second.info_.range_.min_}};
      return {};
    }

    t_cref t_lookup_ref::add_simple(t_err err, t_name name) {
      ERR_GUARD(err) {
        if (is_valid_()) {
          const t_oparams params{false};
          return set_argn_().add_simple_(err, *this, std::move(name), params);
        }
        err = err::E_INVALID_REF;
      }
      return {};
    }

    t_cref t_lookup_ref::add_boolean(t_err err, t_name name) {
      ERR_GUARD(err) {
        if (is_valid_()) {
          const t_oparams params{false};
          return set_argn_().add_boolean_(err, *this, std::move(name), false,
                                          params);
        }
        err = err::E_INVALID_REF;
      }
      return {};
    }

    t_cref t_lookup_ref::add_compound(t_err err, t_name name,
                                                  t_values values) {
      ERR_GUARD(err) {
        if (is_valid_()) {
          const t_oparams params{false};
          return set_argn_().add_compound_(err, *this, std::move(name),
                                           std::move(values), params);
        }
        err = err::E_INVALID_REF;
      }
      return {};
    }

    t_cref t_lookup_ref::add_selection(t_err err, t_name name,
                                                   t_values values) {
      ERR_GUARD(err) {
        if (is_valid_()) {
          const t_oparams params{false};
          return set_argn_().add_selection_(err, *this, std::move(name),
                                           std::move(values), params);
        }
        err = err::E_INVALID_REF;
      }
      return {};
    }

    t_cref t_lookup_ref::add_array(t_err err, t_name name,
                                               const t_rparams& rparams) {
      ERR_GUARD(err) {
        if (is_valid_()) {
          const t_params params{t_oparams{false}, rparams};
          return set_argn_().add_array_(err, *this, std::move(name), true,
                                        params);
        }
        err = err::E_INVALID_REF;
      }
      return {};
    }

    t_ref t_lookup_ref::add_lookup(t_err err, t_name name,
                                               const t_rparams& rparams) {
      ERR_GUARD(err) {
        if (is_valid_()) {
          const t_params params{t_oparams{false}, rparams};
          return set_argn_().add_lookup_(err, *this, std::move(name), params);
        }
        err = err::E_INVALID_REF;
      }
      return {};
    }

    t_bool t_lookup_ref::set_as_initialized() {
      if (is_valid_()) {
        if (!get_().second.info_.range_.cnt_) {
          set_().second.info_.range_.init_ = true;
          return true;
        }
      }
      return false;
    }

    t_bool t_lookup_ref::is_initialized() const {
      if (is_valid_())
        return get_().second.info_.range_.init_;
      return false;
    }

    t_ref t_lookup_ref::add_value(t_err err, t_name name) {
      ERR_GUARD(err) {
        if (is_valid_())
          return set_argn_().add_lookup_value_(err, *this, std::move(name));
        err = err::E_INVALID_REF;
      }
      return {};
    }

    t_bool t_lookup_ref::del_value(const t_name& name) {
      if (is_valid_())
        return set_argn_().del_lookup_value_(*this, name);
      return false;
    }

    t_ref t_lookup_ref::get_value(P_cstr name) {
      if (is_valid_())
        return set_argn_().get_lookup_value_(*this, name);
      return {};
    }

    t_cref t_lookup_ref::get_value(P_cstr name) const {
      if (is_valid_())
        return get_argn_().get_lookup_value_(*this, name);
      return {};
    }

    t_void t_lookup_ref::clear_value() {
      if (is_valid_())
        set_argn_().clr_lookup_value_(*this);
    }

    t_n t_lookup_ref::size_value() const {
      if (is_valid_())
        return t_n{get_().second.info_.range_.cnt_};
      return t_n{0};
    }

    t_ref t_lookup_ref::begin_value() {
      if (get(size_value())) { // is_empty - XXX
        t_fullname endname(get_fullname());
        endname.push_back(".");
        endname.push_back("/");
        t_itr itr = set_argn_().table_.upper_bound(endname);
        if (itr != std::end(get_argn_().table_))
          return make_ref_(&(*itr));
      }
      return {};
    }

    t_cref t_lookup_ref::begin_value() const {
      return cbegin_value();
    }

    t_cref t_lookup_ref::cbegin_value() const {
      return t_lookup_cref(*this).cbegin_value();
    }

    t_ref t_lookup_ref::next_value(t_ref ref) {
      if (ref) {
        const t_fullname& refname = ref.get_fullname();
        t_fullname endname(refname);
        endname.push_back("/");
        t_itr itr = set_argn_().table_.upper_bound(endname);
        if (itr != std::cend(get_argn_().table_) &&
            match(itr->first, refname, refname.size() - 1))
          return make_ref_(&(*itr));
      }
      return {};
    }

    t_cref t_lookup_ref::next_value(t_ref ref) const {
      return cnext_value(ref);
    }

    t_cref t_lookup_ref::cnext_value(t_cref ref) const {
      return t_lookup_cref(*this).cnext_value(ref);
    }

////////////////////////////////////////////////////////////////////////////////

    t_lookup_cref::t_lookup_cref(t_cref ref) : t_collection_cref(ref) {
      if (is_valid_() && get_base_type() != TYPE_X)
        clear_();
    }

    t_lookup_cref::t_lookup_cref(t_err err, t_cref ref)
      : t_collection_cref(err, ref) {
      if (!err && get_base_type() != TYPE_X) {
        clear_();
        err.set(mk_cstr("id is not a lookup argument"));
      }
    }

    t_rparams t_lookup_cref::get_range_params() const {
      if (is_valid_())
        return {t_n{get_().second.info_.range_.max_},
                t_n{get_().second.info_.range_.min_}};
      return {};
    }

    t_bool t_lookup_cref::is_initialized() const {
      if (is_valid_())
        return get_().second.info_.range_.init_;
      return false;
    }

    t_cref t_lookup_cref::get_value(P_cstr name) const {
      if (is_valid_())
        return get_argn_().get_lookup_value_(*this, name);
      return {};
    }

    t_n t_lookup_cref::size_value() const {
      if (is_valid_())
        return t_n{get_().second.info_.range_.cnt_};
      return t_n{0};
    }


    t_cref t_lookup_cref::begin_value() const {
      return cbegin_value();
    }

    t_cref t_lookup_cref::cbegin_value() const {
      if (get(size_value())) { // is_empty - XXX
        t_fullname endname(get_fullname());
        endname.push_back(".");
        endname.push_back("/");
        t_citr itr = get_argn_().table_.upper_bound(endname);
        if (itr != std::cend(get_argn_().table_))
          return make_ref_(&(*itr));
      }
      return {};
    }

    t_cref t_lookup_cref::next_value(t_ref ref) const {
      return cnext_value(ref);
    }

    t_cref t_lookup_cref::cnext_value(t_cref ref) const {
      if (ref) {
        const t_fullname& refname = ref.get_fullname();
        t_fullname endname(refname);
        endname.push_back("/");
        t_citr itr = get_argn_().table_.upper_bound(endname);
        if (itr != std::cend(get_argn_().table_) &&
            match(itr->first, refname, refname.size() - 1))
          return make_ref_(&(*itr));
      }
      return {};
    }

////////////////////////////////////////////////////////////////////////////////

    t_lookup_value_ref::t_lookup_value_ref(t_ref ref) : t_collection_ref(ref) {
      if (is_valid_() && get_type() != TYPE_XI)
        clear_();
    }

    t_lookup_value_ref::t_lookup_value_ref(t_err err, t_ref ref)
      : t_collection_ref(err, ref) {
      if (!err && get_type() != TYPE_XI) {
        clear_();
        err.set(mk_cstr("id is not a lookup value argument"));
      }
    }

    t_lookup_value_cref::t_lookup_value_cref(t_cref ref)
      : t_collection_cref(ref) {
      if (is_valid_() && get_type() != TYPE_XI)
        clear_();
    }

    t_lookup_value_cref::t_lookup_value_cref(t_err err, t_cref ref)
      : t_collection_cref(err, ref) {
      if (!err && get_type() != TYPE_XI) {
        clear_();
        err.set(mk_cstr("id is not a lookup value argument"));
      }
    }

////////////////////////////////////////////////////////////////////////////////

    t_argn::t_argn(t_err err) {
      ERR_GUARD(err) {
        auto p = table_.insert({t_fullname{"</>"}, t_arginfo(TYPE_L)});
        if (p.second)
          p.first->second.path_.push_back(get(get_cstr(TYPE_L)));
        else
          err.set(mk_cstr("could not insert"));
      }
    }

    t_ref t_argn::add_(t_err err, t_ref       parent,
                                   t_name&&    name,
                                   t_arginfo& info) {
      ERR_GUARD(err) {
        t_fullname fullname(parent.get_fullname());
        t_type base = parent.get_base_type();
        t_type type = info.type_;
        if (base == TYPE_Z) {
          fullname.back() = name;
          if (!parent.get_().second.info_.ext_.is_empty()) {
            fullname.back().erase(0, 1);
            fullname.back().insert(0, parent.get_().second.info_.ext_.get_cstr(),
                                   parent.get_().second.info_.ext_.length() -1);
          }
        } else if (base == TYPE_X) {
          t_word attr{"."};
          fullname.push_back(attr);
          fullname.push_back(name);
        } else if (get_base_type(type) == TYPE_Z) {
          parent.set_().second.info_.ext_ = name;
          t_word opt{".opt"};
          opt += std::to_string(++parent.set_().second.info_.list_.options_);
          fullname.push_back(std::move(opt));
          name = name.back();
        } else
          fullname.push_back(name);
        ERR_GUARD(err) {
          auto p = table_.insert(t_arg(fullname, t_argvalue(std::move(info))));
          if (p.second) {
            t_argvalue& v = p.first->second;
            if (base != TYPE_Z)
              v.path_ = parent.get_().second.path_;
            else
              v.path_.assign(parent.get_().second.path_.begin(),
                             parent.get_().second.path_.end() - 1);
            v.path_.push_back(get(get_cstr(get_path_type(type))));
            parent.set_().second.values_.push_back(fullname.back()); // make a pointer?
            parent.set_().second.info_.mem_.push_back(&(*p.first));
            return {this , &(*p.first)};
          } else
            err.set(mk_cstr("name not unique"));
        }
      }
      return {};
    }

    t_ref t_argn::add_(t_err err, t_ref       parent,
                                   t_name&&    name,
                                   t_arginfo& info,
                                   t_value&&   value) {
      t_ref ref = add_(err, parent, std::move(name), info);
      if (ref)
        ref.set_().second.values_.push_back(std::move(value));
      return ref;
    }

    t_ref t_argn::add_(t_err err, t_ref       parent,
                                   t_name&&    name,
                                   t_arginfo& info,
                                   t_values&&  values) {
      t_ref ref = add_(err, parent, std::move(name), info);
      if (ref)
        ref.set_().second.values_ = std::move(values);
      return ref;
    }

    t_bool t_argn::del_(t_ref ref, const t_name& name) {
      switch (ref.get_base_type()) {
        case TYPE_L:
        case TYPE_Z: {
          t_collection_ref collection(ref);
          t_ref found = collection[name];
          if (found) {
            t_fullname argname(ref.get_fullname());
            argname.push_back(name);
            t_itr first = table_.find(argname);
            if (first != std::end(table_)) {
              argname.push_back("/");
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
        t_list_ref root(get_root());
        if (root && !root.is_empty())
          return root[0];
      }
      return {};
    }

    t_ref t_argn::next(t_ref ref) {
      if (ref) {
        switch (ref.get_base_type()) {
          case TYPE_MB:
          case TYPE_S:
          case TYPE_A:
          case TYPE_H:
          case TYPE_C:
          case TYPE_K: {
            const t_fullname& refname = ref.get_fullname();
            t_fullname endname(refname);
            endname.push_back("/");
            t_itr itr = table_.upper_bound(endname);
            if (itr != std::end(table_))
              return {this, &(*itr)};
          } break;
          case TYPE_X: {
            t_lookup_ref lookup(ref);
            return lookup.begin_value();
          } break;
          case TYPE_XI:
          case TYPE_L:
          case TYPE_Z: {
            t_collection_ref collection(ref);
            if (collection && !collection.is_empty())
              return collection[0];
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
        t_list_cref root(get_root());
        if (root && !root.is_empty())
          return root[0];
      }
      return {};
    }

    t_cref t_argn::cnext(t_cref ref) const {
      if (ref) {
        switch (ref.get_base_type()) {
          case TYPE_MB:
          case TYPE_S:
          case TYPE_A:
          case TYPE_H:
          case TYPE_C:
          case TYPE_K: {
            const t_fullname& refname = ref.get_fullname();
            t_fullname endname(refname);
            endname.push_back("/");
            t_citr itr = table_.upper_bound(endname);
            if (itr != std::cend(table_))
              return {this, &(*itr)};
          } break;
          case TYPE_X: {
            t_lookup_cref lookup(ref);
            return lookup.cbegin_value();
          } break;
          case TYPE_XI:
          case TYPE_L:
          case TYPE_Z: {
            t_collection_cref collection(ref);
            if (collection && !collection.is_empty())
              return collection[0];
          } break;
          default:
            break;
        }
      }
      return {};
    }

////////////////////////////////////////////////////////////////////////////////

    t_ref t_argn::add_simple_(t_err err, t_ref             parent,
                                          t_name&&          name,
                                          const t_oparams& params) {
      if (check_name(err, name)) {
        t_arginfo info(params.optional_ ? TYPE_OS : TYPE_S);
        return add_(err, parent, std::move(name), info);
      }
      return {};
    }

    t_ref t_argn::add_simple_(t_err err, t_ref             parent,
                                          t_name&&          name,
                                          t_value&&         value,
                                          const t_oparams& params) {
      if (check_name(err, name) && check_value(err, value)) {
        t_arginfo info(params.optional_ ? TYPE_OS : TYPE_S);
        return add_(err, parent, std::move(name), info, std::move(value));
      }
      return {};
    }

    t_ref t_argn::add_array_(t_err err, t_ref           parent,
                                         t_name&&        name,
                                         t_bool          init,
                                         const t_params& params) {
      if (check_name(err, name) && check_range(err, params)) {
        const t_type type = params.optional_ ? TYPE_OA : TYPE_A;
        t_arginfo info(type, get(params.range_max_), get(params.range_min_),
                       init);
        return add_(err, parent, std::move(name), info);
      }
      return {};
    }

    t_ref t_argn::add_array_(t_err err, t_ref           parent,
                                         t_name&&        name,
                                         t_values&&      values,
                                         const t_params& params) {
      if (check_name(err, name) && check_values(err, values) &&
          check_range(err, params, t_n(values.size()))) { // narrowing
        const t_type type = params.optional_ ? TYPE_OA : TYPE_A;
        t_arginfo info(type, get(params.range_max_), get(params.range_min_),
                       true);
        info.range_.cnt_ = values.size();
        return add_(err, parent, std::move(name), info, std::move(values));
      }
      return {};
    }

    t_ref t_argn::add_selection_(t_err err, t_ref            parent,
                                             t_name&&         name,
                                             t_values&&       values,
                                             const t_oparams& params) {
      if (check_name(err, name) && check_selection_values(err, values)) {
        t_arginfo info(params.optional_ ? TYPE_OH : TYPE_H);
        return add_(err, parent, std::move(name), info, std::move(values));
      }
      return {};
    }

    t_ref t_argn::add_compound_(t_err err, t_ref             parent,
                                            t_name&&          name,
                                            t_values&&        values,
                                            const t_oparams& params) {
      if (check_name(err, name) && check_values(err, values)) {
        t_arginfo info(params.optional_ ? TYPE_OC : TYPE_C);
        return add_(err, parent, std::move(name), info, std::move(values));
      }
      return {};
    }

    t_ref t_argn::add_compound_(t_err err, t_ref            parent,
                                            t_name&&         name,
                                            t_values&&       values,
                                            t_value&&        value,
                                            const t_oparams& params) {
      if (check_name(err, name) && check_values(err, values) &&
          check_value(err, value)) {
        t_arginfo info(params.optional_ ? TYPE_OC : TYPE_C);
        info.ext_ = std::move(value);
        return add_(err, parent, std::move(name), info, std::move(values));
      }
      return {};
    }

    t_ref t_argn::add_boolean_(t_err err, t_ref            parent,
                                           t_name&&         name,
                                           t_bool           state,
                                           const t_oparams& params) {
      if (check_name(err, name)) {
        t_arginfo info(params.optional_ ? TYPE_OB : (state ? TYPE_B : TYPE_MB));
        return add_(err, parent, std::move(name), info, {str(state)});
      }
      return {};
    }

    t_ref t_argn::add_list_(t_err err, t_ref             parent,
                                        t_name&&          name,
                                        const t_oparams& params) {
      if (check_name(err, name)) {
        t_arginfo info(params.optional_ ? TYPE_OL : TYPE_L);
        return add_(err, parent, std::move(name), info);
      }
      return {};
    }

    t_ref t_argn::add_openlist_(t_err err, t_ref             parent,
                                            t_name&&          name,
                                            const t_oparams& params) {
      if (check_name(err, name)) {
        t_arginfo info(params.optional_ ? TYPE_OK : TYPE_K);
        return add_(err, parent, std::move(name), info);
      }
      return {};
    }

    t_ref t_argn::add_option_(t_err err, t_ref             parent,
                                          t_name&&          name,
                                          const t_oparams& params) {
      if (check_name(err, name)) {
        t_arginfo info(params.optional_ ? TYPE_OZ : TYPE_Z);
        info.ext_ = name;
        return add_(err, parent, std::move(name), info);
      }
      return {};
    }

    t_ref t_argn::add_lookup_(t_err err, t_ref           parent,
                                          t_name&&        name,
                                          const t_params& params) {
      if (check_name(err, name) && check_range(err, params)) {
        const t_type type = params.optional_ ? TYPE_OX : TYPE_X;
        t_arginfo info(type, get(params.range_max_), get(params.range_min_));
        return add_(err, parent, std::move(name), info);
      }
      return {};
    }

    inline t_arg& get_(t_itr& itr) {
      return *itr;
    }

    inline t_argvalue& get_value_(t_itr& itr) {
      return itr->second;
    }

    inline const t_arg& get_(t_citr& itr) {
      return *itr;
    }

    inline const t_argvalue& get_value_(t_citr& itr) {
      return itr->second;
    }

    inline t_arg& get_(t_pair& pair) {
      return *pair.first;
    }

    inline t_argvalue& get_value_(t_pair& pair) {
      return pair.first->second;
    }

    t_ref t_argn::add_lookup_value_(t_err err, t_ref ref, t_name&& name) {
      t_lookup_ref lookup(err, ref);
      ERR_GUARD(err) {
        if (!name.is_empty()) {
          auto fullname = lookup.get_fullname();
          fullname.push_back(std::move(name));
          auto pair = table_.insert(t_arg(fullname, t_arginfo(TYPE_XI)));
          if (pair.second) {
            pair.first->second.info_.range_ = lookup.get_().second.info_.range_;
            pair.first->second.info_.range_.init_ = true;
            for (auto member : lookup.set_().second.info_.mem_) {
              p_arg arg = (p_arg)member;
              auto mem_name(fullname);
              mem_name.push_back(arg->first.back());
              auto mem_pair = table_.insert(t_arg(std::move(mem_name),
                                                  t_arginfo(arg->second.info_.type_)));
              if (mem_pair.second) {
                pair.first->second.info_.mem_.push_back(&(*mem_pair.first));
                switch (arg->second.info_.type_) {
                  case argn::TYPE_S:
                    break;
                  case argn::TYPE_MB:
                    mem_pair.first->second.values_.push_back(FALSE_s);
                    break;
                  case argn::TYPE_H:
                    get_value_(mem_pair).values_ = arg->second.values_;
                    get_value_(mem_pair).info_.mem_.push_back(arg);
                    break;
                  case argn::TYPE_C:
                    mem_pair.first->second.values_ = arg->second.values_;
                    break;
                  case argn::TYPE_A:
                    mem_pair.first->second.info_.range_ = arg->second.info_.range_;
                    break;
                  case argn::TYPE_X:
                    mem_pair.first->second.info_.range_ = arg->second.info_.range_;
                    mem_pair.first->second.info_.range_.init_ = true;
                    mem_pair.first->second.info_.mem_   = arg->second.info_.mem_;
                    break;
                  default: err = err::E_INTERNAL;
                    break;
                }
              } else
                err.set(mk_cstr("could not insert"));
              if (err)
                break;
            }
            ERR_GUARD(err) {
                lookup.set_().second.info_.range_.init_ = true;
              ++lookup.set_().second.info_.range_.cnt_;
              return {this, &(*pair.first)};
            }
          } else if (pair.first != std::end(table_))
            err.set(mk_cstr("lookup value exist"));
          else
            err.set(mk_cstr("could not insert lookup value"));
        } else
          err = err::E_NO_NAME;
      }
      return {};
    }

   t_bool t_argn::del_lookup_value_(t_ref ref,  const t_name& name) {
      if (ref) {
        t_fullname argname(ref.get_fullname());
        argname.push_back(name);
        t_itr first = table_.find(argname);
        if (first != std::end(table_)) {
          argname.push_back("/");
          table_.erase(first, table_.upper_bound(argname));
          return true;
        }
      }
      return false;
    }

    t_void t_argn::clr_lookup_value_(t_ref ref) {
      t_lookup_ref lookup(ref);
      if (lookup && !lookup.is_empty()) {
        t_fullname endname(lookup.get_fullname());
        endname.push_back(".");
        endname.push_back("/");
        t_itr first = table_.upper_bound(endname);
        if (first != std::end(table_)) {
          endname.erase(endname.end() - 2);
          table_.erase(first, table_.upper_bound(endname));
        }
        ref.set_().second.info_.range_.cnt_ = 0;
      }
    }

    t_ref t_argn::get_lookup_value_(t_ref ref,  P_cstr name) {
      t_fullname fullname(ref.get_fullname());
      fullname.push_back(get(name));
      return operator[](fullname);
    }

    t_cref t_argn::get_lookup_value_(t_cref ref, P_cstr name) const {
      t_fullname fullname(ref.get_fullname());
      fullname.push_back(get(name));
      return operator[](fullname);
    }

    t_list_ref t_argn::get_root() {
      if (!table_.empty())
        return t_ref{this, &(*std::begin(table_))};
      return t_ref{};
    }

    t_list_cref t_argn::get_root() const {
      return cget_root();
    }

    t_list_cref t_argn::cget_root() const {
      if (!table_.empty())
        return t_cref{this, &(*std::cbegin(table_))};
      return t_cref{};
    }

    t_ref t_argn::operator[](const t_fullname& fullname) {
      auto itr = table_.find(fullname);
      if (itr != std::end(table_))
        return {this, &(*itr)};
      return {};
    }

    t_cref t_argn::operator[](const t_fullname& fullname) const {
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
        std::string type("[");
        const t_arginfo& info = p.second.info_;
        type += '<';
        type += get(get_cstr(info.type_));
        type += '>';

        t_type base = get_base_type(info.type_);
        if (base == TYPE_X || base == TYPE_A) {
          type += '<';
          if (info.range_.max_)
             type += std::to_string(info.range_.max_);
          else
             type += 'n';
          type += ">,<";
          type += std::to_string(info.range_.min_);
          type += ">,<";
          type += std::to_string(info.range_.cnt_);
          type += ">,<";
          type += info.range_.init_ ? "init" : "uninit";
          type += ">";
        }
        if (base == TYPE_L || base == TYPE_Z || base == TYPE_X) {
          // number of options
          auto max = info.mem_.size();
          for (decltype(max) ix = 0; ix < max; ++ix) {
            p_carg arg = (p_carg)info.mem_[ix];
            type += ",";
            type += arg->first.back();
          }
        }
        type += "]";

        std::string path("[");
        auto max_path = p.second.path_.size();
        for (decltype(max_path) i = 0; i < max_path; ++i) {
          path += p.second.path_[i];
          if (i < max_path - 1)
            path += ',';
        }
        path += "]";

        std::string name("[");
        auto max_names = p.first.size();
        for (decltype(max_names) i = 0; i < max_names; ++i) {
          name += p.first[i];
          if (i < max_names - 1)
            name += ',';
        }
        name += ']';

        std::string value("[");
        auto max_values = p.second.values_.size();
        if (max_values) {
          for (decltype(max_values) i = 0; i < max_values; ++i) {
            if (!p.second.values_[i].empty()) {
              value += p.second.values_[i];
            } else
              value += "-";
            if (i < max_values - 1)
              value += ',';
          }
        }
        value += "]";

        types.push_back(type);
        paths.push_back(path);
        names.push_back(name);
        values.push_back(value);

        if (type_max < type.length())
          type_max = type.length();

        if (path_max < path.length())
          path_max = path.length();

        if (name_max < name.length())
          name_max = name.length();

        if (value_max < value.length())
          value_max = value.length();
      }

      unsigned entries = names.size();
      for (unsigned i = 0; i < entries; ++i) {
        std::cout << "  {" << std::left
                  << std::setw(name_max)  << names[i]  << ","
                  << std::setw(value_max) << values[i] << ","
                  << std::setw(path_max)  << paths[i]  << ","
                  << std::setw(type_max)  << types[i]  << "}";
        if (i < entries - 1)
          std::cout << ",";
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

    t_compound_ref t_argn::transform_(t_err err, t_simple_ref& ref,
                                                  t_values&& values) {
      if (check_values(err, values)) {
        ref.set_().second.info_.ext_  = ref.set_().second.values_.front();
        ref.set_().second.values_     = std::move(values);
        ref.set_().second.info_.type_ =
          is_optional(ref.get_().second.info_.type_) ? TYPE_OC : TYPE_C;
        t_compound_ref tmp = ref;
        ref.clear_();
        return tmp;
      }
      return t_ref{};
    }

    t_bool operator==(const t_argn& lh, const t_argn& rh) {
      if (lh.get_size() == rh.get_size())
        return lh.get_root() == rh.get_root();
      return false;
    }

    t_bool operator!=(const t_argn& lh, const t_argn& rh) {
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
          case TYPE_MB:
            return true;
          case TYPE_C: {
            t_compound_cref lh_ref(lh), rh_ref(rh);
            return lh_ref.get_part_values() == rh_ref.get_part_values();
          }
          case TYPE_H: {
            t_selection_cref lh_ref(lh), rh_ref(rh);
            return lh_ref.get_values() == rh_ref.get_values();
          }
          case TYPE_A: {
            t_array_cref lh_ref(lh), rh_ref(rh);
            return lh_ref.get_range_params() == rh_ref.get_range_params();
          }
          case TYPE_X: {
            t_lookup_cref lh_ref(lh), rh_ref(rh);
            if (lh_ref.get_range_params() == rh_ref.get_range_params() &&
                get(lh_ref.get_size()) == get(rh_ref.get_size())) {
              auto max = get(lh_ref.get_size());
              for (decltype(max) ix = 0; ix < max; ++ix)
                if (!is_same_definition(lh_ref[t_ix{ix}], rh_ref[t_ix{ix}]))
                  return false;
              return true;
            }
          } break;
          case TYPE_XI: {
            t_lookup_value_cref lh_ref(lh), rh_ref(rh);
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
            case TYPE_MB: {
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
            case TYPE_A: {
              t_array_cref lh_ref(lh), rh_ref(rh);
              return lh_ref.get_values() == rh_ref.get_values();
            }
            case TYPE_X: {
              t_lookup_cref lh_ref(lh), rh_ref(rh);
              if (get(lh_ref.size_value()) == get(rh_ref.size_value())) {
                if (!get(lh_ref.size_value())) // is_empty - XXX
                  return true;
                else {
                  t_lookup_value_cref lh_value_ref(lh_ref.cbegin_value()),
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
            case TYPE_XI: {
              t_lookup_value_cref lh_ref(lh), rh_ref(rh);
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

    t_void append(t_err err, t_list_ref list, t_cref ref) {
      ERR_GUARD(err) {
        if (list && ref) {
          // make sure something is not inserted from itself.
        } else
          err = err::E_INVALID_REF;
      }
    }
  }

  using namespace syntax;

  t_void parse_syntax(t_err err, argn::r_argn argn, R_text text) {
    ERR_GUARD(err) {
      argn::t_list_ref list(err, argn.get_root());
      parse::parse_list(err, list, parse::strip_space(get(text.get_cstr())));
    }
  }

  t_void build_syntax(t_err err, r_text text, argn::R_argn argn) {
    ERR_GUARD(err) {
      argn::t_list_cref list(err, argn.get_root());
      build::build_list(err, text, list);
    }
  }

  t_void merge_syntax(t_err err, argn::r_argn use, argn::R_argn def) {
    ERR_GUARD(err) {
      argn::t_list_ref  use_list(err, use.get_root());
      argn::t_list_cref def_list(err, def.get_root());
      merge::merge_list(err, use_list, def_list);
    }
  }

////////////////////////////////////////////////////////////////////////////////
}
}
