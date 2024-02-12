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

# Define ${DD_LIBRARY} as an INTERFACE library since it's header-only
add_library(${DD_LIBRARY} INTERFACE)

# Specify include directories for ${DD_LIBRARY}
target_include_directories(${DD_LIBRARY} INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include> 
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include> 
    ${XTENSOR_INCLUDE_DIRS} 
    ${XTL_INCLUDE_DIRS}
)

# Specify compile features for ${DD_LIBRARY}
target_compile_features(${DD_LIBRARY} INTERFACE cxx_auto_type)

# Link ${DD_LIBRARY} with ${CORE_LIBRARY}. Since ${DD_LIBRARY} is an INTERFACE library,
# it does not link directly but specifies that consumers of ${DD_LIBRARY} need to link with ${CORE_LIBRARY}
target_link_libraries(${DD_LIBRARY} INTERFACE ${CORE_LIBRARY})

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
