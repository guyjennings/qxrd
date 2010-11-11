#ifndef QXRDDATAPROCESSOROPTIONSDIALOG_H
#define QXRDDATAPROCESSOROPTIONSDIALOG_H

#include <QDialog>

class QxrdDataProcessor;

namespace Ui {
    class QxrdDataProcessorOptionsDialog;
}

class QxrdDataProcessorOptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QxrdDataProcessorOptionsDialog(QxrdDataProcessor *processor, QWidget *parent = 0);
    ~QxrdDataProcessorOptionsDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QxrdDataProcessorOptionsDialog *ui;
};

#endif // QXRDDATAPROCESSOROPTIONSDIALOG_H
