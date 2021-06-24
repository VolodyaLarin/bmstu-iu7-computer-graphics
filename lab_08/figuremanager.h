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
    QTableWidget *table;
    QTableWidget *clipper;


public:
    FigureManager() = default;
    FigureManager(QTableWidget *table, QTableWidget *clipper);

    QVector<QLine> getLines();
    QVector<QPointF> getClipper();


    void addPoint(const QPoint& point);
    void addLine(const QLine& line);
};

#endif // FIGUREMANAGER_H
