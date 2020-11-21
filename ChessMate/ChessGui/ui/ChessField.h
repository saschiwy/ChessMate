#pragma once
#include <QBrush>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

#include "ChessFigure.h"
#include "ChessEngine/ChessTypes.h"
#include "ChessEngine/Figure.h"

class ChessField : public QGraphicsRectItem
{
public:
    ChessField(QGraphicsItem* parent, ChessNS::Position position);

    ChessNS::Color getColor() const;

    ChessNS::Position getPosition() const;

    QBrush getBrush() const;

    void setFigure(const ChessNS::FigureType& figureType, const ChessNS::Color& color);

    QGraphicsPixmapItem* getFigure() const;

private:
    ChessNS::Color       _color;
    ChessNS::Position    _position;
    QBrush               _brush;
    QGraphicsPixmapItem* _figure{nullptr};
};
