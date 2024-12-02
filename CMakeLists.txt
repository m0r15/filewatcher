cmake_minimum_required(VERSION 3.20)
project(filewatcher
        VERSION 0.1.0
        DESCRIPTION "Files control"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

set(HEADERS
        src/FileSystemEvent.h
        src/SignalManager.h
        src/Watcher.h
)

set(SOURCES
        src/FileSystemEvent.cpp
        src/SignalManager.cpp
        src/Watcher.cpp
        src/main.cpp
)

configure_file(${CMAKE_SOURCE_DIR}/test/res/files
        ${CMAKE_CURRENT_BINARY_DIR}/files
        COPYONLY
)

add_executable(${CMAKE_PROJECT_NAME}
        ${HEADERS}
        ${SOURCES}
)

# BS - thread pool https://github.com/bshoshany/thread-pool/tree/master
target_include_directories(${CMAKE_PROJECT_NAME}
        PRIVATE include
)