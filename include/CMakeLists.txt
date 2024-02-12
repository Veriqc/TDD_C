add_subdirectory(src)

# add tdd
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

message(STATUS "add library")

add_library(${DD_LIBRARY} INTERFACE)
target_include_directories(${DD_LIBRARY} INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include> 
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include> 
    ${XTENSOR_INCLUDE_DIRS} 
    ${XTL_INCLUDE_DIRS}
)
target_compile_features(${DD_LIBRARY} INTERFACE cxx_auto_type)

# Add only .hpp files from the dd folder
target_sources(${DD_LIBRARY} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/dd/Cir_tn.hpp>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/dd/Complex.hpp>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/dd/ComplexCache.hpp>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/dd/ComplexNumbers.hpp>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/dd/ComplexTable.hpp>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/dd/ComplexValue.hpp>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/dd/ComputeTable.hpp>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/dd/Control.hpp>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/dd/DDpackageConfig.hpp>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/dd/Definitions.hpp>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/dd/Edge.hpp>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/dd/Export.hpp>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/dd/GateXarray.hpp>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/dd/Node.hpp>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/dd/Package.hpp>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/dd/Package_fwd.hpp>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/dd/Tdd.hpp>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/dd/Tensor.hpp>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/dd/UniqueTable.hpp>
)