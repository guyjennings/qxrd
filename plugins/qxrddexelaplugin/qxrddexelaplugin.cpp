#include "qxrddexelaplugin.h"
#include "qxrddetectorinterface.h"
#include "qxrddexeladriver.h"
#include "qxrddexelasettings.h"
#include "BusScanner.h"
#include "Dexeladetector.h"

QxrdDexelaPlugin::QxrdDexelaPlugin()
  : inherited("dexelaPlugin")
{
//  m_BusScanner = new BusScanner();

//  m_NDevs = m_BusScanner -> EnumerateDevices();

//  printf("Found %d Dexela Devices\n", m_NDevs);

//  for (int i=0; i<m_NDevs; i++) {
//    DevInfo info = m_BusScanner -> GetDevice(i);

//    printf("Found Dexela Device %d, Model %d, Serial %d\n",
//           i, info.model, info.serialNum);

//    printf("  unit = %d, param = ""%s""\n", info.unit, info.param);
//  }
}

void QxrdDexelaPlugin::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  printMessage("QxrdDexelaPlugin::initialize");
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
