/*!
* \brief:  Implements the player simple ai class
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

#include "PlayerSimpleAi.h"

namespace ChessNS
{
    PlayerSimpleAi::PlayerSimpleAi()
    {
        std::random_device rd;
        _gen = std::mt19937(rd());
    }

    Movement PlayerSimpleAi::move(const Movement&)
    {
        return autoMove();
    }

    Movement PlayerSimpleAi::move(const Position& origin, const Position& destination)
    {
        return autoMove();
    }

    bool PlayerSimpleAi::changePromotedPawn(FigureType toType)
    {
        if (!_board)
            return false;

        _board->at(_lastValidMovement.destination()).figure.setType(toType);
        return true;
    }

    void PlayerSimpleAi::setBoard(const std::shared_ptr<Board>& board)
    {
        _board = board;
    }

    Movement PlayerSimpleAi::autoMove()
    {
        if (!_board)
            return Movement::invalid();

        Movement res;
        auto     movements = _board->getAllPossibleMoves(_playerColor);

        for (auto&& movement : movements)
            if (movement.hasFlag(EventFlag::capture))
                res = movement;

        for (auto&& movement : movements)
            if (movement.hasFlag(EventFlag::promotion))
                res = movement;

        for (auto&& movement : movements)
            if (movement.hasFlag(EventFlag::check))
                res = movement;

        for (auto&& movement : movements)
            if (movement.hasFlag(EventFlag::checkmate))
                res = movement;

        if (!res.isValid() && !movements.empty())
        {
            std::uniform_int_distribution<> dist(0, static_cast<int>(movements.size() - 1));
            res = movements.at(dist(_gen));
        }

        if (res.isValid())
        {
            res                = _board->move(res);
            _lastValidMovement = res;
            if (_lastValidMovement.hasFlag(EventFlag::promotion))
                changePromotedPawn(FigureType::queen);
        }
        return res;
    }
}
