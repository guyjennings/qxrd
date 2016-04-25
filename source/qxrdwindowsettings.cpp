#include "qxrdwindowsettings.h"
#include "qcepsettingssaver.h"
#include "qcepmutexlocker.h"

QxrdWindowSettings::QxrdWindowSettings(QcepSettingsSaverWPtr saver,
                                       QcepObjectWPtr parent) :
  QcepObject("windowSettings", parent),
  m_WindowGeometry(saver, this, "windowGeometry", QByteArray(), "Window Geometry Settings"),
  m_WindowState(saver, this, "windowState", QByteArray(), "Window State Settings")
{
  m_ImagePlotSettings         = QxrdImagePlotSettingsPtr(new QxrdImagePlotSettings(saver, sharedFromThis()));
  m_CenterFinderPlotSettings  = QxrdCenterFinderPlotSettingsPtr(new QxrdCenterFinderPlotSettings(saver,  sharedFromThis()));
  m_IntegratorPlotSettings    = QcepPlotSettingsPtr(new QcepPlotSettings("integratorPlot", saver,  sharedFromThis()));
  m_InputFileBrowserSettings  = QxrdFileBrowserSettingsPtr(new QxrdFileBrowserSettings(saver,  sharedFromThis()));
  m_OutputFileBrowserSettings = QxrdFileBrowserSettingsPtr(new QxrdFileBrowserSettings(saver,  sharedFromThis()));
  m_HistogramDialogSettings   = QxrdHistogramDialogSettingsPtr(new QxrdHistogramDialogSettings(saver,  sharedFromThis()));
  m_SliceDialogSettings       = QxrdSliceDialogSettingsPtr(new QxrdSliceDialogSettings(saver,  sharedFromThis()));
  m_InfoDialogSettings        = QxrdInfoDialogSettingsPtr(new QxrdInfoDialogSettings(saver,  sharedFromThis()));
  m_ScriptDialogSettings      = QxrdScriptDialogSettingsPtr(new QxrdScriptDialogSettings(saver,  sharedFromThis()));
  m_SynchronizedAcquisitionDialogSettings = QxrdSynchronizedAcquisitionDialogSettingsPtr(
        new QxrdSynchronizedAcquisitionDialogSettings(saver,  sharedFromThis()));
  m_AcquisitionExtraInputsDialogSettings = QxrdAcquisitionExtraInputsDialogSettingsPtr(
        new QxrdAcquisitionExtraInputsDialogSettings(saver,  sharedFromThis()));
  m_DistortionCorrectionDialogSettings = QxrdDistortionCorrectionDialogSettingsPtr(
        new QxrdDistortionCorrectionDialogSettings(saver,  sharedFromThis()));
  m_DistortionCorrectionPlotSettings = QxrdDistortionCorrectionPlotSettingsPtr(
        new QxrdDistortionCorrectionPlotSettings(saver,  sharedFromThis()));
}

void QxrdWindowSettings::readSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (settings) {
    QcepProperty::readSettings(this, settings, section);

    m_ImagePlotSettings->readSettings(settings, section+"/plot");
    m_CenterFinderPlotSettings->readSettings(settings, section+"/centerFinderPlot");
    m_IntegratorPlotSettings->readSettings(settings, section+"/integratorPlot");
    m_InputFileBrowserSettings->readSettings(settings, section+"/inputFileBrowser");
    m_OutputFileBrowserSettings->readSettings(settings, section+"/outputFileBrowser");
    m_HistogramDialogSettings->readSettings(settings, section+"/histogramDialog");
    m_SliceDialogSettings->readSettings(settings, section+"/sliceDialog");
    m_InfoDialogSettings->readSettings(settings, section+"/imageInfoDialog");
    m_ScriptDialogSettings->readSettings(settings, section+"/scriptDialog");
    m_SynchronizedAcquisitionDialogSettings->readSettings(settings, section+"/syncAcqDialog");
    m_AcquisitionExtraInputsDialogSettings->readSettings(settings, section+"/extraInputsDialog");
    m_DistortionCorrectionDialogSettings->readSettings(settings, section+"/distortionCorrectionDialog");
    m_DistortionCorrectionPlotSettings->readSettings(settings, section+"/distortionCorrectionPlot");
  }
}

void QxrdWindowSettings::writeSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  //    printf("QxrdWindow::writeSettings\n");

  if (settings) {
    QcepProperty::writeSettings(this, settings, section);

    m_ImagePlotSettings->writeSettings(settings, section+"/plot");
    m_CenterFinderPlotSettings->writeSettings(settings, section+"/centerFinderPlot");
    m_IntegratorPlotSettings->writeSettings(settings, section+"/integratorPlot");
    m_InputFileBrowserSettings->writeSettings(settings, section+"/inputFileBrowser");
    m_OutputFileBrowserSettings->writeSettings(settings, section+"/outputFileBrowser");
    m_HistogramDialogSettings->writeSettings(settings, section+"/histogramDialog");
    m_SliceDialogSettings->writeSettings(settings, section+"/sliceDialog");
    m_InfoDialogSettings->writeSettings(settings, section+"/imageInfoDialog");
    m_ScriptDialogSettings->writeSettings(settings, section+"/scriptDialog");
    m_AcquisitionExtraInputsDialogSettings->writeSettings(settings, section+"/extraInputsDialog");
    m_SynchronizedAcquisitionDialogSettings->writeSettings(settings, section+"/syncAcqDialog");
    m_AcquisitionExtraInputsDialogSettings->writeSettings(settings, section+"/extraInputsDialog");
    m_DistortionCorrectionDialogSettings->writeSettings(settings, section+"/distortionCorrectionDialog");
    m_DistortionCorrectionPlotSettings->writeSettings(settings, section+"/distortionCorrectionPlot");
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

QcepPlotSettingsWPtr QxrdWindowSettings::integratorPlotSettings()
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

QxrdScriptDialogSettingsWPtr QxrdWindowSettings::scriptDialogSettings()
{
  return m_ScriptDialogSettings;
}

QxrdSynchronizedAcquisitionDialogSettingsWPtr QxrdWindowSettings::synchronizedAcquisitionDialogSettings()
{
  return m_SynchronizedAcquisitionDialogSettings;
}

QxrdAcquisitionExtraInputsDialogSettingsWPtr QxrdWindowSettings::acquisitionExtraInputsDialogSettings()
{
  return m_AcquisitionExtraInputsDialogSettings;
}

QxrdDistortionCorrectionDialogSettingsWPtr QxrdWindowSettings::distortionCorrectionDialogSettings()
{
  return m_DistortionCorrectionDialogSettings;
}

QxrdDistortionCorrectionPlotSettingsWPtr QxrdWindowSettings::distortionCorrectionPlotSettings()
{
  return m_DistortionCorrectionPlotSettings;
}
