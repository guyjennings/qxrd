#include "qxrddebug.h"
#include "qxrddexeladriver.h"
#include "qxrddexelasettings.h"
#include "qxrdacqcommon.h"
#include "qxrdexperiment.h"
#include "qxrdsynchronizedacquisition.h"
#include "qcepallocator.h"
#include <QPainter>
#include "BusScanner.h"
#include "DexelaDetector.h"
#include "DexImage.h"
#include <QThread>
#include "qcepmutexlocker.h"
#include "qcepallocator.h"

QxrdDexelaDriver::QxrdDexelaDriver(QString name,
                                                   QxrdDexelaSettingsWPtr det,
                                                   QxrdExperimentWPtr expt,
                                                   QxrdAcqCommonWPtr acq)
  : QxrdDetectorDriver(name, det, expt, acq),
    m_Dexela(qSharedPointerDynamicCast<QxrdDexelaSettings>(det))
{
#ifndef QT_NO_DEBUG
  printf("Dexela Driver \"%s\" Constructed\n", qPrintable(name));
#endif
}

QxrdDexelaDriver::~QxrdDexelaDriver()
{
#ifndef QT_NO_DEBUG
  printf("Dexela Driver \"%s\" Destroyed\n", qPrintable(get_Name()));
#endif
}

QMutex           QxrdDexelaDriver::m_Mutex;
BusScanner      *QxrdDexelaDriver::m_BusScanner;
int              QxrdDexelaDriver::m_DetectorCount;
int              QxrdDexelaDriver::m_Initialized;
QVector<DevInfo> QxrdDexelaDriver::m_Devices;

int QxrdDexelaDriver::scanForDetectors()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (m_Initialized == 0) {
    m_BusScanner = new BusScanner();

    m_DetectorCount = m_BusScanner -> EnumerateDevices();

    printMessage(tr("Found %1 Detectors").arg(m_DetectorCount));

    for (int i=0; i<m_DetectorCount; i++) {
      DevInfo info = m_BusScanner -> GetDevice(i);

      m_Devices.append(info);

      printMessage(tr("Detector %1: Model %2: Serial Number %3")
                   .arg(i)
                   .arg(info.model)
                   .arg(info.serialNum));

//      DexelaDetector det(info);

//      det.OpenBoard();

//      int fwVersion = det.GetFirmwareVersion();

//      int dayMonth, year, time;

//      det.GetFirmwareBuild(dayMonth, year, time);

//      printMessage(tr("Detector %1 : XDim %2, YDim %3, NBuff %4")
//                   .arg(i).arg(det.GetBufferXdim()).arg(det.GetBufferYdim()).arg(det.GetNumBuffers()));

//      printMessage(tr("Detector %1 : Firmware %2: Build %3/%4: %5")
//                   .arg(i).arg(fwVersion).arg(dayMonth).arg(year).arg(time));

//      printMessage(tr("Detector %1 : Exposure Time %2, Gap Time %3")
//                   .arg(i).arg(det.GetExposureTime()).arg(det.GetGapTime()));

//      det.CloseBoard();
    }

    m_Initialized = 1;

    return true;
  } else {
    return false;
  }
}

static int detIndex = 0;

bool QxrdDexelaDriver::startDetectorDriver()
{
  THREAD_CHECK;

  if (qcepDebug(DEBUG_DEXELA)) {
    printMessage(tr("QxrdDexelaDriver::startDetectorDriver"));
  }

  QxrdDexelaSettingsPtr det(m_Dexela);
  QxrdAcqCommonPtr      acq(m_Acquisition);

  if (acq && det && det->checkDetectorEnabled()) {
    scanForDetectors();

    m_DetectorIndex = detIndex++; // det->get_DetectorIndex();

    printMessage(tr("Starting Dexela detector %1: \"%2\"")
                 .arg(m_DetectorIndex)
                 .arg(det->get_DetectorName()));


    if (m_DetectorIndex >= 0 && m_DetectorIndex < m_DetectorCount) {
      m_DexelaDetector = new DexelaDetector(m_Devices[m_DetectorIndex]);

      if (m_DexelaDetector) {
        QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

        try {
          m_DexelaDetector -> OpenBoard();

          m_XDim = m_DexelaDetector -> GetBufferXdim();
          m_YDim = m_DexelaDetector -> GetBufferYdim();

          det -> set_NCols(m_XDim);
          det -> set_NRows(m_YDim);

          m_DexelaDetector -> SetCallback(&QxrdDexelaDriver::staticCallback);
          m_DexelaDetector -> SetCallbackData((void*) this);

          m_DexelaDetector -> SetFullWellMode(High);
          m_DexelaDetector -> SetExposureTime(acq->get_ExposureTime());
          m_DexelaDetector -> SetBinningMode(x11);
          m_DexelaDetector -> SetExposureMode(Expose_and_read);
          m_DexelaDetector -> SetTriggerSource(Internal_Software);
          m_DexelaDetector -> EnablePulseGenerator();

          m_DexelaDetector -> GoLiveSeq();

          m_DexelaDetector -> ToggleGenerator(true);

          printMessage(tr("Dexela Detector Exposure time %1").arg(m_DexelaDetector->GetExposureTime()));
        } catch (DexelaException &e) {
          printMessage(tr("Dexela Exception caught"));
        }
      }
    }

    return changeExposureTime(acq->get_ExposureTime());
  } else {
    return false;
  }
}

void QxrdDexelaDriver::staticCallback(int fc, int buf, DexelaDetector *det)
{
//  printf("QxrdDexelaDriver::staticCallback called fc=%d, buf=%d, det=%x\n", fc, buf, det);
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QxrdDexelaDriver *dex = reinterpret_cast<QxrdDexelaDriver*>(det->GetCallbackData());

  if (dex) {
    INVOKE_CHECK(
          QMetaObject::invokeMethod(dex, "onAcquiredFrame", Qt::QueuedConnection, Q_ARG(int, fc), Q_ARG(int, buf)))
  }
}

void QxrdDexelaDriver::callback(int fc, int buf, DexelaDetector *det)
{
//  printMessage(tr("QxrdDexelaDriver::callback ind:%1, fc:%2, buf:%3")
//               .arg(m_DetectorIndex).arg(fc).arg(buf));

//  if (det != m_DexelaDetector) {
//    printMessage("Dexela detector mismatch in QxrdDexelaDriver::callback");
//  }

}

void QxrdDexelaDriver::onAcquiredFrame(int fc, int buf)
{
  QcepUInt16ImageDataPtr image =
      QcepAllocator::newInt16Image(sharedFromThis(),
                                   tr("frame-%1").arg(fc),
                                   m_XDim, m_YDim,
                                   QcepAllocator::AllocateFromReserve);

  if (image) {
    QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

    quint16 *ptr = image->data();

    m_DexelaDetector -> ReadBuffer(buf, (byte*) ptr);

    QxrdDetectorSettingsPtr det(m_Detector);

    if (det) {
      det->enqueueAcquiredFrame(image);
    }
  }
}

bool QxrdDexelaDriver::stopDetectorDriver()
{
  THREAD_CHECK;

  QxrdDetectorSettingsPtr det(m_Detector);

  if (det) {
    printMessage(tr("Stopping Dexela detector \"%1\"").arg(det->get_DetectorName()));
  }

  return true;
}

static int g_FrameCounter = 0;

bool QxrdDexelaDriver::changeExposureTime(double expos)
{
  THREAD_CHECK;

  QxrdDetectorSettingsPtr det(m_Detector);

  if (det && det->isEnabled()) {
    printMessage(tr("Exposure time changed to %1").arg(expos));

    if (m_DexelaDetector) {
      m_DexelaDetector -> SetExposureTime(expos);
    }

    return true;
  }

  return false;
}

bool QxrdDexelaDriver::beginAcquisition(double /*exposure*/)
{
  THREAD_CHECK;

  g_FrameCounter = 0;

  return true;
}

void QxrdDexelaDriver::beginFrame()
{
}

bool QxrdDexelaDriver::endAcquisition()
{
  THREAD_CHECK;

  return true;
}

bool QxrdDexelaDriver::shutdownAcquisition()
{
  THREAD_CHECK;

  return true;
}

//void QxrdDexelaDriver::onTimerTimeout()
//{
//  QxrdDetectorSettingsPtr det(m_Detector);
//  QxrdAcqCommonPtr        acq(m_Acquisition);

//  if (acq && det && det->checkDetectorEnabled()) {
//    QxrdSynchronizedAcquisitionPtr sacq(acq->synchronizedAcquisition());

//    if (sacq) {
//      sacq->acquiredFrameAvailable(g_FrameCounter);
//    }

//    int nRows = det -> get_NRows();
//    int nCols = det -> get_NCols();

//    int xpmsec = (int)(acq->get_ExposureTime()*1000+0.5);
//    int frame = g_FrameCounter % 8;

//    QcepUInt16ImageDataPtr image = QcepAllocator::newInt16Image(sharedFromThis(),
//                                                                tr("simdet-%1").arg(frame),
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
//        painter.drawText(0, labelHeight, tr("%1").arg(g_FrameCounter));

//        QRgb    *rgb = (QRgb*) imageLabel.bits();
//        int nFrames = nRows / labelHeight;
//        int frameN = g_FrameCounter % nFrames;
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

//    if (qcepDebug(DEBUG_DETECTORIDLING)) {
//      printMessage("enqueue dexela acquired frame");
//    }

//    det->enqueueAcquiredFrame(image);

//    g_FrameCounter++;
//  }
//}
