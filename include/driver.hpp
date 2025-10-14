#pragma once

#include <iostream>
#include <vector>
#include <limits>
#include <utility>

namespace driver_utils
{
    template <typename T>
    T read_elem (std::istream& input, const std::string& message)
    {
        T value{};

        while (!(input >> value))
        {
            std::cerr << message << std::endl;
            std::cin.clear();
            std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
        }

        return value;
    }

    inline std::pair<size_t, std::vector<int>> read_input ()
    {
        size_t cache_size = read_elem<size_t> (std::cin, "ERROR: incorrect input - enter a positive integer.");
        size_t numb_elems = read_elem<size_t> (std::cin, "ERROR: incorrect input - enter a positive integer.");

        std::vector<int> sequence (numb_elems);
        for (size_t i = 0; i < numb_elems; i++)
            sequence[i] = read_elem<int> (std::cin, "ERROR: incorrect input - enter a integer.");

        return {cache_size, sequence};
    }
}
