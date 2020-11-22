#include "PromotionChose.h"
#include <QEventLoop>

PromotionChose::PromotionChose(ChessNS::Color color, QWidget* parent, Qt::WindowFlags f)
    : QDialog(parent, f)
{
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowTitleHint);

    auto cb = [this](const ChessNS::Position& pos) { return fieldPressed(pos); };

    const qreal size = 100;
    _scene           = new QGraphicsScene(this);
    _view            = new QGraphicsView(_scene, this);
    _view->setFixedSize(_figs.size() * size, size);
    this->setFixedSize(_figs.size() * size, size);
    _view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    for (int i = 0; i < static_cast<int>(_figs.size()); i++)
    {
        auto* field = new ChessField(nullptr, ChessNS::Position(i, 0), cb);
        field->setRect(static_cast<qreal>(i) * size, 0, size, size);
        field->setBrush(field->getBrush());
        field->setColor(Qt::lightGray);
        _scene->addItem(field);
        field->setFigure(_figs.at(i), color);
        _scene->addItem(field->getFigure());
    }
}

PromotionChose::~PromotionChose()
{
    _scene->clear();
    delete _view;
    delete _scene;
}

ChessNS::FigureType PromotionChose::trigger()
{
    this->show();
    QEventLoop loop;

    connect(this, SIGNAL(destroyed()), &loop, SLOT(quit()));
    loop.exec();

    QDialog::close();

    return _chosenType;
}

void PromotionChose::fieldPressed(const ChessNS::Position& pos)
{
    _chosenType = _figs.at(static_cast<int>(pos.row));
    QDialog::destroyed();
}
