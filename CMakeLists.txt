cmake_minimum_required(VERSION 3.16)

project(aizo-project1 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(aizo-project1 src/main.cpp
        src/main.h
        src/file_readers/file_reader.cpp
        src/file_readers/file_reader.h
)

target_compile_options(aizo-project1 PRIVATE
        -Wall
        -Wextra
        -Werror
)

