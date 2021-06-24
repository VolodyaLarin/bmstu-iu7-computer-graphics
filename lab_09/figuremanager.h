#ifndef FIGUREMANAGER_H
#define FIGUREMANAGER_H

#include <QTableWidget>
#include <QSpinBox>
#include <QPair>
#include <QLine>
#include <QVector>
#include <QRect>

class FigureManager
{
public:
    QTableWidget *figure;
    QTableWidget *clipper;


public:
    FigureManager() = default;
    FigureManager(QTableWidget *table, QTableWidget *clipper);

    QVector<QPointF> getFigure();

    QVector<QPointF> getClipper();


    void addClipperPoint(const QPoint& point);
    void addFigurePoint(const QPoint& point);

};

#endif // FIGUREMANAGER_H
