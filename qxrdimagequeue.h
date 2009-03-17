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

 private:
  QReadWriteLock m_Lock;
  QString        m_Name;
};

#endif
