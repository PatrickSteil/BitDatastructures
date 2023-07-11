#include <algorithm>
#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include "EliasFano/EliasFano.h"
// #include "RMQ/NaiveRMQ.h"
// #include "RMQ/NLogNRMQ.h"
#include "RMQ/NRMQ.h"

bool isMinimumInRange(uint64_t resultRMQ, std::vector<uint64_t>& numbers,
    uint64_t i, uint64_t j)
{
    assert(i <= j);
    assert(j < numbers.size());
    uint64_t minElement(std::numeric_limits<uint64_t>::max());
    for (uint64_t k(i); k <= j; ++k) {
        if (numbers[k] < minElement) {
            minElement = numbers[k];
        }
    }
    return numbers[i + resultRMQ] == minElement;
}

int main(int argc, char const* argv[])
{
    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::milliseconds;

    if (argc != 4) {
        std::cout << "Not enough arguments!\nExample call: ./ads_programm [pd|rmq] "
                     "INPUT_FILE OUTPUT_FILE"
                  << std::endl;
        return -1;
    }

    auto t1 = high_resolution_clock::now();

    const char* TYPE_OF_PROGRAMM(argv[1]);
    std::ifstream inputFile(argv[2]);
    std::ofstream outputFile(argv[3]);
    std::vector<uint64_t> result;
    std::vector<uint64_t> numbers;

    if (!inputFile) {
        std::cout << "Failed to open the file." << std::endl;
        return -1;
    }

    uint64_t totalSpaceConsumption(0);
    uint64_t n;
    inputFile >> n;
    assert(n > 0);
    numbers.reserve(n);

    uint64_t curNumber = 0;
    for (uint64_t i(0); i < n; ++i) {
        inputFile >> curNumber;
        numbers.push_back(curNumber);
    }

    if (std::strcmp(TYPE_OF_PROGRAMM, "pd") == 0) {
        // elias fano encoding query
        std::vector<uint64_t> queries;
        queries.reserve(100000);
        uint64_t a = 0;
        while (inputFile >> a) {
            queries.push_back(a);
        }
        result.reserve(queries.size());

        EliasFano ef(numbers);
        // ef.printInfo();
        /* curNumber = 0; */
        for (auto q : queries) {
            /* if ((curNumber % 100) == 0) */
            /*     std::cout << curNumber << std::endl; */
            /* ++curNumber; */
            result.push_back(ef.pred(q));
        }
        totalSpaceConsumption = ef.totalSizeByte();
    } else {
        if (std::strcmp(TYPE_OF_PROGRAMM, "rmq") == 0) {
            // range minimum query
            std::vector<std::pair<uint64_t, uint64_t>> queries;
            queries.reserve(10000);
            std::string line;
            uint64_t b(0);
            while (std::getline(inputFile, line)) {
                std::stringstream str(line);

                while (std::getline(str, line, ',') >> b) {
                    queries.push_back(std::make_pair(std::stoi(line), b));
                }
            }
            result.reserve(queries.size());

            // creeate the Range Minimum Query Object
            NRMQ RMQ(numbers);
            // RMQ.printInfo();
            totalSpaceConsumption = RMQ.totalSizeByte();

            // execute the queries
            uint64_t resultRMQ(0);
            for (auto paar : queries) {
                resultRMQ = RMQ.rmq(paar.first, paar.second);
                // std::cout << paar.first << " " << paar.second << " " << resultRMQ <<
                // std::endl;
                assert(isMinimumInRange(resultRMQ, numbers, paar.first, paar.second) || !(printf("Result: %" PRIu64 ", i: %" PRIu64 ", j: %" PRIu64 "\n", resultRMQ, paar.first, paar.second)));
                result.push_back(resultRMQ);
            }
        } else {
            std::cout << "Your type of query (" << TYPE_OF_PROGRAMM
                      << ") is not supported!" << std::endl;
            return -1;
        }
    }

    auto t2 = high_resolution_clock::now();
    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    // write results to file => we dont time this writing
    for (auto r : result)
        outputFile << r << std::endl;
    outputFile.close();

    // std::cout << "********************" << std::endl;
    std::cout << "RESULT algo=" << TYPE_OF_PROGRAMM
              << " name=patrick_steil time=" << ms_int.count()
              << " space=" << (totalSpaceConsumption << 3) << std::endl;
    // std::cout << "********************" << std::endl;
    return 0;
}
