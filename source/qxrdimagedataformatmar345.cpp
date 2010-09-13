/******************************************************************
*
*  $Id: qxrdimagedataformatmar345.cpp,v 1.2 2010/09/13 20:00:40 jennings Exp $
*
*******************************************************************/

#include "qxrdimagedataformatmar345.h"

static QxrdImageDataFormatMar345 fmt;
static QcepImageDataFormatMar345<quint16> rawfmt("raw");
static QcepImageDataFormatMar345<quint32> raw2fmt("raw2");
static QcepImageDataFormatMar345<bool> maskfmt("mask");

QxrdImageDataFormatMar345::QxrdImageDataFormatMar345(QString name)
  : QcepImageDataFormatMar345<double>(name),
    SOURCE_IDENT("$Id: qxrdimagedataformatmar345.cpp,v 1.2 2010/09/13 20:00:40 jennings Exp $")
{
}

/******************************************************************
*
*  $Log: qxrdimagedataformatmar345.cpp,v $
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:39  jennings
*  Moving files into source subdirectory
*
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

