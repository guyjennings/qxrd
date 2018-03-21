#include "qxrddebug.h"
#include "qxrdalliedvisiondriver.h"
#include "qxrdacqcommon.h"
#include "qxrdexperiment.h"
#include "qxrdsynchronizedacquisition.h"
#include "qcepallocator.h"
#include <QPainter>
#include "qxrdalliedvisionsettings.h"
#include "qxrdalliedvisionplugin.h"
#include <QThread>
#include "qcepimagedata.h"

QxrdAlliedVisionDriver::QxrdAlliedVisionDriver(QString name,
                                         QxrdAlliedVisionPluginWPtr plugin,
                                         QxrdAlliedVisionSettingsWPtr det,
                                         QxrdExperimentWPtr expt,
                                         QxrdAcqCommonWPtr acq)
  : QxrdDetectorDriver(name, det, expt, acq),
    m_Plugin(plugin),
    m_AlliedVision(det)
{
#ifndef QT_NO_DEBUG
  printf("Allied Vision Driver \"%s\" Constructed\n", qPrintable(name));
#endif

  connect(&m_Timer, &QTimer::timeout, this, &QxrdAlliedVisionDriver::onTimerTimeout);
}

QxrdAlliedVisionDriver::~QxrdAlliedVisionDriver()
{
#ifndef QT_NO_DEBUG
  printf("Allied Vision Driver \"%s\" Destroyed\n", qPrintable(get_Name()));
#endif
}

void QxrdAlliedVisionDriver::startDetectorDriver()
{
  THREAD_CHECK;

  QxrdDetectorSettingsPtr det(m_Detector);
  QxrdAcqCommonPtr        acq(m_Acquisition);

  if (acq && det && det->checkDetectorEnabled()) {
    splashMessage(tr("Starting Allied Vision Detector \"%1\"").arg(det->get_DetectorName()));

    det -> set_NRows(2048);
    det -> set_NCols(2048);

    changeExposureTime(acq->get_ExposureTime());
  }
}

void QxrdAlliedVisionDriver::stopDetectorDriver()
{
  THREAD_CHECK;

  QxrdDetectorSettingsPtr det(m_Detector);

  if (det) {
    splashMessage(tr("Stopping Allied Vision Detector \"%1\"").arg(det->get_DetectorName()));
  }

  m_Timer.stop();
}

void QxrdAlliedVisionDriver::changeExposureTime(double expos)
{
  THREAD_CHECK;

  QxrdDetectorSettingsPtr det(m_Detector);

  if (det && det->isEnabled()) {
    m_ExposureFactor = det->get_ExposureFactor();
    m_ExposureTime   = expos/m_ExposureFactor;

    printMessage(tr("Exposure time changed to %1").arg(m_ExposureTime));

    m_Timer.start(m_ExposureTime*1000);
  }
}

void QxrdAlliedVisionDriver::beginAcquisition(double /*exposure*/)
{
  THREAD_CHECK;

  m_FrameCounter = 0;
}

void QxrdAlliedVisionDriver::beginFrame()
{
  THREAD_CHECK;
}

void QxrdAlliedVisionDriver::endAcquisition()
{
  THREAD_CHECK;
}

void QxrdAlliedVisionDriver::shutdownAcquisition()
{
  THREAD_CHECK;

  m_Timer.stop();
}

void QxrdAlliedVisionDriver::onTimerTimeout()
{
  QxrdDetectorSettingsPtr det(m_Detector);
  QxrdAcqCommonPtr        acq(m_Acquisition);

  if (acq && det && det->checkDetectorEnabled()) {
    acq -> appendEvent(QxrdAcqCommon::DetectorFrameEvent,
                       det->get_DetectorIndex());

//    QxrdSynchronizedAcquisitionPtr sacq(acq->synchronizedAcquisition());

//    if (sacq) {
//      sacq->acquiredFrameAvailable(m_FrameCounter);
//    }

    int nRows = det -> get_NRows();
    int nCols = det -> get_NCols();

    int xpmsec = (int)(acq->get_ExposureTime()*1000+0.5);
    int frame = m_FrameCounter % 8;

    QcepUInt16ImageDataPtr image =
        QcepAllocator::newInt16Image(sharedFromThis(),
                                     tr("allied-%1").arg(frame),
                                     nCols, nRows,
                                     QcepAllocator::AllocateFromReserve);


    if (image) {
      quint16 *ptr = image->data();

      for (int j=0; j<nRows; j++) {
        for (int i=0; i<nCols; i++) {
          if ((i>=frame*64) && (i<(frame+1)*64) && (j < 64)) {
            *ptr++ = frame;
          } else {
            *ptr++ = xpmsec;
          }
        }
      }

      if ((nRows > 1024) && (nCols > 1024)) {
        const int labelWidth = 256;
        const int labelHeight = 64;

        QImage imageLabel(labelWidth, labelHeight, QImage::Format_RGB32);
        QPainter painter(&imageLabel);

        painter.fillRect(0,0,labelWidth,labelHeight, Qt::black);
        painter.setPen(Qt::white);
        painter.setFont(QFont("Times", labelHeight, QFont::Bold, true));
        painter.drawText(0, labelHeight, tr("%1").arg(m_FrameCounter));

        QRgb    *rgb = (QRgb*) imageLabel.bits();
        int nFrames = nRows / labelHeight;
        int frameN = m_FrameCounter % nFrames;
        int plval = qGray(*rgb);
        int pRgb  = *rgb;

        for (int j=0; j<labelHeight; j++) {
          for (int i=0; i<labelWidth; i++) {
            int x = nCols-labelWidth+i;
            int y = (frameN+1)*labelHeight-j;
            //          int val = image->value(x,y);
            int vRgb = *rgb++;
            int lval = qGray(vRgb);

            if (lval != plval) {
              plval = lval;
            }

            if (vRgb != pRgb) {
              pRgb = vRgb;
            }

            image->setValue(x,y,lval);
          }
        }
      }
//    } else {
//      printMessage("Allied Vision Detector Image == NULL");
    }

    if (qcepDebug(DEBUG_DETECTORIDLING)) {
      printMessage("enqueue Allied Vision detector acquired frame");
    }

    image -> set_ExposureTime(m_ExposureTime);
    image -> set_SummedExposures(1);

    if (m_ExposureFactor > 1) {
      if (m_SubframeCounter == 0) {
        m_AccumulatedData =
            QcepAllocator::newInt32Image(sharedFromThis(),
                                         tr("allied-%1").arg(frame),
                                         nCols, nRows,
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
                       det -> get_DetectorIndex());
  }
}
