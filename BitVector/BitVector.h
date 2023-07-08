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

  bool operator[](unsigned long long i) const {
    assert(0 <= i && i < bits.size());
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

  unsigned long long size() const noexcept { return bits.size(); }

  inline bool valid_index(unsigned long long index) const {
    return 0 <= index && index < bits.size();
  }

  size_t totalSizeByte() const noexcept {
    size_t total = 0;

    total += bits.size() * CHAR_BIT;
    total += sizeof(rankTable);
    total += sizeof(selectTableTrue);
    total += sizeof(selectTableFalse);
    return total;
  }

  unsigned long long rank(bool bit, unsigned long long position) const {
    assert(valid_index(position));

    if (bit) {
      return rankTable[position];
    } else {
      return position - rankTable[position];
    }
  }

  inline unsigned long long select(bool bit, unsigned long long count) const {
    assert(0 <= count and count < selectTableTrue.size());

    if (bit) {
      return selectTableTrue[count];
    } else {
      return selectTableFalse[count];
    }
  }

private:
  inline void buildRankTable() {
    rankTable.resize(bits.size());

    unsigned long long count = 0;
    for (unsigned long long i = 0; i < bits.size(); ++i) {
      rankTable[i] = count;
      count += bits[i];
    }
  }

  inline void buildSelectTable() {
    selectTableTrue.resize(rankTable.back() + 1, -1);

    selectTableFalse.resize(rankTable.size() - rankTable.back() + 1, -1);

    for (unsigned long long i = 0; i < bits.size(); ++i) {
      if (bits[i] && selectTableTrue[rankTable[i]] == -1) {
        selectTableTrue[rankTable[i]] = i;
      }
      if (not bits[i] && selectTableFalse[i - rankTable[i]] == -1) {
        selectTableFalse[i - rankTable[i]] = i;
      }
    }
  }
};