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
    QSpinBox *x_min;
    QSpinBox *x_max;

    QSpinBox *y_min;
    QSpinBox *y_max;


public:
    FigureManager() = default;
    FigureManager(QTableWidget *table, QPair<QSpinBox *, QSpinBox *> x, QPair<QSpinBox *, QSpinBox *> y);

    QVector<QLine> getLines();
    QRect getBorders();
};

#endif // FIGUREMANAGER_H
