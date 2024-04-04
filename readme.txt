Requires CMake version 3.8 or higher, and a C++ compiler supporting C++17 or
higher.

Build and test (for example) by:
  $ mkdir build
  $ cd build
  $ cmake ..
  $ make -j4
  $ ./main < ../test/final1.txt

Please input in LF format.

Action names and atom proposition names should contain only lower letters,
underlines, and hyphens.

The LTL-to-BA algorithm is almost the same as ltl2ba, or the paper "Fast LTL to
Büchi Automata Translation". But we do not implement the optimization using
SCCs to relax the equality condition.
