#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <cstdef>

template <typename T, typename KeyT = int>
class IdealCache
{
    size_t size_;

    using PositionMap = std::unordered_map<KeyT, std::vector<int>>;
    using CacheStore  = std::unordered_map<KeyT, T>;

    PositionMap keys_positions_;
    CacheStore cache_;

    static constexpr int NO_NEXT = -1;

    int find_next_occurrence (const KeyT& key, int curr_index)
    {
        std::vector<int>& vec = keys_positions_[key];
        auto iter = std::lower_bound (vec.begin(), vec.end(), curr_index + 1);  // binary searchng

        return (iter == vec.end() ? NO_NEXT : *iter);
    }

public:
    IdealCache (size_t size) : size_(size) {}

    void set_sequence (const std::vector<KeyT>& sequence)
    {
        keys_positions_.clear();

        for (size_t i = 1; i < sequence.size(); i++)
            keys_positions_[sequence[i]].push_back(i);
    }


};





/*

 1 2 3 4 3 2 1

 |1|-|

 |2|1|

 мы на тройке, скачиваем ------ алгоритм кеша: какое из 1, 2, 3 встретится позже всех - понимаем, что это единица - |3|2|

 на четверке, скачиваем ------ алгоритм кеша: она больше никогда не встретится, значит в кеш не кладем - |3|2|

 3 --- hit

 2 --- hit

 1, скачиваем ------ уже на кеш все равно

*/
