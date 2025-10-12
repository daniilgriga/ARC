#include "ideal_cache.hpp"
#include "page.hpp"
#include "driver.hpp"

int main ()
{
    auto [size, sequence] = read_input();

    IdealCache_t<Page_t, int> cache (size);
    cache.set_sequence (sequence);

    int hits = 0;

    for (int key : sequence)
        if (cache.lookup_update (key, slow_get_page)) hits++;

    std::cout << hits << std::endl;

    return 0;
}
