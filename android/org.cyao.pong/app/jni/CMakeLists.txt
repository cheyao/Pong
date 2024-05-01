cmake_minimum_required(VERSION 3.6)

project(GAME)

set(CMAKE_LINKER /Users/ray/Library/Android/sdk/ndk/25.1.8937393/toolchains/llvm/prebuilt/darwin-x86_64/bin/ld)
# armeabi-v7a requires cpufeatures library
# include(AndroidNdkModules)
# android_ndk_import_module_cpufeatures()

# SDL sources are in a subfolder named "SDL"
add_subdirectory(SDL2)
include_directories(include)

# Your game and its CMakeLists.txt are in a subfolder named "src"
add_subdirectory(project)
