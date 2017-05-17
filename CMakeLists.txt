cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)
project(sv)

option(SHARED "Build as shared library" OFF)

if(CMAKE_VERSION VERSION_LESS "3.1")
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_C_FLAGS "-std=c99 ${CMAKE_C_FLAGS}")
    endif()
else()
    set(CMAKE_C_STANDARD 99)
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
if(MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W2")
else()
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g3 -DDEBUG")
    else()
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
    endif()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra -Werror -fomit-frame-pointer")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-missing-field-initializers -Wno-unused-parameter -Wno-unused-function")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-variable -Wno-unused-value -Wno-unused-result")
endif()

set(sv_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(sv_SOURCE_DIR  ${CMAKE_CURRENT_LIST_DIR}/src)
set(sv_TEST_DIR  ${CMAKE_CURRENT_LIST_DIR}/test)

file(GLOB sv_HEADERS ${sv_HEADERS} ${sv_INCLUDE_DIR}/*.h)
file(GLOB sv_SOURCES ${sv_SOURCES} ${sv_SOURCE_DIR}/*.c)

include_directories(${sv_INCLUDE_DIR})
if(SHARED)
    if(MSVC)
        set(CMAKE_FLAGS
            CMAKE_CXX_FLAGS
            CMAKE_CXX_FLAGS_DEBUG
            CMAKE_CXX_FLAGS_RELEASE
            CMAKE_C_FLAGS
            CMAKE_C_FLAGS_DEBUG
            CMAKE_C_FLAGS_RELEASE
        )
        foreach(CMAKE_FLAG ${CMAKE_FLAGS})
            string(REPLACE "/MD" "/MT" ${CMAKE_FLAG} "${${CMAKE_FLAG}}")
            string(REPLACE "/MDd" "/MTd" ${CMAKE_FLAG} "${${CMAKE_FLAG}}")
        endforeach()
    endif()

    add_library(${PROJECT_NAME} SHARED ${sv_SOURCES} ${sv_HEADERS})
    target_compile_definitions(${PROJECT_NAME} PUBLIC BUILD_DYNAMIC_LINK=1)
else()
    add_library(${PROJECT_NAME} STATIC ${sv_SOURCES} ${sv_HEADERS})
endif()
target_compile_definitions(${PROJECT_NAME} PUBLIC SEMVER_COMPILE=1)

enable_testing()
add_subdirectory(${sv_TEST_DIR})
