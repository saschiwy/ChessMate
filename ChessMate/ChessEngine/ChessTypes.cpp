#include "ChessTypes.h"

namespace ChessNS
{
    Position::Position()
    {
        set(-1, -1);
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

    Movement Movement::invalid() { return Movement(); }

    bool Movement::valid() { return _result == MoveResult::valid && destination().isValid(); }

    Position& Movement::destination() { return _destination; }

    Position& Movement::origin() { return _origin; }

    FigureType& Movement::figureType() { return _type; }

    MoveResult& Movement::moveResult() { return _result; }

    bool Movement::hasFlag(EventType eventType) { return std::find(_events.begin(), _events.end(), eventType) != _events.end(); }

    void Movement::addFlag(EventType eventType) { if (!hasFlag(eventType)) _events.push_back(eventType); }

    Color& Movement::color() { return _byColor; }

    unsigned& Movement::round() { return _round; }

    FigureType& Movement::promotedTo() { return _promotedTo; }

    void Movement::removeFlag(EventType eventType)
    {
        const auto pos = std::find(_events.begin(), _events.end(), eventType);
        if (pos != _events.end())
            _events.erase(pos);
    }
}
