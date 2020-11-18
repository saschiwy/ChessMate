#include "IPlayer.h"
#include "PlayerHuman.h"
#include "PlayerSimpleAi.h"

namespace ChessNS
{
    std::unique_ptr<IPlayer> IPlayer::createPlayer(PlayerType type, Color color, const std::shared_ptr<Board>& board)
    {
        std::unique_ptr<IPlayer> result;
        switch (type)
        {
            case PlayerType::human: result = std::make_unique<PlayerHuman>();
                break;
            case PlayerType::simpleAi: result = std::make_unique<PlayerSimpleAi>();
                break;
            default: return nullptr;
        }

        result->_playerColor = color;
        result->_playerType  = type;
        result->_board       = board;

        return std::move(result);
    }

    PlayerType IPlayer::getPlayerType() const { return _playerType; }

    Color IPlayer::getColor() const { return _playerColor; }
}
