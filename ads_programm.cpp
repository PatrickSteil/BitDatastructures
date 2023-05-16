#include <iostream>
#include <vector>

#include "EliasFano.h"

int main() {

	std::vector<int> numbers = {1, 2, 4, 6, 32, 33, 40, 56};
	EF::EliasFano ef(numbers, 64);

	ef.printInfo();
	return 0;
}