#ifndef ZOOM_H
#define ZOOM_H

class Axis;

#include <QPainter>
#include <QPen>

class Zoom : public QObject
{
    Q_OBJECT

private:
    Axis* axis;

public:
    Zoom(Axis* axis);
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

    // Undo-Redo
    void Undo(void);
    void Redo(void);
    void AddUndoStatus(void);

    void PlotZoomTracks(QPainter& p, QPen& pen);

    // Zoom tracks
    bool zoomXTrackVisible[2] = {false, false};
    qreal zoomXTracksPos[2] = {0.0, 0.0};
    bool zoomYTrackVisible[2] = {false, false};
    qreal zoomYTracksPos[2] = {0.0, 0.0};

    // Undo-Redo
    QVector<QRectF> m_undoRangeHystory;
    QVector<QRectF> m_redoRangeHystory;
    bool m_debounce;

private slots:
    void endTimer();
};

#endif // ZOOM_H
