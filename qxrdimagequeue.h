/******************************************************************
*
*  $Id: qxrdimagequeue.h,v 1.5 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#ifndef QXRDIMAGEQUEUE_H
#define QXRDIMAGEQUEUE_H

#include "qcepmacros.h"

class QxrdImageData;

#include <QQueue>
#include <QReadWriteLock>
#include <QString>

class QxrdImageQueue : private QQueue<QxrdImageData*>
{
public:
  QxrdImageQueue(QString name="");
  ~QxrdImageQueue();

  typedef QQueue<QxrdImageData*> inherited;

  QxrdImageData* dequeue();
  void enqueue(QxrdImageData *data);
  int size() const;

private:
  mutable QReadWriteLock m_Lock;
  QString        m_Name;
  int            m_Debug;
  HEADER_IDENT("$Id: qxrdimagequeue.h,v 1.5 2009/06/27 22:50:32 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdimagequeue.h,v $
*  Revision 1.5  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

