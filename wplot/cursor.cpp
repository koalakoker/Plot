#include "cursor.h"
#include "plotter.h"
#include <QVector>

Cursor::Cursor(Plotter *plotter): plotter(plotter), m_cursorDrag(0)
{

}

void Cursor::PlotCursor(QPainter& p, QPen& pen) {
    /* Cursors */
    int curLabelYPos = 30;
    QRectF m_range = plotter->axis->m_range;
    m_cursorRect.clear();
    for (int cur = 0; cur < m_pos.size(); cur++)
    {
        qreal curXpos       = m_pos.at(cur);
        qreal curYposTop    = m_range.y();
        qreal curYposBottom = m_range.y()+m_range.height();

        pen.setColor(Qt::black);
        pen.setStyle(Qt::DashLine);
        p.setPen(pen);

        p.drawLine(plotter->map(curXpos, curYposTop), plotter->map(curXpos, curYposBottom));
        p.drawText(plotter->mapX(curXpos)+5,curLabelYPos,QString::number(cur+1) + "@" + QString::number(curXpos));

        QPointF top     = plotter->map(curXpos, curYposTop);
        QPointF bottom  = plotter->map(curXpos, curYposBottom);
        QPoint  topLeft    (top.x()    - m_cursorMargin, top.y());
        QPoint  bottomRight(bottom.x() + m_cursorMargin, bottom.y());
        QRect   rect(topLeft, bottomRight);
        m_cursorRect.append(rect);

        this->plotValuesNearCursor(p, cur);
    }
}

void Cursor::plotValuesNearCursor(QPainter& p, int cur) {
    QPen pen;
    QVector<double> values = getCursorValueTrack(cur);
    QVectorIterator<double> it(values);
    qreal curXpos = it.next();
    int i = 0;
    while (it.hasNext()) {
        double yValue = it.next();
        pen.setColor(plotColor[i]);
        pen.setStyle(Qt::SolidLine);
        p.setPen(pen);
        p.drawText(plotter->map(curXpos, yValue) + QPointF(5,0), QString::number(yValue));
        i++;
    }
}

void   Cursor::addCursor(void)
{
    qreal pos = (plotter->invMapX(plotter->m_size.width()) - plotter->invMapX(0))/2;
    addCursor(pos);
    emit cursorChanged();
}
void   Cursor::addCursor(qreal pos)
{
    m_pos.append(pos);
    emit cursorChanged();
}
void   Cursor::addCursorAtPixel(int pos)
{
    m_pos.append(plotter->invMapX(pos));
    emit cursorChanged();
}
void   Cursor::removeCursor(int index)
{
    m_pos.remove(index);
    emit cursorChanged();
}
void Cursor::setCursorPos(int index, qreal pos)
{
    m_pos[index] = pos;
    emit cursorChanged();
}

void   Cursor::cursorScroll(int index, qreal pos)
{
    m_pos[index] += pos;
    emit cursorChanged();
}
void   Cursor::cursorScrollPixel(int index, int pix)
{
    QRectF m_range = plotter->axis->m_range;
    cursorScroll(index, (m_range.width()  * (qreal)(pix))/(qreal)(plotter->m_size.width ()));
}
bool   Cursor::onCursor(QPoint point, int& selectedCursor, bool startDrag)
{
    bool retVal = false;
    for (int cursor = 0; cursor < m_cursorRect.size(); cursor++)
    {
        if (m_cursorRect.at(cursor).contains(point))
        {
            retVal = true;
            selectedCursor = cursor;
            if (startDrag)
            {
                dragCursor(cursor);
            }
        }
    }
    return retVal;
}
void   Cursor::dragCursor(int index)
{
    if ((index >= 0) && (index < m_pos.size()))
    {
        m_cursorDrag = index + 1; // 0 none, index + 1 (zero based) if cursor index is dragged
    }
}
void   Cursor::releaseCursor()
{
    m_cursorDrag = 0;
}
int    Cursor::getCursorDragged()
{
    return m_cursorDrag;
}
QVector<QVector<double>> Cursor::getCursorValueTrack(void)
{
    QVector<QVector<double>> cursorInfo;
    for (int cur = 0; cur < m_pos.size(); cur++)
    {
        cursorInfo.append(getCursorValueTrack(cur));
    }
    return cursorInfo;
}
QVector<double> Cursor::getCursorValueTrack(int cur)
{
    QVector<double> data;
    qreal x = m_pos.at(cur);
    qreal dx = plotter->curve->m_data[1][0] - plotter->curve->m_data[0][0];
    int i = (int)(x/dx);
    for (int track = 0; track < plotter->curve->m_data[0].size(); track++)
    {
        if ((i >= 0) && (i < plotter->curve->m_data.size()))
        {
            data.append(plotter->curve->m_data[i][track]);
        }
        else
        {
            data.append(0);
        }
    }
    return data;
}
QVector<double> Cursor::getSelectedCursorValueTrack(void)
{
    return getCursorValueTrack(m_cursorDrag);
}
