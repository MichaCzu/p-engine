# the name of the target operating system
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)
set(TOOLCHAIN_PREFIX x86_64-w64-mingw32)

# cross compilers to use for C, C++ and Fortran
set(CMAKE_C_COMPILER D:/mingw/bin/gcc.exe)
set(CMAKE_CXX_COMPILER D:/mingw/bin/g++.exe)
set(CMAKE_RC_COMPILER D:/mingw/bin/${TOOLCHAIN_PREFIX}-windres.exe)
set(ENV{PKG_CONFIG_PATH} D:/mingw/${TOOLCHAIN_PREFIX}/lib/pkgconfig)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libstdc++ -static-libgcc")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc")
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_C_FLAGS} -static-libgcc -s")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS} -static-libgcc -static-libstdc++ -s")
# target environment on the build host system
set(CMAKE_FIND_ROOT_PATH D:/mingw/${TOOLCHAIN_PREFIX})

# modify default behavior of FIND_XXX() commands
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
