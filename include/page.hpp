#pragma once

#include <iostream>

namespace page
{
    struct Page_t
    {
        int page_id_;
        std::string data_;
    };

    inline std::ostream& operator<< (std::ostream& ostream, const Page_t& page)
    {
        return ostream << "[Page " << page.page_id_ << ": " << page.data_ << "]";
    }

    inline Page_t slow_get_page (const int& key)
    {
        return {key, "Page number " + std::to_string(key) + " with some data :)"};
    }
}
