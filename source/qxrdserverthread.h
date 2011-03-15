#ifndef QXRDSERVERTHREAD_H
#define QXRDSERVERTHREAD_H

#include "qcepmacros.h"

#include "qxrdthread.h"
#include <QDateTime>

class QxrdServer;

class QxrdServerThread : public QxrdThread
{
  Q_OBJECT;

public:
  QxrdServerThread(QString name, int port);
  ~QxrdServerThread();

  QxrdServer *server() const;

  void shutdown();
  void executeScript(QString cmd);

signals:
  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void execute(QString cmd);

protected:
  void run();

private:
  QString     m_Name;
  int         m_Port;
  QxrdServer *m_Server;
};

#endif // QXRDSERVERTHREAD_H
