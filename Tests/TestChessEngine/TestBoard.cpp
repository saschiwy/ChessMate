/*!
* \brief:  Implements the test board class
*
* The MIT License (MIT)
*
* Copyright (c) 2020 Sascha Schiwy. All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
 */

#include "gtest/gtest.h"
#include "ChessEngine/Board.h"

namespace ChessNS
{
    class TestBoard : public ::testing::Test
    {
    protected:
        TestBoard() = default;

        virtual ~TestBoard() = default;

        static void createFigure(Field& field, FigureType figureType, Color color)
        {
            Board::createFigure(field, figureType, color);
        }

        void renewBoard()
        {
            _board = Board(Board::BoardStartType::empty);
        }

        Board _board{Board::BoardStartType::empty};
    };

    class TestBoardRelationalMove : public ::testing::TestWithParam<Position>
    {
    protected:
        static void createFigure(Field& field, FigureType figureType, Color color)
        {
            Board::createFigure(field, figureType, color);
        }

        Board _board{Board::BoardStartType::empty};
    };

    class TestBoardDiagonalMove : public ::testing::TestWithParam<FigureType>
    {
    protected:
        static void createFigure(Field& field, FigureType figureType, Color color)
        {
            Board::createFigure(field, figureType, color);
        }

        void renewBoard()
        {
            _board = Board(Board::BoardStartType::empty);
        }

        Board _board{Board::BoardStartType::empty};
    };

    class TestBoardRankFieldMove : public ::testing::TestWithParam<FigureType>
    {
    protected:
        static void createFigure(Field& field, FigureType figureType, Color color)
        {
            Board::createFigure(field, figureType, color);
        }

        void renewBoard()
        {
            _board = Board(Board::BoardStartType::empty);
        }

        Board _board{Board::BoardStartType::empty};
    };

    TEST_F(TestBoard, movePawn_empty_valid)
    {
        const Position origin(BoardRow::r2, BoardColumn::cA);
        const Position expectedDestination(BoardRow::r3, BoardColumn::cA);

        createFigure(_board.at(origin), FigureType::pawn, Color::white);

        ASSERT_EQ(FigureType::pawn, _board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::none, _board.at(expectedDestination).figure.getType());

        auto result = _board.move(origin, expectedDestination);
        ASSERT_EQ(MoveResult::valid, result.moveResult());

        ASSERT_EQ(FigureType::none, _board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::pawn, _board.at(expectedDestination).figure.getType());
    }

    TEST_F(TestBoard, movePawnFoward_notEmpty_invalid)
    {
        const Position origin(BoardRow::r2, BoardColumn::cA);
        const Position expectedDestination(BoardRow::r3, BoardColumn::cA);

        createFigure(_board.at(origin), FigureType::pawn, Color::white);
        createFigure(_board.at(expectedDestination), FigureType::pawn, Color::black);

        ASSERT_EQ(FigureType::pawn, _board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::pawn, _board.at(expectedDestination).figure.getType());

        auto result = _board.move(origin, expectedDestination);
        ASSERT_EQ(MoveResult::invalid, result.moveResult());

        ASSERT_EQ(FigureType::pawn, _board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::pawn, _board.at(expectedDestination).figure.getType());
    }

    TEST_F(TestBoard, movePawnCapture_notEmpty_captured)
    {
        const Position origin(BoardRow::r2, BoardColumn::cA);
        const Position expectedDestination(BoardRow::r3, BoardColumn::cB);

        createFigure(_board.at(origin), FigureType::pawn, Color::white);
        createFigure(_board.at(expectedDestination), FigureType::pawn, Color::black);

        ASSERT_EQ(FigureType::pawn, _board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::pawn, _board.at(expectedDestination).figure.getType());

        auto result = _board.move(origin, expectedDestination);
        ASSERT_EQ(MoveResult::valid, result.moveResult());
        ASSERT_TRUE(result.hasFlag(EventFlag::capture));

        ASSERT_EQ(FigureType::none, _board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::pawn, _board.at(expectedDestination).figure.getType());
    }

    TEST_F(TestBoard, movePawn_emptyBackward_inValid)
    {
        const Position origin(BoardRow::r2, BoardColumn::cA);
        const Position expectedDestination(BoardRow::r3, BoardColumn::cA);

        createFigure(_board.at(origin), FigureType::pawn, Color::black);

        ASSERT_EQ(FigureType::pawn, _board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::none, _board.at(expectedDestination).figure.getType());

        auto result = _board.move(origin, expectedDestination);
        ASSERT_EQ(MoveResult::invalid, result.moveResult());

        ASSERT_EQ(FigureType::pawn, _board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::none, _board.at(expectedDestination).figure.getType());
    }

    TEST_F(TestBoard, movePawn_empty3Fields_invalid)
    {
        const Position origin(BoardRow::r2, BoardColumn::cA);
        const Position expectedDestination(BoardRow::r5, BoardColumn::cA);

        createFigure(_board.at(origin), FigureType::pawn, Color::white);

        ASSERT_EQ(FigureType::pawn, _board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::none, _board.at(expectedDestination).figure.getType());

        auto result = _board.move(origin, expectedDestination);
        ASSERT_EQ(MoveResult::invalid, result.moveResult());

        ASSERT_EQ(FigureType::pawn, _board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::none, _board.at(expectedDestination).figure.getType());
    }

    TEST_F(TestBoard, movePawn_empty2FieldsFirst_valid)
    {
        const Position origin(BoardRow::r2, BoardColumn::cA);
        const Position expectedDestination(BoardRow::r4, BoardColumn::cA);

        createFigure(_board.at(origin), FigureType::pawn, Color::white);

        ASSERT_EQ(FigureType::pawn, _board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::none, _board.at(expectedDestination).figure.getType());

        auto result = _board.move(origin, expectedDestination);
        ASSERT_EQ(MoveResult::valid, result.moveResult());

        ASSERT_EQ(expectedDestination, _board.at(expectedDestination).figure.getCurrentPosition());
        ASSERT_EQ(origin, _board.at(expectedDestination).figure.getPreviousPosition());

        ASSERT_EQ(FigureType::none, _board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::pawn, _board.at(expectedDestination).figure.getType());
    }

    TEST_F(TestBoard, movePawn_enPassantToManyMoves_invalid)
    {
        const Position originWhite(BoardRow::r4, BoardColumn::cF);
        const Position originBlack(BoardRow::r7, BoardColumn::cG);

        const Position originWhite2(BoardRow::r4, BoardColumn::cA);
        const Position originBlack2(BoardRow::r7, BoardColumn::cA);

        const Position destinationWhite1(BoardRow::r5, BoardColumn::cF);
        const Position destinationWhite2(BoardRow::r5, BoardColumn::cA);
        const Position destinationBlack1(BoardRow::r5, BoardColumn::cG);
        const Position destinationBlack2(BoardRow::r6, BoardColumn::cA);
        const Position destinationWhite3(BoardRow::r6, BoardColumn::cG);

        createFigure(_board.at(originWhite), FigureType::pawn, Color::white);
        createFigure(_board.at(originWhite2), FigureType::pawn, Color::white);
        createFigure(_board.at(originBlack), FigureType::pawn, Color::black);
        createFigure(_board.at(originBlack2), FigureType::pawn, Color::black);

        ASSERT_EQ(MoveResult::valid, _board.move(originWhite, destinationWhite1).moveResult());
        ASSERT_EQ(MoveResult::valid, _board.move(originBlack, destinationBlack1).moveResult());

        ASSERT_EQ(MoveResult::valid, _board.move(originWhite2, destinationWhite2).moveResult());
        ASSERT_EQ(MoveResult::valid, _board.move(originBlack2, destinationBlack2).moveResult());

        auto result = _board.move(destinationWhite1, destinationWhite3);
        ASSERT_EQ(MoveResult::invalid, result.moveResult());
        ASSERT_FALSE(result.hasFlag(EventFlag::capture));

        ASSERT_EQ(FigureType::pawn, _board.at(destinationBlack1).figure.getType());
        ASSERT_EQ(FigureType::pawn, _board.at(destinationWhite2).figure.getType());
    }

    TEST_F(TestBoard, movePawn_enPassant_valid)
    {
        const Position originWhite(BoardRow::r4, BoardColumn::cF);
        const Position originBlack(BoardRow::r7, BoardColumn::cG);
        const Position destinationWhite1(BoardRow::r5, BoardColumn::cF);
        const Position destinationBlack1(BoardRow::r5, BoardColumn::cG);
        const Position destinationWhite2(BoardRow::r6, BoardColumn::cG);

        createFigure(_board.at(originWhite), FigureType::pawn, Color::white);
        createFigure(_board.at(originBlack), FigureType::pawn, Color::black);

        ASSERT_EQ(MoveResult::valid, _board.move(originWhite, destinationWhite1).moveResult());
        ASSERT_EQ(MoveResult::valid, _board.move(originBlack, destinationBlack1).moveResult());

        auto result = _board.move(destinationWhite1, destinationWhite2);
        ASSERT_EQ(MoveResult::valid, result.moveResult());
        ASSERT_TRUE(result.hasFlag(EventFlag::capture));

        ASSERT_EQ(FigureType::none, _board.at(destinationBlack1).figure.getType());
        ASSERT_EQ(FigureType::pawn, _board.at(destinationWhite2).figure.getType());
    }

    TEST_F(TestBoard, movePawn_emptyAndEnd_promotion)
    {
        const Position origin(BoardRow::r7, BoardColumn::cA);
        const Position expectedDestination(BoardRow::r8, BoardColumn::cA);

        createFigure(_board.at(origin), FigureType::pawn, Color::white);

        ASSERT_EQ(FigureType::pawn, _board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::none, _board.at(expectedDestination).figure.getType());

        auto result = _board.move(origin, expectedDestination);
        ASSERT_EQ(MoveResult::valid, result.moveResult());
        ASSERT_TRUE(result.hasFlag(EventFlag::promotion));

        ASSERT_EQ(FigureType::none, _board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::pawn, _board.at(expectedDestination).figure.getType());
    }

    TEST_F(TestBoard, allowedMove_enPassant_validButNotchanged)
    {
        const Position originWhite(BoardRow::r4, BoardColumn::cF);
        const Position originBlack(BoardRow::r7, BoardColumn::cG);
        const Position destinationWhite1(BoardRow::r5, BoardColumn::cF);
        const Position destinationBlack1(BoardRow::r5, BoardColumn::cG);
        const Position destinationWhite2(BoardRow::r6, BoardColumn::cG);

        createFigure(_board.at(originWhite), FigureType::pawn, Color::white);
        createFigure(_board.at(originBlack), FigureType::pawn, Color::black);

        ASSERT_EQ(MoveResult::valid, _board.move(originWhite, destinationWhite1).moveResult());
        ASSERT_EQ(MoveResult::valid, _board.move(originBlack, destinationBlack1).moveResult());

        auto result = _board.allowed(destinationWhite1, destinationWhite2);
        ASSERT_EQ(MoveResult::valid, result.moveResult());
        ASSERT_TRUE(result.hasFlag(EventFlag::capture));

        ASSERT_EQ(FigureType::pawn, _board.at(destinationBlack1).figure.getType());
        ASSERT_EQ(FigureType::pawn, _board.at(destinationWhite1).figure.getType());
        ASSERT_EQ(FigureType::none, _board.at(destinationWhite2).figure.getType());
    }

    TEST_F(TestBoard, movePawnCapture_notEmptyOwnColor_invalid)
    {
        const Position origin(BoardRow::r2, BoardColumn::cA);
        const Position expectedDestination(BoardRow::r3, BoardColumn::cB);

        createFigure(_board.at(origin), FigureType::pawn, Color::white);
        createFigure(_board.at(expectedDestination), FigureType::pawn, Color::white);

        ASSERT_EQ(FigureType::pawn, _board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::pawn, _board.at(expectedDestination).figure.getType());

        auto result = _board.move(origin, expectedDestination);
        ASSERT_EQ(MoveResult::invalid, result.moveResult());

        ASSERT_EQ(FigureType::pawn, _board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::pawn, _board.at(expectedDestination).figure.getType());
    }

    TEST_P(TestBoardRelationalMove, moveKing_inEachDirection_valid)
    {
        const Position origin(BoardRow::r4, BoardColumn::cD);
        const auto     target = origin + GetParam();

        ASSERT_TRUE(target.isValid());

        createFigure(_board.at(origin), FigureType::king, Color::white);

        ASSERT_EQ(FigureType::king, _board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::none, _board.at(target).figure.getType());

        ASSERT_EQ(MoveResult::valid, _board.move(origin, target).moveResult());

        ASSERT_EQ(FigureType::none, _board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::king, _board.at(target).figure.getType());
    }

    INSTANTIATE_TEST_SUITE_P(moveKing_inEachDirection, TestBoardRelationalMove, ::testing::Values(
                                 Position(1,0),
                                 Position(1,1),
                                 Position(1,-1),
                                 Position(-1,0),
                                 Position(-1,1),
                                 Position(-1,-1),
                                 Position(0,1),
                                 Position(0,-1)
                             ));

    TEST_F(TestBoard, moveKing_outOfBoard_inValid)
    {
        const Position origin(BoardRow::r1, BoardColumn::cD);
        const auto     target = origin - Position(1, 1);

        ASSERT_FALSE(target.isValid());

        createFigure(_board.at(origin), FigureType::king, Color::white);
        ASSERT_EQ(MoveResult::invalid, _board.move(origin, target).moveResult());
        ASSERT_EQ(FigureType::king, _board.at(origin).figure.getType());
    }

    TEST_F(TestBoard, moveKing_2FieldsUp_inValid)
    {
        const Position origin(BoardRow::r1, BoardColumn::cD);
        const auto     target = origin + Position(2, 0);

        ASSERT_TRUE(target.isValid());

        createFigure(_board.at(origin), FigureType::king, Color::white);
        ASSERT_EQ(MoveResult::invalid, _board.move(origin, target).moveResult());
        ASSERT_EQ(FigureType::none, _board.at(target).figure.getType());
        ASSERT_EQ(FigureType::king, _board.at(origin).figure.getType());
    }

    TEST_F(TestBoard, moveKing_captureEnemie_capture)
    {
        const Position origin(BoardRow::r1, BoardColumn::cD);
        const Position target(BoardRow::r2, BoardColumn::cD);

        createFigure(_board.at(origin), FigureType::king, Color::white);
        createFigure(_board.at(target), FigureType::pawn, Color::black);

        auto result = _board.move(origin, target);
        ASSERT_EQ(MoveResult::valid, result.moveResult());
        ASSERT_TRUE(result.hasFlag(EventFlag::capture));

        ASSERT_EQ(FigureType::king, _board.at(target).figure.getType());
        ASSERT_EQ(FigureType::none, _board.at(origin).figure.getType());
    }

    TEST_F(TestBoard, moveKing_moveInChess_invalid)
    {
        const Position origin(BoardRow::r1, BoardColumn::cD);
        const Position target(BoardRow::r1, BoardColumn::cE);
        const Position attacker(BoardRow::r2, BoardColumn::cD);

        createFigure(_board.at(origin), FigureType::king, Color::white);
        createFigure(_board.at(attacker), FigureType::pawn, Color::black);

        ASSERT_EQ(MoveResult::invalid, _board.move(origin, target).moveResult());
    }

    TEST_F(TestBoard, moveKing_castling_valid)
    {
        const Position origin(BoardRow::r1, BoardColumn::cE);
        const Position target(BoardRow::r1, BoardColumn::cG);

        createFigure(_board.at(origin), FigureType::king, Color::white);
        ASSERT_EQ(MoveResult::invalid, _board.move(origin, target).moveResult());

        createFigure(_board.at(BoardRow::r1, BoardColumn::cH), FigureType::rook, Color::white);

        auto result = _board.move(origin, target);
        ASSERT_EQ(MoveResult::valid, result.moveResult());
        ASSERT_TRUE(result.hasFlag(EventFlag::castling));

        ASSERT_EQ(FigureType::king, _board.at(target).figure.getType());
        ASSERT_EQ(FigureType::rook, _board.at(target - Position(0,1)).figure.getType());
    }

    TEST_F(TestBoard, moveKing_castlingChessBetween_invalid)
    {
        const Position origin(BoardRow::r1, BoardColumn::cE);
        const Position target(BoardRow::r1, BoardColumn::cG);
        const Position attacker(BoardRow::r2, BoardColumn::cD);

        createFigure(_board.at(origin), FigureType::king, Color::white);
        createFigure(_board.at(BoardRow::r1, BoardColumn::cH), FigureType::rook, Color::white);
        createFigure(_board.at(attacker), FigureType::pawn, Color::black);

        ASSERT_EQ(MoveResult::invalid, _board.move(origin, target).moveResult());
    }

    TEST_P(TestBoardRankFieldMove, move_emptyFieldDownToTop_valid)
    {
        const Position origin(BoardRow::r1, BoardColumn::cE);
        const Position target(BoardRow::r8, BoardColumn::cE);

        createFigure(_board.at(origin), GetParam(), Color::white);
        ASSERT_EQ(MoveResult::valid, _board.move(origin, target).moveResult());

        ASSERT_EQ(FigureType::none, _board.at(origin).figure.getType());
        ASSERT_EQ(GetParam(), _board.at(target).figure.getType());
    }

    TEST_P(TestBoardRankFieldMove, move_enemieDownToTop_capture)
    {
        const Position origin(BoardRow::r1, BoardColumn::cE);
        const Position target(BoardRow::r8, BoardColumn::cE);

        createFigure(_board.at(origin), GetParam(), Color::white);
        createFigure(_board.at(target), FigureType::pawn, Color::black);

        auto result = _board.move(origin, target);
        ASSERT_EQ(MoveResult::valid, result.moveResult());
        ASSERT_TRUE(result.hasFlag(EventFlag::capture));

        ASSERT_EQ(FigureType::none, _board.at(origin).figure.getType());
        ASSERT_EQ(GetParam(), _board.at(target).figure.getType());
    }

    TEST_P(TestBoardRankFieldMove, move_friendDownToTop_invalid)
    {
        const Position origin(BoardRow::r1, BoardColumn::cE);
        const Position target(BoardRow::r8, BoardColumn::cE);

        createFigure(_board.at(origin), GetParam(), Color::white);
        createFigure(_board.at(target), GetParam(), Color::white);
        ASSERT_EQ(MoveResult::invalid, _board.move(origin, target).moveResult());

        ASSERT_EQ(GetParam(), _board.at(origin).figure.getType());
        ASSERT_EQ(GetParam(), _board.at(target).figure.getType());
    }

    TEST_P(TestBoardRankFieldMove, move_enemieInBetweenDownToTop_invalid)
    {
        const Position origin(BoardRow::r1, BoardColumn::cE);
        const Position target(BoardRow::r8, BoardColumn::cE);
        const Position blocker(BoardRow::r4, BoardColumn::cE);

        createFigure(_board.at(origin), GetParam(), Color::white);
        createFigure(_board.at(blocker), GetParam(), Color::black);
        ASSERT_EQ(MoveResult::invalid, _board.move(origin, target).moveResult());

        ASSERT_EQ(GetParam(), _board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::none, _board.at(target).figure.getType());
    }

    TEST_P(TestBoardRankFieldMove, move_enemieInBetweenTopToDown_invalid)
    {
        const Position origin(BoardRow::r8, BoardColumn::cE);
        const Position target(BoardRow::r1, BoardColumn::cE);
        const Position blocker(BoardRow::r4, BoardColumn::cE);

        createFigure(_board.at(origin), GetParam(), Color::white);
        createFigure(_board.at(blocker), GetParam(), Color::black);
        ASSERT_EQ(MoveResult::invalid, _board.move(origin, target).moveResult());

        ASSERT_EQ(GetParam(), _board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::none, _board.at(target).figure.getType());
    }

    TEST_P(TestBoardRankFieldMove, move_enemieInBetweenLeftToRight_invalid)
    {
        const Position origin(BoardRow::r8, BoardColumn::cA);
        const Position target(BoardRow::r8, BoardColumn::cG);
        const Position blocker(BoardRow::r8, BoardColumn::cF);

        createFigure(_board.at(origin), GetParam(), Color::white);
        createFigure(_board.at(blocker), GetParam(), Color::black);
        ASSERT_EQ(MoveResult::invalid, _board.move(origin, target).moveResult());

        ASSERT_EQ(GetParam(), _board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::none, _board.at(target).figure.getType());
    }

    TEST_P(TestBoardRankFieldMove, move_friendInBetweenRightToLeft_invalid)
    {
        const Position origin(BoardRow::r8, BoardColumn::cC);
        const Position target(BoardRow::r8, BoardColumn::cA);
        const Position blocker(BoardRow::r8, BoardColumn::cB);

        createFigure(_board.at(origin), GetParam(), Color::white);
        createFigure(_board.at(blocker), GetParam(), Color::white);
        ASSERT_EQ(MoveResult::invalid, _board.move(origin, target).moveResult());

        ASSERT_EQ(GetParam(), _board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::none, _board.at(target).figure.getType());
    }

    TEST_P(TestBoardRankFieldMove, move_friendInBetweenDownToTop_invalid)
    {
        const Position origin(BoardRow::r1, BoardColumn::cE);
        const Position target(BoardRow::r8, BoardColumn::cE);
        const Position blocker(BoardRow::r4, BoardColumn::cE);

        createFigure(_board.at(origin), GetParam(), Color::white);
        createFigure(_board.at(blocker), GetParam(), Color::white);
        ASSERT_EQ(MoveResult::invalid, _board.move(origin, target).moveResult());

        ASSERT_EQ(GetParam(), _board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::none, _board.at(target).figure.getType());
    }

    TEST_P(TestBoardRankFieldMove, move_emptyLeftToRight_valid)
    {
        const Position origin(BoardRow::r8, BoardColumn::cA);
        const Position target(BoardRow::r8, BoardColumn::cG);

        createFigure(_board.at(origin), GetParam(), Color::white);
        ASSERT_EQ(MoveResult::valid, _board.move(origin, target).moveResult());

        ASSERT_EQ(FigureType::none, _board.at(origin).figure.getType());
        ASSERT_EQ(GetParam(), _board.at(target).figure.getType());
    }

    INSTANTIATE_TEST_SUITE_P(move_RankAndField, TestBoardRankFieldMove, ::testing::Values(
                                 FigureType::rook, FigureType::queen
                             ));

    TEST_P(TestBoardDiagonalMove, move_diagonalEmpty_valid)
    {
        const auto testFigureType = GetParam();
        const auto origin         = Position(BoardRow::r4, BoardColumn::cD);
        const auto posSigns       = {-1, 1};

        for (auto&& rowSign : posSigns)
        {
            for (auto&& colSign : posSigns)
            {
                int      i      = 1;
                Position target = origin + Position(rowSign, colSign);

                do
                {
                    renewBoard();
                    createFigure(_board.at(origin), testFigureType, Color::white);

                    ASSERT_EQ(MoveResult::valid, _board.move(origin, target).moveResult());
                    ASSERT_EQ(testFigureType, _board.at(target).figure.getType());
                    ASSERT_EQ(FigureType::none, _board.at(origin).figure.getType());

                    ++i;
                    target = origin + Position(i * rowSign, i * colSign);
                }
                while (target.isValid());
            }
        }
    }

    TEST_P(TestBoardDiagonalMove, move_diagonalFriendInbetween_invalid)
    {
        const auto testFigureType = GetParam();
        const auto origin         = Position(BoardRow::r4, BoardColumn::cD);
        const auto target         = Position(BoardRow::r1, BoardColumn::cA);
        const auto blocker        = Position(BoardRow::r2, BoardColumn::cB);

        createFigure(_board.at(origin), testFigureType, Color::white);
        createFigure(_board.at(blocker), FigureType::pawn, Color::white);
        ASSERT_EQ(MoveResult::invalid, _board.move(origin, target).moveResult());
    }

    TEST_P(TestBoardDiagonalMove, move_diagonalEnemieOnTarget_capture)
    {
        const auto testFigureType = GetParam();
        const auto origin         = Position(BoardRow::r4, BoardColumn::cD);
        const auto target         = Position(BoardRow::r1, BoardColumn::cA);

        createFigure(_board.at(origin), testFigureType, Color::white);
        createFigure(_board.at(target), FigureType::pawn, Color::black);

        auto result = _board.move(origin, target);
        ASSERT_EQ(MoveResult::valid, result.moveResult());
        ASSERT_TRUE(result.hasFlag(EventFlag::capture));
    }

    TEST_P(TestBoardDiagonalMove, move_diagonalFriendOnTarget_invalid)
    {
        const auto testFigureType = GetParam();
        const auto origin         = Position(BoardRow::r4, BoardColumn::cD);
        const auto target         = Position(BoardRow::r1, BoardColumn::cA);

        createFigure(_board.at(origin), testFigureType, Color::white);
        createFigure(_board.at(target), FigureType::pawn, Color::white);
        ASSERT_EQ(MoveResult::invalid, _board.move(origin, target).moveResult());
    }

    INSTANTIATE_TEST_SUITE_P(move_RankAndField, TestBoardDiagonalMove, ::testing::Values(
                                 FigureType::bishop, FigureType::queen
                             ));

    TEST_F(TestBoard, moveRook_emptyDiagonal_invalid)
    {
        const Position origin(BoardRow::r8, BoardColumn::cH);
        const Position target(BoardRow::r7, BoardColumn::cG);

        createFigure(_board.at(origin), FigureType::rook, Color::white);
        ASSERT_EQ(MoveResult::invalid, _board.move(origin, target).moveResult());

        ASSERT_EQ(FigureType::rook, _board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::none, _board.at(target).figure.getType());
    }

    TEST_F(TestBoard, moveBishop_FieldAndRank_invalid)
    {
        const Position origin(BoardRow::r8, BoardColumn::cA);
        const Position target(BoardRow::r8, BoardColumn::cG);

        createFigure(_board.at(origin), FigureType::bishop, Color::white);
        ASSERT_EQ(MoveResult::invalid, _board.move(origin, target).moveResult());

        ASSERT_EQ(FigureType::bishop, _board.at(origin).figure.getType());
    }

    TEST_F(TestBoard, moveKnight_validLs_valid)
    {
        const Position origin(BoardRow::r4, BoardColumn::cD);
        const auto     distances = {
            Position(2, 1),
            Position(2, -1),
            Position(1, 2),
            Position(1, -2),
            Position(-2, 1),
            Position(-2, -1),
            Position(-1, 2),
            Position(-1, -2)
        };

        for (auto&& distance : distances)
        {
            const auto target = origin + distance;
            ASSERT_TRUE(target.isValid());

            renewBoard();
            createFigure(_board.at(origin), FigureType::knight, Color::white);
            ASSERT_EQ(MoveResult::valid, _board.move(origin, target).moveResult());
        }
    }

    TEST_F(TestBoard, moveKnight_outOfField_invalid)
    {
        const Position origin(BoardRow::r8, BoardColumn::cD);

        createFigure(_board.at(origin), FigureType::knight, Color::white);
        ASSERT_EQ(MoveResult::invalid, _board.move(origin, origin + Position(2,1)).moveResult());
    }

    TEST_F(TestBoard, moveKnight_toFriend_invalid)
    {
        const Position origin(BoardRow::r4, BoardColumn::cD);
        const Position target(BoardRow::r6, BoardColumn::cC);

        createFigure(_board.at(origin), FigureType::knight, Color::white);
        createFigure(_board.at(target), FigureType::pawn, Color::white);
        ASSERT_EQ(MoveResult::invalid, _board.move(origin, target).moveResult());
    }

    TEST_F(TestBoard, moveKnight_toEnemie_capture)
    {
        const Position origin(BoardRow::r4, BoardColumn::cD);
        const Position target(BoardRow::r6, BoardColumn::cC);

        createFigure(_board.at(origin), FigureType::knight, Color::white);
        createFigure(_board.at(target), FigureType::pawn, Color::black);

        auto result = _board.move(origin, target);
        ASSERT_EQ(MoveResult::valid, result.moveResult());
        ASSERT_TRUE(result.hasFlag(EventFlag::capture));
    }

    TEST_F(TestBoard, moveKnight_withFriendBeteween_valid)
    {
        const Position origin(BoardRow::r4, BoardColumn::cD);
        const Position target(BoardRow::r6, BoardColumn::cC);
        const Position nonBlocker(BoardRow::r5, BoardColumn::cD);

        createFigure(_board.at(origin), FigureType::knight, Color::white);
        createFigure(_board.at(nonBlocker), FigureType::pawn, Color::white);
        ASSERT_EQ(MoveResult::valid, _board.move(origin, target).moveResult());
    }

    TEST_F(TestBoard, checkVictory_kingCanMove_none)
    {
        const Position whiteKingPos(BoardRow::r1, BoardColumn::cA);
        const Position whiteKingDes(BoardRow::r1, BoardColumn::cB);
        const Position blackKingPos(BoardRow::r8, BoardColumn::cH);
        const Position blackKingDes(BoardRow::r8, BoardColumn::cG);

        createFigure(_board.at(whiteKingPos), FigureType::king, Color::white);
        createFigure(_board.at(blackKingPos), FigureType::king, Color::black);

        // King has to move to actualize Board pointers for king (only on non standard boards)
        ASSERT_EQ(MoveResult::valid, _board.move(whiteKingPos, whiteKingDes).moveResult());
        ASSERT_EQ(MoveResult::valid, _board.move(blackKingPos, blackKingDes).moveResult());

        ASSERT_EQ(GameResult::none, _board.checkVictory());
        ASSERT_FALSE(_board.hasEnded());
    }

    TEST_F(TestBoard, checkVictory_kingCanNotMoveAndChess_victoryWhite)
    {
        const Position whiteKingPos(BoardRow::r1, BoardColumn::cB);
        const Position whiteKingDes(BoardRow::r1, BoardColumn::cA);
        const Position blackKingPos(BoardRow::r8, BoardColumn::cG);
        const Position blackKingDes(BoardRow::r8, BoardColumn::cH);
        const Position rook1Pos(BoardRow::r1, BoardColumn::cH);
        const Position rook2Pos(BoardRow::r1, BoardColumn::cG);

        createFigure(_board.at(whiteKingPos), FigureType::king, Color::white);
        createFigure(_board.at(blackKingPos), FigureType::king, Color::black);

        // King has to move to actualize Board pointers for king (only on non standard boards)
        ASSERT_EQ(MoveResult::valid, _board.move(whiteKingPos, whiteKingDes).moveResult());
        ASSERT_EQ(MoveResult::valid, _board.move(blackKingPos, blackKingDes).moveResult());
        ASSERT_EQ(MoveResult::valid, _board.move(whiteKingDes, whiteKingPos).moveResult());

        createFigure(_board.at(rook1Pos), FigureType::rook, Color::white);
        createFigure(_board.at(rook2Pos), FigureType::rook, Color::white);

        ASSERT_EQ(GameResult::victoryWhite, _board.checkVictory());
        ASSERT_TRUE(_board.hasEnded());
    }

    TEST_F(TestBoard, checkVictory_kingCanNotMoveAndChess_victoryBlack)
    {
        const Position whiteKingPos(BoardRow::r1, BoardColumn::cA);
        const Position rook1Pos(BoardRow::r8, BoardColumn::cA);
        const Position rook2Pos(BoardRow::r8, BoardColumn::cB);

        createFigure(_board.at(whiteKingPos), FigureType::king, Color::white);
        createFigure(_board.at(rook1Pos), FigureType::rook, Color::black);
        createFigure(_board.at(rook2Pos), FigureType::rook, Color::black);

        ASSERT_EQ(GameResult::victoryBlack, _board.checkVictory());
        ASSERT_TRUE(_board.hasEnded());
    }

    TEST_F(TestBoard, checkVictory_kingCanNotMoveAndNoChess_draw)
    {
        const Position blackKingPos(BoardRow::r8, BoardColumn::cH);
        const Position rook1Pos(BoardRow::r7, BoardColumn::cA);
        const Position rook2Pos(BoardRow::r1, BoardColumn::cA);
        const Position rook2Dest(BoardRow::r1, BoardColumn::cG);

        createFigure(_board.at(blackKingPos), FigureType::king, Color::black);
        createFigure(_board.at(rook1Pos), FigureType::rook, Color::white);
        createFigure(_board.at(rook2Pos), FigureType::rook, Color::white);

        ASSERT_EQ(MoveResult::valid, _board.move(rook2Pos, rook2Dest).moveResult());
        ASSERT_EQ(GameResult::draw, _board.checkVictory());
        ASSERT_TRUE(_board.hasEnded());
    }
}
