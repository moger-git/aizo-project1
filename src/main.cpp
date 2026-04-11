#include <chrono>
#include <iostream>
#include <string>
#include <type_traits>

#include "lib/Parameters.h"

#include "structures/DynamicArray.h"
#include "structures/SinglyLinkedList.h"
#include "structures/DoublyLinkedList.h"

#include "io/FileLoader.h"
#include "sorting/QuickSorter.h"
#include "sorting/ShellSorter.h"
#include "sorting/BucketSorter.h"
#include "utils/Validator.h"

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

template<typename T>
int run_single_test_by_structure() {
    switch (Parameters::structure) {
        case Parameters::Structures::array:
            return run_single_test_typed<T, DynamicArray<T>>();

        case Parameters::Structures::singleList:
            return run_single_test_typed<T, SinglyLinkedList<T>>();

        case Parameters::Structures::doubleList:
            return run_single_test_typed<T, DoublyLinkedList<T>>();

        default:
            std::cout << "Error: unsupported structure in single test mode.\n";
            return 1;
    }
}

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

int run_benchmark() {
    std::cout << "Benchmark mode not implemented yet.\n";
    std::cout << "Results file: " << Parameters::resultsFile << "\n";
    std::cout << "Iterations: " << Parameters::iterations << "\n";
    std::cout << "Structure size: " << Parameters::structureSize << "\n";
    return 0;
}

int main(int argc, char** argv) {
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