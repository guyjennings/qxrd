#ifndef QCEPDATAEXPORTPARAMETERS_H
#define QCEPDATAEXPORTPARAMETERS_H

#include "qcepmacros.h"
#include "qcepobject.h"
#include "qcepproperty.h"

class QcepDataExportParameters : public QcepObject
{
  Q_OBJECT

public:
  QcepDataExportParameters(QcepSettingsSaverWPtr saver, QString name);

  QString fileFormatFilterString();

public:
  Q_PROPERTY(int outputFormat READ get_OutputFormat WRITE set_OutputFormat)
  QCEP_INTEGER_PROPERTY(OutputFormat)

  Q_PROPERTY(QString fileName READ get_FileName WRITE set_FileName)
  QCEP_STRING_PROPERTY(FileName)

  Q_PROPERTY(QString selectedFilter READ get_SelectedFilter WRITE set_SelectedFilter)
  QCEP_STRING_PROPERTY(SelectedFilter)
};

#endif // QCEPDATAEXPORTPARAMETERS_H
