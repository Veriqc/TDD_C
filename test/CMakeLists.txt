project(test_sort CXX)
set(CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED True)
add_executable(${PROJECT_NAME} "sort.cpp" )
target_compile_features(${PROJECT_NAME} PUBLIC cxx_auto_type)
target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME}
                            PUBLIC
                            ${CMAKE_SOURCE_DIR}/include)

project(test_cnot CXX)
set(CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED True)
add_executable(${PROJECT_NAME}  "cnot.cpp")
target_compile_features(${PROJECT_NAME} PUBLIC cxx_auto_type)
target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME}
                            PUBLIC
                            ${CMAKE_SOURCE_DIR}/include)