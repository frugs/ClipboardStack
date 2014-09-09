cmake_minimum_required(VERSION 2.8.4)
project(ClipboardStack)

set(SOURCE_FILES main.cpp)

link_libraries(X11)

add_executable(ClipboardStack ${SOURCE_FILES})