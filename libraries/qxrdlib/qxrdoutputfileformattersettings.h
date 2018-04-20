#ifndef QXRDOUTPUTFILEFORMATTERSETTINGS_H
#define QXRDOUTPUTFILEFORMATTERSETTINGS_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include "qcepproperty.h"
#include "qcepimagedata-ptr.h"

class QXRD_EXPORT QxrdOutputFileFormatterSettings : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  Q_INVOKABLE QxrdOutputFileFormatterSettings(QString name);
  ~QxrdOutputFileFormatterSettings();

  void initialize(QcepObjectWPtr parent);

  void saveImageData(QString              name,
                     QcepImageDataBasePtr img,
                     QcepImageDataBasePtr overflow,
                     int                  canOverwrite);
};

#endif // QXRDOUTPUTFILEFORMATTERSETTINGS_H
