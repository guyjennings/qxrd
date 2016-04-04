#ifndef QCEPDATAIMPORTDIALOG_H
#define QCEPDATAIMPORTDIALOG_H

#include <QDialog>
#include "ui_qcepdataimportdialog.h"
#include "qcepdatasetmodel-ptr.h"
#include "qcepexperiment-ptr.h"
#include "qcepdataimportparameters-ptr.h"
#include <QModelIndexList>

class QcepDataImportDialog : public QDialog, public Ui::QcepDataImportDialog
{
  Q_OBJECT

public:
  explicit QcepDataImportDialog(QcepDatasetModelPtr indata,
                                const QModelIndexList &inselect,
                                QcepDatasetModelPtr destdata,
                                const QModelIndexList &destselect,
                                QStringList files,
                                QcepExperimentPtr expt,
                                QcepDataImportParametersPtr parms);
  ~QcepDataImportDialog();

  void accept();

private:
  QcepDatasetModelPtr         m_InData;
  QModelIndexList             m_InSelect;
  QcepDatasetModelPtr         m_DestData;
  QModelIndexList             m_DestSelect;
  QStringList                 m_Files;
  QcepExperimentPtr           m_Experiment;
  QcepDataImportParametersPtr m_Parameters;
};

#endif // QCEPDATAIMPORTDIALOG_H
