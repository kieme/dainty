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
#include <assert.h>
#include "dainty_oops.h"

#define ERR_GUARD_TAG(err, tagid) if DAINTY_OOPS_BLOCK_GUARD_TAG(err, tagid)
#define ERR_GUARD(err)            if DAINTY_OOPS_BLOCK_GUARD(err)

#define CON(name)   std::cout <<        name << "()" << std::endl
#define DES(name)   std::cout << '~' << name << "()" << std::endl
#define CALL(name)  std::cout <<        name << "()" << std::endl

////////////////////////////////////////////////////////////////////////////////

using namespace dainty;
using dainty::oops::P_cstr;

namespace asystem {
  void error_policy(const oops::t_info&);
  typedef oops::t_ctxt<error_policy> t_error_context;
  typedef oops::t_except             t_error_except;
}

////////////////////////////////////////////////////////////////////////////////

namespace other
{
  enum errors {
    E1 = 1,
    E2 = 2
  };

  oops::t_def errors_what(oops::t_id id) {
    static const oops::t_def tbl[] = {
      { oops::IGNORE,      P_cstr("other undefined error"), E1 },
      { oops::IGNORE,      P_cstr("other error 1"),         E2 },//E1
      { oops::RECOVERABLE, P_cstr("other error 2"),         0  } //E2
    };
    return tbl[id <= E2 ? id : 0];
  }

  typedef oops::t_oops<errors_what, errors, asystem::t_error_context> err_obj;

  struct D2 {
    D2(err_obj err)
      try {
        ERR_GUARD(err) {
          err = E2;
          CON("D2");
        }
      } catch (const asystem::t_error_except& ex) {
        throw;
      } catch (...) {
        throw;
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
    { oops::IGNORE, P_cstr("undefined error"), E1 },
    { oops::IGNORE, P_cstr("error 1"),         E2 }, //E1
    { oops::IGNORE, P_cstr("error 2"),         E3 }, //E2
    { oops::IGNORE, P_cstr("error 3"),         E4 }, //E3
    { oops::IGNORE, P_cstr("error 4"),         0  }  //E4
  };
  return tbl[id <= E4 ? id : 0];
}

typedef oops::t_oops<errors_what, errors, asystem::t_error_context> err_obj;

struct A {
  A(err_obj err)
    try {
      ERR_GUARD(err) {
        //err = E3;
        CON('A');
      }
    } catch (const asystem::t_error_except& ex) {
      throw;
    } catch (...) {
      throw; // translate to t_error_except
    }

  ~A() {
    DES('A');
  }
};

struct D1 {
  D1(err_obj err)
    try {
      ERR_GUARD(err) {
        CON("D1");
      }
    } catch (const asystem::t_error_except& ex) {
      throw;
    } catch (...) {
      throw; // translate to t_error_except
    }

  ~D1() {
    DES("D1");
  }
};

struct B : A {
  B(err_obj err)
    try : A(err.tag(1)), d1_(err.tag(2)), d2_(err.tag(3)) {
      ERR_GUARD_TAG(err, 4) {
        //err = E2;
        CON("B");
      }
    } catch (const asystem::t_error_except& ex) {
      throw;
    } catch (...) {
      throw; // translate to t_error_except
    }

  ~B() {
    DES("B");
  }

         D1 d1_;
  other::D2 d2_;
};

struct C {
  C(err_obj err)
    try : d1_(err), d2_(err) {
      ERR_GUARD(err) {
        //err = E2;
        CON("C");
      }
    } catch (const asystem::t_error_except& ex) {
      throw;
    } catch (...) {
      throw; // translate to t_error_except
    }

  ~C() {
    DES("C");
  }
         D1 d1_;
  other::D2 d2_;
};

////////////////////////////////////////////////////////////////////////////////

void foo(err_obj err) {
  try {
    ERR_GUARD(err) {
      CALL("foo");
    }
  } catch (const asystem::t_error_except& ex) {
    throw;
  } catch (...) {
    throw; // translate to t_error_except
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
    asystem::t_error_except except;
    err_obj err;
    try {
      B b(err);
    } catch (const asystem::t_error_except& ex) {
      except = ex;
    }

    if (err || except) {
      if (except && !err.knows(except))
        throw except;
      else {
        err.print();
        err.clear();
      }
    }
  }

  { // ex. 2
    asystem::t_error_except except;
    err_obj err;
    try {
      C c(err);
    } catch (const asystem::t_error_except& ex) {
      except = ex;
    }

    if (err || except) {
      if (except && !err.knows(except))
        throw except;
      else {
        err.print();
        err.clear();
      }
    }
  }

  { // ex. 3
    asystem::t_error_except except;
    err_obj err;
    try {
      C c(err.tag(1));
      foo(err.tag(2));
      B b(err.tag(3));
      boo(err.tag(4));
      coo(err.tag(5));
    } catch (const asystem::t_error_except& ex) {
      except = ex;
    }

    if (err || except) {
      switch (err.tag()) {
        case 5: undo_boo();
        case 4:
        case 3: undo_foo();
        case 2:
        case 1:
          break;
      }

      if (except && !err.knows(except))
        throw except;
      else {
        err.print();
        err.clear();
      }
    }
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////

namespace asystem {
  void error_policy(const oops::t_info& info) {
    oops::t_def def(info.what_(info.id_));
    switch (def.category_) {
      case oops::UNRECOVERABLE:
        assert(0);

      case oops::RECOVERABLE:
        std::cout << "throw exception" << std::endl;
        throw t_error_except(info.ctxt_);

      case oops::IGNORE:
        break;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
