#include "gtest/gtest.h"
#include "ChessParser/PgnParser.h"

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
}
