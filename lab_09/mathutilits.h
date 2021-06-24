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

    static bool check_polygon_intersects(const QVector<QPointF>& points);

    static QPointF get_vect(const QPointF& p1, const QPointF& p2);

    static bool point_in_section(const QPointF &point, const QLineF& section);

    static QVector<QLineF> get_sections(const QLineF& section, const QVector<QPointF> &rest_points);

    static bool check_point(const QPointF& point, const QPointF& p1, const QPointF& p2);

    static QPointF find_intersection(const QLineF& section, const QLineF& edge, const QPointF& normal);

    static QVector<QPointF> edgecut_figure(const QVector<QPointF> &figure, const QLineF &edge, const QPointF& normal);


    static QVector<QPointF> cut_figure(const QVector<QPointF> &figure, const QVector<QPointF> &cutter_verteces, const QVector<QPointF> &normals_list);


    static bool is_intersected(const QLineF&l1, const QLineF&l2);
};

#endif // MATHUTILITS_H
