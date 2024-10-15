#pragma once

#include <atomic>
#include <optional>
#include <memory>

template<typename T>
class LockFreeQueue {
private:
    struct Node {
        std::shared_ptr<T> data;
        std::atomic<Node*> next;
        Node(T&& val) : data(std::make_shared<T>(std::move(val))), next(nullptr) {}
        Node() : data(nullptr), next(nullptr) {}
    };

    std::atomic<Node*> head;
    std::atomic<Node*> tail;
    std::atomic<size_t> size;

public:
    LockFreeQueue() : size(0) {
        Node* dummy = new Node();
        head.store(dummy, std::memory_order_relaxed);
        tail.store(dummy, std::memory_order_relaxed);
    }

    ~LockFreeQueue() {
        while (Node* old_head = head.load()) {
            head.store(old_head->next);
            delete old_head;
        }
    }

    void push(T val) {
        Node* new_node = new Node(std::move(val));
        while (true) {
            Node* last = tail.load(std::memory_order_acquire);
            Node* next = last->next.load(std::memory_order_acquire);
            if (last == tail.load(std::memory_order_acquire)) {
                if (next == nullptr) {
                    if (last->next.compare_exchange_weak(next, new_node, 
                                                         std::memory_order_release,
                                                         std::memory_order_relaxed)) {
                        tail.compare_exchange_strong(last, new_node, std::memory_order_acq_rel);
                        size.fetch_add(1, std::memory_order_relaxed);
                        return;
                    }
                } else {
                    tail.compare_exchange_weak(last, next, std::memory_order_release);
                }
            }
        }
    }

    std::optional<T> pop() {
        while (true) {
            Node* first = head.load(std::memory_order_acquire);
            Node* last = tail.load(std::memory_order_acquire);
            Node* next = first->next.load(std::memory_order_acquire);

            if (first == head.load(std::memory_order_acquire)) {
                if (first == last) {
                    if (next == nullptr) {
                        // Queue is empty
                        return std::nullopt;
                    }
                    // Tail is lagging, advance it
                    tail.compare_exchange_weak(last, next, std::memory_order_release);
                } else {
                    T result = std::move(*next->data);
                    if (head.compare_exchange_weak(first, next, std::memory_order_release, std::memory_order_relaxed)) {
                        size.fetch_sub(1, std::memory_order_relaxed);
                        delete first;
                        return result;
                    }
                }
            }
        }
    }

    bool empty() const {
        return head.load(std::memory_order_relaxed)->next.load(std::memory_order_relaxed) == nullptr;
    }

    size_t get_size() const {
        return size.load(std::memory_order_relaxed);
    }
};
