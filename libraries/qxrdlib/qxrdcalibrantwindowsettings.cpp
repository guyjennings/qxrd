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
        new QxrdCalibrantWindow("Calibrant"),
        &QObject::deleteLater);

  return m_Window;
}
