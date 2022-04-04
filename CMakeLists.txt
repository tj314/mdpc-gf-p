cmake_minimum_required(VERSION 3.20)
project(mdpc-gf-p)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -pedantic -g)

add_executable(mdpc-gf-p
        ${CMAKE_SOURCE_DIR}/src/main.cpp
        ${CMAKE_SOURCE_DIR}/src/code.cpp
        ${CMAKE_SOURCE_DIR}/src/protocol.cpp
        ${CMAKE_SOURCE_DIR}/src/random.cpp)

target_include_directories(mdpc-gf-p PRIVATE /usr/include/flint)

target_include_directories(mdpc-gf-p PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_options(mdpc-gf-p PRIVATE -lflint -lgmp -lm)