#include "mathutilits.h"
#include <algorithm>
#include <cmath>

MathUtilits::MathUtilits() {}

double MathUtilits::vect_mul(const QPointF &v1, const QPointF &v2)
{
    return v1.x() * v2.y() - v1.y() * v2.x();
}

double MathUtilits::scalar_mul(const QPointF &v1, const QPointF &v2)
{
    return v1.x() * v2.x() + v1.y() * v2.y();
}

QPointF MathUtilits::get_normal(const QPointF &p1, const QPointF &p2, const QPointF &cp)
{
    auto vect = p2 - p1;
    // Если ищется нормаль к вертикальному вектору - то это нормаль [1, 0],
    // иначе вектор нормали находится из условия равенства 0 скалярного произведения
    // исходного вектора и искомого вектора нормали
    QPointF norm;
    if (vect.x() == 0) norm = {1, 0};
    else norm = {-vect.y() / vect.x(), 1};

    //  Если скалярное произведение найденного вектора нормали и вектора, совпадающего
    //  со следующей стороной многоугольника меньше нуля - нормаль направлена из
    //  многоугольника, берем обратный вектор


    if (scalar_mul(cp - p2, norm) < 0 ){
       norm.setX(-norm.x());
       norm.setY(-norm.y());
    }

    return norm;
}

QVector<QPointF> MathUtilits::get_normal_list(const QVector<QPointF> &points)
{
    auto len = points.length();
    QVector<QPointF> normal_list;
    for (int i = 0; i < len; ++i)
        normal_list.append(
                    get_normal(points[i], points[(i + 1) % len],
                    points[(i + 2) % len])
                );

    return normal_list;

}

bool MathUtilits::check_polygon(QVector<QPointF> &points)
{

    if (!check_polygon_intersects(points)) return false;
    if (points.length() < 3) return false;

   int sign = 1;

   if (vect_mul(points[2] - points[1], points[1] - points[0]) < 0)
       sign = -1;


    //   В цикле проверяем совпадения знаков векторных произведений
    //   всех пар соседних ребер со знаком первого
    //   векторного произведения
   auto len = points.length();
   for (int i = 3; i < len + 3; ++i)
   {
       // Возвращаем false при несовпадении знаков: прямоугольник невыпуклый
       if (sign * vect_mul(points[i % len] - points[(i - 1) % len], points[(i - 1) % len] - points[(i - 2) % len]) < 0)
           return false;
   }



   if (sign < 0)
   {
    auto p = points;
    for (int i = 0; i < p.length(); ++i){
        points[i] = p[p.length() - i - 1];
    }
   }

   return true;
}

bool MathUtilits::check_polygon_intersects(const QVector<QPointF> &points)
{
    if (points.size() == 3) return true;
    for (int i = 0; i < points.size(); ++i)
    {
        for (int j = 0; j < points.size(); ++j)
        {
            if (i == j) continue;
            if (
                    is_intersected(
                        QLineF(points[i], points[(i+1)%points.size()]),
                        QLineF(points[j], points[(j+1)%points.size()])
                    )
                ) return false;
        }
    }

    return true;
}

QPointF MathUtilits::get_vect(const QPointF& p1, const QPointF& p2)
{
    return p2 - p1;
}

bool MathUtilits::point_in_section(const QPointF &point, const QLineF& section)
{
    static auto is_lower = [](const QPointF &p1, const QPointF &p2)->bool {
        if (p1.x() < p2.x()) return true;
        return p1.x() == p2.x() &&  p1.y() < p2.y();
    };

    if (
            abs(
                vect_mul(
                    get_vect(point, section.p1()),
                    get_vect(section.p1(), section.p2())
                    )
                ) <= 1e-6)
    {
        if (
            (is_lower(section.p1(), point) && is_lower(point, section.p2())) ||
            (is_lower(section.p2(), point) && is_lower(point, section.p1()))
           )
        {
            return true;
        }
    }

    return false;
}


QVector<QLineF> MathUtilits::get_sections(const QLineF& section, const QVector<QPointF> &rest_points)
{
    static auto cmp = [](const QPointF &p1, const QPointF &p2)->bool {
        if (p1.x() < p2.x()) return -1;
        if (p1.x() == p2.x()) return p2.y() - p1.y();
        return 1;
    };

    QVector<QPointF> points_list = {section.p1(), section.p2()};

    for (const auto &p : rest_points)
    {
        if (point_in_section(p, section))
        {
            points_list.append(p);
        }
    }

    std::sort(points_list.begin(), points_list.end(), cmp);

    QVector<QLineF> sections_list = {};

    for (int i = 0; i < points_list.length() - 1; ++i)
    {
        sections_list.append(QLineF(points_list[i], points_list[i + 1]));
    }

    return sections_list;
}

bool MathUtilits::check_point(const QPointF& point, const QPointF& p1, const QPointF& p2)
{
    return vect_mul(get_vect(p1, p2), get_vect(p1, point)) <= 0;
}

QPointF MathUtilits::find_intersection(const QLineF& section, const QLineF& edge, const QPointF& normal)
{
    auto wi = get_vect(edge.p1(), section.p1());
    auto d = get_vect(section.p1(), section.p2());
    auto Wck = scalar_mul(wi, normal);
    auto Dck = scalar_mul(d, normal);

    QPointF diff = {section.dx(), section.dy()};

    auto t = -Wck / Dck;

    return {
        section.p1().x() + diff.x() * t,
        section.p1().y() + diff.y() * t
    };
}



QVector<QPointF> MathUtilits::edgecut_figure(const QVector<QPointF> &figure, const QLineF &edge, const QPointF& normal)
{
    QVector<QPointF> res_figure = {};

    if (figure.length() < 3)
        return {};
    auto prev_check = check_point(figure[0], edge.p1(), edge.p2());

    for (int i = 1; i < figure.length() + 1; ++i)
    {
        auto cur_check = check_point(figure[i % figure.length()], edge.p1(), edge.p2());

        if (prev_check)
        {
            if (cur_check) {
                res_figure.append(figure[i % figure.length()]);
            }
            else
            {
                res_figure.append(find_intersection(
                                      {figure[i - 1], figure[i % figure.length()]},
                                      edge,
                                      normal)
                                  );
            }
        }
        else {
             if (cur_check) {
                    res_figure.append(find_intersection(
                                          {figure[i - 1],figure[i % figure.length()]},
                                          edge,
                                          normal)
                                      );
                    res_figure.append(figure[i % figure.length()]);
             }
        }

        prev_check = cur_check;
    }

    return res_figure;
}

QVector<QPointF> MathUtilits::cut_figure(const QVector<QPointF> &figure,
                                         const QVector<QPointF> &cutter_verteces,
                                         const QVector<QPointF> &normals_list)
{
    QVector<QPointF> res_figure = figure;

    for (int i = 0; i < cutter_verteces.length(); ++i)
    {
        QLineF cur_edge = {
            cutter_verteces[i],
            cutter_verteces[(i + 1) % cutter_verteces.length()]
        };

        res_figure = edgecut_figure(res_figure, cur_edge,
                                    normals_list[i]);
        if (res_figure.length() < 3)
            return {};
    }

    return res_figure;
}

bool MathUtilits::is_intersected(const QLineF &l1, const QLineF &l2)
{
    auto denominator = l2.dy() * l1.dx() - l2.dy() * l1.dy();

    double x1 = l1.x1(), x2 = l1.x2(), x3 = l2.x1(), x4 = l2.x2();
    double y1 = l1.y1(), y2 = l1.y2(), y3 = l2.y1(), y4 = l2.y2();

    if (denominator == 0){
        return (x1*y2-x2*y1)*(x4-x3) - (x3*y4-x4*y3)*(x2-x1) == 0 &&
               (x1*y2-x2*y1)*(y4-y3) - (x3*y4-x4*y3)*(y2-y1) == 0;
    }

    double numerator_a=(x4-x2)*(y4-y3)-(x4-x3)*(y4-y2);
    double numerator_b=(x1-x2)*(y4-y2)-(x4-x2)*(y1-y2);
    double Ua=numerator_a/denominator;
    double Ub=numerator_b/denominator;
    return Ua >=0 && Ua <=1 && Ub >=0 && Ub <=1;
}

































//# Функция, которая удаляет ложные ребра
//def make_uniq(sections):
//    for section in sections:
//        section.sort()
//    return list(filter(lambda x: (sections.count(x) % 2) == 1, sections))



//# Функция выброса ложных ребер из результирующего многоугольника
//def get_uniq_sections(figure):
//    all_sections = list()
//    rest_points = figure[2:]
//    for i in range(len(figure)):
//        cur_section = [figure[i], figure[(i + 1) % len(figure)]]

//        all_sections.extend(get_sections(cur_section, rest_points))

//        rest_points.pop(0)
//        rest_points.append(figure[i])

//    return make_uniq(all_sections)


