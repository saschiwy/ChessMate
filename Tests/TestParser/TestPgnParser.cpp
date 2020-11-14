#include "gtest/gtest.h"
#include "ChessParser/PgnParser.h"
#include "ChessEngine//Board.h"

namespace ChessNS
{
    class TestPgnParser : public ::testing::Test
    {
    protected:
        TestPgnParser() = default;

        virtual ~TestPgnParser() = default;
    };

    TEST_F(TestPgnParser, parseSingleGame_completeGame_GameWithResult)
    {
        PgnParser parser;
        auto      game = parser.parseSingleGame("./pgn_examples/fools_mate.pgn");

        ASSERT_EQ(GameResult::victoryBlack, game.result);
        ASSERT_EQ(4, game.movements.size());

        ASSERT_EQ(Position(BoardRow::r3, BoardColumn::cF), game.movements[0].destination);
        ASSERT_EQ(FigureType::pawn, game.movements[0].type);
        ASSERT_EQ(Color::white, game.movements[0].byColor);

        ASSERT_EQ(Position(BoardRow::r5, BoardColumn::cE), game.movements[1].destination);
        ASSERT_EQ(FigureType::pawn, game.movements[1].type);
        ASSERT_EQ(Color::black, game.movements[1].byColor);

        ASSERT_EQ(Position(BoardRow::r4, BoardColumn::cG), game.movements[2].destination);
        ASSERT_EQ(FigureType::pawn, game.movements[2].type);
        ASSERT_EQ(Color::white, game.movements[2].byColor);

        ASSERT_EQ(Position(BoardRow::r4, BoardColumn::cH), game.movements[3].destination);
        ASSERT_EQ(FigureType::queen, game.movements[3].type);
        ASSERT_EQ(Color::black, game.movements[3].byColor);
    }

    TEST_F(TestPgnParser, parseSingleGame_incompleteGame_GameWithNoneResult)
    {
        PgnParser parser;
        auto      game = parser.parseSingleGame("./pgn_examples/fools_mate_incomplete.pgn");

        ASSERT_EQ(GameResult::none, game.result);
        ASSERT_EQ(4, game.movements.size());

        ASSERT_EQ(Position(BoardRow::r3, BoardColumn::cF), game.movements[0].destination);
        ASSERT_EQ(FigureType::pawn, game.movements[0].type);
        ASSERT_EQ(Color::white, game.movements[0].byColor);

        ASSERT_EQ(Position(BoardRow::r5, BoardColumn::cE), game.movements[1].destination);
        ASSERT_EQ(FigureType::pawn, game.movements[1].type);
        ASSERT_EQ(Color::black, game.movements[1].byColor);

        ASSERT_EQ(Position(BoardRow::r4, BoardColumn::cG), game.movements[2].destination);
        ASSERT_EQ(FigureType::pawn, game.movements[2].type);
        ASSERT_EQ(Color::white, game.movements[2].byColor);

        ASSERT_EQ(Position(BoardRow::r4, BoardColumn::cH), game.movements[3].destination);
        ASSERT_EQ(FigureType::queen, game.movements[3].type);
        ASSERT_EQ(Color::black, game.movements[3].byColor);
    }

    TEST_F(TestPgnParser, parseSingleGame_completeGamePlusBoard_BoardAndResultAreEqual)
    {
        Board      board;
        PgnParser  parser;
        const auto game = parser.parseSingleGame("./pgn_examples/black_checkmate.pgn");

        // Create expected Results
        const auto expectedMovements = 105;
        const auto expectedResult    = GameResult::victoryWhite;

        std::vector<std::vector<Figure>> expectedFigures(8, std::vector<Figure>(8));
        expectedFigures[3][7] = Figure(FigureType::pawn, Color::white, Position(3, 7));
        expectedFigures[2][0] = Figure(FigureType::queen, Color::white, Position(2, 0));
        expectedFigures[2][3] = Figure(FigureType::queen, Color::white, Position(2, 3));
        expectedFigures[1][7] = Figure(FigureType::pawn, Color::white, Position(1, 7));
        expectedFigures[1][5] = Figure(FigureType::pawn, Color::white, Position(1, 5));
        expectedFigures[1][6] = Figure(FigureType::king, Color::white, Position(1, 6));
        expectedFigures[3][6] = Figure(FigureType::pawn, Color::black, Position(3, 6));
        expectedFigures[1][2] = Figure(FigureType::king, Color::black, Position(1, 2));

        ASSERT_EQ(expectedMovements, game.movements.size());
        ASSERT_EQ(expectedResult, game.result);

        for (size_t i = 0; i < expectedMovements; i++)
        {
            const auto res = board.move(game.movements.at(i));
            if (game.movements.at(i).result != res)
                FAIL() << "MoveResult {" << static_cast<int>(res) << "} and expected MoveResult {" << static_cast<int>(game.movements.at(i).result) <<
                    "} differ at movement " << i;
        }

        for (int row = 0; row < 8; row++)
            for (int col = 0; col < 8; col++)
            {
                ASSERT_EQ(expectedFigures[row][col].getColor(), board.at(Position(row, col)).figure.getColor());
                ASSERT_EQ(expectedFigures[row][col].getType(), board.at(Position(row, col)).figure.getType());
            }
    }
}
