#include "qxrddetectorpilatus.h"
#include "qxrddetectorthread.h"
#include "qcepproperty.h"
#include "qxrddebug.h"

QxrdDetectorPilatus::QxrdDetectorPilatus(QcepSettingsSaverWPtr saver, QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq, QcepObject *parent) :
  QxrdDetector(saver, expt, acq, QxrdDetectorThread::PilatusDetector, parent),
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

    return reply();
  }
}

QString QxrdDetectorPilatus::reply()
{
  if (QThread::currentThread() != thread()) {
    QString res;

    QMetaObject::invokeMethod(this, "reply",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(QString, res));
    return res;
  } else {
    int pos = m_PilatusReply.indexOf(QChar(24));

    if (pos >= 0) {
      QString res = m_PilatusReply.left(pos);

      m_PilatusReply.remove(0, pos+1);

      return res;
    } else {
      while (m_PilatusSocket.waitForReadyRead(10000)) {
        QString seg = m_PilatusSocket.readAll();

        if (qcepDebug(DEBUG_PILATUS)) {
          printMessage(tr("Data segment size %1, data %2").arg(seg.count()).arg(seg));
        }

        m_PilatusReply += seg;

        pos = m_PilatusReply.indexOf(QChar(24));

        if (pos >= 0) {
          QString res = m_PilatusReply.left(pos);

          m_PilatusReply.remove(0, pos+1);

          if (qcepDebug(DEBUG_PILATUS)) {
            printMessage(tr("Split %1/%2").arg(res.count()).arg(m_PilatusReply.count()));
          }

          return res;
        }
      }

      return "";
    }
  }
}

void QxrdDetectorPilatus::expectReply(QString regexp)
{
  QRegExp exp(regexp);

  QString rply = reply();

  while (!exp.exactMatch(rply)) {
    printMessage(tr("%1 :Does not match: %2").arg(rply).arg(regexp));

    rply = reply();

    if (rply.length() == 0) {
      return;
    }
  }

  printMessage(tr("Matches: %1").arg(rply));
}

void QxrdDetectorPilatus::exposureTime(double exposure)
{
  sendCommand(tr("expt %1").arg(exposure));

  expectReply("15 OK Exposure time set to: (\\d*\\.\\d+) sec(.*)");
}

void QxrdDetectorPilatus::exposurePeriod(double period)
{
  sendCommand(tr("expp %1").arg(period));

  expectReply("15 OK Exposure period set to: (\\d*\\.\\d+) sec(.*)");
}

void QxrdDetectorPilatus::exposureDelay(double delay)
{
  sendCommand(tr("delay %1").arg(delay));

  expectReply("15 OK Delay time set to: (\\d*\\.\\d+) sec(.*)");
}

void QxrdDetectorPilatus::exposuresPerFrame(int nexp)
{
  sendCommand(tr("nexpf %1").arg(nexp));

  expectReply("15 OK Exposures per frame set to: (\\d*)(.*)");
}

void QxrdDetectorPilatus::exposureFrameCount(int nfram)
{
  sendCommand(tr("ni %1").arg(nfram));

  expectReply("15 OK N images set to: (\\d*)(.*)");
}

void QxrdDetectorPilatus::exposure(QString file)
{
  sendCommand(tr("exposure \"%1\"").arg(file));

  expectReply("15 OK  Starting (\\d*\\.\\d+) second background:(.*)");
}

void QxrdDetectorPilatus::extTrigger(QString file)
{
  sendCommand(tr("exttrigger \"%1\"").arg(file));

  expectReply("15 OK Starting externally triggered exposure\(s):(.*)");
}

void QxrdDetectorPilatus::extEnable(QString file)
{
  sendCommand(tr("extenable \"%1\"").arg(file));

  expectReply("15 OK Starting externally enabled exposure\(s):(.*)");
}

void QxrdDetectorPilatus::imagePath(QString path)
{
  sendCommand(tr("imgpath \"%1\"").arg(path));

  expectReply("10 OK (.*)");
}

void QxrdDetectorPilatus::acquireImage(QString fileName, double exposure)
{
  exposureTime(exposure);

  sendCommandReply(tr("exposure \"%1\"").arg(fileName));
}
