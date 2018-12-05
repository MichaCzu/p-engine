# the name of the target operating system
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR i686)

# which compilers to use for C and C++
set(CMAKE_C_COMPILER clang)
set(CMAKE_C_FLAGS "-m32")
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "-m32")
set(CMAKE_SHARED_LINKER_FLAGS "-m32")

set(CMAKE_SYSTEM_LIBRARY_PATH /lib32 /usr/lib32 /usr/local/lib32)
set(FIND_LIBRARY_USE_LIB64_PATHS OFF)
set(CMAKE_IGNORE_PATH /lib /usr/lib /usr/local/lib)

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
