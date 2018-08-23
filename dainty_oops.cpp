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

#include <stdio.h>
#include "dainty_oops.h"

namespace dainty
{
namespace oops
{
  t_def default_what(t_id) {
    return t_def(UNRECOVERABLE, P_cstr{"unspecified oops"});
  }

  void default_policy(R_info info) {
    t_def def(info.what_(info.id_));
    switch (def.category_) {
      case UNRECOVERABLE:
        printf("policy assert unrecoverable oops = %d, %s\n", info.id_,
               get(def.string_));
        assert_now(P_cstr{"oops->default_policy_assert"});
        break;
      case RECOVERABLE:
        printf("policy ignore recoverable oops = %d, %s\n", info.id_,
               get(def.string_));
        break;
      default:
        break;
    }
  }

  void default_print(R_info info, R_data1 data) {
    if (info.what_)
      printf("oops[tag-%d] = %d, %s\n", data.tag_, info.id_,
             get(info.what_(info.id_).string_));
    else
      printf("oops[tag-%d] = no oops\n", data.tag_);
  }

  void default_print(R_info info, R_data2 data) {
    auto data_file = get(data.file_);
    auto info_file = get(info.file_);
    if (info.what_) {
      if (data_file) {
        if (info_file)
          printf("oops[%s:%d, tag-%d, depth-%d] = %d, %s, %s:%d\n",
                 data_file, data.line_, data.tag_, data.depth_, info.id_,
                 get(info.what_(info.id_).string_), info_file, info.line_);
        else
          printf("oops[%s:%d, tag-%d, depth-%d] = %d, %s\n",
                 data_file, data.line_, data.tag_, data.depth_, info.id_,
                 get(info.what_(info.id_).string_));
      } else {
        if (info_file)
          printf("oops[tag-%d, depth-%d] = %d, %s, %s:%d\n",
                 data.tag_, data.depth_, info.id_,
                 get(info.what_(info.id_).string_), info_file, info.line_);
        else
          printf("oops[tag-%d, depth-%d] = %d, %s\n",
                 data.tag_, data.depth_, info.id_,
                 get(info.what_(info.id_).string_));
      }
    } else {
      if (data_file) {
        printf("oops[%s:%d, tag-%d, depth-%d] = no oops\n",
               data_file, data.line_, data.tag_, data.depth_);
      } else {
        printf("oops[tag-%d, depth-%d] = no oops\n", data.tag_, data.depth_);
      }
    }
  }

  void trace_step_in(R_info info, p_what what, P_void context,
                     R_data1 data) {
    printf("step_in-> code = %u, data-%p, context = %p\n", info.id_, &data,
           context);
    for (t_id id = what(0).next_; id; ) {
      t_def def = what(id);
      printf("  -> has code = %u, %s\n", id, get(def.string_));
      id = def.next_;
    }
  }

  void trace_step_in(R_info info, p_what what, P_void context,
                     R_data2 data) {
    printf("step_in-> code = %u, data-%p, depth = %d, context = %p\n",
           info.id_, &data, data.depth_, context);
    for (t_id id = what(0).next_; id; ) {
      t_def def = what(id);
      printf("  -> has code = %u, %s\n", id, get(def.string_));
      id = def.next_;
    }
  }

  void trace_step_out(R_info info, p_what what, P_void context,
                      R_data1 data) {
    printf("step_out-> code = %u, data-%p, context = %p\n",
           info.id_, &data, context);
  }

  void trace_step_out(R_info info, p_what what, P_void context,
                      R_data2 data) {
    auto data_file = get(data.file_);
    if (data_file)
      printf("step_out-> code = %u, data-%p, depth = %d, context = %p, "
             "file = %s, line = %d\n", info.id_, &data, data.depth_, context,
             data_file, data.line_);
    else
      printf("step_out-> code = %u, data-%p, depth = %d, context = %p\n",
             info.id_, &data, data.depth_, context);
  }

  void trace_step_do(R_info info, p_what what, P_void context,
                     R_data1 data) {
    printf("step_do-> code = %u, data-%p, context = %p\n", info.id_,
           &data, context);
  }

  void trace_step_do(R_info info, p_what what, P_void context,
                     R_data2 data) {
    auto data_file = get(data.file_);
    if (data_file)
      printf("step_do-> code = %u, data-%p, depth = %d, context = %p, "
             "file = %s, line = %d\n", info.id_, &data, data.depth_, context,
             data_file, data.line_);
    else
      printf("step_do-> code = %u, data-%p, depth = %d, context = %p\n",
             info.id_, &data, data.depth_, context);
  }
}
}
