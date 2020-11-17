#pragma once

#include "BasicUtils/Matrix.h"
#include "ChessTypes.h"
#include "Figure.h"

namespace ChessNS
{
    struct Field
    {
        bool     empty{true};
        Position position{};
        Figure   figure{};

        Field() = default;
    };

    #ifdef BUILD_TESTS
    class TestBoard;
    class TestBoardRelationalMove;
    class TestBoardDiagonalMove;
    class TestBoardRankFieldMove;
    #endif

    class Board : public Matrix<Field>
    {
    public:

        #ifdef BUILD_TESTS
        friend TestBoard;
        friend TestBoardRelationalMove;
        friend TestBoardDiagonalMove;
        friend TestBoardRankFieldMove;
        #endif

        enum class BoardStartType { empty, standard };

        Board();

        Field& at(Position position);

        Field& at(BoardRow row, BoardColumn column);

        Movement allowed(Position origin, Position destination) const;

        Movement allowed(Movement movement) const;

        Movement move(Position origin, Position destination);

        Movement move(Movement movement);

        bool changeFigureType(const Position& position, FigureType figure);

        bool isCheck(Color color);

        GameResult checkVictory();

        GameResult checkVictory(Color againstColor);

        bool hasEnded() const;

        std::vector<Movement> getAllPossibleMoves(Position origin);

        std::vector<Movement> getAllPossibleMoves(Color ofColor);

        std::vector<Movement> getAllMadeMoves() const;

        unsigned currentMove() const;

        bool isFieldUnderAttack(Field& field, Color byColor);

    private:

        explicit Board(BoardStartType boardStart);

        Movement move(Position origin, Position destination, bool checkVictory);

        Movement allowed(Position origin, Position destination, bool checkVictory) const;

        static void createFigure(Field& field, FigureType figure, Color color);

        std::vector<Field*> getAllOccupiedFields(Color ofColor);

        Field* getFigure(Color byColor, FigureType figureType);

        unsigned              _currentMove{1};
        Color                 _currentColorTurn{Color::white};
        bool                  _ended{false};
        std::vector<Movement> _movements;
    };
}
