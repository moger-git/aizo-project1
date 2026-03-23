#ifndef AIZO_PROJECT1_FILELOADER_H
#define AIZO_PROJECT1_FILELOADER_H

#include <fstream>
#include <sstream>
#include <string>

template<typename T>
bool parse_value(const std::string& line, T& value) {
    std::istringstream iss(line);
    iss >> value;
    return iss && iss.eof();
}

template<>
inline bool parse_value<std::string>(const std::string& line, std::string& value) {
    value = line;
    return true;
}

template<typename T, typename Structure>
class FileLoader {
public:
    static bool load(const std::string& path, Structure& structure) {
        std::ifstream file(path);
        if (!file.is_open()) {
            return false;
        }

        structure.clear();

        std::size_t count = 0;
        if (!(file >> count)) {
            return false;
        }

        std::string line;
        std::getline(file, line);

        for (std::size_t i = 0; i < count; ++i) {
            if (!std::getline(file, line)) {
                return false;
            }

            T value{};
            if (!parse_value<T>(line, value)) {
                return false;
            }

            structure.push_back(value);
        }

        return true;
    }

    static bool save(const std::string& path, const Structure& structure) {
        std::ofstream file(path);
        if (!file.is_open()) {
            return false;
        }

        file << structure.size() << '\n';

        for (std::size_t i = 0; i < structure.size(); ++i) {
            file << structure.at(i) << '\n';
        }

        return true;
    }
};

#endif //AIZO_PROJECT1_FILELOADER_H