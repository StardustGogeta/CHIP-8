# Star-C8
[![Build Status](https://travis-ci.org/StardustGogeta/Star-C8.svg?branch=linux)](https://travis-ci.org/StardustGogeta/Star-C8)
An interpreter for the CHIP-8 virtual machine made by Joseph Weisbecker in the 1970s. This was developed and tested on Ubuntu 14.04 using the fantastic Code::Blocks IDE and GCC as the compiler.
This project also was written in the C language, and employs SDL2 and SDL2_mixer for graphics and audio respectively.
The executable file supports command line control in the following format: `./StarC8 tetris.c8`. If using this method, place the file `beep.wav` in the same directory as the executable.

## Windows Setup
To get this project working on Windows, use the project file `StarC8_win32.cbp` for editing or building.
The libraries SDL2 and SDL2_mixer are required for running this, and the .DLL files must be placed in the same directory as the .EXE file.
Install these libraries in the folder `C:\\MinGW`, or the project file will need to be edited.

### 32-bit Setup
Install 32-bit MinGW with Code::Blocks, or do it separately, placing it into the folder `C:\\MinGW`.
Install the i686 versions of each library in `C:\\MinGW`, by moving all of the `bin`, `include`, `share`, and `lib` folders from `SDL2` and `SDL2_mixer`.
Replace the `SDL2.dll` and `SDL2_mixer.dll` files in the `Debug`, `Release`, and root folders with the 32-bit versions from the `MinGW\\bin` folder.

### 64-bit Setup
Install MinGW64 into a directory of your choice. Go into the Code::Blocks compiler configuration and replace the GCC path with this directory path.
Install the x86-64 versions of each library in `C:\\MinGW`, by moving all of the `bin`, `include`, `share`, and `lib` folders from `SDL2` and `SDL2_mixer`.
The `.dll` files in the project directory are already 64-bit, so no extra work is necessary for these.
