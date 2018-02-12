#include "qxrdroivector.h"
#include "qxrdroi.h"

QxrdROIVector::QxrdROIVector(QString name)
  : inherited(name)
{
}

void QxrdROIVector::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
}

void QxrdROIVector::readSettings(QSettings *settings)
{
  inherited::readSettings(settings);

  int n = settings->beginReadArray("roi");

  //TODO: rewrite to use readObject...

  for (int i=0; i<n; i++) {
    settings->setArrayIndex(i);

    QxrdROIPtr roi = QxrdROI::readROI(sharedFromThis(), settings);

    if (roi) {
      roi->setObjectName(tr("roi-%1").arg(i));

      int i = m_ROICoordinates.count();

      m_ROICoordinates.append(roi);

      connect(roi.data(), &QxrdROI::roiChanged,
              [=]() { emit roiChanged(i); });
    }
  }

  settings->endArray();

  emit roisChanged();
}

void QxrdROIVector::writeSettings(QSettings *settings)
{
  inherited::writeSettings(settings);

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
