cmake_minimum_required(VERSION 3.22)
project(CoolCompiler)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

set(CMAKE_CXX_STANDARD 17)

# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

file(COPY Examples DESTINATION ${CMAKE_BINARY_DIR})

add_subdirectory(Dependencies/fmt EXCLUDE_FROM_ALL)

file(GLOB LEXER_SOURCES
        ${PROJECT_SOURCE_DIR}/Lexer/*.h
        ${PROJECT_SOURCE_DIR}/Lexer/*.cpp)

file(GLOB PARSER_SOURCES
        ${PROJECT_SOURCE_DIR}/Parser/*.h
        ${PROJECT_SOURCE_DIR}/Parser/*.cpp
        ${PROJECT_SOURCE_DIR}/Parser/AST/*.h
        ${PROJECT_SOURCE_DIR}/Parser/AST/*.cpp
        ${PROJECT_SOURCE_DIR}/Parser/AST/Expression/*.h
        ${PROJECT_SOURCE_DIR}/Parser/AST/Expression/*.cpp
        ${PROJECT_SOURCE_DIR}/Parser/AST/Feature/*.h
        ${PROJECT_SOURCE_DIR}/Parser/AST/Feature/*.cpp)

file(GLOB SEMANTIC_SOURCES
        ${PROJECT_SOURCE_DIR}/Semantic/*.h
        ${PROJECT_SOURCE_DIR}/Semantic/*.cpp
        ${PROJECT_SOURCE_DIR}/Semantic/SymbolTable/*.h
        ${PROJECT_SOURCE_DIR}/Semantic/SymbolTable/*.cpp)

file(GLOB CODEGEN_SOURCES
        ${PROJECT_SOURCE_DIR}/CodeGen/*.h
        ${PROJECT_SOURCE_DIR}/CodeGen/*.cpp)

file(GLOB UTILS_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/Utils/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Utils/*.cpp)

add_library(CoolCompiler
        ${UTILS_SOURCES}
        ${LEXER_SOURCES}
        ${PARSER_SOURCES}
        ${SEMANTIC_SOURCES}
        ${CODEGEN_SOURCES})

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader)

target_link_libraries(CoolCompiler PRIVATE ${llvm_libs})
target_link_libraries(CoolCompiler PRIVATE fmt::fmt-header-only)
