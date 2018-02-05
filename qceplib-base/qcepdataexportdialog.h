#ifndef QCEPDATAEXPORTDIALOG_H
#define QCEPDATAEXPORTDIALOG_H

#include "qceplib_global.h"
#include <QDialog>
#include "ui_qcepdataexportdialog.h"
#include "qcepdatasetmodel-ptr.h"
#include "qcepdataexportparameters-ptr.h"
#include "qcepexperiment-ptr.h"
#include <QModelIndexList>

class QCEP_EXPORT QcepDataExportDialog : public QDialog, public Ui::QcepDataExportDialog
{
  Q_OBJECT

private:
  typedef QDialog inherited;

public:
  explicit QcepDataExportDialog(QcepDatasetModelPtr model,
                                const QModelIndexList &idx,
                                QString file,
                                QcepExperimentPtr expt,
                                QcepDataExportParametersPtr parms);
  ~QcepDataExportDialog();

  void accept();

private:
  QcepDatasetModelPtr         m_Model;
  QModelIndexList             m_Indexes;
  QString                     m_File;
  QcepExperimentPtr           m_Experiment;
  QcepDataExportParametersPtr m_Parameters;
};

#endif // QCEPDATAEXPORTDIALOG_H
