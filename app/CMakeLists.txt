cmake_minimum_required(VERSION 3.10)
project(sfml-activity)

set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sfml/${ANDROID_ABI}")

add_library(sfml-system SHARED IMPORTED)
set_target_properties(sfml-system PROPERTIES IMPORTED_LOCATION ${SFML_DIR}/lib/libsfml-system.so)

add_library(sfml-window SHARED IMPORTED)
set_target_properties(sfml-window PROPERTIES IMPORTED_LOCATION ${SFML_DIR}/lib/libsfml-window.so)

add_library(sfml-graphics SHARED IMPORTED)
set_target_properties(sfml-graphics PROPERTIES IMPORTED_LOCATION ${SFML_DIR}/lib/libsfml-graphics.so)

add_library(sfml-activity SHARED IMPORTED)
set_target_properties(sfml-activity PROPERTIES IMPORTED_LOCATION ${SFML_DIR}/lib/libsfml-activity.so)

add_library(openal SHARED IMPORTED)
set_target_properties(openal PROPERTIES IMPORTED_LOCATION ${SFML_DIR}/lib/libopenal.so)

add_library(main SHARED
    src/main/cpp/main.cpp)

target_include_directories(main PRIVATE
    ${SFML_DIR}/include)

target_link_libraries(main
    sfml-graphics
    sfml-window
    sfml-system
    openal
    android
    log
    EGL
    GLESv1_CM)
