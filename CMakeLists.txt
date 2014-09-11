cmake_minimum_required(VERSION 2.8.4)
project(ClipboardStack)

set(SOURCE_FILES main.cpp X11ClipboardAdapter.cpp PastePopListener.cpp QuitListener.cpp ClipboardStack.cpp)

link_libraries(X11)

add_executable(ClipboardStack ${SOURCE_FILES})

SET(CMAKE_CXX_FLAGS "-std=c++0x")