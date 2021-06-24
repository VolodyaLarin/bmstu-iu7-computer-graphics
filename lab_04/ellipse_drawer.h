#ifndef LINEDRAWER_H
#define LINEDRAWER_H

#include <QPainter>

enum EllipseDrawerMethod {
    canonical,
    parametric,
    brethenham,
    middlepoint,
    lib,
    count,
};


class EllipseDrawer
{
private:
    QPainter *painter;

public:
    explicit EllipseDrawer(QPainter &_painter): painter(&_painter) {}
    explicit EllipseDrawer(QPainter *_painter): painter(_painter) {}

    void drawEllipse(const QPoint &center, const QPair<int,int> &size, EllipseDrawerMethod method);

    void drawCircule(const QPoint &center, int radius, EllipseDrawerMethod method);

    void drawCanonicalCircule(const QPoint &center, int radius);
    void drawParametricCircule(const QPoint &center, int radius);
    void drawBrethenhamCircule(const QPoint &center, int radius);
    void drawMiddlepointCircule(const QPoint &center, int radius);

    void drawCanonicalEllipse(const QPoint &center, const QPair<int,int> &size);
    void drawParametricEllipse(const QPoint &center, const QPair<int,int> &sizes);
    void drawBrethenhamEllipse(const QPoint &center, const QPair<int,int> &sizes);
    void drawMiddlepointEllipse(const QPoint &center, const QPair<int,int> &size);


    void setPixel(int x, int y);
    void setPixel(const QPoint &point);

    void setPixel4(const QPoint &point, const QPoint &center);
    void setPixel8(const QPoint &point, const QPoint &center);

    QPoint mirrorX(const QPoint &point, const QPoint &center);
    QPoint mirrorY(const QPoint &point, const QPoint &center);
    QPoint mirrorXY(const QPoint &point, const QPoint &center);
    QPoint mirrorBis(const QPoint &point, const QPoint &center);
};


#endif // LINEDRAWER_H
