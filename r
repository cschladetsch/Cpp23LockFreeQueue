
#!/bin/bash
echo "Building the project..."
cmake -S . -B build
cmake --build build
echo "Running the performance comparison..."
./build/run_tests
