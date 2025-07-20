cmake_minimum_required(VERSION 3.31)
project(learning_c__)

set(CMAKE_CXX_STANDARD 20)

add_executable(benchmark_mutex_queue benchmark_mutex_queue.cpp
        includes/MarketTick.hpp
        MarketTick.cpp)
add_executable(benchmark_lock_free_queue benchmark_lock_free_queue.cpp
        includes/MarketTick.hpp
        MarketTick.cpp
        includes/LockFreeSPSCQueue.hpp
)

