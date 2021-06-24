#include "mycanvas.h"

#include <QCoreApplication>
#include <QDebug>
#include <QMouseEvent>
#include <QPixmap>
#include <QStack>
#include <QTime>
#include <cmath>
#include <iostream>

#define sign(x) (x > 0) ? 1 : ((x < 0) ? -1 : 0)
MyCanvas::MyCanvas(QLabel *label, FigureManager &_fmanager)
    : QLabel(label), fmanager(_fmanager), pixmap(QSize(1920, 1080)) {
  setMouseTracking(true);

  pixmap.fill();
  setPixmap(pixmap);
}

QPoint MyCanvas::getAddingPoint(QMouseEvent *event) {
  QPoint point = event->pos();
  if (is_ctrl && !fmanager.get_is_ended()) {
    auto last = fmanager.getLastPoint();
    auto diff = point - last;

    if (labs(diff.x()) > labs(diff.y()))
      point.setY(last.y());
    else
      point.setX(last.x());
  }

  return point;
}

void MyCanvas::fillFigures() {
  if (!fmanager.get_is_ended()) return;

  QImage image(pixmap.size(), QImage::Format_RGB32);
  image.fill(Qt::white);

  QPainter painter(&image);
  painter.setPen(color);
  drawFiguresTop(painter);

  QStack<QPoint> stack;
  stack.push(fmanager.getFillingPoint());

  while (!stack.isEmpty()) {
    QPoint point = stack.pop();

    if (image.pixelColor(point) == color) continue;

    int xr = point.x() + 1;
    while (image.pixelColor(xr, point.y()) != color && xr + 1 < pixmap.width())
      ++xr;

    int xl = point.x();
    while (image.pixelColor(xl, point.y()) != color && xl > 0) --xl;

    int y = point.y();
    painter.drawLine(xl, y, xr, y);

    if (point.y() > 1)
      findNewFillingPoints(stack, image, xl, xr, point.y() - 1);
    if (point.y() + 1 < pixmap.height())
      findNewFillingPoints(stack, image, xl, xr, point.y() + 1);

    if (is_slowed) {
      setPixmap(QPixmap::fromImage(image));
      QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 0);
    }
  }

  pixmap = QPixmap::fromImage(image);
  setPixmap(pixmap);
}

void MyCanvas::findNewFillingPoints(QStack<QPoint> &stack, const QImage &image,
                                    int xl, int xr, int y) {
  for (int x = xl; x < xr; ++x) {
    while (image.pixelColor(x + 1, y) == color && x + 1 < xr) x++;

    while (image.pixelColor(x + 1, y) != color && x + 1 < xr) x++;

    stack.push(QPoint(x, y));
  }
}

void MyCanvas::mouseReleaseEvent(QMouseEvent *event) {
  if (is_adding_point || event->button() == Qt::MouseButton::RightButton) {
    fmanager.setFillingPoint(event->pos());
    is_adding_point = false;
  } else {
    fmanager.addPoint(getAddingPoint(event));
  }

  drawFigure();
}

void MyCanvas::mouseMoveEvent(QMouseEvent *ev) {
  if (fmanager.get_is_ended()) return;

  drawFigure();
  QPainter painter(&pixmap);
  painter.drawLine(fmanager.getLastPoint(), getAddingPoint(ev));
  setPixmap(pixmap);
}

void MyCanvas::drawFiguresTop(QPainter &painter) {
  auto figures = fmanager.getFigures();
  if (!figures->size()) return;

  auto i = figures->rbegin();

  for (int j = 0; j < i->size() - 1; j++) {
    painter.drawLine((*i)[j], (*i)[j + 1]);
  }
  if (fmanager.get_is_ended())
    painter.drawLine(*i->begin(), *i->rbegin());
  else {
    painter.drawEllipse(*i->begin(), 5, 5);
  }
  for (++i; i != figures->rend(); ++i) {
    for (int j = 0; j < i->size() - 1; j++) {
      painter.drawLine((*i)[j], (*i)[j + 1]);
    }
    painter.drawLine(*i->begin(), *i->rbegin());
  }
}

void MyCanvas::drawFigure() {
  QPainter painter(&pixmap);
  pixmap.fill();

  painter.setPen(Qt::red);

  auto point = fmanager.getFillingPoint();

  const int r = 10;
  painter.drawEllipse(point, r, r);
  painter.drawLine(point.x() - r, point.y(), point.x() + r, point.y());
  painter.drawLine(point.x(), point.y() - r, point.x(), point.y() + r);

  painter.setPen(color);

  drawFiguresTop(painter);

  setPixmap(pixmap);
}
