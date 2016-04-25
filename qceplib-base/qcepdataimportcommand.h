#ifndef QCEPDATAIMPORTCOMMAND_H
#define QCEPDATAIMPORTCOMMAND_H

#include "qcepmacros.h"
#include "qcepobject.h"
#include "qcepdataset-ptr.h"
#include "qcepdatasetmodel-ptr.h"
#include "qcepexperiment-ptr.h"
#include "qcepdataimportparameters-ptr.h"
#include "qcepfileimporter-ptr.h"
#include <QModelIndexList>

class QcepDataImportCommand : public QcepObject
{
  Q_OBJECT

public:
  QcepDataImportCommand(QcepObjectWPtr parent,
                        QcepDatasetModelWPtr model,
                        const QModelIndexList &idx,
                        QStringList files);

  bool exec();
  void copyResults();

private:
  QcepFileImporterPtr         m_FileImporter;
  QcepDatasetPtr              m_ImportedData;
  QcepDatasetModelPtr         m_ImportedDataset;
  QModelIndexList             m_ImportedIndexes;
  QcepDatasetModelPtr         m_Model;
  QModelIndexList             m_Indexes;
  QStringList                 m_Files;
  QcepExperimentPtr           m_Experiment;
  QcepDataImportParametersPtr m_Parameters;
};

#endif // QCEPDATAIMPORTCOMMAND_H
