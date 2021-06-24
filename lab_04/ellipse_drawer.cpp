#include "ellipse_drawer.h"
#include <cmath>

#include <QPoint>



void EllipseDrawer::drawEllipse(const QPoint &center, const QPair<int,int> &size, EllipseDrawerMethod method)
{
    switch (method) {
    case EllipseDrawerMethod::lib:
        if(painter) painter->drawEllipse(center, size.first, size.second);
        break;
    case EllipseDrawerMethod::canonical:
        drawCanonicalEllipse(center, size);
        break;
    case EllipseDrawerMethod::parametric:
        drawParametricEllipse(center, size);
        break;
    case EllipseDrawerMethod::brethenham:
        drawBrethenhamEllipse(center, size);
        break;
    case EllipseDrawerMethod::middlepoint:
        drawMiddlepointEllipse(center, size);
        break;
    default:
        break;
    }
}

void EllipseDrawer::drawCircule(const QPoint &center, int radius, EllipseDrawerMethod method)
{
    switch (method) {
    case EllipseDrawerMethod::lib:
        if(painter) painter->drawEllipse(center, radius, radius);
        break;
    case EllipseDrawerMethod::canonical:
        drawCanonicalCircule(center, radius);
        break;
    case EllipseDrawerMethod::parametric:
        drawParametricCircule(center, radius);
        break;
    case EllipseDrawerMethod::brethenham:
        drawBrethenhamCircule(center, radius);
        break;
    case EllipseDrawerMethod::middlepoint:
        drawMiddlepointCircule(center, radius);
    default:
        break;
    }
}



void EllipseDrawer::drawCanonicalCircule(const QPoint &center, int radius)
{
    int sq_r = radius * radius;
    int end_a = round(center.x() + radius / sqrt(2));

    for (int a = center.x(); a <= end_a; a++)
    {
        int b = round(center.y() + sqrt(sq_r - (a - center.x()) * (a - center.x())));

        setPixel8(QPoint(a,b), center);
    }

}

void EllipseDrawer::drawParametricCircule(const QPoint &center, int radius)
{
    double step = 1. / radius;


    for (double t = 0; t <= M_PI_4; t += step)
    {
        int a = round(center.x() + radius * cos(t));
        int b = round(center.y() + radius * sin(t));
        setPixel8(QPoint(a,b), center);
    }
}

void EllipseDrawer::drawBrethenhamCircule(const QPoint &center, int radius)
{
    int x = 0;
    int y = radius;
    int delta = 2 - 2 * radius;

    setPixel8(QPoint(center.x() + x, center.y() + y), center);

    while (x < y)
    {
        if (delta <= 0)
        {
            int d1 = 2 * delta + 2 * y - 1;
            ++x;
            if (d1 >= 0)
            {
                --y;
                delta += 2 * (x - y +1);
            }
            else {
                delta += 2 * x + 1;
            }

        }
        else {
            int d2 = 2 * (delta - x) - 1;
            --y;
            if (d2 < 0)
            {
                x++;
                delta += 2 * (x-y+1);
            }
            else{
                delta += 2 * y + 1;
            }
        }

        setPixel8(QPoint(x + center.x(), y + center.y()), center);

    }

}

void EllipseDrawer::drawMiddlepointCircule(const QPoint &center, int radius)
{
    int x = radius;
    int y = 0;

    setPixel8(QPoint(center.x() + x, center.y() + y), center);

    int p = 1 - radius;

    while (x > y)
    {
        y++;
        if (p >= 0) {
            --x;
            p += 2 * (y - x) + 1;
        } else{
            p += 2 * y + 1;
        }
        setPixel8(QPoint(center.x() + x, center.y() + y), center);
    }
}

QPoint EllipseDrawer::mirrorX(const QPoint &point, const QPoint &center)
{
    int x = center.x() + center.x() - point.x();
    return QPoint (x, point.y());
}

QPoint EllipseDrawer::mirrorY(const QPoint &point, const QPoint &center)
{
    int y = center.y() + center.y() - point.y();
    return QPoint (point.x(), y);
}
QPoint EllipseDrawer::mirrorXY(const QPoint &point, const QPoint &center)
{
    return mirrorX(mirrorY(point, center), center);
}

QPoint EllipseDrawer::mirrorBis(const QPoint &point, const QPoint &center)
{
    int dx = point.x() - center.x();
    int dy = point.y() - center.y();

    int x = center.x() + dy;
    int y = center.y() + dx;

    return QPoint (x,y);
}


void EllipseDrawer::setPixel(int x, int y)
{
    if (!painter) return;
    painter->drawPoint(x, y);
}

void EllipseDrawer::setPixel(const QPoint &point)
{
    setPixel(point.x(), point.y());
}

void EllipseDrawer::setPixel4(const QPoint &point, const QPoint &center)
{
    if (!painter) return;
    setPixel(point); // 2
    setPixel(mirrorY(point, center)); //3
    setPixel(mirrorXY(point, center)); // 6
    setPixel(mirrorX(point, center)); // 7
}

void EllipseDrawer::setPixel8(const QPoint &point, const QPoint &center)
{
    if (!painter) return;
    setPixel(mirrorBis(point, center)); // 1
    setPixel(point); // 2
    setPixel(mirrorY(point, center)); //3
    setPixel(mirrorY(mirrorBis(point, center), center)); // 4
    setPixel(mirrorXY(mirrorBis(point, center), center)); // 5
    setPixel(mirrorXY(point, center)); // 6
    setPixel(mirrorX(point, center)); // 7
    setPixel(mirrorX(mirrorBis(point, center), center)); // 8
}



void EllipseDrawer::drawCanonicalEllipse(const QPoint &center, const QPair<int,int> &sizes)
{
    int sq_a = sizes.first * sizes.first;
    int sq_b = sizes.second * sizes.second;

    int end1 = round(center.x() + sizes.first / sqrt(1 + sq_b/sq_a));

    for (int x = center.x(); x <= end1; x++)
    {
        int y = round(
                      center.y() + sizes.second * sqrt(
                            1 - (x - center.x()) * (double)(x - center.x()) / sq_a
                        )
                    );
        QPoint point(x,y);
        setPixel4(point, center);
    }

    int end2 = round(center.y() + sizes.second/ sqrt(1 + sq_a/sq_b));

    for (int y = center.y(); y <= end2; y++)
    {
        int x = round(
                    center.x() + sizes.first * sqrt(
                          1 - (y - center.y()) * (double)(y - center.y()) / sq_b
                      )
                  );
        QPoint point(x,y);
        setPixel4(QPoint(x,y), center);
    }

}

void EllipseDrawer::drawParametricEllipse(const QPoint &center, const QPair<int,int> &sizes)
{
    double step = 1. / fmax(sizes.first, sizes.second);


    for (double t = 0; t <= M_PI_2; t += step)
    {
        int a = round(center.x() + sizes.first * cos(t));
        int b = round(center.y() + sizes.second * sin(t));
        setPixel4(QPoint(a,b), center);
    }
}

void EllipseDrawer::drawBrethenhamEllipse(const QPoint &center, const QPair<int, int> &sizes)
{
    int a = sizes.first;
    int b = sizes.second;
    int x = 0;
    int y = b;

    setPixel4(QPoint(center.x() + x, center.y() + y), center);

    int delta = round(b * b / 2. - a * b * 2 + a / 2.);

    while (y >= 0){
        if (delta <= 0){
            int d1 = 2 * delta + 2 * a * y - a;
            ++x;
            if (d1 <= 0)
            {
                delta += 2 * b * x + b;
            } else {
                --y;
                delta += 2 * b * x - 2 * a * y + a + b;
            }
        }
        else {
            int d2= 2 * delta - 2 * b * x - b;
            --y;

           if (d2 > 0) {
               delta -= 2 * y * a + a;
           }
           else{
               ++x;
               delta += 2 * x * b - 2 * y * a + a + b;
           }

        }
        setPixel4(QPoint(center.x() + x, center.y() + y), center);
    }
}

void EllipseDrawer::drawMiddlepointEllipse(const QPoint &center, const QPair<int, int> &size)
{
    int a = size.first;
    int b = size.second;
    int xc = center.x();
    int yc = center.y();

    int sqr_a = a * a;
    int sqr_b = b * b;

    int limit = round(a / sqrt(1 + (double)sqr_b / sqr_a));

    int x = 0;
    int y = b;

    setPixel4(QPoint(x + xc, y + yc), center);

    int fu = sqr_b - round(sqr_a * (b - 1 / 4.));
    while (x < limit){
        if (fu > 0){
            --y;
            fu -= 2 * sqr_a * y;
        }

        ++x;
        fu += sqr_b * (2 * x + 1);
        setPixel4(QPoint(x + xc, y + yc), center);
    }

    limit = round(b / sqrt(1 + (double) sqr_a / sqr_b));

    y = 0;
    x = a;
    setPixel4(QPoint(x + xc, y + yc), center);

    fu = sqr_a - round(sqr_b * (a - 1 / 4.));
    while (y < limit){
        if (fu > 0){
            --x;
            fu -= 2 * sqr_b * x;
        }

        ++y;
        fu += sqr_a * (2 * y + 1);
        setPixel4(QPoint(x + xc, y + yc), center);
    }
}


