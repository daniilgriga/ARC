#include <ideal_cache.hpp>
#include <page.hpp>

int main ()
{
    std::cout << "Ideal cache starting..." << std::endl;

    IdealCache_t<Page_t, int> cache (2);

    std::vector<int> sequence = {1, 2, 3, 4, 3, 2, 1};
    cache.set_sequence (sequence);

    int hits = 0;

    int key =  1;
    if (cache.lookup_update (key, slow_get_page)) hits++;

    key = 2;
    if (cache.lookup_update (key, slow_get_page)) hits++;

    key = 3;
    if (cache.lookup_update (key, slow_get_page)) hits++;

    key = 4;
    if (cache.lookup_update (key, slow_get_page)) hits++;

    key = 3;
    if (cache.lookup_update (key, slow_get_page)) hits++;

    key = 2;
    if (cache.lookup_update (key, slow_get_page)) hits++;

    key = 1;
    if (cache.lookup_update (key, slow_get_page)) hits++;

    std::cout << "Ideal cache ending..." << std::endl;
    std::cout << "Hits: " << hits << std::endl;

    return 0;
}
