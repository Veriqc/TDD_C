project(test CXX)
add_executable(${PROJECT_NAME} try.cpp)
target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME}
                            PUBLIC
                            ${CMAKE_SOURCE_DIR}/include)