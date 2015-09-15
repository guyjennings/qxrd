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

QxrdDetectorPilatus::QxrdDetectorPilatus(QcepSettingsSaverWPtr saver, QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq, QcepObject *parent) :
  QxrdDetector(saver, expt, acq, QxrdDetectorThread::PilatusDetector, parent),
  m_PilatusSocket(),
  m_ExposureTime(-1),
  m_ExposuresPerFrame(-1),
  m_ExposureFrameCount(-1),
  m_PilatusHost         (saver, this, "pilatusHost",          "s11id-pilatus", "Host Address of Computer running Camserver"),
  m_PilatusPort         (saver, this, "pilatusPort",          41234,         "Camserver Port Number"),
  m_PilatusFilePattern  (saver, this, "pilatusFilePattern",   "%s-%-0.5d%s", "File pattern for saved files"),
  m_PilatusDataDirectory(saver, this, "pilatusDataDirectory", "/home/det/shareddata/test/",    "Data directory on Camserver computer"),
  m_LocalDataDirectory  (saver, this, "localDataDirectory",   "/home/bessrc/shareddata/test/", "Data directory as seen on QXRD computer")
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
    QMetaObject::invokeMethod(this, "initialize", Qt::BlockingQueuedConnection);
  } else {
    QxrdDetector::initialize();

    m_PilatusSocket.connectToHost(get_PilatusHost(), get_PilatusPort());
    m_PilatusSocket.waitForConnected();

//    QString a = sendCommandReply("telemetry");

//    printf("Read %s\n", qPrintable(a));

    connect(&m_PilatusSocket, &QTcpSocket::readyRead, this, &QxrdDetectorPilatus::readyRead);
//    connect(&m_FileWatcher,   &QFileSystemWatcher::fileChanged, this, &QxrdDetectorPilatus::fileChanged);
//    connect(&m_FileWatcher,   &QFileSystemWatcher::directoryChanged, this, &QxrdDetectorPilatus::directoryChanged);

    connect(&m_ExpectedFileTimer, &QTimer::timeout, this, &QxrdDetectorPilatus::checkExpectedFiles);

//    m_ExpectedFileTimer.start(1000);

    sendCommand("telemetry");
    imagePath(get_PilatusDataDirectory());

//    m_FileWatcher.addPath(get_LocalDataDirectory());
  }
}

void QxrdDetectorPilatus::readyRead()
{
  if (qcepDebug(DEBUG_PILATUS)) {
    printMessage("QxrdDetectorPilatus::readyRead");

    printMessage(tr("%1 bytes available").arg(m_PilatusSocket.bytesAvailable()));
  }

//  while (m_PilatusSocket.canReadLine()) {
    QString aLine = m_PilatusSocket.readAll();

    printMessage(tr("Line: %1").arg(aLine));
//  }
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
  QxrdAcquisitionPtr acq(m_Acquisition);

  if (acq) {
    exposureTime(acq->get_ExposureTime());
  }
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

  expectReply("15 OK Starting externally triggered exposure\\(s):(.*)");
}

void QxrdDetectorPilatus::extEnable(QString file)
{
  sendCommand(tr("extenable \"%1\"").arg(file));

  expectReply("15 OK Starting externally enabled exposure\\(s):(.*)");
}

void QxrdDetectorPilatus::imagePath(QString path)
{
  sendCommand(tr("imgpath \"%1\"").arg(path));

  expectReply("10 OK (.*)");
}

void QxrdDetectorPilatus::acquireImage(QString fileName, double exposure)
{
  exposureTime(exposure);

  sendCommand(tr("exposure \"%1\"").arg(fileName));
}

void QxrdDetectorPilatus::acquire()
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "acquire");
  } else {
    QxrdAcquisitionPtr acq(m_Acquisition);

    if (acq) {
      double expos   = acq->get_ExposureTime();
      int nsum       = acq->get_SummedExposures();
      int nimg       = acq->get_PostTriggerFiles();

      QString filenm = acq->get_FilePattern();
      int index      = acq->get_FileIndex();

      QString ofil = tr("%1_%2.cbf").arg(filenm).arg(index,5,10,QChar('0'));

      pushFileExpected(ofil);

      for (int i=1; i<nimg; i++) {
        QString fn = tr("%1_%2.cbf").arg(filenm).arg(index+i,5,10,QChar('0'));

        pushFileExpected(fn);
      }

      if (expos != m_ExposureTime) {
        exposureTime(expos);
        exposurePeriod(expos+0.01);
        exposureDelay(0);
        m_ExposureTime = expos;
      }

      if (nsum != m_ExposuresPerFrame) {
        exposuresPerFrame(nsum);
        m_ExposuresPerFrame = nsum;
      }

      if (nimg != m_ExposureFrameCount) {
        exposureFrameCount(nimg);
        m_ExposureFrameCount = nimg;
      }

      exposure(ofil);
    }
  }
}

void QxrdDetectorPilatus::pushFileExpected(QString fn)
{
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

void QxrdDetectorPilatus::checkExpectedFiles()
{
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

      QcepDoubleImageDataPtr data = QcepAllocator::newDoubleImage("", 0,0, this);

      data->readImage(f.fileName());

      printMessage(tr("File %1 has been read").arg(f.fileName()));
    } else {
      return;
    }
  }

  m_ExpectedFileTimer.stop();
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
    proxy->pushProperty(QxrdDetectorProxy::DirectoryProperty, "localDataDirectory","Local Data Directory", "/home/bessrc/shareddata/test/");
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
    proxy->pushProperty(QxrdDetectorProxy::DirectoryProperty, "localDataDirectory","Local Data Directory", get_LocalDataDirectory());
  }
}

void QxrdDetectorPilatus::pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy)
{
  QxrdDetector::pullPropertiesfromProxy(proxy);

  if (proxy) {
    set_PilatusHost         (proxy->property("pilatusHost").toString());
    set_PilatusFilePattern  (proxy->property("pilatusFilePattern").toString());
    set_PilatusDataDirectory(proxy->property("pilatusDataDirectory").toString());
    set_LocalDataDirectory  (proxy->property("localDataDirectory").toString());
  }
}
