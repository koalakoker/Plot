#include "curve.h"
#include "plotter.h"

Curve::Curve(Plotter* plotter, style_t style): plotter(plotter), m_style(style)
{

}

void Curve::plot(QPainter &p, QPen &pen) {
    if (m_data.size() != 0)
    {
        int trackNum = m_data[0].size();
        int sampleNum = m_data.size();

        double x0 = plotter->invMapX(0);
        double x1 = plotter->invMapX(1);
        double dX = x1-x0;
        double dY = m_data[1][0]-m_data[0][0];
        int di = dX/dY;
        if (di <= 0)
        {
            di = 1;
        }

        QVector<double> tSample;
        if (trackNum > 1)
        {
            tSample.reserve(sampleNum);
        }
        for (int track = 1; track < trackNum; track++) // Skip index 0 that is the time (x)
        {
            pen.setColor(plotColor[track-1]);
            p.setPen(pen);

            switch (m_style)
            {
            case POINT_STYLE:
                {
                    if (track == 1) /* See below the optimizaton description */
                    {
                        for (int i = 0; i < m_data.size(); i++)
                        {
                            QPointF point;
                            p.drawPoint(point = plotter->map(m_data[i][0], m_data[i][track]));
                            tSample.append(point.x());
                        }
                    }
                    else
                    {
                        for (int i = 0; i < m_data.size(); i++)
                        {
                            p.drawPoint(QPointF(tSample[i], plotter->mapY(m_data[i][track])));
                        }
                    }
                }
                break;
            case LINE_STYLE:
                {
                    if (track == 1) /* For first trac are computed t/x values and stored in temporary array tSample */
                    {
                        QPointF nextP, prevP = plotter->map(m_data[0][0], m_data[0][track]); /* To track the line the prev point is used */
                        tSample.append(prevP.x());
                        for (int i = 0; i < m_data.size() - di; i+=di)
                        {
                            p.drawLine(prevP, nextP = plotter->map(m_data[i+1][0], m_data[i+1][track]));
                            prevP = nextP;
                            tSample.append(nextP.x());
                        }

                    }
                    else /* For the t/x value are used the preprocessed values */
                    {
                        int j = 0;
                        QPointF nextP, prevP = QPointF(tSample[j], plotter->mapY(m_data[0][track]));
                        for (int i = 0; i < m_data.size() - di; i+=di)
                        {
                            p.drawLine(prevP, nextP = QPointF(tSample[j+1], plotter->mapY(m_data[i+1][track])));
                            prevP = nextP;
                            j++;
                        }
                    }
                }
                break;
            default:
                break;
            }
        }
    }

}
