/******************************************************************
*
*  $Id: qxrdimagedata.cpp,v 1.8 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#include "qxrdimagedata.h"

#include <QDir>
#include <QFileInfo>

QxrdImageData::QxrdImageData(int width, int height)
  : QcepImageData<double>(width, height),
    m_ReadoutMode(this, "readoutMode", 0),
    m_ExposureTime(this, "exposureTime", 0),
    m_SummedExposures(this, "summedExposures", 0),
    m_ImageNumber(this, "imageNumber", 0),
    m_Lock(QReadWriteLock::Recursive),
    SOURCE_IDENT("$Id: qxrdimagedata.cpp,v 1.8 2009/06/27 22:50:32 jennings Exp $")
{
}

QReadWriteLock *QxrdImageData::rwLock()
{
  return &m_Lock;
}

QString QxrdImageData::rawFileName()
{
  QFileInfo info(get_FileName());

  QString name = info.dir().filePath(
      info.completeBaseName()+".raw.tif");

  return name;
}

/******************************************************************
*
*  $Log: qxrdimagedata.cpp,v $
*  Revision 1.8  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

