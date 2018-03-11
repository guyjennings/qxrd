#include "qxrdmaskingwindowsettings.h"
#include "qxrdmaskingwindow.h"
#include "qxrdfilebrowsersettings.h"
#include "qxrdimageplotwidgetsettings.h"
#include <QThread>

QxrdMaskingWindowSettings::QxrdMaskingWindowSettings(QString name)
  : inherited(name, "Masking Window"),
    m_FileBrowserSettings(new QxrdFileBrowserSettings(name+"Browser")),
    m_ImagePlotWidgetSettings(new QxrdImagePlotWidgetSettings(name+"ImagePlot"))
{

}

void QxrdMaskingWindowSettings::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  m_FileBrowserSettings     -> initialize(sharedFromThis());
  m_ImagePlotWidgetSettings -> initialize(sharedFromThis());
}

QcepMainWindowPtr QxrdMaskingWindowSettings::newWindow()
{
  GUI_THREAD_CHECK;

  m_Window =
      QxrdMainWindowPtr(
        new QxrdMaskingWindow("Masking"),
        &QObject::deleteLater);

  return m_Window;
}

void QxrdMaskingWindowSettings::readSettings(QSettings *set)
{
  QxrdMainWindowSettings::readSettings(set);

  set->beginGroup("browser");
  m_FileBrowserSettings->readSettings(set);
  set->endGroup();

  set->beginGroup("imagePlot");
  m_ImagePlotWidgetSettings->readSettings(set);
  set->endGroup();
}

void QxrdMaskingWindowSettings::writeSettings(QSettings *set)
{
  QxrdMainWindowSettings::writeSettings(set);

  set->beginGroup("browser");
  m_FileBrowserSettings->writeSettings(set);
  set->endGroup();

  set->beginGroup("imagePlot");
  m_ImagePlotWidgetSettings->writeSettings(set);
  set->endGroup();
}

QxrdFileBrowserSettingsPtr QxrdMaskingWindowSettings::fileBrowserSettings()
{
  return m_FileBrowserSettings;
}

QxrdImagePlotWidgetSettingsPtr QxrdMaskingWindowSettings::imagePlotWidgetSettings()
{
  return m_ImagePlotWidgetSettings;
}
