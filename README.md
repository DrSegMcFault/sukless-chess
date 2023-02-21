# sukless-chess
The goal of this project is to provide a feature-rich yet minimalistic implementation of chess.
## vcpkg is highly reccomended
1. install vcpkg and the relavent triplet
2. ./vcpkg install sdl2 sdl2-image

## build
```
 cmake -S . -B <your-build-folder> -DCMAKE_TOOLCHAIN_FILE=<path-to-vckpkg/scripts/buildsystems/vcpkg.cmake>
 cd <build-directory>
 make
 ```
