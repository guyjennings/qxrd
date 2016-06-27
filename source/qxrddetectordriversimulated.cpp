#include "qxrddetectordriversimulated.h"
#include "qxrddetectorsettingssimulated.h"
#include "qxrdacquisition.h"
#include "qcepallocator.h"
#include <QImage>
#include <QPainter>

QxrdDetectorDriverSimulated::QxrdDetectorDriverSimulated(
    QString name,
    QxrdDetectorSettingsSimulatedPtr sim,
    QxrdExperimentPtr expt,
    QxrdAcquisitionPtr acq) :
  QxrdDetectorDriver(name, sim, expt, acq),
  m_Simulated(sim)
{
#ifndef QT_NO_DEBUG
  printf("Simulated Detector Driver \"%s\" Constructed\n", qPrintable(name));
#endif

  connect(&m_Timer, &QTimer::timeout, this, &QxrdDetectorDriverSimulated::onTimerTimeout);
}

QxrdDetectorDriverSimulated::~QxrdDetectorDriverSimulated()
{
#ifndef QT_NO_DEBUG
  printf("Simulated Detector Driver \"%s\" Destroyed\n", qPrintable(get_Name()));
#endif
}

bool QxrdDetectorDriverSimulated::startDetectorDriver()
{
  THREAD_CHECK;

  QxrdDetectorSettingsPtr det(m_Detector);
  QxrdAcquisitionPtr      acq(m_Acquisition);

  if (acq && det && det->checkDetectorEnabled()) {
    printMessage(tr("Starting simulated detector \"%1\"").arg(det->get_DetectorName()));

    det -> set_NRows(2048);
    det -> set_NCols(2048);

    return changeExposureTime(acq->get_ExposureTime());
  } else {
    return false;
  }
}

bool QxrdDetectorDriverSimulated::stopDetectorDriver()
{
  THREAD_CHECK;

  QxrdDetectorSettingsPtr det(m_Detector);

  if (det) {
    printMessage(tr("Stopping simulated detector \"%1\"").arg(det->get_DetectorName()));
  }

  m_Timer.stop();

  return true;
}

static int g_FrameCounter = 0;

bool QxrdDetectorDriverSimulated::changeExposureTime(double expos)
{
  THREAD_CHECK;

  QxrdDetectorSettingsPtr det(m_Detector);

  if (det && det->isEnabled()) {
    printMessage(tr("Exposure time changed to %1").arg(expos));

    m_Timer.start(expos*1000);

    return true;
  }

  return false;
}

bool QxrdDetectorDriverSimulated::beginAcquisition(double exposure)
{
  THREAD_CHECK;

  g_FrameCounter = 0;

  return true;
}

void QxrdDetectorDriverSimulated::beginFrame()
{
}

bool QxrdDetectorDriverSimulated::endAcquisition()
{
  THREAD_CHECK;

  return true;
}

bool QxrdDetectorDriverSimulated::shutdownAcquisition()
{
  THREAD_CHECK;

  m_Timer.stop();

  return true;
}

void QxrdDetectorDriverSimulated::onTimerTimeout()
{
  QxrdDetectorSettingsPtr det(m_Detector);
  QxrdAcquisitionPtr      acq(m_Acquisition);

  if (acq && det && det->checkDetectorEnabled()) {
    QxrdDetectorControlWindowPtr   win(det->detectorControlWindow());

    if (win) {
      QxrdSynchronizedAcquisitionPtr sacq(acq->synchronizedAcquisition());

      if (sacq) {
        sacq->acquiredFrameAvailable(g_FrameCounter);
      }

      int nRows = det -> get_NRows();
      int nCols = det -> get_NCols();

      int xpmsec = (int)(acq->get_ExposureTime()*1000+0.5);
      int frame = g_FrameCounter % 8;

      QcepUInt16ImageDataPtr image = QcepAllocator::newInt16Image(tr("simdet-%1").arg(frame),
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

      det->enqueueAcquiredFrame(image);

      g_FrameCounter++;
    }
  }
}
