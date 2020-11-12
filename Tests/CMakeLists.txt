cmake_minimum_required (VERSION 3.8)
project (TestChessMate VERSION ${CHESS_MATE_VERSION} LANGUAGES CXX)

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)
add_executable(TestChessMate ${SOURCES})

target_link_libraries(TestChessMate ChessEngine)
target_link_libraries(TestChessMate gtest gtest_main)