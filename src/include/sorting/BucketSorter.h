#ifndef AIZO_PROJECT1_BUCKETSORTER_H
#define AIZO_PROJECT1_BUCKETSORTER_H

#include <cstddef>
#include <type_traits>

#include "structures/DynamicArray.h"

template<typename T>
class BucketSorter {
    static_assert(std::is_arithmetic_v<T>,
                  "BucketSorter works only for numeric types.");

private:
    std::size_t bucket_count_;

    void insertion_sort(DynamicArray<T>& bucket) {
        for (std::size_t i = 1; i < bucket.size(); ++i) {
            T key = bucket.at(i);
            std::size_t j = i;

            while (j > 0 && bucket.at(j - 1) > key) {
                bucket.at(j) = bucket.at(j - 1);
                --j;
            }

            bucket.at(j) = key;
        }
    }

public:
    explicit BucketSorter(std::size_t bucket_count = 10)
        : bucket_count_(bucket_count) {}

    template<typename Structure>
    void sort(Structure& data) {
        if (data.size() < 2) {
            return;
        }

        T min_value = data.at(0);
        T max_value = data.at(0);

        for (std::size_t i = 1; i < data.size(); ++i) {
            if (data.at(i) < min_value) {
                min_value = data.at(i);
            }
            if (data.at(i) > max_value) {
                max_value = data.at(i);
            }
        }

        if (min_value == max_value) {
            return;
        }

        DynamicArray<T>* buckets = new DynamicArray<T>[bucket_count_];

        const long double min_ld = static_cast<long double>(min_value);
        const long double max_ld = static_cast<long double>(max_value);
        const long double range = max_ld - min_ld;

        for (std::size_t i = 0; i < data.size(); ++i) {
            const long double value_ld = static_cast<long double>(data.at(i));
            const long double normalized = (value_ld - min_ld) / range;

            std::size_t bucket_index =
                static_cast<std::size_t>(normalized * bucket_count_);

            if (bucket_index >= bucket_count_) {
                bucket_index = bucket_count_ - 1;
            }

            buckets[bucket_index].push_back(data.at(i));
        }

        std::size_t pos = 0;

        for (std::size_t i = 0; i < bucket_count_; ++i) {
            insertion_sort(buckets[i]);

            for (std::size_t j = 0; j < buckets[i].size(); ++j) {
                data.at(pos) = buckets[i].at(j);
                ++pos;
            }
        }

        delete[] buckets;
    }
};


#endif //AIZO_PROJECT1_BUCKETSORTER_H

