#ifndef QCEPSETRANGEOFIMAGEDIALOG_H
#define QCEPSETRANGEOFIMAGEDIALOG_H

#include <QDialog>
#include "ui_qcepsetrangeofimagedialog.h"
#include "qcepdatasetmodel-ptr.h"
#include "qcepexperiment-ptr.h"
#include "qcepsetrangeofimagecommand-ptr.h"

class QcepSetRangeOfImageDialog : public QDialog, public Ui::QcepSetRangeOfImageDialog
{
  Q_OBJECT

public:
  explicit QcepSetRangeOfImageDialog(QcepDatasetModelPtr model, const QModelIndex &idx);
  ~QcepSetRangeOfImageDialog();

  void accept();

protected:
  void changeEvent(QEvent *e);

private:
  QcepDatasetModelPtr           m_Model;
  QModelIndex                   m_Index;
  QcepExperimentPtr             m_Experiment;
  QcepSetRangeOfImageCommandPtr m_Command;
};

#endif // QCEPSETRANGEOFIMAGEDIALOG_H
