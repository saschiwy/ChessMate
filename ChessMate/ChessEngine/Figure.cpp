/*!
* \brief:  Implements the figure class
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

#include "Figure.h"
#include "Board.h"

namespace ChessNS
{
    Figure::Figure(FigureType type, Color color, Position position)
        : Figure()
    {
        _previousPosition = position;
        _currentPosition  = position;
        _startPosition    = position;
        _color            = color;
        _type             = type;
    }

    FigureType Figure::getType() const noexcept { return _type; }

    void Figure::setType(FigureType type) noexcept { _type = type; }

    Color Figure::getColor() const noexcept { return _color; }

    Position Figure::getCurrentPosition() const noexcept { return _currentPosition; }

    Position Figure::getPreviousPosition() const noexcept { return _previousPosition; }

    Position Figure::getInitialPosition() const noexcept { return _startPosition; }

    bool Figure::isOpponent(const Figure& other) const noexcept
    {
        return other._color != this->_color
            && other._color != Color::none
            && this->_color != Color::none;
    }

    unsigned Figure::lastMoved() const noexcept { return _lastMoved; }

    unsigned Figure::nbrOfMovements() const noexcept { return _nbrOfMovements; }

    bool Figure::canJump() const noexcept { return _type == FigureType::knight; }

    Movement Figure::moveInt(const Position& destination) const
    {
        Movement result      = Movement::invalid();
        result.origin()      = _currentPosition;
        result.destination() = destination;
        result.color()       = _color;
        result.figureType()  = _type;

        return result;
    }

    Movement Figure::move(const Position& destination, Board* board, bool execute)
    {
        if (board == nullptr)
            return Movement::invalid();

        switch (_type)
        {
            case FigureType::king: return reinterpret_cast<King*>(this)->King::move(destination, board, execute);
            case FigureType::queen: return reinterpret_cast<Queen*>(this)->Queen::move(destination, board, execute);
            case FigureType::rook: return reinterpret_cast<Rook*>(this)->Rook::move(destination, board, execute);
            case FigureType::knight: return reinterpret_cast<Knight*>(this)->Knight::move(destination, board, execute);
            case FigureType::bishop: return reinterpret_cast<Bishop*>(this)->Bishop::move(destination, board, execute);
            case FigureType::pawn: return reinterpret_cast<Pawn*>(this)->Pawn::move(destination, board, execute);
            case FigureType::none: ;
        }
        return moveInt(destination);
    }

    void Figure::executeMove(const Position& destination, Board* board)
    {
        if (board == nullptr)
            return;

        _previousPosition = _currentPosition;
        _currentPosition  = destination;
        _lastMoved        = board->currentMove();
        ++_nbrOfMovements;

        board->at(destination).figure       = *this;
        board->at(destination).empty        = false;
        board->at(_previousPosition).empty  = true;
        board->at(_previousPosition).figure = Figure();
    }

    bool Figure::isPathBlocked(const Position& position, Board* board) const
    {
        if (board == nullptr)
            return false;

        const auto                distance = (position - _currentPosition).getCord();
        const std::pair<int, int> absDist  = {std::abs(distance.first), std::abs(distance.second)};

        // all allowed movements are either horizontal, vertical or diagonal
        const auto rSign  = distance.first == 0 ? 0 : distance.first > 0 ? 1 : -1;
        const auto cSign  = distance.second == 0 ? 0 : distance.second > 0 ? 1 : -1;
        const auto maxOff = absDist.first >= absDist.second ? absDist.first : absDist.second;

        for (int i = 1; i < maxOff; i++)
        {
            const auto pos = _currentPosition + Position(i * rSign, i * cSign);
            if (!board->at(pos).empty)
                return true;
        }

        return false;
    }

    Movement Pawn::move(const Position& destination, Board* board, bool execute)
    {
        auto result = moveInt(destination);
        auto dist   = destination - _currentPosition;

        if (_color == Color::black)
            dist.negate();

        // forward Move
        if (dist == Position(1, 0) && board->at(destination).empty ||
            dist == Position(2, 0) && board->at(destination).empty && _lastMoved == 0 && !isPathBlocked(destination, board))
            result.moveResult() = MoveResult::valid;

            // Capture Move
        else if (dist == Position(1, 1) || dist == Position(1, -1))
        {
            // normal capture
            if (!board->at(destination).empty && board->at(destination).figure.isOpponent(*this))
            {
                result.moveResult() = MoveResult::valid;
                result.addFlag(EventFlag::capture);
            }
            else
            {
                // en passant 
                auto       posShift      = destination;
                auto       originRow     = BoardRow::r4;
                const auto opponentColor = ChessTypes::getOpponent(_color);
                if (_color == Color::white)
                {
                    posShift += Position(-1, 0);
                    originRow = BoardRow::r5;
                }
                else
                {
                    posShift += Position(1, 0);
                }

                auto& f = board->at(posShift);
                if (board->at(destination).empty &&
                    _currentPosition.row == originRow &&
                    f.figure.getColor() == opponentColor &&
                    f.figure.getType() == FigureType::pawn &&
                    f.figure.lastMoved() == board->currentMove() - 1 &&
                    f.figure.nbrOfMovements() == 1
                )
                {
                    result.moveResult() = MoveResult::valid;
                    result.addFlag(EventFlag::capture);

                    if (execute && result.isValid())
                    {
                        f.figure = Figure();
                        f.empty  = true;
                    }
                }
            }
        }

        if (result.isValid() &&
            (_color == Color::black && destination.row == BoardRow::r1 || _color == Color::white && destination.row == BoardRow::r8))
            result.addFlag(EventFlag::promotion);

        if (execute && result.isValid())
            executeMove(destination, board);
        return result;
    }

    Movement King::move(const Position& destination, Board* board, bool execute)
    {
        auto       result      = moveInt(destination);
        const auto coordinates = (destination - _currentPosition).getCord();

        // castling
        if (std::abs(coordinates.second) == 2 && std::abs(coordinates.first) == 0 && _nbrOfMovements == 0)
        {
            const auto row      = _currentPosition.row;
            const auto oppColor = ChessTypes::getOpponent(_color);

            if (coordinates.second > 0) // increasing
            {
                if (board->at(row, BoardColumn::cF).empty &&
                    board->at(row, BoardColumn::cG).empty &&
                    board->at(row, BoardColumn::cH).figure.nbrOfMovements() <= 0 &&
                    board->at(row, BoardColumn::cH).figure.getType() == FigureType::rook &&
                    board->at(row, BoardColumn::cH).figure.getColor() == _color &&
                    !board->isFieldUnderAttack(board->at(row, BoardColumn::cE), oppColor) &&
                    !board->isFieldUnderAttack(board->at(row, BoardColumn::cF), oppColor) &&
                    !board->isFieldUnderAttack(board->at(row, BoardColumn::cG), oppColor))
                {
                    result.moveResult() = MoveResult::valid;
                    result.addFlag(EventFlag::castling);
                    if (execute)
                    {
                        auto* k = reinterpret_cast<Rook*>(&(board->at(row, BoardColumn::cH).figure));
                        k->executeMove(Position(row, BoardColumn::cF), board);
                        --_nbrOfMovements;
                    }
                }
            }

                // else decreasing
            else if (board->at(row, BoardColumn::cD).empty &&
                board->at(row, BoardColumn::cC).empty &&
                board->at(row, BoardColumn::cB).empty &&
                board->at(row, BoardColumn::cA).figure.nbrOfMovements() <= 0 &&
                board->at(row, BoardColumn::cA).figure.getType() == FigureType::rook &&
                board->at(row, BoardColumn::cA).figure.getColor() == _color &&
                !board->isFieldUnderAttack(board->at(row, BoardColumn::cF), oppColor) &&
                !board->isFieldUnderAttack(board->at(row, BoardColumn::cD), oppColor) &&
                !board->isFieldUnderAttack(board->at(row, BoardColumn::cC), oppColor))
            {
                result.moveResult() = MoveResult::valid;
                result.addFlag(EventFlag::castling);

                if (execute)
                {
                    auto* k = reinterpret_cast<Rook*>(&(board->at(row, BoardColumn::cA).figure));
                    k->executeMove(Position(row, BoardColumn::cD), board);
                    --_nbrOfMovements;
                }
            }
        }

        if (std::abs(coordinates.first) <= 1 && std::abs(coordinates.second) <= 1)
        {
            if (board->at(destination).empty)
                result.moveResult() = MoveResult::valid;

            else if (board->at(destination).figure.isOpponent(*this))
            {
                result.moveResult() = MoveResult::valid;
                result.addFlag(EventFlag::capture);
            }
        }

        if (execute && result.isValid())
            executeMove(destination, board);

        return result;
    }

    Movement Knight::move(const Position& destination, Board* board, bool execute)
    {
        if (board == nullptr)
            return Movement::invalid();

        auto       result      = moveInt(destination);
        const auto coordinates = (destination - _currentPosition).getCord();

        if ((std::abs(coordinates.first) != 2 || std::abs(coordinates.second) != 1) &&
            (std::abs(coordinates.first) != 1 || std::abs(coordinates.second) != 2))
            return result;

        if (board->at(destination).empty)
            result.moveResult() = MoveResult::valid;

        else if (board->at(destination).figure.isOpponent(*this))
        {
            result.moveResult() = MoveResult::valid;
            result.addFlag(EventFlag::capture);
        }

        if (execute)
            executeMove(destination, board);

        return result;
    }

    Movement Queen::move(const Position& destination, Board* board, bool execute)
    {
        auto result = reinterpret_cast<Rook*>(this)->Rook::move(destination, board, execute);
        if (result.isValid())
            return result;
        return reinterpret_cast<Bishop*>(this)->Bishop::move(destination, board, execute);
    }

    Movement Bishop::move(const Position& destination, Board* board, bool execute)
    {
        if (board == nullptr)
            return Movement::invalid();

        auto       result      = moveInt(destination);
        const auto coordinates = (destination - _currentPosition).getCord();

        // only diagonal
        if (std::abs(coordinates.first) != std::abs(coordinates.second))
            return result;

        if (isPathBlocked(destination, board))
            return result;

        if (board->at(destination).empty)
            result.moveResult() = MoveResult::valid;

        else if (board->at(destination).figure.isOpponent(*this))
        {
            result.moveResult() = MoveResult::valid;
            result.addFlag(EventFlag::capture);
        }

        if (execute && result.isValid())
            executeMove(destination, board);

        return result;
    }

    Movement Rook::move(const Position& destination, Board* board, bool execute)
    {
        if (board == nullptr)
            return Movement::invalid();

        auto       result      = moveInt(destination);
        const auto coordinates = (destination - _currentPosition).getCord();

        // diagonal is not allowed
        if (std::abs(coordinates.first) > 0 && std::abs(coordinates.second) > 0)
            return Movement::invalid();

        if (isPathBlocked(destination, board))
            return Movement::invalid();

        if (board->at(destination).empty)
            result.moveResult() = MoveResult::valid;

        else if (board->at(destination).figure.isOpponent(*this))
        {
            result.moveResult() = MoveResult::valid;
            result.addFlag(EventFlag::capture);
        }

        if (execute && result.isValid())
            executeMove(destination, board);

        return result;
    }
}
