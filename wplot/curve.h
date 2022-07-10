#ifndef CURVE_H
#define CURVE_H

#include "wplot/sdata.h"

#include <QPainter>
#include <QPen>

class Plotter;

class Curve
{
private:
    Plotter* plotter;

public:
    typedef enum {
        POINT_STYLE,
        LINE_STYLE
    } style_t;

    Curve(Plotter* plotter, style_t style = LINE_STYLE);
    void PlotData(QPainter& p, QPen& pen);
    QVector<SData> m_data;
    style_t m_style;
};

#endif // CURVE_H
