/******************************************************************
*
*  $Id: qxrdimagedataformatmar345.h,v 1.2 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#ifndef QXRDIMAGEDATAFORMATMAR345_H
#define QXRDIMAGEDATAFORMATMAR345_H

#include "qcepmacros.h"

#include "qcepimagedataformatmar345.h"

class QxrdImageDataFormatMar345 : public QcepImageDataFormatMar345<double>
{
public:
  QxrdImageDataFormatMar345(QString name="mar345");

private:
  HEADER_IDENT("$Id: qxrdimagedataformatmar345.h,v 1.2 2009/06/27 22:50:32 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdimagedataformatmar345.h,v $
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

