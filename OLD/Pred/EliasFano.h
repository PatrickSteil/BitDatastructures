#include "../StraightFWDBIV/BitVector.h"
#include <bitset> // just to print stuff
#include <vector>

namespace EF {
class EliasFano {
public:
  EliasFano(const std::vector<uint64_t> &numbers)
      : numElements(numbers.size()), lowerSize(ceil_log2((~0) / numElements)),
        upper((numElements << 1) + 1), lower(numElements * lowerSize) {
    processNumbers(numbers);
  };

  void printInfo() const noexcept {
    std::cout << "Elias-Fano Encoding Stats:       " << std::endl;
    std::cout << "Number of elements:              " << numElements
              << std::endl;
    std::cout << "MaxNumber:                       " << (uint64_t)(~0)
              << std::endl;
    std::cout << "UpperSize:                       " << ceil_log2(numElements)
              << std::endl;
    std::cout << "LowerSize:                       " << lowerSize << std::endl;
    std::cout << "Total Space Consumption:         " << (totalSizeByte() << 3)
              << std::endl;

    std::cout << "Information about the 'upper':   " << std::endl;
    upper.printInfo();
    // upper.printVector();

    std::cout << "Information about the 'lower':   " << std::endl;
    lower.printInfo();
    // lower.printVector();
  }

  inline uint64_t totalSizeByte() const {
    uint64_t total(0);
    total += sizeof(numElements);
    total += sizeof(lowerSize);
    total += upper.totalSizeByte();
    total += lower.totalSizeByte();
    return total;
  }

  inline uint64_t pred(uint64_t number) const {
    size_t upperSize = ceil_log2(numElements);
    uint64_t mask = ((uint64_t)(~0) >> upperSize);
    uint64_t xStar = (number & ~(mask)) >> lowerSize;
    uint64_t p = (xStar > 0 ? upper.selectFalse(xStar) - xStar + 1 : 0);
    uint64_t nextP = upper.selectFalse(xStar + 1) - xStar;
    std::cout << "Number: " << number << " x': " << std::bitset<64>(xStar)
              << " Rank: " << p << " Next P: " << nextP << std::endl;
    // scan corresponding values in lower till predecessor is found
    uint64_t result = (xStar << lowerSize);
    std::cout << "Maske: " << std::bitset<64>(mask) << std::endl;
    // int i(p * lowerSize);
    // std::cout << "Select False: " << p << " for number " << number << " xStar
    // is " << xStar << std::endl;

    // std::cout << number << " or: " << std::bitset<64>(number) << std::endl;
    // std::cout << "Result: " << std::bitset<64>(result) << std::endl;
    uint64_t currentValue = 0;
    uint64_t rank = p;
    if (rank > 0) {
      for (size_t i = 0; i < lowerSize; ++i) {
        currentValue <<= 1;
        currentValue |= lower[rank - lowerSize + i];
      }
      result = (result & ~(mask)) | (currentValue & mask);
    }
    // std::cout << "Select next: " << upper.rankFalse(rank+1) << "\n";
    std::cout << "Number: " << number << std::endl;
    while (rank < lower.size()) {
      currentValue = 0;
      std::cout << "Rank: " << rank << std::endl;
      for (size_t i = 0; i < lowerSize; ++i) {
        currentValue <<= 1;
        currentValue |= lower[rank + i];
      }

      std::cout << "currentValue: " << std::bitset<64>(currentValue) << " vs "
                << std::bitset<64>((number & mask)) << std::endl;
      if (currentValue <= (number & mask)) {
        result = (result & ~(mask)) | (currentValue & mask);
        std::cout << "Current Result: " << std::bitset<64>(result) << "\n\t"
                  << result << std::endl;
      } else
        return result;
      rank += lowerSize;
    }
    std::cout << "Out of rank!" << std::endl;
    return result;
  }

  inline uint64_t get(uint64_t index) const {
    // upper
    uint64_t upperHalf = (upper.selectTrue(index) - index) << lowerSize;
    // lower
    uint64_t rank = upper.rankTrue(index);

    uint64_t lowerHalf = 0;
    for (uint64_t i(0); i < lowerSize; ++i) {
      lowerHalf += lower[rank + i];
      lowerHalf <<= 1;
    }

    // std::cout << upper.selectTrue(index) << ", " << index << ", " <<
    // upperHalf << ", " << rank << ", " << lowerHalf << std::endl;

    return (upperHalf | lowerHalf);
  }

private:
  inline void processNumbers(const std::vector<uint64_t> &numbers) {
    assertmsg(upper.size() == (numbers.size() << 1) + 1,
              "Upper does not have the right size!");
    for (uint64_t i(0); i < numbers.size(); ++i) {
      // represent pi with a 1 at position i + pi
      uint64_t p_i = (numbers[i] & ((uint64_t)(~0) << lowerSize)) >> lowerSize;
      upper.set(i + p_i);

      uint64_t mask = ~((uint64_t)(~0) << lowerSize);
      uint64_t smaller_p_i = numbers[i] & mask;
      for (uint64_t offset(0); offset < lowerSize; ++offset) {
        // std::cout << numbers[i] << " offset " << offset << " and " <<
        // std::bitset<64>((smaller_p_i & ((uint64_t)1 << (lowerSize - offset -
        // 1))) >> (lowerSize - offset - 1)) << std::endl;
        if (uint64_t(
                (smaller_p_i & ((uint64_t)1 << (lowerSize - offset - 1))) >>
                (lowerSize - offset - 1))) {
          // std::cout << "Set " << (i * lowerSize + offset) << " bit in lower"
          //           << std::endl;
          lower.set(i * lowerSize + offset);
        }
      }
    }

    // std::cout << "Build Select Upper\n";
    upper.buildSelectLookup();
    // std::cout << "Build Rank Upper\n";
    upper.buildRankTable();
    // std::cout << "Build Select Lower\n";
    lower.buildSelectLookup();
    // std::cout << "Build Rank Lower\n";
    lower.buildRankTable();
  }

  uint64_t numElements;

  uint64_t lowerSize;
  StraightFWDBIV::BitVector upper;
  StraightFWDBIV::BitVector lower;
};
} // namespace EF