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
  QxrdDataset(QcepSettingsSaverWPtr saver, QString name);

public slots:
  QcepDoubleImageDataPtr image(QString path);
  QcepDoubleImageDataPtr newImage(QString path, int width=0, int height=0);

  QcepIntegratedDataPtr  integratedData(QString path);
  QcepIntegratedDataPtr  newIntegratedData(QString path, int sz=0);
};

#endif // QXRDDATASET_H
