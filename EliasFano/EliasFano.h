#include "../BitVector/BitVector.h"
#include <bitset>
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>

class EliasFano {
private:
    BitVector upperBits;
    BitVector lowerBits;
    uint64_t numElements;
    uint64_t upperSize;
    uint8_t offset;

public:
    EliasFano(std::vector<uint64_t>& numbers)
        : numElements(numbers.size())
    {
        uint64_t maxElement = numbers[numElements - 1];
        offset = 64 - std::ceil(std::log2(maxElement));
        // old version: upperSize = std::floor(std::log2(numElements));
        upperSize = std::ceil(std::log2(numElements));

        // old version: std::vector<bool> upperBitsVEC((numElements << 1) + 1,
        // false);
        std::vector<bool> upperBitsVEC(numElements * 3 + 1, false);
        std::vector<bool> lowerBitsVEC(numElements * (64 - offset - upperSize) + 1, false);

        uint64_t i(0);

        for (uint64_t element : numbers) {
            uint64_t MSB = element >> (64 - offset - upperSize);
            uint64_t LSB = (element & lowerMASK());

            // assert((MSB + i) < ((numElements << 1) + 1));
            upperBitsVEC[MSB + i] = true;

            for (uint64_t j = 0; j < (64 - offset - upperSize); ++j) {
                lowerBitsVEC[i * (64 - offset - upperSize) + j] = ((LSB >> ((64 - offset - upperSize) - 1 - j)) & 1);
            }
            ++i;
        }

        upperBits.buildVector(upperBitsVEC);
        lowerBits.buildVector(lowerBitsVEC);

        // std::cout
        //     << "*****************\nDone building datastructures\n*****************"
        //     << std::endl;
    }

    // Predecessor Method
    uint64_t pred(uint64_t x) const
    {
        // see https://arxiv.org/pdf/2003.11835.pdf
        if ((uint64_t)(*this)[0] >= x) [[unlikely]]
            return std::numeric_limits<uint64_t>::max();
        if ((uint64_t)(*this)[numElements - 1] <= x) [[unlikely]]
            return (*this)[numElements - 1];

        uint64_t xHIGH = (x >> (64 - offset - upperSize));
        uint64_t xLOW = (x & lowerMASK());
        uint64_t lower(0);
        if (xHIGH > 0) [[likely]] {
            lower = upperBits.select(0, xHIGH - 1) - xHIGH + 1;
        }
        uint64_t upper = upperBits.select(0, xHIGH) - xHIGH;

        for (uint64_t i(lower); i < upper; ++i) {
            if (getLowerAsInt(i) > xLOW) {
                return (*this)[i - 1];
            }
        }
        if (lower > 0 && getLowerAsInt(lower) > xLOW)
            return (*this)[lower - 1];
        return (*this)[upper];
    }

    // Acces the index'th element
    uint64_t operator[](uint64_t index) const
    {
        assert(valid_index(index));

        uint64_t result = static_cast<uint64_t>(
            ((uint64_t)upperBits.select(1, index) - index)
            << (64 - offset - upperSize));

        return static_cast<uint64_t>(
            result | static_cast<uint64_t>(getLowerAsInt(index)));
    }

    inline uint64_t getLowerAsInt(uint64_t index) const
    {
        assert(valid_index(index));
        uint64_t lower = 0;
        for (uint64_t i = 0; i < lowerSize(); ++i) {
            assert(index * lowerSize() + i < lowerBits.size());
            lower |= lowerBits[index * lowerSize() + i];
            lower <<= 1;
        }
        lower >>= 1;
        return lower;
    }

    inline void printInfo() const noexcept
    {
        std::cout << "Elias-Fano Stats:\n";
        std::cout << "Upper-Size:            " << upperSize << std::endl;
        std::cout << "Lower-Size:            " << lowerSize() << std::endl;
        std::cout << "Elements:              " << size() << std::endl;
        std::cout << "Space-Consumption:     " << totalSizeByte() << std::endl;
    }

    inline uint64_t lowerSize() const { return 64 - offset - upperSize; }

    inline uint64_t upperMASK() const { return ~(lowerMASK()); }

    inline uint64_t lowerMASK() const
    {
        return (uint64_t)(~0) >> (upperSize);
    }

    inline bool valid_index(uint64_t index) const
    {
        return index < numElements;
    }

    inline uint64_t totalSizeByte() const noexcept
    {
        uint64_t total(0);

        total += upperBits.totalSizeByte();
        total += lowerBits.totalSizeByte();
        total += sizeof(numElements);
        total += sizeof(upperSize);
        total += sizeof(offset);
        return total;
    }

    uint64_t size() const { return numElements; }
};
