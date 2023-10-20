project(test CXX)
set(CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED True)
add_executable(${PROJECT_NAME} try.cpp "sort.cpp")
target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME}
                            PUBLIC
                            ${CMAKE_SOURCE_DIR}/include)