cmake_minimum_required(VERSION 3.9)
project(Pong CXX)
#CXX standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#Options:
option(SANITIZE "Build with sanitizer support if available" OFF)
option(VENDOR "Vendor libraries" OFF)
#CMAKE_BUILD_TYPE Release Debug

set(SRC
#Sources
src/main.cpp
src/game.cpp
#Headers
include/game.hpp
)

set(ASSETS_EMBED_WEB
)

#Check debug
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

#Check platform
if(${CMAKE_SYSTEM_NAME} MATCHES "Android")
	set(ANDROID TRUE)
	set(BUILD_NAME main)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        set(MACOSX TRUE)
	set(BUILD_NAME ${PROJECT_NAME})
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(WINDOWS TRUE)
	set(BUILD_NAME ${PROJECT_NAME})
elseif(EMSCRIPTEN)
	set(WEB TRUE)
	set(BUILD_NAME ${PROJECT_NAME})
else()
	set(BUILD_NAME ${PROJECT_NAME})
endif()

#Check LTO
include(CheckIPOSupported)
check_ipo_supported(RESULT LTOSupported OUTPUT error)

include_directories(include)
if(NOT ANDROID)
	if(WEB)
    		find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-static)

		add_compile_options(-sUSE_SDL=3 -sUSE_SDL_IMAGE=3)
		add_compile_options(${ASSETS_EMBED_WEB})
		add_link_options(-sUSE_SDL=3 -sUSE_SDL_IMAGE=3)
		add_link_options(${ASSETS_EMBED_WEB})

		set(CMAKE_EXECUTABLE_SUFFIX .js)
		
		configure_file(cmake/index.html.in ${CMAKE_BINARY_DIR}/index.html)
	elseif(VENDOR)
    		add_subdirectory(external/SDL3 EXCLUDE_FROM_ALL)
	else()
    		find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
	endif()
endif()

#Find and include SDL2
#Don't need for Android
if (ANDROID)
	message("Building for Android")

	add_library(${BUILD_NAME} SHARED)

	target_sources(${BUILD_NAME} PRIVATE ${SRC})

	target_link_libraries(${BUILD_NAME} PUBLIC SDL3::SDL3)
elseif(MACOSX)
	message("Building for MacOS")

	if(DEBUG)
		add_executable(${BUILD_NAME} ${SRC})
	else()
#Package into a application bundle
		add_executable(${BUILD_NAME} MACOSX_BUNDLE ${SRC})            
		set_target_properties(${BUILD_NAME} PROPERTIES
		    BUNDLE True
		    MACOSX_BUNDLE_ICON_FILE ${CMAKE_SOURCE_DIR}/cmake/${CMAKE_PROJECT_NAME}.ico
		    MACOSX_BUNDLE_GUI_IDENTIFIER org.cyao.${BUILD_NAME}
	   	    MACOSX_BUNDLE_BUNDLE_NAME ${BUILD_NAME}
		    MACOSX_BUNDLE_BUNDLE_VERSION "0.1"
		    MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
		    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/cmake/info.plist.in
		)
	endif()
	target_link_libraries(${BUILD_NAME} PRIVATE SDL3::SDL3)
elseif(WINDOWS)
	message("Building for windows")

	add_executable(${BUILD_NAME} WIN32 ${SRC})
	target_link_libraries(${BUILD_NAME} PRIVATE SDL3::SDL3)
else()
	message("Building for linux (probably)")

	add_executable(${BUILD_NAME} ${SRC})

	target_link_libraries(${BUILD_NAME} PRIVATE SDL3::SDL3)
endif()

# Set flags
if(MSVC)
	if(DEBUG)
		target_compile_options(${BUILD_NAME} PRIVATE /W4 /WX /MDd /Zi /Ob0 /Od /RTC1 /W3 /GR /EHsc)
		target_compile_definitions(${BUILD_NAME} PRIVATE /D_DEBUG /DWIN32 /D_WINDOWS /D_DEBUG)
	else()
		target_compile_options(${BUILD_NAME} PRIVATE /MD /O2 /Ob2)
		target_compile_definitions(${BUILD_NAME} PRIVATE /D_WINDOWS /DWIND32 /DNDEBUG)
	endif()
else()
	if(DEBUG)
		target_compile_options(${BUILD_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror -g)
		target_compile_definitions(${BUILD_NAME} PRIVATE -D_DEBUG)
	else()
		target_compile_options(${BUILD_NAME} PRIVATE -O3)
		target_compile_definitions(${BUILD_NAME} PRIVATE -DNDEBUG)
	endif()
endif()

#Sanitize
if(SANITIZE STREQUAL ON)
	message("Sanitizer emabled")
	add_compile_options(-fsanitize=address)
	add_link_options(-fsanitize=address)
endif()

#do LTO
if(LTOSupported AND NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Android")
	if(NOT DEBUG)
		message(STATUS "IPO / LTO enabled")
		set_property(TARGET ${BUILD_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
	endif()
else()
	message(STATUS "IPO / LTO not supported: <${error}>")
endif()

