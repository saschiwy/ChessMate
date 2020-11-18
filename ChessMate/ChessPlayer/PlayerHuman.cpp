#include "PlayerHuman.h"

namespace ChessNS
{
    Movement PlayerHuman::move(const Movement& move)
    {
        auto result = _board->move(move);

        if (result.valid())
            _lastValidMovement = result;

        return result;
    }

    Movement PlayerHuman::move(const Position& origin, const Position& destination)
    {
        auto result = _board->move(origin, destination);

        if (result.valid())
            _lastValidMovement = result;

        return result;
    }

    bool PlayerHuman::changePromotedPawn(FigureType toType)
    {
        if (_lastValidMovement.valid() && _lastValidMovement.hasFlag(EventType::promotion))
        {
            _lastValidMovement.promotedTo() = toType;
            _board->at(_lastValidMovement.destination()).figure.setType(toType);
            return true;
        }

        return false;
    }
}
