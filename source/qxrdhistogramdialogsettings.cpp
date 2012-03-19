#include "qxrdhistogramdialogsettings.h"

QxrdHistogramDialogSettings::QxrdHistogramDialogSettings(QxrdSettingsSaverWPtr saver, QObject *parent) :
  QObject(parent),
  m_Saver(saver)
{
}

void QxrdHistogramDialogSettings::readSettings(QSettings *settings, QString section)
{
  QcepProperty::readSettings(this, &staticMetaObject, section, settings);

  m_HistogramPlotSettings.readSettings(settings, section+"/plot");
}

void QxrdHistogramDialogSettings::writeSettings(QSettings *settings, QString section)
{
  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);

  m_HistogramPlotSettings.writeSettings(settings, section+"/plot");
}

