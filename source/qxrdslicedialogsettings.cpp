#include "qxrdslicedialogsettings.h"
#include "qcepsettingssaver.h"

QxrdSliceDialogSettings::QxrdSliceDialogSettings(QcepSettingsSaverWPtr saver, QObject *parent) :
  QObject(parent),
  m_SlicePolygon(saver, this, "slicePolygon", QcepPolygon(), "Slice Polygon")
{
  m_SlicePlotSettings = QxrdSlicePlotSettingsPtr(new QxrdSlicePlotSettings(saver, parent));
}

void QxrdSliceDialogSettings::readSettings(QSettings *settings, QString section)
{
  QcepProperty::readSettings(this, settings, section);
  m_SlicePlotSettings->readSettings(settings, section+"/plot");
}

void QxrdSliceDialogSettings::writeSettings(QSettings *settings, QString section)
{
  QcepProperty::writeSettings(this, settings, section);
  m_SlicePlotSettings->writeSettings(settings, section+"/plot");
}

QxrdSlicePlotSettingsWPtr QxrdSliceDialogSettings::slicePlotSettings()
{
  return m_SlicePlotSettings;
}
