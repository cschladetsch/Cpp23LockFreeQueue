# Lock-Free Queue Implementation

This project implements a lock-free queue in C++23, demonstrating concurrent programming techniques and the use of atomic operations.

## Files

- `lock_free_queue.hpp`: Contains the implementation of the lock-free queue.
- `main.cpp`: Driver program to test the lock-free queue with multiple producers and consumers.
- `build.sh`: Bash script to compile the project.

## Requirements

- C++23 compatible compiler (e.g., GCC 11+ or Clang 13+)
- POSIX-compliant operating system (Linux, macOS, etc.)

## Building the Project

To build the project, run the following command in the terminal:

```bash
./build.sh
```

This will compile the source files and create an executable named `lock_free_queue_driver`.

## Running the Program

After building, you can run the program with the following command:

```bash
./lock_free_queue_driver <num_producers> <num_consumers> <items_per_producer>
```

For example:

```bash
./lock_free_queue_driver 3 2 10000
```

This will run the test with 3 producers, 2 consumers, and 10000 items per producer.

## Implementation Details

The lock-free queue is implemented using atomic operations to ensure thread-safety without the use of locks. It supports the following operations:

- `push`: Add an item to the queue
- `pop`: Remove and return an item from the queue
- `empty`: Check if the queue is empty
- `get_size`: Get the current size of the queue

The implementation uses a single-linked list with atomic pointers for the head and tail.

## Performance Considerations

This implementation aims to provide high concurrency and low latency for multi-threaded producer-consumer scenarios. However, actual performance may vary depending on the specific use case and system characteristics.

## License

[Specify your license here, e.g., MIT, GPL, etc.]

## Contributing

[If you want to accept contributions, specify how others can contribute to your project.]
