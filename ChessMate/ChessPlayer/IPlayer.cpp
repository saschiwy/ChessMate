/*!
* \brief:  Declares the IPlayer interface
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
#include "IPlayer.h"
#include "PlayerHuman.h"
#include "PlayerSimpleAi.h"
#include <memory>

namespace ChessNS
{
    std::unique_ptr<IPlayer> IPlayer::createPlayer(PlayerType type, Color color, const std::shared_ptr<Board>& board)
    {
        std::unique_ptr<IPlayer> result;
        switch (type)
        {
            case PlayerType::human: result = std::make_unique<PlayerHuman>();
                break;
            case PlayerType::simpleAi: result = std::make_unique<PlayerSimpleAi>();
                break;
            default: return nullptr;
        }

        result->_playerColor = color;
        result->_playerType  = type;
        result->_board       = board;

        return std::move(result);
    }

    PlayerType IPlayer::getPlayerType() const { return _playerType; }

    Color IPlayer::getColor() const { return _playerColor; }
}
