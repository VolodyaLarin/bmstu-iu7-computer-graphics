#include "linedrawer.h"
#include <cmath>

#include <QPoint>


void LineDrawer::drawLine(const QPoint &start, const QPoint &end,  LineDrawerMethod method)
{
    stairs = 0;
    switch (method) {
    case LineDrawerMethod::CDA:
        drawCDA(start, end);
        break;
    case LineDrawerMethod::LIB:
        if(painter) painter->drawLine(start, end);
        break;
    case LineDrawerMethod::Bresenham_double:
        drawBresenham_double(start, end);
        break;
    case LineDrawerMethod::Bresenham_int:
        drawBresenham_int(start, end);
        break;
    case LineDrawerMethod::Bresenham_antialias:
        drawBresenham_antialias(start, end);
        break;
    case LineDrawerMethod::Wu:
        drawWu(start, end);
        break;
    case LineDrawerMethod::LineDrawerMethodCount:
        break;
    }
}

void LineDrawer::drawCDA(const QPoint &start, const QPoint &end)
{
    int l = end.x() - start.x();
    int ly = end.y() - start.y();

    double dx = l;
    double dy = ly;

    l = labs(l);
    ly = labs(ly);
    if (ly > l) l = ly;


    dx /= l;
    dy /= l;

    double x = start.x();
    double y = start.y();

    for (int i = 0; i < l ; i++)
    {
        setPixel(x + 0.5, y + 0.5, 1);

        if (!painter)
        {
            if (
                    labs(int(x+0.5) - int(x + 0.5 + dx)) > 0 &&
                    labs(int(y+0.5) - int(y + 0.5 + dy)) > 0
               )
                stairs++;
        }

        x += dx;
        y += dy;



    }
}



int int_sign(int x) {
    if (!x) return 0;
    if (x > 0) return 1;
    return -1;
}



void LineDrawer::drawBresenham_double(const QPoint &start, const QPoint &end)
{

    int dx = end.x() - start.x();
    int dy = end.y() - start.y();

    auto ps = start, pe = end;

    if (labs(dy) > labs(dx))
    {
        if (dy < 0) {pe = start, ps = end; dx *= -1; dy *= -1;}

        int xsign = int_sign(dx);


        double m = fabs((double) dx / (double) dy);
        double err = m - 0.5;

        int x = ps.x();
        int y;
        for (y = ps.y(); y <= pe.y(); ++y)
        {
            setPixel(x, y, 1);

            if (err > 0) {
                x += xsign, err -= 1;
                if (!painter) stairs++;
            }
            err += m;
        }
    }
    else {
        if (dx < 0) {pe = start, ps = end; dx *= -1; dy *= -1;}

        int ysign = int_sign(dy);


        double m = fabs((double) dy / (double) dx);
        double err = m - 0.5;

        int x;
        int y = ps.y();
        for (x = ps.x(); x <= pe.x(); ++x)
        {
            setPixel(x, y, 1);

            if (err > 0){
                y += ysign, err -= 1;
                if (!painter) stairs++;
            }
            err += m;
        }
    }
}



void LineDrawer::drawBresenham_int(const QPoint &start, const QPoint &end)
{

    int dx = end.x() - start.x();
    int dy = end.y() - start.y();

    auto ps = start, pe = end;

    if (labs(dy) > labs(dx))
    {
        if (dy < 0) {pe = start, ps = end; dx *= -1; dy *= -1;}

        int xsign = int_sign(dx);

        dx = labs(dx), dy = labs(dy);

        int err = 2 * dx - dy;

        int y;
        int x = ps.x();
        for (y = ps.y(); y <= pe.y(); ++y)
        {
            setPixel(x, y, 1);

            if (err > 0){
                x += xsign, err -= 2 * dy;
                if (!painter) stairs++;
            }
            err += 2 * dx;
        }
    }
    else {
        if (dx < 0) {pe = start, ps = end; dx *= -1; dy *= -1;}

        int ysign = int_sign(dy);

        dx = labs(dx), dy = labs(dy);
        int err = 2 * dy - dx;

        int x;
        int y = ps.y();
        for (x = ps.x(); x <= pe.x(); ++x)
        {
            setPixel(x, y, 1);

            if (err > 0) {
                y += ysign, err -= 2 * dx;
                if (!painter) stairs++;
            }
            err += 2 * dy;
        }
    }



}



void LineDrawer::drawBresenham_antialias(const QPoint &start, const QPoint &end)
{

    int dx = end.x() - start.x();
    int dy = end.y() - start.y();

    auto ps = start, pe = end;

    if (labs(dy) > labs(dx))
    {
        if (dy < 0) {pe = start, ps = end; dx *= -1; dy *= -1;}

        int xsign = int_sign(dx);


        double m = fabs((double) dx / (double) dy);
        double err = m - 0.5;

        int x = ps.x();
        int y;
        for (y = ps.y(); y <= pe.y(); ++y)
        {
            setPixel(x, y, 1 - (err + 0.7) * 0.6 );

            if (err > 0) {
                x += xsign, err -= 1;
                if (!painter) stairs++;
            }
            err += m;
        }
    }
    else {
        if (dx < 0) {pe = start, ps = end; dx *= -1; dy *= -1;}

        int ysign = int_sign(dy);


        double m = fabs((double) dy / (double) dx);
        double err = m - 0.5;

        int x;
        int y = ps.y();
        for (x = ps.x(); x <= pe.x(); ++x)
        {

            setPixel(x, y, 1 - (err + 0.7) * 0.6 );

            if (err > 0) {
                if (!painter) stairs++;
                y += ysign, err -= 1;
            }
            err += m;
        }
    }
}

void LineDrawer::setPixel(int x, int y, double a)
{
    if (!painter) return;

    QColor color = painter->pen().color();
    color.setAlphaF(a);
    painter->setPen(QPen(color));

    painter->drawPoint(x, y);
}

void LineDrawer::drawWu(const QPoint &start, const QPoint &end)
{
    QPoint ps = start, pe = end;

    int dx = pe.x() - ps.x();
    int dy = pe.y() - ps.y();

    if (labs(dy) >= labs(dx)){
        if (pe.y() < ps.y()) ps = end, pe = start, dx*=-1, dy *=-1;

        double grad = (double) dx / (double) dy;

        double intery = ps.x() + grad;

        setPixel(ps.x(), ps.y(), 1);
        setPixel(pe.x(), pe.y(), 1);

        for (int y = ps.y(); y < pe.y(); y++)
        {
            setPixel(int(intery), y, 1 - (intery - int(intery)));
            setPixel(int(intery) + 1, y, (intery - int(intery)));

            if (!painter) {
                if ( labs(int(intery) - int(intery + grad)) > 0 )
                    stairs++;
            }

            intery += grad;
        }
    }
    else {
        if (pe.x() < ps.x()) ps = end, pe = start, dx*=-1, dy *=-1;

        double grad = (double) dy / (double) dx;

        double intery = ps.y() + grad;

        setPixel(ps.x(), ps.y(), 1);
        setPixel(pe.x(), pe.y(), 1);

        for (int x = ps.x(); x < pe.x(); x++)
        {
            setPixel(x, int(intery), 1 + int(intery) - intery);
            setPixel(x, int(intery) + 1, intery - int(intery));

            if (!painter) {
                if ( labs(int(intery) - int(intery + grad)) > 0 )
                    stairs++;
            }

            intery += grad;


        }
    }



}


