template <typename Queue>
TestResult run_test(Queue& queue, int producers, int consumers, int items) {
    std::vector<std::thread> producer_threads;
    std::vector<std::thread> consumer_threads;

    // Start producers
    for (int i = 0; i < producers; ++i) {
        producer_threads.emplace_back([&, i]() {
            for (int j = 0; j < items; ++j) {
                queue.enqueue(j); // Replaced undefined push with enqueue
            }
        });
    }

    // Start consumers
    for (int i = 0; i < consumers; ++i) {
        consumer_threads.emplace_back([&]() {
            while (!done || !queue.empty()) {
                auto value = queue.dequeue(); // Replaced undefined pop with dequeue
                if (value) {
                    // Process value
                }
            }
        });
    }

    // Join producer threads
    for (auto& thread : producer_threads) {
        thread.join();
    }

    // Join consumer threads
    for (auto& thread : consumer_threads) {
        thread.join();
    }
}

