#include <iostream>
#include <vector>
#include <string>
#include "dainty_base_string_segmented.h"

using namespace dainty::base;
using namespace dainty::base::traits;
using namespace dainty::base::string;
using namespace dainty::base::string::segmented;
namespace seg = dainty::base::string::segmented;

///////////////////////////////////////////////////////////////////////////////

constexpr static t_char type[] = " {}<>(),.=!";

enum  t_my_segmented_tag_ { };
using t_my_segmented = t_segmented<t_my_segmented_tag_, 100, t_overflow_grow>;

struct t_my_generator {
  t_n_   cnt = 0;
  t_char delimiter    = '*';

  template<typename TAG, t_n_ N, typename O>
  t_void operator()(t_string<TAG, N, O>& str, t_crange range,
                    seg::t_user usr) noexcept {
    if (usr) {
      str << type[usr] << range;
    } else {
      if (cnt++)
        str << delimiter << range;
      else
        str << range;
    }
  }

  t_void reset() noexcept {
    cnt = 0;
  }
};

enum tag_ { };

__attribute__((noinline))
void insert(std::vector<std::string>& vect) {
  vect.push_back("hallo");
  vect.push_back("bye");
  vect.push_back("sweet");
  vect.push_back("honey");
}

__attribute__((noinline))
void insert(t_my_segmented& segs) {
  segs.push_back("hallo"_SL);
  segs.push_back("bye"_SL);
  segs.push_back("sweet"_SL);
  segs.push_back("honey"_SL);
}

int main() {
  t_my_segmented      segs;
  t_my_generator      generator;
  t_string<tag_, 100, t_overflow_grow> string;

  segs.push_back("hallo"_SL, 3);
  segs.push_back("bye"_SL, 2);
  segs.push_back("sweet"_SL);
  segs.push_back("honey"_SL);

  segs.generate(generator, string);
  generator.reset();
  string.display_then_clear();

  segs.remove(t_seg_no{2});

  segs.generate(generator, string);
  generator.reset();
  string.display_then_clear();

  segs.remove(t_seg_no{0});

  segs.generate(generator,  string);
  generator.reset();
  string.display_then_clear();

  auto id = segs.find("honey"_SL);
  if (id != BAD_ID) {
    std::cout << "found" << std::endl;
    if (segs.change(id, "monkey"_SL))
      std::cout << "changed it" << std::endl;
  }

  segs.generate(generator,  string);
  generator.reset();
  string.display_then_clear();

  segs.push_back("tommy"_SL);
  segs.push_back(10);

  segs.generate(generator,  string);
  generator.reset();
  string.display_then_clear();

  std::vector<std::string> vecs;
  insert(vecs);

  segs.clear();
  insert(segs);

  for (auto range : segs)
    display(range);

  t_my_segmented segs1 = segs;

  segs1.generate(generator, string);
  generator.reset();
  string.display_then_clear();

  segs.assign('/', "hallo/the world is/great"_SL);
  segs.generate(generator, string);
  string.display_then_clear();

  segs.assign('/',
"111111111111111111111111111111111111111111111111/2222222222222222222222222222222222222222222222"
"2222222222222222222222222222222/333333333333333333333333333333333333333333333333333333333333333"_SL);
  segs.generate(generator, string);
  string.display_then_clear();

  return 0;
}

///////////////////////////////////////////////////////////////////////////////

