#include "../Helper.h"
// #include <bitset>
#include <cmath> // ceil
#include <iostream>
#include <vector>
#include <climits>

namespace StraightFWDBIV {

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

class BitVector {
private:
  std::vector<bool> bits;
  std::vector<DualSelect> selectFastLookup;
  std::vector<uint64_t> rankTable;

public:
  BitVector(uint64_t size)
      : bits(size), selectFastLookup(bits.size(), DualSelect(0, 0)),
        rankTable(bits.size() + 1) {}

  inline bool operator[](int i) const {
    assert((uint64_t)i < bits.size());
    return bits[i];
  }

  void printInfo() const noexcept {
    std::cout << "BitVector Stats:          " << std::endl;
    std::cout << "Number of bits:           " << (int)bits.size() << std::endl;
    std::cout << "Space Consumption:        " << totalSizeByte() << std::endl;
    // std::cout << "Number of uint64_t:  " << (int)bit_vector.size() <<
    // std::endl; std::cout << "Block Size:          " << (int)blockSize <<
    // std::endl; std::cout << "Super Block Size:    " << (int)superBlockSize <<
    // std::endl;
  }
  void printVector() const noexcept {
    std::cout << "Vector Content:\n";
    for (uint64_t i(0); i < bits.size(); ++i) {
      std::cout << (*this)[i];
    }
    std::cout << std::endl;
  }

  inline uint64_t totalSizeByte() const {
    uint64_t total(0);
    total += bits.size() * CHAR_BIT;
    total += selectFastLookup.size() * sizeof(DualSelect);
    total += rankTable.size() * sizeof(uint64_t);
    return total;
  }

  inline bool getBit(uint64_t i) const {
    assert((uint64_t)i < bits.size());
    return bits[i];
  }

  inline void unset(uint64_t i) {
    assert((uint64_t)i < bits.size());
    set(i, false);
  }

  inline void set(uint64_t i) {
    assert((uint64_t)i < bits.size());
    set(i, true);
  }

  inline void set(uint64_t i, bool value) {
    assert((uint64_t)i < bits.size());
    bits[i] = value;
  }

  void buildSelectLookup() noexcept {
    uint16_t counter[2] = {0, 0};
    for (uint64_t i(1); i < bits.size(); ++i) {
      ++counter[(*this)[i]];
      selectFastLookup[i] = DualSelect(counter);
    }
  }

  void buildRankTable() {
    uint64_t count = 0;
    for (uint64_t i = 0; i < bits.size(); ++i) {
      count += bits[i];
      rankTable[i + 1] = count;
    }
  }

  uint64_t size() const noexcept { return bits.size(); }

  inline uint64_t selectTrue(int count) const { return select<true>(count); }

  inline uint64_t selectFalse(int count) const { return select<false>(count); }

  inline uint64_t rankFalse(int i) const { return rank<false>(i); }

  inline uint64_t rankTrue(int i) const { return rank<true>(i); }

private:
  template <bool PARITY = true> inline uint64_t rank(uint64_t i) const {
    assert((uint64_t)i <= bits.size());
    if (PARITY)
      return rankTable[i];
    else
      return i - rankTable[i];
  }

  template <bool PARITY = true> inline uint64_t select(int count) const {
    return selectFastLookup[count].vals[PARITY];
    // // maybe try the O(1) version
    // // but here I just implement the binary search variant

    // assert(0 <= count && (uint64_t)count < bits.size(), "count out of
    // bounds!"); int low = 0; int high = size() - 1;

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
};
} // namespace StraightFWDBIV