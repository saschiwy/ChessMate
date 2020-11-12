#include "Movement.h"

namespace ChessNS
{
    Movement::Movement(const Position& origin, const Position& destination, FigureType type, MoveResult result, Color byColor)
        : _destination(destination),
          _origin(origin),
          _type(type),
          _result(result),
          _byColor(byColor) {}

    Position Movement::getDestination() const { return _destination; }

    Position Movement::getOrigin() const { return _origin; }

    FigureType Movement::getType() const { return _type; }

    MoveResult Movement::getResult() const { return _result; }

    Color Movement::getByColor() const { return _byColor; }
}
