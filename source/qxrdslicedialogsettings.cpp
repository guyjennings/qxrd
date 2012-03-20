#include "qxrdslicedialogsettings.h"

QxrdSliceDialogSettings::QxrdSliceDialogSettings(QxrdSettingsSaverWPtr saver, QObject *parent) :
  QObject(parent),
  m_SlicePolygon(saver, this, "slicePolygon", QcepPolygon(), "Slice Polygon")
{
  m_SlicePlotSettings = QxrdSlicePlotSettingsPtr(new QxrdSlicePlotSettings(saver, parent));
}

void QxrdSliceDialogSettings::readSettings(QSettings *settings, QString section)
{
  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
  m_SlicePlotSettings->readSettings(settings, section+"/plot");
}

void QxrdSliceDialogSettings::writeSettings(QSettings *settings, QString section)
{
  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
  m_SlicePlotSettings->writeSettings(settings, section+"/plot");
}

