#include "qxrdslicedialogsettings.h"
#include "qcepsettingssaver.h"
#include "qcepmutexlocker.h"
#include "qxrdsliceplotsettings.h"

QxrdSliceDialogSettings::QxrdSliceDialogSettings(QString name) :
  QcepSerializableObject(name),
  m_SlicePolygon(this, "slicePolygon", QcepPolygon(), "Slice Polygon")
{
}

QxrdSliceDialogSettingsPtr QxrdSliceDialogSettings::newSliceDialogSettings()
{
  QxrdSliceDialogSettingsPtr set(new QxrdSliceDialogSettings("sliceDialogSettings"));

  set -> addChildPtr(QxrdSlicePlotSettings::newSlicePlotSettings());

  return set;
}

void QxrdSliceDialogSettings::addChildPtr(QcepSerializableObjectPtr child)
{
  QcepSerializableObject::addChildPtr(child);

  if (checkPointer<QxrdSlicePlotSettings>(child, m_SlicePlotSettings)) {}
}

void QxrdSliceDialogSettings::readSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, settings, section);

  m_SlicePlotSettings->readSettings(settings, section+"/plot");
}

void QxrdSliceDialogSettings::writeSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, settings, section);

  m_SlicePlotSettings->writeSettings(settings, section+"/plot");
}

QxrdSlicePlotSettingsWPtr QxrdSliceDialogSettings::slicePlotSettings()
{
  return m_SlicePlotSettings;
}
