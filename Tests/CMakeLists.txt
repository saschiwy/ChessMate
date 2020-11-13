cmake_minimum_required (VERSION 3.8)
project (TestChessMate VERSION ${CHESS_MATE_VERSION} LANGUAGES CXX)

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)
add_executable(TestChessMate ${SOURCES})

target_link_libraries(TestChessMate ChessEngine)
target_link_libraries(TestChessMate ChessParser)
target_link_libraries(TestChessMate gtest gtest_main)


add_custom_command(
        TARGET TestChessMate POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/data/pgn_examples
                $<TARGET_FILE_DIR:TestChessMate>/pgn_examples
        )