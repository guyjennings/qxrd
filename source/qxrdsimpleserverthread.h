#ifndef QXRDSIMPLESERVERTHREAD_H
#define QXRDSIMPLESERVERTHREAD_H

#include "qcepmacros.h"
#include "qxrdthread.h"
#include <QDateTime>

class QxrdSimpleServer;

class QxrdSimpleServerThread : public QxrdThread
{
  Q_OBJECT;
public:
  QxrdSimpleServerThread(QString name, int port);
  ~QxrdSimpleServerThread();

  void shutdown();

  QxrdSimpleServer *server() const;

signals:
  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

protected:
  void run();

private:
  QString           m_Name;
  int               m_Port;
  QxrdSimpleServer *m_Server;
};

#endif // QXRDSIMPLESERVERTHREAD_H
