#include <cmath>
#include <vector>

class NLogNRMQ {
public:
  NLogNRMQ(const std::vector<uint64_t> &A) : A(A), M(A.size()) {
    assert(A.size() > 0);
    buildDatastructure();
  };

  inline uint64_t rmq(uint64_t s, uint64_t e) const {
    assertSandEinRange(s, e);

    // some edge cases
    if (s == e)
      return 0;

    uint64_t l = std::floor(std::log2(e - s + 1));

    // now we only perform two lookups => take the samller index
    if (A[M[s][l]] <= A[M[e - (1 << l) + 1][l]])
      return M[s][l] - s;
    return M[e - (1 << l) + 1][l] - s;
  }

  inline void assertSandEinRange(uint64_t &s, uint64_t &e) const {
    assert(s < A.size());
    assert(e < A.size());
    assert(s <= e);
  }

  inline size_t totalSizeByte() const noexcept {
    size_t total(0);
    total += A.size() * sizeof(A[0]);
    total += M.size() * sizeof(M[0]);
    return total;
  }

  inline void printValues() const noexcept {
    for (size_t x(0); x < A.size(); ++x) {
      for (size_t l(0); l < M[x].size(); ++l) {
        std::cout << M[x][l] << "\t";
      }
      std::cout << std::endl;
    }
  }

private:
  inline void buildDatastructure() {
    // pre allocate the space
    for (size_t i(0); i < M.size(); ++i) {
      M[i].assign(std::ceil(std::log2(A.size())), i);
    }

    // M[x][l] = argmin{ A[i]: i \in {M[x][l - 1], M[x + 2**(l-1)][l-1]}}

    for (size_t l(1); (1 << l) <= A.size(); ++l) {
      for (size_t i(0); (i + (1 << l) - 1) < A.size(); ++i) {
        if (A[M[i][l - 1]] < A[M[i + (1 << (l - 1))][l - 1]])
          M[i][l] = M[i][l - 1];
        else
          M[i][l] = M[i + (1 << (l - 1))][l - 1];
      }
    }
  }

  const std::vector<uint64_t> A;
  std::vector<std::vector<uint64_t>> M;
};
