#pragma once
#include <QDialog>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "ChessField.h"
#include <vector>

class PromotionChose : public QDialog
{
Q_OBJECT
public:
    PromotionChose(ChessNS::Color color, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~PromotionChose();

    ChessNS::FigureType trigger();

private:
    void fieldPressed(const ChessNS::Position& pos);

    QGraphicsView*                         _view;
    QGraphicsScene*                        _scene;
    ChessNS::FigureType                    _chosenType{ChessNS::FigureType::queen};
    const std::vector<ChessNS::FigureType> _figs = {
        ChessNS::FigureType::bishop, ChessNS::FigureType::knight, ChessNS::FigureType::rook, ChessNS::FigureType::queen
    };
};
