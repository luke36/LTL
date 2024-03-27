cmake_minimum_required(VERSION 3.5)

project (ltl)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# find_package(BISON)
# find_package(FLEX)

# BISON_TARGET(MyParser parser.y ${CMAKE_CURRENT_SOURCE_DIR}/parser.c)
# FLEX_TARGET(MyScanner lexer.l  ${CMAKE_CURRENT_SOURCE_DIR}/lexer.c)
# ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

set(SOURCES
  ltl.cpp
  parse.cpp
  util.cpp
  main.cpp
)

add_executable(test ${SOURCES})
add_executable(release ${SOURCES})
add_executable(debug ${SOURCES})

target_compile_options(test
  PRIVATE
    -g
    -Wall
)

target_compile_options(debug
  PRIVATE
    -Wall
    -fsanitize=address
    -ftrapv
    -g
)

target_compile_options(release
  PRIVATE
    -O3
)

target_link_libraries(debug
  PRIVATE
    -fsanitize=address
)
