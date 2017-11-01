#include "qxrdslicedialogsettings.h"
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

void QxrdSliceDialogSettings::addChildPtr(QcepObjectPtr child)
{
  QcepSerializableObject::addChildPtr(child);

  if (checkPointer<QxrdSlicePlotSettings>(child, m_SlicePlotSettings)) {}
}

void QxrdSliceDialogSettings::readSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepSerializableObject::readSettings(settings);

  if (m_SlicePlotSettings) {
    settings->beginGroup("plot");
    m_SlicePlotSettings->readSettings(settings);
    settings->endGroup();
  }
}

void QxrdSliceDialogSettings::writeSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepSerializableObject::writeSettings(settings);

  if (m_SlicePlotSettings) {
    settings->beginGroup("plot");
    m_SlicePlotSettings->writeSettings(settings);
    settings->endGroup();
  }
}

QxrdSlicePlotSettingsWPtr QxrdSliceDialogSettings::slicePlotSettings()
{
  return m_SlicePlotSettings;
}