#include "figuremanager.h"


FigureManager::FigureManager(
        QTableWidget *table,
        QPair<QSpinBox *, QSpinBox *> x, QPair<QSpinBox *, QSpinBox *> y)
    :table(table), x_min(x.first), x_max(x.second), y_min(y.first), y_max(y.second)
{

}

QVector<QLine> FigureManager::getLines()
{
    int rows = table->rowCount();

    QVector<QLine> lines(rows);

    for (int i = 0; i < rows; ++i)
    {
        QPoint start, end;


        for (int j =0; j < 4; ++j)
            if (!table->item(i,j))
                return lines;

        start.setX(table->item(i, 0)->text().toInt());
        start.setY(table->item(i, 1)->text().toInt());

        end.setX(table->item(i, 2)->text().toInt());
        end.setY(table->item(i, 3)->text().toInt());

        lines[i].setPoints(start, end);
    }

    return lines;
}

QRect FigureManager::getBorders()
{
    QRect borders;

    int x1 = x_min->value();
    int x2 = x_max->value();

    int y1 = y_min->value();
    int y2 = y_max->value();

    borders.setTop(std::min(y1, y2));
    borders.setBottom(std::max(y1, y2));
    borders.setLeft(std::min(x1, x2));
    borders.setRight(std::max(x1, x2));

    return borders;
}

