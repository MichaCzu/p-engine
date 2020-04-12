cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

if(WIN32)
  set(GUI_TYPE WIN32)
endif(WIN32)
if(APPLE)
  set(GUI_TYPE MACOSX_BUNDLE)
endif(APPLE)

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt." )
endif()

project(P-Engine)
enable_language(CXX)
set(CMAKE_BUILD_TYPE Release)


#the name of the target operating system
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# cross compilers to use for C, C++ and Fortran
set(MINGW_ROOT E:\\MinGW)
set(CMAKE_C_COMPILER ${MINGW_ROOT}\\bin\\gcc.exe)
set(CMAKE_CXX_COMPILER ${MINGW_ROOT}\\bin\\g++.exe)
set(CMAKE_RC_COMPILER ${MINGW_ROOT}\\bin\\windres.exe)
set(CMAKE_MAKE_PROGRAM ${MINGW_ROOT}\\bin\\mingw32-make.exe)
set(PKG_CONFIG_PATH ${MINGW_ROOT}\\lib\\pkgconfig)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wno-narrowing")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wno-narrowing")
# set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
# set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
# set(CMAKE_STATIC_LIBRARY_LINK_CXX_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -static-libgcc")
set(CMAKE_FIND_ROOT_PATH ${MINGW_ROOT})

# modify default behavior of FIND_XXX() commands
# set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
# set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(EXECUTABLE_NAME "P-Engine")
#set(PROJECT_INCLUDE_DIRS "")

file(GLOB_RECURSE PROJECT_SOURCES "src/*.cpp")
include_directories("src/")
#include_directories(${MINGW_ROOT}/include/)
INCLUDE_DIRECTORIES(${MINGW_ROOT}\\include\\) 
add_executable (${EXECUTABLE_NAME} ${PROJECT_SOURCES})


INCLUDE(FindPkgConfig)
#find_package(Boost REQUIRED filesystem)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
PKG_SEARCH_MODULE(SDL2MIXER REQUIRED SDL2_mixer>=2.0.0)
PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)
PKG_SEARCH_MODULE(SDL2NET REQUIRED SDL2_net>=2.0.0)
PKG_SEARCH_MODULE(SDL2GPU REQUIRED SDL2_gpu)
find_package (Threads)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIR} ${SDL2GPU_INCLUDE_DIR} ${SDL2MIXER_INCLUDE_DIR} ${SDL2TTF_INCLUDE_DIR})#${SDL2NET_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${EXECUTABLE_NAME} ${SDL2_LIBRARIES} ${SDL2MIXER_LIBRARIES} ${SDL2TTF_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${SDL2GPU_LIBRARIES} ) #${SDL2NET_LIBRARIES} ${Boost_LIBRARIES})
