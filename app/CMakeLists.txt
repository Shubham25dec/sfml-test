cmake_minimum_required(VERSION 3.10)
project(sfml-activity)

set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sfml/${ANDROID_ABI}")

add_library(sfml-system SHARED IMPORTED)
set_target_properties(sfml-system PROPERTIES
    IMPORTED_LOCATION ${SFML_DIR}/lib/libsfml-system-d.so)

add_library(sfml-window SHARED IMPORTED)
set_target_properties(sfml-window PROPERTIES
    IMPORTED_LOCATION ${SFML_DIR}/lib/libsfml-window-d.so)

add_library(sfml-graphics SHARED IMPORTED)
set_target_properties(sfml-graphics PROPERTIES
    IMPORTED_LOCATION ${SFML_DIR}/lib/libsfml-graphics-d.so)

add_library(sfml-main STATIC IMPORTED)
set_target_properties(sfml-main PROPERTIES
    IMPORTED_LOCATION ${SFML_DIR}/lib/libsfml-main-d.a)

add_library(main SHARED
    src/main/cpp/main.cpp)

target_include_directories(main PRIVATE
    ${SFML_DIR}/include)

target_link_libraries(main
    -Wl,--whole-archive sfml-main -Wl,--no-whole-archive
    sfml-graphics
    sfml-window
    sfml-system
    android
    log
    EGL
    GLESv1_CM)
