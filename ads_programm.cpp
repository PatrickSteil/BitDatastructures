#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Pred/EliasFano.h"
#include "RMQ/NaiveRMQ.h"

int main(int argc, char const *argv[]) {
  using std::chrono::duration;
  using std::chrono::duration_cast;
  using std::chrono::high_resolution_clock;
  using std::chrono::milliseconds;

  if (argc != 4) {
    std::cout << "Not enough arguments!\nExample call: ads_programm [pd|rmq] "
                 "eingabe_datei ausgabe_datei"
              << std::endl;
    return (-1);
  }

  auto t1 = high_resolution_clock::now();

  // read the input file
  const char *TYPE_OF_PROGRAMM(argv[1]);
  std::ifstream inputFile(argv[2]);
  std::ofstream outputFile(argv[3]);
  std::vector<uint64_t> result;
  std::vector<uint64_t> numbers;

  size_t totalSpaceConsumption(0);

  int n;
  inputFile >> n;
  numbers.reserve(n);

  uint64_t curNumber = 0;
  for (int i(0); i < n; ++i) {
    inputFile >> curNumber;
    numbers.push_back(curNumber);
  }

  if (std::strcmp(TYPE_OF_PROGRAMM, "pd") == 0) {
    std::vector<int> queries;
    queries.reserve(1024);
    int a = 0;
    while (inputFile >> a) {
      queries.push_back(a);
    }
    result.reserve(queries.size());

    EF::EliasFano ef(numbers);
    for (auto q : queries) {
      result.push_back(ef.pred(q));
    }
    totalSpaceConsumption = (ef.totalSizeByte() << 3);
  } else {
    if (std::strcmp(TYPE_OF_PROGRAMM, "rmq") == 0) {
      std::vector<std::pair<int, int>> queries;
      std::string line;
      int a(0);
      int b(0);
      while (std::getline(inputFile, line)) {
        std::stringstream str(line);

        while (std::getline(str, line, ',') >> b) {
          a = std::stoi(line);
          queries.push_back(std::make_pair(a, b));
        }
      }
      result.reserve(queries.size());

      RMQ::NaiveRMQ naive(numbers);
      totalSpaceConsumption = (naive.totalSizeByte() << 3);

      for (auto paar : queries) {
        result.push_back(naive.rmq(paar.first, paar.second));
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
            << " namepatrick_steil time=" << ms_int.count()
            << " space=" << totalSpaceConsumption << std::endl;
  // std::cout << "********************" << std::endl;
  return 0;
}