#include "vzoomstate.h"
#include "wplot/wplot.h"
#include "wplot/cursors.h"

VZoomState::VZoomState()
{

}

void VZoomState::mousePressEvent(WPlot& plot, QMouseEvent* event) {}
void VZoomState::mouseReleaseEvent(WPlot& plot, QMouseEvent* event) {}
void VZoomState::mouseMoveEvent(WPlot& plot, QMouseEvent* event) {}
void VZoomState::mouseDoubleClickEvent(WPlot& plot, QMouseEvent* event) {}
void VZoomState::keyPressEvent(WPlot& plot, QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        plot.state = &plot.normalState;
        plot.state->setCursor(plot);
        event->accept();
    }
}
void VZoomState::keyReleaseEvent(WPlot& plot, QKeyEvent* event) {}
void VZoomState::setCursor(WPlot& plot) {
    plot.setCursor(Cursors::get(Cursors::ZoomIn));
}
