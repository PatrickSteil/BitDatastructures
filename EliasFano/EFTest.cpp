#include "EliasFano.h"
#include <bitset>
#include <iostream>
#include <vector>

int main() {
  std::vector<unsigned long long> numbers = {
      0,
      1,
      10,
      13,
      32,
      64,
      1926720561250547757,
      (unsigned long long)4354482840745948438uLL,
      (unsigned long long)18446013172173323708uLL,
      (unsigned long long)18446731432489926016uLL};
  EliasFanoEncoding eliasFano(numbers);
  eliasFano.printInfo();

  assert((unsigned long long)eliasFano[2] == (unsigned long long)10);
  assert((unsigned long long)eliasFano[4] == (unsigned long long)32);
  assert((unsigned long long)eliasFano[6] ==
         (unsigned long long)1926720561250547757);
  assert((unsigned long long)eliasFano[7] ==
         (unsigned long long)4354482840745948438);
  assert((unsigned long long)eliasFano[8] ==
         (unsigned long long)18446013172173323708uLL);
  assert((unsigned long long)eliasFano[9] ==
         (unsigned long long)18446731432489926016uLL);

  std::cout << (unsigned long long)eliasFano.pred(11) << std::endl;
  std::cout << (unsigned long long)eliasFano.pred(4354482840745948439)
            << std::endl;
  return 0;
}