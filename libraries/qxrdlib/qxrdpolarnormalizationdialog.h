#ifndef QXRDPOLARNORMALIZATIONDIALOG_H
#define QXRDPOLARNORMALIZATIONDIALOG_H

#include "qxrdlib_global.h"
#include <QDialog>
#include "ui_qxrdpolarnormalizationdialog.h"

#include "qxrddataprocessor-ptr.h"

class QXRD_EXPORT QxrdPolarNormalizationDialog : public QDialog, public Ui::QxrdPolarNormalizationDialog
{
  Q_OBJECT

public:
  explicit QxrdPolarNormalizationDialog(QxrdDataProcessorWPtr procw);
  ~QxrdPolarNormalizationDialog();

  void accept();

private:
  QxrdDataProcessorPtr m_Processor;
};

#endif // QXRDPOLARNORMALIZATIONDIALOG_H
