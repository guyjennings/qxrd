#ifndef QXRDSIMPLESERVERTHREAD_H
#define QXRDSIMPLESERVERTHREAD_H

#include "qcepmacros.h"
#include <QThread>
#include <QDateTime>

class QxrdSimpleServer;

class QxrdSimpleServerThread : public QThread
{
  Q_OBJECT;
public:
  QxrdSimpleServerThread(QString name, int port);
  ~QxrdSimpleServerThread();

  void shutdown();

  QxrdSimpleServer *server() const;

signals:
  void printMessage(QDateTime ts, QString msg);
  void statusMessage(QDateTime ts, QString msg);
  void criticalMessage(QDateTime ts, QString msg);

protected:
  void run();

private:
  QString           m_Name;
  int               m_Port;
  QxrdSimpleServer *m_Server;
};

#endif // QXRDSIMPLESERVERTHREAD_H
