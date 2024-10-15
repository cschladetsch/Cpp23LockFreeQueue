#ifndef LOCK_FREE_QUEUE_HPP
#define LOCK_FREE_QUEUE_HPP

#include <memory>
#include <atomic>

template <typename T>
class LockFreeQueue {
private:
    struct Node {
        std::shared_ptr<T> data;
        std::atomic<Node*> next;

        Node(T value) : data(std::make_shared<T>(value)), next(nullptr) {}
    };

    std::atomic<Node*> head;
    std::atomic<Node*> tail;

public:
    LockFreeQueue();                    // Constructor
    ~LockFreeQueue();                   // Destructor

    void enqueue(T value);              // Adds value to the queue
    std::shared_ptr<T> dequeue();       // Removes and returns value from the queue

    bool empty() const;                 // Checks if queue is empty
};

#include "lock_free_queue_impl.hpp" // Include template implementation

#endif // LOCK_FREE_QUEUE_HPP

