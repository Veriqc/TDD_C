cmake_minimum_required(VERSION 3.20.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)
option(BUILD_PYBIND "Build the pybind11 component" OFF)
option(BUILD_TEST "Build the test executable" on)
option(BUILD_INSTALL "Build the make install" OFF)

set(DD_LIBRARY "TDD_C")
set(CORE_LIBRARY "TDD_CORE")
set(PYBIND_COM "TDDpy")
set(CORE_INCLUDE_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/mqt-core")

project(TDD_C LANGUAGES CXX VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)

configure_file("cmake/Version.hpp.in" ${CMAKE_BINARY_DIR}/include/dd/TDDCVersion.hpp)
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

add_subdirectory(include)

# You can also specify additional installation details such as version and compatibility
# ... your existing CMake configuration ...

if(BUILD_INSTALL)
    # TODO: modify here
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp")
    # Export the targets
    install(TARGETS TDD_C EXPORT TDD_C_Targets
            INCLUDES DESTINATION include)

    # Create and install the config files
    include(CMakePackageConfigHelpers)
    write_basic_package_version_file(
        TDD_CConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)

    configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/TDD_CConfig.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/TDD_CConfig.cmake"
        INSTALL_DESTINATION lib/cmake/TDD_C)

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/TDD_CConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/TDD_CConfigVersion.cmake"
        DESTINATION lib/cmake/TDD_C)

    install(EXPORT TDD_C_Targets
            FILE TDD_CTargets.cmake
            DESTINATION lib/cmake/TDD_C)

    install(FILES ${CMAKE_BINARY_DIR}/include/dd/TDDCVersion.hpp DESTINATION include/dd)

    if(NOT TARGET uninstall)
            configure_file(
                "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
                "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
                IMMEDIATE @ONLY)
        
            add_custom_target(uninstall
                COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
    endif()
endif()
    
################################################################################
# Target
################################################################################
if(BUILD_PYBIND)
    add_subdirectory(python) 
endif()
if(BUILD_TEST)
    add_subdirectory(test)
endif()