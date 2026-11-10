# ----------------------------------------------------------------------------------------
#  CMAKE File
#  Copyright (C) 2020 - 2026  Helmut Fieres
# ----------------------------------------------------------------------------------------
cmake_minimum_required( VERSION 3.29 )

project( T64-Compiler C CXX ASM )

set( CMAKE_C_STANDARD 11 )
set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_C_EXTENSIONS ON )
set( CMAKE_EXPORT_COMPILE_COMMANDS)
set( CMAKE_CXX_STANDARD_REQUIRED ON)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )

# Cross-platform compiler warnings

if(MSVC)
    add_compile_options( /W4 /permissive- )
else()
    add_compile_options( 
        -Wall 
        -Wno-unused-parameter 
        -Wno-gnu-anonymous-struct 
        -Wnested-anon-types
        -Wextra
        -Wconversion
        -Wsign-conversion
        -Wpedantic
        -Wold-style-cast
        )
endif()

add_library(ELFIO INTERFACE)
target_include_directories(ELFIO INTERFACE ${CMAKE_SOURCE_DIR}/../ELFIO)

add_executable( ${PROJECT_NAME} 

    compiler.h 
    Compiler.cpp
    ConfigOpt.cpp 
    SrcFile.cpp
    Annotation.cpp
    SymTab.cpp
    Scanner.cpp 
    Parser.cpp 
    CodeGen.cpp 
    ListFileGen.cpp 
    ObjFileGen.cpp 
)

target_link_libraries( ${PROJECT_NAME} PRIVATE 

    ELFIO
)