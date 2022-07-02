#ifndef NORMALSTATE_H
#define NORMALSTATE_H

#include "state.h"

class NormalState : public State
{
public:
    NormalState();

    void mousePressEvent(WPlot& plot, QMouseEvent* event);
    void mouseMoveEvent(WPlot& plot, QMouseEvent* event);
    void setCursor(WPlot& plot);
};

#endif // NORMALSTATE_H
