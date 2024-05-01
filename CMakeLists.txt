cmake_minimum_required(VERSION 3.9)
project(Pong CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build, options are:
        Debug Release"
      FORCE)
endif()

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    message("Debug enabled")
    add_definitions(-D_DEBUG)
else()
    add_definitions(-DNDEBUG)
endif()

if(SANITIZE STREQUAL ON)
add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Check LTO
include(CheckIPOSupported)
check_ipo_supported(RESULT LTOSupported OUTPUT error)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
# find_package(SDL2_image REQUIRED)
# include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

# tinyfiledialogs
include_directories(extern/tinyfiledialogs)

# src
include_directories(include)
add_executable(Pong
src/main.cpp
src/game.cpp
)

target_link_libraries(Pong ${SDL2_LIBRARIES})

if(ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_EXE
        NAMES "clang-tidy"
        DOC "Path to clang-tidy executable")
    if(NOT CLANG_TIDY_EXE)
        message(STATUS "clang-tidy not found.")
    else()
        message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
        set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}")
    endif()

    if(CLANG_TIDY_EXE)
	    set_target_properties(AFIV PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")
    endif()
endif()

# Set errors if debug
if(CMAKE_BUILD_TYPE STREQUAL Debug)
if(MSVC)
    target_compile_options(Pong PRIVATE /W4 /WX)
else()
    target_compile_options(Pong PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
endif()

# do LTO
if(LTOSupported)
if(NOT CMAKE_BUILD_TYPE STREQUAL Debug)
    message(STATUS "IPO / LTO enabled")
    set_property(TARGET Pong PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()
else()
    message(STATUS "IPO / LTO not supported: <${error}>")
endif()
