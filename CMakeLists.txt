cmake_minimum_required(VERSION 3.28)

# set(CMAKE_C_COMPILER "/opt/homebrew/bin/gcc-15")
# set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm/bin/clang")

project(c-template C)

file(
        DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.42.0/CPM.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
        EXPECTED_HASH SHA256=2020b4fc42dba44817983e06342e682ecfc3d2f484a581f11cc5731fbe4dce8a
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_executable(
    ${PROJECT_NAME}
    "src/main.c"
    #"src/rlights.h"
)

# If you want only dynamic string support
# CPMAddPackage("gh:antirez/sds#master")

# If you need exceptions...
#CPMAddPackage("gh:ThrowTheSwitch/CException@1.3.4")

# Simple Graphics in OpenGL
CPMAddPackage("gh:raysan5/raylib#5.5")

# Battle tested library for containers and more
#CPMAddPackage("gh:GNOME/glib@2.85.0")

# More performant alternative to classic malloc
#CPMAddPackage("gh:microsoft/mimalloc@3.0.3")

# Meme libraries 
# Algebraic Data types in C
#CPMAddPackage("gh:Hirrolot/datatype99@1.6.5")

# Macro generation of virtual functions and interfaces (dynamic dispatch)
#CPMAddPackage("gh:Hirrolot/interface99@1.0.2")

# SDS library
# target_include_directories(${PROJECT_NAME} PRIVATE ${sds_SOURCE_DIR})
# set(SDS_FILES ${sds_SOURCE_DIR}/sds.c ${sds_SOURCE_DIR}/sds.h)
# add_library(sds ${SDS_FILES})

# CException library
# target_include_directories(${PROJECT_NAME} PRIVATE ${cexception_SOURCE_DIR})
# set(CEXCEPTION_FILES ${CException_SOURCE_DIR}/lib/CException.c ${CException_SOURCE_DIR}/lib/CException.h)
# add_library(cexception ${CEXCEPTION_FILES})

FetchContent_Declare(
    mlib
    GIT_REPOSITORY https://github.com/P-p-H-d/mlib
    GIT_TAG        V0.8.0
)

FetchContent_GetProperties(mlib)
if (NOT mlib_POPULATED)
    FetchContent_MakeAvailable(mlib)
    add_library(mlib INTERFACE)
    target_include_directories(mlib INTERFACE "${mlib_SOURCE_DIR}")
endif()

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W3)  
    # Missing macro limit 1 for MSVC
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wpedantic)
    
    #if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    #    target_compile_options(${PROJECT_NAME} PRIVATE -fmacro-backtrace-limit=1)
    #elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    #    target_compile_options(${PROJECT_NAME} PRIVATE -ftrack-macro-expansion=0)
    #endif()
endif ()

target_link_libraries(${PROJECT_NAME} PRIVATE m mlib raylib) # sds cexception raylib mimalloc-static datatype99 interface99
