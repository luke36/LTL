Requires CMake version 3.8 or higher, and a C++ compiler supporting C++17 or
higher.

Build and test (for example) by:
  $ mkdir build
  $ cd build
  $ cmake ..
  $ make -j4
  $ ./main < ../test/final1.txt

Or if the transition system and the formulae are in separate files:
  $ ./main --ts="TS.txt" --formula="benchmark.txt" --output="result.txt"

Please input in LF format.

Action names and atom proposition names should contain only lower letters,
underlines, and hyphens.

The LTL-to-BA algorithm is almost the same as ltl2ba, or the 2001 paper "Fast
LTL to Büchi Automata Translation". But we do not implement the optimization
using SCCs to relax the equality condition.

ltl.hpp : AST of LTL formulae.
ltl.cpp : Translate LTL formulae to negation normal form.
parse.cpp, parse.hpp : Parse LTL formulae.
nnf.hpp : AST of LTL formulae in negation normal form.
nnf.cpp : Translate LTL formulae in negation normal form to equivalent very weak
          alternating automata.
numbering.hpp : Translate between atom proposition names and
                 action names and their (integer) identifier.
dynbitset.hpp : Bitset with dynamic sizes.
vwaa.hpp, vwaa.cpp : Very weak alternating automate.
gba.hpp : Generalize (nondeterministic) Büchi Automata. The definition differs
          from the textbook by using final transitions instead of final states.
gba.cpp : Translation from VWAA to GBA.
buchi.hpp : (Nondeterministic) Büchi Automata.
buchi.cpp : Translation from GBA to BA.
ts.hpp, ts.cpp : Transition systems.
verify.cpp, verify.hpp : Decide whether the product transition system is
                         empty. The algorithm is a direct copy of "Principles of
                         Model Checking", page 210-211. Note that we do not
                         create the product explicitly.
main.cpp : Entry.
