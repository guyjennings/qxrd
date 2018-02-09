#include "qxrdroivector.h"
#include "qxrdroi.h"

QxrdROIVector::QxrdROIVector(QString name)
  : inherited(name)
{
}

void QxrdROIVector::initialize(QObjectWPtr parent)
{
  inherited::initialize(parent);
}

void QxrdROIVector::readSettings(QSettings *settings)
{
  int n = settings->beginReadArray("roi");

  //TODO: rewrite to use readObject...
//  m_ROICoordinates.resize(0);

  for (int i=0; i<n; i++) {
    settings->setArrayIndex(i);

    QcepObjectPtr obj = QcepObject::readObject(settings);

    if (obj) {
      QxrdROIPtr roi =
          qSharedPointerDynamicCast<QxrdROI>(obj);

      if (roi) {
        roi->initialize(sharedFromThis());

        int i = m_ROICoordinates.count();

        m_ROICoordinates.append(roi);

        connect(roi.data(), &QxrdROI::roiChanged,
                [=]() { emit roiChanged(i); });
      }
    }
  }

  settings->endArray();

  emit roisChanged();
}

void QxrdROIVector::writeSettings(QSettings *settings)
{
  settings->beginWriteArray("roi");

  for (int i=0; i<m_ROICoordinates.count(); i++) {
    settings->setArrayIndex(i);

    QxrdROIPtr roi = m_ROICoordinates.value(i);

    if (roi) {
      roi->writeSettings(settings);
    }
  }

  settings->endArray();
}

int QxrdROIVector::count() const
{
  return m_ROICoordinates.count();
}

QxrdROIPtr QxrdROIVector::roi(int i)
{
  return m_ROICoordinates.value(i);
}

void QxrdROIVector::setRoi(int i, const QxrdROIPtr &r)
{
  m_ROICoordinates.replace(i, r);
}

void QxrdROIVector::append(QxrdROIPtr roi)
{
  m_ROICoordinates.append(roi);
}

void QxrdROIVector::remove(int i)
{
  m_ROICoordinates.remove(i);
}

void QxrdROIVector::exchange(int i1, int i2)
{
  QxrdROIPtr p1 = m_ROICoordinates.value(i1);
  QxrdROIPtr p2 = m_ROICoordinates.value(i2);

  m_ROICoordinates[i1] = p2;
  m_ROICoordinates[i2] = p1;
}
