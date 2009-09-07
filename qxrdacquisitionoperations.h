/******************************************************************
*
*  $Id: qxrdacquisitionoperations.h,v 1.10 2009/09/07 21:38:32 jennings Exp $
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

  QxrdDoubleImageData *takeNextAcquiredImage();

  void newAcquiredImage(QxrdDoubleImageData *img);


signals:
//  void acquiredImageAvailable(QxrdDoubleImageData *image);
  void acquiredInt16ImageAvailable(QxrdInt16ImageData *img);
  void acquiredInt32ImageAvailable(QxrdInt32ImageData *img);

protected:
  mutable QMutex         m_Mutex;
  QxrdDataProcessor     *m_DataProcessor;
//  QxrdImageQueue         m_AcquiredImages;

  HEADER_IDENT("$Id: qxrdacquisitionoperations.h,v 1.10 2009/09/07 21:38:32 jennings Exp $");
};

#endif // QXRDACQUISITIONOPERATIONS_H

/******************************************************************
*
*  $Log: qxrdacquisitionoperations.h,v $
*  Revision 1.10  2009/09/07 21:38:32  jennings
*  Moved acquiredInt{16,32]ImageAvailable signals into base class
*
*  Revision 1.9  2009/08/25 18:43:03  jennings
*  Templatized QxrdImageData and QxrdImageQueue, and added int16, int32 and double variants as typedefs
*
*  Revision 1.8  2009/07/20 00:32:17  jennings
*  Removed image queues for acquired and dark images - use 'connect' args instead
*
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

