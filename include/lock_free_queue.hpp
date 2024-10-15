#ifndef LOCK_FREE_QUEUE_HPP
#define LOCK_FREE_QUEUE_HPP

#include <memory>
#include <atomic>

template <typename T>
class LockFreeQueue {
private:
    struct Node {
        T data;
        std::unique_ptr<Node> next;
        Node(T value) : data(value), next(nullptr) {}
    };

    std::unique_ptr<Node> head;
    Node* tail;

public:
    LockFreeQueue() : head(nullptr), tail(nullptr) {}

    void enqueue(T value) {
        std::unique_ptr<Node> new_node = std::make_unique<Node>(value);
        Node* new_tail = new_node.get();

        if (tail) {
            tail->next = std::move(new_node);
        } else {
            head = std::move(new_node);
        }
        tail = new_tail;
    }

    bool dequeue(T& result) {
        if (!head) return false;
        result = head->data;
        head = std::move(head->next);
        if (!head) {
            tail = nullptr;
        }
        return true;
    }
};

#endif // LOCK_FREE_QUEUE_HPP

