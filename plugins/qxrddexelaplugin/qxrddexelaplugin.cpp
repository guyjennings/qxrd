#include "qxrddexelaplugin.h"
#include "qxrddetectorinterface.h"
#include "qxrddexeladriver.h"
#include "qxrddexelasettings.h"
#include "BusScanner.h"

QxrdDexelaPlugin::QxrdDexelaPlugin()
{
  m_BusScanner = new BusScanner();

  m_NDevs = m_BusScanner -> EnumerateDevices();

  for (int i=0; i<m_NDevs; i++) {
    DevInfo info = m_BusScanner -> GetDevice(i);
  }
}

QString QxrdDexelaPlugin::name() const
{
  return "Dexela Detector Plugin";
}

QxrdDetectorDriverPtr QxrdDexelaPlugin::createDetector(QString name,
                                                       QxrdDetectorSettingsWPtr det,
                                                       QxrdExperimentWPtr expt,
                                                       QxrdAcqCommonWPtr acq)
{
  QxrdDexelaSettingsWPtr set = qSharedPointerDynamicCast<QxrdDexelaSettings>(det);

  QxrdDetectorDriverPtr res =
      QxrdDetectorDriverPtr(
        new QxrdDexelaDriver(name, set, expt, acq));

  return res;
}
