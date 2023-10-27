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

set(PROJECT_NAME TDD_C)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "include/Cir_import.h"
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
    "include/dd/GateMatrixDefinitions.hpp"
    "include/dd/Node.hpp"
    "include/dd/Operations.hpp"
    "include/dd/Package.hpp"
    "include/dd/Package_fwd.hpp"
    "include/dd/Tdd.hpp"
    "include/dd/Tensor.hpp"
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
    "include/xtensor/xaccessible.hpp"
    "include/xtensor/xaccumulator.hpp"
    "include/xtensor/xadapt.hpp"
    "include/xtensor/xarray.hpp"
    "include/xtensor/xassign.hpp"
    "include/xtensor/xaxis_iterator.hpp"
    "include/xtensor/xaxis_slice_iterator.hpp"
    "include/xtensor/xblockwise_reducer.hpp"
    "include/xtensor/xblockwise_reducer_functors.hpp"
    "include/xtensor/xbroadcast.hpp"
    "include/xtensor/xbuffer_adaptor.hpp"
    "include/xtensor/xbuilder.hpp"
    "include/xtensor/xchunked_array.hpp"
    "include/xtensor/xchunked_assign.hpp"
    "include/xtensor/xchunked_view.hpp"
    "include/xtensor/xcomplex.hpp"
    "include/xtensor/xcontainer.hpp"
    "include/xtensor/xcsv.hpp"
    "include/xtensor/xdynamic_view.hpp"
    "include/xtensor/xeval.hpp"
    "include/xtensor/xexception.hpp"
    "include/xtensor/xexpression.hpp"
    "include/xtensor/xexpression_holder.hpp"
    "include/xtensor/xexpression_traits.hpp"
    "include/xtensor/xfixed.hpp"
    "include/xtensor/xfunction.hpp"
    "include/xtensor/xfunctor_view.hpp"
    "include/xtensor/xgenerator.hpp"
    "include/xtensor/xhistogram.hpp"
    "include/xtensor/xindex_view.hpp"
    "include/xtensor/xinfo.hpp"
    "include/xtensor/xio.hpp"
    "include/xtensor/xiterable.hpp"
    "include/xtensor/xiterator.hpp"
    "include/xtensor/xjson.hpp"
    "include/xtensor/xlayout.hpp"
    "include/xtensor/xmanipulation.hpp"
    "include/xtensor/xmasked_view.hpp"
    "include/xtensor/xmath.hpp"
    "include/xtensor/xmime.hpp"
    "include/xtensor/xmultiindex_iterator.hpp"
    "include/xtensor/xnoalias.hpp"
    "include/xtensor/xnorm.hpp"
    "include/xtensor/xnpy.hpp"
    "include/xtensor/xoffset_view.hpp"
    "include/xtensor/xoperation.hpp"
    "include/xtensor/xoptional.hpp"
    "include/xtensor/xoptional_assembly.hpp"
    "include/xtensor/xoptional_assembly_base.hpp"
    "include/xtensor/xoptional_assembly_storage.hpp"
    "include/xtensor/xpad.hpp"
    "include/xtensor/xrandom.hpp"
    "include/xtensor/xreducer.hpp"
    "include/xtensor/xrepeat.hpp"
    "include/xtensor/xscalar.hpp"
    "include/xtensor/xsemantic.hpp"
    "include/xtensor/xset_operation.hpp"
    "include/xtensor/xshape.hpp"
    "include/xtensor/xslice.hpp"
    "include/xtensor/xsort.hpp"
    "include/xtensor/xstorage.hpp"
    "include/xtensor/xstrided_view.hpp"
    "include/xtensor/xstrided_view_base.hpp"
    "include/xtensor/xstrides.hpp"
    "include/xtensor/xtensor.hpp"
    "include/xtensor/xtensor_config.hpp"
    "include/xtensor/xtensor_forward.hpp"
    "include/xtensor/xtensor_simd.hpp"
    "include/xtensor/xutils.hpp"
    "include/xtensor/xvectorize.hpp"
    "include/xtensor/xview.hpp"
    "include/xtensor/xview_utils.hpp"
    "include/xtl/xany.hpp"
    "include/xtl/xbase64.hpp"
    "include/xtl/xbasic_fixed_string.hpp"
    "include/xtl/xclosure.hpp"
    "include/xtl/xcompare.hpp"
    "include/xtl/xcomplex.hpp"
    "include/xtl/xcomplex_sequence.hpp"
    "include/xtl/xdynamic_bitset.hpp"
    "include/xtl/xfunctional.hpp"
    "include/xtl/xhalf_float.hpp"
    "include/xtl/xhalf_float_impl.hpp"
    "include/xtl/xhash.hpp"
    "include/xtl/xhierarchy_generator.hpp"
    "include/xtl/xiterator_base.hpp"
    "include/xtl/xjson.hpp"
    "include/xtl/xmasked_value.hpp"
    "include/xtl/xmasked_value_meta.hpp"
    "include/xtl/xmeta_utils.hpp"
    "include/xtl/xmultimethods.hpp"
    "include/xtl/xoptional.hpp"
    "include/xtl/xoptional_meta.hpp"
    "include/xtl/xoptional_sequence.hpp"
    "include/xtl/xplatform.hpp"
    "include/xtl/xproxy_wrapper.hpp"
    "include/xtl/xsequence.hpp"
    "include/xtl/xspan.hpp"
    "include/xtl/xspan_impl.hpp"
    "include/xtl/xsystem.hpp"
    "include/xtl/xtl_config.hpp"
    "include/xtl/xtype_traits.hpp"
    "include/xtl/xvariant.hpp"
    "include/xtl/xvariant_impl.hpp"
    "include/xtl/xvisitor.hpp"
    "test/test.hpp"
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
    "test/test_circuit.cpp"
    "test/test_tdd.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES} "main.cpp")

target_compile_features(${PROJECT_NAME} PUBLIC cxx_auto_type)
target_link_directories(${PROJECT_NAME} 
                        PUBLIC 
                        ${PROJECT_SOURCE_DIR}/include
                        ${PROJECT_SOURCE_DIR}/test
)
target_include_directories(${PROJECT_NAME}
                            PUBLIC
                            ${PROJECT_BINARY_DIR}
                            ${PROJECT_SOURCE_DIR}/include
                            ${PROJECT_SOURCE_DIR}/test
)
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