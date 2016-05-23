#include "qxrddetectorsettingssimulated.h"
#include "qxrdexperiment.h"
#include "qxrdacquisition.h"
#include "qcepallocator.h"
#include "qxrddebug.h"

#include <stdio.h>
#include <QPainter>

QxrdDetectorSettingsSimulated::QxrdDetectorSettingsSimulated(QxrdExperimentWPtr    expt,
                                             QxrdAcquisitionWPtr   acq,
                                             int                   detNum) :
  QxrdDetectorSettings(expt, acq, SimulatedDetector, detNum)
{
#ifndef QT_NO_DEBUG
  printf("Constructing simulated detector\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetectorSimulated::QxrdDetectorSimulated(%p)\n", this);
  }

  connect(&m_Timer, &QTimer::timeout, this, &QxrdDetectorSettingsSimulated::onTimerTimeout);
}

QxrdDetectorSettingsSimulated::~QxrdDetectorSettingsSimulated()
{
#ifndef QT_NO_DEBUG
  printf("Deleting simulated detector\n");
#endif

  if (QThread::currentThread() != thread()) {
    printf("Deleting simulated detector from wrong thread\n");
  }

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetectorSimulated::~QxrdDetectorSimulated(%p)\n", this);
  }
}

void QxrdDetectorSettingsSimulated::pushDefaultsToProxy(QxrdDetectorProxyPtr proxy)
{
//  QxrdDetectorSettings::pushDefaultsToProxy(proxy, SimulatedDetector);

//  printf("Need to implement QxrdDetectorSimulated::pushDefaultsToProxy\n");
}

void QxrdDetectorSettingsSimulated::pushPropertiesToProxy(QxrdDetectorProxyPtr proxy)
{
  QxrdDetectorSettings::pushPropertiesToProxy(proxy);

//  printf("Need to implement QxrdDetectorSimulated::pushPropertiesToProxy\n");
}

void QxrdDetectorSettingsSimulated::pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy)
{
  QxrdDetectorSettings::pullPropertiesfromProxy(proxy);

//  printf("Need to implement QxrdDetectorSimulated::pullPropertiesfromProxy\n");
}

void QxrdDetectorSettingsSimulated::onExposureTimeChanged()
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "onExposureTimeChanged");
  } else {
    QxrdExperimentPtr expt(m_Experiment);
    QxrdAcquisitionPtr acq(m_Acquisition);

    if (isEnabled() && acq && expt) {
      double newTime = acq->get_ExposureTime();

      expt->printMessage(tr("Exposure time changed to %1").arg(newTime));

      m_Timer.start(newTime*1000);
    }
  }
}

void QxrdDetectorSettingsSimulated::startDetector()
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "startDetector", Qt::BlockingQueuedConnection);
  } else {
    QxrdDetectorSettings::startDetector();

    QxrdAcquisitionPtr acq(m_Acquisition);

    if (checkDetectorEnabled() && acq) {
      printMessage(tr("Starting simulated detector \"%1\"").arg(get_DetectorName()));

      set_NRows(2048);
      set_NCols(2048);

      if (acq->get_ExposureTime() <= 0) {
        acq->set_ExposureTime(0.1);
      }

      onExposureTimeChanged();
    }
  }
}

void QxrdDetectorSettingsSimulated::stopDetector()
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "stopDetector", Qt::BlockingQueuedConnection);
  } else {
    printMessage(tr("Stopping simulated detector \"%1\"").arg(get_DetectorName()));

    QxrdDetectorSettings::stopDetector();

    m_Timer.stop();
  }
}

static int frameCounter = 0;

void QxrdDetectorSettingsSimulated::beginAcquisition(double exposure)
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "beginAcquisition", Qt::BlockingQueuedConnection, Q_ARG(double, exposure));
  } else {
    QxrdDetectorSettings::beginAcquisition(exposure);

    frameCounter = 0;
  }
}

void QxrdDetectorSettingsSimulated::endAcquisition()
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "endAcquisition", Qt::BlockingQueuedConnection);
  } else {
    QxrdDetectorSettings::endAcquisition();
  }
}

void QxrdDetectorSettingsSimulated::shutdownAcquisition()
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "shutdownAcquisition", Qt::BlockingQueuedConnection);
  } else {
    QxrdDetectorSettings::shutdownAcquisition();
  }
}

void QxrdDetectorSettingsSimulated::onTimerTimeout()
{
  QxrdAcquisitionPtr acq(m_Acquisition);

  if (checkDetectorEnabled() && acq) {
    if (acq->synchronizedAcquisition()) {
      acq->synchronizedAcquisition()->acquiredFrameAvailable(frameCounter);
    }

    int nRows = get_NRows();
    int nCols = get_NCols();

    int xpmsec = (int)(acq->get_ExposureTime()*1000+0.5);
    int frame = frameCounter % 8;

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
        painter.drawText(0, labelHeight, tr("%1").arg(frameCounter));

        QRgb    *rgb = (QRgb*) imageLabel.bits();
        int nFrames = nRows / labelHeight;
        int frameN = frameCounter % nFrames;
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

    enqueueAcquiredFrame(image);

    frameCounter++;
  }
}
