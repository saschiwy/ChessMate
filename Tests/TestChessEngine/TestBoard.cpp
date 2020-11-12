#include "gtest/gtest.h"
#include "ChessEngine/Board.h"

namespace ChessNS
{
    class TestBoard : public ::testing::Test
    {
    public:
        TestBoard() = default;

        virtual ~TestBoard() = default;

        static void createFigure(Field& field, FigureType figureType, Color color)
        {
            Board::createFigure(field, figureType, color);
        }
    };

    class TestBoardRelationalMove : public ::testing::TestWithParam<Position>
    {
    protected:
        static void createFigure(Field& field, FigureType figureType, Color color)
        {
            TestBoard::createFigure(field, figureType, color);
        }
    };

    class TestBoardDiagonalMove : public ::testing::TestWithParam<FigureType>
    {
    protected:
        static void createFigure(Field& field, FigureType figureType, Color color)
        {
            TestBoard::createFigure(field, figureType, color);
        }
    };

    class TestBoardRankFieldMove : public ::testing::TestWithParam<FigureType>
    {
    protected:
        static void createFigure(Field& field, FigureType figureType, Color color)
        {
            TestBoard::createFigure(field, figureType, color);
        }
    };

    TEST_F(TestBoard, movePawn_empty_valid)
    {
        const Position origin(BoardRow::r2, BoardColumn::cA);
        const Position expectedDestination(BoardRow::r3, BoardColumn::cA);

        Board board{};
        createFigure(board.at(origin), FigureType::pawn, Color::white);

        ASSERT_EQ(FigureType::pawn, board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::none, board.at(expectedDestination).figure.getType());

        ASSERT_EQ(MoveResult::valid, board.move(origin, expectedDestination));

        ASSERT_EQ(FigureType::none, board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::pawn, board.at(expectedDestination).figure.getType());
    }

    TEST_F(TestBoard, movePawnFoward_notEmpty_invalid)
    {
        const Position origin(BoardRow::r2, BoardColumn::cA);
        const Position expectedDestination(BoardRow::r3, BoardColumn::cA);

        Board board{};
        createFigure(board.at(origin), FigureType::pawn, Color::white);
        createFigure(board.at(expectedDestination), FigureType::pawn, Color::black);

        ASSERT_EQ(FigureType::pawn, board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::pawn, board.at(expectedDestination).figure.getType());

        ASSERT_EQ(MoveResult::invalid, board.move(origin, expectedDestination));

        ASSERT_EQ(FigureType::pawn, board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::pawn, board.at(expectedDestination).figure.getType());
    }

    TEST_F(TestBoard, movePawnCapture_notEmpty_captured)
    {
        const Position origin(BoardRow::r2, BoardColumn::cA);
        const Position expectedDestination(BoardRow::r3, BoardColumn::cB);

        Board board{};
        createFigure(board.at(origin), FigureType::pawn, Color::white);
        createFigure(board.at(expectedDestination), FigureType::pawn, Color::black);

        ASSERT_EQ(FigureType::pawn, board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::pawn, board.at(expectedDestination).figure.getType());

        ASSERT_EQ(MoveResult::capture, board.move(origin, expectedDestination));

        ASSERT_EQ(FigureType::none, board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::pawn, board.at(expectedDestination).figure.getType());
    }

    TEST_F(TestBoard, movePawn_emptyBackward_inValid)
    {
        const Position origin(BoardRow::r2, BoardColumn::cA);
        const Position expectedDestination(BoardRow::r3, BoardColumn::cA);

        Board board{};
        createFigure(board.at(origin), FigureType::pawn, Color::black);

        ASSERT_EQ(FigureType::pawn, board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::none, board.at(expectedDestination).figure.getType());

        ASSERT_EQ(MoveResult::invalid, board.move(origin, expectedDestination));

        ASSERT_EQ(FigureType::pawn, board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::none, board.at(expectedDestination).figure.getType());
    }

    TEST_F(TestBoard, movePawn_empty3Fields_invalid)
    {
        const Position origin(BoardRow::r2, BoardColumn::cA);
        const Position expectedDestination(BoardRow::r5, BoardColumn::cA);

        Board board{};
        createFigure(board.at(origin), FigureType::pawn, Color::white);

        ASSERT_EQ(FigureType::pawn, board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::none, board.at(expectedDestination).figure.getType());

        ASSERT_EQ(MoveResult::invalid, board.move(origin, expectedDestination));

        ASSERT_EQ(FigureType::pawn, board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::none, board.at(expectedDestination).figure.getType());
    }

    TEST_F(TestBoard, movePawn_empty2FieldsFirst_valid)
    {
        const Position origin(BoardRow::r2, BoardColumn::cA);
        const Position expectedDestination(BoardRow::r4, BoardColumn::cA);

        Board board{};
        createFigure(board.at(origin), FigureType::pawn, Color::white);

        ASSERT_EQ(FigureType::pawn, board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::none, board.at(expectedDestination).figure.getType());

        ASSERT_EQ(MoveResult::valid, board.move(origin, expectedDestination));
        ASSERT_EQ(expectedDestination, board.at(expectedDestination).figure.getCurrentPosition());
        ASSERT_EQ(origin, board.at(expectedDestination).figure.getPreviousPosition());

        ASSERT_EQ(FigureType::none, board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::pawn, board.at(expectedDestination).figure.getType());
    }

    TEST_F(TestBoard, movePawn_enPassantToManyMoves_invalid)
    {
        const Position originWhite(BoardRow::r4, BoardColumn::cF);
        const Position originBlack(BoardRow::r7, BoardColumn::cG);
        const Position destinationWhite1(BoardRow::r5, BoardColumn::cF);
        const Position destinationBlack1(BoardRow::r6, BoardColumn::cG);
        const Position destinationWhite2(BoardRow::r6, BoardColumn::cG);
        const Position destinationBlack2(BoardRow::r5, BoardColumn::cG);

        Board board{};
        createFigure(board.at(BoardRow::r1, BoardColumn::cE), FigureType::pawn, Color::white);
        createFigure(board.at(originWhite), FigureType::pawn, Color::white);
        createFigure(board.at(originBlack), FigureType::pawn, Color::black);

        ASSERT_EQ(MoveResult::valid, board.move(Position(BoardRow::r1, BoardColumn::cE), Position(BoardRow::r2, BoardColumn::cE)));
        ASSERT_EQ(MoveResult::valid, board.move(originBlack, destinationBlack1));
        ASSERT_EQ(MoveResult::valid, board.move(originWhite, destinationWhite1));
        ASSERT_EQ(MoveResult::valid, board.move(destinationBlack1, destinationBlack2));
        ASSERT_EQ(MoveResult::invalid, board.move(destinationWhite1, destinationWhite2));
    }

    TEST_F(TestBoard, movePawn_enPassant_valid)
    {
        const Position originWhite(BoardRow::r4, BoardColumn::cF);
        const Position originBlack(BoardRow::r7, BoardColumn::cG);
        const Position destinationWhite1(BoardRow::r5, BoardColumn::cF);
        const Position destinationBlack1(BoardRow::r5, BoardColumn::cG);
        const Position destinationWhite2(BoardRow::r6, BoardColumn::cG);

        Board board{};
        createFigure(board.at(originWhite), FigureType::pawn, Color::white);
        createFigure(board.at(originBlack), FigureType::pawn, Color::black);

        ASSERT_EQ(MoveResult::valid, board.move(originWhite, destinationWhite1));
        ASSERT_EQ(MoveResult::valid, board.move(originBlack, destinationBlack1));
        ASSERT_EQ(MoveResult::capture, board.move(destinationWhite1, destinationWhite2));

        ASSERT_EQ(FigureType::none, board.at(destinationBlack1).figure.getType());
        ASSERT_EQ(FigureType::pawn, board.at(destinationWhite2).figure.getType());
    }

    TEST_F(TestBoard, movePawn_emptyAndEnd_promotion)
    {
        const Position origin(BoardRow::r7, BoardColumn::cA);
        const Position expectedDestination(BoardRow::r8, BoardColumn::cA);

        Board board{};
        createFigure(board.at(origin), FigureType::pawn, Color::white);

        ASSERT_EQ(FigureType::pawn, board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::none, board.at(expectedDestination).figure.getType());

        ASSERT_EQ(MoveResult::promotion, board.move(origin, expectedDestination));

        ASSERT_EQ(FigureType::none, board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::pawn, board.at(expectedDestination).figure.getType());
    }

    TEST_F(TestBoard, allowedMove_enPassant_validButNotchanged)
    {
        const Position originWhite(BoardRow::r4, BoardColumn::cF);
        const Position originBlack(BoardRow::r7, BoardColumn::cG);
        const Position destinationWhite1(BoardRow::r5, BoardColumn::cF);
        const Position destinationBlack1(BoardRow::r5, BoardColumn::cG);
        const Position destinationWhite2(BoardRow::r6, BoardColumn::cG);

        Board board{};
        createFigure(board.at(originWhite), FigureType::pawn, Color::white);
        createFigure(board.at(originBlack), FigureType::pawn, Color::black);

        ASSERT_EQ(MoveResult::valid, board.move(originWhite, destinationWhite1));
        ASSERT_EQ(MoveResult::valid, board.move(originBlack, destinationBlack1));
        ASSERT_EQ(MoveResult::capture, board.allowed(destinationWhite1, destinationWhite2));

        ASSERT_EQ(FigureType::pawn, board.at(destinationBlack1).figure.getType());
        ASSERT_EQ(FigureType::pawn, board.at(destinationWhite1).figure.getType());
        ASSERT_EQ(FigureType::none, board.at(destinationWhite2).figure.getType());
    }

    TEST_F(TestBoard, movePawnCapture_notEmptyOwnColor_invalid)
    {
        const Position origin(BoardRow::r2, BoardColumn::cA);
        const Position expectedDestination(BoardRow::r3, BoardColumn::cB);

        Board board{};
        createFigure(board.at(origin), FigureType::pawn, Color::white);
        createFigure(board.at(expectedDestination), FigureType::pawn, Color::white);

        ASSERT_EQ(FigureType::pawn, board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::pawn, board.at(expectedDestination).figure.getType());

        ASSERT_EQ(MoveResult::invalid, board.move(origin, expectedDestination));

        ASSERT_EQ(FigureType::pawn, board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::pawn, board.at(expectedDestination).figure.getType());
    }

    TEST_P(TestBoardRelationalMove, moveKing_inEachDirection_valid)
    {
        const Position origin(BoardRow::r4, BoardColumn::cD);
        const auto     target = origin + GetParam();

        ASSERT_TRUE(target.isValid());

        Board board{};
        createFigure(board.at(origin), FigureType::king, Color::white);

        ASSERT_EQ(FigureType::king, board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::none, board.at(target).figure.getType());

        ASSERT_EQ(MoveResult::valid, board.move(origin, target));

        ASSERT_EQ(FigureType::none, board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::king, board.at(target).figure.getType());
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

        Board board{};
        createFigure(board.at(origin), FigureType::king, Color::white);
        ASSERT_EQ(MoveResult::invalid, board.move(origin, target));
        ASSERT_EQ(FigureType::king, board.at(origin).figure.getType());
    }

    TEST_F(TestBoard, moveKing_2FieldsUp_inValid)
    {
        const Position origin(BoardRow::r1, BoardColumn::cD);
        const auto     target = origin + Position(2, 0);

        ASSERT_TRUE(target.isValid());

        Board board{};
        createFigure(board.at(origin), FigureType::king, Color::white);
        ASSERT_EQ(MoveResult::invalid, board.move(origin, target));
        ASSERT_EQ(FigureType::none, board.at(target).figure.getType());
        ASSERT_EQ(FigureType::king, board.at(origin).figure.getType());
    }

    TEST_F(TestBoard, moveKing_captureEnemie_capture)
    {
        const Position origin(BoardRow::r1, BoardColumn::cD);
        const Position target(BoardRow::r2, BoardColumn::cD);

        Board board{};
        createFigure(board.at(origin), FigureType::king, Color::white);
        createFigure(board.at(target), FigureType::pawn, Color::black);

        ASSERT_EQ(MoveResult::capture, board.move(origin, target));
        ASSERT_EQ(FigureType::king, board.at(target).figure.getType());
        ASSERT_EQ(FigureType::none, board.at(origin).figure.getType());
    }

    TEST_F(TestBoard, moveKing_moveInChess_invalid)
    {
        const Position origin(BoardRow::r1, BoardColumn::cD);
        const Position target(BoardRow::r1, BoardColumn::cE);
        const Position attacker(BoardRow::r2, BoardColumn::cD);

        Board board{};
        createFigure(board.at(origin), FigureType::king, Color::white);
        createFigure(board.at(attacker), FigureType::pawn, Color::black);

        ASSERT_EQ(MoveResult::invalid, board.move(origin, target));
    }

    TEST_F(TestBoard, moveKing_castling_valid)
    {
        const Position origin(BoardRow::r1, BoardColumn::cE);
        const Position target(BoardRow::r1, BoardColumn::cG);

        Board board{};
        createFigure(board.at(origin), FigureType::king, Color::white);
        ASSERT_EQ(MoveResult::invalid, board.move(origin, target));

        createFigure(board.at(BoardRow::r1, BoardColumn::cH), FigureType::rook, Color::white);
        ASSERT_EQ(MoveResult::valid, board.move(origin, target));

        ASSERT_EQ(FigureType::king, board.at(target).figure.getType());
        ASSERT_EQ(FigureType::rook, board.at(target - Position(0,1)).figure.getType());
    }

    TEST_F(TestBoard, moveKing_castlingChessBetween_invalid)
    {
        const Position origin(BoardRow::r1, BoardColumn::cE);
        const Position target(BoardRow::r1, BoardColumn::cG);
        const Position attacker(BoardRow::r2, BoardColumn::cD);

        Board board{};
        createFigure(board.at(origin), FigureType::king, Color::white);
        createFigure(board.at(BoardRow::r1, BoardColumn::cH), FigureType::rook, Color::white);
        createFigure(board.at(attacker), FigureType::pawn, Color::black);

        ASSERT_EQ(MoveResult::invalid, board.move(origin, target));
    }

    TEST_P(TestBoardRankFieldMove, move_emptyFieldDownToTop_valid)
    {
        const Position origin(BoardRow::r1, BoardColumn::cE);
        const Position target(BoardRow::r8, BoardColumn::cE);

        Board board{};
        createFigure(board.at(origin), GetParam(), Color::white);
        ASSERT_EQ(MoveResult::valid, board.move(origin, target));

        ASSERT_EQ(FigureType::none, board.at(origin).figure.getType());
        ASSERT_EQ(GetParam(), board.at(target).figure.getType());
    }

    TEST_P(TestBoardRankFieldMove, move_enemieDownToTop_capture)
    {
        const Position origin(BoardRow::r1, BoardColumn::cE);
        const Position target(BoardRow::r8, BoardColumn::cE);

        Board board{};
        createFigure(board.at(origin), GetParam(), Color::white);
        createFigure(board.at(target), FigureType::pawn, Color::black);
        ASSERT_EQ(MoveResult::capture, board.move(origin, target));

        ASSERT_EQ(FigureType::none, board.at(origin).figure.getType());
        ASSERT_EQ(GetParam(), board.at(target).figure.getType());
    }

    TEST_P(TestBoardRankFieldMove, move_friendDownToTop_invalid)
    {
        const Position origin(BoardRow::r1, BoardColumn::cE);
        const Position target(BoardRow::r8, BoardColumn::cE);

        Board board{};
        createFigure(board.at(origin), GetParam(), Color::white);
        createFigure(board.at(target), GetParam(), Color::white);
        ASSERT_EQ(MoveResult::invalid, board.move(origin, target));

        ASSERT_EQ(GetParam(), board.at(origin).figure.getType());
        ASSERT_EQ(GetParam(), board.at(target).figure.getType());
    }

    TEST_P(TestBoardRankFieldMove, move_enemieInBetweenDownToTop_invalid)
    {
        const Position origin(BoardRow::r1, BoardColumn::cE);
        const Position target(BoardRow::r8, BoardColumn::cE);
        const Position blocker(BoardRow::r4, BoardColumn::cE);

        Board board{};
        createFigure(board.at(origin), GetParam(), Color::white);
        createFigure(board.at(blocker), GetParam(), Color::black);
        ASSERT_EQ(MoveResult::invalid, board.move(origin, target));

        ASSERT_EQ(GetParam(), board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::none, board.at(target).figure.getType());
    }

    TEST_P(TestBoardRankFieldMove, move_enemieInBetweenTopToDown_invalid)
    {
        const Position origin(BoardRow::r8, BoardColumn::cE);
        const Position target(BoardRow::r1, BoardColumn::cE);
        const Position blocker(BoardRow::r4, BoardColumn::cE);

        Board board{};
        createFigure(board.at(origin), GetParam(), Color::white);
        createFigure(board.at(blocker), GetParam(), Color::black);
        ASSERT_EQ(MoveResult::invalid, board.move(origin, target));

        ASSERT_EQ(GetParam(), board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::none, board.at(target).figure.getType());
    }

    TEST_P(TestBoardRankFieldMove, move_enemieInBetweenLeftToRight_invalid)
    {
        const Position origin(BoardRow::r8, BoardColumn::cA);
        const Position target(BoardRow::r8, BoardColumn::cG);
        const Position blocker(BoardRow::r8, BoardColumn::cF);

        Board board{};
        createFigure(board.at(origin), GetParam(), Color::white);
        createFigure(board.at(blocker), GetParam(), Color::black);
        ASSERT_EQ(MoveResult::invalid, board.move(origin, target));

        ASSERT_EQ(GetParam(), board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::none, board.at(target).figure.getType());
    }

    TEST_P(TestBoardRankFieldMove, move_friendInBetweenRightToLeft_invalid)
    {
        const Position origin(BoardRow::r8, BoardColumn::cC);
        const Position target(BoardRow::r8, BoardColumn::cA);
        const Position blocker(BoardRow::r8, BoardColumn::cB);

        Board board{};
        createFigure(board.at(origin), GetParam(), Color::white);
        createFigure(board.at(blocker), GetParam(), Color::white);
        ASSERT_EQ(MoveResult::invalid, board.move(origin, target));

        ASSERT_EQ(GetParam(), board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::none, board.at(target).figure.getType());
    }

    TEST_P(TestBoardRankFieldMove, move_friendInBetweenDownToTop_invalid)
    {
        const Position origin(BoardRow::r1, BoardColumn::cE);
        const Position target(BoardRow::r8, BoardColumn::cE);
        const Position blocker(BoardRow::r4, BoardColumn::cE);

        Board board{};
        createFigure(board.at(origin), GetParam(), Color::white);
        createFigure(board.at(blocker), GetParam(), Color::white);
        ASSERT_EQ(MoveResult::invalid, board.move(origin, target));

        ASSERT_EQ(GetParam(), board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::none, board.at(target).figure.getType());
    }

    TEST_P(TestBoardRankFieldMove, move_emptyLeftToRight_valid)
    {
        const Position origin(BoardRow::r8, BoardColumn::cA);
        const Position target(BoardRow::r8, BoardColumn::cG);

        Board board{};
        createFigure(board.at(origin), GetParam(), Color::white);
        ASSERT_EQ(MoveResult::valid, board.move(origin, target));

        ASSERT_EQ(FigureType::none, board.at(origin).figure.getType());
        ASSERT_EQ(GetParam(), board.at(target).figure.getType());
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
                    Board board{};
                    createFigure(board.at(origin), testFigureType, Color::white);

                    ASSERT_EQ(MoveResult::valid, board.move(origin, target));
                    ASSERT_EQ(testFigureType, board.at(target).figure.getType());
                    ASSERT_EQ(FigureType::none, board.at(origin).figure.getType());

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

        Board board{};
        createFigure(board.at(origin), testFigureType, Color::white);
        createFigure(board.at(blocker), FigureType::pawn, Color::white);
        ASSERT_EQ(MoveResult::invalid, board.move(origin, target));
    }

    TEST_P(TestBoardDiagonalMove, move_diagonalEnemieOnTarget_capture)
    {
        const auto testFigureType = GetParam();
        const auto origin         = Position(BoardRow::r4, BoardColumn::cD);
        const auto target         = Position(BoardRow::r1, BoardColumn::cA);

        Board board{};
        createFigure(board.at(origin), testFigureType, Color::white);
        createFigure(board.at(target), FigureType::pawn, Color::black);
        ASSERT_EQ(MoveResult::capture, board.move(origin, target));
    }

    TEST_P(TestBoardDiagonalMove, move_diagonalFriendOnTarget_invalid)
    {
        const auto testFigureType = GetParam();
        const auto origin         = Position(BoardRow::r4, BoardColumn::cD);
        const auto target         = Position(BoardRow::r1, BoardColumn::cA);

        Board board{};
        createFigure(board.at(origin), testFigureType, Color::white);
        createFigure(board.at(target), FigureType::pawn, Color::white);
        ASSERT_EQ(MoveResult::invalid, board.move(origin, target));
    }

    INSTANTIATE_TEST_SUITE_P(move_RankAndField, TestBoardDiagonalMove, ::testing::Values(
                                 FigureType::bishop, FigureType::queen
                             ));

    TEST_F(TestBoard, moveRook_emptyDiagonal_invalid)
    {
        const Position origin(BoardRow::r8, BoardColumn::cH);
        const Position target(BoardRow::r7, BoardColumn::cG);

        Board board{};
        createFigure(board.at(origin), FigureType::rook, Color::white);
        ASSERT_EQ(MoveResult::invalid, board.move(origin, target));

        ASSERT_EQ(FigureType::rook, board.at(origin).figure.getType());
        ASSERT_EQ(FigureType::none, board.at(target).figure.getType());
    }

    TEST_F(TestBoard, moveBishop_FieldAndRank_invalid)
    {
        const Position origin(BoardRow::r8, BoardColumn::cA);
        const Position target(BoardRow::r8, BoardColumn::cG);

        Board board{};
        createFigure(board.at(origin), FigureType::bishop, Color::white);
        ASSERT_EQ(MoveResult::invalid, board.move(origin, target));

        ASSERT_EQ(FigureType::bishop, board.at(origin).figure.getType());
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

            Board board{};
            createFigure(board.at(origin), FigureType::knight, Color::white);
            ASSERT_EQ(MoveResult::valid, board.move(origin, target));
        }
    }

    TEST_F(TestBoard, moveKnight_outOfField_invalid)
    {
        const Position origin(BoardRow::r8, BoardColumn::cD);

        Board board{};
        createFigure(board.at(origin), FigureType::knight, Color::white);
        ASSERT_EQ(MoveResult::invalid, board.move(origin, origin + Position(2,1)));
    }

    TEST_F(TestBoard, moveKnight_toFriend_invalid)
    {
        const Position origin(BoardRow::r4, BoardColumn::cD);
        const Position target(BoardRow::r6, BoardColumn::cC);

        Board board{};
        createFigure(board.at(origin), FigureType::knight, Color::white);
        createFigure(board.at(target), FigureType::pawn, Color::white);
        ASSERT_EQ(MoveResult::invalid, board.move(origin, target));
    }

    TEST_F(TestBoard, moveKnight_toEnemie_capture)
    {
        const Position origin(BoardRow::r4, BoardColumn::cD);
        const Position target(BoardRow::r6, BoardColumn::cC);

        Board board{};
        createFigure(board.at(origin), FigureType::knight, Color::white);
        createFigure(board.at(target), FigureType::pawn, Color::black);
        ASSERT_EQ(MoveResult::capture, board.move(origin, target));
    }

    TEST_F(TestBoard, moveKnight_withFriendBeteween_valid)
    {
        const Position origin(BoardRow::r4, BoardColumn::cD);
        const Position target(BoardRow::r6, BoardColumn::cC);
        const Position nonBlocker(BoardRow::r5, BoardColumn::cD);

        Board board{};
        createFigure(board.at(origin), FigureType::knight, Color::white);
        createFigure(board.at(nonBlocker), FigureType::pawn, Color::white);
        ASSERT_EQ(MoveResult::valid, board.move(origin, target));
    }
}
