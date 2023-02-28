# sukless-chess
The goal of this project is to provide a feature-rich yet minimalistic implementation of chess.
## vcpkg is highly reccomended
1. install vcpkg and the relavent triplet
```
git clone https://github.com/microsoft/vcpkg

windows:
.\vcpkg\bootstrap-vcpkg.bat

MacOS and Linux
./vcpkg/bootstrap-vcpkg.sh

All platforms from here

./vcpkg install sdl2 sdl2-image sdl2-mixer --triplet <your-triplet>

./vcpkg integrate install
```
## Build
```
 cmake -S . -B <your-build-folder> -DCMAKE_TOOLCHAIN_FILE=<path-to-vckpkg/scripts/buildsystems/vcpkg.cmake>
 cd <build-directory>
 make
 ```
 ## Run
 ### MacOS
 
```
cd <build-dir> ./chess
```
### Windows
```
cd <build-dir> ./Debug/chess
```
*do your shells equivalent of the bash ``` ./ ```
## Using Sukless-Chess

### Features
1. To view move history, press the left and right arrow keys. *History does not persist across multiple games.
