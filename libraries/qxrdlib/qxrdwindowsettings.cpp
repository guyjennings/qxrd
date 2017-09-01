#include "qxrdwindowsettings.h"
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
#include "qxrdsliceplotsettings.h"
#include "qxrdsynchronizedacquisitionplotsettings.h"
#include "qxrdacquisitionextrainputsplotsettings.h"
#include "qxrdhistogramplotsettings.h"

QxrdWindowSettings::QxrdWindowSettings(QString name) :
  QcepSerializableObject(name),
  m_WindowGeometry(this, "windowGeometry", QByteArray(), "Window Geometry Settings"),
  m_WindowState(this, "windowState", QByteArray(), "Window State Settings")
{
}

QxrdWindowSettingsPtr QxrdWindowSettings::newWindowSettings()
{
  QxrdWindowSettingsPtr set(new QxrdWindowSettings("windowSettings"));

  set -> addChildPtr(QxrdImagePlotSettings::newImagePlotSettings());
  set -> addChildPtr(QxrdCenterFinderPlotSettings::newCenterFinderPlotSettings());
  set -> addChildPtr(QxrdIntegratorPlotSettings::newIntegratorPlotSettings());
  set -> addChildPtr(QxrdInputFileBrowserSettings::newInputFileBrowserSettings());
  set -> addChildPtr(QxrdOutputFileBrowserSettings::newOutputFileBrowserSettings());
  set -> addChildPtr(QxrdHistogramDialogSettings::newHistogramDialogSettings());
  set -> addChildPtr(QxrdSliceDialogSettings::newSliceDialogSettings());
  set -> addChildPtr(QxrdInfoDialogSettings::newInfoDialogSettings());
  set -> addChildPtr(QxrdScriptDialogSettings::newScriptDialogSettings());
  set -> addChildPtr(QxrdSynchronizedAcquisitionDialogSettings::newSynchronizedAcquisitionDialogSettings());
  set -> addChildPtr(QxrdAcquisitionExtraInputsDialogSettings::newAcquisitionExtraInputsDialogSettings());
  set -> addChildPtr(QxrdDistortionCorrectionDialogSettings::newDistortionCorrectionDialogSettings());
  set -> addChildPtr(QxrdDistortionCorrectionPlotSettings::newDistortionCorrectionPlotSettings());

  return set;
}

void QxrdWindowSettings::addChildPtr(QcepSerializableObjectPtr child)
{
  QcepSerializableObject::addChildPtr(child);

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

void QxrdWindowSettings::registerMetaTypes()
{
  qRegisterMetaType<QxrdWindowSettings*>("QxrdWindowSettings*");
  qRegisterMetaType<QxrdImagePlotSettings*>("QxrdImagePlotSettings*");
  qRegisterMetaType<QxrdCenterFinderPlotSettings*>("QxrdCenterFinderPlotSettings*");
  qRegisterMetaType<QxrdIntegratorPlotSettings*>("QxrdIntegratorPlotSettings*");
  qRegisterMetaType<QxrdInputFileBrowserSettings*>("QxrdInputFileBrowserSettings*");
  qRegisterMetaType<QxrdOutputFileBrowserSettings*>("QxrdOutputFileBrowserSettings*");
  qRegisterMetaType<QxrdHistogramDialogSettings*>("QxrdHistogramDialogSettings*");
  qRegisterMetaType<QxrdHistogramPlotSettings*>("QxrdHistogramPlotSettings*");
  qRegisterMetaType<QxrdSliceDialogSettings*>("QxrdSliceDialogSettings*");
  qRegisterMetaType<QxrdSlicePlotSettings*>("QxrdSlicePlotSettings*");
  qRegisterMetaType<QxrdInfoDialogSettings*>("QxrdInfoDialogSettings*");
  qRegisterMetaType<QxrdScriptDialogSettings*>("QxrdScriptDialogSettings*");
  qRegisterMetaType<QxrdSynchronizedAcquisitionDialogSettings*>("QxrdSynchronizedAcquisitionDialogSettings*");
  qRegisterMetaType<QxrdSynchronizedAcquisitionPlotSettings*>("QxrdSynchronizedAcquisitionPlotSettings*");
  qRegisterMetaType<QxrdAcquisitionExtraInputsDialogSettings*>("QxrdAcquisitionExtraInputsDialogSettings*");
  qRegisterMetaType<QxrdAcquisitionExtraInputsPlotSettings*>("QxrdAcquisitionExtraInputsPlotSettings*");
  qRegisterMetaType<QxrdDistortionCorrectionDialogSettings*>("QxrdDistortionCorrectionDialogSettings*");
  qRegisterMetaType<QxrdDistortionCorrectionPlotSettings*>("QxrdDistortionCorrectionPlotSettings*");
}

void QxrdWindowSettings::readSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (settings) {
    QcepSerializableObject::readSettings(settings);

    settings->beginGroup("plot");
    m_ImagePlotSettings->readSettings(settings);
    settings->endGroup();

    settings->beginGroup("centerFinderPlot");
    m_CenterFinderPlotSettings->readSettings(settings);
    settings->endGroup();

    settings->beginGroup("integratorPlot");
    m_IntegratorPlotSettings->readSettings(settings);
    settings->endGroup();

    settings->beginGroup("inputFileBrowser");
    m_InputFileBrowserSettings->readSettings(settings);
    settings->endGroup();

    settings->beginGroup("outputFileBrowser");
    m_OutputFileBrowserSettings->readSettings(settings);
    settings->endGroup();

    settings->beginGroup("histogramDialog");
    m_HistogramDialogSettings->readSettings(settings);
    settings->endGroup();

    settings->beginGroup("sliceDialog");
    m_SliceDialogSettings->readSettings(settings);
    settings->endGroup();

    settings->beginGroup("imageInfoDialog");
    m_InfoDialogSettings->readSettings(settings);
    settings->endGroup();

    settings->beginGroup("scriptDialog");
    m_ScriptDialogSettings->readSettings(settings);
    settings->endGroup();

    settings->beginGroup("syncAcqDialog");
    m_SynchronizedAcquisitionDialogSettings->readSettings(settings);
    settings->endGroup();

    settings->beginGroup("extraInputsDialog");
    m_AcquisitionExtraInputsDialogSettings->readSettings(settings);
    settings->endGroup();

    settings->beginGroup("distortionCorrectionDialog");
    m_DistortionCorrectionDialogSettings->readSettings(settings);
    settings->endGroup();

    settings->beginGroup("distortionCorrectionPlot");
    m_DistortionCorrectionPlotSettings->readSettings(settings);
    settings->endGroup();
  }
}

void QxrdWindowSettings::writeSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  //    printf("QxrdWindow::writeSettings\n");

  if (settings) {
    QcepSerializableObject::writeSettings(settings);

    settings->beginGroup("plot");
    m_ImagePlotSettings->writeSettings(settings);
    settings->endGroup();

    settings->beginGroup("centerFinderPlot");
    m_CenterFinderPlotSettings->writeSettings(settings);
    settings->endGroup();

    settings->beginGroup("integratorPlot");
    m_IntegratorPlotSettings->writeSettings(settings);
    settings->endGroup();

    settings->beginGroup("inputFileBrowser");
    m_InputFileBrowserSettings->writeSettings(settings);
    settings->endGroup();

    settings->beginGroup("outputFileBrowser");
    m_OutputFileBrowserSettings->writeSettings(settings);
    settings->endGroup();

    settings->beginGroup("histogramDialog");
    m_HistogramDialogSettings->writeSettings(settings);
    settings->endGroup();

    settings->beginGroup("sliceDialog");
    m_SliceDialogSettings->writeSettings(settings);
    settings->endGroup();

    settings->beginGroup("imageInfoDialog");
    m_InfoDialogSettings->writeSettings(settings);
    settings->endGroup();

    settings->beginGroup("scriptDialog");
    m_ScriptDialogSettings->writeSettings(settings);
    settings->endGroup();

    settings->beginGroup("extraInputsDialog");
    m_AcquisitionExtraInputsDialogSettings->writeSettings(settings);
    settings->endGroup();

    settings->beginGroup("syncAcqDialog");
    m_SynchronizedAcquisitionDialogSettings->writeSettings(settings);
    settings->endGroup();

    settings->beginGroup("extraInputsDialog");
    m_AcquisitionExtraInputsDialogSettings->writeSettings(settings);
    settings->endGroup();

    settings->beginGroup("distortionCorrectionDialog");
    m_DistortionCorrectionDialogSettings->writeSettings(settings);
    settings->endGroup();

    settings->beginGroup("distortionCorrectionPlot");
    m_DistortionCorrectionPlotSettings->writeSettings(settings);
    settings->endGroup();
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
