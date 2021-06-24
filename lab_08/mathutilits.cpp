#include "mathutilits.h"
#include <algorithm>

MathUtilits::MathUtilits()
{

}

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

std::optional<QLineF> MathUtilits::clip(const QLineF &section, QVector<QPointF> &points, QVector<QPointF> &normals)
{
    //  концы отрезка
    QVector<double> t_start_list = {0};
    QVector<double> t_end_list = {1};
//  Вектор направления отрезка ( вектор = [P2 - P1] )
    auto d = section.p2() - section.p1();

//  Цикл по всем граням многоугольника и поиск параметров t точек пересечения

    for (int i = 0; i < points.length(); ++i)
    {
        //В общем случае в качестве "точки многоугольника" берется начальная вершина
        //этой грани, однако если она совпадает с точкой начала отрезка, берется
        //конечная точка грани
        auto wi = section.p1() - points[(i + 1) % points.length()];

        if (points[i] == section.p1())
        {
            wi = section.p1() - points[i];
        }

        // Скалярное произведение вектора нормали и вектора ориентации (если = 0, то вектор
        //  параллелен стороне многоугольника)
        auto Dck = scalar_mul(d, normals[i]);
        // Скалярное произведение вектора нормали и вектора от "точки многоугольника" до
        // начала отрезка (если оно = 0, то начало отрезка лежит на рассматриваемой
        // грани многоугольника)
        auto Wck = scalar_mul(wi, normals[i]);

        // Если отрезок параллелен грани, и лежит вне многоугольника - выход
        if (Dck == 0){
            if (scalar_mul(wi, normals[i]) < 0)
                return std::nullopt;
            continue;
        }

        // Параметр t, соответствующий точке пересечения рассматриваемого отрезка
        // с очередной гранью
        auto t = -Wck / Dck;

        // Если Dck > 0 - точка входа в многоугольник
        if (Dck > 0)
            t_start_list.append(t);
        else
            t_end_list.append(t);
    }

    // Видимый отрезок находится между "последним" входом и "первым" выходом
    auto t_start = *std::max_element(t_start_list.begin(), t_start_list.end());
    auto t_end = *std::min_element(t_end_list.begin(), t_end_list.end());

    // Если "входной" t < "выходной" t, то отрезок видимый
    if (t_start < t_end)
    {
        QPointF p1 = {section.p1().x() + d.x() * t_start, section.p1().y() + d.y() * t_start};
        QPointF p2 = {section.p1().x() + d.x() * t_end, section.p1().y() + d.y() * t_end};
        return QLineF(p1, p2);
    }

    return std::nullopt;
}
