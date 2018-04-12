#ifndef QXRDMASKINGWINDOWSETTINGS_H
#define QXRDMASKINGWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"
#include "qxrdfilebrowsersettings-ptr.h"
#include "qcepimageplotwidgetsettings-ptr.h"

class QXRD_EXPORT QxrdMaskingWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

private:
  typedef QxrdMainWindowSettings inherited;

public:
  Q_INVOKABLE QxrdMaskingWindowSettings(QString name);
  void initialize(QcepObjectWPtr parent);

  QcepMainWindowPtr newWindow();

  void writeSettings(QSettings *set);
  void readSettings(QSettings *set);

  QxrdFileBrowserSettingsPtr     fileBrowserSettings();
  QcepImagePlotWidgetSettingsPtr imagePlotWidgetSettings();

private:
  QxrdFileBrowserSettingsPtr     m_FileBrowserSettings;
  QcepImagePlotWidgetSettingsPtr m_ImagePlotWidgetSettings;
};

Q_DECLARE_METATYPE(QxrdMaskingWindowSettings*)

#endif // QXRDMASKINGWINDOWSETTINGS_H
