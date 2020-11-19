/*!
* \brief:  Implements the board class
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

#include "Board.h"
#include <utility>

namespace ChessNS
{
    Board::Board()
        : Board(BoardStartType::standard) { }

    Board::Board(BoardStartType boardStart)
    {
        const Field empty;

        Matrix::assign(8, 8, empty);
        for (size_t r = 0; r < 8; r++)
            for (size_t c = 0; c < 8; c++)
                Matrix::at(r, c).position.set(static_cast<int>(r), static_cast<int>(c));

        if (boardStart == BoardStartType::standard)
        {
            // Create Pawns
            for (auto it = Matrix::begin(1); it != Matrix::end(1); ++it)
                createFigure(*it, FigureType::pawn, Color::white);

            for (auto it = Matrix::begin(6); it != Matrix::end(6); ++it)
                createFigure(*it, FigureType::pawn, Color::black);

            createFigure(at(BoardRow::r1, BoardColumn::cA), FigureType::rook, Color::white);
            createFigure(at(BoardRow::r1, BoardColumn::cB), FigureType::knight, Color::white);
            createFigure(at(BoardRow::r1, BoardColumn::cC), FigureType::bishop, Color::white);
            createFigure(at(BoardRow::r1, BoardColumn::cD), FigureType::queen, Color::white);
            createFigure(at(BoardRow::r1, BoardColumn::cE), FigureType::king, Color::white);
            createFigure(at(BoardRow::r1, BoardColumn::cF), FigureType::bishop, Color::white);
            createFigure(at(BoardRow::r1, BoardColumn::cG), FigureType::knight, Color::white);
            createFigure(at(BoardRow::r1, BoardColumn::cH), FigureType::rook, Color::white);

            createFigure(at(BoardRow::r8, BoardColumn::cA), FigureType::rook, Color::black);
            createFigure(at(BoardRow::r8, BoardColumn::cB), FigureType::knight, Color::black);
            createFigure(at(BoardRow::r8, BoardColumn::cC), FigureType::bishop, Color::black);
            createFigure(at(BoardRow::r8, BoardColumn::cD), FigureType::queen, Color::black);
            createFigure(at(BoardRow::r8, BoardColumn::cE), FigureType::king, Color::black);
            createFigure(at(BoardRow::r8, BoardColumn::cF), FigureType::bishop, Color::black);
            createFigure(at(BoardRow::r8, BoardColumn::cG), FigureType::knight, Color::black);
            createFigure(at(BoardRow::r8, BoardColumn::cH), FigureType::rook, Color::black);
        }
    }

    Movement Board::move(Position origin, Position destination, bool checkVictory)
    {
        if (_ended)
            return Movement::invalid();

        if (origin == destination)
            return Movement::invalid();

        if (!destination.isValid())
            return Movement::invalid();

        auto       back          = *this;
        const auto currentColor  = back.at(origin).figure.getColor();
        const auto opponentColor = ChessTypes::getOpponent(currentColor);
        auto       result        = back.at(origin).figure.move(destination, &back, true);

        if (!result.isValid())
            return Movement::invalid();

        if (back.isCheck(currentColor))
            return Movement::invalid();

        if (checkVictory)
        {
            const auto vic = back.checkVictory(opponentColor);
            if (vic == GameResult::victoryBlack || vic == GameResult::victoryWhite)
                result.addFlag(EventFlag::checkmate);
        }

        if (!back._ended && back.isCheck(opponentColor))
            result.addFlag(EventFlag::check);

        *this             = back;
        result.round()    = _currentMove / 2 + 1;
        result.origin()   = origin;
        _currentColorTurn = opponentColor;

        ++_currentMove;
        _movements.emplace_back(result);
        return result;
    }

    Movement Board::allowed(Position origin, Position destination, bool checkVictory) const
    {
        auto back = *this;
        return back.move(origin, destination, checkVictory);
    }

    Field& Board::at(Position position)
    {
        return Matrix::at(static_cast<size_type>(position.row), static_cast<size_type>(position.column));
    }

    Field& Board::at(BoardRow row, BoardColumn column)
    {
        return Matrix::at(static_cast<size_type>(row), static_cast<size_type>(column));
    }

    Movement Board::allowed(Position origin, Position destination) const
    {
        return allowed(origin, destination, true);
    }

    Movement Board::move(Position origin, Position destination)
    {
        if (at(origin).figure.getColor() != _currentColorTurn)
            return Movement::invalid();

        return move(origin, destination, true);
    }

    Movement Board::allowed(Movement movement) const
    {
        auto back = *this;
        return back.move(std::move(movement));
    }

    bool Board::changeFigureType(const Position& position, FigureType figure)
    {
        if (at(position).empty)
            return false;

        at(position).figure.setType(figure);
        return true;
    }

    Movement Board::move(Movement movement)
    {
        if (movement.origin().isValid())
            return move(movement.origin(), movement.destination());

        // estimate origin
        const auto fields = getAllOccupiedFields(movement.color());
        auto       result = Movement::invalid();

        for (auto* f : fields)
        {
            if (f->figure.getType() != movement.figureType())
                continue;

            if (f->figure.move(movement.destination(), this, false).isValid())
            {
                const auto rc = movement.origin().getCord();
                if ((rc.first == -1 && rc.second == -1)
                    || (rc.first != -1 && f->position.getCord().first == rc.first)
                    || (rc.second != -1 && f->position.getCord().second == rc.second))
                {
                    result = move(f->position, movement.destination());
                    break;
                }
            }
        }

        if (movement.promotedTo() != FigureType::none)
        {
            changeFigureType(movement.destination(), movement.promotedTo());

            result.removeFlag(EventFlag::checkmate);
            result.removeFlag(EventFlag::check);

            const auto opponentColor = ChessTypes::getOpponent(movement.color());
            const auto vic           = checkVictory(opponentColor);

            if (vic == GameResult::victoryBlack || vic == GameResult::victoryWhite)
                result.addFlag(EventFlag::checkmate);

            if (!_ended && isCheck(opponentColor))
                result.addFlag(EventFlag::check);

            _movements.back() = result;
        }

        return result;
    }

    bool Board::isCheck(Color color)
    {
        auto* kingField = getFigure(color, FigureType::king);
        if (kingField == nullptr)
            return false;
        return isFieldUnderAttack(*kingField, ChessTypes::getOpponent(color));
    }

    GameResult Board::checkVictory()
    {
        return checkVictory(_currentColorTurn);
    }

    GameResult Board::checkVictory(Color againstColor)
    {
        auto* kingField = getFigure(againstColor, FigureType::king);
        if (kingField == nullptr)
            return GameResult::none;

        // King can still move
        if (!getAllPossibleMoves(kingField->position).empty())
            return GameResult::none;

        // Something can move
        if (!getAllPossibleMoves(againstColor).empty())
            return GameResult::none;

        _ended = true;
        return isCheck(againstColor)
                   ? (againstColor == Color::white
                          ? GameResult::victoryBlack
                          : GameResult::victoryWhite)
                   : GameResult::draw;
    }

    bool Board::hasEnded() const
    {
        return _ended;
    }

    std::vector<Movement> Board::getAllPossibleMoves(Position origin)
    {
        std::vector<Movement> result;

        if (at(origin).empty)
            return result;

        for (auto& field : *this)
        {
            auto res = allowed(origin, field.position, false);
            if (res.isValid())
                result.emplace_back(res);
        }

        return result;
    }

    std::vector<Movement> Board::getAllPossibleMoves(Color ofColor)
    {
        std::vector<Movement> result;
        auto                  fields = getAllOccupiedFields(ofColor);
        for (auto&& field : fields)
        {
            auto moves = getAllPossibleMoves(field->position);
            result.insert(result.end(), moves.begin(), moves.end());
        }
        return result;
    }

    std::vector<Movement> Board::getAllMadeMoves() const
    {
        return _movements;
    }

    unsigned Board::currentMove() const
    {
        return _currentMove;
    }

    bool Board::isFieldUnderAttack(Field& field, Color byColor)
    {
        const auto fields = getAllOccupiedFields(byColor);

        for (auto* f : fields)
        {
            auto res = f->figure.move(field.position, this, false);
            if (res.moveResult() == MoveResult::valid)
                return true;
        }

        return false;
    }

    void Board::createFigure(Field& field, FigureType figure, Color color)
    {
        field.figure = Figure(figure, color, field.position);
        field.empty  = false;
    }

    std::vector<Field*> Board::getAllOccupiedFields(Color ofColor)
    {
        std::vector<Field*> result;
        for (auto& field : *this)
        {
            if (field.figure.getColor() == ofColor)
                result.push_back(&field);
        }

        return result;
    }

    Field* Board::getFigure(Color byColor, FigureType figureType)
    {
        for (auto& field : *this)
        {
            if (field.figure.getType() == figureType && field.figure.getColor() == byColor)
                return &field;
        }

        return nullptr;
    }
}
