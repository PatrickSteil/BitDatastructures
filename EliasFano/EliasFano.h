#include "../BitVector/BitVector.h"
#include <bitset>
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>

class EliasFanoEncoding {
private:
  BitVector upperBits;
  BitVector lowerBits;
  unsigned long long numElements;
  unsigned long long upperSize;

public:
  EliasFanoEncoding(const std::vector<unsigned long long> &numbers)
      : numElements(numbers.size()) {
    // old version: upperSize = std::floor(std::log2(numElements));
    upperSize = std::ceil(std::log2(numElements));

    // old version: std::vector<bool> upperBitsVEC((numElements << 1) + 1,
    // false);
    std::vector<bool> upperBitsVEC(numElements * 3, false);
    std::vector<bool> lowerBitsVEC(numElements * (64 - upperSize), false);

    unsigned long long i(0);

    for (unsigned long long element : numbers) {
      unsigned long long MSB = element >> (64 - upperSize);
      unsigned long long LSB = (element & lowerMASK());

      // assert((MSB + i) < ((numElements << 1) + 1));
      upperBitsVEC[MSB + i] = true;

      for (unsigned long long j = 0; j < (64 - upperSize); ++j) {
        lowerBitsVEC[i * (64 - upperSize) + j] =
            ((LSB >> ((64 - upperSize) - 1 - j)) & 1);
      }
      ++i;
    }

    upperBits.buildVector(upperBitsVEC);
    lowerBits.buildVector(lowerBitsVEC);
  }

  // Predecessor Method
  unsigned long long pred(unsigned long long x) const {
    // see https://arxiv.org/pdf/2003.11835.pdf
    if ((unsigned long long)(*this)[0] >= x)
      return std::numeric_limits<unsigned long long>::infinity();
    if ((unsigned long long)(*this)[numElements - 1] <= x)
      return (*this)[numElements - 1];

    unsigned long long xHIGH = (x >> (64 - upperSize));
    unsigned long long xLOW = (x & lowerMASK());
    unsigned long long lower(0);
    if (xHIGH > 0)
      lower = upperBits.select(0, xHIGH - 1) - xHIGH + 1;
    unsigned long long upper = upperBits.select(0, xHIGH) - xHIGH;

    for (unsigned long long i(lower); i < upper; ++i) {
      if (getLowerAsInt(i) > xLOW) {
        return (*this)[i - 1];
      }
    }
    if (lower > 0 && getLowerAsInt(lower) > xLOW)
      return (*this)[lower - 1];
    return (*this)[upper];
  }

  // Acces the index'th element
  unsigned long long operator[](unsigned long long index) const {
    assert(valid_index(index));

    unsigned long long result = static_cast<unsigned long long>(
        ((unsigned long long)upperBits.select(1, index) - index)
        << (64 - upperSize));

    // std::cout << upperBits.select(1, index) << "\t" << (upperBits.select(1,
    // index)-index) << "\t" << std::bitset<64>(result) << std::endl;

    return static_cast<unsigned long long>(
        result | static_cast<unsigned long long>(getLowerAsInt(index)));
  }

  inline unsigned long long getLowerAsInt(unsigned long long index) const {
    assert(valid_index(index));
    unsigned long long lower = 0;
    for (unsigned long long i = 0; i < lowerSize(); ++i) {
      assert(index * lowerSize() + i < lowerBits.size());
      lower |= lowerBits[index * lowerSize() + i];
      lower <<= 1;
    }
    lower >>= 1;
    return lower;
  }

  inline void printInfo() const noexcept {
    std::cout << "Elias-Fano Stats:\n";
    std::cout << "Upper-Size:            " << upperSize << std::endl;
    std::cout << "Lower-Size:            " << lowerSize() << std::endl;
    std::cout << "Elements:              " << size() << std::endl;
    std::cout << "Space-Consumption:     " << totalSizeByte() << std::endl;
  }

  inline unsigned long long lowerSize() const { return 64 - upperSize; }

  inline unsigned long long upperMASK() const { return ~(lowerMASK()); }

  inline unsigned long long lowerMASK() const {
    return (unsigned long long)(~0) >> upperSize;
  }

  inline bool valid_index(unsigned long long index) const {
    return 0 <= index && index < numElements;
  }

  unsigned long long totalSizeByte() const noexcept {
    unsigned long long total(0);

    total += upperBits.totalSizeByte();
    total += lowerBits.totalSizeByte();
    total += sizeof(numElements);
    total += sizeof(upperSize);

    return total;
  }

  unsigned long long size() const { return numElements; }
};