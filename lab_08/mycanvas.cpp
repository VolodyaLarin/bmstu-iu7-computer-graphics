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
    if (is_draw_clipper) {
        QPoint point = getAddingPoint(event);
        adding_point = point;
        is_adding_line = true;
        fmanager->addPoint(point);
        return;
    }

  if (is_adding_line) {
    QPoint point = getAddingPoint(event);
    fmanager->addLine(QLine(adding_point, point));
    is_adding_line = false;
  } else {
      adding_point = event->pos();
      is_adding_line = true;
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

    auto path = getClipper();
    if (path) {
        painter.drawPath(*path);
    }

  painter.setPen(Qt::blue);
  QVector<QLine> lines = fmanager->getLines();

  for (auto line: lines) {
    painter.drawLine(line);
  }

  setPixmap(pixmap);
}


void MyCanvas::clip() {
    QPainter painter(&pixmap);

    painter.setPen(Qt::red);

    auto path = getClipper();

    if (!path) return;

    painter.fillPath(*path, Qt::white);
    painter.drawPath(*path);



    auto lines = fmanager->getLines();
    QVector<QPointF> points = {};

    for (QPointF point: fmanager->getClipper()){
        points.append(point);
    }

    if (!MathUtilits::check_polygon(points)) return;

    auto normals = MathUtilits::get_normal_list(points);

    painter.setPen(Qt::green);

    for (QLineF line: lines){
        auto clipp = MathUtilits::clip(line, points, normals);
        if(clipp)
        {
            painter.drawLine(*clipp);
        }
    }

    setPixmap(pixmap);
}

std::optional<QPainterPath> MyCanvas::getClipper()
{
    auto borders = fmanager->getClipper();

    if (borders.length() > 1)
    {
        QPainterPath path;
        path.moveTo(borders[0]);

        for (int i = 1; i < borders.length() ; ++i) {
            path.lineTo(borders[i]);
        }

        if (!is_draw_clipper)
            path.closeSubpath();

        return path;
    }

    return std::nullopt;
}
