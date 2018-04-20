#ifndef QXRDOUTPUTFILEFORMATTER_H
#define QXRDOUTPUTFILEFORMATTER_H

#include "qxrdlib_global.h"
#include "qcepobject.h"

#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"
#include "qxrdoutputfileformattersettings-ptr.h"

class QXRD_EXPORT QxrdOutputFileFormatter : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  Q_INVOKABLE QxrdOutputFileFormatter(QString name);
  ~QxrdOutputFileFormatter();

  void initialize(QcepObjectWPtr parent);

  static void saveImageData(QxrdOutputFileFormatterSettingsPtr set,
                            QString                            name,
                            QcepImageDataBasePtr               img,
                            QcepImageDataBasePtr               overflow,
                            int                                canOverwrite);
};

#endif // QXRDOUTPUTFILEFORMATTER_H
