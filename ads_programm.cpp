#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "EliasFano/EliasFano.h"
// #include "RMQ/NaiveRMQ.h"
#include "RMQ/NLogNRMQ.h"
#include "Helper.h"

int main(int argc, char const *argv[]) {
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

  const char *TYPE_OF_PROGRAMM(argv[1]);
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
  assertmsg(n > 0, "n is not positive?");
  numbers.reserve(n);

  uint64_t curNumber = 0;
  for (uint64_t i(0); i < n; ++i) {
    inputFile >> curNumber;
    numbers.push_back(curNumber);
  }

  if (std::strcmp(TYPE_OF_PROGRAMM, "pd") == 0) {
    // elias fano encoding query
    std::vector<uint64_t> queries;
    queries.reserve(1024);
    uint64_t a = 0;
    while (inputFile >> a) {
      queries.push_back(a);
    }
    result.reserve(queries.size());

    EliasFanoEncoding ef(numbers);
    ef.printInfo();
    for (auto q : queries) {
      result.push_back(ef.pred(q));
    }

    // for (uint64_t i(0); i < queries.size(); ++i) {
    //   result.push_back(ef[numbers.size() - i - 1]);
    // }
    totalSpaceConsumption = ef.totalSizeByte();
  } else {
    if (std::strcmp(TYPE_OF_PROGRAMM, "rmq") == 0) {
      // range minimum query
      std::vector<std::pair<int, int>> queries;
      std::string line;
      int b(0);
      while (std::getline(inputFile, line)) {
        std::stringstream str(line);

        while (std::getline(str, line, ',') >> b) {
          queries.push_back(std::make_pair(std::stoi(line), b));
        }
      }
      result.reserve(queries.size());

      // creeate the Range Minimum Query Object
      NLogNRMQ RMQ(numbers);
      totalSpaceConsumption = RMQ.totalSizeByte();

      // execute the queries
      for (auto paar : queries) {
        result.push_back(RMQ.rmq(paar.first, paar.second));
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

  std::cout << "********************" << std::endl;
  std::cout << "RESULT algo=" << TYPE_OF_PROGRAMM
            << " namepatrick_steil time=" << ms_int.count()
            << " space=" << totalSpaceConsumption << std::endl;
  std::cout << "********************" << std::endl;
  return 0;
}