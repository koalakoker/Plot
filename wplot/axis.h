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

    void PlotAxis(QPainter& p, QPen& pen);
    void setRangeX_Min(qreal val) {m_range.setLeft  (val);}
    void setRangeX_Max(qreal val) {m_range.setRight (val);}
    void setRangeY_Min(qreal val) {m_range.setBottom(val);}
    void setRangeY_Max(qreal val) {m_range.setTop   (val);}

    bool m_axsisBottom = false;
    bool m_axsisLeft = false;
    bool m_axsisTop = false;
    bool m_axsisRight = false;

    QPointF m_axisDiv;
    QRectF m_range;
    bool m_axisDivVisible[2] = {true, true};    
};

#endif // AXIS_H
