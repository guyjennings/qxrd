#ifndef QCEPDATAEXPORTPARAMETERS_H
#define QCEPDATAEXPORTPARAMETERS_H

#include "qceplib_global.h"
#include "qcepmacros.h"
#include "qcepserializableobject.h"
#include "qcepproperty.h"

class QCEP_EXPORT QcepDataExportParameters : public QcepSerializableObject
{
  Q_OBJECT

public:
  Q_INVOKABLE QcepDataExportParameters(QString name);

  QString fileFormatFilterString();

public:
  Q_PROPERTY(int outputFormat READ get_OutputFormat WRITE set_OutputFormat)
  QCEP_INTEGER_PROPERTY(OutputFormat)

  Q_PROPERTY(QString fileName READ get_FileName WRITE set_FileName)
  QCEP_STRING_PROPERTY(FileName)

  Q_PROPERTY(QString selectedFilter READ get_SelectedFilter WRITE set_SelectedFilter)
  QCEP_STRING_PROPERTY(SelectedFilter)
};

Q_DECLARE_METATYPE(QcepDataExportParameters*)

#endif // QCEPDATAEXPORTPARAMETERS_H