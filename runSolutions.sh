#!/bin/bash

# Running pieces-moves related problems.

echo "Compiling bishopMoves.cpp"
g++ bishopMoves.cpp
echo "Executing bishopMoves.cpp (both in console and writing in bishopMoves.out file)"
./a.out< bishopMoves.in
./a.out< bishopMoves.in > bishopMoves.out

echo "Compiling knightMoves.cpp"
g++ knightMoves.cpp
echo "Executing knightMoves.cpp (both in console and writing in knightMoves.out file)"
./a.out< knightMoves.in
./a.out< knightMoves.in > knightMoves.out

echo "Compiling pawnMoves.cpp"
g++ pawnMoves.cpp
echo "Executing pawnMoves.cpp (both in console and writing in pawnMoves.out file)"
./a.out< pawnMoves.in
./a.out< pawnMoves.in > pawnMoves.out

echo "Compiling rookMoves.cpp"
g++ rookMoves.cpp
echo "Executing rookMoves.cpp (both in console and writing in rookMoves.out file)"
./a.out< rookMoves.in
./a.out< rookMoves.in > rookMoves.out

# Running check-mate related problems.

echo "Compiling mateInOne.cpp"
g++ mateInOne.cpp
echo "Executing mateInOne.cpp (both in console and writing in mateInOne.out file)"
./a.out< mateInOne.in
./a.out< mateInOne.in > mateInOne.out

echo "Compiling mateInTwo.cpp"
g++ mateInTwo.cpp
echo "Executing mateInTwo.cpp (both in console and writing in mateInTwo.out file)"
./a.out< mateInTwo.in
./a.out< mateInTwo.in > mateInTwo.out

echo "Compiling checkingMoves.cpp"
g++ checkingMoves.cpp
echo "Executing checkingMoves.cpp (both in console and writing in checkingMoves.out file)"
./a.out< checkingMoves.in
./a.out< checkingMoves.in > checkingMoves.out

echo "Compiling pseudoLegalMoves.cpp"
g++ pseudoLegalMoves.cpp
echo "Executing pseudoLegalMoves.cpp (both in console and writing in pseudoLegalMoves.out file)"
./a.out< pseudoLegalMoves.in
./a.out< pseudoLegalMoves.in > pseudoLegalMoves.out

# Running miscellaneous.

echo "Compiling findCaptures.cpp"
g++ findCaptures.cpp
echo "Executing findCaptures.cpp (both in console and writing in findCaptures.out file)"
./a.out< findCaptures.in
./a.out< findCaptures.in > findCaptures.out
