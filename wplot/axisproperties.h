#ifndef AXISPROPERTIES_H
#define AXISPROPERTIES_H

#include <QObject>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>

class AxisPropierties : public QObject
{
    Q_OBJECT
public:
    explicit AxisPropierties(QObject *parent = nullptr);
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
    QLabel lXdiv;
    QLineEdit leXdiv;
    QLineEdit leXmax;
    QLineEdit leYmin;
    QCheckBox cbYdiv;
    QLabel lYdiv;
    QLineEdit leYdiv;
    QLineEdit leYmax;

    void createDialog(void);
    void updateDialog(void);

private slots:
    void updateParent(void);
    void updateControlsVisibility(void);
    void cbChanged(int state);
};

#endif // AXISPROPERTIES_H