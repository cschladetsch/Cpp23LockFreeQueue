#ifndef LOCK_FREE_QUEUE_HPP
#define LOCK_FREE_QUEUE_HPP

#include <memory>
#include <atomic>

template <typename T>
class LockFreeQueue {
public:
    LockFreeQueue();
    ~LockFreeQueue();

    // Push a new element to the queue
    void push(const T& value);

    // Pop an element from the queue
    std::shared_ptr<T> pop();

    // Check if the queue is empty
    bool empty() const;

private:
    struct Node {
        std::shared_ptr<T> data;
        std::atomic<Node*> next;
        Node() : next(nullptr) {}
        Node(const T& value) : data(std::make_shared<T>(value)), next(nullptr) {}
    };

    std::atomic<Node*> head;
    std::atomic<Node*> tail;
};

#include "lock_free_queue_impl.hpp"

#endif // LOCK_FREE_QUEUE_HPP

