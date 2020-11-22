#include <QtWidgets>
#include "ui/ChessWindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    ChessWindow  board;
    board.show();
    board.resize();

    return QApplication::exec();
}
