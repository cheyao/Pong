cmake_minimum_required(VERSION 3.9)
project(Pong CXX)

# CXX standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Check platform
if(${CMAKE_SYSTEM_NAME} MATCHES "Android")
	set(ANDROID TRUE)
	set(BUILD_NAME main)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        set(MACOSX TRUE)
	set(BUILD_NAME Pong)
else()
	set(BUILD_NAME Pong)
endif()

# Set buildname to main
set(SRC
src/main.cpp
src/game.cpp
)

# Check debug
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release CACHE STRING
		"Choose the type of build, options are:
		 Debug Release"
      	FORCE)
endif()

if(CMAKE_BUILD_TYPE STREQUAL Debug)
	message("Debug enabled")
	set(DEBUG TRUE)
endif()

if(DEBUG)
	add_definitions(-D_DEBUG)
else()
	add_definitions(-DNDEBUG)
endif()

# Options:
option(SANITIZE "Build with sanitizer support if available" OFF)
# CMAKE_BUILD_TYPE Release Debug

# Sanitize
if(SANITIZE STREQUAL ON)
	add_compile_options(-fsanitize=address)
	add_link_options(-fsanitize=address)
endif()

# Optimization flags
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Check LTO
include(CheckIPOSupported)
check_ipo_supported(RESULT LTOSupported OUTPUT error)

include_directories(include)

# Find and include SDL2
# Don't need for Android
if (ANDROID)
	message("Building for Android")
	add_library(${BUILD_NAME} SHARED)

	target_sources(${BUILD_NAME} PRIVATE ${SRC})

	target_link_libraries(main SDL2 SDL2main)
elseif(MACOSX)
	message("Building for MacOS")
	find_package(SDL2 REQUIRED)
	include_directories(${SDL2_INCLUDE_DIRS})
	add_executable(${BUILD_NAME} MACOSX_BUNDLE ${SRC})            
	set_target_properties(${BUILD_NAME} PROPERTIES
	    BUNDLE True
	    MACOSX_BUNDLE_ICON_FILE ${CMAKE_SOURCE_DIR}/cmake/Pong.ico
	    MACOSX_BUNDLE_GUI_IDENTIFIER org.cyao.${BUILD_NAME}
	    MACOSX_BUNDLE_BUNDLE_NAME ${BUILD_NAME}
	    MACOSX_BUNDLE_BUNDLE_VERSION "0.1"
	    MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
	    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/cmake/info.plist.in
	)
	target_link_libraries(${BUILD_NAME} ${SDL2_LIBRARIES})
else()
	message("Building for Else")
	find_package(SDL2 REQUIRED)

	include_directories(${SDL2_INCLUDE_DIRS})

	add_executable(${BUILD_NAME} ${SRC})

	target_link_libraries(${BUILD_NAME} ${SDL2_LIBRARIES})
endif()


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
		set_target_properties(${BUILD_NAME} PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")
        endif()
endif()

# Set errors if debug
if(DEBUG)
	if(MSVC)
		target_compile_options(${BUILD_NAME} PRIVATE /W4 /WX)
	else()
		target_compile_options(${BUILD_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
	endif()
endif()

# do LTO
if(LTOSupported AND NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Android")
	if(NOT DEBUG)
		message(STATUS "IPO / LTO enabled")
		set_property(TARGET ${BUILD_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
	endif()
else()
	message(STATUS "IPO / LTO not supported: <${error}>")
endif()
