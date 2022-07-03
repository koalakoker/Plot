#include "zoomstate.h"
#include "wplot.h"
#include "cursors.h"

ZoomState::ZoomState()
{

}

void ZoomState::mousePressEvent(WPlot& plot, QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        if (this->m_shiftKeyPressed)  {
            // Zoom Out
            plot.m_plotter->AddUndoStatus();
            plot.m_plotter->unZoom();
            plot.updatePlot();

        } else {
            // Zoom in
            plot.m_plotter->AddUndoStatus();
            plot.m_plotter->zoomXToCursors(event->pos());
            plot.updatePlot();
        }
    }
}

void ZoomState::mouseReleaseEvent(WPlot& plot, QMouseEvent* event) {}

void ZoomState::mouseMoveEvent(WPlot& plot, QMouseEvent* event) {

}

void ZoomState::mouseDoubleClickEvent(WPlot& plot, QMouseEvent* event) {
    if (this->m_shiftKeyPressed)  {
        // Zoom Out
        plot.m_plotter->AddUndoStatus();
        plot.m_plotter->unZoom();
        plot.updatePlot();

    } else {
        // Zoom in
        plot.m_plotter->AddUndoStatus();
        plot.m_plotter->zoomXToCursors(event->pos());
        plot.updatePlot();
    }
}

void ZoomState::keyPressEvent(WPlot& plot, QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        plot.state = &plot.normalState;
        plot.state->setCursor(plot);
        event->accept();
    }
    if (event->key() == Qt::Key_Shift) {
        this->m_shiftKeyPressed = true;
        plot.setCursor(Cursors::get(Cursors::ZoomOut));
    }
}

void ZoomState::keyReleaseEvent(WPlot& plot, QKeyEvent* event) {
    if (event->key() == Qt::Key_Shift) {
        this->m_shiftKeyPressed = false;
        plot.setCursor(Cursors::get(Cursors::ZoomIn));
    }
}

void ZoomState::setCursor(WPlot& plot) {
    plot.setCursor(Cursors::get(Cursors::ZoomIn));
}
