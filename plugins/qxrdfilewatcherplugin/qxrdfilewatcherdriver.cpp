#include "qxrddebug.h"
#include "qxrdfilewatcherdriver.h"
#include "qxrdacqcommon.h"
#include "qxrdexperiment.h"
#include "qxrdsynchronizedacquisition.h"
#include "qcepallocator.h"
#include <QPainter>
#include "qxrdfilewatchersettings.h"
#include <QThread>

QxrdFileWatcherDriver::QxrdFileWatcherDriver(QString name, QxrdFileWatcherPluginWPtr plugin,
                                                         QxrdDetectorSettingsWPtr det,
                                                         QxrdExperimentWPtr expt,
                                                         QxrdAcqCommonWPtr acq)
  : QxrdDetectorDriver(name, det, expt, acq),
    m_FileWatcher(qSharedPointerDynamicCast<QxrdFileWatcherSettings>(det))
{
#ifndef QT_NO_DEBUG
  printf("File Watcher Driver \"%s\" Constructed\n", qPrintable(name));
#endif

  connect(&m_Timer, &QTimer::timeout, this, &QxrdFileWatcherDriver::onTimerTimeout);
}

QxrdFileWatcherDriver::~QxrdFileWatcherDriver()
{
#ifndef QT_NO_DEBUG
  printf("File Watcher Driver \"%s\" Destroyed\n", qPrintable(get_Name()));
#endif
}

void QxrdFileWatcherDriver::startDetectorDriver()
{
  THREAD_CHECK;

  QxrdDetectorSettingsPtr det(m_Detector);
  QxrdAcqCommonPtr        acq(m_Acquisition);

  if (acq && det && det->checkDetectorEnabled()) {
    printMessage(tr("Starting File Watcher detector \"%1\"").arg(det->get_DetectorName()));

    det -> set_NRows(2048);
    det -> set_NCols(2048);

    changeExposureTime(acq->get_ExposureTime());
  }
}

void QxrdFileWatcherDriver::stopDetectorDriver()
{
  THREAD_CHECK;

  QxrdDetectorSettingsPtr det(m_Detector);

  if (det) {
    printMessage(tr("Stopping File Watcher detector \"%1\"").arg(det->get_DetectorName()));
  }

  m_Timer.stop();
}

static int g_FrameCounter = 0;

void QxrdFileWatcherDriver::changeExposureTime(double expos)
{
  THREAD_CHECK;

  QxrdDetectorSettingsPtr det(m_Detector);

  if (det && det->isEnabled()) {
    printMessage(tr("Exposure time changed to %1").arg(expos));

    m_Timer.start(expos*1000);
  }
}

void QxrdFileWatcherDriver::beginAcquisition(double /*exposure*/)
{
  THREAD_CHECK;

  g_FrameCounter = 0;
}

void QxrdFileWatcherDriver::beginFrame()
{
  THREAD_CHECK;
}

void QxrdFileWatcherDriver::endAcquisition()
{
  THREAD_CHECK;
}

void QxrdFileWatcherDriver::shutdownAcquisition()
{
  THREAD_CHECK;

  m_Timer.stop();
}

void QxrdFileWatcherDriver::onTimerTimeout()
{
  QxrdDetectorSettingsPtr det(m_Detector);
  QxrdAcqCommonPtr        acq(m_Acquisition);

  if (acq && det && det->checkDetectorEnabled()) {
    QxrdSynchronizedAcquisitionPtr sacq(acq->synchronizedAcquisition());

    if (sacq) {
      sacq->acquiredFrameAvailable(g_FrameCounter);
    }

    int nRows = det -> get_NRows();
    int nCols = det -> get_NCols();

    int xpmsec = (int)(acq->get_ExposureTime()*1000+0.5);
    int frame = g_FrameCounter % 8;

    QcepUInt16ImageDataPtr image = QcepAllocator::newInt16Image(sharedFromThis(),
                                                                tr("filewatcher-%1").arg(frame),
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
        painter.drawText(0, labelHeight, tr("%1").arg(g_FrameCounter));

        QRgb    *rgb = (QRgb*) imageLabel.bits();
        int nFrames = nRows / labelHeight;
        int frameN = g_FrameCounter % nFrames;
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
    }

    if (qcepDebug(DEBUG_DETECTORIDLING)) {
      printMessage("enqueue file watcher acquired frame");
    }

    det->enqueueAcquiredFrame(image);

    g_FrameCounter++;
  }
}
