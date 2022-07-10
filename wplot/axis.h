#ifndef AXIS_H
#define AXIS_H

#include "zoom.h"

#include <QPainter>
#include <QPen>

class Plotter;

class Axis
{
public:
    Axis(Plotter* plotter, QRectF range);
    Plotter* plotter;
    Zoom* zoom;

    void plot(QPainter& p, QPen& pen);
    void setRangeX_Min(qreal val) {m_range.setLeft  (val);}
    void setRangeX_Max(qreal val) {m_range.setRight (val);}
    void setRangeY_Min(qreal val) {m_range.setBottom(val);}
    void setRangeY_Max(qreal val) {m_range.setTop   (val);}

    bool m_showBottom = false;
    bool m_showLeft = false;
    bool m_showTop = false;
    bool m_showRight = false;

    QRectF m_range;
    QPointF m_div;
    bool m_divVisible[2] = {true, true};
};

#endif // AXIS_H
