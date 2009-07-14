/******************************************************************
*
*  $Id: qxrdacquisitionsimulated.h,v 1.2 2009/07/14 20:07:00 jennings Exp $
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

  void simulatedInitialize();
  void simulatedAcquisition(int isDark);

signals:
  void acquiredFrame(QString fileName, int index, int isum, int nsum, int iframe, int nframe);

private:
  HEADER_IDENT("$Id");
};

#endif // QXRDACQUISITIONSIMULATED_H

/******************************************************************
*
*  $Log: qxrdacquisitionsimulated.h,v $
*  Revision 1.2  2009/07/14 20:07:00  jennings
*  Implemented simple simulated acquisition
*
*  Revision 1.1  2009/07/13 23:19:37  jennings
*  More acquisition rearrangement
*
*
*******************************************************************/
