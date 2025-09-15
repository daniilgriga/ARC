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
    std::list<KeyT> b2_list_;                                   //    lists

    std::unordered_map<KeyT, ListIter_KeyT> b1_map_;            // hash tables
    std::unordered_map<KeyT, ListIter_KeyT> b2_map_;            //   for ghost lists

    size_t t1_size() const { return t1_list_.size(); }
    size_t t2_size() const { return t2_list_.size(); }
    size_t b1_size() const { return b1_list_.size(); }
    size_t b2_size() const { return b2_list_.size(); }

    size_t L1_size()    { return t1_size() + b1_size(); }       // L1 = T1 + B1
    size_t L2_size()    { return t2_size() + b2_size(); }       // L2 = T2 + B2

    // put in cache
    // move in ... to ...
    // update param_
    // summary -> need ARC logical operations

public:
    ARC_t (size_t size) : size_(size), param_(0) {};            // ctor

};
