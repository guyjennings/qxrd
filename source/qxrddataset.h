#ifndef QXRDDATASET_H
#define QXRDDATASET_H

#include <QObject>
#include "qcepdataset.h"
#include "qcepimagedata-ptr.h"
#include "qcepintegrateddata-ptr.h"

class QxrdDataset : public QcepDataset
{
  Q_OBJECT

public:
  QxrdDataset(QcepSettingsSaverWPtr saver, QString name, QcepObject *parent);

public slots:
  QcepDoubleImageDataPtr image(QString path);
  QcepDoubleImageDataPtr newImage(QString path, int width, int height);

  QcepIntegratedDataPtr  integratedData(QString path);
  QcepIntegratedDataPtr  newIntegratedData(QString path, int sz);
};

#endif // QXRDDATASET_H
