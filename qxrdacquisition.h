/******************************************************************
*
*  $Id: qxrdacquisition.h,v 1.34 2009/07/10 22:54:23 jennings Exp $
*
*******************************************************************/

#ifndef QXRDACQUISITION_H
#define QXRDACQUISITION_H

#include "qcepmacros.h"

#include <QObject>

#include "qxrdrasterdata.h"
#include "qxrdimagequeue.h"
#include "qxrdacquisitionperkinelmer.h"

class QxrdAcquisition : public QxrdAcquisitionPerkinElmer
{
  Q_OBJECT;

public:
  QxrdAcquisition(QxrdAcquisitionThread *thread, QxrdDataProcessor *proc);
  typedef QxrdAcquisitionPerkinElmer inherited;

private:
  HEADER_IDENT("$Id: qxrdacquisition.h,v 1.34 2009/07/10 22:54:23 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdacquisition.h,v $
*  Revision 1.34  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*  Revision 1.33  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

