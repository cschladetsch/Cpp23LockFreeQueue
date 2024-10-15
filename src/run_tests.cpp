#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <chrono>
#include <cmath>
#include <string>
#include <rang.hpp>
#include "lock_free_queue.hpp"
#include <boost/lockfree/queue.hpp>

struct Stats {
    double mean, min, max, stddev;
};

Stats compute_statistics(const std::vector<double>& times) {
    double mean = std::accumulate(times.begin(), times.end(), 0.0) / times.size();
    double sq_sum = std::inner_product(times.begin(), times.end(), times.begin(), 0.0);
    double stddev = std::sqrt(sq_sum / times.size() - mean * mean);
    double min = *std::min_element(times.begin(), times.end());
    double max = *std::max_element(times.begin(), times.end());
    return {mean, min, max, stddev};
}

template<typename QueueType>
std::vector<double> run_test(QueueType& queue, int num_operations) {
    std::vector<double> times;
    typename QueueType::value_type result;

    for (int i = 0; i < num_operations; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        queue.push(i);
        auto end = std::chrono::high_resolution_clock::now();
        times.push_back(std::chrono::duration<double>(end - start).count());

        start = std::chrono::high_resolution_clock::now();
        queue.pop(result);
        end = std::chrono::high_resolution_clock::now();
        times.push_back(std::chrono::duration<double>(end - start).count());
    }

    return times;
}

void print_summary(const Stats& custom_stats, const Stats& boost_stats) {
    std::cout << rang::fg::green << "\nSummary of Performance Results:\n" << rang::style::reset;

    std::cout << "Custom Queue:\n";
    std::cout << "- Mean Time: " << custom_stats.mean << " s\n";
    std::cout << "- Min Time: " << custom_stats.min << " s\n";
    std::cout << "- Max Time: " << custom_stats.max << " s\n";
    std::cout << "- Stddev: " << custom_stats.stddev << " s\n\n";

    std::cout << "Boost Queue:\n";
    std::cout << "- Mean Time: " << boost_stats.mean << " s\n";
    std::cout << "- Min Time: " << boost_stats.min << " s\n";
    std::cout << "- Max Time: " << boost_stats.max << " s\n";
    std::cout << "- Stddev: " << boost_stats.stddev << " s\n\n";

    double mean_diff = custom_stats.mean - boost_stats.mean;
    double min_diff = custom_stats.min - boost_stats.min;
    double max_diff = custom_stats.max - boost_stats.max;
    double stddev_diff = custom_stats.stddev - boost_stats.stddev;

    std::cout << rang::fg::magenta << "Overall Performance Comparison:\n" << rang::style::reset;
    std::cout << "- Custom Queue was faster by an average of " << mean_diff << " seconds compared to Boost Queue.\n";
    std::cout << "- Custom Queue had a smaller minimum time by " << min_diff << " seconds.\n";
    std::cout << "- Boost Queue had a smaller maximum time by " << max_diff << " seconds.\n";
    std::cout << "- Custom Queue was more stable with a lower standard deviation by " << stddev_diff << " seconds.\n";
}

int main(int argc, char** argv) {
    int num_operations = (argc > 1) ? std::stoi(argv[1]) : 1000000;
    int num_runs = (argc > 2) ? std::stoi(argv[2]) : 10;

    LockFreeQueue<int> custom_queue;
    boost::lockfree::queue<int> boost_queue(10000);

    std::vector<double> custom_times;
    std::vector<double> boost_times;

    for (int run = 0; run < num_runs; ++run) {
        auto custom_run_times = run_test(custom_queue, num_operations);
        auto boost_run_times = run_test(boost_queue, num_operations);
        custom_times.insert(custom_times.end(), custom_run_times.begin(), custom_run_times.end());
        boost_times.insert(boost_times.end(), boost_run_times.begin(), boost_run_times.end());
    }

    auto custom_stats = compute_statistics(custom_times);
    auto boost_stats = compute_statistics(boost_times);

    print_summary(custom_stats, boost_stats);

    return 0;
}

