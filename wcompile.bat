cd win64
cmake -G "MinGW Makefiles" -DCMAKE_TOOLCHAIN_FILE=../toolchain-windows64.cmake ..
mingw32-make
pause