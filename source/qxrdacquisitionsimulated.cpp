#include "qxrdacquisitionsimulated.h"
#include "qxrdimagedata.h"
#include "qxrddataprocessor.h"

#include <QDir>
#include <QThread>
#include <QTime>
#include <QImage>
#include <QPainter>
#include <QFont>
#include <QComboBox>

QxrdAcquisitionSimulated::QxrdAcquisitionSimulated(QxrdDataProcessorPtr proc, QxrdAllocatorPtr allocator)
  : QxrdAcquisition(proc, allocator)
{
  connect(&m_Timer, SIGNAL(timeout()), this, SLOT(onTimerTimeout()));
}

void QxrdAcquisitionSimulated::onExposureTimeChanged(double newTime)
{
  emit printMessage(QDateTime::currentDateTime(),
                    tr("Exposure time changed to %1").arg(newTime));

  m_Timer.start(get_ExposureTime()*1000);
}

void QxrdAcquisitionSimulated::onBinningModeChanged(int newMode)
{
  emit printMessage(QDateTime::currentDateTime(),
                    tr("Binning mode changed to %1").arg(newMode));
}

void QxrdAcquisitionSimulated::onCameraGainChanged(int newGain)
{
  emit printMessage(QDateTime::currentDateTime(),
                    tr("Camera Gain Changed to %1").arg(newGain));
}

void QxrdAcquisitionSimulated::setupExposureMenu(QDoubleSpinBox * /*cb*/)
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

void QxrdAcquisitionSimulated::setupCameraGainMenu(QComboBox *cb)
{
  cb -> addItem(tr("High: 0.25 pF"));
  cb -> addItem(tr("0.5 pF"));
  cb -> addItem(tr("1 pF"));
  cb -> addItem(tr("2 pF"));
  cb -> addItem(tr("4 pF"));
  cb -> addItem(tr("Low: 8 pF"));
}

void QxrdAcquisitionSimulated::setupCameraBinningModeMenu(QComboBox *cb)
{
  cb -> addItem(tr("1x1 - 2048x2048 pixels"));
  cb -> addItem(tr("2x2 - 1024x1024 pixels"));
}

void QxrdAcquisitionSimulated::initialize()
{
  THREAD_CHECK;

  set_NRows(2048);
  set_NCols(2048);

  QxrdAcquisition::initialize();
}

void QxrdAcquisitionSimulated::onTimerTimeout()
{
//  QTime tic;
//  tic.start();

  static int frameCounter = 0;

  int nRows = get_NRows();
  int nCols = get_NCols();

//  QImage sampleImage(nCols, nRows, QImage::Format_RGB32);
//  QPainter painter(&sampleImage);
//
//  painter.fillRect(0,0,nCols,nRows, Qt::black);
//  painter.setPen(Qt::white);
//  painter.setFont(QFont("Times", 80, QFont::Bold, true));
//  painter.drawText(nCols/4, nRows/12, QDateTime::currentDateTime().toString("yyyyMMdd:hhmmss.zzz"));
//
//  emit printMessage(QDateTime::currentDateTime(), tr("simulated data generated after %1 msec").arg(tic.elapsed()));
//
//  painter.fillRect(0,0,nCols/4,nRows/4, Qt::lightGray);
//  painter.fillRect(1020,20,50,50, Qt::darkGray);
//
//  emit printMessage(QDateTime::currentDateTime(), tr("simulated data generated after %1 msec").arg(tic.elapsed()));
//
//  painter.setPen(Qt::gray);
//  painter.setBrush(QBrush(QColor(40,40,40,240),Qt::SolidPattern));
//  painter.drawEllipse(nCols/4, nRows/8, nCols/2, nRows/2);
//
//  emit printMessage(QDateTime::currentDateTime(), tr("simulated data generated after %1 msec").arg(tic.elapsed()));
//
//  if (m_AcquiredInt16Data) {
//    quint16 *ptr = m_AcquiredInt16Data->data();
//
//    QRgb    *rgb = (QRgb*) sampleImage.bits();
//    for (int j=0; j<nRows; j++) {
//      for (int i=0; i<nCols; i++) {
//        *ptr++ = *rgb++;
//      }
//    }
//  }

  if (m_AcquiredInt16Data) {
    quint16 *ptr = m_AcquiredInt16Data->data();
    int frame = (frameCounter++) % 8;

    for (int j=0; j<nRows; j++) {
      for (int i=0; i<nCols; i++) {
        if ((i>=frame*64) && (i<(frame+1)*64) && (j < 64)) {
          *ptr++ = frame;
        } else {
          *ptr++ = 0;
        }
      }
    }
  }
//  emit printMessage(QDateTime::currentDateTime(), tr("simulated data generation took %1 msec").arg(tic.elapsed()));

  acquiredFrameAvailable();
}
