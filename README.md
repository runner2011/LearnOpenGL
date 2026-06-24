# LearnOpenGL
LearnOpenGL.com tutorial

## Cross-platform build

This project can be built with CMake on Windows, Linux, and macOS.

Requirements:

- CMake 3.16 or newer
- A C++17 compiler
- OpenGL
- GLFW development files
- Assimp development files

Linux example:

```sh
sudo apt install cmake g++ libglfw3-dev libassimp-dev libgl1-mesa-dev
./build.sh
./build.sh Release
./build.sh run
```

macOS example:

```sh
brew install cmake glfw assimp
./build.sh
```

Windows example:

```bat
build.bat
build.bat Release
build.bat run
```

CMake can also be used directly:

```sh
cmake -S . -B build
cmake --build build --config Debug
```

On Windows, CMake first tries installed packages and then falls back to the existing libraries in `lib/`.
