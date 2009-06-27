/******************************************************************
*
*  $Id: qxrdimagedata.h,v 1.10 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#ifndef QXRDIMAGEDATA_H
#define QXRDIMAGEDATA_H

#include "qcepmacros.h"

#include <QReadWriteLock>
#include "qcepimagedata.h"

class QxrdImageData : public QcepImageData<double>
{
  Q_OBJECT;

public:
  QxrdImageData(int width=0, int height=0);

  QReadWriteLock *rwLock();

  QString rawFileName();

  Q_PROPERTY(int readoutMode READ get_ReadoutMode WRITE set_ReadoutMode);
  QCEP_INTEGER_PROPERTY(ReadoutMode);

  Q_PROPERTY(double exposureTime READ get_ExposureTime WRITE set_ExposureTime);
  QCEP_DOUBLE_PROPERTY(ExposureTime);

  Q_PROPERTY(int summedExposures READ get_SummedExposures WRITE set_SummedExposures);
  QCEP_INTEGER_PROPERTY(SummedExposures);

  Q_PROPERTY(int imageNumber READ get_ImageNumber WRITE set_ImageNumber);
  QCEP_INTEGER_PROPERTY(ImageNumber);

private:
  mutable QReadWriteLock m_Lock;
  HEADER_IDENT("$Id: qxrdimagedata.h,v 1.10 2009/06/27 22:50:32 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdimagedata.h,v $
*  Revision 1.10  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

