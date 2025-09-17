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

    size_t T_size()  const { return t1_size() + t2_size(); }    // T = T1 + T2

    void evict_from_T1_to_B1 ()
    {
        if (t1_list_.empty()) return;

        auto node = t1_list_.begin();
        KeyT key = node->first;

        t1_map_.erase (key);
        t1_list_.erase (node);

        b1_list_.push_front (key);
        b1_map_[key] = b1_list_.begin();
    }

    void evict_from_T2_to_B2 ()
    {
        if (t2_list_.empty()) return;

        auto node = t2_list_.begin();
        KeyT key = node->first;

        t2_map_.erase (key);
        t2_list_.erase (node);

        b2_list_.push_front (key);
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

    void replace ()
    {
        if (!b2_map_.empty() && (t1_size() >= param_ || (t1_size() == param_ && !b2_map_.empty())))
            evict_from_T1_to_B1 ();
        else if (!b1_map_.empty())
            evict_from_T2_to_B2 ();
        else
            evict_from_T1_to_B1 ();
    }

    void move_from_T1_to_T2 (KeyT key, T value)
    {
        auto node_in_T1 = t1_map_.find (key);                   // node_in_T1 (std::unordered_map<KeyT, ListIter>::iterator)
        if (node_in_T1 != t1_map_.end())
        {
            t1_list_.erase (node_in_T1->second);                // node_in_T1->second (list<...>::iterator)
            t1_map_.erase (node_in_T1);
        }

        t2_list_.push_front ({key, value});
        t2_map_[key] = t2_list_.begin();
    }

    void control_ghost_sizes ()
    {
        while (L1_size() > size_)
        {
            auto node = b1_list_.back();
            b1_map_.erase (node);
            b1_list_.pop_back();
        }

        while (L2_size() > size_)
        {
            auto node = b2_list_.back();
            b2_map_.erase (node);
            b2_list_.pop_back();
        }
    }

public:
    ARC_t (size_t size) : size_(size), param_(0) {};            // ctor

    void put (KeyT key, T value)
    {
        if (t1_map_.count (key))                                // 1. check T1
        {
            move_from_T1_to_T2 (key, value);
            return;
        }

        if (t2_map_.count (key))                                // 2. check T2
        {
            auto node = t2_map_[key];
            t2_list_.erase (node);
            t2_list_.push_front ({key, value});
            t2_map_[key] = t2_list_.begin();
            return;
        }

        if (T_size() >= size_)                                  // 3. cache is full?
            replace();

        t1_list_.push_front ({key, value});                     // 4. added to T1
        t1_map_[key] = t1_list_.begin();

        control_ghost_sizes();                                  // 5. check size of ghost lists
    }

};
