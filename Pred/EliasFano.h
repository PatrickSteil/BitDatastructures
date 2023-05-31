#include "../BitVector/BitVector.h"
// #include <bitset> // just to print stuff
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
    std::cout << "Elias-Fano Encoding:             " << std::endl;
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

    upper.printVector();

    std::cout << "Information about the 'lower':   " << std::endl;
    lower.printInfo();

    lower.printVector();
  }

  inline uint64_t totalSizeByte() const noexcept {
    uint64_t total(0);
    total += sizeof(numElements);
    total += sizeof(lowerSize);
    total += upper.totalSizeByte();
    total += lower.totalSizeByte();
    return total;
  }

  inline uint64_t pred(uint64_t number) const noexcept {
    // TODO
    return number;
  }

private:
  inline void processNumbers(const std::vector<uint64_t> &numbers) {
    assertmsg(upper.size() == (numbers.size() << 1) + 1,
              "Upper does not have the right size!");
    for (uint64_t i(0); i < numbers.size(); ++i) {
      // represent pi with a 1 at position i + pi
      uint64_t p_i = (numbers[i] & ((uint64_t)(~0) << lowerSize)) >> lowerSize;
      upper.set(i + p_i);

      uint64_t smaller_p_i = numbers[i] & ~((uint64_t)(~0) << lowerSize);
      for (int offset(0); offset < lowerSize; ++offset) {
        if (int((smaller_p_i & ((uint64_t)1 << (lowerSize - offset - 1))) >>
                (lowerSize - offset - 1))) {
          lower.set(i * lowerSize + offset);
        }
      }
    }
  }

  uint64_t numElements;

  int lowerSize;
  BIV::BitVector upper;
  BIV::BitVector lower;
};
} // namespace EF