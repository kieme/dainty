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
#include "dainty_oops.h"

#define ERR_GUARD_TAG(err, tagid) if DAINTY_OOPS_BLOCK_GUARD_TAG(err, tagid)
#define ERR_GUARD(err)            if DAINTY_OOPS_BLOCK_GUARD(err)

#define CON(name)   std::cout <<        name << "()" << std::endl
#define DES(name)   std::cout << '~' << name << "()" << std::endl
#define CALL(name)  std::cout <<        name << "()" << std::endl

////////////////////////////////////////////////////////////////////////////////

using namespace dainty;
using named::P_cstr;

namespace asystem {
  using t_error_context = oops::t_ctxt<>;
}

////////////////////////////////////////////////////////////////////////////////

namespace other {
  enum errors {
    E1 = 1,
    E2 = 2
  };

  oops::t_def errors_what(oops::t_id id) {
    static const oops::t_def tbl[] = {
      { oops::v_category_ignore,      P_cstr{"other undefined error"}, E1 },
      { oops::v_category_ignore,      P_cstr{"other error 1"},         E2 },//E1
      { oops::v_category_recoverable, P_cstr{"other error 2"},         0  } //E2
    };
    return tbl[id <= E2 ? id : 0];
  }

  typedef oops::t_oops<errors_what, errors, asystem::t_error_context> err_obj;

  struct D2 {
    D2(err_obj err) {
      ERR_GUARD(err) {
        //err = E2;
        CON("D2");
      }
    }
    ~D2() {
      DES("D2");
    }
  };
}

////////////////////////////////////////////////////////////////////////////////

enum errors {
  E1 = 1,
  E2 = 2,
  E3 = 3,
  E4 = 4
};

oops::t_def errors_what(oops::t_id id) {
  static const oops::t_def tbl[] = {
    { oops::v_category_ignore, P_cstr{"undefined error"}, E1 },
    { oops::v_category_ignore, P_cstr{"error 1"},         E2 }, //E1
    { oops::v_category_ignore, P_cstr{"error 2"},         E3 }, //E2
    { oops::v_category_ignore, P_cstr{"error 3"},         E4 }, //E3
    { oops::v_category_ignore, P_cstr{"error 4"},         0  }  //E4
  };
  return tbl[id <= E4 ? id : 0];
}

typedef oops::t_oops<errors_what, errors, asystem::t_error_context> err_obj;

struct A {
  A(err_obj err) {
    ERR_GUARD(err) {
      //err = E3;
      CON('A');
    }
  }
  ~A() {
    DES('A');
  }
};

struct D1 {
  D1(err_obj err) {
    ERR_GUARD(err) {
      CON("D1");
    }
  }
  ~D1() {
    DES("D1");
  }
};

struct B : A {
  B(err_obj err) : A(err.tag(1)), d1_(err.tag(2)), d2_(err.tag(3)) {
    ERR_GUARD_TAG(err, 4) {
      //err = E2;
      CON("B");
    }
  }
  ~B() {
    DES("B");
  }

         D1 d1_;
  other::D2 d2_;
};

struct C {
  C(err_obj err) : d1_(err), d2_(err) {
    ERR_GUARD(err) {
      //err = E2;
      CON("C");
    }
  }
  ~C() {
    DES("C");
  }
         D1 d1_;
  other::D2 d2_;
};

////////////////////////////////////////////////////////////////////////////////

void foo(err_obj err) {
  ERR_GUARD(err) {
    CALL("foo");
  }
}

void boo(err_obj err) {
  ERR_GUARD(err) {
    CALL("boo");
  }
}

void coo(err_obj err) {
  ERR_GUARD(err) {
    CALL("coo");
    err = E1;
  }
}

void undo_foo() {
  CALL("undo_foo");
}

void undo_boo() {
  CALL("undo_boo");
}

void undo_coo() {
  CALL("undo_coo");
}

////////////////////////////////////////////////////////////////////////////////

int main()
{
  { // ex. 1
    err_obj err;

    B b(err);

    if (err) {
      err.print();
      err.clear();
    }
  }

  { // ex. 2
    err_obj err;

    C c(err);

    if (err) {
      err.print();
      err.clear();
    }
  }

  { // ex. 3
    err_obj err;

    C c(err.tag(1));
    foo(err.tag(2));
    B b(err.tag(3));
    boo(err.tag(4));
    coo(err.tag(5));

    if (err) {
      switch (err.tag()) {
        case 5: undo_boo();
        case 4:
        case 3: undo_foo();
        case 2:
        case 1:
          break;
      }
      err.print();
      err.clear();
    }
  }

  {
    err_obj err(nullptr);
    if (err == oops::INVALID) {
    }
  }

  return 0;
}
