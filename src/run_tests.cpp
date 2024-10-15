#include <iostream>
#include <vector>
#include <chrono>
#include "lock_free_queue.hpp"
#include <boost/lockfree/queue.hpp>
#include <numeric>
#include <algorithm>

// Simple function to measure mean
double calculate_mean(const std::vector<double>& times) {
    return std::accumulate(times.begin(), times.end(), 0.0) / times.size();
}

// Function to calculate standard deviation
double calculate_stddev(const std::vector<double>& times, double mean) {
    double sq_sum = std::inner_product(times.begin(), times.end(), times.begin(), 0.0);
    return std::sqrt(sq_sum / times.size() - mean * mean);
}

// Test both queues: Custom and Boost
template <typename QueueType>
std::vector<double> run_test(QueueType& queue, int num_items) {
    std::vector<double> times;
    for (int i = 0; i < 100; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        for (int j = 0; j < num_items; ++j) {
            queue.enqueue(j);
        }
        for (int j = 0; j < num_items; ++j) {
            int result;
            queue.dequeue(result);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        times.push_back(elapsed.count());
    }
    return times;
}

int main() {
    int num_items = 10000;

    LockFreeQueue<int> custom_queue;
    boost::lockfree::queue<int> boost_queue(10000);

    std::cout << "Running performance comparison...\n";

    auto custom_times = run_test(custom_queue, num_items);
    auto boost_times = run_test(boost_queue, num_items);

    double custom_mean = calculate_mean(custom_times);
    double boost_mean = calculate_mean(boost_times);

    double custom_stddev = calculate_stddev(custom_times, custom_mean);
    double boost_stddev = calculate_stddev(boost_times, boost_mean);

    std::cout << "Custom Queue: Mean = " << custom_mean << "s, Stddev = " << custom_stddev << "s\n";
    std::cout << "Boost Queue: Mean = " << boost_mean << "s, Stddev = " << boost_stddev << "s\n";

    return 0;
}

