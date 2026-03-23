#ifndef AIZO_PROJECT1_QUICKSORTER_H
#define AIZO_PROJECT1_QUICKSORTER_H

#include <cstddef>

template<typename T>
class QuickSorter {
private:
    template<typename Structure>
    void swap_values(Structure& data, std::size_t i, std::size_t j) {
        T temp = data.at(i);
        data.at(i) = data.at(j);
        data.at(j) = temp;
    }

    template<typename Structure>
    std::size_t partition(Structure& data, std::size_t left, std::size_t right) {
        T pivot = data.at(right);
        std::size_t i = left;

        for (std::size_t j = left; j < right; ++j) {
            if (data.at(j) < pivot) {
                swap_values(data, i, j);
                ++i;
            }
        }

        swap_values(data, i, right);
        return i;
    }

    template<typename Structure>
    void quick_sort(Structure& data, std::size_t left, std::size_t right) {
        if (left >= right) {
            return;
        }

        std::size_t pivot_index = partition(data, left, right);

        if (pivot_index > 0) {
            quick_sort(data, left, pivot_index - 1);
        }
        quick_sort(data, pivot_index + 1, right);
    }

public:
    template<typename Structure>
    void sort(Structure& data) {
        if (data.size() > 1) {
            quick_sort(data, 0, data.size() - 1);
        }
    }
};


#endif //AIZO_PROJECT1_QUICKSORTER_H