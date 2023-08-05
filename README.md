# Nintendo Wii boot1 decompilation
This is a **work-in-progress** decompilation of boot1 (revisions a, b), which is found in the Nintendo Wii. It aims to be as accurate as possible, while also being able to be used on ARM-based emulators, such as QEMU.

## Status
The decomp is currently in very early stages, and only a few functions have been implemented.

## Building
This project uses regular Makefiles, although we do want the project to use CMake in the future, as it is more versatile than regular Makefiles.

## Pre-requsites
Currently, Linux and macOS systems are supported. We might also add support for Cygwin, just like the original if there is enough demand.

But regardless, you will need the following software installed:

- A GCC ARM cross-compiler, which you can download from [here](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads).
- GNU Make, which is bundled with Xcode on macOS, or available under Linux through your software repositories.
- If you are going to help us out with the decompilation, you will also need Ghidra, which can be downloaded from [here](https://github.com/NationalSecurityAgency/ghidra).

### Building boot1
To be added.
