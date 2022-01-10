#include "dainty_hashbuf.h"

using namespace dainty::hashbuf;

///////////////////////////////////////////////////////////////////////////////

using t_hash_buf = t_hashbuf<int, int, 5>;

t_hash_id hash(const int& val) noexcept {
  return 0xFFFF & ((val >> 16) ^ val);
}

void visit(t_hash_buf::t_Itr itr) {
  std::cout << itr.Key() << "   " << itr.ref() << std::endl;
}

std::ostream& out_keyvalue(std::ostream& out,
                           const t_hash_buf::t_Itr& itr) noexcept {
  out << itr.Key() << ":" << itr.ref();
  return out;
}

bool pred(t_hash_buf::t_Itr itr) {
  return itr.Key() > 10;
}

///////////////////////////////////////////////////////////////////////////////

using t_hash_buf1 = t_hashbuf<int, void, 4>;

void visit1(t_hash_buf1::t_Itr itr) {
  std::cout << itr.Key() << std::endl;
}

std::ostream& out_keyvalue(std::ostream& out,
                                const t_hash_buf1::t_Itr& itr) noexcept {
  out << itr.Key();
  return out;
}

bool pred1(t_hash_buf1::t_Itr itr) {
  return itr.Key() > 10;
}

///////////////////////////////////////////////////////////////////////////////

int main() {
  {
    t_hash_buf buf(10, hash);

    std::cout << "1. bytes = " << buf.bytes() << std::endl;

    buf.add_unique(0);
    buf.add_unique(1);
    std::cout << "2. bytes = " << buf.bytes() << std::endl;
    buf.add_unique(2);
    buf.add_unique(3);
    buf.add_unique(4);
    buf.add_unique(11);
    buf.add_unique(12);
    buf.add_unique(13);
    std::cout << "3. bytes = " << buf.bytes() << std::endl;

    buf.each(visit);
    buf.Each(visit);
    buf.Each_if(pred, visit);

    buf.find(1);
    buf.del(1, REUSE);
    buf.del(12);
    buf.del(4);

    buf.show(std::cout, true, out_keyvalue);

    std::cout << "4. bytes = " << buf.bytes() << std::endl;

    buf.add_unique(12);

    buf.Each_if(pred, visit);

    buf.del_if(pred, NO_REUSE);

    for (t_hash_buf::t_itr itr = buf.begin(); itr; ++itr) {
       std::cout << "value = " << itr.Key() << std::endl;
    }

    for (t_hash_buf::t_Itr itr = buf.Begin(); itr; ++itr) {
       std::cout << "value = " << itr.Key() << std::endl;
    }

    buf.clear(REUSE);

    std::cout << "5. bytes = " << buf.bytes() << std::endl;
  }

  {
    t_hash_buf buf(10, hash);

    std::cout << ".2. bytes = " << buf.bytes() << std::endl;
    *buf.add_unique(0) = -1;
    *buf.add(0)        = -2;
    *buf.add(0)        = -3;
    *buf.add(0)        = -4;
    *buf.add(0)        = -5;

    buf.add_unique(0);
    buf.add_unique(1);
    buf.add_unique(2);
    buf.add_unique(4);
    buf.add_unique(8);
    buf.add_unique(8);
    buf.add_unique(8);
    buf.add_unique(8);

    t_hash_buf::t_itr itr1 = buf.add_unique(2377546);

    t_hash_buf::t_itr itr2 = buf.find_hash(itr1.hash());
    if (itr1 == itr2) {
        std::cout << "hash found the right key directly" << std::endl;
    }

    if (buf.add_unique(8, FIND_OR_ADD))
      std::cout << "found 8 " << std::endl;

    std::cout << ".2. bytes = " << buf.bytes() << std::endl;

    for (t_hash_buf::t_itr itr = buf.begin(); itr; ++itr) {
      std::cout << ".2. key = " << itr.Key() << ", value = " << *itr
                << ", hash = " << itr.hash() << std::endl;
    }

    *buf.add(0) = -6;

    for (t_hash_buf::t_itr itr = buf.find(0); itr; itr.next_same_key())
      std::cout << ".2. same key = " << itr.Key() << ", hash = " << itr.hash() << std::endl;

    for (t_hash_buf::t_itr itr = buf.find(4); itr; itr.next_same_key())
      std::cout << ".2.1. same key = " << itr.Key() << ", hash = " << itr.hash() <<  std::endl;

    for (t_hash_buf::t_itr itr = buf.find(1); itr; itr.next_same_key())
      std::cout << ".2.2. same key = " << itr.Key() << ", hash = " << itr.hash() << std::endl;

    for (t_hash_buf::t_itr itr = buf.find(2); itr; itr.next_same_key())
      std::cout << ".2.3. same key = " << itr.Key() << ", hash = " << itr.hash() << std::endl;

    for (t_hash_buf::t_itr itr = buf.find(7); itr; itr.next_same_key())
      std::cout << ".2.4. same key = " << itr.Key() << ", hash = " << itr.hash() << std::endl;

    for (t_hash_buf::t_itr itr = buf.find(8); itr; itr.next_same_key())
      std::cout << ".2.5. same key = " << itr.Key() << ", hash = " << itr.hash() << std::endl;

    for (t_hash_buf::t_Itr itr = buf.Find(0); itr; itr.Next_same_key())
      std::cout << ".2. same key = " << itr.Key() << ", hash = " << itr.hash() << std::endl;

    for (t_hash_buf::t_Itr itr = buf.Find(4); itr; itr.Next_same_key())
      std::cout << ".2.1. same key = " << itr.Key() << ", hash = " << itr.hash() << std::endl;

    for (t_hash_buf::t_Itr itr = buf.Find(1); itr; itr.Next_same_key())
      std::cout << ".2.2. same key = " << itr.Key() << ", hash = " << itr.hash() << std::endl;

    for (t_hash_buf::t_Itr itr = buf.Find(2); itr; itr.Next_same_key())
      std::cout << ".2.3. same key = " << itr.Key() << ", hash = " << itr.hash() << std::endl;

    for (t_hash_buf::t_Itr itr = buf.Find(7); itr; itr.Next_same_key())
      std::cout << ".2.4. same key = " << itr.Key() << ", hash = " << itr.hash() << std::endl;

    for (t_hash_buf::t_Itr itr = buf.Find(8); itr; itr.Next_same_key())
      std::cout << ".2.5. same key = " << itr.Key() << ", hash = " << itr.hash() << std::endl;

    buf.show(std::cout, true, out_keyvalue);
  }

  {
    t_hash_buf1 buf(10, hash);

    std::cout << ".2. bytes = " << buf.bytes() << std::endl;

    buf.add_unique(0);
    buf.add(0);
    buf.add(0);
    buf.add(0);
    buf.add(0);

    buf.add_unique(0);
    buf.add_unique(1);
    buf.add_unique(2);
    buf.add_unique(4);
    buf.add_unique(8);
    buf.add_unique(8);
    buf.add_unique(8);
    buf.add_unique(8);

    t_hash_buf1::t_itr itr1 = buf.add_unique(2377546);

    t_hash_buf1::t_itr itr2 = buf.find_hash(itr1.hash());
    if (itr1 == itr2) {
        std::cout << "hash found the right key directly" << std::endl;
    }

    if (buf.add_unique(8, FIND_OR_ADD))
      std::cout << "found 8 " << std::endl;

    std::cout << ".2. bytes = " << buf.bytes() << std::endl;

    for (t_hash_buf1::t_itr itr = buf.begin(); itr; ++itr) {
      std::cout << ".2. key = " << itr.Key()
                << ", hash = " << itr.hash() << std::endl;
    }

    buf.add(0);

    for (t_hash_buf1::t_itr itr = buf.find(0); itr; itr.next_same_key())
      std::cout << ".2. same key = " << itr.Key() << ", hash = " << itr.hash() << std::endl;

    for (t_hash_buf1::t_itr itr = buf.find(4); itr; itr.next_same_key())
      std::cout << ".2.1. same key = " << itr.Key() << ", hash = " << itr.hash() <<  std::endl;

    for (t_hash_buf1::t_itr itr = buf.find(1); itr; itr.next_same_key())
      std::cout << ".2.2. same key = " << itr.Key() << ", hash = " << itr.hash() << std::endl;

    for (t_hash_buf1::t_itr itr = buf.find(2); itr; itr.next_same_key())
      std::cout << ".2.3. same key = " << itr.Key() << ", hash = " << itr.hash() << std::endl;

    for (t_hash_buf1::t_itr itr = buf.find(7); itr; itr.next_same_key())
      std::cout << ".2.4. same key = " << itr.Key() << ", hash = " << itr.hash() << std::endl;

    for (t_hash_buf1::t_itr itr = buf.find(8); itr; itr.next_same_key())
      std::cout << ".2.5. same key = " << itr.Key() << ", hash = " << itr.hash() << std::endl;

    for (t_hash_buf1::t_Itr itr = buf.Find(0); itr; itr.Next_same_key())
      std::cout << ".2. same key = " << itr.Key() << ", hash = " << itr.hash() << std::endl;

    for (t_hash_buf1::t_Itr itr = buf.Find(4); itr; itr.Next_same_key())
      std::cout << ".2.1. same key = " << itr.Key() << ", hash = " << itr.hash() << std::endl;

    for (t_hash_buf1::t_Itr itr = buf.Find(1); itr; itr.Next_same_key())
      std::cout << ".2.2. same key = " << itr.Key() << ", hash = " << itr.hash() << std::endl;

    for (t_hash_buf1::t_Itr itr = buf.Find(2); itr; itr.Next_same_key())
      std::cout << ".2.3. same key = " << itr.Key() << ", hash = " << itr.hash() << std::endl;

    for (t_hash_buf1::t_Itr itr = buf.Find(7); itr; itr.Next_same_key())
      std::cout << ".2.4. same key = " << itr.Key() << ", hash = " << itr.hash() << std::endl;

    for (t_hash_buf1::t_Itr itr = buf.Find(8); itr; itr.Next_same_key())
      std::cout << ".2.5. same key = " << itr.Key() << ", hash = " << itr.hash() << std::endl;

    buf.show(std::cout, true, out_keyvalue);

    buf.rehash(30);

    buf.show(std::cout, true, out_keyvalue);
  }

  {
    t_hash_buf  buf (10, hash);
    t_hash_buf1 buf1(10, hash);
    t_hash_buf  buf2(buf);

    buf2 = buf;

    int hh = 1;
    buf .add_unique(hh);
    buf1.add_unique(1);

    t_hash_buf ::t_itr itr  = buf .add(1);
    t_hash_buf1::t_itr itr1 = buf1.add(1);

    buf .del(1);
    buf1.del(1);

    buf .del(1, REUSE);
    buf1.del(1, REUSE);

    buf .del(1, REUSE, [](auto) { });
    buf1.del(1, REUSE, [](auto) { });

    buf .del_all(1);
    buf1.del_all(1);

    buf .del_all(1, REUSE);
    buf1.del_all(1, REUSE);

    buf .del_all(1, REUSE, [](auto) { });
    buf1.del_all(1, REUSE, [](auto) { });

    buf .del(itr);
    buf1.del(itr1);

    buf .del_if(pred);
    buf .del_if(pred, REUSE);

  }
  return 0;
}
