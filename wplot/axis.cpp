#include "axis.h"
#include <QDialog>
#include <QBoxLayout>
#include <QLabel>
#include <QAbstractButton>
#include <QPushButton>
#include <QDebug>

Axis::Axis(QObject *parent)
    : QObject{parent} {
    createDialog();
}
void Axis::set(void) {
    updateDialog();
    m_diag.show();
}
void Axis::onOk(void) {
    updateParent();
    m_diag.close();
}
void Axis::createDialog(void) {
    QVBoxLayout *layMain = new QVBoxLayout;

    // Inputs
    QHBoxLayout *layInputs = new QHBoxLayout;

    // Sx
    QVBoxLayout *layInputsSx = new QVBoxLayout;
    QLabel* lXmin = new QLabel("x min:");
    cbXdiv.setText("Enable x divisions");
    QLabel* lXdiv = new QLabel("x divisions:");
    QLabel* lXmax = new QLabel("x max:");
    layInputsSx->addWidget(lXmin);
    layInputsSx->addWidget(&leXmin);
    layInputsSx->addWidget(&cbXdiv);
    layInputsSx->addWidget(lXdiv);
    layInputsSx->addWidget(&leXdiv);
    layInputsSx->addWidget(lXmax);
    layInputsSx->addWidget(&leXmax);
    connect(&leXmin, SIGNAL(editingFinished()), this, SLOT(updateParent()));
    connect(&leXdiv, SIGNAL(editingFinished()), this, SLOT(updateParent()));
    connect(&leXmax, SIGNAL(editingFinished()), this, SLOT(updateParent()));
    connect(&leYmin, SIGNAL(editingFinished()), this, SLOT(updateParent()));
    connect(&leYdiv, SIGNAL(editingFinished()), this, SLOT(updateParent()));
    connect(&leYmax, SIGNAL(editingFinished()), this, SLOT(updateParent()));

    // Dx
    QVBoxLayout *layInputsDx = new QVBoxLayout;
    QLabel* lYmin = new QLabel("y min:");
    cbYdiv.setText("Enable y divisions");
    QLabel* lYdiv = new QLabel("y divisions:");
    QLabel* lYmax = new QLabel("y max:");
    layInputsDx->addWidget(lYmin);
    layInputsDx->addWidget(&leYmin);
    layInputsDx->addWidget(&cbYdiv);
    layInputsDx->addWidget(lYdiv);
    layInputsDx->addWidget(&leYdiv);
    layInputsDx->addWidget(lYmax);
    layInputsDx->addWidget(&leYmax);

    layInputs->addLayout(layInputsSx);
    layInputs->addLayout(layInputsDx);

    // Buttons
    QHBoxLayout *layButtons = new QHBoxLayout;
    QAbstractButton *bOk = new QPushButton("Ok");
    QAbstractButton *bCancel = new QPushButton("Cancel");
    layButtons->addWidget(bOk);
    layButtons->addWidget(bCancel);

    layMain->addLayout(layInputs);
    layMain->addLayout(layButtons);

    m_diag.setLayout(layMain);
    m_diag.connect(bOk, SIGNAL(clicked()), this, SLOT(onOk()));
    m_diag.connect(bCancel, SIGNAL(clicked()), &m_diag, SLOT(close()));
}
void Axis::updateDialog(void) {
    leXmin.setText(QString::number(m_range->left()));
    if (this->m_axisDivVisible[0]) {
        cbXdiv.setCheckState(Qt::Checked);
    } else {
        cbXdiv.setCheckState(Qt::Unchecked);
    }
    leXdiv.setText(QString::number(m_axisDiv->x()));
    leXmax.setText(QString::number(m_range->right()));
    leYmin.setText(QString::number(m_range->top()));
    if (this->m_axisDivVisible[1]) {
        cbYdiv.setCheckState(Qt::Checked);
    } else {
        cbYdiv.setCheckState(Qt::Unchecked);
    }
    leYdiv.setText(QString::number(m_axisDiv->y()));
    leYmax.setText(QString::number(m_range->bottom()));
}
void Axis::updateParent(void) {
    m_range->setLeft  (leXmin.text().toDouble());
    m_range->setRight (leXmax.text().toDouble());
    m_range->setTop   (leYmin.text().toDouble());
    m_range->setBottom(leYmax.text().toDouble());
    m_axisDiv->setX(leXdiv.text().toDouble());
    m_axisDiv->setY(leYdiv.text().toDouble());
    m_axisDivVisible[0] = (cbXdiv.checkState() == Qt::Checked);
    m_axisDivVisible[1] = (cbYdiv.checkState() == Qt::Checked);
    emit axisUpdated();
}
