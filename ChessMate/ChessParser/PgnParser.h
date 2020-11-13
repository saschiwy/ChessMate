#pragma once
#include <iostream>
#include <map>
#include <vector>

#include "ChessEngine/ChessTypes.h"

namespace ChessNS
{
    class PgnParser
    {
    public:
        Game parseSingleGame(std::istream& in);

        Game parseSingleGame(const std::string& filename);

        std::vector<Game> parseMultipleGames(std::istream& in);

    private:

        Movement parseMovement(std::string& move, unsigned roundCounter, unsigned moveCounter, Color color);

        const std::map<std::string, GameResult> _possibleResults = {
            {"1/2-1/2", GameResult::draw},
            {"1-0", GameResult::victoryWhite},
            {"0-1", GameResult::victoryBlack}
        };

        const std::map<char, FigureType> _figureTypes = {
            {'K', FigureType::king},
            {'Q', FigureType::queen},
            {'B', FigureType::bishop},
            {'N', FigureType::knight},
            {'R', FigureType::rook}
        };

        const std::map<char, int> _columnsByLetter = {
            {'a', 0},
            {'b', 1},
            {'c', 2},
            {'d', 3},
            {'e', 4},
            {'f', 5},
            {'g', 6},
            {'h', 7},
        };

        const std::map<char, int> _rowsByLetter = {
            {'1', 0},
            {'2', 1},
            {'3', 2},
            {'4', 3},
            {'5', 4},
            {'6', 5},
            {'7', 6},
            {'8', 7},
        };
    };
}
