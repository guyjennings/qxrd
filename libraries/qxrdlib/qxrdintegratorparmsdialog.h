#ifndef QXRDINTEGRATORPARMSDIALOG_H
#define QXRDINTEGRATORPARMSDIALOG_H

#include "qxrdlib_global.h"
#include <QDialog>
#include "ui_qxrdintegratorparmsdialog.h"

#include "qxrddataprocessor-ptr.h"

class QXRD_EXPORT QxrdIntegratorParmsDialog : public QDialog, public Ui::QxrdIntegratorParmsDialog
{
  Q_OBJECT

public:
  explicit QxrdIntegratorParmsDialog(QxrdDataProcessorWPtr procw);
  ~QxrdIntegratorParmsDialog();

  void accept();

private:
  QxrdDataProcessorPtr m_Processor;
};

#endif // QXRDINTEGRATORPARMSDIALOG_H
