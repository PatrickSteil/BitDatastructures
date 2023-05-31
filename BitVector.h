#include "Helper.h"
// #include <bitset>
#include <cmath> // ceil
#include <iostream>
#include <vector>

namespace BIV {

class BitVector {

  using UNIT = uint64_t;

  struct DualSelect {
    DualSelect(uint16_t zeros, uint16_t ones) {
      vals[0] = zeros;
      vals[1] = ones;
    }

    DualSelect(uint16_t newVals[2]) {
      vals[0] = newVals[0];
      vals[1] = newVals[1];
    }
    uint16_t vals[2];
  };

public:
  BitVector(size_t numBits = 64)
      : numBits(numBits), blockSize(std::floor(std::log2(numBits) / 2.0)),
        superBlockSize(std::pow(blockSize, 2)),
        blockZeros(std::floor(numBits / blockSize), 0),
        superBlockZeros(std::floor(numBits / superBlockSize), 0),
        selectFastLookup(numBits, DualSelect(0, 0)) {
    assertmsg(numBits >= 0, "numBits should not be negative");
    bit_vector.assign(std::ceil(numBits / 64.0), 0);
    // initially everything is just a 0 => compute the prefix sum for block and
    // super block
    int sum = 0;
    for (size_t i(0); i < superBlockZeros.size(); ++i) {
      superBlockZeros[i] += sum;
      sum += superBlockSize;
    }
    sum = 0;
    for (size_t i(0); i < blockZeros.size(); ++i) {
      if (!(i % blockSize)) {
        sum = 0;
      }
      blockZeros[i] += sum;
      sum += blockSize;
    }
  };

  void printInfo() const noexcept {
    std::cout << "BitVector Stats:" << std::endl;
    std::cout << "Number of bits:      " << (int)numBits << std::endl;
    std::cout << "Number of uint64_t:  " << (int)bit_vector.size() << std::endl;
    std::cout << "Block Size:          " << (int)blockSize << std::endl;
    std::cout << "Super Block Size:    " << (int)superBlockSize << std::endl;
  }

  size_t totalSizeByte() const noexcept {
    size_t total(0);
    total += sizeof(numBits);
    total += sizeof(bit_vector);
    total += sizeof(blockSize);
    total += sizeof(superBlockSize);
    total += sizeof(blockZeros);
    total += sizeof(superBlockZeros);
    return total;
  }

  bool operator[](int i) const {
    assertmsg(0 <= i && (size_t)i < numBits, "index out of bounds!");
    UNIT block = bit_vector[i >> 6];
    return (block >> (i & 63)) & 1ULL;
  }

  void set(int i) {
    assertmsg(0 <= i && (size_t)i < numBits, "index out of bounds!");
    UNIT &block = bit_vector[i >> 6];
    block |= (1 << (i & 63));

    updateBlock(i, -1);
  }

  void unset(int i) {
    assertmsg(0 <= i && (size_t)i < numBits, "index out of bounds!");
    UNIT &block = bit_vector[i >> 6];
    block &= ~(1 << (i & 63));

    updateBlock(i, 1);
  }

  void buildSelectLookup() noexcept {
    uint16_t counter[2] = {0, 0};
    for (size_t i(1); i < numBits; ++i) {
      ++counter[(*this)[i]];
      selectFastLookup[i] = DualSelect(counter);
    }
  }

  size_t size() const noexcept { return numBits; }

  void printVector() const noexcept {
    std::cout << "Vector Content:\n";
    for (size_t i(0); i < numBits; ++i) {
      std::cout << (*this)[i];
    }
    std::cout << std::endl;
    std::cout << "Block Vector:\n";
    for (auto &val : blockZeros)
      std::cout << val << " ";
    std::cout << std::endl;
    std::cout << "Superblock Vector:\n";
    for (auto &val : superBlockZeros)
      std::cout << val << " ";
    std::cout << std::endl;
  }

  inline int selectTrue(int count) const { return select<true>(count); }

  inline int selectFalse(int count) const { return select<false>(count); }

  inline int rankFalse(int i) const {
    assertmsg(0 <= i && (size_t)i < numBits, "index out of bounds!");
    // if i is the index of a super block => done
    if (!(i % superBlockSize)) {
      return superBlockZeros[superBlockPos(i)];
    }

    int val = superBlockZeros[superBlockPos(i)] + blockZeros[blockPos(i)];

    // if it is the index of a block => also done
    if (!(i % blockSize)) {
      return val;
    }

    // otherwise invest time and sum up => don't want to create this lookup
    // table
    for (size_t pos(blockPos(i) * blockSize);
         pos < (blockPos(i) + 1) * blockSize; ++pos) {
      val += !((*this)[pos]);
    }
    return val;
  }

  inline int rankTrue(int i) const { return i - rankFalse(i); }

private:
  template <bool PARITY = true> inline int select(int count) const {
    return selectFastLookup[count].vals[PARITY];
    // // maybe try the O(1) version
    // // but here I just implement the binary search variant

    // assertmsg(0 <= count && (size_t)count < numBits, "count out of bounds!");
    // int low = 0;
    // int high = size() - 1;

    // while (low <= high) {
    //   int mid = (low + high) / 2;

    //   int rankVal = (PARITY ? rankTrue(mid) : rankFalse(mid));

    //   std::cout << low << "\t" << mid << "\t" << high << "\t" << rankVal
    //             << "\t" << count << "\t" << (*this)[mid] << "\n";

    //   if (count == rankVal && ((*this)[mid] == PARITY)) {
    //     return mid;
    //   } else {
    //     if (count > rankVal) {
    //       low = mid + 1;
    //     } else {
    //       high = mid - 1;
    //     }
    //   }
    // }

    // return -1;
  }

  inline int blockPos(int i) const {
    assertmsg(0 <= i && (size_t)i < numBits, "index out of bounds!");
    return std::floor(i / blockSize);
  }

  inline int superBlockPos(int i) const {
    return std::floor(i / superBlockSize);
  }

  inline void updateBlock(int i, int offset = 1) {
    assertmsg(0 <= i && (size_t)i < numBits, "index out of bounds!");
    // also decrease the 0 counter of block and all following super blocks and
    // block inside this super block
    int blockI = blockPos(i) + 1;
    // if it is in the last block => skip
    if (blockI < (int)blockZeros.size()) {
      assertmsg(0 <= blockI && blockI < (int)blockZeros.size(),
                "Index out of bounds");

      for (size_t j(blockI);
           j < blockSize * (std::floor(i / superBlockSize) + 1); ++j) {
        assertmsg(0 <= j && j < blockZeros.size(), "Index out of bounds");
        blockZeros[j] += offset;
      }
    }

    int superI = superBlockPos(i) + 1;
    if (superI < (int)superBlockZeros.size()) {
      assertmsg(0 <= superI && superI < (int)superBlockZeros.size(),
                "Index out of bounds");
      for (size_t j(superI); j < superBlockZeros.size(); ++j) {
        assertmsg(0 <= j && j < superBlockZeros.size(), "Index out of bounds");
        superBlockZeros[j] += offset;
      }
    }
  }

  size_t numBits;
  size_t blockSize;
  size_t superBlockSize;
  std::vector<int> blockZeros;
  std::vector<int> superBlockZeros;
  std::vector<UNIT> bit_vector;
  std::vector<DualSelect> selectFastLookup;
};
} // namespace BIV