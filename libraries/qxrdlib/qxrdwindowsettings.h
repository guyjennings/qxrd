#ifndef QXRDWINDOWSETTINGS_H
#define QXRDWINDOWSETTINGS_H

#include "qxrdlib_global.h"
#include "qxrdmainwindowsettings.h"
#include "qcepobject.h"
#include "qcepproperty.h"
#include "qxrdimageplotsettings-ptr.h"
#include "qxrdintegratorplotsettings-ptr.h"
#include "qxrdfilebrowsersettings-ptr.h"
#include "qxrdhistogramdialogsettings-ptr.h"
#include "qxrdwindowsettings-ptr.h"
#include "qxrdimageplotwidgetsettings-ptr.h"
#include "qxrdcenteringplotwidgetsettings-ptr.h"
#include "qxrdintegratedplotwidgetsettings-ptr.h"
#include "qxrddistortionplotwidgetsettings-ptr.h"

class QXRD_EXPORT QxrdWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

private:
  typedef QxrdMainWindowSettings inherited;

public:
  Q_INVOKABLE QxrdWindowSettings(QString name);

  void initialize(QcepObjectWPtr parent);

  static void registerMetaTypes();

  QcepMainWindowPtr newWindow();

public:
  Q_PROPERTY(QByteArray windowGeometry READ get_WindowGeometry WRITE set_WindowGeometry)
  QCEP_BYTE_ARRAY_PROPERTY(WindowGeometry)

  Q_PROPERTY(QByteArray windowState READ get_WindowState WRITE set_WindowState)
  QCEP_BYTE_ARRAY_PROPERTY(WindowState)

public:
  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  QxrdImagePlotSettingsWPtr                    imagePlotSettings();
  QxrdIntegratorPlotSettingsWPtr               integratorPlotSettings();
  QxrdFileBrowserSettingsWPtr                  fileBrowserSettings();
  QxrdHistogramDialogSettingsWPtr              histogramDialogSettings();

  QxrdImagePlotWidgetSettingsPtr               imagePlotWidgetSettings();
  QxrdCenteringPlotWidgetSettingsPtr           centeringPlotWidgetSettings();
  QxrdIntegratedPlotWidgetSettingsPtr          integratedPlotWidgetSettings();
  QxrdDistortionPlotWidgetSettingsPtr          distortionPlotWidgetSettings();

signals:
  
public slots:
  
private:
  QxrdImagePlotSettingsPtr                     m_ImagePlotSettings;
  QxrdIntegratorPlotSettingsPtr                m_IntegratorPlotSettings;
  QxrdFileBrowserSettingsPtr                   m_FileBrowserSettings;
  QxrdHistogramDialogSettingsPtr               m_HistogramDialogSettings;
  QxrdImagePlotWidgetSettingsPtr               m_ImagePlotWidgetSettings;
  QxrdCenteringPlotWidgetSettingsPtr           m_CenteringPlotWidgetSettings;
  QxrdIntegratedPlotWidgetSettingsPtr          m_IntegratedPlotWidgetSettings;
  QxrdDistortionPlotWidgetSettingsPtr          m_DistortionPlotWidgetSettings;
};

Q_DECLARE_METATYPE(QxrdWindowSettings*)

#endif // QXRDWINDOWSETTINGS_H
