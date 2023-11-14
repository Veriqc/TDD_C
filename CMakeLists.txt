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
set(XTENSOR_FILES
    "extern/xtensor/xaccessible.hpp"
    "extern/xtensor/xaccumulator.hpp"
    "extern/xtensor/xadapt.hpp"
    "extern/xtensor/xarray.hpp"
    "extern/xtensor/xassign.hpp"
    "extern/xtensor/xaxis_iterator.hpp"
    "extern/xtensor/xaxis_slice_iterator.hpp"
    "extern/xtensor/xblockwise_reducer.hpp"
    "extern/xtensor/xblockwise_reducer_functors.hpp"
    "extern/xtensor/xbroadcast.hpp"
    "extern/xtensor/xbuffer_adaptor.hpp"
    "extern/xtensor/xbuilder.hpp"
    "extern/xtensor/xchunked_array.hpp"
    "extern/xtensor/xchunked_assign.hpp"
    "extern/xtensor/xchunked_view.hpp"
    "extern/xtensor/xcomplex.hpp"
    "extern/xtensor/xcontainer.hpp"
    "extern/xtensor/xcsv.hpp"
    "extern/xtensor/xdynamic_view.hpp"
    "extern/xtensor/xeval.hpp"
    "extern/xtensor/xexception.hpp"
    "extern/xtensor/xexpression.hpp"
    "extern/xtensor/xexpression_holder.hpp"
    "extern/xtensor/xexpression_traits.hpp"
    "extern/xtensor/xfixed.hpp"
    "extern/xtensor/xfunction.hpp"
    "extern/xtensor/xfunctor_view.hpp"
    "extern/xtensor/xgenerator.hpp"
    "extern/xtensor/xhistogram.hpp"
    "extern/xtensor/xindex_view.hpp"
    "extern/xtensor/xinfo.hpp"
    "extern/xtensor/xio.hpp"
    "extern/xtensor/xiterable.hpp"
    "extern/xtensor/xiterator.hpp"
    "extern/xtensor/xjson.hpp"
    "extern/xtensor/xlayout.hpp"
    "extern/xtensor/xmanipulation.hpp"
    "extern/xtensor/xmasked_view.hpp"
    "extern/xtensor/xmath.hpp"
    "extern/xtensor/xmime.hpp"
    "extern/xtensor/xmultiindex_iterator.hpp"
    "extern/xtensor/xnoalias.hpp"
    "extern/xtensor/xnorm.hpp"
    "extern/xtensor/xnpy.hpp"
    "extern/xtensor/xoffset_view.hpp"
    "extern/xtensor/xoperation.hpp"
    "extern/xtensor/xoptional.hpp"
    "extern/xtensor/xoptional_assembly.hpp"
    "extern/xtensor/xoptional_assembly_base.hpp"
    "extern/xtensor/xoptional_assembly_storage.hpp"
    "extern/xtensor/xpad.hpp"
    "extern/xtensor/xrandom.hpp"
    "extern/xtensor/xreducer.hpp"
    "extern/xtensor/xrepeat.hpp"
    "extern/xtensor/xscalar.hpp"
    "extern/xtensor/xsemantic.hpp"
    "extern/xtensor/xset_operation.hpp"
    "extern/xtensor/xshape.hpp"
    "extern/xtensor/xslice.hpp"
    "extern/xtensor/xsort.hpp"
    "extern/xtensor/xstorage.hpp"
    "extern/xtensor/xstrided_view.hpp"
    "extern/xtensor/xstrided_view_base.hpp"
    "extern/xtensor/xstrides.hpp"
    "extern/xtensor/xtensor.hpp"
    "extern/xtensor/xtensor_config.hpp"
    "extern/xtensor/xtensor_forward.hpp"
    "extern/xtensor/xtensor_simd.hpp"
    "extern/xtensor/xutils.hpp"
    "extern/xtensor/xvectorize.hpp"
    "extern/xtensor/xview.hpp"
    "extern/xtensor/xview_utils.hpp"
    "extern/xtl/xany.hpp"
    "extern/xtl/xbase64.hpp"
    "extern/xtl/xbasic_fixed_string.hpp"
    "extern/xtl/xclosure.hpp"
    "extern/xtl/xcompare.hpp"
    "extern/xtl/xcomplex.hpp"
    "extern/xtl/xcomplex_sequence.hpp"
    "extern/xtl/xdynamic_bitset.hpp"
    "extern/xtl/xfunctional.hpp"
    "extern/xtl/xhalf_float.hpp"
    "extern/xtl/xhalf_float_impl.hpp"
    "extern/xtl/xhash.hpp"
    "extern/xtl/xhierarchy_generator.hpp"
    "extern/xtl/xiterator_base.hpp"
    "extern/xtl/xjson.hpp"
    "extern/xtl/xmasked_value.hpp"
    "extern/xtl/xmasked_value_meta.hpp"
    "extern/xtl/xmeta_utils.hpp"
    "extern/xtl/xmultimethods.hpp"
    "extern/xtl/xoptional.hpp"
    "extern/xtl/xoptional_meta.hpp"
    "extern/xtl/xoptional_sequence.hpp"
    "extern/xtl/xplatform.hpp"
    "extern/xtl/xproxy_wrapper.hpp"
    "extern/xtl/xsequence.hpp"
    "extern/xtl/xspan.hpp"
    "extern/xtl/xspan_impl.hpp"
    "extern/xtl/xsystem.hpp"
    "extern/xtl/xtl_config.hpp"
    "extern/xtl/xtype_traits.hpp"
    "extern/xtl/xvariant.hpp"
    "extern/xtl/xvariant_impl.hpp"
    "extern/xtl/xvisitor.hpp"
)

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
set(test_file
    "test/test.hpp"
    "test/test_tdd.cpp"
    # "test/test_tn.cpp"
)
source_group("Source Files" FILES ${Source_Files} )

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)
add_library(XTENSOR SHARED ${XTENSOR_FILES} )
set_target_properties(XTENSOR PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(XTENSOR PUBLIC ${PROJECT_SOURCE_DIR}/extern)
target_compile_features(XTENSOR PUBLIC cxx_auto_type)

add_library(TDD SHARED ${ALL_FILES} )
target_include_directories(TDD 
    PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
    )
target_link_libraries(TDD PUBLIC XTENSOR)
target_compile_features(TDD PUBLIC cxx_auto_type)


################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME} PUBLIC XTENSOR TDD)
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