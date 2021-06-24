#include "figuremanager.h"
#include <sstream>

FigureManager::FigureManager()
{

}

void FigureManager::addPoint(const QPoint &point)
{

    if (is_ended)
    {
        figures.push_back(QVector<QPoint>{point});
        is_ended = false;
    }else{
        figures.rbegin()->append(point);
    }

    updateFn();
}

void FigureManager::endFigure()
{
    is_ended = true;

    updateFn();
}

void FigureManager::clear()
{
    is_ended = true;
    figures = {};
    updateFn();
}

QPoint FigureManager::getLastPoint()
{
    return *figures.rbegin()->rbegin();
}

const QVector<QVector<QPoint> >* FigureManager::getFigures()
{
    return &figures;
}

QString FigureManager::getText()
{
    std::string out = "";
    std::ostringstream stream(out);

    int j = 1;
    for (auto fig: figures)
    {
        int i = 1;
        stream << "Фигура " << j << std::endl;
        for (auto point: fig)
        {
            stream << "   " << i << ". (" << point.x() << ";" << point.y() << ")" << std::endl;
            i++;
        }
        j++;
    }
    return QString::fromStdString(stream.str());
}

void FigureManager::setUpdate(std::function<void ()> fn)
{
    this->updateFn = fn;
}

