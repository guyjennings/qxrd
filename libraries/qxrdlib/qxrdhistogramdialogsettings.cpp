#include "qxrdhistogramdialogsettings.h"
#include "qxrdhistogramplotsettings.h"

QxrdHistogramDialogSettings::QxrdHistogramDialogSettings(QString name) :
  QcepObject(name),
  m_HistogramRect(this, "histogramRect", QRectF(), "Histogram Selection Rectangle")
{
}

QxrdHistogramDialogSettingsPtr QxrdHistogramDialogSettings::newHistogramDialogSettings()
{
  QxrdHistogramDialogSettingsPtr set(new QxrdHistogramDialogSettings("histDialogSettings"));

  set->addChildPtr(QxrdHistogramPlotSettings::newHistogramPlotSettings());

  return set;
}

void QxrdHistogramDialogSettings::addChildPtr(QcepObjectPtr child)
{
  QcepObject::addChildPtr(child);

  if (checkPointer<QxrdHistogramPlotSettings>(child, m_HistogramPlotSettings)) {}
}

void QxrdHistogramDialogSettings::readSettings(QSettings *settings)
{
  QcepObject::readSettings(settings);

  if (m_HistogramPlotSettings) {
    settings->beginGroup("plot");
    m_HistogramPlotSettings->readSettings(settings);
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
}

QxrdHistogramPlotSettingsWPtr QxrdHistogramDialogSettings::histogramPlotSettings()
{
  return m_HistogramPlotSettings;
}
