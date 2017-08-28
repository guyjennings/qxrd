#ifndef QCEPDATAIMPORTDIALOG_H
#define QCEPDATAIMPORTDIALOG_H

#include "qceplib_global.h"
#include <QDialog>
#include "ui_qcepdataimportdialog.h"
#include "qcepdatasetmodel-ptr.h"
#include "qcepexperiment-ptr.h"
#include "qcepdataimportparameters-ptr.h"
#include <QModelIndexList>

class QCEP_EXPORT QcepDataImportDialog : public QDialog, public Ui::QcepDataImportDialog
{
  Q_OBJECT

public:
  explicit QcepDataImportDialog(QcepDatasetModelPtr indata,
                                QModelIndexList &inselect,
                                QcepDatasetModelPtr destdata,
                                QModelIndexList &destselect,
                                QStringList files,
                                QcepExperimentPtr expt,
                                QcepDataImportParametersPtr parms);
  ~QcepDataImportDialog();

  void accept();

public slots:
  void importProgress(double pct);
  void importCompleted();

private:
  QcepDatasetModelPtr         m_InData;
  QModelIndexList            &m_InSelect;
  QcepDatasetModelPtr         m_DestData;
  QModelIndexList             m_DestSelect;
  QStringList                 m_Files;
  QcepExperimentPtr           m_Experiment;
  QcepDataImportParametersPtr m_Parameters;
};

#endif // QCEPDATAIMPORTDIALOG_H
