/*!
* \brief:  Declares the figure class
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
#include <memory>
#include "ChessTypes.h"

namespace ChessNS
{
    class Board;

    /*!
     * \class   Figure
     *
     * \brief   A figure parent class.
     */
    class Figure
    {
    public:
        friend Board;

        virtual ~Figure() = default;

        Figure() = default;

        /*!
         * \fn  Figure::Figure(FigureType type, Color color, Position position);
         *
         * \brief   Constructor to create a valid figure
         *
         * \param   type        The type.
         * \param   color       The color.
         * \param   position    The position.
         */
        Figure(FigureType type, Color color, Position position);

        /*!
         * \fn  FigureType Figure::getType() const noexcept;
         *
         * \brief   Gets the type
         *
         * \returns The type.
         */
        FigureType getType() const noexcept;

        /*!
         * \fn  void Figure::setType(FigureType type) noexcept;
         *
         * \brief   Sets the type
         *
         * \param   type    The type.
         */
        void setType(FigureType type) noexcept;

        /*!
         * \fn  Color Figure::getColor() const noexcept;
         *
         * \brief   Gets the color
         *
         * \returns The color.
         */
        Color getColor() const noexcept;

        /*!
         * \fn  Position Figure::getCurrentPosition() const noexcept;
         *
         * \brief   Gets current position
         *
         * \returns The current position.
         */
        Position getCurrentPosition() const noexcept;

        /*!
         * \fn  Position Figure::getPreviousPosition() const noexcept;
         *
         * \brief   Gets the previous position
         *
         * \returns The previous position.
         */
        Position getPreviousPosition() const noexcept;

        /*!
         * \fn  Position Figure::getInitialPosition() const noexcept;
         *
         * \brief   Gets first position of the figure (where it was created)
         *
         * \returns The initial position.
         */
        Position getInitialPosition() const noexcept;

        /*!
         * \fn  bool Figure::isOpponent(const Figure& other) const noexcept;
         *
         * \brief   Query if the 'other' figure is an opponent of this
         *
         * \param   other   The other figure.
         *
         * \returns True if opponent, false if not.
         */
        bool isOpponent(const Figure& other) const noexcept;

        /*!
         * \fn  unsigned Figure::lastMoved() const noexcept;
         *
         * \brief   The last move count of the figure in a game
         *
         * \returns An unsigned.
         */
        unsigned lastMoved() const noexcept;

        /*!
         * \fn  unsigned Figure::nbrOfMovements() const noexcept;
         *
         * \brief   Number of movements in a game
         *
         * \returns The total number of movements.
         */
        unsigned nbrOfMovements() const noexcept;

        /*!
         * \fn  bool Figure::canJump() const noexcept;
         *
         * \brief   Determine if the figure can jump, only a knight can jump
         *
         * \returns True if the figure can jump, false if not.
         */
        bool canJump() const noexcept;

        /*!
         * \fn  virtual Movement Figure::move(const Position& destination, Board* board, bool execute = false);
         *
         * \brief   Moves
         *
         * \param           destination Destination to move to.
         * \param           board       the board, the figure shall move on.
         * \param           execute     (Optional) True to execute, false to evaluate only.
         *
         * \returns The movement with computed flags and MoveResult.
         */
        virtual Movement move(const Position& destination, Board* board, bool execute = false);

    protected:

        /*!
         * \fn  Movement Figure::moveInt(const Position& destination) const;
         *
         * \brief   Move internally, helper for the exposed methods
         *
         * \param   destination the destination.
         *
         * \returns A Movement.
         */
        Movement moveInt(const Position& destination) const;

        /*!
         * \fn  void Figure::executeMove(const Position& destination, Board* board);
         *
         * \brief   Executes the move operation on the board
         *
         * \param           destination Destination for the.
         * \param           board       The chess board.
         */
        void executeMove(const Position& destination, Board* board);

        /*!
         * \fn  bool Figure::isPathBlocked(const Position& position, Board* board) const;
         *
         * \brief   Determines if the path to position is blocked
         *
         * \param           position    The position.
         * \param           board       If non-null, the board.
         *
         * \returns True if path blocked, false if not.
         */
        bool isPathBlocked(const Position& position, Board* board) const;

        unsigned   _lastMoved        = 0;
        unsigned   _nbrOfMovements   = 0;
        Position   _previousPosition = Position(-1, -1);
        Position   _currentPosition  = Position(-1, -1);
        Position   _startPosition    = Position(-1, -1);
        Color      _color            = Color::none;
        FigureType _type             = FigureType::none;

    public:
    };

    /*!
     * \class   Pawn
     *
     * \brief   A pawn derived Figure.
     */
    class Pawn : public Figure
    {
    public:
        Movement move(const Position& destination, Board* board, bool execute) override;
    };

    /*!
     * \class   King
     *
     * \brief   The king derived Figure.
     */
    class King : public Figure
    {
    public:
        Movement move(const Position& destination, Board* board, bool execute) override;
    };

    /*!
     * \class   Knight
     *
     * \brief   A knight derived Figure.
     */
    class Knight : public Figure
    {
    public:
        Movement move(const Position& destination, Board* board, bool execute) override;
    };

    /*!
     * \class   Queen
     *
     * \brief   A queen derived Figure.
     */
    class Queen : public Figure
    {
    public:
        Movement move(const Position& destination, Board* board, bool execute) override;
    };

    /*!
     * \class   Bishop
     *
     * \brief   A bishop derived Figure.
     */
    class Bishop : public Figure
    {
    public:
        Movement move(const Position& destination, Board* board, bool execute) override;
    };

    /*!
     * \class   Rook
     *
     * \brief   A rook derived Figure.
     */
    class Rook : public Figure
    {
    public:
        friend King;

        Movement move(const Position& destination, Board* board, bool execute) override;
    };
}
