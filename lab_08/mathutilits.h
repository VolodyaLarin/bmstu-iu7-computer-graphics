#ifndef MATHUTILITS_H
#define MATHUTILITS_H

#include <QPointF>
#include <QVector>
#include <QLineF>

class MathUtilits
{
public:
    MathUtilits();

    static double vect_mul(const QPointF&v1, const QPointF&v2);
    static double scalar_mul(const QPointF&v1, const QPointF&v2);

    static QPointF get_normal(const QPointF&p1, const QPointF&p2, const QPointF&cp);
    static QVector<QPointF> get_normal_list(const QVector<QPointF>& points);

    static bool check_polygon(QVector<QPointF>& points);

    static std::optional<QLineF> clip(const QLineF& section, QVector<QPointF>& points, QVector<QPointF>& normals);
};

#endif // MATHUTILITS_H
