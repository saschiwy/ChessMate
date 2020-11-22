#pragma once

#include <qgraphicsscene.h>
#include <QMainWindow>

#include "ChessField.h"
#include "ChessEngine/Board.h"
#include "ChessPlayer/IPlayer.h"
#include <mutex>
#include <thread>

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

private:
    void drawBoard();

    void clearScene();

    void fieldPressed(const ChessNS::Position& position);

    void startAiMove();

    void finishAiMove();

    void aiMove();

    void printMoves();

    Ui::MainWindow*                   _ui;
    QGraphicsScene*                   _boardScene{nullptr};
    ChessNS::Matrix<ChessField*>      _fields;
    std::shared_ptr<ChessNS::Board>   _board;
    bool                              _selected{false};
    ChessNS::Position                 _origin;
    std::mutex                        _mtx;
    std::mutex                        _aiMtx;
    std::unique_ptr<ChessNS::IPlayer> _player;
    std::unique_ptr<ChessNS::IPlayer> _ai;
    std::thread                       _aiThread;
    bool                              _aiFinished{false};

public slots:
    void redraw();

signals:
    void requestRedraw();
};
