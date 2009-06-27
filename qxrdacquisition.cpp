/******************************************************************
*
*  $Id: qxrdacquisition.cpp,v 1.48 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#include "qxrdacquisition.h"

QxrdAcquisition::QxrdAcquisition(QxrdAcquisitionThread *thread)
  : inherited(thread),
    SOURCE_IDENT("$Id: qxrdacquisition.cpp,v 1.48 2009/06/27 22:50:32 jennings Exp $")
{
  emit printMessage("Enter QxrdAcquisition::QxrdAcquisition\n");
}



/******************************************************************
*
*  $Log: qxrdacquisition.cpp,v $
*  Revision 1.48  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

