/*!
* \brief:  Declares the pgn parser class
*
* The MIT License (MIT)
*
* Copyright (c) 2020 Sascha Schiwy. All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
 */

#pragma once
#include <iostream>
#include <map>
#include <vector>

#include "ChessEngine/ChessTypes.h"

namespace ChessNS
{
    /*!
     * \class   PgnParser
     *
     * \brief   A pgn syntax parser.
     */
    class PgnParser
    {
    public:

        /*!
         * \fn  Game PgnParser::parseSingleGame(std::istream& in);
         *
         * \brief   Parse single game from file
         *
         * \param [in,out]  in  The in.
         *
         * \returns A Game.
         */
        Game parseSingleGame(std::istream& in);

        /*!
         * \fn  Game PgnParser::parseSingleGame(const std::string& filename);
         *
         * \brief   Parse single game from file
         *
         * \param   filename    Filename of the file.
         *
         * \returns A Game.
         */
        Game parseSingleGame(const std::string& filename);

        /*!
         * \fn  std::vector<Game> PgnParser::parseMultipleGames(std::istream& in);
         *
         * \brief   Parse multiple games in one file
         *
         * \param [in,out]  in  The in.
         *
         * \returns A std::vector<Game>;
         */
        std::vector<Game> parseMultipleGames(std::istream& in);

    private:

        /*!
         * \fn  Movement PgnParser::parseMovement(std::string& move, unsigned roundCounter, unsigned moveCounter, Color color);
         *
         * \brief   Parse a single movement
         *
         * \param [in,out]  move            The move.
         * \param           roundCounter    The round counter.
         * \param           moveCounter     The move counter.
         * \param           color           The color.
         *
         * \returns A Movement.
         */
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
