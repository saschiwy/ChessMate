#pragma once
#include <random>

#include "IPlayer.h"

namespace ChessNS
{
    class PlayerSimpleAi : public IPlayer
    {
    public:

        PlayerSimpleAi();

        Movement move(const Movement& move) override;

        Movement move(const Position& origin, const Position& destination) override;

        bool changePromotedPawn(FigureType toType) override;

    private:
        Movement autoMove();

        Movement     _lastValidMovement{};
        std::mt19937 _gen;
    };
}
