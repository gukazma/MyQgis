#include "ProcessDialog.h"
#include "ui_ProcessDialog.h"
#include <qobjectdefs.h>

ProcessDialog::ProcessDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProcessDialog)
{
    ui->setupUi(this);
    connect(this, SIGNAL(signalSetValue()), this, SLOT(onValueChange()));
}

ProcessDialog::~ProcessDialog()
{
    delete ui;
}


void ProcessDialog::onValueChange()
{
    ui->progressBar->setValue(m_Value);
}

void ProcessDialog::setValue(int value)
{
    m_Value = value;
    emit signalSetValue();
}


