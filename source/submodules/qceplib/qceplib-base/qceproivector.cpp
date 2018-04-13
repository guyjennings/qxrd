#include "qceproivector.h"
#include "qceproi.h"

QcepROIVector::QcepROIVector(QString name)
  : inherited(name)
{
}

void QcepROIVector::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
}

void QcepROIVector::readSettings(QSettings *settings)
{
  inherited::readSettings(settings);

  int n = settings->beginReadArray("roi");

  //TODO: rewrite to use readObject...

  for (int i=0; i<n; i++) {
    settings->setArrayIndex(i);

    QcepROIPtr roi = QcepROI::readROI(sharedFromThis(), settings);

    if (roi) {
      roi->setObjectName(tr("roi-%1").arg(i));

      int i = m_ROICoordinates.count();

      m_ROICoordinates.append(roi);

      connect(roi.data(), &QcepROI::roiChanged,
              [=]() { emit roiChanged(i); });
    }
  }

  settings->endArray();

  emit roisChanged();
}

void QcepROIVector::writeSettings(QSettings *settings)
{
  inherited::writeSettings(settings);

  settings->beginWriteArray("roi");

  for (int i=0; i<m_ROICoordinates.count(); i++) {
    settings->setArrayIndex(i);

    QcepROIPtr roi = m_ROICoordinates.value(i);

    if (roi) {
      roi->writeSettings(settings);
    }
  }

  settings->endArray();
}

int QcepROIVector::count() const
{
  return m_ROICoordinates.count();
}

QcepROIPtr QcepROIVector::roi(int i)
{
  return m_ROICoordinates.value(i);
}

void QcepROIVector::setRoi(int i, const QcepROIPtr &r)
{
  m_ROICoordinates.replace(i, r);
}

void QcepROIVector::append(QcepROIPtr roi)
{
  m_ROICoordinates.append(roi);
}

void QcepROIVector::remove(int i)
{
  m_ROICoordinates.remove(i);
}

void QcepROIVector::exchange(int i1, int i2)
{
  QcepROIPtr p1 = m_ROICoordinates.value(i1);
  QcepROIPtr p2 = m_ROICoordinates.value(i2);

  m_ROICoordinates[i1] = p2;
  m_ROICoordinates[i2] = p1;
}
