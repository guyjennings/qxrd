#include "qxrdwindowsettings.h"
#include "qcepsettingssaver.h"
#include "qcepmutexlocker.h"
#include "qxrdimageplotsettings.h"
#include "qxrdcenterfinderplotsettings.h"
#include "qxrdintegratorplotsettings.h"
#include "qxrdfilebrowsersettings.h"
#include "qxrdhistogramdialogsettings.h"
#include "qxrdslicedialogsettings.h"
#include "qxrdinfodialogsettings.h"
#include "qxrdscriptdialogsettings.h"
#include "qxrdsynchronizedacquisitiondialogsettings.h"
#include "qxrdacquisitionextrainputsdialogsettings.h"
#include "qxrddistortioncorrectiondialogsettings.h"
#include "qxrddistortioncorrectionplotsettings.h"

QxrdWindowSettings::QxrdWindowSettings(QString name) :
  QcepObject(name),
  m_WindowGeometry(this, "windowGeometry", QByteArray(), "Window Geometry Settings"),
  m_WindowState(this, "windowState", QByteArray(), "Window State Settings")
{
}

QxrdWindowSettings::QxrdWindowSettings() :
  QxrdWindowSettings("windowSettings")
{
  addChildPtr(QxrdImagePlotSettingsPtr(new QxrdImagePlotSettings()));
  addChildPtr(QxrdCenterFinderPlotSettingsPtr(new QxrdCenterFinderPlotSettings()));
  addChildPtr(QxrdIntegratorPlotSettingsPtr(new QxrdIntegratorPlotSettings()));
  addChildPtr(QxrdInputFileBrowserSettingsPtr(new QxrdInputFileBrowserSettings()));
  addChildPtr(QxrdOutputFileBrowserSettingsPtr(new QxrdOutputFileBrowserSettings()));
  addChildPtr(QxrdHistogramDialogSettingsPtr(new QxrdHistogramDialogSettings()));
  addChildPtr(QxrdSliceDialogSettingsPtr(new QxrdSliceDialogSettings()));
  addChildPtr(QxrdInfoDialogSettingsPtr(new QxrdInfoDialogSettings()));
  addChildPtr(QxrdScriptDialogSettingsPtr(new QxrdScriptDialogSettings()));
  addChildPtr(QxrdSynchronizedAcquisitionDialogSettingsPtr(new QxrdSynchronizedAcquisitionDialogSettings()));
  addChildPtr(QxrdAcquisitionExtraInputsDialogSettingsPtr(new QxrdAcquisitionExtraInputsDialogSettings()));
  addChildPtr(QxrdDistortionCorrectionDialogSettingsPtr(new QxrdDistortionCorrectionDialogSettings()));
  addChildPtr(QxrdDistortionCorrectionPlotSettingsPtr(new QxrdDistortionCorrectionPlotSettings()));
}

void QxrdWindowSettings::addChildPtr(QcepObjectPtr child)
{
  QcepObject::addChildPtr(child);

  if (checkPointer<QxrdCenterFinderPlotSettings>(child, m_CenterFinderPlotSettings)) {}
  else if (checkPointer<QxrdIntegratorPlotSettings>(child, m_IntegratorPlotSettings)) {}
  else if (checkPointer<QxrdInputFileBrowserSettings>(child,m_InputFileBrowserSettings)) {}
  else if (checkPointer<QxrdOutputFileBrowserSettings>(child, m_OutputFileBrowserSettings)) {}
  else if (checkPointer<QxrdHistogramDialogSettings>(child, m_HistogramDialogSettings)) {}
  else if (checkPointer<QxrdSliceDialogSettings>(child, m_SliceDialogSettings)) {}
  else if (checkPointer<QxrdInfoDialogSettings>(child, m_InfoDialogSettings)) {}
  else if (checkPointer<QxrdScriptDialogSettings>(child, m_ScriptDialogSettings)) {}
  else if (checkPointer<QxrdSynchronizedAcquisitionDialogSettings>(child, m_SynchronizedAcquisitionDialogSettings)) {}
  else if (checkPointer<QxrdAcquisitionExtraInputsDialogSettings>(child, m_AcquisitionExtraInputsDialogSettings)) {}
  else if (checkPointer<QxrdDistortionCorrectionDialogSettings>(child, m_DistortionCorrectionDialogSettings)) {}
  else if (checkPointer<QxrdDistortionCorrectionPlotSettings>(child, m_DistortionCorrectionPlotSettings)) {}
  else if (checkPointer<QxrdImagePlotSettings>(child, m_ImagePlotSettings)) {}
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

QxrdIntegratorPlotSettingsWPtr QxrdWindowSettings::integratorPlotSettings()
{
  return m_IntegratorPlotSettings;
}

QxrdInputFileBrowserSettingsWPtr QxrdWindowSettings::inputFileBrowserSettings()
{
  return m_InputFileBrowserSettings;
}

QxrdOutputFileBrowserSettingsWPtr QxrdWindowSettings::outputFileBrowserSettings()
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
