/******************************************************************
*
*  $Id: qxrdimagedataformatmar345.h,v 1.2 2010/09/13 20:00:40 jennings Exp $
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
  HEADER_IDENT("$Id: qxrdimagedataformatmar345.h,v 1.2 2010/09/13 20:00:40 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdimagedataformatmar345.h,v $
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:39  jennings
*  Moving files into source subdirectory
*
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

