cmake_minimum_required(VERSION 3.1...3.16)
project(hy454 VERSION 0.1 LANGUAGES CXX)

# SDL
set(SDL_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/SDL/include)

# SDL_Mixer
set(SDL_MIXER_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/SDL_Mixer/include)

set(SDL_MIXER_LIB ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/SDL_Mixer/lib/x64)

# TODO: for x86
set(SDL_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/SDL/lib/x64)
include_directories(${SDL_INCLUDE})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/fmtlib)
link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/fmtlib/fmtd.lib)
# If I wanted to be "safe", I wouldn't be using the weird standard library API
# but sure whatever.
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_subdirectory(Engine)
add_subdirectory(UnitTests/UnitTest1)
add_subdirectory(UnitTests/UnitTest2)
add_subdirectory(UnitTests/GenerateGridMap)
add_subdirectory(UnitTests/Main)