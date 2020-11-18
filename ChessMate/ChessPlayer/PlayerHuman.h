#pragma once
#include "IPlayer.h"

namespace ChessNS
{
    class PlayerHuman : public IPlayer
    {
    public:

        Movement move(const Movement& move) override;

        Movement move(const Position& origin, const Position& destination) override;

        bool changePromotedPawn(FigureType toType) override;

    private:
        Movement _lastValidMovement{};
    };
}
