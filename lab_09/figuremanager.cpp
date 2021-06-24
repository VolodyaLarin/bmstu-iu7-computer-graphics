#include "figuremanager.h"


FigureManager::FigureManager(
        QTableWidget *figure_table,
        QTableWidget *clipper)
    :figure(figure_table), clipper(clipper)
{

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


QVector<QPointF> FigureManager::getFigure(){
    int rows = figure->rowCount();

    QVector<QPointF> points(rows);

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < 2; ++j)
            if (!figure->item(i,j))
                return points;

        points[i].setX(figure->item(i, 0)->text().toInt());
        points[i].setY(figure->item(i, 1)->text().toInt());
    }

    return points;
}



void FigureManager::addClipperPoint(const QPoint &point)
{
    int new_row = clipper->rowCount();
    clipper->setRowCount(new_row + 1);

    clipper->setItem(new_row, 0, new QTableWidgetItem(QString::number(point.x())));
    clipper->setItem(new_row, 1, new QTableWidgetItem(QString::number(point.y())));
}




void FigureManager::addFigurePoint(const QPoint &point)
{
    int new_row = figure->rowCount();
    figure->setRowCount(new_row + 1);

    figure->setItem(new_row, 0, new QTableWidgetItem(QString::number(point.x())));
    figure->setItem(new_row, 1, new QTableWidgetItem(QString::number(point.y())));
}


