cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

project(TDD_C LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
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
# Nuget packages function stub.
################################################################################
function(use_package TARGET PACKAGE VERSION)
    message(WARNING "No implementation of use_package. Create yours. "
                    "Package \"${PACKAGE}\" with version \"${VERSION}\" "
                    "for target \"${TARGET}\" is ignored!")
endfunction()



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

find_package(xtensor-python REQUIRED)
if(NOT xtensor-python_FOUND)
    message(FATAL_ERROR "xtensor-python not found. Please make sure xtensor-python is installed.")
else()
    message(STATUS "xtensor-python include directories: ${xtensor-python_INCLUDE_DIRS}")
endif()

find_package(pybind11 REQUIRED)
if(NOT pybind11_FOUND)
    message(FATAL_ERROR "pybind11 not found. Please make sure pybind11 is installed.")
else()
    message(STATUS "pybind11 include directories: ${pybind11_INCLUDE_DIRS}")
endif()


# set(XTENSOR_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/extern/xtensor/include)
# set(XTL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/extern/xtl/include)
# set(XPY_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/extern/xtensor-python/include)
message(STATUS "add file")
set(Header_Files
    "include/dd/Complex.hpp"
    "include/dd/ComplexCache.hpp"
    "include/dd/ComplexNumbers.hpp"
    "include/dd/ComplexTable.hpp"
    "include/dd/ComplexValue.hpp"
    "include/dd/ComputeTable.hpp"
    "include/dd/Control.hpp"
    "include/dd/DDpackageConfig.hpp"
    "include/dd/Definitions.hpp"
    "include/dd/Edge.hpp"
    "include/dd/Export.hpp"
    "include/dd/GateXarray.hpp"
    "include/dd/Node.hpp"
    "include/dd/Tensor.hpp"
    "include/dd/Package.hpp"
    "include/dd/Package_fwd.hpp"
    "include/dd/Tdd.hpp"
    "include/dd/UniqueTable.hpp"
    "include/Definitions.hpp"
    "include/operations/Control.hpp"
)
source_group("Header Files" FILES ${Header_Files})

# set(Source_Files
#     "include/dd/Edge.cpp"
#     # "include/dd/Node.cpp"
# )

# source_group("Source Files" FILES ${Source_Files} )

set(ALL_FILES
    ${Header_Files}
    # ${Source_Files}
)

add_library(TDD_C INTERFACE )
target_include_directories(TDD_C INTERFACE ${PROJECT_SOURCE_DIR}/include ${XTENSOR_INCLUDE_DIRS} ${XTL_INCLUDE_DIRS})
target_compile_features(TDD_C INTERFACE cxx_auto_type)


################################################################################
# Target
################################################################################

pybind11_add_module(TDD "include/python-c.cpp")
find_package(Python3 REQUIRED NumPy)
if(Python3_NumPy_FOUND)
    message(STATUS "add numpy from ${Python3_NumPy_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "NumPy not found for Python ${python_version}, try to run:\n ${Python3_EXECUTABLE} -m pip install numpy")
endif()
target_link_libraries(TDD PUBLIC TDD_C)
target_include_directories(TDD PUBLIC ${xtensor-python_INCLUDE_DIRS} ${Python3_NumPy_INCLUDE_DIRS})

add_subdirectory(test)