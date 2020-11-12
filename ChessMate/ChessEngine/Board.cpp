#include "Board.h"

namespace ChessNS
{
    Board::Board()
    {
        const Field empty
        {
            true,
            Position(),
            Figure()
        };

        Matrix::assign(8, 8, empty);
        for (size_t r = 0; r < 8; r++)
            for (size_t c = 0; c < 8; c++)
                Matrix::at(r, c).position.set(static_cast<int>(r), static_cast<int>(c));
    }

    Board::Board(BoardStartType boardStart)
        : Board()
    {
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

            _kings[0] = &at(BoardRow::r1, BoardColumn::cE);
            _kings[1] = &at(BoardRow::r8, BoardColumn::cE);
        }
    }

    Field& Board::at(Position position)
    {
        return Matrix::at(static_cast<size_type>(position.row), static_cast<size_type>(position.column));
    }

    Field& Board::at(BoardRow row, BoardColumn column)
    {
        return Matrix::at(static_cast<size_type>(row), static_cast<size_type>(column));
    }

    MoveResult Board::allowed(Position origin, Position destination) const
    {
        // copy this
        auto back = *this;
        return back.move(origin, destination);
    }

    MoveResult Board::move(Position origin, Position destination)
    {
        if (origin == destination)
            return MoveResult::invalid;

        if (!destination.isValid())
            return MoveResult::invalid;

        if (at(origin).figure.getColor() != _currentColorTurn)
            return MoveResult::invalid;

        auto       back   = *this;
        const auto result = back.move(back.at(origin), back.at(destination), true);

        if (result == MoveResult::invalid)
            return MoveResult::invalid;

        if (back.checkChess(back.at(destination).figure.getColor()))
            return MoveResult::invalid;

        *this             = back;
        _currentColorTurn = _currentColorTurn == Color::white ? Color::black : Color::white;
        ++_currentMove;
        return result;
    }

    bool Board::checkChess(Color color)
    {
        auto* kingField = _kings[static_cast<unsigned>(color) - 1];
        if (kingField == nullptr)
            return false;
        return isFieldUnderAttack(*kingField, color == Color::white ? Color::black : Color::white);
    }

    MoveResult Board::move(Field& origin, Field& destination, bool execute)
    {
        if (!destination.position.isValid())
            return MoveResult::invalid;

        switch (origin.figure.getType())
        {
            case FigureType::king: return kingMove(origin, destination, execute);
            case FigureType::queen: return queenMove(origin, destination, execute);
            case FigureType::rook: return rookMove(origin, destination, execute);
            case FigureType::knight: return knightMove(origin, destination, execute);
            case FigureType::bishop: return bishopMove(origin, destination, execute);
            case FigureType::pawn: return pawnMove(origin, destination, execute);
            case FigureType::none: return MoveResult::invalid;
        }

        return MoveResult::invalid;
    }

    MoveResult Board::pawnMove(Field& origin, Field& destination, bool execute)
    {
        auto dist = destination.position - origin.position;

        if (origin.figure.getColor() == Color::black)
            dist.negate();

        // forward Move
        if (dist == Position(1, 0) && destination.empty)
        {
            const auto res =
                origin.figure.getColor() == Color::white && destination.position.row == BoardRow::r8 ||
                origin.figure.getColor() == Color::black && destination.position.row == BoardRow::r1
                    ? MoveResult::promotion
                    : MoveResult::valid;

            if (execute) moveFigure(origin, destination);
            return res;
        }

        if (dist == Position(2, 0) && destination.empty && origin.figure.lastMoved() == 0)
        {
            if (execute) moveFigure(origin, destination);
            return MoveResult::valid;
        }

        // Capture Move
        if (dist == Position(1, 1) || dist == Position(1, -1))
        {
            // normal capture
            if (!destination.empty && destination.figure.isOpponent(origin.figure))
            {
                if (execute) moveFigure(origin, destination);
                return MoveResult::capture;
            }

            // en passant 
            auto posShift     = destination.position;
            auto captureColor = Color::white;
            auto originRow    = BoardRow::r4;
            if (origin.figure.getColor() == Color::white)
            {
                posShift += Position(-1, 0);
                captureColor = Color::black;
                originRow    = BoardRow::r5;
            }
            else
            {
                posShift += Position(1, 0);
            }

            auto& f = at(posShift);
            if (destination.empty &&
                origin.position.row == originRow &&
                f.figure.getColor() == captureColor &&
                f.figure.getType() == FigureType::pawn &&
                f.figure.lastMoved() == _currentMove - 1 &&
                f.figure.nbrOfMovements() == 1
            )
            {
                if (execute)
                {
                    moveFigure(origin, destination);
                    f.figure = Figure();
                    f.empty  = true;
                }
                return MoveResult::capture;
            }
        }

        return MoveResult::invalid;
    }

    MoveResult Board::kingMove(Field& origin, Field& destination, bool execute)
    {
        const auto dist        = destination.position - origin.position;
        const auto coordinates = dist.getCord();

        auto moveFigureForKing = [this](Field& o, Field& d)
        {
            moveFigure(o, d);
            _kings[static_cast<unsigned>(d.figure.getColor()) - 1] = &d;
        };

        // castling
        if (std::abs(coordinates.second) == 2 && std::abs(coordinates.first) == 0 && origin.figure.nbrOfMovements() == 0)
        {
            const auto row      = origin.position.row;
            const auto oppColor = origin.figure.getColor() == Color::white ? Color::black : Color::white;

            if (coordinates.second > 0) // increasing
            {
                if (!at(row, BoardColumn::cF).empty ||
                    !at(row, BoardColumn::cG).empty ||
                    at(row, BoardColumn::cH).figure.nbrOfMovements() > 0 || // implies that the figure is a rook and of same color, but only on standard boards
                    at(row, BoardColumn::cH).figure.getType() != FigureType::rook ||
                    at(row, BoardColumn::cH).figure.getColor() != origin.figure.getColor() ||
                    isFieldUnderAttack(at(row, BoardColumn::cE), oppColor) ||
                    isFieldUnderAttack(at(row, BoardColumn::cF), oppColor) ||
                    isFieldUnderAttack(at(row, BoardColumn::cG), oppColor)
                )
                    return MoveResult::invalid;

                // Castling is valid
                if (execute)
                {
                    moveFigureForKing(origin, destination);
                    moveFigure(at(row, BoardColumn::cH), at(row, BoardColumn::cF));
                }
                return MoveResult::valid;
            }

            // else decreasing
            if (!at(row, BoardColumn::cD).empty ||
                !at(row, BoardColumn::cC).empty ||
                !at(row, BoardColumn::cB).empty ||
                at(row, BoardColumn::cA).figure.nbrOfMovements() > 0 || // implies that the figure is a rook and of same color, but only on standard boards
                at(row, BoardColumn::cA).figure.getType() != FigureType::rook ||
                at(row, BoardColumn::cA).figure.getColor() != origin.figure.getColor() ||
                isFieldUnderAttack(at(row, BoardColumn::cF), oppColor) ||
                isFieldUnderAttack(at(row, BoardColumn::cD), oppColor) ||
                isFieldUnderAttack(at(row, BoardColumn::cC), oppColor)
            )
                return MoveResult::invalid;

            // Castling is valid
            if (execute)
            {
                moveFigureForKing(origin, destination);
                moveFigure(at(row, BoardColumn::cA), at(row, BoardColumn::cD));
            }
            return MoveResult::valid;
        }

        if (std::abs(coordinates.first) > 1 || std::abs(coordinates.second) > 1)
            return MoveResult::invalid;

        if (destination.empty)
        {
            if (execute) moveFigureForKing(origin, destination);
            return MoveResult::valid;
        }

        if (destination.figure.isOpponent(origin.figure))
        {
            if (execute) moveFigureForKing(origin, destination);
            return MoveResult::capture;
        }

        return MoveResult::invalid;
    }

    MoveResult Board::queenMove(Field& origin, Field& destination, bool execute)
    {
        const auto res = rookMove(origin, destination, execute);
        if (res == MoveResult::invalid)
            return bishopMove(origin, destination, execute);
        return res;
    }

    MoveResult Board::bishopMove(Field& origin, Field& destination, bool execute)
    {
        const auto dist        = destination.position - origin.position;
        const auto coordinates = dist.getCord();

        // only diagonal
        if (std::abs(coordinates.first) != std::abs(coordinates.second))
            return MoveResult::invalid;

        const auto rSign = coordinates.first > 0 ? 1 : -1;
        const auto cSign = coordinates.second > 0 ? 1 : -1;

        for (int i = 1; i < std::abs(coordinates.first); i++)
        {
            const auto pos = origin.position + Position(i * rSign, i * cSign);
            if (!at(pos).empty)
                return MoveResult::invalid;
        }

        if (destination.empty)
        {
            if (execute) moveFigure(origin, destination);
            return MoveResult::valid;
        }

        if (destination.figure.isOpponent(origin.figure))
        {
            if (execute) moveFigure(origin, destination);
            return MoveResult::capture;
        }

        return MoveResult::invalid;
    }

    MoveResult Board::knightMove(Field& origin, Field& destination, bool execute)
    {
        const auto dist        = destination.position - origin.position;
        const auto coordinates = dist.getCord();

        if ((std::abs(coordinates.first) != 2 || std::abs(coordinates.second) != 1) &&
            (std::abs(coordinates.first) != 1 || std::abs(coordinates.second) != 2))
            return MoveResult::invalid;

        if (destination.empty)
        {
            if (execute) moveFigure(origin, destination);
            return MoveResult::valid;
        }

        if (destination.figure.isOpponent(origin.figure))
        {
            if (execute) moveFigure(origin, destination);
            return MoveResult::capture;
        }

        return MoveResult::invalid;
    }

    MoveResult Board::rookMove(Field& origin, Field& destination, bool execute)
    {
        const auto dist        = destination.position - origin.position;
        const auto coordinates = dist.getCord();

        // diagonal is not allowed
        if (std::abs(coordinates.first) > 0 && std::abs(coordinates.second) > 0)
            return MoveResult::invalid;

        // Check if everything between is empty
        // Move in y direction
        if (std::abs(coordinates.first) > 0)
        {
            const auto sign = coordinates.first > 0 ? 1 : -1;
            for (int i = 1; i < std::abs(coordinates.first); i++)
            {
                const auto pos = origin.position + Position(i * sign, 0);
                if (!at(pos).empty)
                    return MoveResult::invalid;
            }
        }
            // In x direction
        else
        {
            const auto sign = coordinates.second > 0 ? 1 : -1;
            for (int i = 1; i < std::abs(coordinates.second); i++)
            {
                const auto pos = origin.position + Position(0, i * sign);
                if (!at(pos).empty)
                    return MoveResult::invalid;
            }
        }

        if (destination.empty)
        {
            if (execute) moveFigure(origin, destination);
            return MoveResult::valid;
        }

        if (destination.figure.isOpponent(origin.figure))
        {
            if (execute) moveFigure(origin, destination);
            return MoveResult::capture;
        }

        return MoveResult::invalid;
    }

    bool Board::isFieldUnderAttack(Field& field, Color byColor)
    {
        const auto fields = getAllOccupiedFields(byColor);

        for (auto* f : fields)
        {
            const auto res = move(*f, field, false);
            if (res == MoveResult::valid || res == MoveResult::capture)
                return true;
        }

        return false;
    }

    void Board::moveFigure(Field& origin, Field& destination) const
    {
        origin.figure.move(destination.position, _currentMove);
        destination.figure = origin.figure;
        destination.empty  = false;
        origin.empty       = true;
        origin.figure      = Figure();
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
}
