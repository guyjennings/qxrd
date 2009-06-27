/******************************************************************
*
*  $Id: qxrdimagedataformattiff.cpp,v 1.2 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#include "qxrdimagedataformattiff.h"

static QxrdImageDataFormatTiff fmt;

QxrdImageDataFormatTiff::QxrdImageDataFormatTiff(QString name)
  : QcepImageDataFormatTiff<double>(name),
    SOURCE_IDENT("$Id: qxrdimagedataformattiff.cpp,v 1.2 2009/06/27 22:50:32 jennings Exp $")
{
}

/******************************************************************
*
*  $Log: qxrdimagedataformattiff.cpp,v $
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

