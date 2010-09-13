/******************************************************************
*
*  $Id: qxrdintegrateddataqueue.h,v 1.2 2010/09/13 20:00:40 jennings Exp $
*
*******************************************************************/

#ifndef QXRDINTEGRATEDDATAQUEUE_H
#define QXRDINTEGRATEDDATAQUEUE_H

#include <QObject>
#include <QReadWriteLock>
#include <QQueue>

#include "qxrdintegrateddata.h"

class QxrdIntegratedDataQueue : public QObject
{
  Q_OBJECT;

public:
  QxrdIntegratedDataQueue(QString name);
  ~QxrdIntegratedDataQueue();

  QxrdIntegratedDataPtr dequeue();
  void enqueue(QxrdIntegratedDataPtr data);
  int size() const;
  QxrdIntegratedDataPtr operator[](int n);

  void deallocate();

private:
  mutable QReadWriteLock m_Lock;
  QQueue< QxrdIntegratedDataPtr >     m_Queue;
  QString        m_Name;
  int            m_Debug;

  HEADER_IDENT("$Id: qxrdintegrateddataqueue.h,v 1.2 2010/09/13 20:00:40 jennings Exp $");
};

#endif // QXRDINTEGRATEDDATAQUEUE_H

/******************************************************************
*
*  $Log: qxrdintegrateddataqueue.h,v $
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:39  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.2.1  2010/06/11 21:20:40  jennings
*  Added QxrdSharedPointer, QxrdIntegratedData and QxrdIntegratedDataQueue
*
*
*******************************************************************/
