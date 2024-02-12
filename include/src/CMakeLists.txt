set(MQT_CORE_TARGET_NAME ${PROJECT_NAME})
set(MQT_CORE_TARGETS "")

if(NOT TARGET project_warnings)
  # Use the warnings specified in CompilerWarnings.cmake
  add_library(project_warnings INTERFACE)

  # Standard compiler warnings
  include(${PROJECT_SOURCE_DIR}/cmake/CompilerWarnings.cmake)
  set_project_warnings(project_warnings)

  # Add MQT alias
  add_library(MQT::ProjectWarnings ALIAS project_warnings)
  set_target_properties(project_warnings PROPERTIES EXPORT_NAME ProjectWarnings)
endif()

if(NOT TARGET project_options)
  # Use the options specified in CompilerOptions.cmake
  add_library(project_options INTERFACE)

  # Standard compiler options
  include(${PROJECT_SOURCE_DIR}/cmake/CompilerOptions.cmake)
  enable_project_options(project_options)

  # Sanitizer options if supported by compiler
  include(${PROJECT_SOURCE_DIR}/cmake/Sanitizers.cmake)
  enable_sanitizers(project_options)

  # Add MQT alias
  add_library(MQT::ProjectOptions ALIAS project_options)
  set_target_properties(project_options PROPERTIES EXPORT_NAME ProjectOptions)
endif()

if(NOT TARGET ${MQT_CORE_TARGET_NAME})
  # main project library
  add_library(
    ${MQT_CORE_TARGET_NAME}
    ${MQT_CORE_INCLUDE_BUILD_DIR}/CircuitOptimizer.hpp
    ${MQT_CORE_INCLUDE_BUILD_DIR}/Definitions.hpp
    ${MQT_CORE_INCLUDE_BUILD_DIR}/operations/Expression.hpp
    ${MQT_CORE_INCLUDE_BUILD_DIR}/operations/ClassicControlledOperation.hpp
    ${MQT_CORE_INCLUDE_BUILD_DIR}/operations/CompoundOperation.hpp
    ${MQT_CORE_INCLUDE_BUILD_DIR}/operations/Control.hpp
    ${MQT_CORE_INCLUDE_BUILD_DIR}/operations/NonUnitaryOperation.hpp
    ${MQT_CORE_INCLUDE_BUILD_DIR}/operations/Operation.hpp
    ${MQT_CORE_INCLUDE_BUILD_DIR}/operations/StandardOperation.hpp
    ${MQT_CORE_INCLUDE_BUILD_DIR}/operations/SymbolicOperation.hpp
    ${MQT_CORE_INCLUDE_BUILD_DIR}/parsers/qasm3_parser/Scanner.hpp
    ${MQT_CORE_INCLUDE_BUILD_DIR}/parsers/qasm3_parser/Token.hpp
    ${MQT_CORE_INCLUDE_BUILD_DIR}/parsers/qasm3_parser/Parser.hpp
    ${MQT_CORE_INCLUDE_BUILD_DIR}/parsers/qasm3_parser/Statement.hpp
    ${MQT_CORE_INCLUDE_BUILD_DIR}/parsers/qasm3_parser/Types.hpp
    ${MQT_CORE_INCLUDE_BUILD_DIR}/parsers/qasm3_parser/InstVisitor.hpp
    ${MQT_CORE_INCLUDE_BUILD_DIR}/parsers/qasm3_parser/NestedEnvironment.hpp
    ${MQT_CORE_INCLUDE_BUILD_DIR}/parsers/qasm3_parser/Exception.hpp
    ${MQT_CORE_INCLUDE_BUILD_DIR}/parsers/qasm3_parser/Gate.hpp
    ${MQT_CORE_INCLUDE_BUILD_DIR}/parsers/qasm3_parser/StdGates.hpp
    ${MQT_CORE_INCLUDE_BUILD_DIR}/parsers/qasm3_parser/passes/CompilerPass.hpp
    ${MQT_CORE_INCLUDE_BUILD_DIR}/parsers/qasm3_parser/passes/ConstEvalPass.hpp
    ${MQT_CORE_INCLUDE_BUILD_DIR}/parsers/qasm3_parser/passes/TypeCheckPass.hpp
    ${MQT_CORE_INCLUDE_BUILD_DIR}/Permutation.hpp
    ${MQT_CORE_INCLUDE_BUILD_DIR}/QuantumComputation.hpp
    CircuitOptimizer.cpp
    operations/ClassicControlledOperation.cpp
    operations/Expression.cpp
    operations/NonUnitaryOperation.cpp
    operations/Operation.cpp
    operations/StandardOperation.cpp
    operations/SymbolicOperation.cpp
    parsers/GRCSParser.cpp
    parsers/QASM3Parser.cpp
    parsers/QCParser.cpp
    parsers/RealParser.cpp
    parsers/TFCParser.cpp
    parsers/qasm3_parser/Parser.cpp
    parsers/qasm3_parser/Scanner.cpp
    parsers/qasm3_parser/Types.cpp
    parsers/qasm3_parser/Statement.cpp
    parsers/qasm3_parser/passes/ConstEvalPass.cpp
    parsers/qasm3_parser/passes/TypeCheckPass.cpp
    QuantumComputation.cpp)

  # set include directories
  target_include_directories(
    ${MQT_CORE_TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${MQT_CORE_INCLUDE_BUILD_DIR}>
                                   $<INSTALL_INTERFACE:${MQT_CORE_INCLUDE_INSTALL_DIR}>)

  # add options and warnings to the library
  target_link_libraries(${MQT_CORE_TARGET_NAME} PRIVATE MQT::ProjectOptions MQT::ProjectWarnings)

  # add MQT alias
  add_library(MQT::Core ALIAS ${MQT_CORE_TARGET_NAME})
  set_target_properties(
    ${MQT_CORE_TARGET_NAME}
    PROPERTIES VERSION ${PROJECT_VERSION}
               SOVERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
               EXPORT_NAME Core)
  list(APPEND MQT_CORE_TARGETS ${MQT_CORE_TARGET_NAME})
endif()