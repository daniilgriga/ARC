#pragma once

#include <iostream>

struct Page_t
{
    int page_id_;
    std::string data_;

    Page_t (int id, const std::string& data) : page_id_(id), data_(data) {}
};

static Page_t slow_get_page (const int& key)
{
    return {key, "Page number " + std::to_string(key) + ": some data :)"};
}
