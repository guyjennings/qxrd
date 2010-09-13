/******************************************************************
*
*  $Id: qxrdacquisitionsimulated.cpp,v 1.2 2010/09/13 20:00:39 jennings Exp $
*
*******************************************************************/

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
  : QxrdAcquisition(proc, allocator),
    SOURCE_IDENT("$Id: qxrdacquisitionsimulated.cpp,v 1.2 2010/09/13 20:00:39 jennings Exp $")
{
  connect(&m_Timer, SIGNAL(timeout()), this, SLOT(onTimerTimeout()));
}

void QxrdAcquisitionSimulated::onExposureTimeChanged(double newTime)
{
  emit printMessage(tr("Exposure time changed to %1").arg(newTime));

  m_Timer.start(get_ExposureTime()*1000);
}

void QxrdAcquisitionSimulated::onBinningModeChanged(int newMode)
{
  emit printMessage(tr("Binning mode changed to %1").arg(newMode));
}

void QxrdAcquisitionSimulated::onCameraGainChanged(int newGain)
{
  emit printMessage(tr("Camera Gain Changed to %1").arg(newGain));
}

void QxrdAcquisitionSimulated::setupExposureMenu(QDoubleSpinBox *cb)
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
//  emit printMessage(tr("simulated data generated after %1 msec").arg(tic.elapsed()));
//
//  painter.fillRect(0,0,nCols/4,nRows/4, Qt::lightGray);
//  painter.fillRect(1020,20,50,50, Qt::darkGray);
//
//  emit printMessage(tr("simulated data generated after %1 msec").arg(tic.elapsed()));
//
//  painter.setPen(Qt::gray);
//  painter.setBrush(QBrush(QColor(40,40,40,240),Qt::SolidPattern));
//  painter.drawEllipse(nCols/4, nRows/8, nCols/2, nRows/2);
//
//  emit printMessage(tr("simulated data generated after %1 msec").arg(tic.elapsed()));
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
//  emit printMessage(tr("simulated data generation took %1 msec").arg(tic.elapsed()));

  acquiredFrameAvailable();
}

/******************************************************************
*
*  $Log: qxrdacquisitionsimulated.cpp,v $
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.2  2010/09/11 19:45:04  jennings
*  *** empty log message ***
*
*  Revision 1.1.2.1  2010/07/22 18:39:37  jennings
*  Moving files into source subdirectory
*
*  Revision 1.17.4.22  2010/06/23 21:50:19  jennings
*  Used a simplified image generator for simulated acquisition
*
*  Revision 1.17.4.21  2010/05/25 18:47:15  jennings
*  Added memory limit handling
*
*  Revision 1.17.4.20  2010/05/24 21:02:37  jennings
*  Moved all image data allocation into allocator object
*  Added partial handling for insufficient memory available when allocating data
*  Reordered program initialization so that allocator and file saver are created first
*
*  Revision 1.17.4.19  2010/05/21 20:04:42  jennings
*  Detector and Processor type names and numbers are intialized by the corresponding thread object
*
*  Revision 1.17.4.18  2010/05/21 18:51:22  jennings
*  *** empty log message ***
*
*  Revision 1.17.4.17  2010/05/19 20:37:31  jennings
*  ComboBox -> SpinBox
*
*  Revision 1.17.4.16  2010/05/19 01:12:14  jennings
*  Tidied up PE acquisition code
*
*  Revision 1.17.4.15  2010/05/18 05:40:43  jennings
*  Optimize simulated acquisition
*
*  Revision 1.17.4.14  2010/05/18 03:49:26  jennings
*  Corrected initialization problem with simulated acquisition
*
*  Revision 1.17.4.13  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.17.4.12  2010/04/22 21:53:00  jennings
*  Simulated acquisition is beginning to work
*
*  Revision 1.17.4.11  2010/04/22 21:42:07  jennings
*  Simulated acquisition is beginning to work
*
*  Revision 1.17.4.10  2010/04/22 19:40:33  jennings
*  Further rearrangement of acquisition code
*
*  Revision 1.17.4.9  2010/04/21 20:51:21  jennings
*  Added detectorType and detectorTypeName properties to each detector type
*
*  Revision 1.17.4.8  2010/04/21 16:57:04  jennings
*  Don't start QxrdAcquisitionSimulated timer when acquisition object is first constructed - at that time the
*  exposure time parameter will not yet have been set.
*
*  Revision 1.17.4.7  2010/04/20 21:19:48  jennings
*  Added 'onExposureTimeChanged' etc. to simulated acquisition
*
*  Revision 1.17.4.6  2010/04/19 21:48:27  jennings
*  More rearrangement
*
*  Revision 1.17.4.5  2010/04/19 19:20:16  jennings
*  More reorganization of acquisition code
*
*  Revision 1.17.4.4  2010/04/18 17:20:48  jennings
*  Further updates
*
*  Revision 1.17.4.3  2010/04/17 05:13:58  jennings
*  Partially working
*
*  Revision 1.17.4.2  2010/04/15 19:34:24  jennings
*  Reorganization of acquisition object classes
*
*  Revision 1.17.4.1  2010/04/12 21:21:46  jennings
*  Version 0.4.0, some rearrangement of the Acquisition classes
*
*  Revision 1.17  2009/09/22 20:37:23  jennings
*  Dynamic properties attached to the acquisition object are propagated through to saved images
*
*  Revision 1.16  2009/09/07 21:44:00  jennings
*  imulated acquisition
*
*  Revision 1.15  2009/09/04 15:15:42  jennings
*  Added log file routines
*  Removed newlines from any printMessage calls.
*
*  Revision 1.14  2009/09/03 21:16:24  jennings
*  Added properties and user interface elements for pre- and post- trigger counts
*  Added properties and user interface elements for fine-grained control of processing chain
*
*  Revision 1.13  2009/08/25 18:43:03  jennings
*  Templatized QxrdImageData and QxrdImageQueue, and added int16, int32 and double variants as typedefs
*
*  Revision 1.12  2009/08/08 20:09:45  jennings
*  Added timestamp to simulated images
*
*  Revision 1.11  2009/08/04 16:45:20  jennings
*  Moved mask data into separate class
*
*  Revision 1.10  2009/08/03 20:58:59  jennings
*  Minor fixups
*
*  Revision 1.9  2009/08/02 18:01:16  jennings
*  Draw stuff into simulated images with a QPainter
*
*  Revision 1.8  2009/07/20 00:33:02  jennings
*  Simulated data calculation simplified
*
*  Revision 1.7  2009/07/17 21:10:39  jennings
*  Modifications related to mask display
*
*  Revision 1.6  2009/07/17 19:25:57  jennings
*  Calculate range of data in simulated image
*
*  Revision 1.5  2009/07/17 14:00:59  jennings
*  Rearranging acquisition and data processor
*
*  Revision 1.4  2009/07/16 22:06:00  jennings
*  Made various image display variables into properties
*
*  Revision 1.3  2009/07/16 20:09:55  jennings
*  Set values in simulated data images
*
*  Revision 1.2  2009/07/14 20:07:00  jennings
*  Implemented simple simulated acquisition
*
*  Revision 1.1  2009/07/13 23:19:37  jennings
*  More acquisition rearrangement
*
*
*******************************************************************/
