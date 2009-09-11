/******************************************************************
*
*  $Id: qxrdimagedataformattiff.cpp,v 1.3 2009/09/11 18:57:04 jennings Exp $
*
*******************************************************************/

#include "qxrdimagedataformattiff.h"

static QxrdImageDataFormatTiff fmt;
static QcepImageDataFormatTiff<quint16> rawfmt("raw");
static QcepImageDataFormatTiff<quint32> raw2fmt("raw2");
static QcepImageDataFormatTiff<bool> maskfmt("mask");

QxrdImageDataFormatTiff::QxrdImageDataFormatTiff(QString name)
  : QcepImageDataFormatTiff<double>(name),
    SOURCE_IDENT("$Id: qxrdimagedataformattiff.cpp,v 1.3 2009/09/11 18:57:04 jennings Exp $")
{
}

/******************************************************************
*
*  $Log: qxrdimagedataformattiff.cpp,v $
*  Revision 1.3  2009/09/11 18:57:04  jennings
*  Added format conversions for TIFF->raw16, TIFF->raw32, TIFF->bool, Mar345->raw16, Mar345-raw32, Mar345->bool
*  No conversions for his files though
*
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

