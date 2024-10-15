#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <boost/lockfree/queue.hpp>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <cstdlib>   // For std::atoi
#include <rang.hpp>
#include <unistd.h>  // For usleep (microseconds)

#include "lock_free_queue.hpp"

// Function to show spinner
void show_spinner(int iteration, int total_iterations) {
    static const char spinner_chars[] = {'|', '/', '-', '\\'};
    std::cout << "\r" << rang::fg::magenta << spinner_chars[iteration % 4] 
              << " Running tests... " 
              << (100 * iteration / total_iterations) << "% complete" << rang::style::reset;
    std::cout.flush();
}

int main(int argc, char** argv) {
    // Default values
    int num_elements = 1000000;
    int num_iterations = 1000;

    // Check if command-line arguments are passed
    if (argc > 1) {
        num_elements = std::atoi(argv[1]);
    }
    if (argc > 2) {
        num_iterations = std::atoi(argv[2]);
    }

    std::vector<double> custom_times;
    std::vector<double> boost_times;

    for (int i = 0; i < num_iterations; ++i) {
        show_spinner(i, num_iterations);

        // Custom LockFreeQueue
        LockFreeQueue<int> custom_queue;
        auto start_custom = std::chrono::high_resolution_clock::now();
        for (int j = 0; j < num_elements; ++j) {
            custom_queue.enqueue(j);
        }
        for (int j = 0; j < num_elements; ++j) {
            custom_queue.dequeue();
        }
        auto end_custom = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> custom_duration = end_custom - start_custom;
        custom_times.push_back(custom_duration.count());

        // Boost LockFreeQueue
        boost::lockfree::queue<int> boost_queue(num_elements);
        auto start_boost = std::chrono::high_resolution_clock::now();
        for (int j = 0; j < num_elements; ++j) {
            boost_queue.push(j);
        }
        int value;
        for (int j = 0; j < num_elements; ++j) {
            boost_queue.pop(value);
        }
        auto end_boost = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> boost_duration = end_boost - start_boost;
        boost_times.push_back(boost_duration.count());

        usleep(10000); // Sleep for 10 milliseconds to make the spinner visible
    }

    std::cout << "\r" << rang::fg::green << "Tests completed!" << rang::style::reset << std::endl;

    // Helper lambda for calculating statistics
    auto calculate_stats = [](const std::vector<double>& times) {
        double mean = std::accumulate(times.begin(), times.end(), 0.0) / times.size();
        double min = *std::min_element(times.begin(), times.end());
        double max = *std::max_element(times.begin(), times.end());
        double sq_sum = std::inner_product(times.begin(), times.end(), times.begin(), 0.0);
        double stddev = std::sqrt(sq_sum / times.size() - mean * mean);
        return std::tuple<double, double, double, double>(mean, min, max, stddev);
    };

    // Calculate statistics for both queues
    auto [custom_mean, custom_min, custom_max, custom_stddev] = calculate_stats(custom_times);
    auto [boost_mean, boost_min, boost_max, boost_stddev] = calculate_stats(boost_times);

    // Print side-by-side comparison of the results
    std::cout << rang::style::bold << "Performance Comparison (in seconds):" << rang::style::reset << std::endl;
    std::cout << rang::fg::cyan << "Metric         " << rang::fg::green << "Custom Queue     " 
              << rang::fg::red << "Boost Queue" << rang::style::reset << std::endl;
    std::cout << "Mean:          " << rang::fg::green << custom_mean << "             " 
              << rang::fg::red << boost_mean << rang::style::reset << std::endl;
    std::cout << "Min:           " << rang::fg::green << custom_min << "             " 
              << rang::fg::red << boost_min << rang::style::reset << std::endl;
    std::cout << "Max:           " << rang::fg::green << custom_max << "             " 
              << rang::fg::red << boost_max << rang::style::reset << std::endl;
    std::cout << "Std Deviation: " << rang::fg::green << custom_stddev << "          " 
              << rang::fg::red << boost_stddev << rang::style::reset << std::endl;

    return 0;
}

