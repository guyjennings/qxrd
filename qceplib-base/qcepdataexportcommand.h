#ifndef QCEPDATAEXPORTCOMMAND_H
#define QCEPDATAEXPORTCOMMAND_H

#include "qcepmacros.h"
#include "qcepobject.h"
#include "qcepdatasetmodel-ptr.h"
#include "qcepdataexportparameters-ptr.h"
#include "qcepexperiment-ptr.h"
#include <QModelIndexList>

class QcepDataExportCommand : public QcepObject
{
  Q_OBJECT

public:
  QcepDataExportCommand(QcepDatasetModelPtr model, const QModelIndexList &idx, QString file);

  bool exec();

private:
  QcepDatasetModelPtr         m_Model;
  QModelIndexList             m_Indexes;
  QString                     m_File;
  QcepExperimentPtr           m_Experiment;
  QcepDataExportParametersPtr m_Parameters;
};

#endif // QCEPDATAEXPORTCOMMAND_H
