# The MIT License (MIT)
#
# Copyright (c) 2020 Sascha Schiwy. All rights reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required (VERSION 3.8)

project ("ChessMate")

option(BUILD_TESTS "Build test programs" ON)
option(BUILD_DOC "Build documentation" OFF)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CHESS_MATE_VERSION 0.0.1)

if (BUILD_TESTS)
    configure_file(libs/gtest.txt.in googletest-download/CMakeLists.txt)

    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download
    )
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download
    )
    
    # Prevent GoogleTest from overriding our compiler/linker options
    # when building with Visual Studio
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    
    # Add googletest directly to our build. This adds the following targets:
    # gtest, gtest_main, gmock and gmock_main
    add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                     ${CMAKE_BINARY_DIR}/googletest-build
    )
    
    include_directories(${CMAKE_BINARY_DIR}/googletest-src/googletest/include)
    include_directories(${CMAKE_BINARY_DIR}/googletest-src/googletest/include/gtest)
    add_definitions(-DBUILD_TESTS)
endif()

include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)

include_directories(ChessMate)

# Include sub-projects.
add_subdirectory ("ChessMate")

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif()

if (BUILD_DOC)
    add_subdirectory(doc)
endif()
