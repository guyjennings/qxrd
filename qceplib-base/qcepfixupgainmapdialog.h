#ifndef QCEPFIXUPGAINMAPDIALOG_H
#define QCEPFIXUPGAINMAPDIALOG_H

#include "qceplib_global.h"
#include <QDialog>
#include "ui_qcepfixupgainmapdialog.h"
#include "qcepdatasetmodel-ptr.h"
#include "qcepexperiment-ptr.h"
#include "qcepfixupgainmapcommand-ptr.h"

class QCEP_EXPORT QcepFixupGainMapDialog : public QDialog, public Ui::QcepFixupGainMapDialog
{
  Q_OBJECT

private:
  typedef QDialog inherited;

public:
  explicit QcepFixupGainMapDialog(QcepDatasetModelPtr model, const QModelIndex &idx);
  ~QcepFixupGainMapDialog();

  void accept();

protected:
  void changeEvent(QEvent *e);

private:
  QcepDatasetModelPtr        m_Model;
  QModelIndex                m_Index;
  QcepExperimentPtr          m_Experiment;
  QcepFixupGainMapCommandPtr m_Command;
};

#endif // QCEPFIXUPGAINMAPDIALOG_H
