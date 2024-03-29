cmake_minimum_required(VERSION 3.5)

project (ltl)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
  ltl.cpp
  parse.cpp
  util.cpp
  main.cpp
  nnf.cpp
  numbering.cpp
)

add_executable(main ${SOURCES})

target_compile_options(main
  PRIVATE
    -O2
    -Wall
    -Wextra
)

