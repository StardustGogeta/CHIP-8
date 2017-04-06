# Star-C8
[![Build Status](https://travis-ci.org/StardustGogeta/Star-C8.svg?branch=linux)](https://travis-ci.org/StardustGogeta/Star-C8)
An interpreter for the CHIP-8 virtual machine made by Joseph Weisbecker in the 1970s. This was developed and tested on Ubuntu 14.04 using the fantastic Code::Blocks IDE and GCC as the compiler.
This project also was written in the C language, and employs SDL2 and SDL2_mixer for graphics and audio respectively.
The executable file supports command line control in the following format: "./StarC8 tetris.c8". If using this method, place the file "beep.wav" in the same directory as the executable.

## Windows Setup
To get this project working on Windows, use the project file StarC8_win32.cbp for editing or building.
The libraries SDL2 and SDL2_mixer are required for running this, and the .DLL files must be placed in the same directory as the .EXE file.
Install these libraries in the folder C:\\MinGW, or the project file will need to be edited.
**Make sure to use the 32-bit versions of these libraries, as the x86-64 versions will create errors!**
