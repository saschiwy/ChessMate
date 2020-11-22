#pragma once
#include <QBrush>
#include <QGraphicsRectItem>

#include "ChessEngine/ChessTypes.h"
#include "ChessEngine/Figure.h"

class ChessField : public QGraphicsRectItem
{
public:
    ChessField(QGraphicsItem* parent, ChessNS::Position position, std::function<void(const ChessNS::Position&)> cb);

    ~ChessField();

    ChessNS::Color getColor() const;

    ChessNS::Position getPosition() const;

    QBrush getBrush() const;

    void setFigure(const ChessNS::FigureType& figureType, const ChessNS::Color& color);

    QGraphicsPixmapItem* getFigure();

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    void resetColor();

    void setColor(Qt::GlobalColor color);

    void actualizePix();

private:
    ChessNS::Color                                _color{};
    ChessNS::Position                             _position{};
    QBrush                                        _brush{};
    QGraphicsPixmapItem                           _figure{};
    Qt::GlobalColor                               _stdColor{};
    ChessNS::FigureType                           _figureType{ChessNS::FigureType::none};
    ChessNS::Color                                _figureColor{ChessNS::Color::none};
    std::function<void(const ChessNS::Position&)> _cb;
};
