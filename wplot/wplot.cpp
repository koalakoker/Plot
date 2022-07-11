#include "wplot/wplot.h"
#include "wplot/axis.h"
#include "wplot/cursor.h"
#include "wplot/curve.h"

#include <QGestureEvent>
#include <QMouseEvent>
#include <QMenu>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QAbstractButton>
#include <QPushButton>
#include <QDebug>

WPlot::WPlot(QWidget *parent) :
    QWidget(parent), m_plotter(NULL)
{
    setFocusPolicy(Qt::StrongFocus);
    this->wParent = parent;

    grabGesture(Qt::PinchGesture);
    setMouseTracking(true);

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(ShowContextMenu(QPoint)));

    this->state = &this->normalState;
    this->state->setCursor(*this);
}
WPlot::~WPlot()
{
}

// Add point
void WPlot::addPoint(double t, SData y)
{
    foreach (double val, y.data())
    {
        if (val > m_y_max)
        {
            m_y_max = val;
        }

        if (val < m_y_min)
        {
            m_y_min = val;
        }

    }

    y.prepend(t);
    m_data.append(y);
}

// Create plot
void WPlot::createPlot(void)
{
    if (m_data.size() != 0)
    {
        m_x_min = m_data[0][0];
        m_x_max = m_data[m_data.size()-1][0];
    }

    QSize plotterSize;
    if (wParent == NULL) {
        plotterSize = this->size();
    } else {
        plotterSize = this->wParent->size();
        this->resize(plotterSize);
    }

    m_plotter = new Plotter(
                plotterSize,
                QRectF(m_x_min, m_y_min, m_x_max - m_x_min, m_y_max - m_y_min));
    m_plotter->curve->m_data = m_data;
    m_axisProps.m_range = &m_plotter->axis->m_range;
    m_axisProps.m_axisDiv = &m_plotter->axis->m_div;
    m_axisProps.m_axisDivVisible = m_plotter->axis->m_divVisible;
    emit newPlotter();
    updatePlot();
}

// Load data file
void WPlot::loadDataFile(QString fileName)
{
    double y_max = 0, y_min = 0;

    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly))
    {
        m_data.clear();
        QTextStream stream(&file);
        QString line;
        while (!stream.atEnd())
        {
            line = stream.readLine();
            QStringList fields = line.split(QRegularExpression("[\t ]+"));
            SData sample;
            for (int i = 0; i < fields.size(); i++)
            {
                double val = fields.at(i).toDouble();
                sample.append(val);
                if (i != 0)
                {
                    if (val > y_max)
                    {
                        y_max = val;
                    }

                    if (val < y_min)
                    {
                        y_min = val;
                    }
                }
            }
            m_data.append(sample);
        }
        m_fileName = fileName;

        if (m_data.size() != 0)
        {
            m_x_min = m_data[0][0];
            m_x_max = m_data[m_data.size()-1][0];
        }

        if (m_plotter)
        {
            delete m_plotter; // remove old data plotter
        }

        m_plotter = new Plotter(
                    size(),
                    QRectF(m_x_min, y_min, m_x_max - m_x_min, y_max - y_min));
        m_plotter->curve->m_data = m_data;
        m_axisProps.m_range = &m_plotter->axis->m_range;
        emit newPlotter();
        updatePlot();
    }
    file.close();
}

// Save data file
void WPlot::saveDataFile(QString fileName)
{
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        for (int row = 0; row < m_data.size(); row++)
        {
            SData dataRow = m_data.at(row);
            for (int i = 0; i < dataRow.size(); i++)
            {
                double val = dataRow.data().at(i);
                if (i == 0)
                {
                    stream << val;
                }
                else
                {
                    stream << " " << val;
                }
            }
            stream << Qt::endl;
        }
    }
    file.close();
}

// Slots
void WPlot::updatePlot(void) {
    m_plotImage = m_plotter->plot();
    repaint();
}
void WPlot::zoom_Undo(void)
{
    if (!m_plotter)
        return;
    m_plotter->axis->zoom->undo();
    updatePlot();
}
void WPlot::zoom_Redo(void)
{
    if (!m_plotter)
        return;
    m_plotter->axis->zoom->redo();
    updatePlot();
}
void WPlot::zoom(void)
{
    this->state = &this->zoomState;
    this->state->setCursor(*this);
}
void WPlot::hZoom(void)
{
    this->state = &this->hZoomState;
    this->state->setCursor(*this);
}
void WPlot::vZoom(void)
{
    this->state = &this->vZoomState;
    this->state->setCursor(*this);
}
void WPlot::fullHZoom(void) {
    m_plotter->axis->m_range.setLeft(m_x_min);
    m_plotter->axis->m_range.setRight(m_x_max);
    updatePlot();
}
void WPlot::fullVZoom(void) {
    m_plotter->axis->m_range.setBottom(m_y_min);
    m_plotter->axis->m_range.setTop(m_y_max);
    updatePlot();
}

void WPlot::open_data_file(void)
{
    QString fileName = QFileDialog::getOpenFileName(this,"Open data file","","*.*");
    if (fileName != "")
    {
        this->loadDataFile(fileName);
    }
}
void WPlot::export_data_file(void)
{
    QString fileName = QFileDialog::getSaveFileName(this,"Export data file","","*.*");
    this->saveDataFile(fileName);
}
void WPlot::toggleAxisBottomLeft(void) {
    this->m_plotter->axis->m_showBottom ^= 1;
    this->m_plotter->axis->m_showLeft ^= 1;
    this->updatePlot();
}
void WPlot::toggleAxisTopRigth(void) {
    this->m_plotter->axis->m_showTop ^= 1;
    this->m_plotter->axis->m_showRight ^= 1;
    this->updatePlot();
}

// Context Menu
void WPlot::ShowContextMenu(QPoint pos)
{
    bool onCursor = m_plotter->cursor->on(pos, this->selectedCursor, false);

    QMenu contextMenu("Context menu", this);

    // Cursor Menu
    QMenu cursorMenu("Cursor", this);

    QAction addCursorAction("Add cursor", this);
    connect(&addCursorAction, SIGNAL(triggered()), this, SLOT(addCursor()));
    cursorMenu.addAction(&addCursorAction);

    QAction removeCursorAction("Remove cursor", this);
    QAction setCursorPosAction("Set cursor", this);
    if (onCursor) {
        connect(&removeCursorAction, SIGNAL(triggered()), this, SLOT(removeCursor()));
        cursorMenu.addAction(&removeCursorAction);

        connect(&setCursorPosAction, SIGNAL(triggered()), this, SLOT(setCursorPos()));
        cursorMenu.addAction(&setCursorPosAction);
    }

    contextMenu.addMenu(&cursorMenu);

    // Zoom Menu
    QMenu zoomMenu("Zoom", this);

    QAction zoomAction("Zoom", this);
    connect(&zoomAction, SIGNAL(triggered()), this, SLOT(zoom()));
    zoomMenu.addAction(&zoomAction);

    QAction hZoomAction("Horizontal zoom", this);
    connect(&hZoomAction, SIGNAL(triggered()), this, SLOT(hZoom()));
    zoomMenu.addAction(&hZoomAction);

    QAction vZoomAction("Vertical zoom", this);
    connect(&vZoomAction, SIGNAL(triggered()), this, SLOT(vZoom()));
    zoomMenu.addAction(&vZoomAction);

    QAction zoomUndoAction("Zoom Undo", this);
    connect(&zoomUndoAction, SIGNAL(triggered()), this, SLOT(zoom_Undo()));
    zoomMenu.addAction(&zoomUndoAction);

    QAction zoomRedoAction("Zoom Redo", this);
    connect(&zoomRedoAction, SIGNAL(triggered()), this, SLOT(zoom_Redo()));
    zoomMenu.addAction(&zoomRedoAction);

    QAction zoomFullHorizontalAction("Full hor. range", this);
    connect(&zoomFullHorizontalAction, SIGNAL(triggered()), this,SLOT(fullHZoom()));
    zoomMenu.addAction(&zoomFullHorizontalAction);

    QAction zoomFullVerticalAction("Full ver. range", this);
    connect(&zoomFullVerticalAction, SIGNAL(triggered()), this,SLOT(fullVZoom()));
    zoomMenu.addAction(&zoomFullVerticalAction);

    contextMenu.addMenu(&zoomMenu);

    // Save/Load Menu
    QMenu saveLoadMenu("Save/Load", this);

    QAction openDataAction("Load data", this);
    connect(&openDataAction, SIGNAL(triggered()), this, SLOT(open_data_file()));
    saveLoadMenu.addAction(&openDataAction);

    QAction exportDataAction("Save data", this);
    connect(&exportDataAction, SIGNAL(triggered()), this, SLOT(export_data_file()));
    saveLoadMenu.addAction(&exportDataAction);

    contextMenu.addMenu(&saveLoadMenu);

    // Axis menu
    QMenu axisMenu("Axis", this);

    QAction setAxisAction("Axis properties", this);
    connect(&setAxisAction, SIGNAL(triggered()), &this->m_axisProps, SLOT(set()));
    connect(&this->m_axisProps, SIGNAL(axisUpdated()), this, SLOT(updatePlot()));
    axisMenu.addAction(&setAxisAction);

    QAction toggleAxisBottomLeftAction("Toggle axis bottom left", this);
    connect(&toggleAxisBottomLeftAction, SIGNAL(triggered()), this, SLOT(toggleAxisBottomLeft()));
    axisMenu.addAction(&toggleAxisBottomLeftAction);

    QAction toggleAxisTopRigthAction("Toggle axis top rigth", this);
    connect(&toggleAxisTopRigthAction, SIGNAL(triggered()), this, SLOT(toggleAxisTopRigth()));
    axisMenu.addAction(&toggleAxisTopRigthAction);

    contextMenu.addMenu(&axisMenu);

    contextMenu.exec(mapToGlobal(pos));
}

// Cursor functions
void WPlot::addCursor(void) {
    m_plotter->cursor->add();
    updatePlot();
}
void WPlot::removeCursor(void) {
    m_plotter->cursor->remove(this->selectedCursor);
    updatePlot();
}
void WPlot::setCursorPos(void) {
    this->setCurPosDiag = new QDialog();
    QVBoxLayout *lay = new QVBoxLayout;
    QLabel *label = new QLabel("Insert x position:");
    this->curPosEdit = new QLineEdit();
    QAbstractButton *bOk = new QPushButton("Ok");
    QAbstractButton *bCancel = new QPushButton("Cancel");

    QHBoxLayout *buttonsLay = new QHBoxLayout;
    buttonsLay->addWidget(bOk);
    buttonsLay->addWidget(bCancel);

    lay->addWidget(label);
    lay->addWidget(this->curPosEdit);
    lay->addLayout(buttonsLay);

    this->setCurPosDiag->setLayout(lay);
    this->setCurPosDiag->show();
    this->setCurPosDiag->connect(bOk,SIGNAL(clicked()),this,SLOT(cursorNewPos()));
    this->setCurPosDiag->connect(bCancel,SIGNAL(clicked()),this->setCurPosDiag,SLOT(close()));
}
void WPlot::cursorNewPos(void) {
    QString inputText = this->curPosEdit->text();
    bool isValid;
    qreal value = inputText.toDouble(&isValid);
    if (isValid) {
        this->m_plotter->cursor->setPos(this->selectedCursor, value);
        this->updatePlot();
    }
    this->setCurPosDiag->close();
}

// Events
void WPlot::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawImage(QPoint(0, 0), m_plotImage);
}
void WPlot::keyPressEvent(QKeyEvent* event) {
    this->state->keyPressEvent(*this, event);
    if (!event->isAccepted()) {
        QWidget::keyPressEvent(event);
    }
};
void WPlot::keyReleaseEvent(QKeyEvent* event) {
    this->state->keyReleaseEvent(*this, event);
    QWidget::keyReleaseEvent(event);
};
void WPlot::mousePressEvent(QMouseEvent* event)
{
    if (!m_plotter) return;
    this->state->mousePressEvent(*this, event);
}
void WPlot::mouseDoubleClickEvent(QMouseEvent* event)
{
    this->state->mouseDoubleClickEvent(*this, event);
}
void WPlot::mouseReleaseEvent(QMouseEvent* event)
{
    if (!m_plotter) return;
    this->state->mouseReleaseEvent(*this, event);
}
void WPlot::mouseMoveEvent(QMouseEvent* event)
{
    if (!m_plotter) return;
    this->state->mouseMoveEvent(*this, event);
}
void WPlot::wheelEvent(QWheelEvent* event)
{
    if (!m_plotter)
        return;
    QPoint angleDelta = event->angleDelta();
    if (angleDelta != QPoint(0,0))
    {
        m_plotter->axis->zoom->addUndoStatus(); // To be filetered
        m_plotter->axis->zoom->x((qreal)angleDelta.y()/120);
        m_plotter->axis->zoom->y((qreal)angleDelta.x()/120);
        updatePlot();
    }
}
bool WPlot::event(QEvent *event)
{
    if (!m_plotter)
        QWidget::event(event);
    if (event->type() == QEvent::Gesture)
    {
        QGestureEvent* gest = static_cast<QGestureEvent*>(event);
        if (QGesture *pinch = gest->gesture(Qt::PinchGesture))
        {
            QPinchGesture* pinchGest = static_cast<QPinchGesture *>(pinch);
            qreal factor = pinchGest->scaleFactor();
            m_plotter->axis->zoom->addUndoStatus(); // To be filetered
            m_plotter->axis->zoom->x((factor-1)*5);
            m_plotter->axis->zoom->y((factor-1)*5);
            updatePlot();
        }
    }
    return QWidget::event(event);
}
void WPlot::resizeEvent(QResizeEvent *event)
{
    if (!m_plotter)
        return;
    if (event->type() == QEvent::Resize)
    {
        m_plotter->setSize(size());
        updatePlot();
    }
}
