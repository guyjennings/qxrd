#ifndef QXRDDETECTORPROXY_H
#define QXRDDETECTORPROXY_H

#include "qcepobject.h"
#include "qxrddetectorthread-ptr.h"
#include "qxrddetectorsettings-ptr.h"
#include "qxrdacquisition-ptr.h"
#include <QVector>
#include "qxrddetectorconfigurationdialog.h"

class QxrdDetectorProxy : public QcepObject
{
  Q_OBJECT

public:
  explicit QxrdDetectorProxy(
      QxrdDetectorThreadPtr thr, QxrdDetectorSettingsPtr det, QxrdAcquisitionPtr acq);
  explicit QxrdDetectorProxy(int detectorType,    QxrdAcquisitionPtr acq);
  void initialize();

signals:

public slots:
  int                   detectorType();
  QString               detectorTypeName();
  QxrdDetectorThreadPtr detectorThread();
  QxrdDetectorSettingsPtr       detector();
  int                   detectorNumber();
  QString               detectorName();
  void setDetectorName(QString name);

  bool configureDetector();
  bool settingsChanged();

  bool enabled();
  void setEnabled(bool a);

public:
  typedef enum {
    DetectorNumberProperty,
    DetectorTypeProperty,
    StringProperty,
    IntegerProperty,
    BooleanProperty,
    DoubleProperty,
    DirectoryProperty,
    FilenameProperty,
    ExtensionProperty,
    FixedIntegerProperty,
    PEDetNumProperty,
    PESubTypeProperty,
    PEGainProperty,
    PEBinningProperty,
    PilatusModeProperty
  } PropertyType;

  void clearProperties();
  void pushProperty(PropertyType type, QString name, QString description, QVariant value);
  void pushPropertiesToDialog(QxrdDetectorConfigurationDialog *dialog);
  void pullPropertiesFromDialog(QxrdDetectorConfigurationDialog *dialog);

private:
  QxrdAcquisitionPtr    m_Acquisition;
  QxrdDetectorThreadPtr m_DetectorThread;
  QxrdDetectorSettingsPtr       m_Detector;
  int                   m_DetectorType;
  bool                  m_SettingsChanged;
  bool                  m_Initialized;
  QVector<PropertyType> m_PropertyTypes;
  QVector<QString>      m_PropertyNames;
  QVector<QString>      m_PropertyDescriptions;
  QVector<QVariant>     m_PropertyValues;
};

#endif // QXRDDETECTORPROXY_H
