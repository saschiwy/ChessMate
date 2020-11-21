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

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)
add_executable(ChessGui ${SOURCES})

target_link_libraries(ChessGui Qt5::Widgets)
target_link_libraries(ChessGui ChessEngine)
target_link_libraries(ChessGui ChessPlayer)

if (WIN32)
	set(QT_DIR ${Qt5_DIR}/../../../)
	if (CMAKE_BUILD_TYPE STREQUAL "Debug")
		set(QT_DEPL_OP --debug)
	else()
		set(QT_DEPL_OP --release)
	endif()

	add_custom_command(
		TARGET ChessGui POST_BUILD
		COMMAND ${QT_DIR}/bin/windeployqt.exe ${QT_DEPL_OP} --qmldir ${QT_DIR}/qml/ ${CMAKE_CURRENT_BINARY_DIR}/ChessGui.exe
	)
endif()

add_custom_command(
        TARGET ChessGui POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/res
                $<TARGET_FILE_DIR:ChessGui>/res
        )