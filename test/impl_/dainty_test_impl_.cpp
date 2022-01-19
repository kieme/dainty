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

#define DAINTY_INTERNAL_INCLUDE_IOSTREAM
#include "dainty_internal_.h"
#include "dainty_test_impl_.h"

// HOW many constructed
// HOW many constructed how
// HOW to read out data
// SEQUENCE - buildin
//

namespace dainty
{
namespace test
{
  using r_entry_ = types::r_of<t_class_sequence::t_entry>;
  using R_entry_ = types::R_of<t_class_sequence::t_entry>;

  /////////////////////////////////////////////////////////////////////////////

  t_void t_class_info::reset() {
    sequence.ix                = 0;
    counters.default_construct = 0;
    counters.copy_construct    = 0;
    counters.move_construct    = 0;
    counters.copy_assign       = 0;
    counters.move_assign       = 0;
    counters.destruct          = 0;
    counters.set               = 0;
    counters.Get               = 0;
  }

  t_void t_class_info::display() const {
    std::cout
      << "\nname:"
      << "\ntype sizeof: "    << size_of
      << "\ntype alignment: " << align_of
      << "\ncounters:"
      << "\n  default_construct = " << counters.default_construct
      << "\n  copy_construct    = " << counters.copy_construct
      << "\n  move_construct    = " << counters.move_construct
      << "\n  copy_assign       = " << counters.copy_assign
      << "\n  move_assign       = " << counters.move_assign
      << "\n  destruct          = " << counters.destruct
      << "\n  set               = " << counters.set
      << "\n  Get               = " << counters.Get
      << "\n";

    std::cout << "sequece:" << std::endl;
    for (int ix = 0; ix < sequence.ix; ++ix) {
      R_entry_ entry = sequence.history[ix];
      switch (entry.method) {
        case METHOD_DEFAULT_CONSTRUCT:
          std::cout << "default_construct: self-" << entry.self;
          break;
        case METHOD_COPY_CONSTRUCT:
          std::cout << "copy_construct   : self-" << entry.self
                    << ", other-" << entry.other;
          break;
        case METHOD_MOVE_CONSTRUCT:
          std::cout << "move_construct   : self-" << entry.self
                    << ", other-" << entry.other;
          break;
        case METHOD_COPY_ASSIGNMENT:
          std::cout << "copy_assignment  : self-" << entry.self
                    << ", other-" << entry.other;
          break;
        case METHOD_MOVE_ASSIGNMENT:
          std::cout << "move_assignment  : self-" << entry.self
                    << ", other-" << entry.other;
          break;
        case METHOD_DESTRUCT:
          std::cout << "destruct         : self-" << entry.self;
          break;
        case METHOD_SET:
          std::cout << "set              : self-" << entry.self;
          break;
        case METHOD_GET:
          std::cout << "Get              : self-" << entry.self;
          break;
      }
      std::cout << std::endl;
    }
  }

  /////////////////////////////////////////////////////////////////////////////
}
}

///////////////////////////////////////////////////////////////////////////////

namespace dainty
{
namespace test
{
namespace impl_
{
  using r_class_sequence_ = types::r_of<t_class_sequence>;
  using r_class_counters_ = types::r_of<t_class_counters>;

  /////////////////////////////////////////////////////////////////////////////

  t_void invoke_default_construct_(t_class_info* info, P_addr self) {
    if (info) {
      r_class_counters_ counters = info->counters;
      ++counters.default_construct;

      r_class_sequence_ sequence = info->sequence;
      if (sequence.ix < 100) {
        r_entry_ entry = sequence.history[sequence.ix++];
        entry.self   = self;
        entry.method = METHOD_DEFAULT_CONSTRUCT;
      }
    }
  }

  t_void invoke_copy_construct_(t_class_info* info, P_addr self, P_addr other) {
    if (info) {
      r_class_counters_ counters = info->counters;
      ++counters.copy_construct;

      r_class_sequence_ sequence = info->sequence;
      if (sequence.ix < 100) {
        r_entry_ entry = sequence.history[sequence.ix++];
        entry.self   = self;
        entry.other  = other;
        entry.method = METHOD_COPY_CONSTRUCT;
      }
    }
  }

  t_void invoke_move_construct_(t_class_info* info, P_addr self, P_addr other) {
    if (info) {
      r_class_counters_ counters = info->counters;
      ++counters.move_construct;

      r_class_sequence_ sequence = info->sequence;
      if (sequence.ix < 100) {
        r_entry_ entry = sequence.history[sequence.ix++];
        entry.self   = self;
        entry.other  = other;
        entry.method = METHOD_MOVE_CONSTRUCT;
      }
    }
  }

  t_void invoke_destruct_(t_class_info* info, P_addr self) {
    if (info) {
      r_class_counters_ counters = info->counters;
      ++counters.destruct;

      r_class_sequence_ sequence = info->sequence;
      if (sequence.ix < 100) {
        r_entry_ entry = sequence.history[sequence.ix++];
        entry.self   = self;
        entry.method = METHOD_DESTRUCT;
      }
    }
  }

  t_void invoke_copy_assignment_(t_class_info* info, P_addr self, P_addr other) {
    if (info) {
      r_class_counters_ counters = info->counters;
      ++counters.copy_assign;

      r_class_sequence_ sequence = info->sequence;
      if (sequence.ix < 100) {
        r_entry_ entry = sequence.history[sequence.ix++];
        entry.self   = self;
        entry.other  = other;
        entry.method = METHOD_COPY_ASSIGNMENT;
      }
    }
  }

  t_void invoke_move_assignment_(t_class_info* info, P_addr self, P_addr other) {
    if (info) {
      r_class_counters_ counters = info->counters;
      ++counters.move_assign;

      r_class_sequence_ sequence = info->sequence;
      if (sequence.ix < 100) {
        r_entry_ entry = sequence.history[sequence.ix++];
        entry.self   = self;
        entry.other  = other;
        entry.method = METHOD_MOVE_ASSIGNMENT;
      }
    }
  }

  t_void invoke_set_(t_class_info* info, P_addr self) {
    if (info) {
      r_class_counters_ counters = info->counters;
      ++counters.set;

      r_class_sequence_ sequence = info->sequence;
      if (sequence.ix < 100) {
        r_entry_ entry = sequence.history[sequence.ix++];
        entry.self   = self;
        entry.method = METHOD_SET;
      }
    }
  }

  t_void invoke_Get_(t_class_info* info, P_addr self) {
    if (info) {
      r_class_counters_ counters = info->counters;
      ++counters.Get;

      r_class_sequence_ sequence = info->sequence;
      if (sequence.ix < 100) {
        r_entry_ entry = sequence.history[sequence.ix++];
        entry.self   = self;
        entry.method = METHOD_GET;
      }
    }
  }

  /////////////////////////////////////////////////////////////////////////////
}
}
}
