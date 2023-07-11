#include "EliasFano.h"
#include <bitset>
#include <iostream>
#include <vector>

int main() {
  std::vector<uint64_t> numbers = {
      0,
      1,
      10,
      13,
      32,
      64,
      1926720561250547757,
      (uint64_t)4354482840745948438uLL,
      (uint64_t)18446013172173323708uLL,
      (uint64_t)18446731432489926016uLL};
  EliasFano eliasFano(numbers);
  eliasFano.printInfo();

  assert((uint64_t)eliasFano[2] == (uint64_t)10);
  assert((uint64_t)eliasFano[4] == (uint64_t)32);
  assert((uint64_t)eliasFano[6] ==
         (uint64_t)1926720561250547757);
  assert((uint64_t)eliasFano[7] ==
         (uint64_t)4354482840745948438);
  assert((uint64_t)eliasFano[8] ==
         (uint64_t)18446013172173323708uLL);
  assert((uint64_t)eliasFano[9] ==
         (uint64_t)18446731432489926016uLL);

  std::cout << (uint64_t)eliasFano.pred(11) << std::endl;
  std::cout << (uint64_t)eliasFano.pred(4354482840745948439)
            << std::endl;
  return 0;
}
