#ifndef NORMALSTATE_H
#define NORMALSTATE_H

#include "state.h"

class NormalState : public State
{
public:
    NormalState();

    void mousePressEvent(WPlot& plot, QMouseEvent* event);
    void mouseMoveEvent(WPlot& plot, QMouseEvent* event);
};

#endif // NORMALSTATE_H
