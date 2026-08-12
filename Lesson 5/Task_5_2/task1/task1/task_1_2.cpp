#include <thread>
#include <vector>
#include <stdexcept>
#include <functional>
#include <iostream>
#include <iomanip>

void sum_vector_chunks(const std::vector<int> &v1, const std::vector<int> &v2,
                       size_t begin, size_t end, std::vector<int> &res)
{
    for(auto idx = begin; idx != end; ++idx) {
        res[idx] = v1[idx] + v2[idx];
    }
}

std::vector<int> sum_vectors(const std::vector<int> &v1, const std::vector<int> &v2,
                             unsigned number_of_threads)
{
    if(v1.size() != v2.size()) {
        throw std::invalid_argument("vector sizes are different");
    }

    if(number_of_threads == 0) {
        throw std::invalid_argument("number of thread cannot be zero");
    }

    if(number_of_threads > v1.size()) {
        number_of_threads = v1.size();
    }

    size_t total_size = v1.size();
    if(total_size == 0) {
        return {};
    }

    size_t chunk_size = total_size / number_of_threads;

    std::vector<int> ans(v1.size(), 0);
    std::vector<std::thread> threads;
    size_t chunk_begin = 0;
    size_t chunk_end = 0;
    for(size_t i = 0; i < number_of_threads; ++i) {
        chunk_begin = chunk_end;
        if(i == number_of_threads - 1) {
            chunk_end = total_size;
        } else {
            chunk_end += chunk_size;
        }

        threads.emplace_back(&sum_vector_chunks, std::ref(v1), std::ref(v2),
                             chunk_begin, chunk_end, std::ref(ans));
    }

    for(auto &t : threads) {
        t.join();
    }

    return ans;
}

double calculate_sum_vectors_time(size_t vectors_size, unsigned threads_count)
{
    std::vector<int> v1(vectors_size, 1);
    std::vector<int> v2(vectors_size, 2);
    auto start = std::chrono::steady_clock::now();
    auto r = sum_vectors(v1, v2, threads_count);
    std::chrono::duration<double> duration = std::chrono::steady_clock::now() - start;
    return duration.count();
}

void task_1_2()
{
    constexpr int width = 15;

    std::cout << "Hardware concurrency - " << std::thread::hardware_concurrency() << std::endl;
    std::cout << std::setw(width) << ""
        << std::setw(width) << "1000"
        << std::setw(width) << "10000"
        << std::setw(width) << "100000"
        << std::setw(width) << "1000000"
        << '\n';

    std::cout << std::setw(width) << "1 thread"
        << std::setw(width) << std::fixed << std::setprecision(7)
        << calculate_sum_vectors_time(1000, 1)
        << std::setw(width)
        << calculate_sum_vectors_time(10000, 1)
        << std::setw(width)
        << calculate_sum_vectors_time(100000, 1)
        << std::setw(width)
        << calculate_sum_vectors_time(1000000, 1)
        << '\n';

    std::cout << std::setw(width) << "2 threads"
        << std::setw(width) << std::fixed << std::setprecision(7)
        << calculate_sum_vectors_time(1000, 2)
        << std::setw(width)
        << calculate_sum_vectors_time(10000, 2)
        << std::setw(width)
        << calculate_sum_vectors_time(100000, 2)
        << std::setw(width)
        << calculate_sum_vectors_time(1000000, 2)
        << '\n';

    std::cout << std::setw(width) << "4 threads"
        << std::setw(width) << std::fixed << std::setprecision(7)
        << calculate_sum_vectors_time(1000, 4)
        << std::setw(width)
        << calculate_sum_vectors_time(10000, 4)
        << std::setw(width)
        << calculate_sum_vectors_time(100000, 4)
        << std::setw(width)
        << calculate_sum_vectors_time(1000000, 4)
        << '\n';

    std::cout << std::setw(width) << "8 threads"
        << std::setw(width) << std::fixed << std::setprecision(7)
        << calculate_sum_vectors_time(1000, 8)
        << std::setw(width)
        << calculate_sum_vectors_time(10000, 8)
        << std::setw(width)
        << calculate_sum_vectors_time(100000, 8)
        << std::setw(width)
        << calculate_sum_vectors_time(1000000, 8)
        << '\n';

    std::cout << std::setw(width) << "16 threads"
        << std::setw(width) << std::fixed << std::setprecision(7)
        << calculate_sum_vectors_time(1000, 16)
        << std::setw(width)
        << calculate_sum_vectors_time(10000, 16)
        << std::setw(width)
        << calculate_sum_vectors_time(100000, 16)
        << std::setw(width)
        << calculate_sum_vectors_time(1000000, 16)
        << '\n';
}
