#include "zoom.h"
#include "axis.h"
#include "plotter.h"

#include <QTimer>

Zoom::Zoom(Axis *axis): axis(axis), m_debounce(false) {}

void Zoom::PlotZoomTracks(QPainter& p, QPen& pen) {
    QRectF m_range = axis->m_range;
    for (int i = 0; i < 2; i++) {
        if (this->zoomXTrackVisible[i]) {
            qreal curXpos       = this->zoomXTracksPos[i];
            qreal curYposTop    = m_range.y();
            qreal curYposBottom = m_range.y() + m_range.height();

            pen.setColor(Qt::black);
            p.setPen(pen);

            p.drawLine(axis->plotter->map(curXpos, curYposTop), axis->plotter->map(curXpos, curYposBottom));
        }
    }
    for (int i = 0; i < 2; i++) {
        if (this->zoomYTrackVisible[i]) {
            qreal curYpos       = this->zoomYTracksPos[i];
            qreal curXposLeft    = m_range.x();
            qreal curXposRigth = m_range.x()+ m_range.width();

            pen.setColor(Qt::black);
            p.setPen(pen);

            p.drawLine(axis->plotter->map(curXposLeft, curYpos), axis->plotter->map(curXposRigth, curYpos));
        }
    }
}

void Zoom::zoomX(qreal val)
{
    QRectF m_range = axis->m_range;
    qreal delta = m_range.width() * val * 0.05;
    axis->m_range.setLeft(m_range.left() + delta);
    axis->m_range.setRight(m_range.right() - delta);
}
void Zoom::zoomY(qreal val)
{
    QRectF m_range = axis->m_range;
    qreal delta = m_range.height() * val * 0.05;
    axis->m_range.setTop   (m_range.top()    + delta);
    axis->m_range.setBottom(m_range.bottom() - delta);
}
void Zoom::zoomXToCursors(QPoint point)
{
    QRectF m_range = axis->m_range;
    qreal xMin,xMax;
    qreal xPos = axis->plotter->invMapX(point.x());

    //QVector<qreal> xPoints;
    std::vector<qreal> xPoints;
    xPoints.push_back(xMin = m_range.left());
    xPoints.push_back(xMax = m_range.right());

    int i;
    for (i = 0; i < axis->plotter->cursor->m_pos.size(); i++)
    {
        xPoints.push_back(axis->plotter->cursor->m_pos.at(i));
    }


    std::sort(xPoints.begin(), xPoints.end());

    for (i = 0; i < (int)(xPoints.size()); i++)
    {
        if (xPos < xPoints.at(i))
        {
            xMin = xPoints.at(i-1);
            xMax = xPoints.at(i);
            break;
        }
    }

    if ((m_range.left() == xMin) && (m_range.right() == xMax))
    {
        zoomX(5);
    }
    else
    {
        axis->m_range.setLeft(xMin);
        axis->m_range.setRight(xMax);
    }
}
void Zoom::zoomXToZoomRange() {
    qreal left  = this->zoomXTracksPos[0];
    qreal rigth = this->zoomXTracksPos[1];

    for (int i = 0; i < 2;  i++) {
        this->zoomXTrackVisible[i] = false;
    }

    if (left == rigth) return;
    if (left > rigth) {
        qreal tmp = left;
        left = rigth;
        rigth = tmp;
    }
    axis->m_range.setLeft(left);
    axis->m_range.setRight(rigth);
}
void Zoom::zoomYToZoomRange(void) {
    qreal top    = this->zoomYTracksPos[0];
    qreal bottom = this->zoomYTracksPos[1];

    for (int i = 0; i < 2;  i++) {
        this->zoomYTrackVisible[i] = false;
    }

    if (top == bottom) return;
    if (top > bottom) {
        qreal tmp = top;
        top = bottom;
        bottom = tmp;
    }
    axis->m_range.setTop(top);
    axis->m_range.setBottom(bottom);
}
void Zoom::unZoom(void)
{
    zoomX(-5);
}
void Zoom::startZoomXTrack(QPoint point) {
    for (int i = 0;  i < 2; i++) {
        this->zoomXTrackVisible[i] = true;
        this->zoomXTracksPos[i] = axis->plotter->invMapX(point.x());
    }
}
void Zoom::zoomTrackScrollPixelX(int pix) {
    QRectF m_range = axis->m_range;
    this->zoomXTracksPos[1] += (m_range.width()  * (qreal)(pix))/(qreal)(axis->plotter->m_size.width ());
}
void Zoom::startZoomYTrack(QPoint point) {
    for (int i = 0;  i < 2; i++) {
        this->zoomYTrackVisible[i] = true;
        this->zoomYTracksPos[i] = axis->plotter->invMapY(point.y());
    }
}
void Zoom::zoomTrackScrollPixelY(int pix) {
    QRectF m_range = axis->m_range;
    this->zoomYTracksPos[1] -= (m_range.height()  * (qreal)(pix))/(qreal)(axis->plotter->m_size.height());
}

// Undo
void Zoom::Undo(void)
{
    QRectF m_range = axis->m_range;
    if (!m_undoRangeHystory.isEmpty())
    {
        m_redoRangeHystory.append(m_range);
        axis->m_range = m_undoRangeHystory.takeLast();
    }
}
void Zoom::Redo(void)
{
    QRectF m_range = axis->m_range;
    if (!m_redoRangeHystory.isEmpty())
    {
        m_undoRangeHystory.append(m_range);
        axis->m_range = m_redoRangeHystory.takeLast();
    }
}
void Zoom::AddUndoStatus(void)
{
    QRectF m_range = axis->m_range;
    if (!m_debounce)
    {
        m_undoRangeHystory.append(m_range);
        m_redoRangeHystory.clear();
        QTimer::singleShot(500, this, SLOT(endTimer()));
        m_debounce = true;
    }
}
void Zoom::endTimer()
{
    m_debounce = false;
}

