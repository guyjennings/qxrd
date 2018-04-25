#include "qxrdcalibrantwindowsettings.h"
#include "qxrdcalibrantwindow.h"
#include <QThread>

QxrdCalibrantWindowSettings::QxrdCalibrantWindowSettings(QString name)
  : inherited(name, "Calibrant Window")
{
}

void QxrdCalibrantWindowSettings::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
}

QcepMainWindowPtr QxrdCalibrantWindowSettings::newWindow()
{
  GUI_THREAD_CHECK;

  m_Window =
      QxrdMainWindowPtr(
        NEWPTR(QxrdCalibrantWindow("Calibrant")));

  return m_Window;
}
