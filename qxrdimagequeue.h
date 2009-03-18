#ifndef QXRDIMAGEQUEUE_H
#define QXRDIMAGEQUEUE_H

class QxrdImageData;

#include <QQueue>
#include <QReadWriteLock>
#include <QString>

class QxrdImageQueue : private QQueue<QxrdImageData*>
{
 public:
  QxrdImageQueue(QString name="");
  typedef QQueue<QxrdImageData*> inherited;

  QxrdImageData* dequeue();
  void enqueue(QxrdImageData *data);
  int size() const;

 private:
  mutable QReadWriteLock m_Lock;
  QString        m_Name;
};

#endif
