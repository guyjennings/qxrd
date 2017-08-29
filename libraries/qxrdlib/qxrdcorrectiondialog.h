#ifndef QXRDCORRECTIONDIALOG_H
#define QXRDCORRECTIONDIALOG_H

#include "qxrdlib_global.h"
#include <QDockWidget>
#include "qxrdacquisition.h"
#include "qxrddataprocessor.h"
#include "ui_qxrdcorrectiondialog.h"

class QXRD_EXPORT QxrdCorrectionDialog : public QDockWidget, public Ui::QxrdCorrectionDialog
{
    Q_OBJECT

public:
    explicit QxrdCorrectionDialog(QWidget *parent, QxrdAcquisitionWPtr acq, QxrdDataProcessorWPtr proc);
    ~QxrdCorrectionDialog();

protected:
    void changeEvent(QEvent *e);
};

#endif // QXRDCORRECTIONDIALOG_H
