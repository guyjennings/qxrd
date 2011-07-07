#ifndef QXRDSIMPLESERVERTHREAD_H
#define QXRDSIMPLESERVERTHREAD_H

#include "qcepmacros.h"
#include "qxrdthread.h"
#include <QDateTime>

class QxrdSimpleServer;

class QxrdSimpleServerThread : public QxrdThread
{
  Q_OBJECT
public:
  QxrdSimpleServerThread(QString name, int port);
  ~QxrdSimpleServerThread();

  void shutdown();

  QxrdSimpleServer *server() const;

protected:
  void run();

private:
  QString           m_Name;
  int               m_Port;
  QxrdSimpleServer *m_Server;
};

#endif // QXRDSIMPLESERVERTHREAD_H
