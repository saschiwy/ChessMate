#include "ChessWindow.h"
#include "./ui_ChessWindow.h"
#include "ChessField.h"
#include "ChessEngine/ChessTypes.h"

ChessWindow::ChessWindow(QWidget* parent)
    : QMainWindow(parent),
      _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    const int size = 600 / 8;
    _fields.assign(8, 8, nullptr);

    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            const auto pos  = ChessNS::Position(row, col);
            auto*      item = new ChessField(nullptr, pos);
            const auto x    = static_cast<qreal>(col * size);
            const auto y    = static_cast<qreal>((7 - row) * size);

            item->setRect(x, y, size, size);
            item->setBrush(item->getBrush());

            _fields.at(row, col) = item;
        }
    }

    drawBoard();
}

ChessWindow::~ChessWindow()
{
    delete _ui;
}

void ChessWindow::resize() const
{
    _ui->graphicsView->fitInView(_boardScene->sceneRect(), Qt::KeepAspectRatio);
    _ui->graphicsView->show();
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

void ChessWindow::setBoard(const std::shared_ptr<ChessNS::Board>& board)
{
    _board = board;
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            auto& f = board->at(static_cast<ChessNS::BoardRow>(row), static_cast<ChessNS::BoardColumn>(col));
            if (f.empty)
                continue;

            auto coord = f.position.getCord();
            _fields.at(coord.first, coord.second)->setFigure(f.figure.getType(), f.figure.getColor());
        }
    }

    drawBoard();
}

void ChessWindow::drawBoard()
{
    _boardScene = new QGraphicsScene(this);
    _ui->graphicsView->setScene(_boardScene);
    _ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

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
            _boardScene->addItem(_fields.at(row, col));
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
