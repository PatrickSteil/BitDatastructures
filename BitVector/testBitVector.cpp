#include <chrono>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#include "BitVector.h"

int main() {
  using std::chrono::duration;
  using std::chrono::duration_cast;
  using std::chrono::high_resolution_clock;
  using std::chrono::milliseconds;
  std::srand(
      std::time(nullptr)); // use current time as seed for random generator

  size_t numBits = (1 << 4);

  auto t1 = high_resolution_clock::now();
  BIV::BitVector vec(numBits);

  std::vector<bool> vals = {0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0};

  for (int i(0); i < std::floor(numBits / 2); ++i) {
    // vec.set(std::rand() % numBits);
    if (vals[i])
      vec.set(i);
  }

  vec.buildSelectLookup();

  auto t2 = high_resolution_clock::now();
  auto ms_int = duration_cast<milliseconds>(t2 - t1);

  std::cout << "Time took " << ms_int.count() << " and space "
            << (vec.totalSizeByte() << 3) << std::endl;

  vec.printVector();

  std::cout << "Select_0(0):\n" << vec.selectFalse(0) << std::endl;
  std::cout << "Select_0(2):\n" << vec.selectFalse(2) << std::endl;

  std::cout << "Select_1(0):\n" << vec.selectTrue(0) << std::endl;
  std::cout << "Select_1(4):\n" << vec.selectTrue(2) << std::endl;

  return 0;
}