/******************************************************************
*
*  $Id: qxrdringsetfitrefiner.cpp,v 1.2 2010/09/13 20:00:41 jennings Exp $
*
*******************************************************************/

#include "qxrdringsetfitrefiner.h"
#include "levmar.h"

QxrdRingSetFitRefiner::QxrdRingSetFitRefiner(QObject *parent) :
    QxrdFitRefiner(parent),
    SOURCE_IDENT("$Id: qxrdringsetfitrefiner.cpp,v 1.2 2010/09/13 20:00:41 jennings Exp $")
{
}

/******************************************************************
*
*  $Log: qxrdringsetfitrefiner.cpp,v $
*  Revision 1.2  2010/09/13 20:00:41  jennings
*  Merged
*
*  Revision 1.1.2.2  2010/08/13 21:57:36  jennings
*  A little more powder fit implementation
*
*  Revision 1.1.2.1  2010/08/03 20:14:38  jennings
*  Added classes for resampling ring centroids and fitting ring parameters
*
*
*******************************************************************/

