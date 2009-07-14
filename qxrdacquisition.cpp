/******************************************************************
*
*  $Id: qxrdacquisition.cpp,v 1.51 2009/07/14 05:36:14 jennings Exp $
*
*******************************************************************/

#include "qxrdacquisition.h"

QxrdAcquisition::QxrdAcquisition(QxrdDataProcessor *proc)
  : QxrdAcquisitionPerkinElmer(proc),
    SOURCE_IDENT("$Id: qxrdacquisition.cpp,v 1.51 2009/07/14 05:36:14 jennings Exp $")
{
  emit printMessage("Enter QxrdAcquisition::QxrdAcquisition\n");
}



/******************************************************************
*
*  $Log: qxrdacquisition.cpp,v $
*  Revision 1.51  2009/07/14 05:36:14  jennings
*  Moved data storage into individual channel objects, accelerated fitting (somewhat)
*
*  Revision 1.50  2009/07/13 23:19:37  jennings
*  More acquisition rearrangement
*
*  Revision 1.49  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*  Revision 1.48  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

