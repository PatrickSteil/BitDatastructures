#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>

#include "EliasFano.h"

int main(int argc, char const* argv[])
{
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
    std::vector<int> numbers;
    std::vector<int> queries;
    std::ifstream inputFile(argv[2]);

    int n;
    inputFile >> n;
    numbers.reserve(n);

    int lc = 0;
    int curNumber = 0;
    char buf[2048];
    do {
        inputFile.read(buf, sizeof(buf));
        int k = inputFile.gcount();
        for (int i = 0; i < k; ++i) {
            switch (buf[i]) {
            case '\r':
                break;
            case '\n':
                curNumber = 0;
                lc++;
                break;
            case ' ':
                curNumber = 0;
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                curNumber = 10 * curNumber + buf[i] - '0';
                break;
            default:
                std::cerr << "Bad format\n";
            }
        }
        numbers.push_back(curNumber);
        --n;
    } while (n >= 0);

    do {
        inputFile.read(buf, sizeof(buf));
        int k = inputFile.gcount();
        for (int i = 0; i < k; ++i) {
            switch (buf[i]) {
            case '\r':
                break;
            case '\n':
                curNumber = 0;
                lc++;
                break;
            case ' ':
                curNumber = 0;
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                curNumber = 10 * curNumber + buf[i] - '0';
                break;
            default:
                std::cerr << "Bad format\n";
            }
        }
        queries.push_back(curNumber);
        --n;
    } while (inputFile);

    EF::EliasFano ef(numbers);

    auto t2 = high_resolution_clock::now();
    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    std::cout << "RESULT algo=" << argv[1]
              << " namepatrick_steil time=" << ms_int.count()
              << " space=" << (ef.totalSizeByte() << 3) << std::endl;
    return 0;
}