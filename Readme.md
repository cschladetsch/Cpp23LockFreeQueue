# Lock-Free Queue Implementation

A high-performance, lock-free queue implementation in C++23 with a multi-threaded producer-consumer test harness. This project demonstrates concurrent programming techniques and the use of atomic operations.

## Features

- Lock-free queue implementation using C++23 features
- Multi-threaded producer-consumer test harness
- Colorful console output using the Rang library
- Easy-to-use build and run scripts

## Requirements

- C++23 compatible compiler (e.g., GCC 11+ or Clang 13+)
- POSIX-compliant operating system (Linux, macOS, etc.)
- curl or wget (for downloading the Rang library)

## Files

- `lock_free_queue.hpp`: Lock-free queue implementation
- `main.cpp`: Producer-consumer test harness
- `build.sh`: Compilation script (auto-downloads Rang library if needed)
- `r`: Build and run script with configurable parameters

## Building the Project

To build the project, run:

```bash
./build.sh
```

This script will automatically download the Rang library if it's not already present in the project directory. The script requires either `curl` or `wget` to be installed for downloading the Rang library.

## Running the Program

Use the `r` script to build and run the program:

```bash
./r [p] [c] [i]
```

Where:
- `p`: number of producers (default: 3)
- `c`: number of consumers (default: 2)
- `i`: items per producer (default: 10000)

For verbose output, use the `-v` or `--verbose` flag:

```bash
./r -v [p] [c] [i]
```

Examples:
- Run with default settings: `./r`
- Run with custom settings: `./r 5 4 20000`
- Run with verbose output: `./r -v 5 4 20000`

## Implementation Details

The lock-free queue is implemented using atomic operations to ensure thread-safety without the use of locks. It supports the following operations:

- `push`: Add an item to the queue
- `pop`: Remove and return an item from the queue
- `empty`: Check if the queue is empty
- `get_size`: Get the current size of the queue

The implementation uses a single-linked list with atomic pointers for the head and tail.

## Performance Considerations

This implementation aims to provide high concurrency and low latency for multi-threaded producer-consumer scenarios. However, actual performance may vary depending on the specific use case and system characteristics.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

MIT.
