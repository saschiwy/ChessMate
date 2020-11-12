#pragma once
#include <memory>

#include "ChessTypes.h"

namespace ChessNS
{
    class Figure
    {
    public:
        Figure();

        explicit Figure(FigureType type, Color color, Position position);

        FigureType getType() const;

        void setType(FigureType type);

        Color getColor() const;

        Position getCurrentPosition() const;

        Position getPreviousPosition() const;

        Position getInitialPosition() const;

        bool isOpponent(const Figure& other) const;

        void move(const Position& newPosition, unsigned moveCounter);

        unsigned lastMoved() const;

        unsigned nbrOfMovements() const;

    protected:
        unsigned   _lastMoved;
        unsigned   _nbrOfMovements;
        Position   _previousPosition;
        Position   _currentPosition;
        Position   _startPosition;
        Color      _color;
        FigureType _type;
    };
}
