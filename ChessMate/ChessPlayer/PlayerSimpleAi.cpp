#include "PlayerSimpleAi.h"

namespace ChessNS
{
    PlayerSimpleAi::PlayerSimpleAi()
    {
        std::random_device rd;
        _gen = std::mt19937(rd());
    }

    Movement PlayerSimpleAi::move(const Movement&)
    {
        return autoMove();
    }

    Movement PlayerSimpleAi::move(const Position& origin, const Position& destination)
    {
        return autoMove();
    }

    bool PlayerSimpleAi::changePromotedPawn(FigureType toType)
    {
        _board->at(_lastValidMovement.destination()).figure.setType(toType);
        return true;
    }

    Movement PlayerSimpleAi::autoMove()
    {
        auto movements = _board->getAllPossibleMoves(_playerColor);

        for (auto&& movement : movements)
            if (movement.hasFlag(EventType::checkmate))
                return movement;

        for (auto&& movement : movements)
            if (movement.hasFlag(EventType::check))
                return movement;

        for (auto&& movement : movements)
            if (movement.hasFlag(EventType::promotion))
                return movement;

        for (auto&& movement : movements)
            if (movement.hasFlag(EventType::capture))
                return movement;

        const std::uniform_int_distribution<> dist(0, static_cast<int>(movements.size() - 1));
        _lastValidMovement = _board->move(movements.at(dist(_gen)));
        if (_lastValidMovement.hasFlag(EventType::promotion))
            changePromotedPawn(FigureType::queen);
        return _lastValidMovement;
    }
}
