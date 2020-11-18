#pragma once

#include <iostream>
#include <array>
#include "ChessEngine/Board.h"
#include "ChessEngine/Figure.h"
#include "ChessPlayer/IPlayer.h"
#include "BasicUtils/StringHelper.h"

int getRow(char r);

int getColumn(char c);

ChessNS::Position parseInputSingle(const std::string& input);

std::array<ChessNS::Position, 2> parseInputDouble(const std::string& input);

std::string printFlags(ChessNS::Movement& movement);
