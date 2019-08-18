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
#include "dainty_dacli.h"

using namespace dainty::dacli;
using namespace dainty::dacli::argn;

///////////////////////////////////////////////////////////////////////////////

inline
std::ostream& operator<<(std::ostream& out, const t_name& name) {
  out << get(name.get_cstr());
  return out;
}

inline
std::ostream& operator<<(std::ostream& out, const t_text& text) {
  out << get(text.get_cstr());
  return out;
}

t_name make_long(const t_fullname& fullname) {
  t_name name("[");
  auto max = get(fullname.get_segs_num());
  for (decltype(max) i = 0; i < max; ++i) {
    name << fullname[t_seg_no{i}];
    if (i < max - 1)
      name << ',';
  }
  name << "]";
  return name;
}

void print_values(t_err& err, t_cref ref) {
  if (!err) {
    switch(ref.get_base_type()) {
      case TYPE_XI: {
        t_lookup_value_cref dict_value(ref);
        std::cout << "lookup value argument : " << make_long(dict_value.get_fullname()) << std::endl;
      } break;
      case TYPE_X: {
        t_lookup_cref dict(ref);
        std::cout << "lookup argument : " << make_long(dict.get_fullname()) << std::endl;
      } break;
      case TYPE_G: {
        t_group_cref group(ref);
        std::cout << "group argument : " << make_long(group.get_fullname()) << std::endl;
      } break;
      case TYPE_Z: {
        t_options_cref options(ref);
        std::cout << "option argument : " << make_long(options.get_fullname()) << std::endl;
      } break;
      case TYPE_K: {
        t_open_group_cref open_group(ref);
        std::cout << "open_group argument : " << make_long(open_group.get_fullname()) << std::endl;
      } break;
      case TYPE_S: {
        t_simple_cref simple(ref);
        std::cout << "simple argument : " << make_long(simple.get_fullname()) << std::endl;
      } break;
      case TYPE_L: {
        t_list_cref list(ref);
        std::cout << "list argument : " << make_long(list.get_fullname()) << std::endl;
      } break;
      case TYPE_C: {
        t_compound_cref compound(ref);
        std::cout << "compound argument : " << make_long(compound.get_fullname()) << std::endl;
      } break;
      case TYPE_H: {
        t_selection_cref selection(ref);
        std::cout << "selection argument : " << make_long(selection.get_fullname()) << std::endl;
      } break;
      case TYPE_MB: {
        t_boolean_cref boolean(ref);
        std::cout << "boolean argument : " << make_long(boolean.get_fullname()) << std::endl;
      } break;
      default: {
        err = err::E_XXX;
      } break;
    }
  }
}

void print_values(t_err& err, const t_argn& argn) {
  if (!err)
    for (t_cref ref = argn.begin(); ref; ref = argn.next(ref))
      print_values(err, ref);
}

///////////////////////////////////////////////////////////////////////////////
// read one value you know and update it
//
// 1. vlans@n{param1=,param2=}=[vlan-1:{1,2},vlan-2:{2,3},vlan-3:{3,4}]
// 2. vlans@n{param1=,param2=}=[]

void example1() {
  t_err err;
  t_argn argn(err);
  parse_notation(err, argn,
               "(vlans@n{param1=,param2=}="
               "[vlan-1:{1,2},vlan-2:{2,3},vlan-3:{3,4}])"_SL);
  if (!err) {
    t_fullname vlans_dict{'.', "/.vlans"_SL};
    t_lookup_ref       dict  (err, argn[vlans_dict]);
    t_lookup_value_ref vlan_1(err, dict.get_value("vlan-1"_SL));
    t_simple_ref         param1(err, vlan_1["param1"_SL]);
    t_simple_ref         param2(err, vlan_1["param2"_SL]);
    if (!err) {
      std::cout << "param1 = " << param1.get_value() << std::endl;
      param1.set_value(err, "9");
      std::cout << "param1 = " << param1.get_value() << std::endl;

      std::cout << "param2 = " << param2.get_value() << std::endl;
      param2.set_value(err, "7");
      std::cout << "param2 = " << param2.get_value() << std::endl;

      argn.print();

      //dict.del_value("<vlan-2>");
      dict.clear_value();

      argn.print();

      t_text text;
      build_notation(err, text, argn);
      std::cout << text << std::endl;

      print_values(err, argn);
    }
  }
  if (err) {
    argn.print();
    std::cout << "err" << std::endl;
  }
}

void example2() {
  t_err err;
  t_argn argn(err);
  parse_notation(err, argn,
                 "(vlans@n{param1=,param2=}="
                 "[vlan-1:{1,2},vlan-2:{2,3},vlan-3:{3,4}])"_SL);
  if (!err)
    print_values(err, argn);
}

///////////////////////////////////////////////////////////////////////////////
// write values

///////////////////////////////////////////////////////////////////////////////

void example3() {
  t_err err;
  t_argn argn(err);

  t_lookup_ref vlans{t_group_ref{argn.get_root()}.
                       add_lookup(err, "vlans"_SL, UNBOUND_RANGE)};
  vlans.add_simple(err, "param1"_SL);
  vlans.add_simple(err, "param2"_SL);
  {
    t_lookup_value_ref vlan{vlans.add_value(err, "vlan-1"_SL)};
    t_simple_ref{vlan["param1"_SL]}.set_value(err, "1"_SL);
    t_simple_ref{vlan["param2"_SL]}.set_value(err, "2"_SL);
  }

  {
    t_lookup_value_ref vlan(err, vlans.add_value(err, "vlan-2"_SL));
    if (!err) {
      t_simple_ref param1(err, vlan["param1"_SL]);
      param1.set_value(err, "2"_SL);
      t_simple_ref param2(err, vlan["param2"_SL]);
      param1.set_value(err, "3"_SL);
    }
  }

  {
    t_lookup_value_ref vlan(err, vlans.add_value(err, "vlan-3"_SL));
    if (!err) {
      t_simple_ref param1(err, vlan["param1"_SL]);
      param1.set_value(err, "3"_SL);
      t_simple_ref param2(err, vlan["param2"_SL]);
      param1.set_value(err, "4"_SL);
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// write values
// (vlans@5-10{param1=,groups@n{mode=[proxy|snoop]}=}=)
// {vlan-1:(1,[]),vlan-2:{2,[172.168.2.3:{snoop}, 172.168.2.2:{proxy}]},vlan-3:{3,[]}])

///////////////////////////////////////////////////////////////////////////////

void example4() {
  t_err err;
  t_argn argn(err);

  t_group_ref root(argn.get_root());

  t_lookup_ref vlans(root.add_lookup(err, "vlans"_SL, UNBOUND_RANGE));
  vlans.add_simple(err, "param1"_SL);

  t_lookup_ref groups(vlans.add_lookup(err, "groups"_SL, UNBOUND_RANGE));
  groups.add_selection(err, "mode"_SL, {'.', "proxy.snoop"_SL});

  {
    t_lookup_value_ref vlan(vlans.add_value(err, "vlan-1"_SL));
    t_simple_ref param1(err, vlan[0_ix]);
    param1.set_value(err, "1"_SL);
    t_lookup_ref groups(err, vlan[1_ix]);
  }

  {
    t_lookup_value_ref vlan(vlans.add_value(err, "vlan-2"_SL));
    t_simple_ref param1(vlan[0_ix]);
    param1.set_value(err, "2"_SL);
    t_lookup_ref groups(vlan[1_ix]);
    t_lookup_value_ref group1(groups.add_value(err, "172.168.2.3"_SL));
    t_selection_ref mode1(group1[0_ix]);
    mode1.set_value(err, "snoop"_SL);
    t_lookup_value_ref group2(groups.add_value(err, "172.168.2.2"_SL));
    t_selection_ref mode2(group2[0_ix]);
    mode2.set_value(err, "snoop"_SL);
  }

  {
    t_lookup_value_ref vlan(vlans.add_value(err, "<vlan-3>"_SL));
    t_simple_ref param1(vlan[0_ix]);
    param1.set_value(err, "<3>"_SL);
    t_lookup_ref groups(vlan[1_ix]);
  }
}

///////////////////////////////////////////////////////////////////////////////

void example5() {
  t_err err;
  t_argn table(err);
  t_group_ref cmd_ref(t_group_ref{table.get_root()}.add_group(err, "cmd"_SL,
                      t_oparams{true}));
  t_options_ref cmd_option_ref(cmd_ref.add_options(err, ""_SL, t_oparams{true}));

  if (err)
    err.print();

  // register - once
  t_argn argn(err);
  parse_notation(err, argn, "(cmd@n=, args=(in=(*),out=(*)))"_SL);

  t_argn argn1(err);
  parse_notation(err, argn1, "(cmd@n=[mcast,vlan], args=(in=(),out=()))"_SL);

  merge_notation(err, argn1, argn);
  if (!err) {
    t_group_ref group = cmd_option_ref.add_group(err,
"name"_SL);//make_name(t_list_cref{argn1["cmd"]}.get_values()),
    append(err, group, argn1[t_fullname{' ', "args"_SL}]);
  }

  // incoming
  t_argn argn2(err);
  parse_notation(err, argn2,
                 "(cmd=(</,mcast,vlan>=(args=(in=(),out=())))"_SL);
  merge_notation(err, argn2, table);
}

t_value make_flat(const t_fullname& fullname) {
  t_value value;
  value << '[';

  value << ']';
  return value;
}

void example5_1() {
  t_err err;
  t_argn table(err);
  t_group_ref   cmd_ref = table.get_root().add_group(err, {"cmd"_SL}, OPTIONAL_TRUE);
  t_options_ref cmd_option_ref{err, cmd_ref.add(err,
                         "([list]=(in_args(*),out_args=(*))|"
                         " [config]=(in_args(*),out_args=(*))|"
                         " [help]=(in_args(*),out_args=(*))|"
                         " [test]=(in_args=(*),out_args=(*)))"_SL)};
  table.print();

  if (err)
    err.print();

  // register - once
  t_argn argn1(err);
  parse_notation(err, argn1,
                 "(cmd@=[/,mcast,vlan], in_args=(yes), :out_args=(no)))"_SL);

  t_argn argn(err);
  parse_notation(err, argn,  "(cmd@n=, args=(in=(*), :out=(*)))"_SL);

  merge_notation(err, argn1, argn);
  if (!err) {
    t_group_cref root = argn1.cget_root();
    t_list_cref cmd{root["<cmd>"_SL]};
    t_group_ref group{cmd_option_ref.add_group(err, make_flat(cmd.get_values()))};
      //make_name(t_list_cref{argn1["cmd"]}.get_values()),
      //group.add(err, root["<in_args>"]);
      //group.add(err, root["<out_args>"]);

    // incoming
    t_argn argn2(err);
    parse_notation(err, argn2, "(cmd=(</,mcast,vlan>=(args=(in=(),out=())))"_SL);
    merge_notation(err, argn2, table);
  }
}

///////////////////////////////////////////////////////////////////////////////

void example6() {
  t_err err;
  t_argn argn(err);

  argn.get_root().add(err, ":name@n{a=,b=}=[]"_SL);
  argn.print();

  t_text text;
  build_notation(err, text, argn);

  std::cout << text << std::endl;
}

///////////////////////////////////////////////////////////////////////////////

int main() {
  example5_1();
  return 0;
}

int main7(int argc, const char* argv[]) {
  t_err err;
  t_argn use(err);

  if (argc == 3) {
    t_argn def(err);
    parse_notation(err, def, dainty::base::string::mk_range(argv[1]));
    def.print();

    t_argn use(err);
    parse_notation(err, use, dainty::base::string::mk_range(argv[2]));
    use.print();

    merge_notation(err, use, def);
    use.print();

    if (!err) {
      t_text text;
      build_notation(err, text, use);
      std::cout << text << std::endl;
    } else
      err.print();
  } else
    std::printf("'def' 'use'\n");
  return 0;
}

int main1(int argc, const char* argv[]) {
  t_err err;
  t_argn use(err);

  if (argc > 1) {
    parse_notation(err, use, dainty::base::string::mk_range(argv[1]));
  } else
    err = err::E_XXX;
  if (!err) {
    use.print();
    t_text text;
    build_notation(err, text, use);
    std::cout << text << std::endl;
  } else
    err.print();
  return 0;
}

