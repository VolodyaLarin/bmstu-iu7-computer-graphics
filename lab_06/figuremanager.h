#ifndef FIGUREMANAGER_H
#define FIGUREMANAGER_H

#include <QPoint>
#include <QVector>

class FigureManager
{
private:
    std::function<void()> updateFn = [](){};
    QVector<QVector<QPoint>> figures = {};
    QPoint filling_point = QPoint(50,50);

    bool is_ended = true;
public:
    FigureManager();
    void addPoint(const QPoint &point);
    void endFigure();
    void clear();
    QPoint getLastPoint();
    const QVector<QVector<QPoint>>* getFigures();
    bool get_is_ended() {return is_ended;}

    QString getText();

    void setFillingPoint(const QPoint&);
    QPoint getFillingPoint() {return filling_point;}

    void setUpdate(std::function<void()> fn);
};

#endif // FIGUREMANAGER_H
