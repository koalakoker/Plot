#include "vzoomstate.h"
#include "wplot/wplot.h"
#include "wplot/cursors.h"

VZoomState::VZoomState()
{

}
void VZoomState::mousePressEvent(WPlot& plot, QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        this->m_drag = true;
        plot.m_plotter->startZoomYTrack(event->pos());
        plot.m_lastPoint = event->pos();
        plot.updatePlot();
        event->accept();
    }
}
void VZoomState::mouseReleaseEvent(WPlot& plot, QMouseEvent* event) {
    this->m_drag = false;
    plot.m_plotter->AddUndoStatus();
    plot.m_plotter->zoomYToZoomRange();
    plot.updatePlot();
    event->accept();
}
void VZoomState::mouseMoveEvent(WPlot& plot, QMouseEvent* event) {
    if (this->m_drag) {
        QPoint delta =  event->pos() - plot.m_lastPoint;
        plot.m_lastPoint = event->pos();
        plot.m_plotter->zoomTrackScrollPixelY(delta.y());
        plot.updatePlot();
        event->accept();
    }
}
void VZoomState::mouseDoubleClickEvent(WPlot& plot, QMouseEvent* event) {Q_UNUSED(plot); Q_UNUSED(event);}
void VZoomState::keyPressEvent(WPlot& plot, QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        plot.state = &plot.normalState;
        plot.state->setCursor(plot);
        event->accept();
    } else {
        State::keyPressEvent(plot, event);
    }
}
void VZoomState::keyReleaseEvent(WPlot& plot, QKeyEvent* event) {Q_UNUSED(plot); Q_UNUSED(event);}
void VZoomState::setCursor(WPlot& plot) {
    plot.setCursor(Cursors::get(Cursors::ZoomIn));
}
