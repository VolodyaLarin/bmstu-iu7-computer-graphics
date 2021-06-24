#include "mycanvas.h"

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
  if (is_adding_line) {
    auto table = fmanager->table;
    int new_row = table->rowCount();
    table->setRowCount(new_row + 1);
    QPoint point = getAddingPoint(event);
    table->setItem(new_row, 0, new QTableWidgetItem(QString::number(adding_point.x())));
    table->setItem(new_row, 1, new QTableWidgetItem(QString::number(adding_point.y())));
    table->setItem(new_row, 2, new QTableWidgetItem(QString::number(point.x())));
    table->setItem(new_row, 3, new QTableWidgetItem(QString::number(point.y())));
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
  auto borders = fmanager->getBorders();
  borders.setBottomRight(borders.bottomRight() - QPoint(1,1));
  painter.drawRect(borders);

  painter.setPen(Qt::blue);
  QVector<QLine> lines = fmanager->getLines();

  for (auto line: lines) {
    painter.drawLine(line);
  }

  setPixmap(pixmap);
}


void MyCanvas::clip() {
  QPainter painter(&pixmap);

  auto borders = fmanager->getBorders();

  QPainterPath path;
  path.moveTo(borders.topLeft() + QPoint(1,1));
  path.lineTo(borders.topRight() + QPoint(-1,1));
  path.lineTo(borders.bottomRight() + QPoint(-1,0));
  path.lineTo(borders.bottomLeft() + QPoint(1, 0));

  painter.fillPath(path, Qt::white);

  painter.setPen(Qt::green);

  QVector<QLine> lines = fmanager->getLines();

  LineClipping clipper(borders);

  for (auto line: lines) {
      auto line_clipped = clipper.CohenSutherland(line);
      if  (line_clipped.first)
        painter.drawLine(line_clipped.second);

  }

  setPixmap(pixmap);
}
