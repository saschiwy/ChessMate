#pragma once
#include <memory>

#include "ChessTypes.h"

namespace ChessNS
{
    class Board;

    class Figure
    {
    public:
        friend Board;

        virtual ~Figure() = default;

        Figure() = default;

        Figure(FigureType type, Color color, Position position);

        FigureType getType() const noexcept;

        void setType(FigureType type) noexcept;

        Color getColor() const noexcept;

        Position getCurrentPosition() const noexcept;

        Position getPreviousPosition() const noexcept;

        Position getInitialPosition() const noexcept;

        bool isOpponent(const Figure& other) const noexcept;

        unsigned lastMoved() const noexcept;

        unsigned nbrOfMovements() const noexcept;

        bool canJump() const noexcept;

        virtual Movement move(const Position& destination, Board* board, bool execute = false);

    protected:

        Movement moveInt(const Position& destination) const;

        void executeMove(const Position& destination, Board* board);

        bool isPathBlocked(const Position& position, Board* board) const;

        unsigned   _lastMoved        = 0;
        unsigned   _nbrOfMovements   = 0;
        Position   _previousPosition = Position(-1, -1);
        Position   _currentPosition  = Position(-1, -1);
        Position   _startPosition    = Position(-1, -1);
        Color      _color            = Color::none;
        FigureType _type             = FigureType::none;

    public:
    };

    class Pawn : public Figure
    {
    public:
        Movement move(const Position& destination, Board* board, bool execute) override;
    };

    class King : public Figure
    {
    public:
        Movement move(const Position& destination, Board* board, bool execute) override;
    };

    class Knight : public Figure
    {
    public:
        Movement move(const Position& destination, Board* board, bool execute) override;
    };

    class Queen : public Figure
    {
    public:
        Movement move(const Position& destination, Board* board, bool execute) override;
    };

    class Bishop : public Figure
    {
    public:
        Movement move(const Position& destination, Board* board, bool execute) override;
    };

    class Rook : public Figure
    {
    public:
        friend King;

        Movement move(const Position& destination, Board* board, bool execute) override;
    };
}
