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
    m_diag.close();
    m_range->setLeft  (leXmin.text().toDouble());
    m_range->setRight (leXmax.text().toDouble());
    m_range->setTop   (leYmin.text().toDouble());
    m_range->setBottom(leYmax.text().toDouble());
    emit axisUpdated();
}

void Axis::createDialog(void) {
    QVBoxLayout *layMain = new QVBoxLayout;

    // Inputs
    QHBoxLayout *layInputs = new QHBoxLayout;

    // Sx
    QVBoxLayout *layInputsSx = new QVBoxLayout;
    QLabel* lXmin = new QLabel("x min:");
    QLabel* lXdiv = new QLabel("x division:");
    QLabel* lXmax = new QLabel("x max:");
    layInputsSx->addWidget(lXmin);
    layInputsSx->addWidget(&leXmin);
    layInputsSx->addWidget(lXdiv);
    layInputsSx->addWidget(&leXdiv);
    layInputsSx->addWidget(lXmax);
    layInputsSx->addWidget(&leXmax);

    // Dx
    QVBoxLayout *layInputsDx = new QVBoxLayout;
    QLabel* lYmin = new QLabel("y min:");
    QLabel* lYdiv = new QLabel("y division:");
    QLabel* lYmax = new QLabel("y max:");
    layInputsDx->addWidget(lYmin);
    layInputsDx->addWidget(&leYmin);
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
    leXmax.setText(QString::number(m_range->right()));
    leYmin.setText(QString::number(m_range->top()));
    leYmax.setText(QString::number(m_range->bottom()));
}
