#ifndef LOCK_FREE_QUEUE_HPP
#define LOCK_FREE_QUEUE_HPP

#include <queue>
#include <mutex>

template<typename T>
class LockFreeQueue {
public:
    using value_type = T;  // Define value_type as T

    // Constructor
    LockFreeQueue() = default;

    // Destructor
    ~LockFreeQueue() = default;

    // Push item into the queue (matches Boost's push)
    bool push(const T& item) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(item);
        return true;
    }

    // Pop item from the queue (matches Boost's pop)
    bool pop(T& result) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return false;
        }
        result = queue_.front();
        queue_.pop();
        return true;
    }

private:
    std::queue<T> queue_;  // Underlying standard queue
    std::mutex mutex_;      // Mutex for thread-safety
};

#endif // LOCK_FREE_QUEUE_HPP

