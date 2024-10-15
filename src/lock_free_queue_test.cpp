#include <iostream>
#include "lock_free_queue.hpp"

int main() {
    LockFreeQueue<int> queue;

    queue.push(1);
    queue.push(2);
    queue.push(3);

    std::shared_ptr<int> value = queue.pop();
    if (value) {
        std::cout << "Popped value: " << *value << std::endl;
    } else {
        std::cout << "Queue is empty." << std::endl;
    }

    return 0;
}

