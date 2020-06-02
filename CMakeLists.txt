cmake_minimum_required( VERSION 3.10 )

project( imgui-glfw )

set( CMAKE_CXX_STANDARD 11 )

set( THIRDPARTY_DIR ${CMAKE_CURRENT_LIST_DIR}/thirdparty )


############################################################
### GLFW subproject

set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL "the folder is not in this repo" FORCE )
set( GLFW_BUILD_TESTS OFF CACHE BOOL "the folder is not in this repo" FORCE )
set( GLFW_BUILD_DOCS OFF CACHE BOOL "the folder is not in this repo" FORCE )

add_subdirectory( ${THIRDPARTY_DIR}/glfw )


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
target_link_libraries( ${MAIN_APP_NAME} imgui glfw fmt::fmt )
