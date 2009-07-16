/******************************************************************
*
*  $Id: qxrdacquisitionsimulated.cpp,v 1.3 2009/07/16 20:09:55 jennings Exp $
*
*******************************************************************/

#include "qxrdacquisitionsimulated.h"
#include "qxrdimagedata.h"

#include <QDir>
#include <QThread>

QxrdAcquisitionSimulated::QxrdAcquisitionSimulated(QxrdDataProcessor *proc)
  : QxrdAcquisitionOperations(proc),
    SOURCE_IDENT("$Id: qxrdacquisitionsimulated.cpp,v 1.3 2009/07/16 20:09:55 jennings Exp $")
{
}

void QxrdAcquisitionSimulated::simulatedInitialize()
{
  THREAD_CHECK;

  set_NRows(2048);
  set_NCols(2048);

  for (int i=0; i<10; i++) {
    returnImageToPool(new QxrdImageData(get_NCols(), get_NRows()));
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
    set_FilesInAcquiredSequence(get_FilesInSequence());
  }

  if (get_FilesInAcquiredSequence ()<= 0) {
    set_FilesInAcquiredSequence(1);
  }

  int n = get_FilesInSequence();

  for (int i=0; i<n; i++) {
    if (get_Cancelling()) {
      set_Cancelling(false);
      return /*true*/;
    }

    QxrdImageData *acquiredData = takeNextFreeImage();

    acquiredData -> resize(get_NCols(), get_NRows());
    acquiredData -> clear();

    int nRows = get_NRows();
    int nCols = get_NCols();

    for (int j=0; j<nCols; j++) {
      for (int i=0; i<nRows; i++) {
        double r=sqrt(pow(i-nRows/2,2)+pow(j-nCols/2,2));

        if (r > 0) {
          acquiredData -> setValue(i, j, sin(r/100.0)/r);
        } else {
          acquiredData -> setValue(i, j, 1);
        }
      }
    }

    QString fileName;
    QString fileBase;

    if (get_AcquireDark()) {
      fileBase = get_FilePattern()+tr("-%1.dark.tif").arg(get_FileIndex(),5,10,QChar('0'));
      fileName = QDir(get_OutputDirectory())
                 .filePath(get_FilePattern()+tr("-%1.dark.tif")
                           .arg(get_FileIndex(),5,10,QChar('0')));
    } else {
      fileBase = get_FilePattern()+tr("-%1.tif").arg(get_FileIndex(),5,10,QChar('0'));
      fileName = QDir(get_OutputDirectory())
                 .filePath(get_FilePattern()+tr("-%1.tif")
                           .arg(get_FileIndex(),5,10,QChar('0')));
    }

    set_FileBase(fileBase);
    set_FileName(fileName);

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

    newAcquiredImage(acquiredData);

    set_FileIndex(get_FileIndex()+1);
  }

  haltAcquire();
}


/******************************************************************
*
*  $Log: qxrdacquisitionsimulated.cpp,v $
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
