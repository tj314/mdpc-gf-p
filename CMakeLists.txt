cmake_minimum_required(VERSION 3.16)
project(mceliece_qcmdpc_gf_p)

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()


find_library(NTL_LIB ntl)
if(NOT NTL_LIB)
    message(FATAL_ERROR "ntl library not found!")
endif()


add_executable(mceliece_qcmdpc_gf_p
  ${CMAKE_SOURCE_DIR}/main.cpp
  ${CMAKE_SOURCE_DIR}/src/code/code.cpp
  ${CMAKE_SOURCE_DIR}/src/protocol/protocol.cpp
)

target_include_directories(mceliece_qcmdpc_gf_p PRIVATE
  ${CMAKE_SOURCE_DIR}/src/code
  ${CMAKE_SOURCE_DIR}/src/protocol
)

target_link_libraries(mceliece_qcmdpc_gf_p ${NTL_LIB})
