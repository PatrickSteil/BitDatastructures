#include "Assert.h"
#include "BitVector.h"
#include <cmath> // ceil and log2
// #include <tgmath.h>
#include <vector>

namespace EF {
class EliasFano {
public:
    EliasFano(std::vector<int> numbers = {})
        : numElements(numbers.size())
        , lowerSize(std::ceil(std::log2((~0) / numElements)))
        , upper((numElements << 1) + 1)
        , lower(numElements * lowerSize)
    {
        processNumbers(numbers);
    };

    void printInfo() const noexcept
    {
        std::cout << "Elias-Fano Encoding:" << std::endl;
        std::cout << "Number of elements:  " << numElements << std::endl;
        std::cout << "MaxNumber:           " << (~0) << std::endl;
        std::cout << "UpperSize:           " << std::ceil(std::log2(numElements)) << std::endl;
        std::cout << "LowerSize:           " << lowerSize << std::endl;

        std::cout << "Information about the 'upper':" << std::endl;
        upper.printInfo();
        std::cout << "Information about the 'lower':" << std::endl;
        lower.printInfo();
    }

    size_t totalSizeByte() const noexcept
    {
        size_t total(0);
        total += sizeof(numElements);
        total += sizeof(lowerSize);
        total += upper.totalSizeByte();
        total += lower.totalSizeByte();
        return total;
    }

private:
    inline void processNumbers(std::vector<int>& numbers)
    {
        assertmsg(upper.size() == (numbers.size() << 1) + 1, "Upper does not have the right size!");
        for (size_t i(0); i < numbers.size(); ++i) {
            // represent pi with a 1 at position i + pi
            int p_i = (numbers[i] & ((~0) << lowerSize)) >> lowerSize;
            upper.set(i + p_i);
        }
    }

    size_t numElements;

    int lowerSize;
    BIV::BitVector upper;
    BIV::BitVector lower;
};
}