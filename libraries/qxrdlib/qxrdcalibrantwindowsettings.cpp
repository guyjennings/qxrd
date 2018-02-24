#include "qxrdcalibrantwindowsettings.h"
#include "qxrdcalibrantwindow.h"
#include <QThread>

QxrdCalibrantWindowSettings::QxrdCalibrantWindowSettings(QString name, QString desc)
  : inherited(name, desc)
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
        new QxrdCalibrantWindow("Calibrant"));

  return m_Window;
}
