#ifndef QXRDWINDOWSETTINGS_H
#define QXRDWINDOWSETTINGS_H

#include <QObject>
#include "qcepproperty.h"
#include "qxrdimageplotsettings.h"
#include "qxrdcenterfinderplotsettings.h"
#include "qxrdfilebrowsersettings.h"
#include "qxrdhistogramdialogsettings.h"
#include "qxrdinfodialogsettings.h"
#include "qxrdslicedialogsettings.h"
#include "qxrdacquisitionextrainputsdialogsettings.h"

class QxrdWindowSettings : public QObject
{
  Q_OBJECT
public:
  explicit QxrdWindowSettings(QxrdSettingsSaverWPtr saver, QObject *parent);
  
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
  QxrdPlotSettingsWPtr                         integratorPlotSettings();
  QxrdFileBrowserSettingsWPtr                  inputFileBrowserSettings();
  QxrdFileBrowserSettingsWPtr                  outputFileBrowserSettings();
  QxrdHistogramDialogSettingsWPtr              histogramDialogSettings();
  QxrdSliceDialogSettingsWPtr                  sliceDialogSettings();
  QxrdInfoDialogSettingsWPtr                   infoDialogSettings();
  QxrdAcquisitionExtraInputsDialogSettingsWPtr acquisitionExtraInputsDialogSettings();
signals:
  
public slots:
  
private:
  QxrdImagePlotSettingsPtr                     m_ImagePlotSettings;
  QxrdCenterFinderPlotSettingsPtr              m_CenterFinderPlotSettings;
  QxrdPlotSettingsPtr                          m_IntegratorPlotSettings;
  QxrdFileBrowserSettingsPtr                   m_InputFileBrowserSettings;
  QxrdFileBrowserSettingsPtr                   m_OutputFileBrowserSettings;
  QxrdHistogramDialogSettingsPtr               m_HistogramDialogSettings;
  QxrdSliceDialogSettingsPtr                   m_SliceDialogSettings;
  QxrdInfoDialogSettingsPtr                    m_InfoDialogSettings;
  QxrdAcquisitionExtraInputsDialogSettingsPtr  m_AcquisitionExtraInputsDialogSettings;
};

typedef QSharedPointer<QxrdWindowSettings> QxrdWindowSettingsPtr;
typedef QWeakPointer<QxrdWindowSettings>   QxrdWindowSettingsWPtr;

#endif // QXRDWINDOWSETTINGS_H
