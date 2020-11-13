cmake_minimum_required (VERSION 3.8)
project (ChessParser VERSION ${CHESS_MATE_VERSION} LANGUAGES CXX)
file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)

add_library(ChessParser ${SOURCES})
target_link_libraries (ChessParser ChessEngine)