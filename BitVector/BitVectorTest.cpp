#include "BitVector.h"
#include <iostream>
#include <vector>

int main() {
  std::vector<bool> bits = {0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0};
  BitVector bv(bits);

  // // Perform rank queries
  // std::cout << "Rank of 1 at position 2: " << bv.rank(1, 2) << std::endl;
  // std::cout << "Rank of 0 at position 5: " << bv.rank(0, 5) << std::endl;
  // std::cout << "Rank of 1 at position 8: " << bv.rank(1, 8) << std::endl;

  // // Perform select queries
  // std::cout << "Select 1 after rank 0: " << bv.select(1, 0) << std::endl;
  // std::cout << "Select 0 after rank 3: " << bv.select(0, 3) << std::endl;
  // std::cout << "Select 1 after rank 4: " << bv.select(1, 4) << std::endl;

  // std::cout << std::endl;

  // std::vector<bool> bits2 = {1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0};
  // BitVector bv2(bits2);

  // // Perform rank queries
  // std::cout << "Rank of 1 at position 4: " << bv2.rank(1, 2) << std::endl;
  // std::cout << "Rank of 0 at position 5: " << bv2.rank(0, 5) << std::endl;
  // std::cout << "Rank of 1 at position 3: " << bv2.rank(1, 3) << std::endl;

  // // Perform select queries
  // std::cout << "Select 1 after rank 0: " << bv2.select(1, 0) << std::endl;
  // std::cout << "Select 0 after rank 3: " << bv2.select(0, 3) << std::endl;
  // std::cout << "Select 1 after rank 4: " << bv2.select(1, 4) << std::endl;

  // std::cout << std::endl;

  std::vector<bool> bits3 = {1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  BitVector bv3(bits3);

  std::cout << "Select 0 at rank 18: " << bv3.select(0, 18 - 1) << std::endl;
  return 0;
}
