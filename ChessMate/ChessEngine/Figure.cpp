#include "Figure.h"

namespace ChessNS
{
    Figure::Figure()
        : _lastMoved(0),
          _nbrOfMovements(0),
          _previousPosition(Position(-1, -1)),
          _currentPosition(Position(-1, -1)),
          _startPosition(Position(-1, -1)),
          _color(Color::none),
          _type(FigureType::none) {}

    Figure::Figure(FigureType type, Color color, Position position)
        : _lastMoved(0),
          _nbrOfMovements(0),
          _previousPosition(position),
          _currentPosition(position),
          _startPosition(position),
          _color(color),
          _type(type) {}

    FigureType Figure::getType() const { return _type; }

    void Figure::setType(FigureType type) { _type = type; }

    Color Figure::getColor() const { return _color; }

    Position Figure::getCurrentPosition() const { return _currentPosition; }

    Position Figure::getPreviousPosition() const { return _previousPosition; }

    Position Figure::getInitialPosition() const { return _startPosition; }

    bool Figure::isOpponent(const Figure& other) const { return other._color != this->_color; }

    void Figure::move(const Position& newPosition, unsigned moveCounter)
    {
        _currentPosition = newPosition;
        _lastMoved       = moveCounter;
        ++_nbrOfMovements;
    }

    unsigned Figure::lastMoved() const { return _lastMoved; }

    unsigned Figure::nbrOfMovements() const { return _nbrOfMovements; }
}
