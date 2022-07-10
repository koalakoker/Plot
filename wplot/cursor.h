#ifndef CURSOR_H
#define CURSOR_H

#include <QPainter>
#include <QPen>

class Plotter;

class Cursor : public QObject {
    Q_OBJECT

public:
    QVector<qreal> m_pos;

    Cursor(Plotter* plotter);
    void PlotCursor(QPainter& p, QPen& pen);
    int getCursorNumber(void) {return m_pos.size();}
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

private:
    Plotter* plotter;
    void plotValuesNearCursor(QPainter &p, int cur);

    QVector<QRect> m_cursorRect;
    int m_cursorDrag; // 0 none, index + 1 (zero based) if cursor index is dragged
    const int m_cursorMargin = 5;

signals:
    void cursorChanged();

};

#endif // CURSOR_H
