#include <iostream>
#include <string>
#include <chrono>

#include "include/structures/DynamicArray.h"
#include "include/structures/SinglyLinkedList.h"
#include "include/structures/DoublyLinkedList.h"
#include "include/io/FileLoader.h"
#include "include/sorting/QuickSorter.h"
#include "include/utils/Validator.h"

enum class Mode {
    Help,
    SingleTest,
    Benchmark,
    Unknown
};

enum class StructureType {
    Array,
    SinglyList,
    DoublyList,
    Unknown
};

enum class AlgorithmType {
    Quick,
    Shell,
    Bucket,
    Unknown
};

enum class DataType {
    Int,
    DoubleType,
    UnsignedInt,
    StringType,
    Unknown
};

struct AppConfig {
    Mode mode = Mode::Help;
    StructureType structure = StructureType::Array;
    AlgorithmType algorithm = AlgorithmType::Quick;
    DataType data_type = DataType::Int;

    std::string input_file;
    std::string output_file;
    std::string results_file;

    int repeats = 1;
    std::size_t size = 0;
};

void print_help() {
    std::cout
        << "Usage:\n"
        << "  program help\n"
        << "  ./cmake-build-debug/aizo-project1 single <input> <output> <algorithm> <structure> <type>\n"
        << "  ./cmake-build-debug/aizo-project1 benchmark <results_csv> <algorithm> <structure> <type> <repeats> <size>\n\n"
        << "Examples:\n"
        << "  ./cmake-build-debug/aizo-project1 single ../data/data.txt ../data/sorted.txt quick array int\n"
        << "  ./cmake-build-debug/aizo-project1 benchmark ../results/results.csv quick array int 50 10000\n";
}

Mode parse_mode(const std::string& text) {
    if (text == "help") {
        return Mode::Help;
    }
    if (text == "single") {
        return Mode::SingleTest;
    }
    if (text == "benchmark") {
        return Mode::Benchmark;
    }
    return Mode::Unknown;
}

StructureType parse_structure(const std::string& text) {
    if (text == "array") {
        return StructureType::Array;
    }
    if (text == "singly") {
        return StructureType::SinglyList;
    }
    if (text == "doubly") {
        return StructureType::DoublyList;
    }
    return StructureType::Unknown;
}

AlgorithmType parse_algorithm(const std::string& text) {
    if (text == "quick") {
        return AlgorithmType::Quick;
    }
    if (text == "shell") {
        return AlgorithmType::Shell;
    }
    if (text == "bucket") {
        return AlgorithmType::Bucket;
    }
    return AlgorithmType::Unknown;
}

DataType parse_data_type(const std::string& text) {
    if (text == "int") {
        return DataType::Int;
    }
    if (text == "double") {
        return DataType::DoubleType;
    }
    if (text == "uint") {
        return DataType::UnsignedInt;
    }
    if (text == "string") {
        return DataType::StringType;
    }
    return DataType::Unknown;
}

bool load_config_from_args(int argc, char** argv, AppConfig& cfg) {
    if (argc < 2) {
        cfg.mode = Mode::Help;
        return true;
    }

    cfg.mode = parse_mode(argv[1]);

    if (cfg.mode == Mode::Help) {
        return true;
    }

    if (cfg.mode == Mode::SingleTest) {
        if (argc != 7) {
            return false;
        }

        cfg.input_file = argv[2];
        cfg.output_file = argv[3];
        cfg.algorithm = parse_algorithm(argv[4]);
        cfg.structure = parse_structure(argv[5]);
        cfg.data_type = parse_data_type(argv[6]);

        return cfg.algorithm != AlgorithmType::Unknown &&
               cfg.structure != StructureType::Unknown &&
               cfg.data_type != DataType::Unknown;
    }

    if (cfg.mode == Mode::Benchmark) {
        if (argc != 8) {
            return false;
        }

        cfg.results_file = argv[2];
        cfg.algorithm = parse_algorithm(argv[3]);
        cfg.structure = parse_structure(argv[4]);
        cfg.data_type = parse_data_type(argv[5]);
        cfg.repeats = std::stoi(argv[6]);
        cfg.size = static_cast<std::size_t>(std::stoull(argv[7]));

        return cfg.algorithm != AlgorithmType::Unknown &&
               cfg.structure != StructureType::Unknown &&
               cfg.data_type != DataType::Unknown &&
               cfg.repeats > 0 &&
               cfg.size > 0;
    }

    return false;
}

template<typename T, typename Structure>
int run_single_test_typed(const AppConfig& cfg) {
    Structure data;

    if (!FileLoader<T, Structure>::load(cfg.input_file, data)) {
        std::cout << "Error: cannot load input file: " << cfg.input_file << "\n";
        return 1;
    }

    std::cout << "Loaded " << data.size() << " elements:\n";
    data.print();

    auto start = std::chrono::high_resolution_clock::now();

    if (cfg.algorithm == AlgorithmType::Quick) {
        QuickSorter<T> sorter;
        sorter.sort(data);
    } else {
        std::cout << "Error: algorithm not implemented yet.\n";
        return 1;
    }
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    if (!is_sorted_ascending(data)) {
        std::cout << "Error: sorting validation failed.\n";
        return 1;
    }

    if (!cfg.output_file.empty()) {
        if (!FileLoader<T, Structure>::save(cfg.output_file, data)) {
            std::cout << "Error: cannot save output file: " << cfg.output_file << "\n";
            return 1;
        }
    }
    std::cout << "Sorted " << data.size() << " elements:\n";
    data.print();

    std::cout << "Single test finished successfully.\n";
    std::cout << "Sorting time: " << duration << " microsecond\n";
    return 0;
}

template<typename T>
int run_single_test_by_structure(const AppConfig& cfg) {
    switch (cfg.structure) {
        case StructureType::Array:
            return run_single_test_typed<T, DynamicArray<T>>(cfg);
        case StructureType::SinglyList:
            return run_single_test_typed<T, SinglyLinkedList<T>>(cfg);
        case StructureType::DoublyList:
            return run_single_test_typed<T, DoublyLinkedList<T>>(cfg);
        default:
            std::cout << "Error: unknown structure.\n";
            return 1;
    }
}

int run_single_test(const AppConfig& cfg) {
    switch (cfg.data_type) {
        case DataType::Int:
            return run_single_test_by_structure<int>(cfg);
        case DataType::DoubleType:
            return run_single_test_by_structure<double>(cfg);
        case DataType::UnsignedInt:
            return run_single_test_by_structure<unsigned int>(cfg);
        case DataType::StringType:
            return run_single_test_by_structure<std::string>(cfg);
        default:
            std::cout << "Error: unknown data type.\n";
            return 1;
    }
}

int run_benchmark(const AppConfig& cfg) {
    std::cout << "Benchmark mode is planned.\n";
    std::cout << "Results file: " << cfg.results_file << "\n";
    std::cout << "Repeats: " << cfg.repeats << "\n";
    std::cout << "Size: " << cfg.size << "\n";
    return 0;
}

int main(int argc, char** argv) {
    AppConfig cfg;

    if (!load_config_from_args(argc, argv, cfg)) {
        std::cout << "Error: invalid arguments.\n";
        print_help();
        return 1;
    }

    switch (cfg.mode) {
        case Mode::Help:
            print_help();
            return 0;
        case Mode::SingleTest:
            return run_single_test(cfg);
        case Mode::Benchmark:
            return run_benchmark(cfg);
        default:
            std::cout << "Error: unknown mode.\n";
            print_help();
            return 1;
    }
}