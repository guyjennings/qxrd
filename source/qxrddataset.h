#ifndef QXRDDATASET_H
#define QXRDDATASET_H

#include <QObject>
#include "qcepdataset.h"
#include "qxrdimagedata-ptr.h"
#include "qxrdintegrateddata-ptr.h"

class QxrdDataset : public QcepDataset
{
  Q_OBJECT

public:
  QxrdDataset(QcepSettingsSaverWPtr saver, QString name);

public slots:
  QxrdDoubleImageDataPtr image(QString path);
  QxrdDoubleImageDataPtr newImage(QString path, int width=0, int height=0);

  QxrdIntegratedDataPtr  integratedData(QString path);
  QxrdIntegratedDataPtr  newIntegratedData(QString path, int sz=0);
};

#endif // QXRDDATASET_H
