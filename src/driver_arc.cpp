#include "arc.hpp"
#include "page.hpp"
#include "driver.hpp"

int main ()
{
    auto [size, sequence] = read_input();

    arc_cache::ARC_t<Page_t, int> cache (size);

    int hits = 0;

    for (int key : sequence)
        if (cache.lookup_update (key, slow_get_page)) hits++;

    std::cout << hits << std::endl;

    return 0;
}
