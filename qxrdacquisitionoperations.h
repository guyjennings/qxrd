/******************************************************************
*
*  $Id: qxrdacquisitionoperations.h,v 1.3 2009/07/09 01:15:09 jennings Exp $
*
*******************************************************************/

#ifndef QXRDACQUISITIONOPERATIONS_H
#define QXRDACQUISITIONOPERATIONS_H

#include "qcepmacros.h"

#include "qxrdacquisitionscripting.h"

#include "qxrdimagequeue.h"

class QxrdAcquisitionOperations : public QxrdAcquisitionScripting
{
  Q_OBJECT;
public:
  QxrdAcquisitionOperations(QxrdAcquisitionThread *thread);

  QxrdImageData *takeNextFreeImage();
  QxrdImageData *takeNextAcquiredImage();

  void returnImageToPool(QxrdImageData *img);
  void newAcquiredImage(QxrdImageData *img);

signals:
  void acquiredImageAvailable();

private:
  mutable QMutex         m_Mutex;
  QxrdImageQueue         m_FreeImages;
  QxrdImageQueue         m_AcquiredImages;
  HEADER_IDENT("$Id: qxrdacquisitionoperations.h,v 1.3 2009/07/09 01:15:09 jennings Exp $");
};

#endif // QXRDACQUISITIONOPERATIONS_H

/******************************************************************
*
*  $Log: qxrdacquisitionoperations.h,v $
*  Revision 1.3  2009/07/09 01:15:09  jennings
*  Added some locks
*
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

