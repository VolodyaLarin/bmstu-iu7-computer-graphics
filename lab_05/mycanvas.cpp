#include "mycanvas.h"

#include <QMouseEvent>
#include <iostream>
#include <cmath>
#include <QTime>
#include <QCoreApplication>
#include <QPixmap>

#define sign(x) (x > 0) ? 1 : ((x < 0) ? -1 : 0)
MyCanvas::MyCanvas(QLabel *label, FigureManager &_fmanager):
    QLabel(label), fmanager(_fmanager), pixmap(QSize(1920, 1080))
{
    setMouseTracking(true);

    pixmap.fill();
    setPixmap(pixmap);

}



QPoint MyCanvas::getAddingPoint(QMouseEvent *event)
{
    QPoint point = event->pos();
    if (is_ctrl && !fmanager.get_is_ended())
    {
        auto last = fmanager.getLastPoint();
        auto diff = point - last;

        if (labs(diff.x()) > labs(diff.y())) point.setY(last.y());
        else point.setX(last.x());
    }

    return point;
}


void MyCanvas::fillEdge(const QLine &line, int partition) {
    if (!line.dy()) return;

    QLine wline = line;
    if (wline.dy() < 0) wline = QLine(line.p2(), line.p1());
    double x = wline.x1();
    double dx = wline.dx() / fabs(static_cast<double>(wline.dy()));

    QImage image = pixmap.toImage();
    for (int y = wline.y1(); y < wline.y2(); y++) {
        reverseXLine(image, QPoint(round(x),y), partition);
        x += dx;

        if (is_slowed){
            QPixmap temp_pixmap(pixmap);
            QPainter painter(&temp_pixmap);
            drawFiguresTop(painter);

            setPixmap(temp_pixmap);
            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 0);
        }

    }
}

void MyCanvas::reverseXLine(const QImage& image, const QPoint &point, int partition)
{
    int start_x = point.x();
    int end_x = partition;
    int y = point.y();

    if (start_x > end_x){
        std::swap(start_x, end_x);
    }

    QPainter painter(&pixmap);
    for (int x = start_x; x < end_x; x++) {
        QColor color = image.pixelColor(x,y);
        QColor newc = this->color;

        if (color != QColor(255,255,255)) {
            newc = QColor(255,255,255);
        }

        painter.setPen(newc);
        painter.drawPoint(x,y);
    }

}



void MyCanvas::fillFigures()
{
    if (!fmanager.get_is_ended()) return;

    auto figures = fmanager.getFigures();



    for (auto i = figures->begin(); i != figures->end(); ++i ) {
        if (i->size() <= 2) continue;

        auto cmp = [](QPoint a, QPoint b) {
                    return a.x() < b.x();
                };

        int partition = (std::max_element(i->constBegin(), i->constEnd(), cmp)->x()
                         + std::min_element(i->constBegin(), i->constEnd(), cmp)->x()) / 2;

        for (int j = 0; j < i->size() - 1; j++) {
            fillEdge(QLine((*i)[j], (*i)[j + 1]), partition);
        }
        fillEdge(QLine(*i->begin(), *i->rbegin()), partition);
    }

    QPainter painter (&pixmap);
    drawFiguresTop(painter);
    setPixmap(pixmap);
}

void MyCanvas::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MouseButton::RightButton)
        fmanager.endFigure();
    else
        fmanager.addPoint(getAddingPoint(event));
    drawFigure();
}

void MyCanvas::mouseMoveEvent(QMouseEvent *ev)
{
    if (fmanager.get_is_ended()) return;

    drawFigure();
    QPainter painter(&pixmap);
    painter.drawLine(fmanager.getLastPoint(), getAddingPoint(ev));
    setPixmap(pixmap);
}

void MyCanvas::drawFiguresTop(QPainter &painter)
{
    auto figures = fmanager.getFigures();
    if (!figures->size()) return;

    auto i = figures->rbegin();

    for (int j = 0; j < i->size() - 1; j++) {
        painter.drawLine((*i)[j], (*i)[j + 1]);
    }
    if (fmanager.get_is_ended()) painter.drawLine(*i->begin(), *i->rbegin());
    else {
        painter.drawEllipse(*i->begin(), 5, 5);
    }
    for (++i; i != figures->rend() ; ++i ) {
        for (int j = 0; j < i->size() - 1; j++) {
            painter.drawLine((*i)[j], (*i)[j + 1]);
        }
        painter.drawLine(*i->begin(), *i->rbegin());
    }
}

void MyCanvas::drawFigure()
{
    QPainter painter(&pixmap);

    painter.setPen(color);

    pixmap.fill();

    drawFiguresTop(painter);

    setPixmap(pixmap);
}
