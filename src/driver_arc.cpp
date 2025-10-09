#include "arc.hpp"
#include "page.hpp"
#include "driver.hpp"

int main ()
{
    auto input_data = read_input();

    ARC_t<Page_t, int> cache (input_data.cache_size);

    int hits = 0;

    for (int key : input_data.sequence)
        if (cache.lookup_update (key, slow_get_page)) hits++;

    std::cout << hits << std::endl;

    return 0;
}
