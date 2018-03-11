#ifndef QXRDMASKINGWINDOWSETTINGS_H
#define QXRDMASKINGWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"
#include "qxrdfilebrowsersettings-ptr.h"
#include "qxrdimageplotwidgetsettings-ptr.h"

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
  QxrdImagePlotWidgetSettingsPtr imagePlotWidgetSettings();

private:
  QxrdFileBrowserSettingsPtr     m_FileBrowserSettings;
  QxrdImagePlotWidgetSettingsPtr m_ImagePlotWidgetSettings;
};

Q_DECLARE_METATYPE(QxrdMaskingWindowSettings*)

#endif // QXRDMASKINGWINDOWSETTINGS_H
