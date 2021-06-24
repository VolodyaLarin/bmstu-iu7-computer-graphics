#include "figuremanager.h"


FigureManager::FigureManager(
        QTableWidget *table,
        QTableWidget *clipper)
    :table(table), clipper(clipper)
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

QVector<QPointF> FigureManager::getClipper(){
    int rows = clipper->rowCount();

    QVector<QPointF> points(rows);

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < 2; ++j)
            if (!clipper->item(i,j))
                return points;

        points[i].setX(clipper->item(i, 0)->text().toInt());
        points[i].setY(clipper->item(i, 1)->text().toInt());
    }

    return points;
}

void FigureManager::addPoint(const QPoint &point)
{
    int new_row = clipper->rowCount();
    clipper->setRowCount(new_row + 1);

    clipper->setItem(new_row, 0, new QTableWidgetItem(QString::number(point.x())));
    clipper->setItem(new_row, 1, new QTableWidgetItem(QString::number(point.y())));
}

void FigureManager::addLine(const QLine &line)
{
    int new_row = table->rowCount();
    table->setRowCount(new_row + 1);

    table->setItem(new_row, 0, new QTableWidgetItem(QString::number(line.p1().x())));
    table->setItem(new_row, 1, new QTableWidgetItem(QString::number(line.p1().y())));
    table->setItem(new_row, 2, new QTableWidgetItem(QString::number(line.p2().x())));
    table->setItem(new_row, 3, new QTableWidgetItem(QString::number(line.p2().y())));
}


