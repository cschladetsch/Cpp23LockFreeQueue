#ifndef LOCK_FREE_QUEUE_IMPL_HPP
#define LOCK_FREE_QUEUE_IMPL_HPP

#include "lock_free_queue.hpp"

template <typename T>
LockFreeQueue<T>::LockFreeQueue() : head(new Node), tail(head.load()) {}

template <typename T>
LockFreeQueue<T>::~LockFreeQueue() {
    while (head.load() != nullptr) {
        Node* old_head = head.load();
        head.store(old_head->next);
        delete old_head;
    }
}

template <typename T>
void LockFreeQueue<T>::push(const T& value) {
    Node* new_node = new Node(value);
    Node* old_tail = tail.exchange(new_node);
    old_tail->next.store(new_node);
}

template <typename T>
std::shared_ptr<T> LockFreeQueue<T>::pop() {
    Node* old_head = head.load();
    if (old_head == tail.load()) {
        return nullptr; // Queue is empty
    }
    std::shared_ptr<T> result = old_head->next.load()->data;
    head.store(old_head->next);
    delete old_head;
    return result;
}

template <typename T>
bool LockFreeQueue<T>::empty() const {
    return head.load() == tail.load();
}

#endif // LOCK_FREE_QUEUE_IMPL_HPP

