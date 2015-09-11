#include "qxrddetectorproxy.h"
#include "qxrdacquisition.h"
#include "qxrddetector.h"
#include "qxrddetectorthread.h"
#include "qxrddetectorconfigurationdialog.h"

QxrdDetectorProxy::QxrdDetectorProxy(QxrdDetectorThreadPtr thr,
                                     QxrdDetectorPtr       det,
                                     QxrdAcquisitionPtr    acq)
  : QcepObject("detectorProxy", acq.data()),
    m_Acquisition(acq),
    m_DetectorThread(thr),
    m_Detector(det),
    m_SettingsChanged(false)
{
  if (det) {
    m_DetectorType = det->get_DetectorType();
  }
}

QxrdDetectorProxy::QxrdDetectorProxy(int detectorType, QxrdAcquisitionPtr acq)
  : QcepObject("detectorProxy", acq.data()),
    m_Acquisition(acq),
    m_DetectorThread(),
    m_Detector(),
    m_DetectorType(detectorType)
{
}

int QxrdDetectorProxy::detectorType()
{
  return m_DetectorType;
}

QString QxrdDetectorProxy::detectorTypeName()
{
  return QxrdDetectorThread::detectorTypeName(m_DetectorType);
}

QxrdDetectorThreadPtr QxrdDetectorProxy::detectorThread()
{
  return m_DetectorThread;
}

QxrdDetectorPtr QxrdDetectorProxy::detector()
{
  return m_Detector;
}

bool QxrdDetectorProxy::configureDetector()
{
  if (m_Detector) {
    m_Detector->pushPropertiesToProxy(sharedFromThis());
  } else {
    QxrdDetectorThread::pushDefaultsToProxy(m_DetectorType, sharedFromThis());
  }

  QxrdDetectorConfigurationDialog *d =
      new QxrdDetectorConfigurationDialog(sharedFromThis());

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

void QxrdDetectorProxy::pushProperty(PropertyType type, QString name, QString description, QVariant value)
{
  m_PropertyTypes.append(type);
  m_PropertyNames.append(name);
  m_PropertyDescriptions.append(description);
  m_PropertyValues.append(value);
}

void QxrdDetectorProxy::pushPropertiesToDialog(QxrdDetectorConfigurationDialog *dialog)
{
  printf("Need to implement QxrdDetectorProxy::pushPropertiesToDialog\n");
}

void QxrdDetectorProxy::pullPropertiesFromDialog(QxrdDetectorConfigurationDialog *dialog)
{
  printf("Need to implement QxrdDetectorProxy::pullPropertiesFromDialog\n");
}
