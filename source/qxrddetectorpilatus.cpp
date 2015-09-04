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
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "initialize");
  } else {
    m_PilatusSocket.connectToHost(get_PilatusHost(), get_PilatusPort());
    m_PilatusSocket.waitForConnected();

    QString a = sendCommandReply("telemetry");

    printf("Read %s\n", qPrintable(a));
  }
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
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "sendCommand", Q_ARG(QString, cmd));
  } else {
    m_PilatusSocket.write(qPrintable(cmd+"\n"));
    m_PilatusSocket.waitForBytesWritten();
  }
}

QString QxrdDetectorPilatus::sendCommandReply(QString cmd)
{
  if (QThread::currentThread() != thread()) {
    QString res;

    QMetaObject::invokeMethod(this, "sendCommandReply",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(QString,res),
                              Q_ARG(QString, cmd));

    return res;
  } else {
    m_PilatusSocket.write(qPrintable(cmd+"\n"));
    m_PilatusSocket.waitForBytesWritten();

    QString a;

    while (m_PilatusSocket.waitForReadyRead(10000)) {
      QString seg = m_PilatusSocket.readAll();

      if (qcepDebug(DEBUG_PILATUS)) {
        printMessage(tr("Data segment size %1, data %2").arg(seg.count()).arg(seg));
      }

      a += seg;

      if (a.contains(QChar(24))) {
        QStringList sl = a.split(QChar(24));

        if (qcepDebug(DEBUG_PILATUS)) {
          printMessage(tr("Split %1/%2").arg(sl.value(0).count()).arg(sl.value(1).count()));
        }

        return sl.value(0);
      }
    }

    return a;
  }
}
