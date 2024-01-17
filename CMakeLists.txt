cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)
option(BUILD_PYBIND "Build the pybind11 component" ON)
option(BUILD_TEST "Build the test executable")
project(TDD_C LANGUAGES CXX VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
configure_file("cmake/Version.hpp.in" ${CMAKE_BINARY_DIR}/dd/TDDCVersion.hpp)
################################################################################
# Set target arch type if empty. Visual studio solution generator provides it.
################################################################################
if(NOT CMAKE_VS_PLATFORM_NAME)
    set(CMAKE_VS_PLATFORM_NAME "x64")
endif()
message("${CMAKE_VS_PLATFORM_NAME} architecture in use")

if(NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64"
     OR "${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86"))
    message(FATAL_ERROR "${CMAKE_VS_PLATFORM_NAME} arch is not supported!")
endif()

################################################################################
# Global configuration types
################################################################################
set(CMAKE_CONFIGURATION_TYPES
    "Debug"
    "Release"
    CACHE STRING "" FORCE
)

################################################################################
# Global compiler options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_CXX_FLAGS "")
    set(CMAKE_CXX_FLAGS_DEBUG "")
    set(CMAKE_CXX_FLAGS_RELEASE "")
endif()

################################################################################
# Global linker options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_EXE_LINKER_FLAGS "")
    set(CMAKE_MODULE_LINKER_FLAGS "")
    set(CMAKE_SHARED_LINKER_FLAGS "")
    set(CMAKE_STATIC_LINKER_FLAGS "")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS}")
endif()



################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(PROJECT_NAME tdd_main)

################################################################################
# Source groups
################################################################################
message(STATUS "add submodule")
find_package(xtensor REQUIRED)
if(NOT xtensor_FOUND)
    message(FATAL_ERROR "xtensor not found. Please make sure xtensor is installed.")
else()
    message(STATUS "xtensor include directories: ${xtensor_INCLUDE_DIRS}")
endif()

find_package(xtl REQUIRED)
if(NOT xtl_FOUND)
    message(FATAL_ERROR "xtl not found. Please make sure xtl is installed.")
else()
    message(STATUS "xtl include directories: ${xtl_INCLUDE_DIRS}")
endif()


# set(XTENSOR_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/extern/xtensor/include)
# set(XTL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/extern/xtl/include)
# set(XPY_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/extern/xtensor-python/include)
message(STATUS "add library")

add_library(TDD_C INTERFACE )
target_include_directories(TDD_C INTERFACE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include> $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/generated>
$<INSTALL_INTERFACE:include> ${XTENSOR_INCLUDE_DIRS} ${XTL_INCLUDE_DIRS})
target_compile_features(TDD_C INTERFACE cxx_auto_type)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
        DESTINATION include
        FILES_MATCHING PATTERN "*.hpp")

# You can also specify additional installation details such as version and compatibility
install(TARGETS TDD_C EXPORT TDD_C_Targets
        INCLUDES DESTINATION include)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "TDD_CConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)

install(EXPORT TDD_C_Targets
        FILE cmake/TDD_CConfig.cmake
        DESTINATION lib/cmake/TDD_C)
install(FILES ${CMAKE_BINARY_DIR}/dd/TDDCVersion.hpp DESTINATION include/dd)

################################################################################
# Target
################################################################################
if(BUILD_PYBIND)
    add_subdirectory(python) 
endif()
if(BUILD_TEST)
    add_subdirectory(test)
endif()