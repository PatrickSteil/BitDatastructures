#include <cmath>
#include <vector>

namespace RMQ {
class NLogNRMQ {
public:
  NLogNRMQ(const std::vector<uint64_t> &A) : A(A), M(A.size()) {
    assertmsg(A.size() > 0, "Array is empty??");
    buildValues();
  };

  inline uint64_t rmq(int s, int e) const {
    assertmsg(0 <= s && s < (int)A.size(),
              "s (the first index) is out of bounds!");
    assertmsg(0 <= e && e < (int)A.size(),
              "e (the second index) is out of bounds!");
    assertmsg(s <= e, "s should not be greater than e!");

    if (s == e) return s;
    if (s + 1 == e) return (A[s] < A[e] ? s : e);

    int l = std::floor(std::log2(e - s - 1));
    uint64_t m1 = subRMQ(s, s + (1 << l) - 1);
    uint64_t m2 = subRMQ(e - (1 << l) + 1, e);

    return (A[m1] < A[m2]) ? m1 : m2;
  }

  inline uint64_t subRMQ(int s, int e) const {
    assertmsg(0 <= s && s < (int)A.size(),
              "s (the first index) is out of bounds!");
    assertmsg(0 <= e && e < (int)A.size(),
              "e (the second index) is out of bounds!");
    assertmsg(s <= e, "s should not be greater than e!");
    return M[s][e];
  }

  inline size_t totalSizeByte() const noexcept {
    size_t total(0);
    total += sizeof(A);
    total += sizeof(M);
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
  inline void buildValues() {
    // pre allocate the space
    for (size_t i(0); i < M.size(); ++i) {
      M[i].assign(std::floor(std::log2(A.size())), i);
    }

    // M[x][l] = argmin{ A[i]: i \in {M[x][l - 1], M[x + 2**(l-1)][l-1]}}

    for (size_t l(1); l < M[0].size(); ++l) {
      for (size_t x(0); x < A.size(); ++x) {
        if ((size_t) x + (1 << (l - 1)) >= A.size()) continue;
        assertmsg((size_t) x + (1 << (l - 1)) < A.size(), "Something wrong?");
        M[x][l] = (A[M[x][l - 1]] < A[M[x + (1 << (l - 1))][l - 1]])
                      ? M[x][l - 1]
                      : M[x + (1 << (l - 1))][l - 1];
      }
    }
  }

  const std::vector<uint64_t> A;
  std::vector<std::vector<uint64_t>> M;
};
} // namespace RMQ