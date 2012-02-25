#ifndef QXRDINTEGRATEDDATAQUEUE_H
#define QXRDINTEGRATEDDATAQUEUE_H

#include <QObject>
#include <QReadWriteLock>
#include <QQueue>
#include <QSharedPointer>
#include "qxrdintegrateddata.h"

class QxrdIntegratedDataQueue : public QObject
{
  Q_OBJECT

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
};

#endif // QXRDINTEGRATEDDATAQUEUE_H
