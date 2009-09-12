/******************************************************************
*
*  $Id: qxrdimagedata.h,v 1.12 2009/09/12 14:44:20 jennings Exp $
*
*******************************************************************/

#ifndef QXRDIMAGEDATA_H
#define QXRDIMAGEDATA_H

#include "qcepmacros.h"

#include <QDir>
#include <QFileInfo>
#include "qcepimagedata.h"

template <typename T>
class QxrdImageData : public QcepImageData<T>
{
//  Q_OBJECT;
//
public:
  QxrdImageData(int width=0, int height=0);

  QString rawFileName();

private:
  HEADER_IDENT("$Id: qxrdimagedata.h,v 1.12 2009/09/12 14:44:20 jennings Exp $");
};

typedef QxrdImageData<quint16> QxrdInt16ImageData;
typedef QxrdImageData<quint32> QxrdInt32ImageData;
typedef QxrdImageData<double>  QxrdDoubleImageData;

template <typename T>
QxrdImageData<T>::QxrdImageData(int width, int height)
  : QcepImageData<T>(width, height),
    SOURCE_IDENT("$Id: qxrdimagedata.h,v 1.12 2009/09/12 14:44:20 jennings Exp $")
{
}

template <typename T>
QString QxrdImageData<T>::rawFileName()
{
  QFileInfo info(QcepImageData<T>::get_FileName());

  QString name = info.dir().filePath(
      info.completeBaseName()+".raw.tif");

  return name;
}

#endif

/******************************************************************
*
*  $Log: qxrdimagedata.h,v $
*  Revision 1.12  2009/09/12 14:44:20  jennings
*  *** empty log message ***
*
*  Revision 1.11  2009/08/25 18:43:03  jennings
*  Templatized QxrdImageData and QxrdImageQueue, and added int16, int32 and double variants as typedefs
*
*  Revision 1.10  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

