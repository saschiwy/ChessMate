#pragma once
#include <utility>

namespace ChessNS
{
    enum class Color { none, white, black };

    enum class FigureType { none, king, queen, rook, knight, bishop, pawn };

    enum class MoveResult { invalid, valid, capture, promotion };

    enum class BoardColumn { cA, cB, cC, cD, cE, cF, cG, cH };

    enum class BoardRow { r1, r2, r3, r4, r5, r6, r7, r8 };

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

    struct Move
    {
        Position origin{};
        Position destination{};
    };
}
