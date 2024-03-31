cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_COMPILER clang)

project (ltl)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
  ltl.cpp
  parse.cpp
  util.cpp
  main.cpp
  nnf.cpp
  numbering.cpp
  dynbitset.cpp
  vwaa.cpp
)

add_executable(main ${SOURCES})

target_compile_options(main
  PRIVATE
    -O2
    -Wall
    -Wextra
)

