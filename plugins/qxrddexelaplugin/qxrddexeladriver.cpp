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
#include "qcepimagedata.h"
#include "qcepmutexlocker.h"
#include "qcepallocator.h"
#include "qxrddexelaplugin.h"

QxrdDexelaDriver::QxrdDexelaDriver(QString name,
                                   QxrdDexelaPluginWPtr plugin,
                                   QxrdDexelaSettingsWPtr det,
                                   QxrdExperimentWPtr expt,
                                   QxrdAcqCommonWPtr acq)
  : QxrdDetectorDriver(name, det, expt, acq),
    m_Dexela(det),
    m_DexelaPlugin(plugin),
    m_DexelaDetector(NULL)
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

void QxrdDexelaDriver::startDetectorDriver()
{
  THREAD_CHECK;

  if (m_DexelaDetector) {
    stopDetectorDriver();
  }

  if (qcepDebug(DEBUG_DEXELA)) {
    printMessage(tr("QxrdDexelaDriver::startDetectorDriver"));
  }

  QxrdDexelaSettingsPtr det(m_Dexela);
  QxrdAcqCommonPtr      acq(m_Acquisition);

  if (det == NULL) {
    printMessage("Attempting to start Dexela Detector with settings == NULL");
  }

  if (acq == NULL) {
    printMessage("Attempting to start Dexela Detector with acquisition == NULL");
  }

  if (acq && det && det->checkDetectorEnabled()) {

    printMessage(tr("Starting Dexela detector %1: \"%2\"")
                 .arg(det->get_DetectorIndex())
                 .arg(det->get_DetectorName()));

    int index = det->get_DetectorIndex();

    QxrdDexelaPluginPtr plugin(m_DexelaPlugin);

    if (plugin && index >= 0 && index < plugin -> deviceCount()) {
      DevInfo info = plugin -> device(index);
      m_DexelaDetector = new DexelaDetector(info);

      if (m_DexelaDetector) {
        try {
          m_DexelaDetector -> OpenBoard();

          m_DexelaDetector -> SetCallback(&QxrdDexelaDriver::staticCallback);
          m_DexelaDetector -> SetCallbackData((void*) this);

          int xDim = m_DexelaDetector -> GetBufferXdim();
          int yDim = m_DexelaDetector -> GetBufferYdim();
          int nBuf = m_DexelaDetector -> GetNumBuffers();

          printMessage(tr("Detector dimensions %1 x %2, nBuffers %3").arg(xDim).arg(yDim).arg(nBuf));

          connect(acq -> prop_ExposureTime(), &QcepDoubleProperty::valueChanged,
                  this,                       &QxrdDexelaDriver::restartDetector);

          connect(det -> prop_ExposureFactor(), &QcepIntProperty::valueChanged,
                  this,                         &QxrdDexelaDriver::restartDetector);

          connect(det -> prop_HardwareSync(),   &QcepBoolProperty::valueChanged,
                  this,                         &QxrdDexelaDriver::restartDetector);

          connect(det -> prop_HBinning(),       &QcepIntProperty::valueChanged,
                  this,                         &QxrdDexelaDriver::restartDetector);

          connect(det -> prop_VBinning(),       &QcepIntProperty::valueChanged,
                  this,                         &QxrdDexelaDriver::restartDetector);

          changeExposureTime(acq->get_ExposureTime());

          restartDetector();
        } catch (DexelaException &e) {
          printMessage(tr("Dexela Exception caught: Description %1: function %2")
                       .arg(e.what()).arg(e.GetFunctionName()));
        }
      }
    }
  }

  if (qcepDebug(DEBUG_DEXELA)) {
    printMessage(tr("QxrdDexelaDriver::startDetectorDriver finished"));
  }
}

void QxrdDexelaDriver::restartDetector()
{
  THREAD_CHECK;

  if (qcepDebug(DEBUG_DEXELA)) {
    printMessage(tr("QxrdDexelaDriver::restartDetectorDriver"));
  }

  QxrdDexelaSettingsPtr det(m_Dexela);
  QxrdAcqCommonPtr      acq(m_Acquisition);

  if (det == NULL) {
    printMessage("Attempting to restart Dexela Detector with settings == NULL");
  }

  if (acq == NULL) {
    printMessage("Attempting to restart Dexela Detector with acquisition == NULL");
  }

  if (acq && det && det->checkDetectorEnabled()) {
    printMessage(tr("Restarting Dexela detector %1: \"%2\"")
                 .arg(det->get_DetectorIndex())
                 .arg(det->get_DetectorName()));

    if (m_DexelaDetector == NULL) {
      startDetectorDriver();
    } else {
      try {

        m_DexelaDetector -> SetFullWellMode(High);
        m_DexelaDetector -> SetBinningMode(x11);

        m_ExposureFactor = det->get_ExposureFactor();
        m_ExposureTime   = acq->get_ExposureTime() / m_ExposureFactor;

        switch (det->get_HardwareSync()) {
        case QxrdDexelaSettings::SoftwareSync:
          m_DexelaDetector -> SetExposureTime(m_ExposureTime);
          m_DexelaDetector -> SetExposureMode(Expose_and_read);
          m_DexelaDetector -> SetTriggerSource(Internal_Software);
          m_DexelaDetector -> EnablePulseGenerator(m_ExposureTime);
          break;

        case QxrdDexelaSettings::HardwareSync:
          m_DexelaDetector -> SetExposureMode (Expose_and_read);
          m_DexelaDetector -> SetTriggerSource(Ext_Duration_Trig);
          break;
        }

        det -> set_ReadoutTime(m_DexelaDetector -> GetReadOutTime());

        m_XDim = m_DexelaDetector -> GetBufferXdim();
        m_YDim = m_DexelaDetector -> GetBufferYdim();

        det -> set_NCols(m_XDim);
        det -> set_NRows(m_YDim);

        m_DexelaDetector -> GoLiveSeq();

        if (det->get_HardwareSync() == QxrdDexelaSettings::SoftwareSync) {
          m_DexelaDetector -> ToggleGenerator(true);
        }

        printMessage(tr("Dexela Detector Exposure time %1").arg(m_DexelaDetector->GetExposureTime()));
      } catch (DexelaException &e) {
        printMessage(tr("Dexela Exception caught: Description %1: function %2")
                     .arg(e.what()).arg(e.GetFunctionName()));
      }
    }
  }
}

void QxrdDexelaDriver::staticCallback(int fc, int buf, DexelaDetector *det)
{
//  printf("QxrdDexelaDriver::staticCallback called fc=%d, buf=%d, det=%x\n", fc, buf, det);
//  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QxrdDexelaDriver *dex = reinterpret_cast<QxrdDexelaDriver*>(det->GetCallbackData());

  if (dex) {
    INVOKE_CHECK(
          QMetaObject::invokeMethod(dex, "onAcquiredFrame", Qt::QueuedConnection, Q_ARG(int, fc), Q_ARG(int, buf)))
//    dex -> onAcquiredFrame(fc, buf);
  }
}

void QxrdDexelaDriver::onAcquiredFrame(int fc, int buf)
{
  THREAD_CHECK;

  QxrdDexelaSettingsPtr det(m_Dexela);
  QxrdAcqCommonPtr acq(m_Acquisition);

  if (acq && det && det->checkDetectorEnabled()) {
    acq -> appendEvent(QxrdAcqCommon::DetectorFrameEvent,
                       det->get_DetectorIndex());

//    QxrdSynchronizedAcquisitionPtr sacq(acq->synchronizedAcquisition());

//    if (sacq) {
//      sacq->acquiredFrameAvailable(m_FrameCounter);
//    }

    QcepUInt16ImageDataPtr image =
        QcepAllocator::newInt16Image(sharedFromThis(),
                                     tr("frame-%1").arg(fc),
                                     m_XDim, m_YDim,
                                     QcepAllocator::AllocateFromReserve);

    if (image) {
      //    QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

      quint16 *ptr = image->data();

      try {
        m_DexelaDetector -> ReadBuffer(buf, (byte*) ptr);
      } catch (DexelaException &e) {
        printMessage(tr("Dexela Exception caught: Description %1: function %2")
                     .arg(e.what()).arg(e.GetFunctionName()));
      }

      if (qcepDebug(DEBUG_DEXELA)) {
        printMessage(tr("Acquired Frame %1 from %2 on detector %3")
                     .arg(fc).arg(buf).arg(det->get_DetectorIndex()));
      }

      image -> set_ExposureTime(m_ExposureTime);
      image -> set_SummedExposures(1);

      if (m_ExposureFactor > 1) {
        if (m_SubframeCounter == 0) {
          m_AccumulatedData =
              QcepAllocator::newInt32Image(sharedFromThis(),
                                           tr("dexela-%1").arg(m_FrameCounter),
                                           m_XDim, m_YDim,
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

void QxrdDexelaDriver::stopDetectorDriver()
{
  THREAD_CHECK;

  QxrdDetectorSettingsPtr det(m_Detector);

  if (det) {
    printMessage(tr("Stopping Dexela detector \"%1\"").arg(det->get_DetectorName()));
  }

  if (m_DexelaDetector) {
    m_DexelaDetector -> CloseBoard();

    delete m_DexelaDetector;

    m_DexelaDetector = NULL;
  }
}

void QxrdDexelaDriver::changeExposureTime(double expos)
{
  THREAD_CHECK;

  QxrdDetectorSettingsPtr det(m_Detector);

  if (det && det->isEnabled()) {
    m_ExposureFactor = det->get_ExposureFactor();
    m_ExposureTime   = expos/m_ExposureFactor;

    printMessage(tr("Exposure time changed to %1").arg(m_ExposureTime));

    if (m_DexelaDetector) {
      try {
        m_DexelaDetector -> SetExposureTime(m_ExposureTime);
      } catch (DexelaException &e) {
        printMessage(tr("Dexela Exception caught: Description %1: function %2")
                     .arg(e.what()).arg(e.GetFunctionName()));
      }
    }
  }
}

void QxrdDexelaDriver::beginAcquisition(double /*exposure*/)
{
  THREAD_CHECK;

  m_FrameCounter = 0;
}

void QxrdDexelaDriver::beginFrame()
{
  THREAD_CHECK;
}

void QxrdDexelaDriver::endAcquisition()
{
  THREAD_CHECK;
}

void QxrdDexelaDriver::shutdownAcquisition()
{
  THREAD_CHECK;
}
