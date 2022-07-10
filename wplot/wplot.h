#ifndef WPLOT_H
#define WPLOT_H

#include <QWidget>
#include <QLineEdit>

#include "plotter.h"
#include "sdata.h"
#include "axisproperties.h"

#include "wplot/state/state.h"
#include "wplot/state/normalstate.h"
#include "wplot/state/zoomstate.h"
#include "wplot/state/hzoomstate.h"
#include "wplot/state/vzoomstate.h"

class WPlot : public QWidget
{
    Q_OBJECT

public:
    explicit WPlot(QWidget *parent = 0);
    ~WPlot();
    void addPoint(double t, SData y);
    void createPlot(void);
    void loadDataFile(QString fileName);
    void saveDataFile(QString fileName);

    QWidget* wParent;
    Plotter *m_plotter;
    AxisPropierties m_axis;
    QString m_fileName;

    QPoint m_lastPoint;

    State* state;
    NormalState normalState;
    ZoomState zoomState;
    HZoomState hZoomState;
    VZoomState vZoomState;

public slots:
    void updatePlot(void);
    void zoom_Undo(void);
    void zoom_Redo(void);
    void zoom(void);
    void hZoom(void);
    void vZoom(void);
    void toggleAxisBottomLeft(void);
    void toggleAxisTopRigth(void);

signals:
    void newPlotter(void);

private:
    QVector<SData> m_data;
    QImage m_plotImage;

    double m_y_max = 0, m_y_min = 0;
    int selectedCursor;
    QDialog* setCurPosDiag;
    QLineEdit* curPosEdit;

private slots:
    void ShowContextMenu(QPoint pos);
    void addCursor(void);
    void removeCursor(void);
    void setCursorPos(void);
    void cursorNewPos(void);
    void open_data_file(void);
    void export_data_file(void);

protected:
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    bool event(QEvent* event);
    void resizeEvent(QResizeEvent *event);
};

#endif // WPLOT_H
