#include <iostream>
#include <vector>
#include <cmath> // ceil
#include "Assert.h"


namespace BIV {
class BitVector {

    using BLOCK = uint64_t;

public:
	BitVector(size_t numBits = 64) : 
		numBits(numBits),
		shiftBlockSize(6) {
			assertmsg(numBits >= 0, "numBits should not be negative");
		bit_vector.assign(std::ceil(numBits / 64.0), 0);
	};
	
	void printInfo() const noexcept {
	    std::cout << "BitVector Stats:" << std::endl;
	    std::cout << "Number of bits:      " << (int) numBits << std::endl;
	    std::cout << "Number of uint64_t:  " << (int) bit_vector.size() << std::endl;
	}
	
	bool operator [](int i) const {
		assertmsg(0 <= i && (size_t) i < numBits, "index out of bounds!");
	    BLOCK block = bit_vector[i >> shiftBlockSize];
	    return (block >> (i & 63)) &1ULL;
	}
	
	void set(int i) {
		assertmsg(0 <= i && (size_t) i < numBits, "index out of bounds!");
	    BLOCK& block = bit_vector[i >> shiftBlockSize];
	    block |= (1 << (i & 63));
	}
	
	void unset(int i) {
		assertmsg(0 <= i && (size_t) i < numBits, "index out of bounds!");
	    BLOCK& block = bit_vector[i >> shiftBlockSize];
	    block &= ~(1 << (i & 63));
	}
	
	size_t size() const noexcept {
	    return numBits;
	}

private:
	size_t numBits;
	int shiftBlockSize;
	std::vector<BLOCK> bit_vector;
};
}