#include "lock_free_queue.hpp"
#include "rang.hpp"
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>

LockFreeQueue<int> queue;
std::atomic<bool> done(false);
std::atomic<long long> total_pushed(0);
std::atomic<long long> total_popped(0);
bool verbose = false;

void producer(int id, int items)
{
    for (int i = 0; i < items; ++i)
    {
        int value = id * 10000 + i;
        queue.push(value);
        total_pushed.fetch_add(value, std::memory_order_relaxed);
        if (verbose && i % 1000 == 0) {
            std::cout << rang::fg::cyan << "Producer " << id << " pushed: " << value << rang::fg::reset << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(10));  // Simulate some work
    }
}

void consumer(int id) {
    while (!done || !queue.empty()) {
        auto value_opt = queue.pop();
        if (value_opt) {
            total_popped.fetch_add(*value_opt, std::memory_order_relaxed);
            if (verbose && *value_opt % 10000 == 0) {
                std::cout << rang::fg::green << "Consumer " << id << " popped: " << *value_opt << rang::fg::reset << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::microseconds(15));  // Simulate some work
    }
}

int main(int argc, char* argv[])
{
    if (argc < 4 || argc > 5) {
        std::cerr << rang::fg::red << "Usage: " << argv[0] << " [-v] <p> <c> <i>" << rang::fg::reset << std::endl;
        return 1;
    }

    int arg_offset = 0;
    if (std::string(argv[1]) == "-v") {
        verbose = true;
        arg_offset = 1;
    }

    int num_producers = std::stoi(argv[1 + arg_offset]);
    int num_consumers = std::stoi(argv[2 + arg_offset]);
    int items_per_producer = std::stoi(argv[3 + arg_offset]);

    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    auto start_time = std::chrono::high_resolution_clock::now();

    // Start producers and consumers
    for (int i = 0; i < num_producers; ++i) producers.emplace_back(producer, i, items_per_producer);
    for (int i = 0; i < num_consumers; ++i) consumers.emplace_back(consumer, i);

    // Wait for producers to finish
    for (auto& t : producers) t.join();

    // Signal consumers to finish and wait for them
    std::this_thread::sleep_for(std::chrono::seconds(1));
    done = true;
    for (auto& t : consumers) t.join();

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    // Print summary
    std::cout << rang::style::bold << rang::fg::yellow 
              << "Summary:" << rang::style::reset << std::endl;
    std::cout << "Time: " << duration.count() << " ms" << std::endl;
    std::cout << "Queue size: " << queue.get_size() << std::endl;
    std::cout << "Total pushed: " << total_pushed.load() << std::endl;
    std::cout << "Total popped: " << total_popped.load() << std::endl;

    if (total_pushed.load() != total_popped.load()) {
        std::cerr << rang::fg::red << "ERROR: Mismatch between pushed and popped values!" << rang::fg::reset << std::endl;
        return 1;
    }

    return 0;
}
