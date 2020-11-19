/*!
* \brief:  Implements the chess mate console based game
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

#include "ChessMate.h"

int main()
{
    auto                             board = std::make_shared<ChessNS::Board>();
    auto                             white = ChessNS::IPlayer::createPlayer(ChessNS::PlayerType::human, ChessNS::Color::white, board);
    auto                             black = ChessNS::IPlayer::createPlayer(ChessNS::PlayerType::simpleAi, ChessNS::Color::black, board);
    std::string                      input;
    std::array<ChessNS::Position, 2> positions;
    ChessNS::Movement                movement;

    while (!board->hasEnded())
    {
        do
        {
            do
            {
                std::cout << "Enter a valid movement (syntax: a2-a3): ";
                std::cin >> input;
                positions = parseInputDouble(input);
            }
            while (!positions[0].isValid() || !positions[1].isValid());

            movement = white->move(positions[0], positions[1]);
        }
        while (movement.moveResult() == ChessNS::MoveResult::invalid);
        std::cout << printFlags(movement) << std::endl;

        if (movement.hasFlag(ChessNS::EventFlag::promotion))
        {
            std::cout << "Pawn can be promoted, choose (Q, R, B, N): ";
            std::cin >> input;

            // todo: Finish
        }

        if (board->hasEnded())
            break;

        std::cout << "Computer reacts with: ... ";
        movement = black->move(movement);
        std::cout << movement.origin().toString() << "-" << movement.destination().toString() << " " << printFlags(movement) << std::endl;

        if (movement.hasFlag(ChessNS::EventFlag::promotion))
            std::cout << "Promoted to: " << ChessNS::toString(movement.promotedTo()) << std::endl;
    }

    std::cout << "Press Enter to quit!" << std::endl;
    std::cin >> input;

    return 0;
}

int getRow(char r)
{
    return static_cast<int>(r - '1');
}

int getColumn(char c)
{
    c = static_cast<char>(std::tolower(c));
    return static_cast<int>(c - 'a');
}

ChessNS::Position parseInputSingle(const std::string& input)
{
    ChessNS::Position result;

    if (input.length() == 2)
        result.set(getRow(input[1]), getColumn(input[0]));

    return result;
}

std::array<ChessNS::Position, 2> parseInputDouble(const std::string& input)
{
    std::array<ChessNS::Position, 2> result;

    if (input.length() != 5)
        return result;

    auto strings = ChessNS::StringHelper::split(input, '-');
    if (strings.size() != 2)
        return result;

    result[0] = parseInputSingle(strings[0]);
    result[1] = parseInputSingle(strings[1]);
    return result;
}

std::string printFlags(ChessNS::Movement& movement)
{
    std::string result;
    for (size_t i = 0; i < 5; i++)
    {
        const auto e = static_cast<ChessNS::EventFlag>(i);
        if (movement.hasFlag(e))
            result += " (" + ChessNS::toString(e) + ")";
    }

    return result;
}
