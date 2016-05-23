#include "qxrddetectorproxy.h"
#include "qxrdacquisition.h"
#include "qxrddetectorsettings.h"
#include "qxrddetectorconfigurationdialog.h"
#include "qcepproperty.h"
#include <QLineEdit>
#include <QCheckBox>

QxrdDetectorProxy::QxrdDetectorProxy(QxrdDetectorSettingsPtr       det,
                                     QxrdAcquisitionPtr    acq)
  : QcepObject("detectorProxy"),
    m_Acquisition(acq),
    m_Detector(det),
    m_DetectorType(QxrdDetectorSettings::NoDetector),
    m_SettingsChanged(false),
    m_Initialized(false)
{

  if (det) {
    setEnabled(det->get_Enabled());
    m_DetectorType = det->get_DetectorType();
  }
}

QxrdDetectorProxy::QxrdDetectorProxy(int detectorType, QxrdAcquisitionPtr acq)
  : QcepObject("detectorProxy"),
    m_Acquisition(acq),
    m_Detector(),
    m_DetectorType(detectorType),
    m_SettingsChanged(false),
    m_Initialized(false)
{
  setEnabled(true);
}

void QxrdDetectorProxy::initialize()
{
  if (!m_Initialized) {
    QxrdDetectorProxyPtr myself(qSharedPointerDynamicCast<QxrdDetectorProxy>(sharedFromThis()));

    if (myself) {
      if (m_Detector) {
        m_Detector->pushPropertiesToProxy(myself);
      } else {
        QxrdDetectorSettings::pushDefaultsToProxy(myself, m_DetectorType);
      }

      m_Initialized = true;
    } else {
      printf("Proxy initialize failed\n");
    }
  }
}

bool QxrdDetectorProxy::enabled()
{
  return property("enabled").toBool();
}

void QxrdDetectorProxy::setEnabled(bool a)
{
  setProperty("enabled", a);
}

int QxrdDetectorProxy::detectorNumber()
{
  return property("detectorNumber").toInt();
}

int QxrdDetectorProxy::detectorType()
{
  return property("detectorType").toInt();
}

QString QxrdDetectorProxy::detectorTypeName()
{
  return QxrdDetectorSettings::detectorTypeName(detectorType());
}

QString QxrdDetectorProxy::detectorName()
{
  return property("detectorName").toString();
}

void QxrdDetectorProxy::setDetectorName(QString name)
{
  setProperty("detectorName", name);
}

QxrdDetectorSettingsPtr QxrdDetectorProxy::detector()
{
  return m_Detector;
}

bool QxrdDetectorProxy::configureDetector()
{
  QxrdDetectorProxyPtr myself(qSharedPointerDynamicCast<QxrdDetectorProxy>(sharedFromThis()));

  if (m_Detector) {
    m_Detector->pushPropertiesToProxy(myself);
  } else {
    QxrdDetectorSettings::pushDefaultsToProxy(myself, m_DetectorType);
  }

  QxrdDetectorConfigurationDialog *d =
      new QxrdDetectorConfigurationDialog(myself);

  if (d->exec() == QDialog::Accepted) {
    m_SettingsChanged = true;
  }

  delete d;

  return m_SettingsChanged;
}

bool QxrdDetectorProxy::settingsChanged()
{
  return m_SettingsChanged;
}

void QxrdDetectorProxy::clearProperties()
{
  m_PropertyTypes.clear();
  m_PropertyNames.clear();
  m_PropertyDescriptions.clear();
  m_PropertyValues.clear();
}

void QxrdDetectorProxy::pushProperty(PropertyType type, QString name, QString description, QVariant value)
{
  m_PropertyTypes.append(type);
  m_PropertyNames.append(name);
  m_PropertyDescriptions.append(description);
  m_PropertyValues.append(value);

  setProperty(qPrintable(name), value);
}

void QxrdDetectorProxy::pushPropertiesToDialog(QxrdDetectorConfigurationDialog *dialog)
{
  if (dialog) {
    int nProps = m_PropertyTypes.count();

    for (int i = 0; i<nProps; i++) {
      dialog->appendProperty(m_PropertyTypes.value(i),
                             m_PropertyNames.value(i),
                             m_PropertyDescriptions.value(i),
                             m_PropertyValues.value(i));
    }
  }
}

void QxrdDetectorProxy::pullPropertiesFromDialog(QxrdDetectorConfigurationDialog *dialog)
{
  if (dialog) {
    int nProps = m_PropertyTypes.count();

    for (int i=0; i<nProps; i++) {
      int      propType = m_PropertyTypes.value(i);
      QString  propName = m_PropertyNames.value(i);
      QVariant propVal  = dialog->propertyValue(propType, i);

      if (propVal.isValid()) {
        setProperty(qPrintable(propName), propVal);
      }
    }

    clearProperties();
  }
}
