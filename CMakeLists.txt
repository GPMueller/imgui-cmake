cmake_minimum_required( VERSION 3.10 )

project( imgui-glfw )

set( CMAKE_CXX_STANDARD 11 )

set( THIRDPARTY_DIR ${CMAKE_CURRENT_LIST_DIR}/thirdparty )

set( IMGUI_COMPILE_GLFW ON  )
set( IMGUI_TARGET_WEB   OFF )

if( ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten" )
    set( IMGUI_COMPILE_GLFW OFF )
    set( IMGUI_TARGET_WEB   ON  )
endif()

############################################################
### GLFW subproject

if( IMGUI_COMPILE_GLFW )
    set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL "the folder is not in this repo" FORCE )
    set( GLFW_BUILD_TESTS    OFF CACHE BOOL "the folder is not in this repo" FORCE )
    set( GLFW_BUILD_DOCS     OFF CACHE BOOL "the folder is not in this repo" FORCE )

    add_subdirectory( ${THIRDPARTY_DIR}/glfw )
endif()


############################################################
### Dear ImGui library

set( IMGUI_SRC_DIR ${THIRDPARTY_DIR}/imgui )

set( IMGUI_PUBLIC_HEADERS ${IMGUI_SRC_DIR}/imgui.h )

set( IMGUI_SOURCES
    ${IMGUI_SRC_DIR}/imgui.cpp
    ${IMGUI_SRC_DIR}/imgui_draw.cpp
    ${IMGUI_SRC_DIR}/imgui_demo.cpp
    ${IMGUI_SRC_DIR}/imgui_widgets.cpp
    ${THIRDPARTY_DIR}/glad/glad.c )

add_library( imgui STATIC ${IMGUI_SOURCES} )
target_include_directories( imgui PRIVATE ${THIRDPARTY_DIR} ${IMGUI_SRC_DIR} )


############################################################
### fmtlib

add_subdirectory( ${THIRDPARTY_DIR}/fmt )


####################################################
### The application

set( MAIN_APP_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/imgui_impl/opengl3.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/imgui_impl/glfw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/imgui_impl/fonts.cpp )

set( MAIN_APP_NAME imguiapp )
add_executable( ${MAIN_APP_NAME} ${MAIN_APP_SOURCES} )
target_include_directories( ${MAIN_APP_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${THIRDPARTY_DIR}
    ${THIRDPARTY_DIR}/fmt/include )
target_link_libraries( ${MAIN_APP_NAME} imgui fmt::fmt )

if( IMGUI_COMPILE_GLFW )
    target_link_libraries( ${MAIN_APP_NAME} glfw )
elseif( IMGUI_TARGET_WEB )
    ### Set specific flags for emcc and to place the result under ui-web/js/
    set_target_properties( ${MAIN_APP_NAME} PROPERTIES
        COMPILE_FLAGS "-O3 -g -s WASM=1 -s USE_WEBGL2=1 -s USE_GLFW=3 -s FULL_ES3=1 -s DEMANGLE_SUPPORT=1 -s DISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR=1 -s ASSERTIONS=1 -s DISABLE_EXCEPTION_CATCHING=0 -s FORCE_FILESYSTEM=1"
        LINK_FLAGS "-O3 -g -s WASM=1 -s USE_WEBGL2=1 -s USE_GLFW=3 -s FULL_ES3=1 -s DEMANGLE_SUPPORT=1 -s DISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR=1 -s ASSERTIONS=1 -s DISABLE_EXCEPTION_CATCHING=0 -s FORCE_FILESYSTEM=1 -s ALLOW_MEMORY_GROWTH=1 -s EXTRA_EXPORTED_RUNTIME_METHODS=['cwrap, FS']"
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/ui-web/js
        # SUFFIX ".html"
    )

    target_compile_definitions( ${MAIN_APP_NAME} PRIVATE -DIMGUIAPP_TARGET_WEB)
endif()