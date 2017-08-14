#ifndef QXRDZINGERDIALOG_H
#define QXRDZINGERDIALOG_H

#include <QDialog>
#include "ui_qxrdzingerdialog.h"

#include "qxrddataprocessor-ptr.h"

class QxrdZingerDialog : public QDialog, public Ui::QxrdZingerDialog
{
  Q_OBJECT

public:
  explicit QxrdZingerDialog(QxrdDataProcessorWPtr procw, QWidget *parent = 0);
  ~QxrdZingerDialog();

  void accept();

private:
  QxrdDataProcessorWPtr  m_Processor;
};

#endif // QXRDZINGERDIALOG_H