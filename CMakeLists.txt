cmake_minimum_required(VERSION 3.11)
project(hmw_1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

#Library with db
add_library(student_dp STATIC
        src/student.cpp
        src/utils.cpp
        src/db1.cpp
        src/db2.cpp
        src/db3.cpp
        src/db_1_lm.cpp
        src/db1_sp.cpp
        src/db_2_lm.cpp
        src/dp2_sp.cpp
        src/sort_phone.cpp

)

target_include_directories(student_dp PUBLIC include)

#Main
add_executable(hmw_1
        src/main.cpp
)

target_link_libraries(hmw_1
        student_dp
)

#Benchmark
add_executable(benchmark
        benchmarks/benchmark.cpp
)

target_include_directories(benchmark PUBLIC include)

target_link_libraries(benchmark
        student_dp
)
add_executable(srt_bench
        benchmarks/sorting_benchmark.cpp)

target_include_directories(srt_bench PUBLIC include)

target_link_libraries(srt_bench
        student_dp)