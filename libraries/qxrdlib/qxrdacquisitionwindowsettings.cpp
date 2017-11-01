#include "qxrdacquisitionwindowsettings.h"
#include "qxrdacquisitionwindow.h"

QxrdAcquisitionWindowSettings::QxrdAcquisitionWindowSettings(QString name)
  : QxrdMainWindowSettings(name)
{
}

QxrdMainWindowPtr QxrdAcquisitionWindowSettings::newWindow()
{
  m_Window =
      QxrdMainWindowPtr(
        new QxrdAcquisitionWindow("Acquisition", m_Application, m_Experiment));

  return m_Window;
}
