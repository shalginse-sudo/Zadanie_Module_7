#include <iterator>
#include <algorithm>
#include <future>
#include <list>
#include <iostream>

template<typename It, typename Func>
void parallel_for_each(It begin, It end, Func f)
{
    const std::size_t min_chunk_size = 2;
    const auto length = std::distance(begin, end);

    if(length <= min_chunk_size) {
        std::for_each(begin, end, f);
        return;
    }

    It middle = begin;
    std::advance(middle, length / 2);

    auto left_half = std::async(std::launch::async, [begin, middle, &f]() { parallel_for_each(begin, middle, f); });

    parallel_for_each(middle, end, f);

    left_half.get();
}

void task_3_2()
{
    std::list<int> l{ 1, 2, 3, 4 };

    parallel_for_each(l.begin(), l.end(), [](int &v) { v *= v; });

    for(auto el : l) {
        std::cout << el << " ";
    }
    std::cout << std::endl;
}