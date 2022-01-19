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

#include "dainty_valuestore.h"
#include "dainty_test.h"

using namespace dainty::valuestore;
using namespace dainty::test;

int main() {
  t_class_info class_info;
  t_class<>::set_monitor(class_info);

  {
    t_class<> t1;
    t_class<> t2(dainty::util::x_cast(t1));
    t_class<> t3(t2);

    t_class<> t4;
    t3 = t4;

    t_class<> t5;
    t3 = dainty::util::x_cast(t5);
  }

  class_info.display();
  class_info.reset();

  t_class<int>::set_monitor(class_info);
  {
    t_class<int> t1;

    set(t1) = 1;
    Get(t1);
  }

  class_info.display();
  return 0;
}
