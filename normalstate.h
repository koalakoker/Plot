#ifndef NORMALSTATE_H
#define NORMALSTATE_H

#include "state.h"
#include <QWidget>

class NormalState : public State
{
public:
    NormalState();

    void mousePressEvent(WPlot& plot, QMouseEvent* event);
    void mouseMoveEvent(WPlot& plot, QMouseEvent* event);
    void mouseDoubleClickEvent(WPlot& plot, QMouseEvent* event);
    void keyPressEvent(WPlot& plot, QKeyEvent* event);
    void keyReleaseEvent(WPlot& plot, QKeyEvent* event) {};
    void setCursor(WPlot& plot);
};

#endif // NORMALSTATE_H
