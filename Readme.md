## Building the Project

1. Ensure you have the following installed:
   - CMake (version 3.14 or higher)
   - G++ (with C++23 support)
   - Git

2. Clone the repository:
   ```
   git clone https://github.com/yourusername/lock-free-queue.git
   cd lock-free-queue
   ```

3. Create a build directory and run CMake:
   ```
   mkdir build
   cd build
   cmake ..
   ```

4. Build the project:
   ```
   cmake --build .
   ```

5. (Optional) To build with Boost support for performance comparison:
   ```
   cmake -DUSE_BOOST=ON ..
   cmake --build .
   ```

If you encounter any issues related to the compiler, ensure that g++ is installed and in your PATH. You can check this by running:
```
g++ --version
```

If g++ is not found, you may need to install it:
- On Ubuntu/Debian: `sudo apt-get install g++`
- On CentOS/RHEL: `sudo yum install gcc-c++`

After installing g++, repeat steps 3 and 4 to rebuild the project.
