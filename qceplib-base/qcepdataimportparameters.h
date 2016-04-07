#ifndef QCEPDATAIMPORTPARAMETERS_H
#define QCEPDATAIMPORTPARAMETERS_H

#include "qcepmacros.h"
#include "qcepobject.h"
#include "qcepproperty.h"

class QcepDataImportParameters : public QcepObject
{
  Q_OBJECT

public:
  QcepDataImportParameters(QcepSettingsSaverWPtr saver, QString name);

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

#endif // QCEPDATAIMPORTPARAMETERS_H
