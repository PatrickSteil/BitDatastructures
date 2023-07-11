#include <cmath>
#include <limits>
#include <vector>

class NRMQ {
public:
  NRMQ(const std::vector<uint64_t> &A) : A(A) {
    assert(A.size() > 0);
    s = std::ceil(std::log2(A.size()) / 4);
    m = std::floor(A.size() / s);
    buildDatastructure();
    // std::cout
    //     << "*****************\nDone building datastructures\n*****************"
    //     << std::endl;
  };

  inline uint64_t rmq(uint64_t i, uint64_t j) const {
    assertSandEinRange(i, j);

    if (i == j) [[unlikely]]
      return 0;

    uint64_t bestMin(std::numeric_limits<uint64_t>::max());
    uint64_t result(i);

    // get range from [Left-inside-block:next-block-start] and
    // [previous-block-start:right-inside-block]
    uint64_t leftMostStartingBlock = std::ceil(i / (double)s);
    uint64_t rightMostStartingBlock = std::floor(j / (double)s);

    // check if distance too small or inside same block
    if (leftMostStartingBlock >= rightMostStartingBlock || j - i < s) [[unlikely]] {
      for (uint64_t index(i); index <= j; ++index) {
        if (A[index] < bestMin) [[unlikely]] {
          bestMin = A[index];
          result = index;
        }
      }
      return result - i;
    }

    bestMin = B[leftMostStartingBlock];
    result = BPos[leftMostStartingBlock];

    for (uint64_t blockIndex(leftMostStartingBlock + 1);
         blockIndex < rightMostStartingBlock; ++blockIndex) {
      if (B[blockIndex] < bestMin) [[unlikely]] {
        bestMin = B[blockIndex];
        result = BPos[blockIndex];
      }
    }

    for (uint64_t index(i); index <= (leftMostStartingBlock * s); ++index) {
      if (A[index] < bestMin) [[unlikely]] {
        bestMin = A[index];
        result = index;
      }
    }

    for (uint64_t index(rightMostStartingBlock * s); index <= j; ++index) {
      if (A[index] < bestMin) [[unlikely]] {
        bestMin = A[index];
        result = index;
      }
    }

    return result - i;
  }

  inline void assertSandEinRange(uint64_t &s, uint64_t &e) const {
    assert(0 <= s && s < A.size());
    assert(0 <= e && e < A.size());
    assert(s <= e);
  }

  inline size_t totalSizeByte() const noexcept {
    size_t total(0);
    total += A.size() * sizeof(A[0]);
    total += B.size() * sizeof(B[0]);
    total += BPos.size() * sizeof(BPos[0]);
    return total;
  }

private:
  inline void buildDatastructure() {
    B.assign(m + 1, std::numeric_limits<uint64_t>::max());
    BPos.assign(m + 1, 0);

    for (uint64_t i(0); i < A.size(); ++i) {
      if (A[i] < B[(uint64_t)i / s]) {
        B[(uint64_t)i / s] = A[i];
        BPos[(uint64_t)i / s] = i; // - ((uint64_t) i / s) * s;
      }
    }
  }

  const std::vector<uint64_t> A;
  std::vector<uint64_t> B;
  std::vector<uint64_t> BPos;
  uint64_t s;
  uint64_t m;
};