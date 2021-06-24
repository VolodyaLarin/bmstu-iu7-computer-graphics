#ifndef LINEDRAWER_H
#define LINEDRAWER_H

#include <QPainter>

enum LineDrawerMethod {
    CDA = 0,
    LIB = 5,
    Bresenham_double = 1,
    Bresenham_int = 2,
    Bresenham_antialias = 3,
    Wu = 4,
    LineDrawerMethodCount = 6,
};


class LineDrawer
{
private:
    QPainter *painter;

public:
    int stairs = 0;

    explicit LineDrawer(QPainter &_painter): painter(&_painter) {}
    explicit LineDrawer(QPainter *_painter): painter(_painter) {}

    void drawLine(const QPoint &start, const QPoint &end,  LineDrawerMethod method);
    void drawBresenham_double(const QPoint &start, const QPoint &end);
    void drawBresenham_int(const QPoint &start, const QPoint &end);
    void drawBresenham_antialias(const QPoint &start, const QPoint &end);
    void drawCDA(const QPoint &start, const QPoint &end);
    void drawWu(const QPoint &start, const QPoint &end);
    void setPixel(int x, int y, double a);
};

#endif // LINEDRAWER_H
