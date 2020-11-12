#pragma once

#include "BasicUtils/Matrix.h"
#include "ChessTypes.h"
#include "Figure.h"

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
    #endif

    class Board : public Matrix<Field>
    {
    public:

        #ifdef BUILD_TESTS
        friend TestBoard;
        #endif

        enum class BoardStartType { empty, standard };

        Board();

        explicit Board(BoardStartType boardStart);

        Field& at(Position position);

        Field& at(BoardRow row, BoardColumn column);

        MoveResult allowed(Position origin, Position destination) const;

        MoveResult move(Position origin, Position destination);

        bool checkChess(Color color);

    private:

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

        unsigned _currentMove{1};
        Color    _currentColorTurn{Color::white};
        Field*   _kings[2]{};
    };
}
