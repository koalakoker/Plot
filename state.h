#ifndef STATE_H
#define STATE_H

#include <QMouseEvent>

class WPlot;

class State
{
public:
    State();
    virtual void mousePressEvent(WPlot& plot, QMouseEvent* event) = 0;
    virtual void mouseMoveEvent(WPlot& plot, QMouseEvent* event) = 0;
};

#endif // STATE_H
