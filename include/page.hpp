#pragma once

#include <iostream>

struct Page_t
{
    int page_id_;
    std::string data_;
};

inline std::ostream& operator<< (std::ostream& ostream, const Page_t& page)
{
    return ostream << "[Page " << page.page_id_ << ": " << page.data_ << "]";
}

static Page_t slow_get_page (const int& key)
{
    return {key, "Page number " + std::to_string(key) + " with some data :)"};
}
