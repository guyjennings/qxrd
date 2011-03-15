#ifndef QXRDTHREAD_H
#define QXRDTHREAD_H

#include <QThread>

class QxrdThread : public QThread
{
  Q_OBJECT
public:
  explicit QxrdThread(QObject *parent = 0);

  virtual void shutdown() = 0;
};

#endif // QXRDTHREAD_H
