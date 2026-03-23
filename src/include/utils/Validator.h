#ifndef AIZO_PROJECT1_VALIDATOR_H
#define AIZO_PROJECT1_VALIDATOR_H

#include <cstddef>

template<typename Structure>
bool is_sorted_ascending(const Structure& data) {
    if (data.size() < 2) {
        return true;
    }

    for (std::size_t i = 1; i < data.size(); ++i) {
        if (data.at(i) < data.at(i - 1)) {
            return false;
        }
    }

    return true;
}

#endif //AIZO_PROJECT1_VALIDATOR_H