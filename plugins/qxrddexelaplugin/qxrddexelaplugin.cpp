#include "qxrddexelaplugin.h"
#include "qxrddetectorinterface.h"
#include "qxrddexeladriver.h"
#include "qxrddexelasettings.h"
#include "BusScanner.h"
#include "Dexeladetector.h"
#include "qxrddexelaplugin-ptr.h"

QxrdDexelaPlugin::QxrdDexelaPlugin()
  : inherited("dexelaPlugin"),
    m_BusScanner(NULL),
    m_NDevs(0)
{
}

void QxrdDexelaPlugin::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  printMessage("QxrdDexelaPlugin::initialize");

  m_BusScanner = new BusScanner();

  m_NDevs = m_BusScanner -> EnumerateDevices();

  printMessage(tr("Found %1 Dexela Devices").arg(m_NDevs));

  for (int i=0; i<m_NDevs; i++) {
    DevInfo info = m_BusScanner -> GetDevice(i);

    printMessage(tr("Found Dexela Device %1, Model %2, Serial %3")
                 .arg(i).arg(info.model).arg(info.serialNum));

    printMessage(tr("  unit = %1, param = ""%2""").arg(info.unit).arg(info.param));
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
  QxrdDexelaPluginPtr plugin(
        qSharedPointerDynamicCast<QxrdDexelaPlugin>(sharedFromThis()));

  QxrdDexelaSettingsPtr set(
        qSharedPointerDynamicCast<QxrdDexelaSettings>(det));

  QxrdDetectorDriverPtr res =
      QxrdDetectorDriverPtr(
        new QxrdDexelaDriver(name, plugin, set, expt, acq));

  return res;
}

int QxrdDexelaPlugin::deviceCount()
{
  return m_NDevs;
}

DevInfo QxrdDexelaPlugin::device(int n)
{
  if (n >= 0 && n < m_NDevs) {
    return m_BusScanner -> GetDevice(n);
  } else {
    return DevInfo();
  }
}
