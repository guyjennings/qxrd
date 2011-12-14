#ifndef QXRDSERVERTHREAD_H
#define QXRDSERVERTHREAD_H

#include "qcepmacros.h"

#include "qxrdthread.h"
#include <QDateTime>

class QxrdServer;
class QxrdExperiment;

class QxrdServerThread : public QxrdThread
{
  Q_OBJECT

public:
  QxrdServerThread(QxrdExperiment *doc, QString name, int port);
  ~QxrdServerThread();

  QxrdServer *server() const;

  void shutdown();
  void executeScript(QString cmd);

signals:
  void execute(QString cmd);

protected:
  void run();

private:
  QxrdExperiment   *m_Experiment;
  QString           m_Name;
  int               m_Port;
  QAtomicPointer<QxrdServer> m_Server;
};

#endif // QXRDSERVERTHREAD_H
