#ifndef AXIS_H
#define AXIS_H

#include <QObject>
#include <QDialog>
#include <QLineEdit>

class Axis : public QObject
{
    Q_OBJECT
public:
    explicit Axis(QObject *parent = nullptr);
    QRectF* m_range;

public slots:
    void set(void);
    void onOk(void);

signals:
    void axisUpdated(void);

private:
    QDialog m_diag;
    QLineEdit leXmin;
    QLineEdit leXdiv;
    QLineEdit leXmax;
    QLineEdit leYmin;
    QLineEdit leYdiv;
    QLineEdit leYmax;

    void createDialog(void);
    void updateDialog(void);
};

#endif // AXIS_H
