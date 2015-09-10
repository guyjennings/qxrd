#ifndef QXRDDETECTORPROXY_H
#define QXRDDETECTORPROXY_H

#include "qcepobject.h"
#include "qxrddetectorthread-ptr.h"
#include "qxrddetector-ptr.h"
#include "qxrdacquisition-ptr.h"
#include <QVector>

class QxrdDetectorProxy : public QcepObject, public QEnableSharedFromThis<QxrdDetectorProxy>
{
  Q_OBJECT

public:
  explicit QxrdDetectorProxy(
      QxrdDetectorThreadPtr thr, QxrdDetectorPtr det, QxrdAcquisitionPtr acq);
  explicit QxrdDetectorProxy(int detectorType,    QxrdAcquisitionPtr acq);

signals:

public slots:
  int                   detectorType();
  QString               detectorTypeName();
  QxrdDetectorThreadPtr detectorThread();
  QxrdDetectorPtr       detector();

  void                  configureDetector();

public:
  typedef enum {
    StringProperty,
    IntegerProperty,
    DoubleProperty,
    DirectoryProperty
  } PropertyType;

  void                  pushProperty(PropertyType type,
                                     QString      name,
                                     QVariant     value);

private:
  QxrdAcquisitionPtr    m_Acquisition;
  QxrdDetectorThreadPtr m_DetectorThread;
  QxrdDetectorPtr       m_Detector;
  int                   m_DetectorType;
  QVector<PropertyType> m_PropertyTypes;
  QVector<QString>      m_PropertyNames;
  QVector<QVariant>     m_PropertyValues;
};

#endif // QXRDDETECTORPROXY_H
