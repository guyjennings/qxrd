#ifndef QCEPDATAIMPORTPARAMETERS_H
#define QCEPDATAIMPORTPARAMETERS_H

#include "qceplib_global.h"
#include "qcepmacros.h"
#include "qcepobject.h"
#include "qcepproperty.h"

class QCEP_EXPORT QcepDataImportParameters : public QcepObject
{
  Q_OBJECT

public:
  Q_INVOKABLE QcepDataImportParameters(QString name);

  QString lastDirectory();
  QString fileFormatFilterString();
  void setLastDirectory(QStringList files);

public:
  Q_PROPERTY(int inputFormat READ get_InputFormat WRITE set_InputFormat)
  QCEP_INTEGER_PROPERTY(InputFormat)

  Q_PROPERTY(QString lastImportPath READ get_LastImportPath WRITE set_LastImportPath)
  QCEP_STRING_PROPERTY(LastImportPath)

  Q_PROPERTY(QString selectedFilter READ get_SelectedFilter WRITE set_SelectedFilter)
  QCEP_STRING_PROPERTY(SelectedFilter)
};

Q_DECLARE_METATYPE(QcepDataImportParameters*)

#endif // QCEPDATAIMPORTPARAMETERS_H
