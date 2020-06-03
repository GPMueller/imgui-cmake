imgui-cmake
========================================

A cross-platform, C++11, Dear ImGui app
using CMake as build system and GLFW as
windowing backend.


Native application
----------------------------------------

Build the app by calling

```
mkdir -p build && cd build
cmake ..
cmake --build . -j4
cd ..
```


Web application
----------------------------------------

You need to have emscripten available.
Then, to build,

```
source /path/to/emsdk/emsdk_env.sh
mkdir -p build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=/path/to/emsdk/emscripten/<version>/cmake/Modules/Platform/Emscripten.cmake
cmake --build . -j4
cd ..
```

And to run the app, for example

```
cd ui-web
python3 -m http.server
open index.html
```


Dependencies
----------------------------------------

* Dear ImGui
* fmtlib
* GLFW
* glad
* OpenGL 3