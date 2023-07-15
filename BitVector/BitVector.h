#include <cassert>
#include <climits>
#include <iostream>
#include <vector>

class BitVector {
private:
  std::vector<bool> bits;
  std::vector<int> rankTable;
  std::vector<int> selectTableTrue;
  std::vector<int> selectTableFalse;

public:
  BitVector() {}

  BitVector(const std::vector<bool> &inputBits) { buildVector(inputBits); }

  bool operator[](uint64_t i) const {
    assert(i < bits.size());
    return bits[i];
  }

  void printVector() {
    std::cout << "*****************\nSize: " << bits.size() << "\n";
    for (auto b : bits) {
      std::cout << b;
    }
    std::cout << "\n*****************\n";
  }

  inline void buildVector(const std::vector<bool> &inputBits) {
    bits = inputBits;
    buildRankTable();
    buildSelectTable();
  }

  inline uint64_t size() const noexcept { return bits.size(); }

  inline bool valid_index(uint64_t index) const { return index < bits.size(); }

  size_t totalSizeByte() const noexcept {
    size_t total = 0;

    total += bits.size() * CHAR_BIT;
    total += rankTable.size() * sizeof(rankTable[0]);
    total += selectTableTrue.size() * sizeof(selectTableTrue[0]);
    total += selectTableFalse.size() * sizeof(selectTableFalse[0]);
    return total;
  }

  uint64_t rank(bool bit, uint64_t position) const {
    assert(valid_index(position));

    if (bit) {
      return rankTable[position];
    } else {
      return position - rankTable[position];
    }
  }

  inline uint64_t select(bool bit, uint64_t count) const {
    assert((!bit || count < selectTableTrue.size()) &&
           (bit || count < selectTableFalse.size()));

    if (bit) {
      return selectTableTrue[count];
    } else {
      return selectTableFalse[count];
    }
  }

private:
  inline void buildRankTable() {
    rankTable.resize(bits.size());

    uint64_t count = 0;
    for (uint64_t i = 0; i < bits.size(); ++i) {
      rankTable[i] = count;
      count += bits[i];
    }
  }

  inline void buildSelectTable() {
    selectTableTrue.resize(rankTable.back() + 1, -1);

    selectTableFalse.resize(rankTable.size() - rankTable.back() + 1, -1);

    for (uint64_t i = 0; i < bits.size(); ++i) {
      if (bits[i] && selectTableTrue[rankTable[i]] == -1) {
        selectTableTrue[rankTable[i]] = i;
      }
      if (not bits[i] && selectTableFalse[i - rankTable[i]] == -1) {
        selectTableFalse[i - rankTable[i]] = i;
      }
    }
  }
};
