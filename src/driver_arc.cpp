#include <arc.hpp>
#include <page.hpp>

int main ()
{
    std::cout << "Cache starting..." << std::endl;

    ARC_t<Page_t, int> cache(5);

    int hits = 0;
    int key =  1;
    if (cache.lookup_update (key, slow_get_page)) hits++;

    key = 2;
    if (cache.lookup_update (key, slow_get_page)) hits++;

    key = 3;
    if (cache.lookup_update (key, slow_get_page)) hits++;

    key = 4;
    if (cache.lookup_update (key, slow_get_page)) hits++;

    key = 5;
    if (cache.lookup_update (key, slow_get_page)) hits++;

    key = 6;
    if (cache.lookup_update (key, slow_get_page)) hits++;

    key = 2;
    if (cache.lookup_update (key, slow_get_page)) hits++;

    std::cout << "Cache ending..." << std::endl;
    std::cout << "Hits: " << hits << std::endl;

    return 0;
}
