# CMakeList.txt : CMake project for ChessMate, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

add_subdirectory(ChessEngine)
add_subdirectory(ChessParser)

# Add source to this project's executable.
add_executable (ChessMate "ChessMate.cpp" "ChessMate.h" "BasicUtils/StringHelper.h")
