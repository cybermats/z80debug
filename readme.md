# Z80 Debugger

This is a debugger for the Z80 CPU without any ties to any specific computer.
I was lacking a reasonable (in my view) GUI based Z80 debugger, so I wrote one myself.

It currently handles the basics:

* Load one or more binary files at specific addresses in memory.
* Load accompanying list files for debug info.
* Both memory and registers can be modified.
* Breakpoints.

## Build

To build, you need:

* [Qt 5.15](www.qt.io)
* CMake - Comes with qt
* [libz80](https://github.com/ggambetta/libz80) 2.1.0 from ggambetta

It should hopefully be straightforward to build based on the CMakeLists.txt.

## Todo
* Separate listing of all breakpoints.
* Conditional breakpoints
* IO handling (currently completely ignored. IN returns 0).
* Editing and compiling.

## Licenses
This software is released under [GPL v3](license.md).

This software is using [qhexedit2](https://github.com/Simsys/qhexedit2) by simsys.

