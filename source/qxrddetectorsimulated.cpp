#include "qxrddetectorsimulated.h"
#include "qxrdexperiment.h"
#include "qxrdacquisition.h"
#include "qxrdallocator.h"

#include <QPainter>

QxrdDetectorSimulated::QxrdDetectorSimulated(QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq) :
  QxrdDetector(expt, acq)
{
  connect(&m_Timer, SIGNAL(timeout()), this, SLOT(onTimerTimeout()));
}

int QxrdDetectorSimulated::detectorType() const
{
  return QxrdDetectorThread::SimulatedDetector;
}

void QxrdDetectorSimulated::onExposureTimeChanged()
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "onExposureTimeChanged");
  } else {
    QxrdExperimentPtr expt(m_Experiment);
    QxrdAcquisitionPtr acq(m_Acquisition);

    if (acq && expt) {
      double newTime = acq->get_ExposureTime();

      expt->printMessage(tr("Exposure time changed to %1").arg(newTime));

      m_Timer.start(newTime*1000);
    }
  }
}

void QxrdDetectorSimulated::setupExposureMenu(QDoubleSpinBox * /*cb*/)
{
//  cb -> addItem(tr("0.067"));
//  cb -> addItem(tr("0.1"));
//  cb -> addItem(tr("0.2"));
//  cb -> addItem(tr("0.5"));
//  cb -> addItem(tr("1"));
//  cb -> addItem(tr("2"));
//  cb -> addItem(tr("5"));
//  cb -> addItem(tr("7"));
//
//  cb -> setValidator(new QDoubleValidator(0.0667,8,3,cb));
}

void QxrdDetectorSimulated::setupCameraGainMenu(QComboBox *cb)
{
  for (int i=0; i<16; i++) {
    QString msg;

    if (i==0) {
      msg = "High: ";
    } else if (i==15) {
      msg = "Low: ";
    }

    double value = 0.1;
    if (i & 1) {
      value += 0.3;
    }

    if (i & 2) {
      value += 0.9;
    }

    if (i & 4) {
      value += 4.7;
    }

    if (i & 8) {
      value += 10.0;
    }

    msg += tr("%1 pF").arg(value);

    cb -> addItem(msg);
  }
//  cb -> addItem(tr("High: 0.25 pF"));
//  cb -> addItem(tr("0.5 pF"));
//  cb -> addItem(tr("1 pF"));
//  cb -> addItem(tr("2 pF"));
//  cb -> addItem(tr("4 pF"));
//  cb -> addItem(tr("Low: 8 pF"));
}

void QxrdDetectorSimulated::setupCameraBinningModeMenu(QComboBox *cb)
{
  cb -> addItem(tr("1x1 - 2048x2048 pixels"));
  cb -> addItem(tr("2x2 - 1024x1024 pixels"));
}

void QxrdDetectorSimulated::initialize()
{
  THREAD_CHECK;

  QxrdAcquisitionPtr acq(m_Acquisition);

  if (acq) {
    acq->set_NRows(2048);
    acq->set_NCols(2048);

    if (acq->get_ExposureTime() <= 0) {
      acq->set_ExposureTime(0.1);
    }

    onExposureTimeChanged();
  }
}

static int frameCounter = 0;

void QxrdDetectorSimulated::beginAcquisition()
{
  frameCounter = 0;
}

void QxrdDetectorSimulated::endAcquisition()
{
}

void QxrdDetectorSimulated::shutdownAcquisition()
{
}

void QxrdDetectorSimulated::onTimerTimeout()
{
  QxrdAcquisitionPtr acq(m_Acquisition);

  if (acq) {
    if (acq->synchronizedAcquisition()) {
      acq->synchronizedAcquisition()->acquiredFrameAvailable(frameCounter);
    }

    int nRows = acq->get_NRows();
    int nCols = acq->get_NCols();

    QxrdInt16ImageDataPtr image = QxrdAllocator::newInt16Image(acq->allocator(),
                                                               QxrdAllocator::AllocateFromReserve,
                                                               nCols, nRows);
    int xpmsec = (int)(acq->get_ExposureTime()*1000+0.5);
    int frame = frameCounter % 8;

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

    acq->enqueueAcquiredFrame(image);

    frameCounter++;
  }
}
