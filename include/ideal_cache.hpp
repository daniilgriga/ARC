#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <cstddef>

template <typename T, typename KeyT = int>
class IdealCache_t
{
    size_t size_;
    size_t curr_position_;

    using PositionMap = std::unordered_map<KeyT, std::vector<int>>;
    using CacheStore  = std::unordered_map<KeyT, T>;

    PositionMap keys_positions_;
    CacheStore cache_;

    static constexpr int NO_NEXT_ = -1;

    int find_next_occurrence (const KeyT& key, int curr_index)
    {
        std::vector<int>& vec = keys_positions_[key];
        auto iter = std::lower_bound (vec.begin(), vec.end(), curr_index + 1);  // binary searchng

        return (iter == vec.end() ? NO_NEXT_ : *iter);
    }

public:
    IdealCache_t (int size)
    {
        if (size < 0)
            throw std::invalid_argument ("Cache size cannot be negative");

        size_ = static_cast<size_t>(size);
        curr_position_ = 0;
    }

    void set_sequence (const std::vector<KeyT>& sequence)
    {
        keys_positions_.clear();

        for (size_t i = 0; i < sequence.size(); i++)
            keys_positions_[sequence[i]].push_back(i);
    }

    template <typename FuncT>
    bool lookup_update (const KeyT& key, FuncT get_page)
    {
        if (size_ == 0)
            return false;

        size_t curr_pos = curr_position_++;

        if (cache_.find (key) != cache_.end())
            return true;                                                        // HIT

        T page = get_page (key);

        int next_for_new = find_next_occurrence (key, curr_pos);
        if (next_for_new == NO_NEXT_)
            return false;

        if (cache_.size() < size_)
        {
            cache_.try_emplace (key, page);
            return false;                                                       // MISS
        }

        KeyT key_for_evict = 0;
        int latest_position = NO_NEXT_;

        for (auto iter = cache_.begin(); iter != cache_.end(); iter++)
        {
            const KeyT& key_in_cache = iter->first;

            int next_position = find_next_occurrence (key_in_cache, curr_pos);
            if (next_position == NO_NEXT_)
            {
                key_for_evict = key_in_cache;
                break;
            }

            if (next_position > latest_position)
            {
                latest_position = next_position;
                key_for_evict = key_in_cache;
            }
        }

        cache_.erase (key_for_evict);
        cache_.try_emplace (key, page);

        return false;
    }
};
