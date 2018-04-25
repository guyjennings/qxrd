#include "qxrdhistogramdialogsettings.h"
#include "qxrdhistogramplotwidgetsettings.h"

QxrdHistogramDialogSettings::QxrdHistogramDialogSettings(QString name) :
  QcepObject(name),
  m_HistogramRect(this, "histogramRect", QRectF(), "Histogram Selection Rectangle"),
  m_HistogramPlotWidgetSettings(NEWPTR(QxrdHistogramPlotWidgetSettings(name)))
{
}

void QxrdHistogramDialogSettings::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  if (m_HistogramPlotWidgetSettings) {
    m_HistogramPlotWidgetSettings -> initialize(sharedFromThis());
  }
}

void QxrdHistogramDialogSettings::registerMetaTypes()
{
  qRegisterMetaType<QxrdHistogramDialogSettings*>("QxrdHistogramDialogSettings*");
  qRegisterMetaType<QxrdHistogramPlotWidgetSettings*>("QxrdHistogramPlotWidgetSettings*");
}

void QxrdHistogramDialogSettings::readSettings(QSettings *settings)
{
  QcepObject::readSettings(settings);

  if (m_HistogramPlotWidgetSettings) {
    settings->beginGroup("plotWidget");
    m_HistogramPlotWidgetSettings->readSettings(settings);
    settings->endGroup();
  }
}

void QxrdHistogramDialogSettings::writeSettings(QSettings *settings)
{
  QcepObject::writeSettings(settings);

  if (m_HistogramPlotWidgetSettings) {
    settings->beginGroup("plotWidget");
    m_HistogramPlotWidgetSettings->writeSettings(settings);
    settings->endGroup();
  }
}

QxrdHistogramPlotWidgetSettingsPtr QxrdHistogramDialogSettings::histogramPlotWidgetSettings()
{
  return m_HistogramPlotWidgetSettings;
}
