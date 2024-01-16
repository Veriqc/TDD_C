# List your test source files here
add_definitions(-DPROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")
set(TEST_SOURCES
    # test_whole.cpp
    test_tn.cpp
    test_tdd.cpp
    test_bv.cpp
    # Add new test files here
)

# Loop through each source file to create an executable and link libraries
foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_link_libraries(${TEST_NAME} PRIVATE TDD_C)
endforeach()
