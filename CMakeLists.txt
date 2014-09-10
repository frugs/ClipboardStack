cmake_minimum_required(VERSION 2.8.4)
project(ClipboardStack)

set(SOURCE_FILES main.cpp ClipboardStack.cpp PastePopListener.cpp QuitListener.cpp)

link_libraries(X11)

add_executable(ClipboardStack ${SOURCE_FILES})

SET(CMAKE_CXX_FLAGS "-std=c++0x")