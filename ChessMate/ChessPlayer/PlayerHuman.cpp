/*!
* \brief:  Implements the player human class
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
#include "PlayerHuman.h"

namespace ChessNS
{
    Movement PlayerHuman::move(const Movement& move)
    {
        if (!_board)
            return Movement::invalid();

        auto result = _board->move(move);

        if (result.isValid())
            _lastValidMovement = result;

        return result;
    }

    Movement PlayerHuman::move(const Position& origin, const Position& destination)
    {
        if (!_board)
            return Movement::invalid();

        auto result = _board->move(origin, destination);

        if (result.isValid())
            _lastValidMovement = result;

        return result;
    }

    bool PlayerHuman::changePromotedPawn(FigureType toType)
    {
        if (_lastValidMovement.isValid() && _lastValidMovement.hasFlag(EventFlag::promotion))
        {
            _lastValidMovement.promotedTo() = toType;
            _board->at(_lastValidMovement.destination()).figure.setType(toType);
            return true;
        }

        return false;
    }

    void PlayerHuman::setBoard(const std::shared_ptr<Board>& board)
    {
        _board = board;
    }
}
