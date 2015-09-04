#include "qxrddataset.h"
#include "qcepimagedata.h"
#include "qcepintegrateddata.h"
#include "qcepallocator.h"

QxrdDataset::QxrdDataset(QcepSettingsSaverWPtr saver, QString name, QcepObject *parent) :
  QcepDataset(saver, name, parent)
{
}

QcepDoubleImageDataPtr QxrdDataset::image(QString path)
{
  QcepDataObjectPtr obj = item(path);

  return qSharedPointerDynamicCast<QcepDoubleImageData>(obj);
}

QcepDoubleImageDataPtr QxrdDataset::newImage(QString path, int width, int height)
{
  QcepDataGroupPtr group = createGroup(directoryName(path));

  if (group) {
    QcepDoubleImageDataPtr img = QcepAllocator::newDoubleImage(object(path), width, height, group.data());

    if (img) {
      group->append(img);

      emit dataObjectChanged();

      return img;
    }
  }

  return QcepDoubleImageDataPtr();
}

QcepIntegratedDataPtr QxrdDataset::integratedData(QString path)
{
  QcepDataObjectPtr obj = item(path);

  return qSharedPointerDynamicCast<QcepIntegratedData>(obj);
}

QcepIntegratedDataPtr QxrdDataset::newIntegratedData(QString path, int sz)
{
  QcepDataGroupPtr group = createGroup(directoryName(path));

  if (group) {
    QcepIntegratedDataPtr integ = QcepAllocator::newIntegratedData(object(path), sz, group.data());

    if (integ) {
      group->append(integ);

      emit dataObjectChanged();

      return integ;
    }
  }

  return QcepIntegratedDataPtr();
}
