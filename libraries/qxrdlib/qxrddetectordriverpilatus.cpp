#include "qxrddetectordriverpilatus.h"
#include "qxrdthread.h"
#include "qcepmacros.h"
#include "qxrddetectorsettingspilatus.h"
#include "qxrddebug.h"
#include "qxrdacquisition.h"
#include "qxrddetectorpilatusremote.h"
#include <QProcess>
#include "qxrdexperiment.h"
#include "qxrddataprocessor.h"
#include "qcepallocator.h"

QxrdDetectorDriverPilatus::QxrdDetectorDriverPilatus(
    QString name,
    QxrdDetectorSettingsPilatusPtr pilatus,
    QxrdExperimentPtr expt,
    QxrdAcquisitionPtr acq) :
  QxrdDetectorDriver(name, pilatus, expt, acq),
  m_Pilatus(pilatus),
  m_PilatusSocket(),
  m_ExposureTime(-1),
  m_ExposuresPerFrame(-1),
  m_ExposureFrameCount(-1),
  m_Remote(new QxrdDetectorPilatusRemote())
{
#ifndef QT_NO_DEBUG
  printf("Pilatus Detector Driver \"%s\" Constructed\n", qPrintable(name));
#endif
}

QxrdDetectorDriverPilatus::~QxrdDetectorDriverPilatus()
{
#ifndef QT_NO_DEBUG
  printf("Pilatus Detector Driver \"%s\" Destroyed\n", qPrintable(get_Name()));
#endif
}

bool QxrdDetectorDriverPilatus::startDetectorDriver()
{
  THREAD_CHECK;

  QxrdDetectorSettingsPilatusPtr pil(m_Pilatus);

  if (pil) {
    printMessage(tr("Starting Pilatus Detector at %1:%2")
                 .arg(pil->get_PilatusHost())
                 .arg(pil->get_PilatusPort()));

    m_PilatusSocket.connectToHost(pil->get_PilatusHost(), pil->get_PilatusPort());
    m_PilatusSocket.waitForConnected();

    if (qcepDebug(DEBUG_PILATUS)) {
      printMessage("Connected to pilatus...");
    }

    connect(&m_PilatusSocket, &QTcpSocket::readyRead,
            this, &QxrdDetectorDriverPilatus::readyRead);

    sendCommand("telemetry");
    sendCommand("nimages 1");
    sendCommand("nexpframe 1");

    imagePath(pil->get_PilatusDataDirectory());
  }

  return true;
}

bool QxrdDetectorDriverPilatus::stopDetectorDriver()
{
  THREAD_CHECK;

  QxrdDetectorSettingsPilatusPtr pil(m_Pilatus);

  if (pil) {
    printMessage(tr("Stopping Pilatus Detector at %1").arg(
                   pil->get_PilatusHost()));

    m_PilatusSocket.close();
  }

  return true;
}

bool QxrdDetectorDriverPilatus::changeExposureTime(double /*exposure*/)
{
  THREAD_CHECK;

  return true;
}

bool QxrdDetectorDriverPilatus::beginAcquisition(double exposure)
{
  THREAD_CHECK;

  QxrdDetectorSettingsPilatusPtr pil(m_Pilatus);

  if (pil) {
    if (qcepDebug(DEBUG_PILATUS)) {
      printMessage(tr("QxrdDetectorDriverPilatus::beginAcquisition(%1)").arg(exposure));
    }

    imagePath(pil->get_PilatusDataDirectory());

    beginExposure(exposure);
  }

  return true;
}

bool QxrdDetectorDriverPilatus::endAcquisition()
{
  return true;
}

bool QxrdDetectorDriverPilatus::shutdownAcquisition()
{
  return true;
}

void QxrdDetectorDriverPilatus::executeCommand(QString cmd)
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "executeCommand", Qt::BlockingQueuedConnection, Q_ARG(QString, cmd));
  } else {
    sendCommand(cmd);
  }
}

void QxrdDetectorDriverPilatus::readyRead()
{
  if (qcepDebug(DEBUG_PILATUS)) {
    printMessage("QxrdDetectorDriverPilatus::readyRead");

    printMessage(tr("%1 bytes available").arg(m_PilatusSocket.bytesAvailable()));
  }

  m_Buffer.append(m_PilatusSocket.readAll());

  while(1) {
    int ind1 = m_Buffer.indexOf('\n');
    int ind2 = m_Buffer.indexOf(char(24));

    int ind = ind1;

    if (ind1 >= 0 && ind2 >= 0) {
      if (ind1 < ind2) {
        ind = ind1;
      } else {
        ind = ind2;
      }
    } else if (ind1 >= 0) {
      ind = ind1;
    } else {
      ind = ind2;
    }

    if (ind >= 0) {
      QString line = m_Buffer.left(ind); // Don't include newline
      m_Buffer.remove(0, ind+1);

      if (qcepDebug(DEBUG_PILATUS)) {
        printMessage(tr("(%1) : \"%2\"").arg(line.count()).arg(QString(line)));
      }

      interpretReply(line);
    } else { // No new lines when expected
      return;
    }
  }
}

void QxrdDetectorDriverPilatus::sendCommand(QString cmd)
{
  THREAD_CHECK;

  if (qcepDebug(DEBUG_PILATUS)) {
    printMessage(tr("QxrdDetectorDriverPilatus::sendCommand(\"%1\")").arg(cmd));
  }

  m_PilatusSocket.write(qPrintable(cmd+"\n"));
  m_PilatusSocket.waitForBytesWritten();
}

void QxrdDetectorDriverPilatus::imagePath(QString path)
{
  QxrdDetectorSettingsPilatusPtr pil(m_Pilatus);

  if (pil) {
    if (pil->checkDetectorEnabled()) {
      sendCommand(tr("imgpath \"%1\"").arg(path));
    }
  }
}

void QxrdDetectorDriverPilatus::beginExposure(double exposure)
{
  QxrdDetectorSettingsPilatusPtr pil(m_Pilatus);

  if (pil) {
    int expMode = pil->get_ExposureMode();

    if (expMode == NoExternalTrigger) {
      sendCommand(tr("ExpTime %1").arg(exposure));
      sendCommand("nexpframe 1");
    } else if (expMode == ExternalTrigger) {
      sendCommand("Delay 0");
      sendCommand(tr("ExpPeriod %1").arg(exposure));
      sendCommand(tr("ExpTime %1").arg(exposure));
      sendCommand("nexpframe 1");
    } else if (expMode == ExternalEnable) {
      int nFrames = (int) (pil->get_EnableFrequency()*exposure);
      sendCommand(tr("nexpframe %1").arg(nFrames));
    }
  }
}

void QxrdDetectorDriverPilatus::expose()
{
  QxrdDetectorSettingsPilatusPtr pil(m_Pilatus);
  QxrdAcquisitionPtr             acq(m_Acquisition);

  if (pil && acq) {
    m_CurrentFile = acq->currentFileBase(pil->get_DetectorNumber(),
                                         pil->get_PilatusExtension());

    if (pil->get_ReadFilesLocally()) { // Check to see if file exists...
      if (pil->get_DeleteFilesAfterReading()) {
        // Attempt to delete file before acquisition...
      }
    }

    int expMode = pil->get_ExposureMode();

    if (expMode == NoExternalTrigger) {
      sendCommand(tr("Exposure \"%1\"").arg(m_CurrentFile));
    } else if (expMode == ExternalTrigger) {
      sendCommand(tr("ExtTrigger \"%1\"").arg(m_CurrentFile));
    } else if (expMode == ExternalEnable) {
      sendCommand(tr("ExtEnable \"%1\"").arg(m_CurrentFile));
    }
  }
}

void QxrdDetectorDriverPilatus::beginFrame()
{
  if (qcepDebug(DEBUG_PILATUS)) {
    printMessage("QxrdDetectorPilatus::beginFrame");
  }

  expose();
}

void QxrdDetectorDriverPilatus::interpretReply(QString reply)
{
  QxrdDetectorSettingsPilatusPtr pil(m_Pilatus);

  if (pil) {
    if (qcepDebug(DEBUG_PILATUS)) {
      printMessage(tr("QxrdDetectorPilatus::interpretReply(\"%1\")").arg(reply));
    }

    if (reply.startsWith("1 ERR")) {
      criticalMessage(tr("Error with pilatus detector: %1").arg(reply));
    } else if (reply.startsWith("7 OK")) { // Image has been saved...
      if (pil->get_ReadFilesLocally() == false) {
        pil->enqueueAcquiredFrame(QcepUInt16ImageDataPtr());
      } else {
        remoteCopy(m_CurrentFile);
        //      pushFileExpected(m_CurrentFile);
        loadAndPush(m_CurrentFile);

        if (pil->get_DeleteFilesAfterReading()) {
          remoteDelete(m_CurrentFile);
        }
      }
    } else if (reply.startsWith("Image format:")) {
      QRegExp matcher("Image format\\: (\\d+)\\(w\\) x (\\d+)\\(h\\) pixels(.*)");

      if (matcher.exactMatch(reply)) {
        int width = matcher.cap(1).toInt();
        int height = matcher.cap(2).toInt();

        printMessage(tr("Detector dimensions %1 cols x %2 rows").arg(width).arg(height));

        pil->set_NRows(height);
        pil->set_NCols(width);
        pil->set_HBinning(1);
        pil->set_VBinning(1);
      } else {
        printMessage(tr("Unmatched: %1, %2").arg(reply).arg(matcher.pattern()));
        printMessage(tr("Matched Length %1").arg(matcher.matchedLength()));
      }
    }
  }
}

void QxrdDetectorDriverPilatus::remoteConnect(QString sshCmd)
{
  if (m_Remote) {
    m_Remote->connectToRemote(sshCmd);
  }
}

void QxrdDetectorDriverPilatus::remoteCommand(QString cmd)
{
  if (m_Remote) {
    m_Remote->executeRemote(cmd);
  }
}

void QxrdDetectorDriverPilatus::remoteDelete(QString file)
{
  QxrdDetectorSettingsPilatusPtr pil(m_Pilatus);

  if (pil) {
    QString cmd = tr("%1 -o ForwardX11=No %2@%3 rm %4/%5")
        .arg(pil->get_PilatusSSH())
        .arg(pil->get_PilatusUser())
        .arg(pil->get_PilatusHost())
        .arg(pil->get_PilatusDataDirectory()).arg(file);

    if (qcepDebug(DEBUG_PILATUS)) {
      printMessage(tr("Deleting: %1").arg(cmd));
    }

    int rc = QProcess::execute(cmd);

    if (rc) {
      printMessage(tr("rc = %1").arg(rc));
    }
  }
}

void QxrdDetectorDriverPilatus::remoteCopy(QString file)
{
  QxrdDetectorSettingsPilatusPtr pil(m_Pilatus);
  QxrdExperimentPtr              expt(m_Experiment);

  if (pil && expt) {
    QxrdDataProcessorPtr proc = expt->dataProcessor();

    if (proc) {
      QString dest = proc->filePathInRawOutputDirectory(file);

      QString cmd = tr("%1 -o ForwardX11=No %2@%3:%4/%5 %6")
          .arg(pil->get_PilatusSCP())
          .arg(pil->get_PilatusUser())
          .arg(pil->get_PilatusHost())
          .arg(pil->get_PilatusDataDirectory()).arg(file)
          .arg(dest);

      if (qcepDebug(DEBUG_PILATUS)) {
        printMessage(cmd);
      }

      int rc = QProcess::execute(cmd);

      if (rc) {
        printMessage(tr("rc = %1").arg(rc));
      }
    }
  }
}

void QxrdDetectorDriverPilatus::remoteTransfer(QString file)
{
  // Transfer contents of a remote file to a local memory buffer:"

  QxrdDetectorSettingsPilatusPtr pil(m_Pilatus);
  QxrdExperimentPtr              expt(m_Experiment);

  if (pil && expt) {
    QxrdDataProcessorPtr proc = expt->dataProcessor();

    if (proc) {
      QString dest = proc->filePathInRawOutputDirectory(file);

      remoteCommand(tr("echo -n transfer: ; ls -la %1").arg(file));
      remoteCommand(tr("cat %1").arg(file));
    }
  }
}

void QxrdDetectorDriverPilatus::loadAndPush(QString f)
{
  QxrdDetectorSettingsPilatusPtr pil(m_Pilatus);
  QxrdExperimentPtr              expt(m_Experiment);

  if (pil && expt) {
    QxrdDataProcessorPtr proc = expt->dataProcessor();

    if (proc) {
      QString dest = proc->filePathInRawOutputDirectory(f);

      QcepUInt32ImageDataPtr data = QcepAllocator::newInt32Image("pilatus", 0,0, QcepAllocator::AllocateFromReserve);

      if (data->readImage(dest)) {
        printMessage(tr("Read %1 successfully").arg(dest));
      }

      pil->enqueueAcquiredFrame(data);
    }
  }
}

