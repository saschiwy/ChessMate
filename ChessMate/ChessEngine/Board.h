#pragma once

#include "BasicUtils/Matrix.h"
#include "ChessTypes.h"
#include "Figure.h"
#include "Movement.h"

namespace ChessNS
{
    struct Field
    {
        bool     empty;
        Position position;
        Figure   figure;
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

        MoveResult allowed(Position origin, Position destination) const;

        MoveResult move(Position origin, Position destination);

        bool checkChess(Color color);

        Ending checkVictory();

        std::vector<Movement> getAllPossibleMoves(Position origin);

        std::vector<Movement> getAllPossibleMoves(Color ofColor);

    private:
        explicit Board(BoardStartType boardStart);

        MoveResult move(Field& origin, Field& destination, bool execute);

        MoveResult pawnMove(Field& origin, Field& destination, bool execute);

        MoveResult kingMove(Field& origin, Field& destination, bool execute);

        MoveResult queenMove(Field& origin, Field& destination, bool execute);

        MoveResult bishopMove(Field& origin, Field& destination, bool execute);

        MoveResult knightMove(Field& origin, Field& destination, bool execute);

        MoveResult rookMove(Field& origin, Field& destination, bool execute);

        bool isFieldUnderAttack(Field& field, Color byColor);

        void moveFigure(Field& origin, Field& destination) const;

        static void createFigure(Field& field, FigureType figure, Color color);

        std::vector<Field*> getAllOccupiedFields(Color ofColor);

        Field* getFigure(Color byColor, FigureType figureType);

        unsigned _currentMove{1};
        Color    _currentColorTurn{Color::white};
    };
}
