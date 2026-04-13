#ifndef AIZO_PROJECT1_CSVLOGGER_H
#define AIZO_PROJECT1_CSVLOGGER_H

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <ctime>

struct BenchmarkStatistics {
    std::string dateOfSort;
    std::string sortMethod;
    std::string sortParameter;
    std::string variableType;
    std::string dataType;
    std::string dataDistribution;
    std::size_t arrayLength{};
    int tries{};
    long long minSortDuration{};
    long long maxSortDuration{};
    double averageSortDuration{};
    double meanSortDuration{};
};

class CsvLogger {
public:
    static std::string now_string() {
        const std::time_t now = std::time(nullptr);
        const std::tm* local = std::localtime(&now);

        std::ostringstream oss;
        oss << std::put_time(local, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    static bool append(const std::string& path, const BenchmarkStatistics& s) {
        std::filesystem::path resultsPath(path);

        if (std::filesystem::path parentPath = resultsPath.parent_path(); !parentPath.empty()) {
            std::error_code ec;
            std::filesystem::create_directories(parentPath, ec);
            if (ec) {
                return false;
            }
        }

        const bool fileExists = std::filesystem::exists(resultsPath);

        std::ofstream file(resultsPath, std::ios::app);
        if (!file.is_open()) {
            return false;
        }

        if (!fileExists) {
            file
                << "dateOfSort;sortMethod;sortParameter;variableType;dataType;"
                << "dataDistribution;arrayLength;tries;minSortDuration;"
                << "maxSortDuration;averageSortDuration;meanSortDuration\n";
        }

        file << s.dateOfSort << ';'
             << s.sortMethod << ';'
             << s.sortParameter << ';'
             << s.variableType << ';'
             << s.dataType << ';'
             << s.dataDistribution << ';'
             << s.arrayLength << ';'
             << s.tries << ';'
             << s.minSortDuration << ';'
             << s.maxSortDuration << ';'
             << std::fixed << std::setprecision(2) << s.averageSortDuration << ';'
             << std::fixed << std::setprecision(2) << s.meanSortDuration
             << '\n';

        return true;
    }
};

#endif //AIZO_PROJECT1_CSVLOGGER_H
