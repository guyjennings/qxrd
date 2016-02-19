#ifndef QXRDPOLARTRANSFORMDIALOG_H
#define QXRDPOLARTRANSFORMDIALOG_H

#include <QDialog>
#include "ui_qxrdpolartransformdialog.h"

#include "qxrddataprocessor-ptr.h"

class QxrdPolarTransformDialog : public QDialog, public Ui::QxrdPolarTransformDialog
{
  Q_OBJECT

public:
  explicit QxrdPolarTransformDialog(QxrdDataProcessorWPtr procw, QWidget *parent = 0);
  ~QxrdPolarTransformDialog();

  void accept();

private:
  QxrdDataProcessorPtr m_Processor;
};

#endif // QXRDPOLARTRANSFORMDIALOG_H
