#ifndef QCEPDATAEXPORTCOMMAND_H
#define QCEPDATAEXPORTCOMMAND_H

#include "qceplib_global.h"
#include "qcepmacros.h"
#include "qcepobject.h"
#include "qcepdatasetmodel-ptr.h"
#include "qcepdataexportparameters-ptr.h"
#include "qcepexperiment-ptr.h"
#include <QModelIndexList>

class QCEP_EXPORT QcepDataExportCommand : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  QcepDataExportCommand(QcepDatasetModelWPtr model,
                        const QModelIndexList &idx,
                        QString file);

  bool exec();

private:
  QcepDatasetModelWPtr         m_Model;
  QModelIndexList             m_Indexes;
  QString                     m_File;
  QcepExperimentWPtr          m_Experiment;
  QcepDataExportParametersWPtr m_Parameters;
};

#endif // QCEPDATAEXPORTCOMMAND_H
