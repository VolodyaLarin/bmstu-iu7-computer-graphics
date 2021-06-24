#ifndef LINECLIPPING_H_
#define LINECLIPPING_H_

#include <cmath>
#include <QLine>
#include <QRect>

class LineClipping {
 protected:
  typedef int OutCode;

  const int INSIDE = 0b0000;
  const int LEFT = 0b0001;
  const int RIGHT = 0b0010;
  const int BOTTOM = 0b0100;
  const int TOP = 0b1000;

  int xmin = 0, xmax = 0, ymin = 0, ymax = 0;
 public:
  LineClipping() = default;
  LineClipping(const QRect &rect) {
      setBorders(rect);
  }
  void setBorders(const QRect &rect) {
    xmin = rect.left();
    xmax = rect.right();
    ymin = rect.top();
    ymax = rect.bottom();
  }
  OutCode ComputeCode(double x, double y) {
    OutCode code;

    code = INSIDE;

    if (x < xmin)
      code |= LEFT;
    else if (x > xmax)
      code |= RIGHT;

    if (y < ymin)
      code |= BOTTOM;
    else if (y > ymax)
      code |= TOP;

    return code;
  }
  QPair<bool, QLine> CohenSutherland(const QLine& line) {
     return CohenSutherland(line.p1(),line.p2());
  }

  QPair<bool, QLine> CohenSutherland(const QPoint &begin, const QPoint &end) {

    double x0 = begin.x();
    double y0 = begin.y();

    double x1 = end.x();
    double y1 = end.y();
    
    OutCode code0 = ComputeCode(x0, y0);
    OutCode code1 = ComputeCode(x1, y1);
    bool is_visible = false;

    while (true) {
      if ((code0 | code1) == 0b0000) {
        is_visible = true;
        break;
      } else if ((code0 & code1) != 0b0000 ) {
        break;
      } else {
        double new_x, new_y;

        OutCode currentCode = code1 > code0 ? code1 : code0;

        if (currentCode & TOP) {
          new_x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
          new_y = ymax;
        } else if (currentCode & BOTTOM) {
          new_x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
          new_y = ymin;
        } else if (currentCode & RIGHT) {
          new_y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
          new_x = xmax;
        } else if (currentCode & LEFT) {
          new_y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
          new_x = xmin;
        }

        if (currentCode == code0) {
          x0 = new_x;
          y0 = new_y;
          code0 = ComputeCode(x0, y0);
        } else {
          x1 = new_x;
          y1 = new_y;
          code1 = ComputeCode(x1, y1);
        }
      }
    }

    return QPair<bool, QLine>(
                is_visible,
                QLine(floor(x0), floor(y0), floor(x1), floor(y1))
                );
  }
};

#endif
