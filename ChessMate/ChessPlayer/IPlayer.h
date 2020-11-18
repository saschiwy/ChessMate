#pragma once
#include "ChessEngine/ChessTypes.h"
#include "ChessEngine/Board.h"

namespace ChessNS
{
    enum class PlayerType { human, simpleAi };

    class IPlayer
    {
    public:

        static std::unique_ptr<IPlayer> createPlayer(PlayerType type, Color color, const std::shared_ptr<Board>& board);

        virtual Movement move(const Movement& move) = 0;

        virtual Movement move(const Position& origin, const Position& destination) = 0;

        virtual bool changePromotedPawn(FigureType toType) = 0;

        PlayerType getPlayerType() const;

        Color getColor() const;

    protected:
        std::shared_ptr<Board> _board;
        PlayerType             _playerType{};
        Color                  _playerColor{};
    };
}
