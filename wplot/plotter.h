#ifndef PLOTTER_H
#define PLOTTER_H

#include <QObject>
#include <QPainter>

#include "wplot/sdata.h"

const Qt::GlobalColor plotColor[] = {Qt::black, Qt::blue, Qt::green, Qt::red, Qt::yellow, Qt::gray};

class Plotter : public QObject {
    Q_OBJECT

public:
    typedef enum {
        POINT_STYLE,
        LINE_STYLE
    } style_t;

    Plotter(QSize size, QRectF range, QVector<SData> data, style_t style = LINE_STYLE);

    void setRangeX_Min(qreal val) {m_range.setLeft  (val);}
    void setRangeX_Max(qreal val) {m_range.setRight (val);}
    void setRangeY_Min(qreal val) {m_range.setBottom(val);}
    void setRangeY_Max(qreal val) {m_range.setTop   (val);}

    // Scroll
    void scrollX(qreal val);
    void scrollY(qreal val);
    void scrollXpixel(int pix);
    void scrollYpixel(int pix);

    // Zoom
    void zoomX(qreal val);
    void zoomY(qreal val);
    void zoomXToCursors(QPoint point);
    void zoomXToZoomRange(void);
    void zoomYToZoomRange(void);
    void unZoom(void);
    void startZoomXTrack(QPoint point);
    void zoomTrackScrollPixelX(int pix);
    void startZoomYTrack(QPoint point);
    void zoomTrackScrollPixelY(int pix);

    // Resize
    void setSize(QSize size) {m_size = size;}

    // Cursors
    int getCursorNumber(void) {return m_cursorPos.size();}
    void addCursor(void);
    void addCursor(qreal pos);
    void addCursorAtPixel(int pos);
    void removeCursor(int index);
    void setCursorPos(int index, qreal pos);
    void cursorScroll(int index, qreal pos);
    void cursorScrollPixel(int index, int pix);
    bool onCursor(QPoint point, int& selectedCursor, bool startDrag = false);
    void dragCursor(int index);
    void releaseCursor();
    int getCursorDragged();
    QVector<QVector<double>> getCursorValueTrack(void);
    QVector<double> getCursorValueTrack(int cur);
    QVector<double> getSelectedCursorValueTrack(void);

    // Axis
    bool m_axsisBottom = false;
    bool m_axsisLeft = false;
    bool m_axsisTop = false;
    bool m_axsisRight = false;
    QRectF m_range;
    QPointF m_axisDiv;
    bool m_axisDivVisible[2] = {true, true};

    // Undo-Redo
    void Undo(void);
    void Redo(void);
    void AddUndoStatus(void);

signals:
    void done(QImage);
    void cleanup();
    void cursorChanged();

public slots:
    QImage plot();
    void endTimer();

private:
    void PlotData(QPainter& p, QPen& pen);
    void PlotCursor(QPainter& p, QPen& pen);
    void PlotAxis(QPainter& p, QPen& pen);
    void PlotZoomTracks(QPainter& p, QPen& pen);

    QPointF map(double x, double y);
    double invMapX(double x);
    double invMapY(double y);
    double mapX(double x);
    double mapY(double y);

    QSize m_size;
    QVector<SData> m_data;

    style_t m_style;

    // Cursors
    void plotValuesNearCursor(QPainter &p, int cur);
    QVector<qreal> m_cursorPos;
    QVector<QRect> m_cursorRect;
    int m_cursorDrag; // 0 none, index + 1 (zero based) if cursor index is dragged
    const int m_cursorMargin = 5;

    // Zoom tracks
    bool zoomXTrackVisible[2] = {false, false};
    qreal zoomXTracksPos[2] = {0.0, 0.0};
    bool zoomYTrackVisible[2] = {false, false};
    qreal zoomYTracksPos[2] = {0.0, 0.0};

    // Undo-Redo
    QVector<QRectF> m_undoRangeHystory;
    QVector<QRectF> m_redoRangeHystory;
    bool m_debounce;
};

#endif // PLOTTER_H
