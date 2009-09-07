/******************************************************************
*
*  $Id: qxrdacquisitionsimulated.cpp,v 1.16 2009/09/07 21:44:00 jennings Exp $
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

QxrdAcquisitionSimulated::QxrdAcquisitionSimulated(QxrdDataProcessor *proc)
  : QxrdAcquisitionOperations(proc),
    SOURCE_IDENT("$Id: qxrdacquisitionsimulated.cpp,v 1.16 2009/09/07 21:44:00 jennings Exp $")
{
}

void QxrdAcquisitionSimulated::simulatedInitialize()
{
  THREAD_CHECK;

  set_NRows(2048);
  set_NCols(2048);

  for (int i=0; i<10; i++) {
    m_DataProcessor -> returnImageToPool(new QxrdDoubleImageData(get_NCols(), get_NRows()));
  }
}

void QxrdAcquisitionSimulated::simulatedAcquisition(int isDark)
{
  THREAD_CHECK;

  set_AcquireDark(isDark);
  set_Cancelling(0);

  if (get_AcquireDark()) {
    set_ExposuresToSum(get_DarkSummedExposures());
    set_FilesInAcquiredSequence(1);
  } else {
    set_ExposuresToSum(get_SummedExposures());
    set_FilesInAcquiredSequence(get_PostTriggerFiles());
  }

  if (get_FilesInAcquiredSequence ()<= 0) {
    set_FilesInAcquiredSequence(1);
  }

  int n = get_PostTriggerFiles();

  for (int i=0; i<n; i++) {
    QTime tic;

    tic.start();

    emit printMessage(tr("Started acquisition %1").arg(i));

    if (get_Cancelling()) {
      set_Cancelling(false);
      return /*true*/;
    }

    QxrdInt16ImageData *acquiredData = new QxrdInt16ImageData(get_NCols(), get_NRows());

    acquiredData -> resize(get_NCols(), get_NRows());
    acquiredData -> clear();

    emit printMessage(tr("Cleared data after %1").arg(tic.restart()));

    int nRows = get_NRows();
    int nCols = get_NCols();

    if (false) {
      for (int j=0; j<nRows; j++) {
        for (int i=0; i<nCols; i++) {
          acquiredData -> setValue(i, j, i+j);
          //        double r=sqrt(pow(i-nCols/2,2)+pow(j-nRows/2,2));
          //
          //        if (r > 0) {
          //          acquiredData -> setValue(i, j, 1/r/*sin(r/100.0)/(r/100.0)*/);
          //        } else {
          //          acquiredData -> setValue(i, j, 1);
          //        }
        }
      }
    } else {
      QImage sampleImage(nCols, nRows, /*QImage::Format_Indexed8*/ QImage::Format_RGB32);
//      for (int i=0; i<256; i++) {
//        sampleImage.setColor(i, qRgb(i,i,i));
//      }

      QPainter painter(&sampleImage);

//      painter.setBackgroundMode(Qt::OpaqueMode);
      painter.setPen(Qt::white);
      painter.setFont(QFont("Times", 80, QFont::Bold, true));
      painter.drawText(nCols/4, nRows/12, QDateTime::currentDateTime().toString("yyyyMMdd:hhmmss.zzz"));

      painter.fillRect(0,0,nCols/4,nRows/4, Qt::lightGray);

      for (int j=0; j<(nRows/4); j++) {
        for (int i=0; i<(nCols/4); i++) {
          sampleImage.setPixel(i,j,qRgb(20,20,20));
        }
      }

      painter.fillRect(1020,20,50,50, Qt::darkGray);

      painter.setPen(Qt::gray);
      painter.setBrush(QBrush(QColor(40,40,40,240),Qt::SolidPattern));
//      painter.setBackground(Qt::white);
      painter.drawEllipse(nCols/4, nRows/8, nCols/2, nRows/2);

      QImage newImage = sampleImage.mirrored(false, true);

      for (int j=0; j<nRows; j++) {
        for (int i=0; i<nCols; i++) {
          acquiredData -> setValue(i,j, qGray(newImage.pixel/*Index*/(i,j)));
        }
      }
    }

    emit printMessage(tr("Simulated data after %1").arg(tic.restart()));

//    acquiredData -> showMaskAll();
    acquiredData -> calculateRange();

    emit printMessage(tr("Initialize mask and range after %1").arg(tic.restart()));

    QString fileName;
    QString fileBase;

    if (get_AcquireDark()) {
      fileBase = get_FilePattern()+tr("-%1.dark.tif").arg(get_FileIndex(),5,10,QChar('0'));
      fileName = QDir(m_DataProcessor -> get_OutputDirectory())
                 .filePath(get_FilePattern()+tr("-%1.dark.tif")
                           .arg(get_FileIndex(),5,10,QChar('0')));
    } else {
      fileBase = get_FilePattern()+tr("-%1.tif").arg(get_FileIndex(),5,10,QChar('0'));
      fileName = QDir(m_DataProcessor -> get_OutputDirectory())
                 .filePath(get_FilePattern()+tr("-%1.tif")
                           .arg(get_FileIndex(),5,10,QChar('0')));
    }

    set_FileBase(fileBase);
    m_DataProcessor -> set_FileName(fileName);

    emit acquiredFrame(fileName, get_FileIndex(),
                       0, get_ExposuresToSum(),
                       i, get_FilesInAcquiredSequence());

    QFileInfo finfo(fileName);

    acquiredData -> set_FileName(fileName);
    acquiredData -> set_Title(finfo.fileName());
    acquiredData -> set_ReadoutMode(get_ReadoutMode());
    acquiredData -> set_ExposureTime(get_ExposureTime());
    acquiredData -> set_SummedExposures(get_ExposuresToSum());

    if (get_AcquireDark()) {
      acquiredData -> set_ImageNumber(-1);
    } else {
      acquiredData -> set_ImageNumber(i);
    }

    m_DataProcessor -> incrementAcquiredCount();

    emit printMessage(tr("Simulated image available after %1").arg(tic.restart()));

    emit acquiredInt16ImageAvailable(acquiredData);

    set_FileIndex(get_FileIndex()+1);
  }

  haltAcquire();
}


/******************************************************************
*
*  $Log: qxrdacquisitionsimulated.cpp,v $
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
