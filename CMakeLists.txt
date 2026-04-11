cmake_minimum_required(VERSION 3.16)

project(aizo-project1 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(aizo-project1 src/main.cpp
        src/main.h
        src/include/io/FileLoader.h
        src/include/structures/DynamicArray.h
        src/include/structures/SinglyLinkedList.h
        src/include/structures/DoublyLinkedList.h
        src/include/sorting/QuickSorter.h
        src/include/utils/Validator.h
)

target_include_directories(aizo-project1 PUBLIC src src/include)

target_link_libraries(aizo-project1 ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/libparameters.so)

target_compile_options(aizo-project1 PRIVATE
        -Wall
        -Wextra
        -Werror
)

