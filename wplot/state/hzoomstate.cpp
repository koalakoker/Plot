#include "wplot/state/hzoomstate.h"
#include "wplot/wplot.h"
#include "wplot/cursors.h"

HZoomState::HZoomState()
{

}

void HZoomState::mousePressEvent(WPlot& plot, QMouseEvent* event) {
    this->m_drag = true;
}
void HZoomState::mouseReleaseEvent(WPlot& plot, QMouseEvent* event) {
    this->m_drag = false;
}
void HZoomState::mouseMoveEvent(WPlot& plot, QMouseEvent* event) {}
void HZoomState::mouseDoubleClickEvent(WPlot& plot, QMouseEvent* event) {}
void HZoomState::keyPressEvent(WPlot& plot, QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        plot.state = &plot.normalState;
        plot.state->setCursor(plot);
        event->accept();
    }
}
void HZoomState::keyReleaseEvent(WPlot& plot, QKeyEvent* event) {}
void HZoomState::setCursor(WPlot& plot) {
    plot.setCursor(Cursors::get(Cursors::ZoomIn));
}
