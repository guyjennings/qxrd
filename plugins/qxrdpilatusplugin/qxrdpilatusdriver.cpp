#include "qxrddebug.h"
#include "qxrdpilatusdriver.h"
#include "qxrdacqcommon.h"
#include "qxrdexperiment.h"
#include "qxrdsynchronizedacquisition.h"
#include "qcepallocator.h"
#include <QPainter>
#include "qxrdpilatussettings.h"
#include "qxrdpilatusremote.h"
#include "qxrdprocessor.h"
#include <QThread>
#include "qcepimagedata.h"

QxrdPilatusDriver::QxrdPilatusDriver(QString name,
                                     QxrdPilatusPluginWPtr plugin,
                                     QxrdPilatusSettingsWPtr det,
                                     QxrdExperimentWPtr expt,
                                     QxrdAcqCommonWPtr acq)
  : QxrdDetectorDriver(name, det, expt, acq),
    m_Plugin(plugin),
    m_Pilatus(det),
    m_PilatusSocket(),
    m_ExposureTime(-1),
    m_ExposuresPerFrame(-1),
    m_ExposureFrameCount(-1)
{
#ifndef QT_NO_DEBUG
  printf("Pilatus Detector Driver \"%s\" Constructed\n", qPrintable(name));
#endif
}

QxrdPilatusDriver::~QxrdPilatusDriver()
{
#ifndef QT_NO_DEBUG
  printf("Pilatus Detector Driver \"%s\" Destroyed\n", qPrintable(get_Name()));
#endif
}

void QxrdPilatusDriver::startDetectorDriver()
{
  THREAD_CHECK;

  QxrdPilatusSettingsPtr det(m_Pilatus);

  if (det && det->checkDetectorEnabled()) {
    printMessage(tr("Starting Pilatus Detector at %1:%2")
                 .arg(det->get_PilatusHost())
                 .arg(det->get_PilatusPort()));

    m_PilatusSocket.connectToHost(det->get_PilatusHost(), det->get_PilatusPort());
    m_PilatusSocket.waitForConnected();

    if (qcepDebug(DEBUG_PILATUS)) {
      printMessage("Connected to pilatus...");
    }

    connect(&m_PilatusSocket, &QTcpSocket::readyRead,
            this, &QxrdPilatusDriver::readyRead);

    sendCommand("telemetry");
    sendCommand("nimages 1");
    sendCommand("nexpframe 1");

    imagePath(det->get_PilatusDataDirectory());
  }
}

void QxrdPilatusDriver::stopDetectorDriver()
{
  THREAD_CHECK;

  QxrdPilatusSettingsPtr det(m_Pilatus);

  if (det && det->checkDetectorEnabled()) {
    printMessage(tr("Stopping Pilatus Detector at %1").arg(
                   det->get_PilatusHost()));

    m_PilatusSocket.close();
  }
}

void QxrdPilatusDriver::changeExposureTime(double expos)
{
  THREAD_CHECK;

//  QxrdDetectorSettingsPtr det(m_Detector);

//  if (det && det->isEnabled()) {
//  m_ExposureFactor = det->get_ExposureFactor();
//  m_ExposureTime   = expos/m_ExposureFactor;
//    printMessage(tr("Exposure time changed to %1").arg(expos));

//    m_Timer.start(m_ExposureTime*1000);

//    return true;
//  }

//  return false;
}

void QxrdPilatusDriver::beginAcquisition(double exposure)
{
  THREAD_CHECK;

  QxrdPilatusSettingsPtr det(m_Pilatus);

  if (det && det->checkDetectorEnabled()) {
    if (qcepDebug(DEBUG_PILATUS)) {
      printMessage(tr("QxrdPilatusDriver::beginAcquisition(%1)").arg(exposure));
    }

    imagePath(det->get_PilatusDataDirectory());

    beginExposure(exposure);
  }

  m_FrameCounter = 0;
}

void QxrdPilatusDriver::endAcquisition()
{
  THREAD_CHECK;
}

void QxrdPilatusDriver::shutdownAcquisition()
{
  THREAD_CHECK;

//  m_Timer.stop();
}

//void QxrdPilatusDriver::onTimerTimeout()
//{
//  QxrdDetectorSettingsPtr det(m_Detector);
//  QxrdAcquisitionPtr      acq(m_Acquisition);

//  if (acq && det && det->checkDetectorEnabled()) {
//    QxrdSynchronizedAcquisitionPtr sacq(acq->synchronizedAcquisition());

//    if (sacq) {
//      sacq->acquiredFrameAvailable(m_FrameCounter);
//    }

//    int nRows = det -> get_NRows();
//    int nCols = det -> get_NCols();

//    int xpmsec = (int)(acq->get_ExposureTime()*1000+0.5);
//    int frame = m_FrameCounter % 8;

//    QcepUInt16ImageDataPtr image = QcepAllocator::newInt16Image(tr("simdet-%1").arg(frame),
//                                                                nCols, nRows,
//                                                                QcepAllocator::AllocateFromReserve);


//    if (image) {
//      quint16 *ptr = image->data();

//      for (int j=0; j<nRows; j++) {
//        for (int i=0; i<nCols; i++) {
//          if ((i>=frame*64) && (i<(frame+1)*64) && (j < 64)) {
//            *ptr++ = frame;
//          } else {
//            *ptr++ = xpmsec;
//          }
//        }
//      }

//      if ((nRows > 1024) && (nCols > 1024)) {
//        const int labelWidth = 256;
//        const int labelHeight = 64;

//        QImage imageLabel(labelWidth, labelHeight, QImage::Format_RGB32);
//        QPainter painter(&imageLabel);

//        painter.fillRect(0,0,labelWidth,labelHeight, Qt::black);
//        painter.setPen(Qt::white);
//        painter.setFont(QFont("Times", labelHeight, QFont::Bold, true));
//        painter.drawText(0, labelHeight, tr("%1").arg(m_FrameCounter));

//        QRgb    *rgb = (QRgb*) imageLabel.bits();
//        int nFrames = nRows / labelHeight;
//        int frameN = m_FrameCounter % nFrames;
//        int plval = qGray(*rgb);
//        int pRgb  = *rgb;

//        for (int j=0; j<labelHeight; j++) {
//          for (int i=0; i<labelWidth; i++) {
//            int x = nCols-labelWidth+i;
//            int y = (frameN+1)*labelHeight-j;
//            //          int val = image->value(x,y);
//            int vRgb = *rgb++;
//            int lval = qGray(vRgb);

//            if (lval != plval) {
//              plval = lval;
//            }

//            if (vRgb != pRgb) {
//              pRgb = vRgb;
//            }

//            image->setValue(x,y,lval);
//          }
//        }
//      }
//    }

//    det->enqueueAcquiredFrame(image);

//    m_FrameCounter++;
//  }
//}

void QxrdPilatusDriver::executeCommand(QString cmd)
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "executeCommand", Qt::BlockingQueuedConnection, Q_ARG(QString, cmd));
  } else {
    sendCommand(cmd);
  }
}

void QxrdPilatusDriver::readyRead()
{
  if (qcepDebug(DEBUG_PILATUS)) {
    printMessage("QxrdPilatusDriver::readyRead");

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

void QxrdPilatusDriver::sendCommand(QString cmd)
{
  THREAD_CHECK;

  if (qcepDebug(DEBUG_PILATUS)) {
    printMessage(tr("QxrdPilatusDriver::sendCommand(\"%1\")").arg(cmd));
  }

  m_PilatusSocket.write(qPrintable(cmd+"\n"));
  m_PilatusSocket.waitForBytesWritten();
}

void QxrdPilatusDriver::imagePath(QString path)
{
  QxrdPilatusSettingsPtr det(m_Pilatus);

  if (det && det->checkDetectorEnabled()) {
    sendCommand(tr("imgpath \"%1\"").arg(path));
  }
}

void QxrdPilatusDriver::beginExposure(double exposure)
{
  QxrdPilatusSettingsPtr det(m_Pilatus);

  if (det && det->checkDetectorEnabled()) {
    int expMode = det->get_ExposureMode();

    if (expMode == NoExternalTrigger) {
      sendCommand(tr("ExpTime %1").arg(exposure));
      sendCommand("nexpframe 1");
    } else if (expMode == ExternalTrigger) {
      sendCommand("Delay 0");
      sendCommand(tr("ExpPeriod %1").arg(exposure));
      sendCommand(tr("ExpTime %1").arg(exposure));
      sendCommand("nexpframe 1");
    } else if (expMode == ExternalEnable) {
      int nFrames = (int) (det->get_EnableFrequency()*exposure);
      sendCommand(tr("nexpframe %1").arg(nFrames));
    }
  }
}

void QxrdPilatusDriver::expose()
{
  QxrdPilatusSettingsPtr det(m_Pilatus);
  QxrdAcqCommonPtr       acq(m_Acquisition);

  if (acq && det && det->checkDetectorEnabled()) {
    m_CurrentFile = acq->currentFileBase(det->get_DetectorNumber(),
                                         det->get_PilatusExtension());

    if (det->get_ReadFilesLocally()) { // Check to see if file exists...
      if (det->get_DeleteFilesAfterReading()) {
        //TODO: Attempt to delete file before acquisition...
      }
    }

    int expMode = det->get_ExposureMode();

    if (expMode == NoExternalTrigger) {
      sendCommand(tr("Exposure \"%1\"").arg(m_CurrentFile));
    } else if (expMode == ExternalTrigger) {
      sendCommand(tr("ExtTrigger \"%1\"").arg(m_CurrentFile));
    } else if (expMode == ExternalEnable) {
      sendCommand(tr("ExtEnable \"%1\"").arg(m_CurrentFile));
    }
  }
}

void QxrdPilatusDriver::beginFrame()
{
  if (qcepDebug(DEBUG_PILATUS)) {
    printMessage("QxrdDetectorPilatus::beginFrame");
  }

  expose();
}

void QxrdPilatusDriver::interpretReply(QString reply)
{
  QxrdPilatusSettingsPtr det(m_Pilatus);

  if (det && det->checkDetectorEnabled()) {
    if (qcepDebug(DEBUG_PILATUS)) {
      printMessage(tr("QxrdPilatusDriver::interpretReply(\"%1\")").arg(reply));
    }

    if (reply.startsWith("1 ERR")) {
      criticalMessage(tr("Error with pilatus detector: %1").arg(reply));
    } else if (reply.startsWith("7 OK")) { // Image has been saved...
      if (det->get_ReadFilesLocally() == false) {
        det->enqueueAcquiredFrame(QcepUInt16ImageDataPtr());
      } else {
        remoteCopy(m_CurrentFile);
        //      pushFileExpected(m_CurrentFile);
        loadAndPush(m_CurrentFile);

        if (det->get_DeleteFilesAfterReading()) {
          remoteDelete(m_CurrentFile);
        }
      }
    } else if (reply.startsWith("Image format:")) {
      QRegExp matcher("Image format\\: (\\d+)\\(w\\) x (\\d+)\\(h\\) pixels(.*)");

      if (matcher.exactMatch(reply)) {
        int width = matcher.cap(1).toInt();
        int height = matcher.cap(2).toInt();

        printMessage(tr("Detector dimensions %1 cols x %2 rows").arg(width).arg(height));

        det->set_NRows(height);
        det->set_NCols(width);
        det->set_HBinning(1);
        det->set_VBinning(1);
      } else {
        printMessage(tr("Unmatched: %1, %2").arg(reply).arg(matcher.pattern()));
        printMessage(tr("Matched Length %1").arg(matcher.matchedLength()));
      }
    }
  }
}

void QxrdPilatusDriver::remoteConnect(QString sshCmd)
{
  if (m_Remote) {
    m_Remote->connectToRemote(sshCmd);
  }
}

void QxrdPilatusDriver::remoteCommand(QString cmd)
{
  if (m_Remote) {
    m_Remote->executeRemote(cmd);
  }
}

void QxrdPilatusDriver::remoteDelete(QString file)
{
  QxrdPilatusSettingsPtr det(m_Pilatus);

  if (det && det->checkDetectorEnabled()) {
    QString cmd = tr("%1 -o ForwardX11=No %2@%3 rm %4/%5")
        .arg(det->get_PilatusSSH())
        .arg(det->get_PilatusUser())
        .arg(det->get_PilatusHost())
        .arg(det->get_PilatusDataDirectory()).arg(file);

    if (qcepDebug(DEBUG_PILATUS)) {
      printMessage(tr("Deleting: %1").arg(cmd));
    }

    int rc = QProcess::execute(cmd);

    if (rc) {
      printMessage(tr("rc = %1").arg(rc));
    }
  }
}

void QxrdPilatusDriver::remoteCopy(QString file)
{
  QxrdPilatusSettingsPtr det(m_Pilatus);
  QxrdExperimentPtr      expt(m_Experiment);

  if (det && expt && det->checkDetectorEnabled()) {
    //TODO: is this the right one...
    QxrdProcessorPtr proc = expt->processor();

    if (proc) {
      QString dest = proc->filePathInRawOutputDirectory(file);

      QString cmd = tr("%1 -o ForwardX11=No %2@%3:%4/%5 %6")
          .arg(det->get_PilatusSCP())
          .arg(det->get_PilatusUser())
          .arg(det->get_PilatusHost())
          .arg(det->get_PilatusDataDirectory()).arg(file)
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

void QxrdPilatusDriver::remoteTransfer(QString file)
{
  // Transfer contents of a remote file to a local memory buffer:"

  QxrdPilatusSettingsPtr  det(m_Pilatus);
  QxrdExperimentPtr       expt(m_Experiment);

  if (det && expt && det->checkDetectorEnabled()) {
    //TODO: is this the right one...
    QxrdProcessorPtr proc = expt->processor();

    if (proc) {
      QString dest = proc->filePathInRawOutputDirectory(file);

      remoteCommand(tr("echo -n transfer: ; ls -la %1").arg(file));
      remoteCommand(tr("cat %1").arg(file));
    }
  }
}

void QxrdPilatusDriver::loadAndPush(QString f)
{
  QxrdPilatusSettingsPtr det(m_Pilatus);
  QxrdAcqCommonPtr       acq(m_Acquisition);
  QxrdExperimentPtr      expt(m_Experiment);

  if (acq && det && expt && det->checkDetectorEnabled()) {
    QxrdProcessorPtr proc = expt->processor();

    if (proc) {
      acq -> appendEvent(QxrdAcqCommon::DetectorFrameEvent,
                         det->get_DetectorIndex());

//      QxrdSynchronizedAcquisitionPtr sacq(acq->synchronizedAcquisition());

//      if (sacq) {
//        sacq->acquiredFrameAvailable(m_FrameCounter);
//      }

    //TODO: is this the right one...
      QString dest = proc->filePathInRawOutputDirectory(f);

      QcepUInt32ImageDataPtr image =
          QcepAllocator::newInt32Image(QcepObjectWPtr()/*sharedFromThis()*/,
                                       "pilatus",
                                       0,0,
                                       QcepAllocator::AllocateFromReserve);

      if (image->readImage(dest)) {
        printMessage(tr("Read %1 successfully").arg(dest));
      }

      if (qcepDebug(DEBUG_DETECTORIDLING)) {
        printMessage("enqueue pilatus acquired frame");
      }

      image -> set_ExposureTime(m_ExposureTime);
      image -> set_SummedExposures(1);

      if (m_ExposureFactor > 1) {
        if (m_SubframeCounter == 0) {
          m_AccumulatedData =
              QcepAllocator::newInt32Image(QcepObjectWPtr()/*sharedFromThis()*/,
                                           tr("accum-%1").arg(m_FrameCounter),
                                           image->get_Width(), image->get_Height(),
                                           QcepAllocator::AllocateFromReserve);
        }

        m_AccumulatedData -> set_ExposureTime(m_ExposureTime);
        m_AccumulatedData -> accumulateImage(image);

        m_SubframeCounter++;

        if (m_SubframeCounter == m_ExposureFactor) {
          det->enqueueAcquiredFrame(m_AccumulatedData);

          m_AccumulatedData = QcepUInt32ImageDataPtr();
          m_SubframeCounter = 0;
        }
      } else {
        det->enqueueAcquiredFrame(image);
      }

      m_FrameCounter++;

      acq -> appendEvent(QxrdAcqCommon::DetectorFramePostedEvent,
                         det->get_DetectorIndex());
    }
  }
}

