#include "lock_free_queue.hpp"
#include "rang.hpp"
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>
#include <iomanip>
#include <optional>
#include <algorithm>
#include <limits>
#include <stdexcept>

struct TestResult {
    int producers;
    int consumers;
    int items;
    double duration;
    double throughput;
};

template<typename Queue>
TestResult run_test(Queue& queue, int num_producers, int num_consumers, int items_per_producer) {
    std::atomic<bool> done(false);
    std::atomic<long long> total_pushed(0);
    std::atomic<long long> total_popped(0);

    auto producer = [&](int id) {
        try {
            for (int i = 0; i < items_per_producer; ++i) {
                int value = id * 10000 + i;
                queue.push(value);
                total_pushed.fetch_add(1, std::memory_order_relaxed);
            }
            std::cout << "Producer " << id << " finished\n";
        } catch (const std::exception& e) {
            std::cerr << "Exception in producer " << id << ": " << e.what() << std::endl;
        }
    };

    auto consumer = [&](int id) {
        try {
            while (!done || !queue.empty()) {
                auto value_opt = queue.pop();
                if (value_opt) {
                    total_popped.fetch_add(1, std::memory_order_relaxed);
                } else {
                    std::this_thread::yield();
                }
            }
            std::cout << "Consumer " << id << " finished\n";
        } catch (const std::exception& e) {
            std::cerr << "Exception in consumer " << id << ": " << e.what() << std::endl;
        }
    };

    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    auto start_time = std::chrono::high_resolution_clock::now();

    try {
        for (int i = 0; i < num_producers; ++i) {
            producers.emplace_back(producer, i);
        }
        for (int i = 0; i < num_consumers; ++i) {
            consumers.emplace_back(consumer, i);
        }

        for (auto& t : producers) {
            if (t.joinable()) t.join();
        }

        std::cout << "All producers finished\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        done = true;
        for (auto& t : consumers) {
            if (t.joinable()) t.join();
        }

        std::cout << "All consumers finished\n";

    } catch (const std::exception& e) {
        std::cerr << "Exception in thread management: " << e.what() << std::endl;
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    if (total_pushed.load() != total_popped.load()) {
        std::cout << rang::fg::red << "Mismatch: Pushed " << total_pushed.load() 
                  << ", Popped " << total_popped.load() << rang::fg::reset << std::endl;
    }

    double duration_seconds = duration.count() / 1000.0;
    double throughput = (total_pushed.load() + total_popped.load()) / duration_seconds;

    return {num_producers, num_consumers, items_per_producer, static_cast<double>(duration.count()), throughput};
}

void print_results(const std::string& queue_type, const TestResult& result) {
    std::cout << rang::style::bold
              << rang::fg::blue << std::setw(15) << queue_type << rang::fg::reset
              << rang::fg::green << std::setw(10) << result.producers 
              << std::setw(10) << result.consumers 
              << std::setw(15) << result.items << rang::fg::reset
              << rang::fg::yellow << std::setw(15) << std::fixed << std::setprecision(2) << result.duration << " ms"
              << std::setw(20) << std::fixed << std::setprecision(2) << result.throughput << " ops/s"
              << rang::style::reset << std::endl;
}

int main() {
    try {
        const std::vector<int> producer_counts = {1, 2, 4};
        const std::vector<int> consumer_counts = {1, 2, 4};
        const std::vector<int> item_counts = {10000, 100000, 1000000};

        std::vector<TestResult> results;

        std::cout << rang::style::bold 
                  << rang::fg::blue << std::setw(15) << "Queue Type"
                  << rang::fg::green << std::setw(10) << "Producers" 
                  << std::setw(10) << "Consumers" 
                  << std::setw(15) << "Items/Producer"
                  << rang::fg::yellow << std::setw(15) << "Duration" 
                  << std::setw(20) << "Throughput"
                  << rang::style::reset << std::endl;

        for (int producers : producer_counts) {
            for (int consumers : consumer_counts) {
                for (int items : item_counts) {
                    std::cout << "Starting test with " << producers << " producers, " 
                              << consumers << " consumers, and " << items << " items per producer\n";
                    
                    LockFreeQueue<int> your_queue;
                    auto result = run_test(your_queue, producers, consumers, items);
                    results.push_back(result);
                    print_results("Your LockFree", result);

                    std::cout << std::string(85, '-') << std::endl;
                }
            }
        }

        auto min_result = *std::min_element(results.begin(), results.end(), 
            [](const TestResult& a, const TestResult& b) { return a.throughput < b.throughput; });
        auto max_result = *std::max_element(results.begin(), results.end(), 
            [](const TestResult& a, const TestResult& b) { return a.throughput < b.throughput; });

        std::cout << rang::style::bold << rang::fg::cyan 
                  << "\nBest performance: " << max_result.throughput << " ops/s"
                  << " (P: " << max_result.producers << ", C: " << max_result.consumers 
                  << ", I: " << max_result.items << ")" << rang::style::reset << std::endl;

        std::cout << rang::style::bold << rang::fg::magenta 
                  << "Worst performance: " << min_result.throughput << " ops/s"
                  << " (P: " << min_result.producers << ", C: " << min_result.consumers 
                  << ", I: " << min_result.items << ")" << rang::style::reset << std::endl;

    } catch (const std::exception& e) {
        std::cerr << rang::fg::red << "Fatal error: " << e.what() << rang::fg::reset << std::endl;
        return 1;
    }

    return 0;
}
