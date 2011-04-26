#ifndef QXRDIMAGEQUEUE_H
#define QXRDIMAGEQUEUE_H

#include "qcepmacros.h"

#include "qxrdimagedata.h"
#include "qxrdmaskdata.h"
#include <QQueue>
#include <QReadWriteLock>
#include <QString>
#include <QSharedPointer>

template <typename T>
class QxrdImageQueue : QObject
{
public:
  QxrdImageQueue(QString name);
  ~QxrdImageQueue();

  QSharedPointer<T> dequeue();
  void enqueue(QSharedPointer<T> data);
  int size() const;
  QSharedPointer<T> operator[](int n);

  void deallocate();
//  void preallocate(int nFrames, int nCols, int nRows);

  Q_PROPERTY(int     nRows      READ get_NRows WRITE set_NRows);
  QCEP_INTEGER_PROPERTY(NRows);

  Q_PROPERTY(int     nCols      READ get_NCols WRITE set_NCols);
  QCEP_INTEGER_PROPERTY(NCols);

private:
  mutable QReadWriteLock m_Lock;
  QQueue< QSharedPointer<T> >     m_Queue;
  QString        m_Name;
};

typedef QxrdImageQueue<QxrdInt16ImageData>   QxrdInt16ImageQueue;
typedef QxrdImageQueue<QxrdInt32ImageData>   QxrdInt32ImageQueue;
typedef QxrdImageQueue<QxrdDoubleImageData>  QxrdDoubleImageQueue;
typedef QxrdImageQueue<QxrdMaskData>         QxrdMaskQueue;

typedef QSharedPointer<QxrdInt16ImageQueue>  QxrdInt16ImageQueuePtr;
typedef QSharedPointer<QxrdInt32ImageQueue>  QxrdInt32ImageQueuePtr;
typedef QSharedPointer<QxrdDoubleImageQueue> QxrdDoubleImageQueuePtr;
typedef QSharedPointer<QxrdMaskQueue>        QxrdMaskQueuePtr;

#endif
