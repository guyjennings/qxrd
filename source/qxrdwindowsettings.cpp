#include "qxrdwindowsettings.h"

QxrdWindowSettings::QxrdWindowSettings(QxrdSettingsSaverWPtr saver,
                                       QObject *parent) :
  QObject(parent),
  m_WindowGeometry(saver, this, "windowGeometry", QByteArray(), "Window Geometry Settings"),
  m_WindowState(saver, this, "windowState", QByteArray(), "Window State Settings"),
  m_ImagePlotSettings(saver, NULL),
  m_CenterFinderPlotSettings(saver, NULL),
  m_IntegratorPlotSettings(saver, NULL),
  m_InputFileBrowserSettings(saver, NULL),
  m_OutputFileBrowserSettings(saver, NULL),
  m_HistogramDialogSettings(saver, NULL),
  m_SliceDialogSettings(saver, NULL),
  m_InfoDialogSettings(saver, NULL)
{
}

void QxrdWindowSettings::readSettings(QSettings *settings, QString section)
{
  if (settings) {
    QcepProperty::readSettings(this, &staticMetaObject, section, settings);

    m_ImagePlotSettings.readSettings(settings, section+"/plot");
    m_CenterFinderPlotSettings.readSettings(settings, section+"/centerFinderPlot");
    m_IntegratorPlotSettings.readSettings(settings, section+"/integratorPlot");
    m_InputFileBrowserSettings.readSettings(settings, section+"/inputFileBrowser");
    m_OutputFileBrowserSettings.readSettings(settings, section+"/outputFileBrowser");
    m_HistogramDialogSettings.readSettings(settings, section+"/histogramDialog");
    m_SliceDialogSettings.readSettings(settings, section+"/sliceDialog");
    m_InfoDialogSettings.readSettings(settings, section+"/imageInfoDialog");
  }
}

void QxrdWindowSettings::writeSettings(QSettings *settings, QString section)
{
  //    printf("QxrdWindow::writeSettings\n");

  if (settings) {
    QcepProperty::writeSettings(this, &staticMetaObject, section, settings);

    m_ImagePlotSettings.writeSettings(settings, section+"/plot");
    m_CenterFinderPlotSettings.writeSettings(settings, section+"/centerFinderPlot");
    m_IntegratorPlotSettings.writeSettings(settings, section+"/integratorPlot");
    m_InputFileBrowserSettings.writeSettings(settings, section+"/inputFileBrowser");
    m_OutputFileBrowserSettings.writeSettings(settings, section+"/outputFileBrowser");
    m_HistogramDialogSettings.writeSettings(settings, section+"/histogramDialog");
    m_SliceDialogSettings.writeSettings(settings, section+"/sliceDialog");
    m_InfoDialogSettings.writeSettings(settings, section+"/imageInfoDialog");
  }
}

QxrdImagePlotSettingsWPtr QxrdWindowSettings::imagePlotSettings()
{
  return &m_ImagePlotSettings;
}

QxrdCenterFinderPlotSettingsWPtr QxrdWindowSettings::centerFinderPlotSettings()
{
  return &m_CenterFinderPlotSettings;
}

QxrdPlotSettingsWPtr QxrdWindowSettings::integratorPlotSettings()
{
  return &m_IntegratorPlotSettings;
}

QxrdFileBrowserSettingsWPtr QxrdWindowSettings::inputFileBrowserSettings()
{
  return &m_InputFileBrowserSettings;
}

QxrdFileBrowserSettingsWPtr QxrdWindowSettings::outputFileBrowserSettings()
{
  return &m_OutputFileBrowserSettings;
}

QxrdSliceDialogSettingsWPtr QxrdWindowSettings::sliceDialogSettings()
{
  return &m_SliceDialogSettings;
}

QxrdHistogramDialogSettingsWPtr QxrdWindowSettings::histogramDialogSettings()
{
  return &m_HistogramDialogSettings;
}

QxrdInfoDialogSettingsWPtr QxrdWindowSettings::infoDialogSettings()
{
  return &m_InfoDialogSettings;
}
