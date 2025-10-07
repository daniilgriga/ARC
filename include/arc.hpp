#pragma once

#include <iostream>
#include <unordered_map>
#include <list>
#include <cstddef>
#include <iomanip>
#include <cassert>

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
    size_t B_size()  const { return b1_size() + b2_size(); }    // B = B1 + B2

    void evict_from_T_to_B (std::list<ListNodeType>& t_list, std::unordered_map<KeyT, ListIter>&      t_map,
                            std::list<KeyT>&         b_list, std::unordered_map<KeyT, ListIter_KeyT>& b_map)
    {
        if (t_list.empty()) return;

        auto iter = --t_list.end();
        KeyT key = iter->first;

        t_map.erase (key);
        t_list.erase (iter);

        b_list.push_front (key);
        b_map[key] = b_list.begin();
    }

    void evict_from_T1_to_B1 ()
    {
        evict_from_T_to_B (t1_list_, t1_map_, b1_list_, b1_map_);
    }

    void evict_from_T2_to_B2 ()
    {
        evict_from_T_to_B (t2_list_, t2_map_, b2_list_, b2_map_);
    }

    void handle_with_b1 ()
    {
        size_t delta = 1;
        if (b1_list_.size() > 0)
            delta = std::max<size_t>(1, b2_list_.size() / b1_list_.size());
        param_ = std::min(param_ + delta, size_);
    }

    void handle_with_b2 ()
    {
        size_t delta = 1;
        if (b2_list_.size() > 0)
            delta = std::max<size_t>(1, b1_list_.size() / b2_list_.size());
        if (param_ >= delta)
            param_ -= delta;
        else
            param_ = 0;
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

    void move_from_T1_to_T2 (const KeyT& key, const T& value)
    {
        auto T1_iter = t1_map_.find (key);                      // T1_iter (std::unordered_map<KeyT, ListIter>::iterator)
        if (T1_iter != t1_map_.end())
        {
            t1_list_.erase (T1_iter->second);                   // T1_iter->second (list<...>::iterator)
            t1_map_.erase (T1_iter);
        }

        t2_list_.push_front ({key, value});
        t2_map_[key] = t2_list_.begin();
    }

    void control_ghost_sizes ()
    {
        while (L1_size() > size_)
        {
            auto iter = b1_list_.back();
            b1_map_.erase (iter);
            b1_list_.pop_back();
        }

        while (L2_size() > size_)
        {
            auto iter = b2_list_.back();
            b2_map_.erase (iter);
            b2_list_.pop_back();
        }
    }

    void handling_b_list (const KeyT& key, std::list<KeyT>& b_list, std::unordered_map<KeyT, ListIter_KeyT>& b_map, void (ARC_t::*handling_b_func)())
    {
        (this->*handling_b_func) ();
        auto iter = b_map.find (key);
        if (iter != b_map.end())
        {
            b_list.erase (iter->second);
            b_map.erase (iter);
        }
    }

    void put_without_ghosts_check (const KeyT& key, const T& value)
    {
        if (t1_map_.count (key))                                // 1. check T1
        {
            move_from_T1_to_T2 (key, value);
            return;
        }

        if (t2_map_.count (key))                                // 2. check T2
        {
            auto iter = t2_map_[key];
            t2_list_.erase (iter);
            t2_list_.push_front ({key, value});
            t2_map_[key] = t2_list_.begin();
            return;
        }

        if (T_size() >= size_)                                  // 3. cache is full?
        {
            // std::cout << "Cache is full --- need replace" << std::endl;
            replace();
        }

        t1_list_.push_front ({key, value});                     // 4. added to T1
        t1_map_[key] = t1_list_.begin();

        control_ghost_sizes();                                  // 5. check size of ghost lists
    }

public:
    ARC_t (int size)                                             // ctor
    {
        assert (size && "Cache size cannot be negative");

        size_ = static_cast<size_t>(size);
        param_ = 0;
    }

// ====== Methods for completeness of the user interface =====
    void put (const KeyT& key, T& value)
    {
        bool exist_b1 = b1_map_.count (key);
        bool exist_b2 = b2_map_.count (key);

        if (exist_b1)
            handling_b_list (key, b1_list_, b1_map_, &ARC_t::handle_with_b1);
        else if (exist_b2)
            handling_b_list (key, b2_list_, b2_map_, &ARC_t::handle_with_b2);

        put_without_ghosts_check (key, value);
    }

    bool get (const KeyT& key, T& value)
    {
        auto t1_iter = t1_map_.find (key);
        if (t1_iter != t1_map_.end())
        {
            value = t1_iter->second->second;
            move_from_T1_to_T2 (key, value);
            return true;
        }

        auto t2_iter = t2_map_.find (key);
        if (t2_iter != t2_map_.end())
        {
            value = t2_iter->second->second;
            t2_list_.erase (t2_iter->second);
            t2_list_.push_front ({key, value});
            t2_map_[key] = t2_list_.begin();

            return true;
        }

        return false;
    }
// ===========================================================

    template <typename FuncT>
    bool lookup_update (const KeyT& key, FuncT get_page)
    {
        if (size_ == 0)
            return false;

        if (t1_map_.count(key))                                 // HIT
        {
            T value = t1_map_[key]->second;
            move_from_T1_to_T2 (key, value);

            return true;
        }

        if (t2_map_.count(key))                                 // HIT
        {
            T value = t2_map_[key]->second;
            t2_list_.erase (t2_map_[key]);
            t2_list_.push_front ({key, value});
            t2_map_[key] = t2_list_.begin();

            return true;
        }

        bool exist_b1 = b1_map_.count (key);                    // MISS
        bool exist_b2 = b2_map_.count (key);

        if (exist_b1)
            handling_b_list (key, b1_list_, b1_map_, &ARC_t::handle_with_b1);
        else if (exist_b2)
            handling_b_list (key, b2_list_, b2_map_, &ARC_t::handle_with_b2);

        T value = get_page (key);
        put_without_ghosts_check (key, value);

        return false;
    }

    void dump ()                                                // just for visualisation of cache
    {
        std::cout << std::endl;
        std::cout << "======================================================" << std::endl;
        std::cout << "========== ARC DUMP" << " [size = " << std::setw(2) << size_ << "]" << " [param = " << param_ << "] ==========" << std::endl;
        std::cout << "Real items (T1 + T2): "  << T_size() << std::endl;
        std::cout << "Ghost items (B1 + B2): " << B_size() << std::endl;
        std::cout << std::endl;

        std::cout << "######## T1 (recently used, size = " << t1_size() << ") ##########" << std::endl;
        if (t1_list_.empty())
        {
            std::cout << "\t\t   [empty]" << std::endl;
        }
        else
        {
            int idx = 1;
            for (auto it = t1_list_.begin(); it != t1_list_.end(); ++it)
            {
                const auto& node = *it;
                std::cout << "    " << idx++ << ". key = " << std::setw(5) << node.first << " ---> value = \"" << node.second << "\"" << std::endl;
            }
        }

        std::cout << std::endl;

        std::cout << "######## T2 (frequently used, size = " << t2_size() << ") ########" << std::endl;
        if (t2_list_.empty())
        {
            std::cout << "\t\t   [empty]" << std::endl;
        }
        else
        {
            int idx = 1;
            for (auto it = t2_list_.begin(); it != t2_list_.end(); ++it)
            {
                const auto& node = *it;
                std::cout << "    " << idx++ << ". key = " << std::setw(5) << node.first << " ---> value = \"" << node.second << "\"" << std::endl;
            }
        }

        std::cout << "######## B1 (ghosts from T1, size = " << b1_size() << ")  ########" << std::endl;
        if (b1_list_.empty())
        {
            std::cout << "\t\t   [empty]" << std::endl;
        }
        else
        {
            int idx = 1;
            for (auto it = b1_list_.begin(); it != b1_list_.end(); ++it)
            {
                const auto& key = *it;
                std::cout << "    " << idx++ << ". key = " << std::setw(5) << key << std::endl;
            }
        }


        std::cout << "######## B2 (ghosts from T2, size = " << b2_size() << ")  ########" << std::endl;
        if (b2_list_.empty())
        {
            std::cout << "\t\t   [empty]" << std::endl;
        }
        else
        {
            int idx = 1;
            for (auto it = b2_list_.begin(); it != b2_list_.end(); ++it)
            {
                const auto& key = *it;
                std::cout << "    " << idx++ << ". key = " << std::setw(5) << key << std::endl;
            }
        }

        std::cout << "======================================================" << std::endl;
    }
};
