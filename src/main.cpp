#include <chrono>
#include <iostream>
#include <string>
#include <type_traits>

#include "lib/Parameters.h"

#include "structures/DynamicArray.h"
#include "structures/SinglyLinkedList.h"
#include "structures/DoublyLinkedList.h"
#include "structures/Stack.h"
#include "structures/BinaryTree.h"

#include "io/FileLoader.h"
#include "sorting/QuickSorter.h"
#include "sorting/ShellSorter.h"
#include "sorting/BucketSorter.h"
#include "utils/Validator.h"
#include "io/CsvLogger.h"
#include <limits>
#include <random>

//Sprawdzenie czy istnieje dodatkowa zmienna dla badania B
std::string get_distribution_from_env() {
    const char* value = std::getenv("AIZO_DISTRIBUTION");
    if (value == nullptr) {
        return "random";
    }
    return std::string(value);
}

//Wybieranie algorytmu sortowanie
template<typename T, typename Structure>
int sort_selected_algorithm(Structure& data, long long& sort_time_us) {
    auto start = std::chrono::high_resolution_clock::now();

    switch (Parameters::algorithm) {
    case Parameters::Algorithms::quick: {
            QuickSorter<T> sorter;
            sorter.sort(data);
            break;
    }

    case Parameters::Algorithms::shell: {
            ShellSorter<T> sorter(Parameters::shellParameter);
            sorter.sort(data);
            break;
    }

    case Parameters::Algorithms::bucket: {
            if constexpr (std::is_arithmetic_v<T>) {
                BucketSorter<T> sorter;
                sorter.sort(data);
            } else {
                std::cout << "Error: bucket sort works only for numeric types.\n";
                return 1;
            }
            break;
    }

    default:
        std::cout << "Error: unsupported algorithm.\n";
        return 1;
    }

    auto end = std::chrono::high_resolution_clock::now();
    sort_time_us =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    return 0;
}

//Wykonanie pojedynczego testu dla wybranego typu i struktury
template<typename T, typename Structure>
int run_single_test_typed() {
    Structure data;

    if (!FileLoader<T, Structure>::load(Parameters::inputFile, data)) {
        std::cout << "Error: cannot load input file: " << Parameters::inputFile << "\n";
        return 1;
    }

    long long sort_time_us = 0;

    int sort_result = sort_selected_algorithm<T>(data, sort_time_us);
    if (sort_result != 0) {
        return sort_result;
    }

    if (!is_sorted_ascending(data)) {
        std::cout << "Error: sorting validation failed.\n";
        return 1;
    }

    if (!Parameters::outputFile.empty()) {
        if (!FileLoader<T, Structure>::save(Parameters::outputFile, data)) {
            std::cout << "Error: cannot save output file: " << Parameters::outputFile << "\n";
            return 1;
        }
    }

    std::cout << "Single test finished successfully.\n";
    std::cout << "Elements: " << data.size() << "\n";
    std::cout << "Sorting time: " << sort_time_us << " us\n";

    if (!Parameters::outputFile.empty()) {
        std::cout << "Output file: " << Parameters::outputFile << "\n";
    }

    return 0;
}

//Wybieranie struktury dla pojedynczego testu
template<typename T>
int run_single_test_by_structure() {
    switch (Parameters::structure) {
        case Parameters::Structures::array:
            return run_single_test_typed<T, DynamicArray<T>>();

        case Parameters::Structures::singleList:
            return run_single_test_typed<T, SinglyLinkedList<T>>();

        case Parameters::Structures::doubleList:
            return run_single_test_typed<T, DoublyLinkedList<T>>();

        case Parameters::Structures::stack:
            return run_single_test_typed<T, Stack<T>>();

        case Parameters::Structures::binaryTree:
            return run_single_test_typed<T, BinaryTree<T>>();

        default:
            std::cout << "Error: unsupported structure in single test mode.\n";
            return 1;
    }
}

//Wybieranie typu danych dla pojedynczego testu
int run_single_test() {
    switch (Parameters::dataType) {
        case Parameters::DataTypes::typeInt:
            return run_single_test_by_structure<int>();

        case Parameters::DataTypes::typeDouble:
            return run_single_test_by_structure<double>();

        case Parameters::DataTypes::typeUnsignedInt:
            return run_single_test_by_structure<unsigned int>();

        case Parameters::DataTypes::typeString:
            return run_single_test_by_structure<std::string>();

        default:
            std::cout << "Error: unsupported data type in single test mode.\n";
            return 1;
    }
}

//=================================

//Zamiana wartości enum do tekstu (algorytmy)
std::string algorithm_to_string() {
    switch (Parameters::algorithm) {
        case Parameters::Algorithms::bucket: return "bucket";
        case Parameters::Algorithms::quick:  return "quick";
        case Parameters::Algorithms::shell:  return "shell";
        default: return "unknown";
    }
}

//Zamiana wartości do tekstu (struktury)
std::string structure_to_string() {
    switch (Parameters::structure) {
        case Parameters::Structures::array:      return "array";
        case Parameters::Structures::singleList: return "singleList";
        case Parameters::Structures::doubleList: return "doubleList";
        case Parameters::Structures::queue:      return "queue";
        case Parameters::Structures::stack:      return "stack";
        case Parameters::Structures::binaryTree: return "binaryTree";
        default: return "unknown";
    }
}

//Zamiana wartości do tekstu (typy)
std::string datatype_to_string() {
    switch (Parameters::dataType) {
        case Parameters::DataTypes::typeInt:          return "int";
        case Parameters::DataTypes::typeFloat:        return "float";
        case Parameters::DataTypes::typeDouble:       return "double";
        case Parameters::DataTypes::typeChar:         return "char";
        case Parameters::DataTypes::typeString:       return "string";
        case Parameters::DataTypes::typeUnsignedInt:  return "unsigned int";
        case Parameters::DataTypes::typeUnsignedLong: return "unsigned long";
        case Parameters::DataTypes::typeUnsignedChar: return "unsigned char";
        default: return "unknown";
    }
}

//Zamiana wartości do tekstu (pivoty, opcji)
std::string sort_parameter_to_string() {
    if (Parameters::algorithm == Parameters::Algorithms::quick) {
        switch (Parameters::pivot) {
            case Parameters::Pivots::random: return "pivot=random";
            case Parameters::Pivots::left:   return "pivot=left";
            case Parameters::Pivots::right:  return "pivot=right";
            case Parameters::Pivots::middle: return "pivot=middle";
            default: return "pivot=none";
        }
    }

    if (Parameters::algorithm == Parameters::Algorithms::shell) {
        switch (Parameters::shellParameter) {
            case Parameters::ShellParameters::option1: return "gap=option1";
            case Parameters::ShellParameters::option2: return "gap=option2";
            case Parameters::ShellParameters::option3: return "gap=option3";
            case Parameters::ShellParameters::option4: return "gap=option4";
            default: return "gap=none";
        }
    }

    return "none";
}

//Wypełnienie struktury losowymi danymi
template<typename T, typename Structure>
void fill_random_data(Structure& data, const std::size_t size, std::mt19937& rng) {
    data.clear();

    if constexpr (std::is_same_v<T, int>) {
        std::uniform_int_distribution dist(
            std::numeric_limits<int>::min(),
            std::numeric_limits<int>::max()
        );

        for (std::size_t i = 0; i < size; ++i) {
            data.push_back(dist(rng));
        }

    } else if constexpr (std::is_same_v<T, unsigned int>) {
        std::uniform_int_distribution dist(
            std::numeric_limits<unsigned int>::min(),
            std::numeric_limits<unsigned int>::max()
        );

        for (std::size_t i = 0; i < size; ++i) {
            data.push_back(dist(rng));
        }

    } else if constexpr (std::is_same_v<T, double>) {
        std::uniform_real_distribution dist(
            std::numeric_limits<double>::lowest(),
            std::numeric_limits<double>::max()
        );

        for (std::size_t i = 0; i < size; ++i) {
            data.push_back(dist(rng));
        }

    } else if constexpr (std::is_same_v<T, std::string>) {
        std::uniform_int_distribution lenDist(1, 100);
        std::uniform_int_distribution charDist(0, 25);

        for (std::size_t i = 0; i < size; ++i) {
            const int len = lenDist(rng);
            std::string s;

            for (int j = 0; j < len; ++j) {
                s.push_back(static_cast<char>('a' + charDist(rng)));
            }

            data.push_back(s);
        }
    }
}

//Badania dla wybranego typu i struktury
template<typename T, typename Structure>
int run_benchmark_typed() {
    std::mt19937 rng(std::random_device{}());

    long long min_us = std::numeric_limits<long long>::max();
    long long max_us = 0;
    long long sum_us = 0;

    for (int i = 0; i < Parameters::iterations; ++i) {
        Structure data;
        fill_random_data<T>(data, static_cast<std::size_t>(Parameters::structureSize), rng);

        long long sort_time_us = 0;
        int sort_result = sort_selected_algorithm<T>(data, sort_time_us);
        if (sort_result != 0) {
            return sort_result;
        }

        if (!is_sorted_ascending(data)) {
            std::cout << "Error: sorting validation failed in iteration " << i << "\n";
            return 1;
        }

        if (sort_time_us < min_us) {
            min_us = sort_time_us;
        }
        if (sort_time_us > max_us) {
            max_us = sort_time_us;
        }
        sum_us += sort_time_us;
    }

    //Tworzemie struktury statystyk do zapisania do .csv
    BenchmarkStatistics stats;
    stats.dateOfSort = CsvLogger::now_string();
    stats.sortMethod = algorithm_to_string();
    stats.sortParameter = sort_parameter_to_string();
    stats.variableType = structure_to_string();
    stats.dataType = datatype_to_string();
    stats.dataDistribution = "random";
    stats.arrayLength = static_cast<std::size_t>(Parameters::structureSize);
    stats.tries = Parameters::iterations;
    stats.minSortDuration = min_us;
    stats.maxSortDuration = max_us;
    stats.averageSortDuration = static_cast<double>(sum_us) / Parameters::iterations;
    stats.meanSortDuration = stats.averageSortDuration;

    if (!CsvLogger::append(Parameters::resultsFile, stats)) {
        std::cout << "Error: cannot write CSV file: " << Parameters::resultsFile << "\n";
        return 1;
    }

    //Wypisanie wyników badania
    std::cout << "Benchmark finished.\n";
    std::cout << "algorithm      = " << stats.sortMethod << "\n";
    std::cout << "parameter      = " << stats.sortParameter << "\n";
    std::cout << "structure      = " << stats.variableType << "\n";
    std::cout << "type           = " << stats.dataType << "\n";
    std::cout << "size           = " << stats.arrayLength << "\n";
    std::cout << "iterations     = " << stats.tries << "\n";
    std::cout << "min(us)        = " << stats.minSortDuration << "\n";
    std::cout << "max(us)        = " << stats.maxSortDuration << "\n";
    std::cout << "average(us)    = " << stats.averageSortDuration << "\n";
    std::cout << "csv            = " << Parameters::resultsFile << "\n";

    return 0;
}

//Wybieranie struktury dla badania
template<typename T>
int run_benchmark_by_structure() {
    switch (Parameters::structure) {
    case Parameters::Structures::array:
        return run_benchmark_typed<T, DynamicArray<T>>();
    case Parameters::Structures::singleList:
        return run_benchmark_typed<T, SinglyLinkedList<T>>();
    case Parameters::Structures::doubleList:
        return run_benchmark_typed<T, DoublyLinkedList<T>>();
    case Parameters::Structures::stack:
        return run_benchmark_typed<T, Stack<T>>();
    case Parameters::Structures::binaryTree:
        return run_benchmark_typed<T, BinaryTree<T>>();
    default:
        std::cout << "Error: unsupported structure in benchmark mode.\n";
        return 1;
    }
}

//Wybieranie typu danych dla badania
int run_benchmark() {
    switch (Parameters::dataType) {
    case Parameters::DataTypes::typeInt:
        return run_benchmark_by_structure<int>();

    case Parameters::DataTypes::typeDouble:
        return run_benchmark_by_structure<double>();

    case Parameters::DataTypes::typeUnsignedInt:
        return run_benchmark_by_structure<unsigned int>();

    case Parameters::DataTypes::typeString:
        return run_benchmark_by_structure<std::string>();

    default:
        std::cout << "Error: unsupported data type in benchmark mode.\n";
        return 1;
    }
}

//Pomóc
void print_project_help() {
    std::cout
        << "\n"
        << "==================== PROJECT HELP ====================\n"
        << "\n"
        << "This project supports:\n"
        << "  - single file sorting mode\n"
        << "  - benchmark mode\n"
        << "  - research scripts for alpha, A, B, C, omega\n"
        << "\n"
        << "Implemented algorithms:\n"
        << "  4 - Bucket sort\n"
        << "  5 - Quick sort\n"
        << "  6 - Shell sort\n"
        << "\n"
        << "Implemented structures:\n"
        << "  0 - Array\n"
        << "  1 - Single linked list\n"
        << "  2 - Double linked list\n"
        << "  4 - Stack\n"
        << "  5 - Binary tree\n"
        << "\n"
        << "Implemented data types:\n"
        << "  0 - int\n"
        << "  2 - double\n"
        << "  4 - string\n"
        << "  5 - unsigned int\n"
        << "\n"
        << "Quick sort pivot values:\n"
        << "  0 - random\n"
        << "  1 - left\n"
        << "  2 - right\n"
        << "  3 - middle\n"
        << "\n"
        << "Shell sort parameter values:\n"
        << "  0 - option1\n"
        << "  1 - option2\n"
        << "  2 - option3\n"
        << "  3 - option4\n"
        << "\n"
        << "======================================================\n"
        << "SINGLE FILE MODE\n"
        << "======================================================\n"
        << "Sort values from input file and save them to output file.\n"
        << "\n"
        << "Example:\n"
        << "  ./cmake-build-debug/aizo-project1 --singleFile \\\n"
        << "      --inputFile data/data.txt \\\n"
        << "      --outputFile data/sorted.txt \\\n"
        << "      -a 5 -s 0 -t 0 -p 3\n"
        << "\n"
        << "Meaning:\n"
        << "  - quick sort\n"
        << "  - array\n"
        << "  - int\n"
        << "  - middle pivot\n"
        << "\n"
        << "======================================================\n"
        << "BENCHMARK MODE\n"
        << "======================================================\n"
        << "Generate benchmark data, repeat sorting many times,\n"
        << "measure sort time in microseconds, validate output,\n"
        << "and append results to CSV.\n"
        << "\n"
        << "Example:\n"
        << "  ./cmake-build-debug/aizo-project1 --benchmark \\\n"
        << "      -a 5 -s 0 -t 0 -p 3 \\\n"
        << "      -l 10000 -n 20 -r results/benchmark.csv\n"
        << "\n"
        << "Meaning:\n"
        << "  - quick sort\n"
        << "  - array\n"
        << "  - int\n"
        << "  - middle pivot\n"
        << "  - 10000 elements\n"
        << "  - 100 iterations\n"
        << "  - results saved to CSV file\n"
        << "\n"
        << "======================================================\n"
        << "PYTHON RESEARCH SCRIPTS\n"
        << "======================================================\n"
        << "Use these scripts from project root:\n"
        << "\n"
        << "  python3 run_alpha.py\n"
        << "      -> results/benchmark_alpha.csv\n"
        << "\n"
        << "  python3 run_A.py\n"
        << "      -> results/benchmark_A.csv\n"
        << "\n"
        << "  python3 run_B.py\n"
        << "      -> results/benchmark_B.csv\n"
        << "\n"
        << "  python3 run_C.py\n"
        << "      -> results/benchmark_C.csv\n"
        << "\n"
        << "  python3 run_omega.py\n"
        << "      -> results/benchmark_omega.csv\n"
        << "\n"
        << "======================================================\n"
        << "RESEARCH DESCRIPTION\n"
        << "======================================================\n"
        << "alpha:\n"
        << "  Influence of algorithm parameters.\n"
        << "  - quick sort: compare pivots\n"
        << "  - shell sort: compare shell parameters\n"
        << "\n"
        << "A:\n"
        << "  Influence of data size.\n"
        << "  Compare at least 4 different sizes.\n"
        << "\n"
        << "B:\n"
        << "  Influence of data distribution.\n"
        << "  Compare random / descending / ascending / halfSorted.\n"
        << "\n"
        << "C:\n"
        << "  Influence of data type.\n"
        << "  Compare int / double / unsigned int / string.\n"
        << "\n"
        << "omega:\n"
        << "  One chosen algorithm, one chosen case,\n"
        << "  compare 5 structures:\n"
        << "    array, single list, double list, stack, binary tree.\n"
        << "\n"
        << "======================================================\n"
        << "INPUT FILE FORMAT\n"
        << "======================================================\n"
        << "Example data file:\n"
        << "  5\n"
        << "  10\n"
        << "  -2\n"
        << "  7\n"
        << "  0\n"
        << "  99\n"
        << "\n"
        << "First line = number of elements.\n"
        << "Next lines = values.\n"
        << "\n"
        << "======================================================\n";
}

int main(int argc, char** argv) {
    //Odczytanie parametrów
    int returnCode = Parameters::readParameters(argc - 1, argv + 1);
    if (returnCode != 0) {
        std::cout << "failed to parse parameters\n";
        return returnCode;
    }

    if (Parameters::runMode == Parameters::RunModes::help) {
        Parameters::help();
        return 0;
    }

    if (Parameters::runMode == Parameters::RunModes::singleFile) {
        return run_single_test();
    }

    if (Parameters::runMode == Parameters::RunModes::benchmark) {
        return run_benchmark();
    }

    std::cout << "Error: unknown run mode.\n";
    return 1;
}