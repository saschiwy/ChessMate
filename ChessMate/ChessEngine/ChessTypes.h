#pragma once
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace ChessNS
{
    enum class Color { none, white, black };

    enum class FigureType { none, king, queen, rook, knight, bishop, pawn };

    std::string toString(FigureType e);

    enum class MoveResult { invalid, valid };

    enum class EventType { capture, promotion, check, checkmate, castling };

    std::string toString(EventType e);

    enum class BoardColumn { cA, cB, cC, cD, cE, cF, cG, cH };

    std::string toString(BoardColumn e);

    enum class BoardRow { r1, r2, r3, r4, r5, r6, r7, r8 };

    std::string toString(BoardRow e);

    enum class GameResult { none, victoryWhite, victoryBlack, draw, resignWhite, resignBlack };

    typedef std::vector<EventType> Events;

    class ChessTypes
    {
    public:
        static Color getOpponent(Color color) { return color == Color::white ? Color::black : Color::white; }
    };

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

        std::string toString() const;

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

        static Movement invalid();

        bool valid();

        Position& destination();

        Position& origin();

        FigureType& figureType();

        MoveResult& moveResult();

        bool hasFlag(EventType eventType);

        void addFlag(EventType eventType);

        Color& color();

        unsigned& round();

        FigureType& promotedTo();

        void removeFlag(EventType eventType);

    private:

        Position   _destination{};
        Position   _origin{};
        FigureType _type{};
        MoveResult _result{};
        Events     _events{};
        Color      _byColor{};
        unsigned   _round{};
        FigureType _promotedTo{};
    };

    class Game
    {
    public:
        GameResult            result{};
        std::vector<Movement> movements{};
    };
}
