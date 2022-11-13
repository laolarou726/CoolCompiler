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

add_library(CoolCompiler Lexer/Token.cpp Lexer/Token.h Lexer/Error.cpp Lexer/Error.h Lexer/Lexer.cpp Lexer/Lexer.h Parser/AST/AST.cpp Parser/AST/AST.h Parser/AST/Program.cpp Parser/AST/Program.h Parser/AST/Class.cpp Parser/AST/Class.h Parser/AST/Feature/FeatureMethod.cpp Parser/AST/Feature/FeatureMethod.h Parser/AST/Feature/FeatureAttribute.cpp Parser/AST/Feature/FeatureAttribute.h Parser/AST/Formal.cpp Parser/AST/Formal.h Parser/AST/Expression/Expression.cpp Parser/AST/Expression/Expression.h Parser/AST/Expression/Assignment.cpp Parser/AST/Expression/Assignment.h Parser/AST/Expression/Id.cpp Parser/AST/Expression/Id.h Parser/AST/Expression/Integer.cpp Parser/AST/Expression/Integer.h Parser/AST/Expression/String.cpp Parser/AST/Expression/String.h Parser/AST/Expression/Boolean.cpp Parser/AST/Expression/Boolean.h Parser/AST/Expression/Self.cpp Parser/AST/Expression/Self.h Parser/AST/Expression/Block.cpp Parser/AST/Expression/Block.h Parser/AST/Expression/MethodAccess.cpp Parser/AST/Expression/MethodAccess.h Parser/AST/Expression/AtMethodAccess.cpp Parser/AST/Expression/AtMethodAccess.h Parser/AST/Expression/SelfMethodAccess.cpp Parser/AST/Expression/SelfMethodAccess.h Parser/AST/Expression/MathBinop.cpp Parser/AST/Expression/MathBinop.h Parser/AST/Expression/Comparison.cpp Parser/AST/Expression/Comparison.h Parser/AST/Expression/If.cpp Parser/AST/Expression/If.h Parser/AST/Expression/While.cpp Parser/AST/Expression/While.h Parser/AST/Expression/New.cpp Parser/AST/Expression/New.h Parser/AST/Expression/IsVoid.cpp Parser/AST/Expression/IsVoid.h Parser/AST/Expression/Tilde.cpp Parser/AST/Expression/Tilde.h Parser/AST/Expression/Not.cpp Parser/AST/Expression/Not.h Parser/AST/Expression/Case.cpp Parser/AST/Expression/Case.h Parser/AST/Expression/CaseAction.cpp Parser/AST/Expression/CaseAction.h Parser/AST/Expression/Let.cpp Parser/AST/Expression/Let.h Parser/AST/Expression/InnerLet.cpp Parser/AST/Expression/InnerLet.h Parser/Parser.cpp Parser/Parser.h Utils/ConsoleColors.h Utils/StringUtils.h Semantic/SemanticAnalyzer.cpp Semantic/SemanticAnalyzer.h Semantic/SymbolTable/SymbolTableEntry.h Semantic/SymbolTable/SymbolTable.h Semantic/SymbolTable/List.h Semantic/SymbolTable/Scope.h Semantic/SymbolTable/ScopeEntry.h Parser/AST/Feature/FeatureBase.h Parser/AST/Feature/FeatureBase.cpp)

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader)

target_link_libraries(CoolCompiler PRIVATE ${llvm_libs})
target_link_libraries(CoolCompiler PRIVATE fmt::fmt-header-only)
