/******************************************************************
*
*  $Id: qxrdimagedataformatmar345.cpp,v 1.2 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#include "qxrdimagedataformatmar345.h"

static QxrdImageDataFormatMar345 fmt;

QxrdImageDataFormatMar345::QxrdImageDataFormatMar345(QString name)
  : QcepImageDataFormatMar345<double>(name),
    SOURCE_IDENT("$Id: qxrdimagedataformatmar345.cpp,v 1.2 2009/06/27 22:50:32 jennings Exp $")
{
}

/******************************************************************
*
*  $Log: qxrdimagedataformatmar345.cpp,v $
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

