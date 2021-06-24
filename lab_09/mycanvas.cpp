#include "mycanvas.h"
#include "mathutilits.h"

#include <QCoreApplication>
#include <QDebug>
#include <QMouseEvent>
#include <QPixmap>
#include <QStack>
#include <QTime>
#include <cmath>
#include <iostream>
#include <QPainterPath>

#include "lineclipping.h"


#define sign(x) (x > 0) ? 1 : ((x < 0) ? -1 : 0)
MyCanvas::MyCanvas(QLabel *label, FigureManager *fmanager)
    : QLabel(label), fmanager(fmanager), pixmap(QSize(1920, 1080)) {
  setMouseTracking(true);

  pixmap.fill();
  setPixmap(pixmap);
}

void MyCanvas::mouseReleaseEvent(QMouseEvent *event) {
    if (is_draw_clipper || is_draw_figure) {
        QPoint point = getAddingPoint(event);
        adding_point = point;
        is_adding_line = true;

        if (is_draw_clipper)
            fmanager->addClipperPoint(point);
        else
            fmanager->addFigurePoint(point);
        return;
    }

  drawFigure();
}

void MyCanvas::mouseMoveEvent(QMouseEvent *ev) {
  if (!is_adding_line) return;

  drawFigure();
  QPainter painter(&pixmap);
  painter.drawLine(adding_point, getAddingPoint(ev));
  setPixmap(pixmap);
}


QPoint MyCanvas::getAddingPoint(QMouseEvent *event) {
  QPoint point = event->pos();
  if (is_adding_line && is_ctrl) {
    auto last = adding_point;
    auto diff = point - last;

    if (labs(diff.x()) > labs(diff.y()))
      point.setY(last.y());
    else
      point.setX(last.x());
  }

  return point;
}

void MyCanvas::drawFigure() {
  QPainter painter(&pixmap);
  pixmap.fill();

  painter.setPen(Qt::red);

    auto path = getPath(fmanager->getClipper(), is_draw_clipper);
    if (path) {
        painter.drawPath(*path);
    }

  painter.setPen(Qt::blue);

  path = getPath(fmanager->getFigure(), is_draw_figure);
  if (path) {
      painter.drawPath(*path);
  }

  setPixmap(pixmap);
}


void MyCanvas::clip() {
    QPainter painter(&pixmap);

    painter.setPen(Qt::red);
    auto verteces_list = fmanager->getClipper();
    auto figure_list = fmanager->getFigure();

    auto path = getPath(verteces_list);
    if (!path) return;

    if (!MathUtilits::check_polygon(verteces_list)) return;

    painter.fillPath(*path, Qt::white);

    auto normals_list = MathUtilits::get_normal_list(verteces_list);
    auto cutted_figure = MathUtilits::cut_figure(figure_list, verteces_list, normals_list);

    painter.setPen(Qt::green);

    path = getPath(cutted_figure);
    if (!path) return;
    painter.drawPath(*path);

    painter.setPen(Qt::red);

    path = getPath(verteces_list);
    // painter.drawPath(*path);

    setPixmap(pixmap);
}

std::optional<QPainterPath> MyCanvas::getPath(const QList<QPointF> &borders, bool is_draw)
{
    if (borders.length() > 1)
    {
        QPainterPath path;
        path.moveTo(borders[0]);

        for (int i = 1; i < borders.length() ; ++i) {
            path.lineTo(borders[i]);
        }

        if (!is_draw)
            path.closeSubpath();

        return path;
    }

    return std::nullopt;
}
