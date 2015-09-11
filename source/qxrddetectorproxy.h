#ifndef QXRDDETECTORPROXY_H
#define QXRDDETECTORPROXY_H

#include "qcepobject.h"
#include "qxrddetectorthread-ptr.h"
#include "qxrddetector-ptr.h"
#include "qxrdacquisition-ptr.h"
#include <QVector>
#include "qxrddetectorconfigurationdialog.h"

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

  bool configureDetector();
  bool settingsChanged();
  bool enabled();

public:
  typedef enum {
    StringProperty,
    IntegerProperty,
    DoubleProperty,
    DirectoryProperty
  } PropertyType;

  void pushProperty(PropertyType type, QString name, QString description, QVariant value);
  void pushPropertiesToDialog(QxrdDetectorConfigurationDialog *dialog);
  void pullPropertiesFromDialog(QxrdDetectorConfigurationDialog *dialog);

private:
  QxrdAcquisitionPtr    m_Acquisition;
  QxrdDetectorThreadPtr m_DetectorThread;
  QxrdDetectorPtr       m_Detector;
  int                   m_DetectorType;
  bool                  m_SettingsChanged;
  QVector<PropertyType> m_PropertyTypes;
  QVector<QString>      m_PropertyNames;
  QVector<QString>      m_PropertyDescriptions;
  QVector<QVariant>     m_PropertyValues;
  QVector<QWidget*>     m_PropertyWidgets;
};

#endif // QXRDDETECTORPROXY_H
