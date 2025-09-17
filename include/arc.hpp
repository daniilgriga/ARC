#pragma once

#include <iostream>
#include <unordered_map>
#include <list>
#include <cstddef>

template <typename T, typename KeyT = int>
class ARC_t
{
    size_t size_;
    size_t param_;

    using ListNodeType = typename std::pair<KeyT, T>;
    using ListIter = typename std::list<ListNodeType>::iterator;

    std::list<ListNodeType> t1_list_;                           // active
    std::list<ListNodeType> t2_list_;                           //   lists

    std::unordered_map<KeyT, ListIter> t1_map_;                 // hash tables
    std::unordered_map<KeyT, ListIter> t2_map_;                 //   for active lists

    using ListIter_KeyT = typename std::list<KeyT>::iterator;

    std::list<KeyT> b1_list_;                                   // 'ghost'
    std::list<KeyT> b2_list_;                                   //   lists

    std::unordered_map<KeyT, ListIter_KeyT> b1_map_;            // hash tables
    std::unordered_map<KeyT, ListIter_KeyT> b2_map_;            //   for ghost lists

    size_t t1_size() const { return t1_list_.size(); }
    size_t t2_size() const { return t2_list_.size(); }
    size_t b1_size() const { return b1_list_.size(); }
    size_t b2_size() const { return b2_list_.size(); }

    size_t L1_size() const { return t1_size() + b1_size(); }    // L1 = T1 + B1
    size_t L2_size() const { return t2_size() + b2_size(); }    // L2 = T2 + B2

    void evict_from_t1 ()
    {
        if (t1_list_.empty()) return;

        auto node = t1_list_.begin();
        KeyT key = node->first;

        t1_map_.erase (key);
        t1_list_.erase (node);

        b1_list_.push_front (node);
        b1_map_[key] = b1_list_.begin();
    }

    void evict_from_t2 ()
    {
        if (t2_list_.empty()) return;

        auto node = t2_list_.begin();
        KeyT key = node->first;

        t2_map_.erase (key);
        t2_list_.erase (node);

        b2_list_.push_front (node);
        b2_map_[key] = b2_list_.begin();
    }

    void update_param (bool hit_in_B1)
    {
        if (hit_in_B1)
        {
            size_t delta = std::max<size_t>(1, b2_list_.size() / b1_list_.size());
            param_ = std::min(param_ + delta, size_);
        }
        else
        {
            size_t delta = std::max<size_t>(1, b1_list_.size() / b2_list_.size());
            param_ = std::max(param_ - delta, size_t{0});
        }
    }

    //  ARC logical operations

public:
    ARC_t (size_t size) : size_(size), param_(0) {};            // ctor

};
