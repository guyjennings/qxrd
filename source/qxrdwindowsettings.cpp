#include "qxrdwindowsettings.h"

QxrdWindowSettings::QxrdWindowSettings(QxrdSettingsSaverWPtr saver,
                                       QObject *parent) :
  QObject(parent),
  m_WindowGeometry(saver, this, "windowGeometry", QByteArray(), "Window Geometry Settings"),
  m_WindowState(saver, this, "windowState", QByteArray(), "Window State Settings")
{
  m_ImagePlotSettings         = QxrdImagePlotSettingsPtr(new QxrdImagePlotSettings(saver, NULL));
  m_CenterFinderPlotSettings  = QxrdCenterFinderPlotSettingsPtr(new QxrdCenterFinderPlotSettings(saver, NULL));
  m_IntegratorPlotSettings    = QxrdPlotSettingsPtr(new QxrdPlotSettings(saver, NULL));
  m_InputFileBrowserSettings  = QxrdFileBrowserSettingsPtr(new QxrdFileBrowserSettings(saver, NULL));
  m_OutputFileBrowserSettings = QxrdFileBrowserSettingsPtr(new QxrdFileBrowserSettings(saver, NULL));
  m_HistogramDialogSettings   = QxrdHistogramDialogSettingsPtr(new QxrdHistogramDialogSettings(saver, NULL));
  m_SliceDialogSettings       = QxrdSliceDialogSettingsPtr(new QxrdSliceDialogSettings(saver, NULL));
  m_InfoDialogSettings        = QxrdInfoDialogSettingsPtr(new QxrdInfoDialogSettings(saver, NULL));
  m_AcquisitionExtraInputsDialogSettings = QxrdAcquisitionExtraInputsDialogSettingsPtr(
        new QxrdAcquisitionExtraInputsDialogSettings(saver, NULL));
}

void QxrdWindowSettings::readSettings(QSettings *settings, QString section)
{
  if (settings) {
    QcepProperty::readSettings(this, &staticMetaObject, section, settings);

    m_ImagePlotSettings->readSettings(settings, section+"/plot");
    m_CenterFinderPlotSettings->readSettings(settings, section+"/centerFinderPlot");
    m_IntegratorPlotSettings->readSettings(settings, section+"/integratorPlot");
    m_InputFileBrowserSettings->readSettings(settings, section+"/inputFileBrowser");
    m_OutputFileBrowserSettings->readSettings(settings, section+"/outputFileBrowser");
    m_HistogramDialogSettings->readSettings(settings, section+"/histogramDialog");
    m_SliceDialogSettings->readSettings(settings, section+"/sliceDialog");
    m_InfoDialogSettings->readSettings(settings, section+"/imageInfoDialog");
  }
}

void QxrdWindowSettings::writeSettings(QSettings *settings, QString section)
{
  //    printf("QxrdWindow::writeSettings\n");

  if (settings) {
    QcepProperty::writeSettings(this, &staticMetaObject, section, settings);

    m_ImagePlotSettings->writeSettings(settings, section+"/plot");
    m_CenterFinderPlotSettings->writeSettings(settings, section+"/centerFinderPlot");
    m_IntegratorPlotSettings->writeSettings(settings, section+"/integratorPlot");
    m_InputFileBrowserSettings->writeSettings(settings, section+"/inputFileBrowser");
    m_OutputFileBrowserSettings->writeSettings(settings, section+"/outputFileBrowser");
    m_HistogramDialogSettings->writeSettings(settings, section+"/histogramDialog");
    m_SliceDialogSettings->writeSettings(settings, section+"/sliceDialog");
    m_InfoDialogSettings->writeSettings(settings, section+"/imageInfoDialog");
    m_AcquisitionExtraInputsDialogSettings->writeSettings(settings, section+"/extraInputsDialog");
  }
}

QxrdImagePlotSettingsWPtr QxrdWindowSettings::imagePlotSettings()
{
  return m_ImagePlotSettings;
}

QxrdCenterFinderPlotSettingsWPtr QxrdWindowSettings::centerFinderPlotSettings()
{
  return m_CenterFinderPlotSettings;
}

QxrdPlotSettingsWPtr QxrdWindowSettings::integratorPlotSettings()
{
  return m_IntegratorPlotSettings;
}

QxrdFileBrowserSettingsWPtr QxrdWindowSettings::inputFileBrowserSettings()
{
  return m_InputFileBrowserSettings;
}

QxrdFileBrowserSettingsWPtr QxrdWindowSettings::outputFileBrowserSettings()
{
  return m_OutputFileBrowserSettings;
}

QxrdSliceDialogSettingsWPtr QxrdWindowSettings::sliceDialogSettings()
{
  return m_SliceDialogSettings;
}

QxrdHistogramDialogSettingsWPtr QxrdWindowSettings::histogramDialogSettings()
{
  return m_HistogramDialogSettings;
}

QxrdInfoDialogSettingsWPtr QxrdWindowSettings::infoDialogSettings()
{
  return m_InfoDialogSettings;
}

QxrdAcquisitionExtraInputsDialogSettingsWPtr QxrdWindowSettings::acquisitionExtraInputsDialogSettings()
{
  return m_AcquisitionExtraInputsDialogSettings;
}
