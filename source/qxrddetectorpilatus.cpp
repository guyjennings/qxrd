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
  m_PilatusHost            (saver, this, "pilatusHost",          "s11id-pilatus", "Host Address of Computer running Camserver"),
  m_PilatusPort            (saver, this, "pilatusPort",          41234,         "Camserver Port Number"),
  m_PilatusFilePattern     (saver, this, "pilatusFilePattern",   "%s-%-0.5d%s", "File pattern for saved files"),
  m_PilatusDataDirectory   (saver, this, "pilatusDataDirectory", "/home/det/shareddata/test/",    "Data directory on Camserver computer"),
  m_ReadFilesLocally       (saver, this, "readFilesLocally",     true, "Attempt to read acquired files into QXRD for further processing"),
  m_LocalDataDirectory     (saver, this, "localDataDirectory",   "/home/bessrc/shareddata/test/", "Data directory as seen on QXRD computer"),
  m_DeleteFilesAfterReading(saver, this, "deleteFilesAfterReading", false, "Delete files from Camserver computer after reading"),
  m_ExposureMode           (saver, this, "exposureMode",         0, "Pilatus Exposure Mode = (0:No Trigger, 1:ExtTrigger, 2:ExtEnable"),
  m_EnableFrequency        (saver, this, "enableFrequency",      1000, "Frequency of ext enable signal")
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
    connect(&m_ExpectedFileTimer, &QTimer::timeout, this, &QxrdDetectorPilatus::checkExpectedFiles);

    //    m_ExpectedFileTimer.start(1000);

    sendCommand("telemetry");
    sendCommand("nimages 1");
    sendCommand("nexpframe 1");
    imagePath(get_PilatusDataDirectory());
  }
}

void QxrdDetectorPilatus::stopDetector()
{
  printMessage(tr("Stopping Pilatus Detector at %1").arg(get_PilatusHost()));
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

void QxrdDetectorPilatus::readyRead()
{
  if (qcepDebug(DEBUG_PILATUS)) {
    printMessage("QxrdDetectorPilatus::readyRead");

    printMessage(tr("%1 bytes available").arg(m_PilatusSocket.bytesAvailable()));
  }

  QByteArray data = m_Preread + m_PilatusSocket.readAll();

  QList<QByteArray> lines = data.split(char(24)/*'\n'*/);

  if (lines.last() != "") {
    m_Preread = lines.last();
    lines.last() == "";
  }

  foreach(QByteArray line, lines) {
    if (line.count()) {
      QStringList lines = QString(line).split('\n');

      foreach(QString line, lines) {
        if (line.count()) {
          if (qcepDebug(DEBUG_PILATUS)) {
            printMessage(tr("(%1) : \"%2\"").arg(line.count()).arg(QString(line)));
          }

          interpretReply(line);
        }
      }
    }
  }

//  while (m_PilatusSocket.bytesAvailable()) {
//    if (qcepDebug(DEBUG_PILATUS)) {
//      printMessage(tr("%1 bytes available").arg(m_PilatusSocket.bytesAvailable()));
//    }

//    while (m_PilatusSocket.canReadLine()) {
//      QString aLine = m_PilatusSocket.readLine();

//      if (qcepDebug(DEBUG_PILATUS)) {
//        printMessage(tr("Line: %1").arg(aLine));
//      }

//      interpretReply(aLine);
//    }

//    if (m_PilatusSocket.bytesAvailable()) {
//      char c;
//      m_PilatusSocket.getChar(&c);

//      if (qcepDebug(DEBUG_PILATUS)) {
//        printMessage(tr("%1 bytes available, first char %2")
//                     .arg(m_PilatusSocket.bytesAvailable())
//                     .arg((int) c));
//      }

//      if (c != 24) {
//        m_PilatusSocket.ungetChar(c);
//        break;
//      }
//    }
//  }
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

QString QxrdDetectorPilatus::sendCommandReply(QString cmd)
{
  THREAD_CHECK;

  m_PilatusSocket.write(qPrintable(cmd+"\n"));
  m_PilatusSocket.waitForBytesWritten();

  return reply();
}

QString QxrdDetectorPilatus::reply()
{
  THREAD_CHECK;

  if (checkDetectorEnabled()) {
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

  return "";
}

void QxrdDetectorPilatus::expectReply(QString regexp)
{
  //  QRegExp exp(regexp);

  //  QString rply = reply();

  //  while (!exp.exactMatch(rply)) {
  //    printMessage(tr("%1 :Does not match: %2").arg(rply).arg(regexp));

  //    rply = reply();

  //    if (rply.length() == 0) {
  //      return;
  //    }
  //  }

  //  printMessage(tr("Matches: %1").arg(rply));
}

void QxrdDetectorPilatus::onExposureTimeChanged()
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "onExposureTimeChanged", Qt::BlockingQueuedConnection);
  } else {
    if (checkDetectorEnabled()) {
      QxrdAcquisitionPtr acq(m_Acquisition);

      if (acq) {
        exposureTime(acq->get_ExposureTime());
      }
    }
  }
}

void QxrdDetectorPilatus::exposureTime(double exposure)
{
  if (checkDetectorEnabled()) {
    sendCommand(tr("expt %1").arg(exposure));

    expectReply("15 OK Exposure time set to: (\\d*\\.\\d+) sec(.*)");
  }
}

void QxrdDetectorPilatus::exposurePeriod(double period)
{
  if (checkDetectorEnabled()) {
    sendCommand(tr("expp %1").arg(period));

    expectReply("15 OK Exposure period set to: (\\d*\\.\\d+) sec(.*)");
  }
}

void QxrdDetectorPilatus::exposureDelay(double delay)
{
  if (checkDetectorEnabled()) {
    sendCommand(tr("delay %1").arg(delay));

    expectReply("15 OK Delay time set to: (\\d*\\.\\d+) sec(.*)");
  }
}

void QxrdDetectorPilatus::exposuresPerFrame(int nexp)
{
  if (checkDetectorEnabled()) {
    sendCommand(tr("nexpf %1").arg(nexp));

    expectReply("15 OK Exposures per frame set to: (\\d*)(.*)");
  }
}

void QxrdDetectorPilatus::exposureFrameCount(int nfram)
{
  if (checkDetectorEnabled()) {
    sendCommand(tr("ni %1").arg(nfram));

    expectReply("15 OK N images set to: (\\d*)(.*)");
  }
}

void QxrdDetectorPilatus::exposure(QString file)
{
  if (checkDetectorEnabled()) {
    sendCommand(tr("exposure \"%1\"").arg(file));

    expectReply("15 OK  Starting (\\d*\\.\\d+) second background:(.*)");
  }
}

void QxrdDetectorPilatus::extTrigger(QString file)
{
  if (checkDetectorEnabled()) {
    sendCommand(tr("exttrigger \"%1\"").arg(file));

    expectReply("15 OK Starting externally triggered exposure\\(s\\):(.*)");
  }
}

void QxrdDetectorPilatus::extEnable(QString file)
{
  if (checkDetectorEnabled()) {
    sendCommand(tr("extenable \"%1\"").arg(file));

    expectReply("15 OK Starting externally enabled exposure\\(s\\):(.*)");
  }
}

void QxrdDetectorPilatus::imagePath(QString path)
{
  if (checkDetectorEnabled()) {
    sendCommand(tr("imgpath \"%1\"").arg(path));

    expectReply("10 OK (.*)");
  }
}

void QxrdDetectorPilatus::acquireImage(QString fileName, double exposure)
{
  if (checkDetectorEnabled()) {
    exposureTime(exposure);

    sendCommand(tr("exposure \"%1\"").arg(fileName));
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
    m_CurrentFile = acq->currentFileBase(get_DetectorNumber());

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
  printMessage(tr("QxrdDetectorPilatus::interpretReply(\"%1\")").arg(reply));


  if (reply.startsWith("7 OK")) { // Image has been saved...
    if (get_ReadFilesLocally() == false) {
      enqueueAcquiredFrame(QcepInt16ImageDataPtr());
    } else {
      pushFileExpected(m_CurrentFile);
    }
  } else if (reply.startsWith("Image format:")) {
    QRegExp matcher("Image format\\: (\\d+)\\(w\\) x (\\d+)\\(h\\) pixels(.*)");

    if (matcher.exactMatch(reply)) {
      int width = matcher.cap(1).toInt();
      int height = matcher.cap(2).toInt();

      printMessage(tr("Detector dimensions %1 cols x %2 rows").arg(width).arg(height));

      set_NRows(height);
      set_NCols(width);
    } else {
      printMessage(tr("Unmatched: %1, %2").arg(reply).arg(matcher.pattern()));
      printMessage(tr("Matched Length %1").arg(matcher.matchedLength()));
    }
  }
}

//void QxrdDetectorPilatus::acquire()
//{
//  if (QThread::currentThread() != thread()) {
//    QMetaObject::invokeMethod(this, "acquire");
//  } else {
//    QxrdAcquisitionPtr acq(m_Acquisition);

//    if (checkDetectorEnabled() && acq) {
//      double expos   = acq->get_ExposureTime();
//      int nsum       = acq->get_SummedExposures();
//      int nimg       = acq->get_PostTriggerFiles();

//      QString filenm = acq->get_FilePattern();
//      int index      = acq->get_FileIndex();

//      QString ofil = tr("%1_%2.cbf").arg(filenm).arg(index,5,10,QChar('0'));

//      pushFileExpected(ofil);

//      for (int i=1; i<nimg; i++) {
//        QString fn = tr("%1_%2.cbf").arg(filenm).arg(index+i,5,10,QChar('0'));

//        pushFileExpected(fn);
//      }

//      if (expos != m_ExposureTime) {
//        exposureTime(expos);
//        exposurePeriod(expos+0.01);
//        exposureDelay(0);
//        m_ExposureTime = expos;
//      }

//      if (nsum != m_ExposuresPerFrame) {
//        exposuresPerFrame(nsum);
//        m_ExposuresPerFrame = nsum;
//      }

//      if (nimg != m_ExposureFrameCount) {
//        exposureFrameCount(nimg);
//        m_ExposureFrameCount = nimg;
//      }

//      exposure(ofil);
//    }
//  }
//}

void QxrdDetectorPilatus::pushFileExpected(QString fn)
{
  if (checkDetectorEnabled()) {
    printMessage(tr("Expect file %1").arg(fn));

    QDir d(get_LocalDataDirectory());

    QString fp = d.filePath(fn);

    QFile f(fp);

    if (f.exists()) {
      printMessage(tr("File %1 exists").arg(f.fileName()));

      f.remove();
    }

    //  m_FileWatcher.addPath(fp);

    m_ExpectedFiles.append(fp);

    if (m_ExpectedFiles.count() == 1) {
      printMessage("Starting timer");
      connect(&m_ExpectedFileTimer, &QTimer::timeout, this, &QxrdDetectorPilatus::checkExpectedFiles);
      m_ExpectedFileTimer.start(100);
    }
  }
}

void QxrdDetectorPilatus::checkExpectedFiles()
{
  if (checkDetectorEnabled()) {
    QDir d(get_LocalDataDirectory());

    d.entryList();

    while (m_ExpectedFiles.count() > 0) {
      QString fn = m_ExpectedFiles.first();

      if (qcepDebug(DEBUG_PILATUS)) {
        printMessage(tr("Looking for %1").arg(fn));
      }

      QFile f(fn);

      if (f.exists()) {
        m_ExpectedFiles.pop_front();

        if (qcepDebug(DEBUG_PILATUS)) {
          printMessage(tr("File %1 exists").arg(f.fileName()));
        }

        QcepInt32ImageDataPtr data = QcepAllocator::newInt32Image(QcepAllocator::AllocateFromReserve, 0,0, this);

        data->readImage(f.fileName());

        enqueueAcquiredFrame(data);
      } else {
        return;
      }
    }

    m_ExpectedFileTimer.stop();
  }
}

//void QxrdDetectorPilatus::fileChanged(const QString &path)
//{
//  printMessage(tr("QxrdDetectorPilatus::fileChanged: %1").arg(path));
//}

//void QxrdDetectorPilatus::directoryChanged(const QString &path)
//{
//  printMessage(tr("QxrdDetectorPilatus::directoryChanged: %1").arg(path));
//}

void QxrdDetectorPilatus::pushDefaultsToProxy(QxrdDetectorProxyPtr proxy)
{
  QxrdDetector::pushDefaultsToProxy(proxy, QxrdDetectorThread::PilatusDetector);

  if (proxy) {
    proxy->pushProperty(QxrdDetectorProxy::StringProperty, "pilatusHost",          "Camserver Host",    "s11id-pilatus");
    proxy->pushProperty(QxrdDetectorProxy::FixedIntegerProperty,   "pilatusPort",  "Camserver Port",    41234);
    proxy->pushProperty(QxrdDetectorProxy::StringProperty, "pilatusFilePattern",   "Save File Pattern", "%s-%-0.5d%s");
    proxy->pushProperty(QxrdDetectorProxy::StringProperty, "pilatusDataDirectory", "Camserver Data Directory", "/home/det/shareddata/test/");
    proxy->pushProperty(QxrdDetectorProxy::BooleanProperty, "readFilesLocally",     "Attempt to read acquired files into QXRD for further processing", true);
    proxy->pushProperty(QxrdDetectorProxy::DirectoryProperty, "localDataDirectory","Local Data Directory", "/home/bessrc/shareddata/test/");
    proxy->pushProperty(QxrdDetectorProxy::BooleanProperty, "deleteFilesAfterReading", "Delete files from camserver computer after use", false);
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
    proxy->pushProperty(QxrdDetectorProxy::StringProperty, "pilatusFilePattern",   "Save File Pattern", get_PilatusFilePattern());
    proxy->pushProperty(QxrdDetectorProxy::StringProperty, "pilatusDataDirectory", "Camserver Data Directory", get_PilatusDataDirectory());
    proxy->pushProperty(QxrdDetectorProxy::BooleanProperty, "readFilesLocally",     "Attempt to read acquired files into QXRD for further processing", get_ReadFilesLocally());
    proxy->pushProperty(QxrdDetectorProxy::DirectoryProperty, "localDataDirectory","Local Data Directory", get_LocalDataDirectory());
    proxy->pushProperty(QxrdDetectorProxy::BooleanProperty, "deleteFilesAfterReading", "Delete files from camserver computer after use", get_DeleteFilesAfterReading());
    proxy->pushProperty(QxrdDetectorProxy::PilatusModeProperty, "exposureMode",     "Pilatus Exposure Mode", get_ExposureMode());
    proxy->pushProperty(QxrdDetectorProxy::DoubleProperty, "enableFrequency",       "Ext Enable Frequency", get_EnableFrequency());
  }
}

void QxrdDetectorPilatus::pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy)
{
  QxrdDetector::pullPropertiesfromProxy(proxy);

  if (proxy) {
    set_PilatusHost            (proxy->property("pilatusHost").toString());
    set_PilatusFilePattern     (proxy->property("pilatusFilePattern").toString());
    set_PilatusDataDirectory   (proxy->property("pilatusDataDirectory").toString());
    set_ReadFilesLocally       (proxy->property("readFilesLocally").toBool());
    set_LocalDataDirectory     (proxy->property("localDataDirectory").toString());
    set_DeleteFilesAfterReading(proxy->property("deleteFilesAfterReading").toBool());
    set_ExposureMode           (proxy->property("exposureMode").toInt());
    set_EnableFrequency        (proxy->property("enableFrequency").toDouble());
  }
}
