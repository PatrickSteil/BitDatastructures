#include <tgmath.h>
#include <cmath>	// ceil and log2
#include <vector>
#include "Assert.h"
#include "BitVector.h"

namespace EF {
class EliasFano {
public: 
	EliasFano(std::vector<int> numbers = {}, size_t u = (~0)) :
		u(u),
		numElements(numbers.size()),
		upperSize(std::ceil(std::log2(numElements))),
		lowerSize(std::ceil(std::log2(u / numElements))),
		upper((numElements << 1) + 1),
		lower(numElements * lowerSize) {
			processNumbers(numbers);
	};



	void printInfo() const noexcept {
		std::cout << "Elias-Fano Encoding:" << std::endl;
		std::cout << "Number of elements:  " << numElements << std::endl;
		std::cout << "MaxNumber:           " << u << std::endl;
		std::cout << "UpperSize:           " << upperSize << std::endl;
		std::cout << "LowerSize:           " << lowerSize << std::endl;

		std::cout << "Information about the 'upper':" << std::endl;
		upper.printInfo();
		std::cout << "Information about the 'lower':" << std::endl;
		lower.printInfo();
	}

private:
	inline void processNumbers(std::vector<int>& numbers) {
		assertmsg(upper.size() == (numbers.size() << 1) + 1, "Upper does not have the right size!");
		for (size_t i(0); i < numbers.size(); ++i) {
			// represent pi with a 1 at position i + pi
			int p_i = (numbers[i] & ((~0) << lowerSize)) >> lowerSize;
			upper.set(i + p_i);
		}
	}

	size_t u;
	size_t numElements;

	int upperSize;
	int lowerSize;
	BIV::BitVector upper;
	BIV::BitVector lower;
};
}