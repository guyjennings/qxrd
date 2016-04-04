#ifndef QCEPDATAIMPORTCOMMAND_H
#define QCEPDATAIMPORTCOMMAND_H

#include "qcepmacros.h"
#include "qcepobject.h"
#include "qcepdatasetmodel-ptr.h"
#include "qcepexperiment-ptr.h"
#include "qcepdataimportparameters-ptr.h"
#include <QModelIndexList>

class QcepDataImportCommand : public QcepObject
{
  Q_OBJECT

public:
  QcepDataImportCommand(QcepDatasetModelPtr model, const QModelIndexList &idx, QStringList files);

  bool exec();

private:
  QcepDatasetModelPtr         m_ImportedData;
  QModelIndexList             m_ImportedIndexes;
  QcepDatasetModelPtr         m_Model;
  QModelIndexList             m_Indexes;
  QStringList                 m_Files;
  QcepExperimentPtr           m_Experiment;
  QcepDataImportParametersPtr m_Parameters;
};

#endif // QCEPDATAIMPORTCOMMAND_H
