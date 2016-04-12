#include "qxrddetectorpilatus.h"
#include "qxrddetectorthread.h"
#include "qcepproperty.h"
#include "qxrddebug.h"
#include "qxrdacquisition.h"
#include <QDir>
#include <QFile>
#include "qcepimagedata.h"
#include "qcepallocator.h"
#include "qxrddetectorproxy.h"
#include "qxrddetectorpilatusremote.h"
#include "qxrdexperiment.h"
#include "qxrddataprocessor.h"

QxrdDetectorPilatus::QxrdDetectorPilatus(QcepSettingsSaverWPtr saver,
                                         QxrdExperimentWPtr    expt,
                                         QxrdAcquisitionWPtr   acq,
                                         int                   detNum,
                                         QcepObject           *parent) :
  QxrdDetector(saver, expt, acq, QxrdDetectorThread::PilatusDetector, detNum, parent),
  m_PilatusSocket(),
  m_ExposureTime(-1),
  m_ExposuresPerFrame(-1),
  m_ExposureFrameCount(-1),
  m_Remote(new QxrdDetectorPilatusRemote(this)),
  m_PilatusHost            (saver, this, "pilatusHost",          "s11id-pilatus", "Host Address of Computer running Camserver"),
  m_PilatusPort            (saver, this, "pilatusPort",          41234,         "Camserver Port Number"),
  m_PilatusUser            (saver, this, "pilatusUser",          "det",         "Camserver User Name"),
  m_PilatusSSH             (saver, this, "pilatusSSH",           "ssh",         "ssh command path"),
  m_PilatusSCP             (saver, this, "pilatusSCP",           "scp",         "scp command path"),
  m_PilatusDataDirectory   (saver, this, "pilatusDataDirectory", "/home/det/shareddata/test/",    "Data directory on Camserver computer"),
  m_ReadFilesLocally       (saver, this, "readFilesLocally",     true, "Attempt to read acquired files into QXRD for further processing"),
  m_DeleteFilesAfterReading(saver, this, "deleteFilesAfterReading", false, "Delete files from Camserver computer after reading"),
  m_ExposureMode           (saver, this, "exposureMode",         0, "Pilatus Exposure Mode = (0:No Trigger, 1:ExtTrigger, 2:ExtEnable"),
  m_EnableFrequency        (saver, this, "enableFrequency",      1000, "Frequency of ext enable signal"),
  m_PilatusExtension       (saver, this, "pilatusExtension",     "cbf",         "File format to be used by camserver")
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

void QxrdDetectorPilatus::startDetector()
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "startDetector"/*, Qt::BlockingQueuedConnection*/);
  } else {
    QxrdDetector::startDetector();

    printMessage(tr("Starting Pilatus Detector at %1:%2").arg(get_PilatusHost()).arg(get_PilatusPort()));

    m_PilatusSocket.connectToHost(get_PilatusHost(), get_PilatusPort());
    m_PilatusSocket.waitForConnected();

    if (qcepDebug(DEBUG_PILATUS)) {
      printMessage("Connected to pilatus...");
    }

    connect(&m_PilatusSocket, &QTcpSocket::readyRead, this, &QxrdDetectorPilatus::readyRead);

    sendCommand("telemetry");
    sendCommand("nimages 1");
    sendCommand("nexpframe 1");

    imagePath(get_PilatusDataDirectory());
  }
}

void QxrdDetectorPilatus::stopDetector()
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "stopDetector");
  } else {
    QxrdDetector::stopDetector();

    printMessage(tr("Stopping Pilatus Detector at %1").arg(get_PilatusHost()));

    m_PilatusSocket.close();
  }
}

void QxrdDetectorPilatus::beginAcquisition(double exposure)
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "beginAcquisition", Qt::BlockingQueuedConnection, Q_ARG(double, exposure));
  } else {
    QxrdDetector::beginAcquisition(exposure);

    if (qcepDebug(DEBUG_PILATUS)) {
      printMessage(tr("QxrdDetectorPilatus::beginAcquisition(%1)").arg(exposure));
    }

    imagePath(get_PilatusDataDirectory());

    beginExposure(exposure);
  }
}

void QxrdDetectorPilatus::endAcquisition()
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "endAcquisition", Qt::BlockingQueuedConnection);
  } else {
    QxrdDetector::endAcquisition();
  }
}

void QxrdDetectorPilatus::shutdownAcquisition()
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "shutdownAcquisition", Qt::BlockingQueuedConnection);
  } else {
    QxrdDetector::shutdownAcquisition();
  }
}

void QxrdDetectorPilatus::executeCommand(QString cmd)
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "executeCommand", Qt::BlockingQueuedConnection, Q_ARG(QString, cmd));
  } else {
    sendCommand(cmd);
  }
}

void QxrdDetectorPilatus::readyRead()
{
  if (qcepDebug(DEBUG_PILATUS)) {
    printMessage("QxrdDetectorPilatus::readyRead");

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

void QxrdDetectorPilatus::sendCommand(QString cmd)
{
  THREAD_CHECK;

  if (qcepDebug(DEBUG_PILATUS)) {
    printMessage(tr("QxrdDetectorPilatus::sendCommand(\"%1\")").arg(cmd));
  }

  m_PilatusSocket.write(qPrintable(cmd+"\n"));
  m_PilatusSocket.waitForBytesWritten();
}

void QxrdDetectorPilatus::imagePath(QString path)
{
  if (checkDetectorEnabled()) {
    sendCommand(tr("imgpath \"%1\"").arg(path));
  }
}

void QxrdDetectorPilatus::beginExposure(double exposure)
{
  int expMode = get_ExposureMode();

  if (expMode == NoExternalTrigger) {
    sendCommand(tr("ExpTime %1").arg(exposure));
    sendCommand("nexpframe 1");
  } else if (expMode == ExternalTrigger) {
    sendCommand("Delay 0");
    sendCommand(tr("ExpPeriod %1").arg(exposure));
    sendCommand(tr("ExpTime %1").arg(exposure));
    sendCommand("nexpframe 1");
  } else if (expMode == ExternalEnable) {
    int nFrames = (int) (get_EnableFrequency()*exposure);
    sendCommand(tr("nexpframe %1").arg(nFrames));
  }
}

void QxrdDetectorPilatus::expose()
{
  QxrdAcquisitionPtr acq(m_Acquisition);

  if (acq) {
    m_CurrentFile = acq->currentFileBase(get_DetectorNumber(), get_PilatusExtension());

    if (get_ReadFilesLocally()) { // Check to see if file exists...
      if (get_DeleteFilesAfterReading()) {
        // Attempt to delete file before acquisition...
      }
    }

    int expMode = get_ExposureMode();

    if (expMode == NoExternalTrigger) {
      sendCommand(tr("Exposure \"%1\"").arg(m_CurrentFile));
    } else if (expMode == ExternalTrigger) {
      sendCommand(tr("ExtTrigger \"%1\"").arg(m_CurrentFile));
    } else if (expMode == ExternalEnable) {
      sendCommand(tr("ExtEnable \"%1\"").arg(m_CurrentFile));
    }
  }
}

void QxrdDetectorPilatus::beginFrame()
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "beginFrame", Qt::BlockingQueuedConnection);
  } else {
    if (qcepDebug(DEBUG_PILATUS)) {
      printMessage("QxrdDetectorPilatus::beginFrame");
    }

    QxrdDetector::beginFrame();

    expose();
  }
}

void QxrdDetectorPilatus::interpretReply(QString reply)
{
  if (qcepDebug(DEBUG_PILATUS)) {
    printMessage(tr("QxrdDetectorPilatus::interpretReply(\"%1\")").arg(reply));
  }

  if (reply.startsWith("1 ERR")) {
    criticalMessage(tr("Error with pilatus detector: %1").arg(reply));
  } else if (reply.startsWith("7 OK")) { // Image has been saved...
    if (get_ReadFilesLocally() == false) {
      enqueueAcquiredFrame(QcepInt16ImageDataPtr());
    } else {
      remoteCopy(m_CurrentFile);
//      pushFileExpected(m_CurrentFile);
      loadAndPush(m_CurrentFile);

      if (get_DeleteFilesAfterReading()) {
        remoteDelete(m_CurrentFile);
      }
    }
  } else if (reply.startsWith("Image format:")) {
    QRegExp matcher("Image format\\: (\\d+)\\(w\\) x (\\d+)\\(h\\) pixels(.*)");

    if (matcher.exactMatch(reply)) {
      int width = matcher.cap(1).toInt();
      int height = matcher.cap(2).toInt();

      printMessage(tr("Detector dimensions %1 cols x %2 rows").arg(width).arg(height));

      set_NRows(height);
      set_NCols(width);
      set_HBinning(1);
      set_VBinning(1);
    } else {
      printMessage(tr("Unmatched: %1, %2").arg(reply).arg(matcher.pattern()));
      printMessage(tr("Matched Length %1").arg(matcher.matchedLength()));
    }
  }
}

void QxrdDetectorPilatus::pushDefaultsToProxy(QxrdDetectorProxyPtr proxy)
{
  QxrdDetector::pushDefaultsToProxy(proxy, QxrdDetectorThread::PilatusDetector);

  if (proxy) {
    proxy->pushProperty(QxrdDetectorProxy::StringProperty, "pilatusHost",          "Camserver Host",    "s11id-pilatus");
    proxy->pushProperty(QxrdDetectorProxy::FixedIntegerProperty,   "pilatusPort",  "Camserver Port",    41234);
    proxy->pushProperty(QxrdDetectorProxy::StringProperty,  "pilatusUser",         "Camserver User",    "det");
    proxy->pushProperty(QxrdDetectorProxy::FilenameProperty,  "pilatusSSH",        "ssh command",       "ssh");
    proxy->pushProperty(QxrdDetectorProxy::FilenameProperty,  "pilatusSCP",        "scp command",       "scp");
    proxy->pushProperty(QxrdDetectorProxy::DirectoryProperty, "pilatusDataDirectory", "Camserver Data Directory", "/home/det/shareddata/test/");
    proxy->pushProperty(QxrdDetectorProxy::BooleanProperty, "readFilesLocally",     "Attempt to read acquired files into QXRD for further processing", true);
    proxy->pushProperty(QxrdDetectorProxy::BooleanProperty, "deleteFilesAfterReading", "Delete files from camserver computer after use", false);
    proxy->pushProperty(QxrdDetectorProxy::ExtensionProperty, "pilatusExtension",   "File format for camserver", "cbf");
    proxy->pushProperty(QxrdDetectorProxy::PilatusModeProperty, "exposureMode",     "Pilatus Exposure Mode", 0);
    proxy->pushProperty(QxrdDetectorProxy::DoubleProperty, "enableFrequency",       "Ext Enable Frequency", 1000);
  }
}

void QxrdDetectorPilatus::pushPropertiesToProxy(QxrdDetectorProxyPtr proxy)
{
  QxrdDetector::pushPropertiesToProxy(proxy);

  if (proxy) {
    proxy->pushProperty(QxrdDetectorProxy::StringProperty, "pilatusHost",          "Camserver Host",    get_PilatusHost());
    proxy->pushProperty(QxrdDetectorProxy::FixedIntegerProperty,   "pilatusPort",  "Camserver Port",    get_PilatusPort());
    proxy->pushProperty(QxrdDetectorProxy::StringProperty, "pilatusUser",          "Camserver User",    get_PilatusUser());
    proxy->pushProperty(QxrdDetectorProxy::FilenameProperty,  "pilatusSSH",        "ssh command",       get_PilatusSSH());
    proxy->pushProperty(QxrdDetectorProxy::FilenameProperty,  "pilatusSCP",        "scp command",       get_PilatusSCP());
    proxy->pushProperty(QxrdDetectorProxy::DirectoryProperty, "pilatusDataDirectory", "Camserver Data Directory", get_PilatusDataDirectory());
    proxy->pushProperty(QxrdDetectorProxy::BooleanProperty, "readFilesLocally",     "Attempt to read acquired files into QXRD for further processing", get_ReadFilesLocally());
    proxy->pushProperty(QxrdDetectorProxy::BooleanProperty, "deleteFilesAfterReading", "Delete files from camserver computer after use", get_DeleteFilesAfterReading());
    proxy->pushProperty(QxrdDetectorProxy::ExtensionProperty, "pilatusExtension",   "File format for camserver", get_PilatusExtension());
    proxy->pushProperty(QxrdDetectorProxy::PilatusModeProperty, "exposureMode",     "Pilatus Exposure Mode", get_ExposureMode());
    proxy->pushProperty(QxrdDetectorProxy::DoubleProperty, "enableFrequency",       "Ext Enable Frequency", get_EnableFrequency());
  }
}

void QxrdDetectorPilatus::pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy)
{
  QxrdDetector::pullPropertiesfromProxy(proxy);

  if (proxy) {
    set_PilatusHost            (proxy->property("pilatusHost").toString());
    set_PilatusUser            (proxy->property("pilatusUser").toString());
    set_PilatusSSH             (proxy->property("pilatusSSH").toString());
    set_PilatusSCP             (proxy->property("pilatusSCP").toString());
    set_PilatusDataDirectory   (proxy->property("pilatusDataDirectory").toString());
    set_ReadFilesLocally       (proxy->property("readFilesLocally").toBool());
    set_DeleteFilesAfterReading(proxy->property("deleteFilesAfterReading").toBool());
    set_PilatusExtension       (proxy->property("pilatusExtension").toString());
    set_ExposureMode           (proxy->property("exposureMode").toInt());
    set_EnableFrequency        (proxy->property("enableFrequency").toDouble());
  }
}

void QxrdDetectorPilatus::remoteConnect(QString sshCmd)
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "remoteConnect", Q_ARG(QString, sshCmd));
  } else {
    if (m_Remote) {
      m_Remote->connectToRemote(sshCmd);
    }
  }
}

void QxrdDetectorPilatus::remoteCommand(QString cmd)
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "remoteCommand", Q_ARG(QString, cmd));
  } else {
    if (m_Remote) {
      m_Remote->executeRemote(cmd);
    }
  }
}

void QxrdDetectorPilatus::remoteDelete(QString file)
{
  QString cmd = tr("%1 -o ForwardX11=No %2@%3 rm %4/%5")
      .arg(get_PilatusSSH())
      .arg(get_PilatusUser())
      .arg(get_PilatusHost())
      .arg(get_PilatusDataDirectory()).arg(file);

  if (qcepDebug(DEBUG_PILATUS)) {
    printMessage(tr("Deleting: %1").arg(cmd));
  }

  int rc = QProcess::execute(cmd);

  if (rc) {
    printMessage(tr("rc = %1").arg(rc));
  }
}

void QxrdDetectorPilatus::remoteCopy(QString file)
{
  QxrdExperimentPtr  expt(m_Experiment);

  if (expt) {
    QxrdDataProcessorPtr proc = expt->dataProcessor();

    if (proc) {
      QString dest = proc->filePathInRawOutputDirectory(file);

      QString cmd = tr("%1 -o ForwardX11=No %2@%3:%4/%5 %6")
          .arg(get_PilatusSCP())
          .arg(get_PilatusUser())
          .arg(get_PilatusHost())
          .arg(get_PilatusDataDirectory()).arg(file)
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

void QxrdDetectorPilatus::remoteTransfer(QString file)
{
  // Transfer contents of a remote file to a local memory buffer:"

  QxrdExperimentPtr  expt(m_Experiment);

  if (expt) {
    QxrdDataProcessorPtr proc = expt->dataProcessor();

    if (proc) {
      QString dest = proc->filePathInRawOutputDirectory(file);

      remoteCommand(tr("echo -n transfer: ; ls -la %1").arg(file));
      remoteCommand(tr("cat %1").arg(file));
    }
  }
}

void QxrdDetectorPilatus::loadAndPush(QString f)
{
  QxrdExperimentPtr  expt(m_Experiment);

  if (expt) {
    QxrdDataProcessorPtr proc = expt->dataProcessor();

    if (proc) {
      QString dest = proc->filePathInRawOutputDirectory(f);

      QcepInt32ImageDataPtr data = QcepAllocator::newInt32Image(dest, 0,0, QcepAllocator::AllocateFromReserve);

      if (data->readImage(dest)) {
        printMessage(tr("Read %1 successfully").arg(dest));
      }

      enqueueAcquiredFrame(data);
    }
  }
}
