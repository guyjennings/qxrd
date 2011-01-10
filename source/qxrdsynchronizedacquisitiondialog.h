#ifndef QXRDSYNCHRONIZEDACQUISITIONDIALOG_H
#define QXRDSYNCHRONIZEDACQUISITIONDIALOG_H

#include <QDockWidget>
#include "qxrdacquisition.h"

namespace Ui {
    class QxrdSynchronizedAcquisitionDialog;
}

class QxrdSynchronizedAcquisitionDialog : public QDockWidget
{
    Q_OBJECT

public:
    explicit QxrdSynchronizedAcquisitionDialog(QWidget *parent = 0, QxrdAcquisition *acq = 0);
    ~QxrdSynchronizedAcquisitionDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QxrdSynchronizedAcquisitionDialog *ui;
};

#endif // QXRDSYNCHRONIZEDACQUISITIONDIALOG_H
