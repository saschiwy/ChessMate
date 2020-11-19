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
#pragma once
#include "ChessEngine/ChessTypes.h"
#include "ChessEngine/Board.h"

namespace ChessNS
{
    /*!
     * \enum    PlayerType
     *
     * \brief   Values that represent player types
     */
    enum class PlayerType { human, simpleAi };

    /*!
     * \class   IPlayer
     *
     * \brief   A player interface.
     */
    class IPlayer
    {
    public:

        /*!
         * \fn  static std::unique_ptr<IPlayer> IPlayer::createPlayer(PlayerType type, Color color, const std::shared_ptr<Board>& board);
         *
         * \brief   Creates a player
         *
         * \param   type    The type.
         * \param   color   The color.
         * \param   board   The board.
         *
         * \returns The new player.
         */
        static std::unique_ptr<IPlayer> createPlayer(PlayerType type, Color color, const std::shared_ptr<Board>& board);

        /*!
         * \fn  virtual Movement IPlayer::move(const Movement& move) = 0;
         *
         * \brief   Moves the given movement
         *
         * \param   move    The move.
         *
         * \returns The movement with computed flags and MoveResult.
         */
        virtual Movement move(const Movement& move) = 0;

        /*!
         * \fn  virtual Movement IPlayer::move(const Position& origin, const Position& destination) = 0;
         *
         * \brief   Moves a figure from origin to destination
         *
         * \param   origin      The origin.
         * \param   destination Destination for the.
         *
         * \returns The movement with computed flags and MoveResult.
         */
        virtual Movement move(const Position& origin, const Position& destination) = 0;

        /*!
         * \fn  virtual bool IPlayer::changePromotedPawn(FigureType toType) = 0;
         *
         * \brief   Change a promoted pawn immediately after a ChessNS::Movement with ChessNS::EventType::promotion
         *
         * \param   toType  the ChessNS::FigureType it shall become.
         *
         * \returns True if it succeeds, false if it fails.
         */
        virtual bool changePromotedPawn(FigureType toType) = 0;

        /*!
         * \fn  PlayerType IPlayer::getPlayerType() const;
         *
         * \brief   Gets player type
         *
         * \returns The player type.
         */
        PlayerType getPlayerType() const;

        /*!
         * \fn  Color IPlayer::getColor() const;
         *
         * \brief   Gets the player color
         *
         * \returns The color.
         */
        Color getColor() const;

    protected:
        std::shared_ptr<Board> _board;
        PlayerType             _playerType{};
        Color                  _playerColor{};
    };
}
