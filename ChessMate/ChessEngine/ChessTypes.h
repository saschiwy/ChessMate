/*!
* \brief:  Declares the chess types
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
#include <string>
#include <utility>
#include <vector>

/*!
 * \namespace   ChessNS
 *
 * \brief   A Namespace to describe the chess related types, classes and methods.
 */
namespace ChessNS
{
    /*!
     * \enum    Color
     *
     * \brief   Values that represent colors of the game
     */
    enum class Color { none, white, black };

    /*!
     * \enum    FigureType
     *
     * \brief   Values that represent the available figure types
     */
    enum class FigureType { none, king, queen, rook, knight, bishop, pawn };

    /*!
     * \fn  std::string toString(FigureType e);
     *
     * \brief   Convert this object into a string representation
     *
     * \param   e   A FigureType to process.
     *
     * \returns A std::string that represents this object.
     */
    std::string toString(FigureType e);

    /*!
     * \enum    MoveResult
     *
     * \brief   Values that represent move results
     */
    enum class MoveResult { invalid, valid };

    /*!
     * \enum    EventFlag
     *
     * \brief   Values that represent event flags
     */
    enum class EventFlag { capture, promotion, check, checkmate, castling };

    /*!
     * \fn  std::string toString(EventType e);
     *
     * \brief   Convert this object into a string representation
     *
     * \param   e   An EventType to process.
     *
     * \returns A std::string that represents this object.
     */
    std::string toString(EventFlag e);

    /*!
     * \enum    BoardColumn
     *
     * \brief   Values that represent board columns
     */
    enum class BoardColumn { cA, cB, cC, cD, cE, cF, cG, cH };

    /*!
     * \fn  std::string toString(BoardColumn e);
     *
     * \brief   Convert this object into a string representation
     *
     * \param   e   A BoardColumn to process.
     *
     * \returns A std::string that represents this object.
     */
    std::string toString(BoardColumn e);

    /*!
     * \enum    BoardRow
     *
     * \brief   Values that represent board rows
     */
    enum class BoardRow { r1, r2, r3, r4, r5, r6, r7, r8 };

    /*!
     * \fn  std::string toString(BoardRow e);
     *
     * \brief   Convert this object into a string representation
     *
     * \param   e   A BoardRow to process.
     *
     * \returns A std::string that represents this object.
     */
    std::string toString(BoardRow e);

    /*!
     * \enum    GameResult
     *
     * \brief   Values that represent supported game results
     */
    enum class GameResult { none, victoryWhite, victoryBlack, draw };

    /*!
     * \typedef std::vector<EventType> Events
     *
     * \brief   Defines an alias representing the events container
     */
    typedef std::vector<EventFlag> Events;

    /*!
     * \class   ChessTypes
     *
     * \brief   A helper class for chess tzpes.
     */
    class ChessTypes
    {
    public:

        /*!
         * \fn  static Color ChessTypes::getOpponent(Color color)
         *
         * \brief   Gets the opponent color of the one requested
         *
         * \param   color   The color.
         *
         * \returns The opponent.
         */
        static Color getOpponent(Color color)
        {
            switch (color)
            {
                case Color::white: return Color::black;
                case Color::black: return Color::white;
                default: return Color::none;
            }
        }
    };

    /*!
     * \struct  Position
     *
     * \brief   A struct which describes a position on a chess board.
     */
    struct Position
    {
        Position();

        /*!
         * \fn  explicit Position(BoardRow row, BoardColumn column);
         *
         * \brief   Constructor
         *
         * \param   row     The row.
         * \param   column  The column.
         */
        explicit Position(BoardRow row, BoardColumn column);

        /*!
         * \fn  explicit Position(int row, int column);
         *
         * \brief   Constructor
         *
         * \param   row     The row.
         * \param   column  The column.
         */
        explicit Position(int row, int column);

        /*!
         * \fn  std::pair<int, int> getCord() const;
         *
         * \brief   Gets the coordinates of the board as 0 based integer in <Row, Column>
         *
         * \returns The cord as <Row, Column>.
         */
        std::pair<int, int> getCord() const;

        /*!
         * \fn  void set(int row, int column);
         *
         * \brief   Sets the position with 0 based integer
         *
         * \param   row     The row.
         * \param   column  The column.
         */
        void set(int row, int column);

        /*!
         * \fn  bool isValid() const;
         *
         * \brief   Query if this object is a valid position on a chess board
         *
         * \returns True if valid, false if not.
         */
        bool isValid() const;

        /*!
         * \fn  friend bool operator==(const Position& lhs, const Position& rhs);
         *
         * \brief   Equality operator
         *
         * \param   lhs The first instance to compare.
         * \param   rhs The second instance to compare.
         *
         * \returns True if the parameters are considered equivalent.
         */
        friend bool operator==(const Position& lhs, const Position& rhs);

        /*!
         * \fn  friend bool operator!=(const Position& lhs, const Position& rhs);
         *
         * \brief   Inequality operator
         *
         * \param   lhs The first instance to compare.
         * \param   rhs The second instance to compare.
         *
         * \returns True if the parameters are not considered equivalent.
         */
        friend bool operator!=(const Position& lhs, const Position& rhs);

        /*!
         * \fn  Position& operator+=(const Position& other);
         *
         * \brief   Addition assignment operator
         *
         * \param   other   The other.
         *
         * \returns The result of the operation.
         */
        Position& operator+=(const Position& other);

        /*!
         * \fn  Position& operator-=(const Position& other);
         *
         * \brief   Subtraction assignment operator
         *
         * \param   other   The other.
         *
         * \returns The result of the operation.
         */
        Position& operator-=(const Position& other);

        /*!
         * \fn  friend Position operator+(Position lhs, const Position& rhs);
         *
         * \brief   Addition operator
         *
         * \param   lhs The first value.
         * \param   rhs A value to add to it.
         *
         * \returns The result of the operation.
         */
        friend Position operator+(Position lhs, const Position& rhs);

        /*!
         * \fn  friend Position operator-(Position lhs, const Position& rhs);
         *
         * \brief   Subtraction operator
         *
         * \param   lhs The first value.
         * \param   rhs A value to subtract from it.
         *
         * \returns The result of the operation.
         */
        friend Position operator-(Position lhs, const Position& rhs);

        /*!
         * \fn  void negate();
         *
         * \brief   Negates the position, row *= -1 and column *= -1
         */
        void negate();

        /*!
         * \fn  std::string toString() const;
         *
         * \brief   Gives a string in form of column row, e.g. a1 or g7
         *
         * \returns The string representation.
         */
        std::string toString() const;

        /*! \brief   The row */
        BoardRow row{};
        /*! \brief   The column */
        BoardColumn column{};
    private:
        int  _row{};
        int  _column{};
        bool _valid{};
    };

    /*!
     * \class   Movement
     *
     * \brief   A movement representation class, which give information about all results of a move on a chess board.
     */
    class Movement
    {
    public:
        Movement() = default;

        /*!
         * \fn  static Movement Movement::invalid();
         *
         * \brief   Gets an invalid Movement.
         *
         * \returns A Movement.
         */
        static Movement invalid();

        /*!
         * \fn  bool Movement::isValid();
         *
         * \brief   Query if this object is a valid movement
         *
         * \returns True if valid, false if not.
         */
        bool isValid();

        /*!
         * \fn  Position& Movement::destination();
         *
         * \brief   Gets the destination of the movement.
         *
         * \returns A reference to the destination of the movement.
         */
        Position& destination();

        /*!
         * \fn  Position& Movement::origin();
         *
         * \brief   Gets the origin of the movement.
         *
         * \returns A reference to the origin of the movement.
         */
        Position& origin();

        /*!
         * \fn  FigureType& Movement::figureType();
         *
         * \brief   Gets the Figure type which made or shall make the move.
         *
         * \returns A reference to a FigureType.
         */
        FigureType& figureType();

        /*!
         * \fn  MoveResult& Movement::moveResult();
         *
         * \brief   Move result, invalid or valid
         *
         * \returns A reference to the MoveResult.
         */
        MoveResult& moveResult();

        /*!
         * \fn  bool Movement::hasFlag(EventType eventType);
         *
         * \brief   Query if has flag 'eventType'
         *
         * \param   eventFlag   Type of the event.
         *
         * \returns True if flag, false if not.
         */
        bool hasFlag(EventFlag eventFlag);

        /*!
         * \fn  void Movement::addFlag(EventFlag eventFlag);
         *
         * \brief   Adds a flag to the movement
         *
         * \param   eventFlag   The event flag.
         */
        void addFlag(EventFlag eventFlag);

        /*!
         * \fn  Color& Movement::color();
         *
         * \brief   Gets the color of the player who made the movement
         *
         * \returns A reference to a Color.
         */
        Color& color();

        /*!
         * \fn  unsigned& Movement::round();
         *
         * \brief   Gets the round in which the move was made
         *
         * \returns A reference to an unsigned.
         */
        unsigned& round();

        /*!
         * \fn  FigureType& Movement::promotedTo();
         *
         * \brief   Promoted to a different figure in case a pawn promotion occurred.
         *
         * \returns A reference to a FigureType.
         */
        FigureType& promotedTo();

        /*!
         * \fn  void Movement::removeFlag(EventFlag eventFlag);
         *
         * \brief   Removes the flag described by eventFlag
         *
         * \param   eventFlag   The event flag.
         */
        void removeFlag(EventFlag eventFlag);

    private:

        Position   _destination{};
        Position   _origin{};
        FigureType _type{};
        MoveResult _result{};
        Events     _events{};
        Color      _byColor{};
        unsigned   _round{};
        FigureType _promotedTo{};
    };

    /*!
     * \class   Game
     *
     * \brief   A summary of a full chess game.
     */
    class Game
    {
    public:
        /*! \brief   The result */
        GameResult result{};
        /*! \brief   The movements */
        std::vector<Movement> movements{};
    };
}
