#include "qxrdhistogramdialogsettings.h"
#include "qxrdhistogramplotsettings.h"

QxrdHistogramDialogSettings::QxrdHistogramDialogSettings(QString name) :
  QcepSerializableObject(name),
  m_HistogramRect(this, "histogramRect", QRectF(), "Histogram Selection Rectangle")
{
}

QxrdHistogramDialogSettingsPtr QxrdHistogramDialogSettings::newHistogramDialogSettings()
{
  QxrdHistogramDialogSettingsPtr set(new QxrdHistogramDialogSettings("histDialogSettings"));

  set->addChildPtr(QxrdHistogramPlotSettings::newHistogramPlotSettings());

  return set;
}

void QxrdHistogramDialogSettings::addChildPtr(QcepSerializableObjectPtr child)
{
  QcepSerializableObject::addChildPtr(child);

  if (checkPointer<QxrdHistogramPlotSettings>(child, m_HistogramPlotSettings)) {}
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
