#include "ChessTypes.h"

namespace ChessNS
{
    Position::Position()
    {
        set(0, 0);
    }

    Position::Position(const BoardRow nRow, const BoardColumn nColumn)
    {
        set(static_cast<int>(nRow), static_cast<int>(nColumn));
    }

    Position::Position(int nRow, int nColumn)
    {
        set(nRow, nColumn);
    }

    std::pair<int, int> Position::getCord() const
    {
        return {_row, _column};
    }

    void Position::set(int nRow, int nColumn)
    {
        _row    = nRow;
        _column = nColumn;
        if (_row > 7 || _row < 0 || _column > 7 || _column < 0)
            _valid = false;
        else
        {
            _valid = true;
            row    = static_cast<BoardRow>(_row);
            column = static_cast<BoardColumn>(_column);
        }
    }

    bool Position::isValid() const
    {
        return _valid;
    }

    bool operator==(const Position& lhs, const Position& rhs)
    {
        return lhs._row == rhs._row && lhs._column == rhs._column;
    }

    bool operator!=(const Position& lhs, const Position& rhs)
    {
        return lhs._row != rhs._row || lhs._column != rhs._column;
    }

    Position& Position::operator+=(const Position& other)
    {
        set(this->_row + other._row,
            this->_column + other._column);

        return *this;
    }

    Position& Position::operator-=(const Position& other)
    {
        set(this->_row - other._row,
            this->_column - other._column);

        return *this;
    }

    void Position::negate()
    {
        _row    = -_row;
        _column = -_column;
    }

    Position operator+(const Position lhs, const Position& rhs)
    {
        return Position(lhs._row + rhs._row, lhs._column + rhs._column);
    }

    Position operator-(Position lhs, const Position& rhs)
    {
        return Position(lhs._row - rhs._row, lhs._column - rhs._column);
    }
}