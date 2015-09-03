#include "qxrddetectorpilatus.h"
#include "qxrddetectorthread.h"
#include "qcepproperty.h"
#include "qxrddebug.h"

QxrdDetectorPilatus::QxrdDetectorPilatus(QcepSettingsSaverWPtr saver, QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq) :
  QxrdDetector(saver, expt, acq, QxrdDetectorThread::PilatusDetector),
  m_PilatusSocket(),
  m_PilatusHost         (saver, this, "pilatusHost",          "s11id-pilatus", "Host Address of Computer running Camserver"),
  m_PilatusPort         (saver, this, "pilatusPort",          41234,         "Camserver Port Number"),
  m_PilatusFilePattern  (saver, this, "pilatusFilePattern",   "%s-%-0.5d%s", "File pattern for saved files"),
  m_PilatusDataDirectory(saver, this, "pilatusDataDirectory", "",            "Data directory on Camserver computer"),
  m_LocalDataDirectory  (saver, this, "localDataDirectory",   "",            "Data directory as seen on QXRD computer")
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetectorPilatus::QxrdDetectorPilatus(%p)\n", this);
  }
}

QxrdDetectorPilatus::~QxrdDetectorPilatus()
{
#ifndef QT_NO_DEBUG
  printf("Deleting Pilatus detector\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetectorPilatus::~QxrdDetectorPilatus(%p)\n", this);
  }
}

void QxrdDetectorPilatus::initialize()
{
  m_PilatusSocket.connectToHost(get_PilatusHost(), get_PilatusPort());
  m_PilatusSocket.waitForConnected();

  QString a = sendCommandReply("telemetry");

  printf("Read %s\n", qPrintable(a));
}

void QxrdDetectorPilatus::beginAcquisition()
{
}

void QxrdDetectorPilatus::endAcquisition()
{
}

void QxrdDetectorPilatus::shutdownAcquisition()
{
}

void QxrdDetectorPilatus::sendCommand(QString cmd)
{
  m_PilatusSocket.write(qPrintable(cmd+"\n"));
  m_PilatusSocket.waitForBytesWritten();
}

QString QxrdDetectorPilatus::sendCommandReply(QString cmd)
{
  m_PilatusSocket.write(qPrintable(cmd+"\n"));
  m_PilatusSocket.waitForBytesWritten();

  QString a;

  while (m_PilatusSocket.waitForReadyRead(10000)) {
    a += m_PilatusSocket.readAll();

    if (a.contains(QChar(24))) {
      QStringList sl = a.split(QChar(24));

      return sl.value(0);
    }
  }

  return a;
}
