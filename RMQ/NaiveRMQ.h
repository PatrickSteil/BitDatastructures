#include <vector>

class NaiveRMQ {
public:
  NaiveRMQ(const std::vector<uint64_t> &A)
      : numValues(A.size()), values(numValues * numValues) {
    buildDatastructure(A);
  };

  inline uint64_t rmq(int s, int e) const {
    assert(0 <= s && s < (int)numValues);
    assert(0 <= e && e < (int)numValues);
    assert(s <= e);

    if (s == e)
      return s;
    return values[s * numValues + e] - s;
  }

  inline size_t totalSizeByte() const noexcept {
    size_t total(0);
    total += sizeof(numValues);
    total += values.size() * sizeof(values[0]);
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
  inline void buildDatastructure(const std::vector<uint64_t> &A) {
    assert(A.size() > 0);
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