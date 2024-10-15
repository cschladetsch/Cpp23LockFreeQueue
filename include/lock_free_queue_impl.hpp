#ifndef LOCK_FREE_QUEUE_IMPL_HPP
#define LOCK_FREE_QUEUE_IMPL_HPP

#include "lock_free_queue.hpp"

template <typename T>
LockFreeQueue<T>::LockFreeQueue() {
    Node* dummy = new Node(T());
    head.store(dummy);
    tail.store(dummy);
}

template <typename T>
LockFreeQueue<T>::~LockFreeQueue() {
    while (Node* oldHead = head.load()) {
        head.store(oldHead->next);
        delete oldHead;
    }
}

template <typename T>
void LockFreeQueue<T>::enqueue(T value) {
    Node* newNode = new Node(value);
    Node* oldTail = tail.exchange(newNode);
    oldTail->next = newNode;
}

template <typename T>
std::shared_ptr<T> LockFreeQueue<T>::dequeue() {
    Node* oldHead = head.load();
    Node* nextNode = oldHead->next;
    if (nextNode == nullptr) {
        return nullptr;
    }
    head.store(nextNode);
    return nextNode->data;
}

template <typename T>
bool LockFreeQueue<T>::empty() const {
    return head.load()->next == nullptr;
}

#endif // LOCK_FREE_QUEUE_IMPL_HPP

