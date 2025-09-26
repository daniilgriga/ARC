#include <vector>
#include <arc.hpp>
#include <page.hpp>

int main ()
{
    size_t cache_size = 0;
    std::cin >> cache_size;

    size_t numb_items = 0;
    std::cin >> numb_items;

    ARC_t<Page_t, int> cache (cache_size);

    int key = 0;
    std::vector<int> vec = {};

    for (size_t i = 0; i < numb_items; i++)
    {
        std::cin >> key;
        vec.push_back (key);
    }

    int hits = 0;

    for (size_t i = 0; i < numb_items; i++)
        if (cache.lookup_update (vec[i], slow_get_page)) hits++;

    std::cout << hits << std::endl;

    return 0;
}
