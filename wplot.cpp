#include "wplot.h"

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
    m_drag = false;

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
    qreal x_min = 0, x_max = 0;
    if (m_data.size() != 0)
    {
        x_min = m_data[0][0];
        x_max = m_data[m_data.size()-1][0];
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
                QRectF(x_min, m_y_min, x_max - x_min, m_y_max - m_y_min),
                m_data,
                Plotter::LINE_STYLE);
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

        qreal x_min = 0, x_max = 0;
        if (m_data.size() != 0)
        {
            x_min = m_data[0][0];
            x_max = m_data[m_data.size()-1][0];
        }

        if (m_plotter)
        {
            delete m_plotter; // remove old data plotter
        }

        m_plotter = new Plotter(
                    size(),
                    QRectF(x_min, y_min, x_max - x_min, y_max - y_min),
                    m_data,
                    Plotter::LINE_STYLE);
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
    m_plotter->Undo();
    updatePlot();
}
void WPlot::zoom_Redo(void)
{
    if (!m_plotter)
        return;
    m_plotter->Redo();
    updatePlot();
}
void WPlot::zoom(void)
{
    this->state = &this->zoomState;
    this->state->setCursor(*this);
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
    this->m_plotter->m_axsisBottom ^= 1;
    this->m_plotter->m_axsisLeft ^= 1;
    this->updatePlot();
}
void WPlot::toggleAxisTopRigth(void) {
    this->m_plotter->m_axsisTop ^= 1;
    this->m_plotter->m_axsisRight ^= 1;
    this->updatePlot();
}

// Context Menu
void WPlot::ShowContextMenu(QPoint pos)
{
    bool onCursor = m_plotter->onCursor(pos, this->selectedCursor, false);

    QMenu contextMenu("Context menu", this);

    QAction addCursorAction("Add cursor", this);
    connect(&addCursorAction, SIGNAL(triggered()), this, SLOT(addCursor()));
    contextMenu.addAction(&addCursorAction);

    QAction removeCursorAction("Remove cursor", this);
    QAction setCursorPosAction("Set cursor", this);
    if (onCursor) {
        connect(&removeCursorAction, SIGNAL(triggered()), this, SLOT(removeCursor()));
        contextMenu.addAction(&removeCursorAction);

        connect(&setCursorPosAction, SIGNAL(triggered()), this, SLOT(setCursorPos()));
        contextMenu.addAction(&setCursorPosAction);
    }

    QAction zoomAction("Zoom", this);
    connect(&zoomAction, SIGNAL(triggered()), this, SLOT(zoom()));
    contextMenu.addAction(&zoomAction);

    QAction zoomUndoAction("Zoom Undo", this);
    connect(&zoomUndoAction, SIGNAL(triggered()), this, SLOT(zoom_Undo()));
    contextMenu.addAction(&zoomUndoAction);

    QAction zoomRedoAction("Zoom Redo", this);
    connect(&zoomRedoAction, SIGNAL(triggered()), this, SLOT(zoom_Redo()));
    contextMenu.addAction(&zoomRedoAction);

    QAction openDataAction("Open data", this);
    connect(&openDataAction, SIGNAL(triggered()), this, SLOT(open_data_file()));
    contextMenu.addAction(&openDataAction);

    QAction exportDataAction("Export data", this);
    connect(&exportDataAction, SIGNAL(triggered()), this, SLOT(export_data_file()));
    contextMenu.addAction(&exportDataAction);

    QAction toggleAxisBottomLeftAction("Toggle axis bottom left", this);
    connect(&toggleAxisBottomLeftAction, SIGNAL(triggered()), this, SLOT(toggleAxisBottomLeft()));
    contextMenu.addAction(&toggleAxisBottomLeftAction);

    QAction toggleAxisTopRigthAction("Toggle axis top rigth", this);
    connect(&toggleAxisTopRigthAction, SIGNAL(triggered()), this, SLOT(toggleAxisTopRigth()));
    contextMenu.addAction(&toggleAxisTopRigthAction);

    contextMenu.exec(mapToGlobal(pos));
}

// Cursor functions
void WPlot::addCursor(void) {
    m_plotter->addCursor();
    updatePlot();
}
void WPlot::removeCursor(void) {
    m_plotter->removeCursor(this->selectedCursor);
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
        this->m_plotter->setCursorPos(this->selectedCursor, value);
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
    qDebug() << "All states double click" << event;
}
void WPlot::mouseReleaseEvent(QMouseEvent* event)
{
    if (!m_plotter) return;
    if (event->button() == dragButton)
    {
        if (m_drag)
        {
            m_drag = false;
        }
        if (m_plotter->getCursorDragged() != 0)
        {
            m_plotter->releaseCursor();
        }
    }
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
        m_plotter->AddUndoStatus(); // To be filetered
        m_plotter->zoomX((qreal)angleDelta.y()/120);
        m_plotter->zoomY((qreal)angleDelta.x()/120);
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
            m_plotter->AddUndoStatus(); // To be filetered
            m_plotter->zoomX((factor-1)*5);
            m_plotter->zoomY((factor-1)*5);
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
