#include "qxrdmaskingwindowsettings.h"
#include "qxrdmaskingwindow.h"

QxrdMaskingWindowSettings::QxrdMaskingWindowSettings(QString name)
  : QxrdMainWindowSettings(name)
{

}

QxrdMainWindowPtr QxrdMaskingWindowSettings::newWindow()
{
  m_Window =
      QxrdMainWindowPtr(
        new QxrdMaskingWindow("Masking", m_Application, m_Experiment));

  return m_Window;
}
