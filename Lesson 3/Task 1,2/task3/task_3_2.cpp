#include <iterator>
#include <algorithm>
#include <future>
#include <list>
#include <iostream>

template<typename It, typename Func>
void for_each(unsigned threads_num, It begin, It end, Func f)
{
    if(threads_num == 0) {
        throw std::invalid_argument("zero threads number");
    }

    if(begin == end) {
        return;
    }

    auto distance = std::distance(begin, end);
    threads_num = std::min<unsigned>(threads_num, distance);

    std::vector<std::future<void>> futures;

    auto chunk_size = distance / threads_num;
    auto chunk_begin = begin;
    for(unsigned i = 0; i < threads_num; ++i) {
        It chunk_end;
        if(i == threads_num - 1) {
            chunk_end = end;
        } else {
            chunk_end = chunk_begin;
            std::advance(chunk_end, chunk_size);
        }

        futures.push_back(std::async(std::launch::async, [chunk_begin, chunk_end, &f]() {
            for(auto it = chunk_begin; it != chunk_end; ++it) {
                f(*it);
            }
        }));

        chunk_begin = chunk_end;
    }

    for(auto &f : futures) {
        f.get();
    }
}

void task_3_2()
{
    std::list<int> l{ 1, 2, 3, 4 };

    for_each(2, l.begin(), l.end(), [](int &v) { v *= v; });

    for(auto el : l) {
        std::cout << el << " ";
    }
    std::cout << std::endl;
}