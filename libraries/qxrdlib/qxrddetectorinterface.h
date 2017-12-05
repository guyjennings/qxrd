#ifndef QXRDDETECTORINTERFACE_H
#define QXRDDETECTORINTERFACE_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepobject.h"
#include "qcepimagedata-ptr.h"

class QXRD_EXPORT QxrdDetectorInterface : public QcepObject
{
  Q_OBJECT
public:
  QxrdDetectorInterface(QString name);

signals:
  void imageDataAvailable(QcepDoubleImageDataPtr img);

public slots:
  void ping();

  QcepDoubleImageDataPtr newImage();
};

#endif // QXRDDETECTORINTERFACE_H
