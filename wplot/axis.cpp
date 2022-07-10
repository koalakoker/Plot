#include "axis.h"
#include "plotter.h"

Axis::Axis(Plotter *plotter, QRectF range) : plotter(plotter) {
    float defDiv = 5.0;
    m_range = range;
    m_axisDiv.setX((range.right()-range.left())/defDiv);
    m_axisDiv.setY((range.bottom()-range.top())/defDiv);
    zoom = new Zoom(this);
}

void Axis::PlotAxis(QPainter& p, QPen& pen) {
    int maxDiv;
    int divLen = 10;
    int hSpacer = 5;
    int vSpacer = 5;
    int w = plotter->m_size.width();
    int h = plotter->m_size.height();

    if (m_axisDivVisible[1]) {
        maxDiv = (int)((m_range.bottom() - m_range.top()) / m_axisDiv.y());
        for (int div = -maxDiv; div <= maxDiv; div++) {

            if (div == 0) {
                pen.setColor(Qt::black);
                pen.setStyle(Qt::SolidLine);
            } else {
                pen.setColor(Qt::lightGray);
                pen.setStyle(Qt::DotLine);
            }
            p.setPen(pen);

            qreal Ypos      = -(m_axisDiv.y() * div);
            qreal XposLeft  = m_range.x();
            qreal XposRigth = m_range.x() + m_range.width();
            p.drawLine(plotter->map(XposLeft, Ypos), plotter->map(XposRigth, Ypos));

            if (m_axsisLeft) {
                pen.setColor(Qt::black);
                pen.setStyle(Qt::SolidLine);
                p.setPen(pen);
                QFontMetrics fm(p.font());

                QPointF point = plotter->map(XposLeft, Ypos);
                int y = (int)(point.y());
                p.drawLine(QPoint(0, y), QPoint(divLen, y));
                QString valueStr = QString::number(plotter->invMapY(y),'g', 3);
                QSize sz = fm.size(Qt::TextSingleLine,valueStr);
                p.drawText(QPoint(divLen + hSpacer, y + (sz.height()/2) - hSpacer) , valueStr);
            }
            if (m_axsisRight) {
                pen.setColor(Qt::black);
                pen.setStyle(Qt::SolidLine);
                p.setPen(pen);
                QFontMetrics fm(p.font());

                QPointF point = plotter->map(XposRigth, Ypos);
                int y = (int)(point.y());
                p.drawLine(QPoint(w, y), QPoint(w - divLen, y));
                QString valueStr = QString::number(plotter->invMapY(y),'g', 3);
                QSize sz = fm.size(Qt::TextSingleLine,valueStr);
                p.drawText(QPoint(w - divLen - hSpacer - sz.width(), y + (sz.height()/2) - hSpacer) , valueStr);
            }
        }
    }
    if (m_axisDivVisible[0]) {
        maxDiv = (int)((m_range.right() - m_range.left()) / m_axisDiv.x());
        for (int div = -maxDiv; div <= maxDiv; div++) {

            if (div == 0) {
                pen.setColor(Qt::black);
                pen.setStyle(Qt::SolidLine);
            } else {
                pen.setColor(Qt::lightGray);
                pen.setStyle(Qt::DotLine);
            }
            p.setPen(pen);

            qreal Xpos       = -(m_axisDiv.x() * div);
            qreal YposTop    = m_range.y();
            qreal YposBottom = m_range.y() + m_range.height();
            p.drawLine(plotter->map(Xpos, YposTop), plotter->map(Xpos, YposBottom));

            if (m_axsisBottom) {
                pen.setColor(Qt::black);
                pen.setStyle(Qt::SolidLine);
                p.setPen(pen);

                QPointF bottomPoint = plotter->map(Xpos, YposBottom);
                int x = (int)(bottomPoint.x());
                p.drawLine(QPoint(x, h), QPoint(x, h - divLen));
                QString valueStr = QString::number(plotter->invMapX(x),'g', 3);
                p.drawText(QPoint(x + vSpacer, h - hSpacer), valueStr);
            }

            if (m_axsisTop) {
                pen.setColor(Qt::black);
                pen.setStyle(Qt::SolidLine);
                p.setPen(pen);
                QFontMetrics fm(p.font());

                QPointF topPoint = plotter->map(Xpos, YposTop);
                int x = (int)(topPoint.x());
                p.drawLine(QPoint(x, 0), QPoint(x, divLen));
                QString valueStr = QString::number(plotter->invMapX(x),'g', 3);
                QSize sz = fm.size(Qt::TextSingleLine,valueStr);
                p.drawText(QPoint(x + vSpacer, sz.height()), valueStr);
            }
        }
    }

    zoom->PlotZoomTracks(p, pen);
}
