#include "normalstate.h"
#include "wplot.h"

NormalState::NormalState()
{

}

void NormalState::mousePressEvent(WPlot& plot, QMouseEvent* event) {
    if (event->button() == plot.dragButton)
    {
        int selected = 0;
        plot.m_lastPoint = event->pos();
        if (plot.m_plotter->onCursor(plot.m_lastPoint, selected, true))
        {

        }
        else
        {
            plot.m_movingUndo = true;
            plot.m_drag = true;
        }

    }
}

void NormalState::mouseMoveEvent(WPlot& plot, QMouseEvent* event) {
    if (plot.m_drag)
    {
        if (plot.m_movingUndo) // Add undo to be done only once
        {
            plot.m_plotter->AddUndoStatus();
            plot.m_movingUndo = false;
        }

        QPoint delta = plot.m_lastPoint - event->pos();
        plot.m_lastPoint = event->pos();
        plot.m_plotter->scrollXpixel( delta.x());
        plot.m_plotter->scrollYpixel(-delta.y());
        plot.updatePlot();
    }

    int index = plot.m_plotter->getCursorDragged();
    if (index != 0)
    {
        QPoint delta =  event->pos() - plot.m_lastPoint;
        plot.m_lastPoint = event->pos();
        plot.m_plotter->cursorScrollPixel(index-1, delta.x());
        plot.updatePlot();
    }

    int selected = 0;
    if (plot.m_plotter->onCursor(event->pos(), selected, false))
    {
        plot.setCursor(Qt::SizeHorCursor);
    }
    else
    {
        plot.setCursor(Qt::ArrowCursor);
    }
}

void NormalState::keyPressEvent(WPlot& plot, QKeyEvent* event) {
    event->ignore();
}

void NormalState::setCursor(WPlot& plot) {
    plot.setCursor(Qt::ArrowCursor);
}
