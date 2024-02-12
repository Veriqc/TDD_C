# set required cmake version
cmake_minimum_required(VERSION 3.19...3.28)
project(
  quantum-core
  LANGUAGES CXX
  VERSION 1.0.0
  DESCRIPTION "MQT Core - The Backbone of the Munich Quantum Toolkit")
set(MQT_CORE_INCLUDE_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/mqt-core")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# add main library code
add_subdirectory(src)

add_subdirectory(test)
