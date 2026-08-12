#include <thread>
#include <iostream>
#include <mutex>
#include <string>
#include <atomic>

namespace {
    void print(const std::string &str)
    {
        static std::mutex mutex;
        std::lock_guard<std::mutex> guard(mutex);
        std::cout << str;
    }

    
    void add_clients_to_queue(unsigned max_clients_number, std::atomic<unsigned> &current_clients_count)
    {
        while(max_clients_number) {
            auto count = current_clients_count.fetch_add(1, std::memory_order_relaxed) + 1;
            --max_clients_number;
            print("client was added to queue, current queue size is " + std::to_string(count) + "\n");

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    void process_clients_queue(unsigned clients_to_process, std::atomic<unsigned> &current_clients_count)
    {
        while(clients_to_process) {
            if(current_clients_count.load(std::memory_order_relaxed)) {
                --clients_to_process;
                auto count = current_clients_count.fetch_sub(1, std::memory_order_relaxed) - 1;
                print("client was removed from queue, current queue size is " + std::to_string(count) + "\n");
            }

            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }
}

void task_2_1(unsigned max_clients_number)
{
    std::atomic<unsigned> current_clients_count = 0;

    std::thread t1(&add_clients_to_queue, max_clients_number, std::ref(current_clients_count));
    std::thread t2(&process_clients_queue, max_clients_number, std::ref(current_clients_count));

    t1.join();
    t2.join();
}
