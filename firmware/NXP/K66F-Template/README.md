## before you start:
- make sure the folders in clang.mk amd arm.mk are pointing to the right toolchain.

## How to prepare the firmware image
### compile and instrument the target with clang
1. cd into Target
2. mkdir build
3. cmake ..
4. make
### compile the rest with arm
1. cd to the root
2. mkdir build
3. cmake ..
4. make