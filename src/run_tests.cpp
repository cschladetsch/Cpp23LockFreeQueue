#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <chrono>
#include "lock_free_queue.hpp" // Custom Queue header
#include <boost/lockfree/queue.hpp> // Boost Queue header
#include <rang.hpp> // For colorful output

// Function to calculate the mean
double calculate_mean(const std::vector<double>& times) {
    return std::accumulate(times.begin(), times.end(), 0.0) / times.size();
}

// Function to calculate the standard deviation
double calculate_stddev(const std::vector<double>& times, double mean) {
    double sq_sum = std::inner_product(times.begin(), times.end(), times.begin(), 0.0);
    return std::sqrt(sq_sum / times.size() - mean * mean);
}

// Function to print the results in a summarized manner
void print_summary(const std::string& queue_name, const std::vector<double>& times) {
    double mean = calculate_mean(times);
    double stddev = calculate_stddev(times, mean);
    double min_time = *std::min_element(times.begin(), times.end());
    double max_time = *std::max_element(times.begin(), times.end());

    std::cout << rang::fg::green << queue_name << " Results: \n";
    std::cout << rang::fg::yellow << "Mean time: " << rang::fg::reset << mean << " seconds\n";
    std::cout << rang::fg::yellow << "Standard deviation: " << rang::fg::reset << stddev << " seconds\n";
    std::cout << rang::fg::yellow << "Min time: " << rang::fg::reset << min_time << " seconds\n";
    std::cout << rang::fg::yellow << "Max time: " << rang::fg::reset << max_time << " seconds\n";
    std::cout << rang::style::reset << std::endl;
}

// Template to run the test for both queue types
template <typename QueueType>
std::vector<double> run_test(QueueType& queue, int iterations, int elements_per_iteration) {
    std::vector<double> times;

    for (int i = 0; i < iterations; ++i) {
        auto start = std::chrono::high_resolution_clock::now();

        // Enqueue and Dequeue elements
        for (int j = 0; j < elements_per_iteration; ++j) {
            queue.push(j); // Enqueue operation
        }
        for (int j = 0; j < elements_per_iteration; ++j) {
            int value;
            queue.pop(value);
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        times.push_back(elapsed.count());
    }

    return times;
}

int main(int argc, char** argv) {
    const int iterations = 100; // Number of test runs
    const int elements_per_iteration = 1000; // Elements per iteration

    // Custom Queue test
    std::cout << rang::fg::blue << "Running performance comparison for Custom Queue..." << rang::fg::reset << std::endl;
    LockFreeQueue<int> custom_queue;
    std::vector<double> custom_times = run_test(custom_queue, iterations, elements_per_iteration);

    // Boost Queue test
    std::cout << rang::fg::blue << "Running performance comparison for Boost Queue..." << rang::fg::reset << std::endl;
    boost::lockfree::queue<int> boost_queue(1024);
    std::vector<double> boost_times = run_test(boost_queue, iterations, elements_per_iteration);

    // Print the summary of results
    print_summary("Custom Queue", custom_times);
    print_summary("Boost Queue", boost_times);

    return 0;
}

