#ifndef QXRDWINDOWSETTINGS_H
#define QXRDWINDOWSETTINGS_H

#include "qcepserializableobject.h"
#include "qcepproperty.h"
#include "qxrdimageplotsettings-ptr.h"
#include "qxrdintegratorplotsettings-ptr.h"
#include "qxrdcenterfinderplotsettings-ptr.h"
#include "qxrdfilebrowsersettings-ptr.h"
#include "qxrdhistogramdialogsettings-ptr.h"
#include "qxrdinfodialogsettings-ptr.h"
#include "qxrdscriptdialogsettings-ptr.h"
#include "qxrdslicedialogsettings-ptr.h"
#include "qxrdsynchronizedacquisitiondialogsettings-ptr.h"
#include "qxrdacquisitionextrainputsdialogsettings-ptr.h"
#include "qxrddistortioncorrectiondialogsettings-ptr.h"
#include "qxrddistortioncorrectionplotsettings-ptr.h"
#include "qxrdwindowsettings-ptr.h"

class QxrdWindowSettings : public QcepSerializableObject
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdWindowSettings(QString name);
  static QxrdWindowSettingsPtr newWindowSettings();

  virtual void addChildPtr(QcepSerializableObjectPtr child);

  static void registerMetaTypes();

public:
  Q_PROPERTY(QByteArray windowGeometry READ get_WindowGeometry WRITE set_WindowGeometry)
  QCEP_BYTE_ARRAY_PROPERTY(WindowGeometry)

  Q_PROPERTY(QByteArray windowState READ get_WindowState WRITE set_WindowState)
  QCEP_BYTE_ARRAY_PROPERTY(WindowState)

public:
  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  QxrdImagePlotSettingsWPtr                    imagePlotSettings();
  QxrdCenterFinderPlotSettingsWPtr             centerFinderPlotSettings();
  QxrdIntegratorPlotSettingsWPtr               integratorPlotSettings();
  QxrdInputFileBrowserSettingsWPtr             inputFileBrowserSettings();
  QxrdOutputFileBrowserSettingsWPtr            outputFileBrowserSettings();
  QxrdHistogramDialogSettingsWPtr              histogramDialogSettings();
  QxrdSliceDialogSettingsWPtr                  sliceDialogSettings();
  QxrdInfoDialogSettingsWPtr                   infoDialogSettings();
  QxrdScriptDialogSettingsWPtr                 scriptDialogSettings();
  QxrdSynchronizedAcquisitionDialogSettingsWPtr synchronizedAcquisitionDialogSettings();
  QxrdAcquisitionExtraInputsDialogSettingsWPtr acquisitionExtraInputsDialogSettings();
  QxrdDistortionCorrectionDialogSettingsWPtr   distortionCorrectionDialogSettings();
  QxrdDistortionCorrectionPlotSettingsWPtr     distortionCorrectionPlotSettings();
signals:
  
public slots:
  
private:
  QMutex                                       m_Mutex;
  QxrdImagePlotSettingsPtr                     m_ImagePlotSettings;
  QxrdCenterFinderPlotSettingsPtr              m_CenterFinderPlotSettings;
  QxrdIntegratorPlotSettingsPtr                m_IntegratorPlotSettings;
  QxrdInputFileBrowserSettingsPtr              m_InputFileBrowserSettings;
  QxrdOutputFileBrowserSettingsPtr             m_OutputFileBrowserSettings;
  QxrdHistogramDialogSettingsPtr               m_HistogramDialogSettings;
  QxrdSliceDialogSettingsPtr                   m_SliceDialogSettings;
  QxrdInfoDialogSettingsPtr                    m_InfoDialogSettings;
  QxrdScriptDialogSettingsPtr                  m_ScriptDialogSettings;
  QxrdSynchronizedAcquisitionDialogSettingsPtr m_SynchronizedAcquisitionDialogSettings;
  QxrdAcquisitionExtraInputsDialogSettingsPtr  m_AcquisitionExtraInputsDialogSettings;
  QxrdDistortionCorrectionDialogSettingsPtr    m_DistortionCorrectionDialogSettings;
  QxrdDistortionCorrectionPlotSettingsPtr      m_DistortionCorrectionPlotSettings;
};

Q_DECLARE_METATYPE(QxrdWindowSettings*)

#endif // QXRDWINDOWSETTINGS_H
