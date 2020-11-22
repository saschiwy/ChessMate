#include "ChessField.h"
#include <utility>

ChessField::ChessField(QGraphicsItem* parent, ChessNS::Position position, std::function<void(const ChessNS::Position&)> cb)
    : QGraphicsRectItem(parent),
      _cb(std::move(cb))
{
    _position       = position;
    const auto cord = position.getCord();
    if ((cord.first + cord.second) % 2 == 0)
    {
        _color    = ChessNS::Color::black;
        _stdColor = Qt::darkGray;
        _brush.setColor(Qt::darkGray);
    }
    else
    {
        _color    = ChessNS::Color::white;
        _stdColor = Qt::lightGray;
        _brush.setColor(Qt::lightGray);
    }

    _brush.setStyle(Qt::SolidPattern);
    setBrush(_brush);
}

ChessField::~ChessField() { }

ChessNS::Color ChessField::getColor() const
{
    return _color;
}

ChessNS::Position ChessField::getPosition() const
{
    return _position;
}

QBrush ChessField::getBrush() const
{
    return _brush;
}

void ChessField::setFigure(const ChessNS::FigureType& figureType, const ChessNS::Color& color)
{
    _figureType  = figureType;
    _figureColor = color;
    if (figureType == ChessNS::FigureType::none || color == ChessNS::Color::none)
        return;

    actualizePix();
}

QGraphicsPixmapItem* ChessField::getFigure()
{
    return &_figure;
}

void ChessField::mousePressEvent(QGraphicsSceneMouseEvent*)
{
    if (_cb)
        _cb(_position);
}

void ChessField::resetColor()
{
    _brush.setColor(_stdColor);
    setBrush(_brush);
}

void ChessField::setColor(Qt::GlobalColor color)
{
    _brush.setColor(color);
    setBrush(_brush);
}

void ChessField::actualizePix()
{
    if (_figureType == ChessNS::FigureType::none)
    {
        _figure.setPixmap(QPixmap());
    }

    QString pixPath = "./res/";
    switch (_figureType)
    {
        case ChessNS::FigureType::king: pixPath += "King";
            break;
        case ChessNS::FigureType::queen: pixPath += "Queen";
            break;
        case ChessNS::FigureType::rook: pixPath += "Rook";
            break;
        case ChessNS::FigureType::knight: pixPath += "Knight";
            break;
        case ChessNS::FigureType::bishop: pixPath += "Bishop";
            break;
        case ChessNS::FigureType::pawn: pixPath += "Pawn";
            break;
        case ChessNS::FigureType::none: return;
    }

    switch (_figureColor)
    {
        case ChessNS::Color::white: pixPath += "W.png";
            break;
        case ChessNS::Color::black: pixPath += "B.png";
            break;
        case ChessNS::Color::none: return;
    }

    const auto pix = QPixmap(pixPath);
    _figure.setPixmap(pix);

    auto rec = rect();
    rec.setX(rec.x() + rec.width() / 2 - (pix.width() / 2));
    rec.setY(rec.y() + rec.height() / 2 - (pix.height() / 2));

    _figure.setPos(rec.x(), rec.y());
}
