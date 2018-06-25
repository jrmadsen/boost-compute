cmake_minimum_required(VERSION 3.1.3 FATAL_ERROR)

project(BoostCompute LANGUAGES C CXX VERSION 0.0.1)

find_package(BoostCompute REQUIRED)
find_package(OpenCL REQUIRED)

message(STATUS "BoostCompute include: ${BoostCompute_INCLUDE_DIRS}")
message(STATUS "BoostCompute libraries: ${BoostCompute_LIBRARIES}")
message(STATUS "OpenCL include: ${OpenCL_INCLUDE_DIRS}")
message(STATUS "OpenCL libraries: ${OpenCL_LIBRARIES}")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")

include_directories(${BoostCompute_INCLUDE_DIRS} ${OpenCL_INCLUDE_DIRS})

add_executable(hello hello.cpp)
target_link_libraries(hello ${BoostCompute_LIBRARIES} ${OpenCL_LIBRARIES})

