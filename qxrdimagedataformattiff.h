/******************************************************************
*
*  $Id: qxrdimagedataformattiff.h,v 1.2 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#ifndef QXRDIMAGEDATAFORMATTIFF_H
#define QXRDIMAGEDATAFORMATTIFF_H

#include "qcepmacros.h"

#include "qcepimagedataformattiff.h"

class QxrdImageDataFormatTiff : public QcepImageDataFormatTiff<double>
{
public:
  QxrdImageDataFormatTiff(QString name="tiff");

private:
  HEADER_IDENT("$Id: qxrdimagedataformattiff.h,v 1.2 2009/06/27 22:50:32 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdimagedataformattiff.h,v $
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

