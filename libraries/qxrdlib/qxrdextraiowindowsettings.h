#ifndef QXRDEXTRAIOWINDOWSETTINGS_H
#define QXRDEXTRAIOWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"
#include "qxrdextrainputsplotwidgetsettings-ptr.h"
#include "qxrdextraoutputsplotwidgetsettings-ptr.h"

class QXRD_EXPORT QxrdExtraIOWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdExtraIOWindowSettings(QString name);

  QxrdMainWindowPtr newWindow();

  void writeSettings(QSettings *set);
  void readSettings(QSettings *set);

  QxrdExtraInputsPlotWidgetSettingsPtr  extraInputsPlotWidgetSettings();
  QxrdExtraOutputsPlotWidgetSettingsPtr extraOutputsPlotWidgetSettings();

private:
  QxrdExtraInputsPlotWidgetSettingsPtr  m_ExtraInputsPlotWidgetSettings;
  QxrdExtraOutputsPlotWidgetSettingsPtr m_ExtraOutputsPlotWidgetSettings;
};

Q_DECLARE_METATYPE(QxrdExtraIOWindowSettings*)

#endif // QXRDEXTRAIOWINDOWSETTINGS_H