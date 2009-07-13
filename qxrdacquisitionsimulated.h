/******************************************************************
*
*  $Id: qxrdacquisitionsimulated.h,v 1.1 2009/07/13 23:19:37 jennings Exp $
*
*******************************************************************/

#ifndef QXRDACQUISITIONSIMULATED_H
#define QXRDACQUISITIONSIMULATED_H

#include "qcepmacros.h"

#include "qxrdacquisitionoperations.h"

class QxrdAcquisitionSimulated : public QxrdAcquisitionOperations
{
  Q_OBJECT;
public:
  QxrdAcquisitionSimulated(QxrdDataProcessor *proc);

private:
  HEADER_IDENT("$Id");
};

#endif // QXRDACQUISITIONSIMULATED_H

/******************************************************************
*
*  $Log: qxrdacquisitionsimulated.h,v $
*  Revision 1.1  2009/07/13 23:19:37  jennings
*  More acquisition rearrangement
*
*
*******************************************************************/
