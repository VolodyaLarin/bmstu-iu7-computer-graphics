#ifndef MYCANVAS_H
#define MYCANVAS_H

#include <QLabel>
#include <QPainter>
#include <vector>
#include "figuremanager.h"

class MyCanvas : public QLabel
{
public:
    FigureManager &fmanager;
    QPixmap pixmap;
    QColor color = QColor(0,0,0);

    bool is_ctrl = false;
    bool is_slowed = false;

    MyCanvas(QLabel *label, FigureManager &fmanager);

    void mouseReleaseEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;


    void drawFigure();
    void drawFiguresTop(QPainter &painter);
    QPoint getAddingPoint(QMouseEvent *event);

    void fillFigures();

    void fillEdge(const QLine &line, int partition);
    void reverseXLine(const QImage& image, const QPoint& point, int partition);

};

#endif // MYCANVAS_H
