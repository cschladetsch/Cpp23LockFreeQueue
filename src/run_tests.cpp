#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <chrono>
#include <cmath>  // Added for std::sqrt
#include "lock_free_queue.hpp"
#include <boost/lockfree/queue.hpp>

// Function to calculate the mean of times
double calculate_mean(const std::vector<double>& times) {
    return std::accumulate(times.begin(), times.end(), 0.0) / times.size();
}

// Function to calculate the standard deviation
double calculate_stddev(const std::vector<double>& times, double mean) {
    double sq_sum = std::inner_product(times.begin(), times.end(), times.begin(), 0.0);
    return std::sqrt(sq_sum / times.size() - mean * mean);
}

// Function template to test the queue performance
template<typename QueueType>
std::vector<double> run_test(QueueType& queue, int num_operations) {
    std::vector<double> times;
    typename QueueType::value_type result;  // Correctly declare the result variable

    for (int i = 0; i < num_operations; ++i) {
        // Timing the enqueue operation
        auto start = std::chrono::high_resolution_clock::now();
        queue.push(i);  // Push operation (enqueue)
        auto end = std::chrono::high_resolution_clock::now();
        times.push_back(std::chrono::duration<double>(end - start).count());

        // Timing the dequeue operation
        start = std::chrono::high_resolution_clock::now();
        queue.pop(result);  // Pop operation (dequeue)
        end = std::chrono::high_resolution_clock::now();
        times.push_back(std::chrono::duration<double>(end - start).count());
    }

    return times;
}

int main() {
    int num_operations = 10000;

    // LockFreeQueue test
    LockFreeQueue<int> custom_queue;
    std::vector<double> custom_times = run_test(custom_queue, num_operations);

    // Boost queue test
    boost::lockfree::queue<int> boost_queue(10000);  // Adjust size if necessary
    std::vector<double> boost_times = run_test(boost_queue, num_operations);

    // Calculating and printing statistics
    double custom_mean = calculate_mean(custom_times);
    double custom_stddev = calculate_stddev(custom_times, custom_mean);
    std::cout << "Custom Queue Mean: " << custom_mean << ", Stddev: " << custom_stddev << std::endl;

    double boost_mean = calculate_mean(boost_times);
    double boost_stddev = calculate_stddev(boost_times, boost_mean);
    std::cout << "Boost Queue Mean: " << boost_mean << ", Stddev: " << boost_stddev << std::endl;

    return 0;
}

