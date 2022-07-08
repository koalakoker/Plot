#ifndef AXIS_H
#define AXIS_H

#include <QObject>
#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>

class Axis : public QObject
{
    Q_OBJECT
public:
    explicit Axis(QObject *parent = nullptr);
    QRectF* m_range;
    QPointF* m_axisDiv;
    bool* m_axisDivVisible;

public slots:
    void set(void);
    void onOk(void);

signals:
    void axisUpdated(void);

private:
    QDialog m_diag;
    QLineEdit leXmin;
    QCheckBox cbXdiv;
    QLineEdit leXdiv;
    QLineEdit leXmax;
    QLineEdit leYmin;
    QCheckBox cbYdiv;
    QLineEdit leYdiv;
    QLineEdit leYmax;

    void createDialog(void);
    void updateDialog(void);

private slots:
    void updateParent(void);
};

#endif // AXIS_H
