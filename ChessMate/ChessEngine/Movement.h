#pragma once
#include "ChessTypes.h"

namespace ChessNS
{
    class Movement
    {
    public:
        Movement(const Position& origin, const Position& destination, FigureType type, MoveResult result, Color byColor);

        Position getDestination() const;

        Position getOrigin() const;

        FigureType getType() const;

        MoveResult getResult() const;

        Color getByColor() const;

    protected:
        Position   _destination{};
        Position   _origin{};
        FigureType _type{};
        MoveResult _result{};
        Color      _byColor{};
    };
}
