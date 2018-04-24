#ifndef QXRDOUTPUTFILEFORMATTERHDF_H
#define QXRDOUTPUTFILEFORMATTERHDF_H

#include "qxrdlib_global.h"
#include "qcepobject.h"

#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"
#include "qxrdoutputfileformattersettings-ptr.h"
#include "qxrdoutputfileformatter.h"

class QXRD_EXPORT QxrdOutputFileFormatterHDF : public QxrdOutputFileFormatter
{
  Q_OBJECT

private:
  typedef QxrdOutputFileFormatter inherited;

public:
  Q_INVOKABLE QxrdOutputFileFormatterHDF(QString name);

  void saveImageData(QxrdOutputFileFormatterSettingsPtr set,
                     QcepImageDataBasePtr               img,
                     QcepImageDataBasePtr               overflow);
};

#endif // QXRDOUTPUTFILEFORMATTERHDF_H
