#pragma once

#include <iostream>
#include <vector>
#include <limits>
#include <utility>

struct CacheInput_t
{
    size_t cache_size;
    std::vector<int> sequence;
};

template <typename T>
T read_elem (const std::string& message)
{
    T value{};

    std::cin >> value;
    if (std::cin)
    {
        return value;
    }
    else
    {
        std::cerr << message << std::endl;
        std::cin.clear();
        std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
    }

    return T{}; // for delete warning [-Wreturn-type]
}

CacheInput_t read_input ()
{
    size_t cache_size = read_elem<size_t> ("ERROR: incorrect input - enter a positive integer.");
    size_t numb_elems = read_elem<size_t> ("ERROR: incorrect input - enter a positive integer.");

    std::vector<int> sequence (numb_elems);
    for (size_t i = 0; i < numb_elems; i++)
        sequence[i] = read_elem<int> ("ERROR: incorrect input - enter a integer.");

    return {cache_size, std::move(sequence)};
}
