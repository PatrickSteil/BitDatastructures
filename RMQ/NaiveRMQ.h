#include <vector>

namespace RMQ {
class NaiveRMQ {
public:
  NaiveRMQ(const std::vector<uint64_t> &A)
      : numValues(A.size()), values(numValues * numValues) {
    buildValues(A);
  };

  inline uint64_t rmq(int s, int e) const {
    assertmsg(0 <= s && s < (int)numValues,
              "s (the first index) is out of bounds!");
    assertmsg(0 <= e && e < (int)numValues,
              "e (the second index) is out of bounds!");
    assertmsg(s <= e, "s should not be greater than e!");

    if (s == e)
      return s;
    return values[s * numValues + e];
  }

  inline size_t totalSizeByte() const noexcept {
    size_t total(0);
    total += sizeof(numValues);
    total += sizeof(values);
    return total;
  }

  void printInfo() const noexcept {
    for (size_t i(0); i < numValues; ++i) {
      for (size_t j(0); j < numValues; ++j) {
        std::cout << (int)values[i * numValues + j] << "\t";
      }
      std::cout << std::endl;
    }
  }

private:
  inline void buildValues(const std::vector<uint64_t> &A) {
    assertmsg(A.size() > 0, "Array is empty??");
    for (size_t i(0); i < numValues; ++i) {
      values[i * numValues + i] = i;
      for (size_t j(i + 1); j < numValues; ++j) {
        values[i * numValues + j] = (A[values[i * numValues + j - 1]] > A[j]
                                         ? j
                                         : values[i * numValues + j - 1]);
      }
    }
  }

  size_t numValues;
  std::vector<uint64_t> values;
};
} // namespace RMQ