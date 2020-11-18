#include "ChessTypes.h"

namespace ChessNS
{
    std::string toString(FigureType e)
    {
        switch (e)
        {
            case FigureType::none: return "none";
            case FigureType::king: return "king";
            case FigureType::queen: return "queen";
            case FigureType::rook: return "rook";
            case FigureType::knight: return "knight";
            case FigureType::bishop: return "bishop";
            case FigureType::pawn: return "pawn";
            default: return "undefined";
        }
    }

    std::string toString(EventType e)
    {
        switch (e)
        {
            case EventType::capture: return "capture";
            case EventType::promotion: return "promotion";
            case EventType::check: return "check";
            case EventType::checkmate: return "checkmate";
            case EventType::castling: return "castling";
            default: return "undefined";
        }
    }

    std::string toString(BoardColumn e)
    {
        switch (e)
        {
            case BoardColumn::cA: return "a";
            case BoardColumn::cB: return "b";
            case BoardColumn::cC: return "c";
            case BoardColumn::cD: return "d";
            case BoardColumn::cE: return "e";
            case BoardColumn::cF: return "f";
            case BoardColumn::cG: return "g";
            case BoardColumn::cH: return "h";
            default: return "undefined";
        }
    }

    std::string toString(BoardRow e)
    {
        switch (e)
        {
            case BoardRow::r1: return "1";
            case BoardRow::r2: return "2";
            case BoardRow::r3: return "3";
            case BoardRow::r4: return "4";
            case BoardRow::r5: return "5";
            case BoardRow::r6: return "6";
            case BoardRow::r7: return "7";
            case BoardRow::r8: return "8";
            default: return "undefined";
        }
    }

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

    std::string Position::toString() const
    {
        return ChessNS::toString(column) + ChessNS::toString(row);
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
