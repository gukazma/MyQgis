#ifndef PROCESSDIALOG_H
#define PROCESSDIALOG_H

#include <QDialog>
#include <qobjectdefs.h>

namespace Ui {
class ProcessDialog;
}

class ProcessDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProcessDialog(QWidget *parent = nullptr);
    ~ProcessDialog();
    void setValue(int value);
signals:
    void signalSetValue();

private slots:
    void onValueChange();

private:
    Ui::ProcessDialog *ui;
    int m_Value;
};

#endif // PROCESSDIALOG_H
