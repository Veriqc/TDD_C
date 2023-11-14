cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

project(TDD_C CXX)
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
# Common utils
################################################################################
include(CMake/Utils.cmake)

################################################################################
# Additional Global Settings(add specific info there)
################################################################################
include(CMake/GlobalSettingsInclude.cmake OPTIONAL)

################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(PROJECT_NAME tdd_main)

################################################################################
# Source groups
################################################################################
add_subdirectory(${PROJECT_SOURCE_DIR}/extern/xtl)
add_subdirectory(${PROJECT_SOURCE_DIR}/extern/xtensor)
set(XTENSOR_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/extern/xtensor/include)
set(XTL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/extern/xtl/include)
set(Header_Files
    "include/CircuitOptimizer.hpp"
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
    "include/dd/Operations.hpp"
    "include/dd/Package.hpp"
    "include/dd/Package_fwd.hpp"
    "include/dd/Tdd.hpp"
    "include/dd/UniqueTable.hpp"
    "include/Definitions.hpp"
    "include/operations/ClassicControlledOperation.hpp"
    "include/operations/CompoundOperation.hpp"
    "include/operations/Control.hpp"
    "include/operations/Expression.hpp"
    "include/operations/NonUnitaryOperation.hpp"
    "include/operations/Operation.hpp"
    "include/operations/OpType.hpp"
    "include/operations/StandardOperation.hpp"
    "include/operations/SymbolicOperation.hpp"
    "include/parsers/qasm_parser/Parser.hpp"
    "include/parsers/qasm_parser/Scanner.hpp"
    "include/parsers/qasm_parser/Token.hpp"
    "include/Permutation.hpp"
    "include/QuantumComputation.hpp"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "include/CircuitOptimizer.cpp"
    "include/dd/Edge.cpp"
    "include/dd/Node.cpp"
    "include/dd/Operations.cpp"
    "include/operations/Expression.cpp"
    "include/operations/NonUnitaryOperation.cpp"
    "include/operations/Operation.cpp"
    "include/operations/StandardOperation.cpp"
    "include/operations/SymbolicOperation.cpp"
    "include/parsers/GRCSParser.cpp"
    "include/parsers/qasm_parser/Parser.cpp"
    "include/parsers/qasm_parser/Scanner.cpp"
    "include/parsers/QASMParser.cpp"
    "include/parsers/QCParser.cpp"
    "include/parsers/RealParser.cpp"
    "include/parsers/TFCParser.cpp"
    "include/QuantumComputation.cpp"
)

source_group("Source Files" FILES ${Source_Files} )

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

add_library(TDD_C SHARED ${ALL_FILES} )
target_include_directories(TDD_C PUBLIC ${PROJECT_SOURCE_DIR}/include ${XTENSOR_INCLUDE_DIR} ${XTL_INCLUDE_DIR})
target_compile_features(TDD_C PUBLIC cxx_auto_type)


################################################################################
# Target
################################################################################
add_subdirectory(extern/pybind11)
pybind11_add_module(TDD "include/python-c.cpp")
target_link_libraries(TDD PUBLIC TDD_C)


add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME} PUBLIC TDD_C)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_auto_type)

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "_CONSOLE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_CONSOLE"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE;
            /INCREMENTAL
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE;
            /INCREMENTAL
        )
    endif()
endif()
add_subdirectory(test)