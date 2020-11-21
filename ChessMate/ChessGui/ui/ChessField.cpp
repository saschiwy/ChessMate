#include "ChessField.h"

ChessField::ChessField(QGraphicsItem* parent, ChessNS::Position position)
    : QGraphicsRectItem(parent)
{
    _position       = position;
    const auto cord = position.getCord();
    if ((cord.first + cord.second) % 2 == 0)
    {
        _color = ChessNS::Color::black;
        _brush.setColor(Qt::darkGray);
    }
    else
    {
        _color = ChessNS::Color::white;
        _brush.setColor(Qt::lightGray);
    }

    _brush.setStyle(Qt::SolidPattern);
    setBrush(_brush);
}

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
    if (figureType == ChessNS::FigureType::none || color == ChessNS::Color::none)
    {
        _figure = nullptr;
        return;
    }

    QString pixPath = "./res/";

    switch (figureType)
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

    switch (color)
    {
        case ChessNS::Color::white: pixPath += "W.png";
            break;
        case ChessNS::Color::black: pixPath += "B.png";
            break;
        case ChessNS::Color::none: return;
    }

    const auto pix = QPixmap(pixPath);
    _figure        = new QGraphicsPixmapItem(this);
    _figure->setPixmap(pix);

    auto rec = rect();
    rec.setX(rec.x() + rec.width() / 2 - (pix.width() / 2));
    rec.setY(rec.y() + rec.height() / 2 - (pix.height() / 2));

    _figure->setPos(rec.x(), rec.y());
}

QGraphicsPixmapItem* ChessField::getFigure() const
{
    return _figure;
}
