#include "lock_free_queue.hpp"
#include <iostream>

int main() {
    LockFreeQueue<int> queue;

    queue.enqueue(1);
    queue.enqueue(2);
    queue.enqueue(3);

    std::shared_ptr<int> value1 = queue.dequeue();
    std::shared_ptr<int> value2 = queue.dequeue();
    std::shared_ptr<int> value3 = queue.dequeue();

    std::cout << "Dequeued: " << (value1 ? *value1 : -1) << std::endl;
    std::cout << "Dequeued: " << (value2 ? *value2 : -1) << std::endl;
    std::cout << "Dequeued: " << (value3 ? *value3 : -1) << std::endl;

    return 0;
}

