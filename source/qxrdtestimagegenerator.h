#ifndef QXRDTESTIMAGEGENERATOR_H
#define QXRDTESTIMAGEGENERATOR_H

#include "qxrdtestgenerator.h"
#include "qcepimagedata-ptr.h"
#include "qcepimagedata.h"

class QxrdTestImageGenerator : public QxrdTestGenerator
{
  Q_OBJECT

public:
  explicit QxrdTestImageGenerator(QString name);

  void start();

protected slots:
  void generateTest();

signals:
  void newImageAvailable(QcepDoubleImageDataPtr data);

public slots:
};

Q_DECLARE_METATYPE(QcepDoubleImageDataPtr)

#endif // QXRDTESTIMAGEGENERATOR_H
