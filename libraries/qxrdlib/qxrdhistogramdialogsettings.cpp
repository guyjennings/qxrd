#include "qxrdhistogramdialogsettings.h"
#include "qxrdhistogramplotsettings.h"
#include "qxrdhistogramplotwidgetsettings.h"

QxrdHistogramDialogSettings::QxrdHistogramDialogSettings(QString name) :
  QcepObject(name),
  m_HistogramRect(this, "histogramRect", QRectF(), "Histogram Selection Rectangle"),
  m_HistogramPlotWidgetSettings(new QxrdHistogramPlotWidgetSettings(name))
{
  m_HistogramPlotSettings =
      QxrdHistogramPlotSettings::newHistogramPlotSettings();
}

QxrdHistogramDialogSettingsPtr QxrdHistogramDialogSettings::newHistogramDialogSettings()
{
  QxrdHistogramDialogSettingsPtr set(new QxrdHistogramDialogSettings("histDialogSettings"));

  return set;
}

void QxrdHistogramDialogSettings::readSettings(QSettings *settings)
{
  QcepObject::readSettings(settings);

  if (m_HistogramPlotSettings) {
    settings->beginGroup("plot");
    m_HistogramPlotSettings->readSettings(settings);
    settings->endGroup();
  }

  if (m_HistogramPlotWidgetSettings) {
    settings->beginGroup("plotWidget");
    m_HistogramPlotWidgetSettings->readSettings(settings);
    settings->endGroup();
  }
}

void QxrdHistogramDialogSettings::writeSettings(QSettings *settings)
{
  QcepObject::writeSettings(settings);

  if (m_HistogramPlotSettings) {
    settings->beginGroup("plot");
    m_HistogramPlotSettings->writeSettings(settings);
    settings->endGroup();
  }

  if (m_HistogramPlotWidgetSettings) {
    settings->beginGroup("plotWidget");
    m_HistogramPlotWidgetSettings->writeSettings(settings);
    settings->endGroup();
  }
}

QxrdHistogramPlotSettingsWPtr QxrdHistogramDialogSettings::histogramPlotSettings()
{
  return m_HistogramPlotSettings;
}

QxrdHistogramPlotWidgetSettingsPtr QxrdHistogramDialogSettings::histogramPlotWidgetSettings()
{
  return m_HistogramPlotWidgetSettings;
}
