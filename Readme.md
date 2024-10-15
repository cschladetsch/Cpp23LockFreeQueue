# Lock-Free Queue Comparison Project

This project provides a comparison between a custom `LockFreeQueue` implementation and Boost's `boost::lockfree::queue`. It runs performance tests to compare the two implementations in a multi-threaded environment.

## Sample

![image](./resources/Image-01.jpg)

## Table of Contents

1. [Installation](#installation)
2. [Usage](#usage)
3. [Running the Tests](#running-the-tests)
4. [Custom Implementation](#custom-implementation)
5. [Boost Lock-Free Queue](#boost-lock-free-queue)
6. [Results](#results)
7. [License](#license)

## Installation

### Prerequisites

- **C++17 compatible compiler**: GCC or Clang
- **Boost Lock-Free library**: Ensure you have the necessary headers installed.

You can install the required dependencies by running:

```bash
sudo apt-get install libboost-lockfree-dev
```

### Clone the Repository

```bash
git clone https://github.com/cschladetsch/Cpp23LockFreeQueue.git
cd Cpp23LockFreeQueue
```

## Usage

### Configuration

Make sure the correct compiler is set in the project configuration:

```bash
export CC=/usr/bin/gcc
export CXX=/usr/bin/g++
```

### Building the Project

To build the project, run the `r` script, which automates the build and execution:

```bash
./r
```

This will:

- Build the project.
- Run the performance comparison between the custom `LockFreeQueue` and `boost::lockfree::queue`.
- Display results for minimum, maximum, mean, and standard deviation of execution times.

## Running the Tests

The project includes a script `./r` that builds the project and runs the performance comparison tests.

You can modify the test parameters inside `src/run_tests.cpp` to adjust the number of iterations, producers, and consumers:

```cpp
int num_iterations = 1000; // Number of iterations
int num_producers = 1;     // Number of producer threads
int num_consumers = 1;     // Number of consumer threads
```

### Running Performance Comparison

Once built, the `run_tests` executable will perform performance tests on both the custom queue and Boost's queue and provide a summary of the results:

```bash
Running the performance comparison...
Custom Queue Results:
- Mean: ...
- Min: ...
- Max: ...
- Stddev: ...

Boost Queue Results:
- Mean: ...
- Min: ...
- Max: ...
- Stddev: ...
```

## Custom Implementation

The custom `LockFreeQueue` is defined in `include/lock_free_queue.hpp`. This implementation provides thread-safe, lock-free enqueue and dequeue operations, with a simple API comparable to Boost's queue.

## Boost Lock-Free Queue

This project uses `boost::lockfree::queue` to compare performance with the custom implementation. Boost's queue is a well-established, production-quality lock-free queue implementation.

## Results

The output of the tests will show the performance differences between the two implementations:

- **Mean Time**: The average time taken for enqueue and dequeue operations over multiple iterations.
- **Min/Max Time**: The shortest and longest time recorded for operations.
- **Standard Deviation**: The variability in the operation times.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.
