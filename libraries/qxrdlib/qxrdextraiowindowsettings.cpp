#include "qxrdextraiowindowsettings.h"
#include "qxrdextraiowindow.h"

QxrdExtraIOWindowSettings::QxrdExtraIOWindowSettings(QString name)
  : QxrdMainWindowSettings(name)
{

}

QxrdMainWindowPtr QxrdExtraIOWindowSettings::newWindow()
{
  QxrdExtraIOWindowSettingsPtr myself = qSharedPointerDynamicCast<QxrdExtraIOWindowSettings>(sharedFromThis());

  m_Window =
      QxrdMainWindowPtr(
        new QxrdExtraIOWindow(myself, "Extra I/O", m_Application, m_Experiment));

  return m_Window;
}
