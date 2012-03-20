#include "qxrdhistogramdialogsettings.h"

QxrdHistogramDialogSettings::QxrdHistogramDialogSettings(QxrdSettingsSaverWPtr saver, QObject *parent) :
  QObject(parent),
  m_HistogramRect(saver, this, "histogramRect", QwtDoubleRect(), "Histogram Selection Rectangle"),
  m_HistogramPlotSettings(saver, parent)
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

QxrdHistogramPlotSettingsWPtr QxrdHistogramDialogSettings::histogramPlotSettings()
{
  return &m_HistogramPlotSettings;
}
