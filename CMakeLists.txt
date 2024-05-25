cmake_minimum_required(VERSION 3.8)

project (ltl)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
  ltl.cpp
  parse.cpp
  main.cpp
  nnf.cpp
  vwaa.cpp
  gba.cpp
  buchi.cpp
  ts.cpp
  verify.cpp
)

add_executable(main ${SOURCES})

target_compile_options(main
  PRIVATE
    -g
    -O2
    # -flto
    -Wall
    -Wextra
)

