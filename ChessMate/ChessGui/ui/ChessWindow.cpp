#include "ChessWindow.h"
#include "./ui_ChessWindow.h"
#include "ChessField.h"
#include "ChessEngine/ChessTypes.h"
#include <QDebug>

#include "PromotionChose.h"

ChessWindow::ChessWindow(QWidget* parent)
    : QMainWindow(parent),
      _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    _board = std::make_shared<ChessNS::Board>();

    const int size = 600 / 8;
    _fields.assign(8, 8, nullptr);

    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            const auto pos  = ChessNS::Position(row, col);
            auto*      item = new ChessField(nullptr, pos, [this](const ChessNS::Position& posN) { fieldPressed(posN); });
            const auto x    = static_cast<qreal>(col * size);
            const auto y    = static_cast<qreal>((7 - row) * size);

            item->setRect(x, y, size, size);
            item->setBrush(item->getBrush());
            _fields.at(row, col) = item;
        }
    }

    _boardScene = new QGraphicsScene(this);
    _ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _ui->graphicsView->setScene(_boardScene);

    _player = ChessNS::IPlayer::createPlayer(ChessNS::PlayerType::human, ChessNS::Color::white, _board);
    _ai     = ChessNS::IPlayer::createPlayer(ChessNS::PlayerType::simpleAi, ChessNS::Color::black, _board);
    connect(this, SIGNAL(requestRedraw()), this, SLOT(redraw()));
    drawBoard();
}

ChessWindow::~ChessWindow()
{
    finishAiMove();
    delete _ui;
}

void ChessWindow::resize() const
{
    _ui->graphicsView->fitInView(_boardScene->sceneRect(), Qt::KeepAspectRatio);
}

void ChessWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    resize();
}

std::shared_ptr<ChessNS::Board> ChessWindow::getBoard() const
{
    return _board;
}

void ChessWindow::drawBoard()
{
    for (int row = 0; row < 8 && _board; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            auto&      f    = _board->at(static_cast<ChessNS::BoardRow>(row), static_cast<ChessNS::BoardColumn>(col));
            const auto cord = f.position.getCord();
            _fields.at(cord.first, cord.second)->setFigure(f.figure.getType(), f.figure.getColor());
        }
    }

    const auto fieldSize = _fields.at(1, 1)->rect().height();

    for (int row = 0; row < 8; row++)
    {
        auto* item = new QGraphicsTextItem();
        item->setZValue(1);
        item->setFont(QFont("", 14));
        item->setDefaultTextColor(Qt::black);
        item->setPlainText(QString::fromStdString(std::to_string(8 - row)));
        const auto yPos = row * fieldSize + fieldSize / 2 - 14;
        item->setPos(-20, yPos);
        _boardScene->addItem(item);

        for (int col = 0; col < 8; col++)
        {
            _fields.at(row, col)->actualizePix();
            _boardScene->addItem(_fields.at(row, col));
            _boardScene->addItem(_fields.at(row, col)->getFigure());
        }
    }

    const auto yPos = 8 * fieldSize;
    for (int col = 0; col < 8; col++)
    {
        auto* item = new QGraphicsTextItem();
        item->setZValue(1);
        item->setFont(QFont("", 14));
        item->setDefaultTextColor(Qt::black);
        item->setPlainText(QString('A' + static_cast<char>(col)));

        const auto xPos = col * fieldSize + fieldSize / 2 - 10;
        item->setPos(xPos, yPos);
        _boardScene->addItem(item);
    }

    resize();
}

void ChessWindow::clearScene()
{
    _ui->graphicsView->setScene(nullptr);

    for (auto&& field : _fields)
    {
        _boardScene->removeItem(field->getFigure());
        _boardScene->removeItem(field);
    }
    _boardScene->clear();
    _ui->graphicsView->setScene(_boardScene);
}

void ChessWindow::fieldPressed(const ChessNS::Position& position)
{
    std::unique_lock<std::mutex> l(_mtx);
    ChessNS::Movement            res{};

    if (!_selected)
    {
        if (_board->at(position).empty)
            return;

        if (_board->at(position).figure.getColor() != _board->getCurrentColorTurn())
            return;

        //if (_board->at(position).figure.getColor() != _player->getColor())
        //    return;

        auto possibilities = _board->getAllPossibleMoves(position);
        for (auto&& possibility : possibilities)
        {
            const auto cord = possibility.destination().getCord();
            _fields.at(cord.first, cord.second)->setColor(Qt::darkRed);
        }

        _origin   = position;
        _selected = true;
    }

    else
    {
        _selected = false;
        res       = _board->move(_origin, position);

        if (res.moveResult() == ChessNS::MoveResult::valid &&
            _board->at(position).figure.getColor() == _player->getColor())
        {
            _ai->move(ChessNS::Movement::invalid());
        }

        if (res.hasFlag(ChessNS::EventFlag::promotion) &&
            _board->at(position).figure.getColor() == _player->getColor())
        {
            PromotionChose promotion(_player->getColor());
            res.promotedTo() = promotion.trigger();
            _board->at(position).figure.setType(res.promotedTo());
        }

        for (auto&& field : _fields)
            field->resetColor();
    }

    if (res.moveResult() == ChessNS::MoveResult::valid)
    {
        clearScene();
        drawBoard();
        printMoves();
    }
}

void ChessWindow::startAiMove()
{
    finishAiMove();
    _aiThread = std::thread(&ChessWindow::aiMove, this);
}

void ChessWindow::finishAiMove()
{
    std::unique_lock<std::mutex> l(_aiMtx);
    _aiFinished = true;

    l.unlock();
    if (_aiThread.joinable())
        _aiThread.join();
}

void ChessWindow::aiMove()
{
    std::unique_lock<std::mutex> l(_aiMtx);
    std::unique_lock<std::mutex> l2(_mtx);

    ChessNS::Movement result;
    _aiFinished = false;

    do
    {
        result      = _ai->move(result);
        _aiFinished = result.isValid();

        if (!_aiFinished)
        {
            l.unlock();
            l2.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            l2.lock();
            l.lock();
        }
        else
        {
            requestRedraw();
        }
    }
    while (!_aiFinished);
}

void ChessWindow::printMoves()
{
    _ui->listWidget->clear();
    unsigned count = 1;

    auto moves = _board->getAllMadeMoves();
    for (auto&& move : moves)
    {
        std::string flags;
        for (size_t i = 0; i < 5; i++)
        {
            const auto e = static_cast<ChessNS::EventFlag>(i);
            if (move.hasFlag(e))
                flags += " (" + ChessNS::toString(e) + ")";
        }

        _ui->listWidget->
             addItem(QString::fromStdString(std::to_string(count++) + ". " + move.origin().toString() + "-" + move.destination().toString() + flags));
    }
}

void ChessWindow::redraw()
{
    drawBoard();
}
