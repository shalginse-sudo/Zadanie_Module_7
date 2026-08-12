#include <thread>
#include <iostream>
#include <mutex>

namespace {
    std::mutex mutex;

    void add_clients_to_queue(unsigned max_clients_number, unsigned &current_clients_count)
    {
        std::unique_lock<std::mutex> guard(mutex);

        while(max_clients_number) {
            ++current_clients_count;
            --max_clients_number;
            std::cout << "client was added to queue, current queue size is "
                      << current_clients_count << std::endl;

            guard.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(1));
            guard.lock();
        }
    }

    void process_clients_queue(unsigned clients_to_process, unsigned &current_clients_count)
    {
        std::unique_lock<std::mutex> guard(mutex);
        while(clients_to_process) {
            if(current_clients_count) {
                --clients_to_process;
                --current_clients_count;
                std::cout << "client was removed from queue, current queue size is "
                    << current_clients_count << std::endl;
            }

            guard.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(2));
            guard.lock();
        }
    }
}

void task_1_1(unsigned max_clients_number)
{
    unsigned current_clients_count = 0;

    std::thread t1(&add_clients_to_queue, max_clients_number, std::ref(current_clients_count));
    std::thread t2(&process_clients_queue, max_clients_number, std::ref(current_clients_count));

    t1.join();
    t2.join();
}
