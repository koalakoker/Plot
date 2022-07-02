#include "zoomstate.h"
#include "wplot.h"
#include "cursors.h"

ZoomState::ZoomState()
{

}

void ZoomState::mousePressEvent(WPlot& plot, QMouseEvent* event) {
    if (event->button() == plot.dragButton) {
        plot.m_plotter->AddUndoStatus();
        plot.m_plotter->zoomXToCursors(event->pos());
        plot.updatePlot();
    }
}

void ZoomState::mouseMoveEvent(WPlot& plot, QMouseEvent* event) {

}

void ZoomState::setCursor(WPlot& plot) {
    plot.setCursor(Cursors::get(Cursors::ZoomIn));
}
