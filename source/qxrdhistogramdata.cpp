/******************************************************************
*
*  $Id: qxrdhistogramdata.cpp,v 1.2 2010/09/13 20:00:40 jennings Exp $
*
*******************************************************************/

#include "qxrdhistogramdata.h"

QxrdHistogramData::QxrdHistogramData(QObject *parent) :
    QObject(parent),
    SOURCE_IDENT("$Id: qxrdhistogramdata.cpp,v 1.2 2010/09/13 20:00:40 jennings Exp $")
{
}

/******************************************************************
*
*  $Log: qxrdhistogramdata.cpp,v $
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:39  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.2.1  2010/06/17 16:02:36  jennings
*  Skeleton code to support result serialization during crucial points in processing
*  Separate QxrdDoubleImageData class which can hold a reference to a mask
*
*
*******************************************************************/

