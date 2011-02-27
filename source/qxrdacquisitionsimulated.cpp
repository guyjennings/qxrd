#include "qxrdacquisitionsimulated.h"
#include "qxrdimagedata.h"
#include "qxrddataprocessor.h"
#include "qxrdallocator.h"
#include "qxrdsynchronizedacquisition.h"

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
  if (synchronizedAcquisition()) {
    synchronizedAcquisition()->acquiredFrameAvailable(m_CurrentPhase);
  }

  static int frameCounter = 0;

  int nRows = get_NRows();
  int nCols = get_NCols();

  QxrdInt16ImageDataPtr image = m_Allocator->newInt16Image();

  if (image) {
    quint16 *ptr = image->data();
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

  acquiredFrameAvailable(image);
}
