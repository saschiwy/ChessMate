#pragma once

#include <qgraphicsscene.h>
#include <QMainWindow>

#include "ChessField.h"
#include "ChessEngine/Board.h"
#include <map>

QT_BEGIN_NAMESPACE

namespace Ui
{
    class MainWindow;
}

QT_END_NAMESPACE

class ChessWindow : public QMainWindow
{
Q_OBJECT

public:
    ChessWindow(QWidget* parent = nullptr);

    ~ChessWindow();

    void resize() const;

    void resizeEvent(QResizeEvent* event) override;

    std::shared_ptr<ChessNS::Board> getBoard() const;

    void setBoard(const std::shared_ptr<ChessNS::Board>& board);

    void drawBoard();

private:

    Ui::MainWindow*                 _ui;
    QGraphicsScene*                 _boardScene{};
    ChessNS::Matrix<ChessField*>    _fields;
    std::shared_ptr<ChessNS::Board> _board;
};
