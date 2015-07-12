#include "qxrdhistogramdialogsettings.h"
#include "qcepsettingssaver.h"

QxrdHistogramDialogSettings::QxrdHistogramDialogSettings(QcepSettingsSaverWPtr saver, QObject *parent) :
  QObject(parent),
  m_HistogramRect(saver, this, "histogramRect", QRectF(), "Histogram Selection Rectangle")
{
  m_HistogramPlotSettings = QxrdHistogramPlotSettingsPtr(new QxrdHistogramPlotSettings(saver, parent));
}

void QxrdHistogramDialogSettings::readSettings(QSettings *settings, QString section)
{
  QcepProperty::readSettings(this, settings, section);

  m_HistogramPlotSettings->readSettings(settings, section+"/plot");
}

void QxrdHistogramDialogSettings::writeSettings(QSettings *settings, QString section)
{
  QcepProperty::writeSettings(this, settings, section);

  m_HistogramPlotSettings->writeSettings(settings, section+"/plot");
}

QxrdHistogramPlotSettingsWPtr QxrdHistogramDialogSettings::histogramPlotSettings()
{
  return m_HistogramPlotSettings;
}
