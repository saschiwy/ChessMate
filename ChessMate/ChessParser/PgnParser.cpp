/*!
* \brief:  Implements the pgn parser class
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

#include "PgnParser.h"
#include "BasicUtils/StringHelper.h"
#include <fstream>
#include <string>

namespace ChessNS
{
    Game PgnParser::parseSingleGame(std::istream& in)
    {
        bool        run         = true;
        bool        foundStart  = false;
        bool        inComment   = false;
        unsigned    moveCounter = 1;
        std::string line;
        Game        result;

        while (run && std::getline(in, line))
        {
            if (!foundStart)
            {
                if (line.substr(0, 2) != "1.")
                    continue;

                foundStart = true;
            }

            auto splittedLine = StringHelper::split(line, ' ');
            for (auto it = splittedLine.begin(); it != splittedLine.end(); ++it)
            {
                if (it->empty())
                    continue;

                // end of game reached
                if (!inComment && _possibleResults.find(*it) != _possibleResults.end())
                {
                    result.result = _possibleResults.at(*it);
                    run           = false;
                    break;
                }

                // Comment
                if (it->substr(0, 1) == "{" && !inComment)
                    inComment = true;

                if (inComment)
                {
                    if (it->find("}") != std::string::npos)
                        inComment = false;
                    continue;
                }

                // round Counter
                const auto point = it->find(".");
                if (point != std::string::npos)
                {
                    if (it->length() == point + 1)
                        continue;

                    *it = it->substr(point + 1);
                }

                result.movements.emplace_back(parseMovement(*it, moveCounter / 2 + 1, moveCounter, moveCounter % 2 == 0 ? Color::black : Color::white));
                moveCounter++;
            }
        }

        return result;
    }

    Game PgnParser::parseSingleGame(const std::string& filename)
    {
        std::ifstream in(filename);
        if (!in.good())
            return Game();

        return parseSingleGame(in);
    }

    std::vector<Game> PgnParser::parseMultipleGames(std::istream& in)
    {
        std::vector<Game> result;
        while (!in.eof())
        {
            auto game = parseSingleGame(in);
            if (game.result != GameResult::none)
                result.insert(result.end(), game);
        }

        return result;
    }

    Movement PgnParser::parseMovement(std::string& move, unsigned roundCounter, unsigned moveCounter, Color color)
    {
        Movement movement;
        movement.moveResult() = MoveResult::valid;
        movement.color()      = color;
        movement.round()      = roundCounter;

        // Castling
        if (move == "O-O")
        {
            movement.figureType()  = FigureType::king;
            movement.destination() = color == Color::white ? Position(BoardRow::r1, BoardColumn::cG) : Position(BoardRow::r8, BoardColumn::cG);
            movement.addFlag(EventFlag::castling);
            if (move.length() == 3)
                return movement;

            move = move.substr(3);
        }

        else if (move == "O-O-O")
        {
            movement.figureType()  = FigureType::king;
            movement.destination() = color == Color::white ? Position(BoardRow::r1, BoardColumn::cC) : Position(BoardRow::r8, BoardColumn::cC);
            movement.addFlag(EventFlag::castling);
            if (move.length() == 5)
                return movement;
            move = move.substr(5);
        }

        else
        {
            // type
            if (_figureTypes.find(move[0]) != _figureTypes.end())
            {
                movement.figureType() = _figureTypes.at(move[0]);
                move                  = move.substr(1);
            }
            else
                movement.figureType() = FigureType::pawn;
        }

        // capture
        const auto x = move.find('x');
        if (x != std::string::npos)
            movement.addFlag(EventFlag::capture);

        // check is not interesting for movements here
        if (move.back() == '+')
        {
            movement.addFlag(EventFlag::check);
            move = move.substr(0, move.length() - 1);
        }

        // Checkmate is also
        if (move.back() == '#')
        {
            movement.addFlag(EventFlag::checkmate);
            move = move.substr(0, move.length() - 1);
        }

        // promotion
        if (movement.figureType() == FigureType::pawn && _figureTypes.find(move.back()) != _figureTypes.end())
        {
            movement.addFlag(EventFlag::promotion);
            movement.promotedTo() = _figureTypes.at(move.back());
            move                  = move.substr(0, move.length() - 2);
        }

        // Last two both letters are the destination now
        if (!movement.hasFlag(EventFlag::castling))
        {
            movement.destination().set(_rowsByLetter.at(move.at(move.length() - 1)),
                                       _columnsByLetter.at(move.at(move.length() - 2)));

            move = move.substr(0, move.length() - 2);

            if (x != std::string::npos)
                move = move.substr(0, move.length() - 1);

            // If something is left, it's a origin clarification
            if (move.length() == 2)
                movement.origin().set(_rowsByLetter.at(move[0]), _columnsByLetter.at(move[1]));

            else if (move.length() == 1)
            {
                if (_rowsByLetter.find(move[0]) != _rowsByLetter.end())
                    movement.origin().set(_rowsByLetter.at(move[0]), -1);
                else
                    movement.origin().set(-1, _columnsByLetter.at(move[0]));
            }
        }

        return movement;
    }
}
