/*!
* \brief:  Declares the board class
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

#include "BasicUtils/Matrix.h"
#include "ChessTypes.h"
#include "Figure.h"

namespace ChessNS
{
    /*!
     * \struct  Field
     *
     * \brief   A field on the chess board.
     */
    struct Field
    {
        /*! \brief   Is the field empty */
        bool empty{true};
        /*! \brief   The position of the field*/
        Position position{};
        /*! \brief   The figure placed on the field */
        Figure figure{};

        Field() = default;
    };

    #ifdef BUILD_TESTS
    class TestBoard;
    class TestBoardRelationalMove;
    class TestBoardDiagonalMove;
    class TestBoardRankFieldMove;
    #endif

    /*!
     * \class   Board
     *
     * \brief   A chess board class which is a 8x8 Matrix.
     */
    class Board : public Matrix<Field>
    {
    public:

        #ifdef BUILD_TESTS
        friend TestBoard;
        friend TestBoardRelationalMove;
        friend TestBoardDiagonalMove;
        friend TestBoardRankFieldMove;
        #endif

        /*!
         * \enum    BoardStartType
         *
         * \brief   Values that represent the start types of the board, standard means the usual positions of all figures
         */
        enum class BoardStartType { empty, standard };

        Board();

        /*!
         * \fn  Field& Board::at(Position position);
         *
         * \brief   Gets the field at a given position
         *
         * \param   position    The position.
         *
         * \returns A reference to the Field.
         */
        Field& at(Position position);

        /*!
         * \fn  Field& Board::at(BoardRow row, BoardColumn column);
         *
         * \brief   Gets the field at a given position
         *
         * \param   row     The row.
         * \param   column  The column.
         *
         * \returns A reference to a Field.
         */
        Field& at(BoardRow row, BoardColumn column);

        /*!
         * \fn  Movement Board::allowed(Position origin, Position destination) const;
         *
         * \brief   Determines if a movement is allowed and simulates the move on the current board
         *          with respect to all other figures and chess rules like chess, valid ...
         *
         * \param   origin      The origin of a move.
         * \param   destination Destination for the move.
         *
         * \returns The result the move would have.
         */
        Movement allowed(Position origin, Position destination) const;

        /*!
         * \fn  Movement Board::allowed(Movement movement) const;
         *
         * \brief   Determines if a movement is allowed and simulates the move on the current board
         *          with respect to all other figures and chess rules like chess, valid ...
         *
         * \param   movement    A movement with the destination and if needed information about the origin.
         *                      Here also the figure type is required. This method is used for PGN parsed moves
         *                      which usually have no information about the origin.
         *
         * \returns The result the move would have.
         */
        Movement allowed(Movement movement) const;

        /*!
         * \fn  Movement Board::move(Position origin, Position destination);
         *
         * \brief   Determines if a movement is allowed and the also makes the move on the current board
         *          with respect to all other figures and chess rules like chess, valid ...
         *
         * \param   origin      The origin.
         * \param   destination Destination for the.
         *
         * \returns The result of the move.
         */
        Movement move(Position origin, Position destination);

        /*!
         * \fn  Movement Board::move(Movement movement);
         *
         * \brief   Determines if a movement is allowed and simulates the move on the current board
         *          with respect to all other figures and chess rules like chess, valid ...
         *
         * \param   movement    A movement with the destination and if needed information about the origin.
         *                      Here also the figure type is required. This method is used for PGN parsed moves
         *                      which usually have no information about the origin.
         *
         * \returns The result of the move
         */
        Movement move(Movement movement);

        /*!
         * \fn  bool Board::changeFigureType(const Position& position, FigureType figure);
         *
         * \brief   Change the figure type on a field
         *
         * \param   position    The position of the figure.
         * \param   figure      The new type.
         *
         * \returns True if it succeeds, false empty.
         */
        bool changeFigureType(const Position& position, FigureType figure);

        /*!
         * \fn  bool Board::isCheck(Color color);
         *
         * \brief   Query if 'color' is in check
         *
         * \param   color   The color.
         *
         * \returns True if check, false if not.
         */
        bool isCheck(Color color);

        /*!
         * \fn  GameResult Board::checkVictory();
         *
         * \brief   Checks victory of both colors
         *
         * \returns The GameResult.
         */
        GameResult checkVictory();

        /*!
         * \fn  GameResult Board::checkVictory(Color againstColor);
         *
         * \brief   Check victory
         *
         * \param   againstColor    The color which is checkmate.
         *
         * \returns The GameResult.
         */
        GameResult checkVictory(Color againstColor);

        /*!
         * \fn  bool Board::hasEnded() const;
         *
         * \brief   Query if the game has ended
         *
         * \returns True if ended, false if not.
         */
        bool hasEnded() const;

        /*!
         * \fn  std::vector<Movement> Board::getAllPossibleMoves(Position origin);
         *
         * \brief   Gets all possible moves of a figure on a field
         *
         * \param   origin  The field of the figure.
         *
         * \returns all possible moves.
         */
        std::vector<Movement> getAllPossibleMoves(Position origin);

        /*!
         * \fn  std::vector<Movement> Board::getAllPossibleMoves(Color ofColor);
         *
         * \brief   Gets all possible moves a color can make now
         *
         * \param   ofColor The color which shall make the moves.
         *
         * \returns all possible moves.
         */
        std::vector<Movement> getAllPossibleMoves(Color ofColor);

        /*!
         * \fn  std::vector<Movement> Board::getAllMadeMoves() const;
         *
         * \brief   Gets all made moves until now
         *
         * \returns all made moves.
         */
        std::vector<Movement> getAllMadeMoves() const;

        /*!
         * \fn  unsigned Board::currentMove() const;
         *
         * \brief   Current move counter
         *
         * \returns The move counter.
         */
        unsigned currentMove() const;

        /*!
         * \fn  bool Board::isFieldUnderAttack(Field& field, Color byColor);
         *
         * \brief   Query if 'field' is field under attack and if a king can move there
         *
         * \param           field   The field.
         * \param           byColor The by color.
         *
         * \returns True if field under attack, false if not.
         */
        bool isFieldUnderAttack(Field& field, Color byColor);

        /*!
         * \fn  Color Board::getCurrentColorTurn() const;
         *
         * \brief   Gets current color turn
         *
         * \returns The current color turn.
         */
        Color getCurrentColorTurn() const;

    private:

        explicit Board(BoardStartType boardStart);

        Movement move(Position origin, Position destination, bool checkVictory);

        Movement allowed(Position origin, Position destination, bool checkVictory) const;

        static void createFigure(Field& field, FigureType figure, Color color);

        std::vector<Field*> getAllOccupiedFields(Color ofColor);

        Field* getFigure(Color byColor, FigureType figureType);

        unsigned              _currentMove{1};
        Color                 _currentColorTurn{Color::white};
        bool                  _ended{false};
        std::vector<Movement> _movements;
    };
}
