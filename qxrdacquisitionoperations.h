/******************************************************************
*
*  $Id: qxrdacquisitionoperations.h,v 1.7 2009/07/17 14:00:59 jennings Exp $
*
*******************************************************************/

#ifndef QXRDACQUISITIONOPERATIONS_H
#define QXRDACQUISITIONOPERATIONS_H

#include "qcepmacros.h"

#include "qxrdacquisitionscripting.h"

#include "qxrdimagequeue.h"

class QxrdDataProcessor;

class QxrdAcquisitionOperations : public QxrdAcquisitionScripting
{
  Q_OBJECT;
public:
  QxrdAcquisitionOperations(QxrdDataProcessor *proc);

  QxrdImageData *takeNextAcquiredImage();

  void newAcquiredImage(QxrdImageData *img);


signals:
  void acquiredImageAvailable(QxrdImageData *image);

protected:
  mutable QMutex         m_Mutex;
  QxrdDataProcessor     *m_DataProcessor;
  QxrdImageQueue         m_AcquiredImages;

  HEADER_IDENT("$Id: qxrdacquisitionoperations.h,v 1.7 2009/07/17 14:00:59 jennings Exp $");
};

#endif // QXRDACQUISITIONOPERATIONS_H

/******************************************************************
*
*  $Log: qxrdacquisitionoperations.h,v $
*  Revision 1.7  2009/07/17 14:00:59  jennings
*  Rearranging acquisition and data processor
*
*  Revision 1.6  2009/07/17 12:41:33  jennings
*  Rearranging acquisition and data processor
*
*  Revision 1.5  2009/07/13 23:19:37  jennings
*  More acquisition rearrangement
*
*  Revision 1.4  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*  Revision 1.3  2009/07/09 01:15:09  jennings
*  Added some locks
*
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

