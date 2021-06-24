#ifndef MYCANVAS_H
#define MYCANVAS_H

#include <QLabel>
#include <QPainter>
#include "figuremanager.h"

class MyCanvas : public QLabel
{
public:
    bool is_draw_clipper = false;
    bool is_draw_figure = false;
    bool is_adding_line = false;

    QPoint adding_point;

    FigureManager *fmanager;
    QPixmap pixmap;
    QColor color = QColor(0,0,0);

    bool is_ctrl = false;
    bool is_adding_point = false;
    bool is_slowed = false;

    MyCanvas(QLabel *label, FigureManager *fmanager);

    void mouseReleaseEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;


    void drawFigure();

    void clip();


    std::optional<QPainterPath> getPath(const QList<QPointF> &borders, bool is_draw = false);

    QPoint getAddingPoint(QMouseEvent *event);

};

#endif // MYCANVAS_H
