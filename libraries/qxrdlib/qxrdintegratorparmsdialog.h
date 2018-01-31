#ifndef QXRDINTEGRATORPARMSDIALOG_H
#define QXRDINTEGRATORPARMSDIALOG_H

#include "qxrdlib_global.h"
#include <QDialog>
#include "ui_qxrdintegratorparmsdialog.h"

#include "qxrdprocessor-ptr.h"

class QXRD_EXPORT QxrdIntegratorParmsDialog : public QDialog, public Ui::QxrdIntegratorParmsDialog
{
  Q_OBJECT

public:
  explicit QxrdIntegratorParmsDialog(QxrdProcessorWPtr procw);
  ~QxrdIntegratorParmsDialog();

  void accept();

private:
  QxrdProcessorWPtr m_Processor;
};

#endif // QXRDINTEGRATORPARMSDIALOG_H
