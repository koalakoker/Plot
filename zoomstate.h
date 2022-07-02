#ifndef ZOOMSTATE_H
#define ZOOMSTATE_H

#include "state.h"

class ZoomState : public State
{
public:
    ZoomState();

    void mousePressEvent(WPlot& plot, QMouseEvent* event);
    void mouseMoveEvent(WPlot& plot, QMouseEvent* event);
};

#endif // ZOOMSTATE_H
