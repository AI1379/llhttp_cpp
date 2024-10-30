cmake_minimum_required(VERSION 3.14)
project(llhttp_cpp VERSION 0.1.0)
cmake_policy(SET CMP0135 NEW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

include(FetchContent)

message(STATUS "PATH: $ENV{PATH}")
message(STATUS "C++ compiler id: ${CMAKE_CXX_COMPILER_ID}")

if (WIN32)
    message(STATUS "Configuring for Windows")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    cmake_policy(SET CMP0091 NEW)
    set(CMAKE_CXX_FLAGS_DEBUG
            "${CMAKE_CXX_FLAGS_DEBUG} -D_ITERATOR_DEBUG_LEVEL=2 -MTd")
    set(CMAKE_CXX_FLAGS_RELEASE
            "${CMAKE_CXX_FLAGS_RELEASE} -D_ITERATOR_DEBUG_LEVEL=0 -MT")
elseif (UNIX AND NOT APPLE)
    message(STATUS "Configuring for Linux")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
elseif (APPLE)
    message(STATUS "Configuring for macOS")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_LIBCPP_DEBUG=1")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
else ()
    message(FATAL_ERROR "Unsupported platform")
endif ()

option(LLHTTP_CPP_TEST_ENABLED "Enable testing" ON)
option(LLHTTP_CPP_ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(LLHTTP_CPP_GENERATE_FROM_TS "Generate C++ code from TypeScript" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_DEBUG
        "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -Wall"
)
if (NOT MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -Wextra -Wpedantic -Werror")
endif ()
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O3")
if (LLHTTP_CPP_ENABLE_ASAN)
    # TODO: check why clang complain about -fsanitize=address when build on Windows
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fsanitize=address")
endif ()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(LLHTTP_CPP_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(LLHTTP_CPP_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(LLHTTP_CPP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(LLHTTP_CPP_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)
set(LLHTTP_CPP_MODULE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

list(APPEND CMAKE_MODULE_PATH ${LLHTTP_CPP_MODULE_DIR})

FetchContent_Declare(llhttp
        URL "https://github.com/nodejs/llhttp/archive/refs/tags/release/v9.2.1.tar.gz")

set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")
set(BUILD_STATIC_LIBS ON CACHE INTERNAL "")
FetchContent_MakeAvailable(llhttp)

if (LLHTTP_CPP_TEST_ENABLED)
    add_compile_definitions(LLHTTP_CPP_TEST_ENABLED)
endif ()

add_subdirectory(src)

if (LLHTTP_CPP_TEST_ENABLED)
    add_subdirectory(test)
endif ()
