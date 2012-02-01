#ifndef QXRDCORRECTIONDIALOG_H
#define QXRDCORRECTIONDIALOG_H

#include <QDockWidget>
#include "qxrdacquisition.h"
#include "qxrddataprocessor.h"
#include "ui_qxrdcorrectiondialog.h"

class QxrdCorrectionDialog : public QDockWidget, public Ui::QxrdCorrectionDialog
{
    Q_OBJECT

public:
    explicit QxrdCorrectionDialog(QWidget *parent, QxrdAcquisitionPtr acq, QxrdDataProcessorPtr proc);
    ~QxrdCorrectionDialog();

protected:
    void changeEvent(QEvent *e);
};

#endif // QXRDCORRECTIONDIALOG_H
