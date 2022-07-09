#include "wplot/state/hzoomstate.h"
#include "wplot/wplot.h"
#include "wplot/cursors.h"

HZoomState::HZoomState()
{

}
void HZoomState::mousePressEvent(WPlot& plot, QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        this->m_drag = true;
        plot.m_plotter->startZoomXTrack(event->pos());
        plot.m_lastPoint = event->pos();
        plot.updatePlot();
        event->accept();
    }
}
void HZoomState::mouseReleaseEvent(WPlot& plot, QMouseEvent* event) {
    this->m_drag = false;
    plot.m_plotter->AddUndoStatus();
    plot.m_plotter->zoomXToZoomRange();
    plot.updatePlot();
    event->accept();
}
void HZoomState::mouseMoveEvent(WPlot& plot, QMouseEvent* event) {
    if (this->m_drag) {
        QPoint delta =  event->pos() - plot.m_lastPoint;
        plot.m_lastPoint = event->pos();
        plot.m_plotter->zoomTrackScrollPixelX(delta.x());
        plot.updatePlot();
        event->accept();
    }
}
void HZoomState::mouseDoubleClickEvent(WPlot& plot, QMouseEvent* event) {Q_UNUSED(plot); Q_UNUSED(event);}
void HZoomState::keyPressEvent(WPlot& plot, QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        plot.state = &plot.normalState;
        plot.state->setCursor(plot);
        event->accept();
    } else {
        State::keyPressEvent(plot, event);
    }
}
void HZoomState::keyReleaseEvent(WPlot& plot, QKeyEvent* event) {Q_UNUSED(plot); Q_UNUSED(event);}
void HZoomState::setCursor(WPlot& plot) {
    plot.setCursor(Cursors::get(Cursors::ZoomIn));
}
