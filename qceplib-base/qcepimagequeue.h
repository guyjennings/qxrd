#ifndef QCEPIMAGEQUEUE_H
#define QCEPIMAGEQUEUE_H

#include "qceplib_global.h"
#include "qcepmacros.h"

#include "qcepimagedata.h"
#include "qcepmaskdata.h"
#include <QQueue>
#include <QString>
#include <QSharedPointer>

class QCEP_EXPORT QcepImageQueueBase : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  QcepImageQueueBase(QString name);
  ~QcepImageQueueBase();

//  void preallocate(int nFrames, int nCols, int nRows);

//  Q_PROPERTY(int nRows READ get_NRows WRITE set_NRows)
//  QCEP_INTEGER_PROPERTY(NRows)

//  Q_PROPERTY(int nCols READ get_NCols WRITE set_NCols)
//  QCEP_INTEGER_PROPERTY(NCols)

  Q_PROPERTY(int size  READ get_Size  WRITE set_Size STORED false)
  QCEP_INTEGER_PROPERTY(Size)

  Q_PROPERTY(int count READ get_Count WRITE set_Count STORED false)
  QCEP_INTEGER_PROPERTY(Count)
};

template <typename T>
class QCEP_EXPORT QcepImageQueue : public QcepImageQueueBase
{

private:
  typedef QcepImageQueueBase inherited;

public:
  QcepImageQueue(QString name);
  ~QcepImageQueue();

  void deallocate();

  QSharedPointer<T> dequeue();
  void enqueue(QSharedPointer<T> data);
  QSharedPointer<T> operator[](int n);

private:
  mutable QMutex               m_Lock;
  QQueue< QSharedPointer<T> >  m_Queue;
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
