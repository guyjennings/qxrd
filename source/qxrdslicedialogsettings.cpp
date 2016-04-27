#include "qxrdslicedialogsettings.h"
#include "qcepsettingssaver.h"
#include "qcepmutexlocker.h"

QxrdSliceDialogSettings::QxrdSliceDialogSettings(QcepObjectWPtr parent) :
  QcepObject("sliceDialogSettings", parent),
  m_SlicePolygon(this, "slicePolygon", QcepPolygon(), "Slice Polygon")
{
  m_SlicePlotSettings = QxrdSlicePlotSettingsPtr(new QxrdSlicePlotSettings(parent));
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
