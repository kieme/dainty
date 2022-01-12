#include <iostream>
#include <string>
#include "dainty_valuestore.h"

using namespace dainty::valuestore;

using t_store_ = t_valuestore<int, std::string, void*, long double>;

union t_uni {
  int i;
  std::string s;
  void* p;
  long double d;
};

int main() {
  t_store_ store(std::string("hi"));
  t_store_ store2(pass<std::string>());

  std::cout << alignof(t_store_) << ", " << t_store_::ALIGNMENT << ", " << store.Ref<std::string>() << std::endl;;

  std::cout << store2.Ref<std::string>() << std::endl;;

  //void* ptr = 0;
  //long double db = 0.0;

  //store = 3;
  //store = ptr;
  //store = db;

  std::string kate("kate");
  int i;
  store.swap(kate);

  std::cout << alignof(t_store_) << ", " << t_store_::ALIGNMENT << ", " << Ref<std::string>(store) << std::endl;;

  store = std::string("allo");

  destruct<std::string>(store);

  std::cout << alignof(t_store_) << ", " << t_store_::ALIGNMENT << ", " << Ref<std::string>(store) << std::endl;;

  return 0;
}
