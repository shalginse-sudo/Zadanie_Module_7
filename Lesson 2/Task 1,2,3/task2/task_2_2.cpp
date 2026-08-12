#include <thread>
#include <chrono>
#include <vector>
#include <optional>
#include <iostream>
#include <iomanip>
#include <random>
#include <mutex>
#include <condition_variable>
#include <cstdlib>
#include <sstream>

namespace {
    unsigned generate_sleep_milliseconds()
    {
        static std::mutex mutex;
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<unsigned> dist(0, 2000);

        std::lock_guard guard(mutex);
        return dist(gen);
    }

    struct progress_line
    {
        mutable std::mutex mutex;
        mutable std::condition_variable may_be_printed_cv;
        bool may_be_printed = false;

        std::thread::id thread_id;
        std::chrono::steady_clock::time_point start;
        std::optional<std::chrono::steady_clock::time_point> end;
        unsigned progress = 0;
    };

    void print(const std::vector<progress_line> &lines, unsigned progress_length)
    {
        system("cls");

        std::ostringstream out;

        unsigned column_width = 10;
        unsigned progress_column_width = std::max(progress_length, 12u) + 3;

        out << std::left
            << std::setw(column_width) << "Number"
            << std::setw(column_width) << "Thread id"
            << std::setw(progress_column_width) << "Progress bar"
            << std::setw(column_width) << "Time"
            << std::endl;

        for(size_t i = 0; i < lines.size(); ++i) {
            std::unique_lock lock(lines[i].mutex);
            lines[i].may_be_printed_cv.wait(lock, [&lines, i]() { return lines[i].may_be_printed; });

            out << std::left
                << std::setw(column_width) << i
                << std::setw(column_width) << lines[i].thread_id
                << std::setw(progress_column_width) << std::string(lines[i].progress, '#');
            if(lines[i].end) {
                std::chrono::duration<double> duration = *lines[i].end - lines[i].start;
                out << std::setw(column_width) << duration.count();
            }
            out << std::endl;
        }

        std::cout << out.str();
    }

    bool is_all_lines_completed(const std::vector<progress_line> &lines) {
        for(const auto &l : lines) {
            std::lock_guard guard(l.mutex);
            if(!l.end) {
                return false;
            }
        }

        return true;
    }

    void do_progress(progress_line &line, unsigned rest_progress)
    {
        std::unique_lock lock(line.mutex);

        line.thread_id = std::this_thread::get_id();
        line.start = std::chrono::steady_clock::now();
        line.may_be_printed = true;
        line.may_be_printed_cv.notify_all();

        while(rest_progress) {
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(generate_sleep_milliseconds()));
            lock.lock();

            ++line.progress;
            --rest_progress;
        }

        line.end = std::chrono::steady_clock::now();
    }
}

void task_2_2(unsigned threads_num, unsigned progress_length)
{
    std::vector<progress_line> lines(threads_num);
    std::vector<std::thread> threads;
    for(unsigned i = 0; i < threads_num; ++i) {
        threads.emplace_back(&do_progress, std::ref(lines[i]), progress_length);
    }

    do {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        print(lines, progress_length);
    } while(!is_all_lines_completed(lines));

    print(lines, progress_length);

    for(auto &t : threads) {
        t.join();
    }
}