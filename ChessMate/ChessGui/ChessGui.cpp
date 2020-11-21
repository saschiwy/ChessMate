#include <QtWidgets>
#include "ui/ChessWindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    ChessWindow  board;
    board.show();
    board.resize();
    board.setBoard(std::make_shared<ChessNS::Board>());
    return QApplication::exec();
}
