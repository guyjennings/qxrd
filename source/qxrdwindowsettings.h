#ifndef QXRDWINDOWSETTINGS_H
#define QXRDWINDOWSETTINGS_H

#include "qcepobject.h"
#include "qcepproperty.h"
#include "qxrdimageplotsettings.h"
#include "qxrdcenterfinderplotsettings.h"
#include "qxrdfilebrowsersettings.h"
#include "qxrdhistogramdialogsettings.h"
#include "qxrdinfodialogsettings.h"
#include "qxrdscriptdialogsettings.h"
#include "qxrdslicedialogsettings.h"
#include "qxrdsynchronizedacquisitiondialogsettings.h"
#include "qxrdacquisitionextrainputsdialogsettings.h"
#include "qxrddistortioncorrectiondialogsettings.h"
#include "qxrddistortioncorrectionplotsettings.h"

class QxrdWindowSettings : public QcepObject
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdWindowSettings(QString name);
  QxrdWindowSettings();

  virtual void addChildPtr(QcepObjectPtr child);

public:
  Q_PROPERTY(QByteArray windowGeometry READ get_WindowGeometry WRITE set_WindowGeometry)
  QCEP_BYTE_ARRAY_PROPERTY(WindowGeometry)

  Q_PROPERTY(QByteArray windowState READ get_WindowState WRITE set_WindowState)
  QCEP_BYTE_ARRAY_PROPERTY(WindowState)

public:
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

  QxrdImagePlotSettingsWPtr                    imagePlotSettings();
  QxrdCenterFinderPlotSettingsWPtr             centerFinderPlotSettings();
  QcepPlotSettingsWPtr                         integratorPlotSettings();
  QxrdFileBrowserSettingsWPtr                  inputFileBrowserSettings();
  QxrdFileBrowserSettingsWPtr                  outputFileBrowserSettings();
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
  bool checkBrowserPointer(QcepObjectPtr child);

private:
  QMutex                                       m_Mutex;
  QxrdImagePlotSettingsPtr                     m_ImagePlotSettings;
  QxrdCenterFinderPlotSettingsPtr              m_CenterFinderPlotSettings;
  QcepPlotSettingsPtr                          m_IntegratorPlotSettings;
  QxrdFileBrowserSettingsPtr                   m_InputFileBrowserSettings;
  QxrdFileBrowserSettingsPtr                   m_OutputFileBrowserSettings;
  QxrdHistogramDialogSettingsPtr               m_HistogramDialogSettings;
  QxrdSliceDialogSettingsPtr                   m_SliceDialogSettings;
  QxrdInfoDialogSettingsPtr                    m_InfoDialogSettings;
  QxrdScriptDialogSettingsPtr                  m_ScriptDialogSettings;
  QxrdSynchronizedAcquisitionDialogSettingsPtr m_SynchronizedAcquisitionDialogSettings;
  QxrdAcquisitionExtraInputsDialogSettingsPtr  m_AcquisitionExtraInputsDialogSettings;
  QxrdDistortionCorrectionDialogSettingsPtr    m_DistortionCorrectionDialogSettings;
  QxrdDistortionCorrectionPlotSettingsPtr      m_DistortionCorrectionPlotSettings;
};

typedef QSharedPointer<QxrdWindowSettings> QxrdWindowSettingsPtr;
typedef QWeakPointer<QxrdWindowSettings>   QxrdWindowSettingsWPtr;

Q_DECLARE_METATYPE(QxrdWindowSettings*)

#endif // QXRDWINDOWSETTINGS_H
