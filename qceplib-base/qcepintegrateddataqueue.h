#ifndef QCEPINTEGRATEDDATAQUEUE_H
#define QCEPINTEGRATEDDATAQUEUE_H

#include <QObject>
#include <QReadWriteLock>
#include <QQueue>
#include <QSharedPointer>
#include "qcepintegrateddata-ptr.h"

class QcepIntegratedDataQueue : public QObject
{
  Q_OBJECT

public:
  QcepIntegratedDataQueue(QString name);
  ~QcepIntegratedDataQueue();

  QcepIntegratedDataPtr dequeue();
  void enqueue(QcepIntegratedDataPtr data);
  int size() const;
  QcepIntegratedDataPtr operator[](int n);

  void deallocate();

private:
  mutable QReadWriteLock m_Lock;
  QQueue< QcepIntegratedDataPtr >     m_Queue;
  QString        m_Name;
  int            m_Debug;
};

#endif // QCEPINTEGRATEDDATAQUEUE_H
