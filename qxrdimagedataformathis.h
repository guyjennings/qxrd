/******************************************************************
*
*  $Id: qxrdimagedataformathis.h,v 1.3 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#ifndef QIMGIMAGEDATAFORMATHIS_H
#define QIMGIMAGEDATAFORMATHIS_H

#include "qcepmacros.h"

#include "qcepimagedata.h"
#include "qcepimagedataformat.h"

class QxrdImageDataFormatHis : public QcepImageDataFormat<double>
{
 public:
  QxrdImageDataFormatHis(QString name = "his");

  QxrdImageDataFormatHis* canLoadFile(QString path);
  QxrdImageDataFormatHis* loadFile(QString path, QcepImageData<double> *img);
  QxrdImageDataFormatHis* saveFile(QString path, QcepImageData<double> *img);
  Priority priority() const;

private:
  HEADER_IDENT("$Id: qxrdimagedataformathis.h,v 1.3 2009/06/27 22:50:32 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdimagedataformathis.h,v $
*  Revision 1.3  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

