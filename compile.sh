#!/bin/bash
com=1
run=1
debug=0
if [ -f CMakeLists.txt ]; then

  for var in $@
  do
    if [ "$var" == "-nc" ] || [ "$var" == "--nocompile" ]; then
      com=0
    elif [ "$var" == "-nr" ]  || [ "$var" = "--norun" ]; then
      run=0
    elif [ "$var" == "-d" ]  || [ "$var" = "--debug" ]; then
      debug=1
    fi
  done

  if [ "$com" = 1 ]; then
    cd bin
    cmake .. || exit 1
    make || exit 1
    cd ..
  fi

  if [ "$run" = 1 ] || [ "$debug" = 1 ]; then
    cd bin
    if [ "$debug" = 1 ]; then
     gdb ./P-Engine
    else
     ./P-Engine
   fi
    cd ..
  fi
fi
