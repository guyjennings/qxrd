#include "qxrddataset.h"
#include "qxrdimagedata.h"
#include "qxrdintegrateddata.h"
#include "qxrdallocator.h"

QxrdDataset::QxrdDataset(QcepSettingsSaverWPtr saver, QString name) :
  QcepDataset(saver, name)
{
}

QxrdDoubleImageDataPtr QxrdDataset::image(QString path)
{
  QcepDataObjectPtr obj = item(path);

  return qSharedPointerDynamicCast<QxrdDoubleImageData>(obj);
}

QxrdDoubleImageDataPtr QxrdDataset::newImage(QString path, int width, int height)
{
  QcepDataGroupPtr group = createGroup(directoryName(path));

  if (group) {
    QxrdDoubleImageDataPtr img = QxrdAllocator::newDoubleImage(object(path), width, height);

    if (img) {
      group->append(img);

      emit dataObjectChanged();

      return img;
    }
  }

  return QxrdDoubleImageDataPtr();
}

QxrdIntegratedDataPtr QxrdDataset::integratedData(QString path)
{
  QcepDataObjectPtr obj = item(path);

  return qSharedPointerDynamicCast<QxrdIntegratedData>(obj);
}

QxrdIntegratedDataPtr QxrdDataset::newIntegratedData(QString path, int sz)
{
  QcepDataGroupPtr group = createGroup(directoryName(path));

  if (group) {
    QxrdIntegratedDataPtr integ = QxrdAllocator::newIntegratedData(object(path), sz);

    if (integ) {
      group->append(integ);

      emit dataObjectChanged();

      return integ;
    }
  }

  return QxrdIntegratedDataPtr();
}
