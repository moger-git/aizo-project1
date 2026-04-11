#ifndef AIZO_PROJECT1_SHELLSORTER_H
#define AIZO_PROJECT1_SHELLSORTER_H

#include <cstddef>
#include "lib/Parameters.h"

template<typename T>
class ShellSorter {
private:
    Parameters::ShellParameters parameter_;

    std::size_t first_gap_option1(std::size_t n) const {
        return n / 2;
    }

    std::size_t next_gap_option1(std::size_t current_gap) const {
        return current_gap / 2;
    }

    std::size_t first_gap_knuth(std::size_t n) const {
        std::size_t gap = 1;
        while (gap < n / 3) {
            gap = 3 * gap + 1;
        }
        return gap;
    }

    std::size_t next_gap_knuth(std::size_t current_gap) const {
        return current_gap / 3;
    }

    std::size_t first_gap_hibbard(std::size_t n) const {
        std::size_t gap = 1;
        while ((gap * 2 + 1) < n) {
            gap = gap * 2 + 1;
        }
        return gap;
    }

    std::size_t next_gap_hibbard(std::size_t current_gap) const {
        return current_gap / 2;
    }

    std::size_t first_gap_ciura(std::size_t n) const {
        static const std::size_t ciura[] = {1, 4, 10, 23, 57, 132, 301, 701, 1750};
        std::size_t best = 1;

        for (std::size_t gap : ciura) {
            if (gap < n) {
                best = gap;
            } else {
                break;
            }
        }

        if (best == 1 && n > 2) {
            return n / 2;
        }

        return best;
    }

    std::size_t next_gap_ciura(std::size_t current_gap) const {
        static const std::size_t ciura[] = {1, 4, 10, 23, 57, 132, 301, 701, 1750};

        if (current_gap <= 1) {
            return 0;
        }

        for (int i = 8; i >= 0; --i) {
            if (ciura[i] == current_gap) {
                return (i == 0) ? 0 : ciura[i - 1];
            }
        }

        return current_gap / 2;
    }

    std::size_t first_gap(std::size_t n) const {
        switch (parameter_) {
            case Parameters::ShellParameters::option1:
                return first_gap_option1(n);
            case Parameters::ShellParameters::option2:
                return first_gap_knuth(n);
            case Parameters::ShellParameters::option3:
                return first_gap_hibbard(n);
            case Parameters::ShellParameters::option4:
                return first_gap_ciura(n);
            default:
                return first_gap_option1(n);
        }
    }

    std::size_t next_gap(std::size_t current_gap) const {
        switch (parameter_) {
            case Parameters::ShellParameters::option1:
                return next_gap_option1(current_gap);
            case Parameters::ShellParameters::option2:
                return next_gap_knuth(current_gap);
            case Parameters::ShellParameters::option3:
                return next_gap_hibbard(current_gap);
            case Parameters::ShellParameters::option4:
                return next_gap_ciura(current_gap);
            default:
                return next_gap_option1(current_gap);
        }
    }

public:
    explicit ShellSorter(
        Parameters::ShellParameters parameter = Parameters::ShellParameters::option1
    ) : parameter_(parameter) {}

    template<typename Structure>
    void sort(Structure& data) {
        const std::size_t n = data.size();
        if (n < 2) {
            return;
        }

        for (std::size_t gap = first_gap(n); gap > 0; gap = next_gap(gap)) {
            for (std::size_t i = gap; i < n; ++i) {
                T temp = data.at(i);
                std::size_t j = i;

                while (j >= gap && data.at(j - gap) > temp) {
                    data.at(j) = data.at(j - gap);
                    j -= gap;
                }

                data.at(j) = temp;
            }

            if (gap == 1) {
                break;
            }
        }
    }
};

#endif //AIZO_PROJECT1_SHELLSORTER_H
