#ifndef QCEPSETDATAVALUERANGEDIALOG_H
#define QCEPSETDATAVALUERANGEDIALOG_H

#include <QDialog>
#include "ui_qcepsetdatavaluerangedialog.h"
#include "qcepdatasetmodel-ptr.h"
#include "qcepexperiment-ptr.h"
#include "qcepsetdatavaluerangecommand-ptr.h"

class QcepSetDataValueRangeDialog : public QDialog, public Ui::QcepSetDataValueRangeDialog
{
  Q_OBJECT

public:
  explicit QcepSetDataValueRangeDialog(QcepDatasetModelPtr model, const QModelIndex &idx);
  ~QcepSetDataValueRangeDialog();

  void accept();

protected:
  void changeEvent(QEvent *e);

private:
  QcepDatasetModelPtr             m_Model;
  QModelIndex                     m_Index;
  QcepExperimentPtr               m_Experiment;
  QcepSetDataValueRangeCommandPtr m_Command;
};

#endif // QCEPSETDATAVALUERANGEDIALOG_H
