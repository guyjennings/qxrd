#ifndef QCEPIMAGEQUEUE_H
#define QCEPIMAGEQUEUE_H

#include "qcepmacros.h"

#include "qcepimagedata.h"
#include "qcepmaskdata.h"
#include <QQueue>
#include <QString>
#include <QSharedPointer>

template <typename T>
class QcepImageQueue : QObject
{
public:
  QcepImageQueue(QString name);
  ~QcepImageQueue();

  QSharedPointer<T> dequeue();
  void enqueue(QSharedPointer<T> data);
  int size() const;
  QSharedPointer<T> operator[](int n);

  void deallocate();
//  void preallocate(int nFrames, int nCols, int nRows);

  Q_PROPERTY(int     nRows      READ get_NRows WRITE set_NRows)
  QCEP_INTEGER_PROPERTY(NRows)

  Q_PROPERTY(int     nCols      READ get_NCols WRITE set_NCols)
  QCEP_INTEGER_PROPERTY(NCols)

private:
  mutable QMutex m_Lock;
  QQueue< QSharedPointer<T> >     m_Queue;
  QString        m_Name;
};

typedef QcepImageQueue<QcepImageDataBase>    QcepImageBaseQueue;
typedef QcepImageQueue<QcepUInt16ImageData>  QcepInt16ImageQueue;
typedef QcepImageQueue<QcepUInt32ImageData>  QcepInt32ImageQueue;
typedef QcepImageQueue<QcepDoubleImageData>  QcepDoubleImageQueue;
typedef QcepImageQueue<QcepMaskData>         QcepMaskQueue;

typedef QSharedPointer<QcepImageBaseQueue>   QcepImageBaseQueuePtr;
typedef QSharedPointer<QcepInt16ImageQueue>  QcepInt16ImageQueuePtr;
typedef QSharedPointer<QcepInt32ImageQueue>  QcepInt32ImageQueuePtr;
typedef QSharedPointer<QcepDoubleImageQueue> QcepDoubleImageQueuePtr;
typedef QSharedPointer<QcepMaskQueue>        QcepMaskQueuePtr;

#endif
