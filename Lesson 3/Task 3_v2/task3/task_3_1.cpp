#include <vector>
#include <future>
#include <thread>
#include <iostream>

void sort(std::vector<int> &vector)
{
    if(vector.empty() || vector.size() == 1) {
        return;
    }

    for(size_t i = 0; i < vector.size() - 1; ++i) {
        std::promise<size_t> promise;
        auto future = promise.get_future();

        std::thread([&vector, promise = std::move(promise), i]() mutable {
            int min = vector[i];
            size_t min_pos = i;
            for(size_t j = i+1; j < vector.size(); ++j) {
                if(vector[j] < min) {
                    min = vector[j];
                    min_pos = j;
                }
            }

            promise.set_value(min_pos);
        }).detach();

        std::swap(vector[i], vector[future.get()]);
    }
}

void print_vector(const std::vector<int> &v)
{
    for(size_t i = 0; i < v.size(); ++i) {
        std::cout << v[i] << " ";
    }
}

void task_3_1()
{
    std::vector<int> v{ 3, 1, 6, 0, -1, 56, 2 };

    std::cout << "Vector before sort: ";
    print_vector(v);
    std::cout << std::endl;

    sort(v);

    std::cout << "Vector after sort: ";
    print_vector(v);
    std::cout << std::endl;
}