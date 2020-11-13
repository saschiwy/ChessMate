#pragma once
#include <utility>
#include <vector>

namespace ChessNS
{
    enum class Color { none, white, black };

    enum class FigureType { none, king, queen, rook, knight, bishop, pawn };

    enum class MoveResult { invalid, valid, capture, promotion, check, checkmate };

    enum class BoardColumn { cA, cB, cC, cD, cE, cF, cG, cH };

    enum class BoardRow { r1, r2, r3, r4, r5, r6, r7, r8 };

    enum class GameResult { none, victoryWhite, victoryBlack, draw, resignWhite, resignBlack };

    struct Position
    {
        Position();

        explicit Position(BoardRow row, BoardColumn column);

        explicit Position(int row, int column);

        std::pair<int, int> getCord() const;

        void set(int row, int column);

        bool isValid() const;

        friend bool operator==(const Position& lhs, const Position& rhs);

        friend bool operator!=(const Position& lhs, const Position& rhs);

        Position& operator+=(const Position& other);

        Position& operator-=(const Position& other);

        friend Position operator+(Position lhs, const Position& rhs);

        friend Position operator-(Position lhs, const Position& rhs);

        void negate();

        BoardRow    row{};
        BoardColumn column{};
    private:
        int  _row{};
        int  _column{};
        bool _valid{};
    };

    class Movement
    {
    public:
        Movement() = default;

        Movement(const Position& destination, const Position& origin, FigureType type, MoveResult result, Color byColor);

        Movement(const Position& destination, const Position& origin, FigureType type, MoveResult result, Color byColor, unsigned round);

        Position   destination{};
        Position   origin{};
        FigureType type{};
        MoveResult result{};
        Color      byColor{};
        unsigned   round{};
        FigureType promotedTo{};
    };

    class Game
    {
    public:
        GameResult                result{};
        std::vector<Movement> movements{};
    };
}
