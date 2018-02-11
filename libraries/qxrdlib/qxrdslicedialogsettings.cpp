#include "qxrdslicedialogsettings.h"
#include "qcepmutexlocker.h"
#include "qxrdsliceplotsettings.h"

QxrdSliceDialogSettings::QxrdSliceDialogSettings(QString name) :
  inherited(name),
  m_SlicePolygon(this, "slicePolygon", QcepPolygon(), "Slice Polygon")
{
  m_SlicePlotSettings =
      QxrdSlicePlotSettings::newSlicePlotSettings();
}

QxrdSliceDialogSettingsPtr QxrdSliceDialogSettings::newSliceDialogSettings()
{
  QxrdSliceDialogSettingsPtr set(new QxrdSliceDialogSettings("sliceDialogSettings"));

  return set;
}

void QxrdSliceDialogSettings::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  if (m_SlicePlotSettings) {
    m_SlicePlotSettings -> initialize(sharedFromThis());
  }
}
void QxrdSliceDialogSettings::readSettings(QSettings *settings)
{
  inherited::readSettings(settings);

  if (m_SlicePlotSettings) {
    settings->beginGroup("plot");
    m_SlicePlotSettings->readSettings(settings);
    settings->endGroup();
  }
}

void QxrdSliceDialogSettings::writeSettings(QSettings *settings)
{
  inherited::writeSettings(settings);

  if (m_SlicePlotSettings) {
    settings->beginGroup("plot");
    m_SlicePlotSettings->writeSettings(settings);
    settings->endGroup();
  }
}

QxrdSlicePlotSettingsWPtr QxrdSliceDialogSettings::slicePlotSettings()
{
  return m_SlicePlotSettings;
}
