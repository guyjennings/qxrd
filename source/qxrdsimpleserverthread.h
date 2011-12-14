#ifndef QXRDSIMPLESERVERTHREAD_H
#define QXRDSIMPLESERVERTHREAD_H

#include "qcepmacros.h"
#include "qxrdthread.h"
#include <QDateTime>

class QxrdSimpleServer;
class QxrdExperiment;

class QxrdSimpleServerThread : public QxrdThread
{
  Q_OBJECT
public:
  QxrdSimpleServerThread(QxrdExperiment *doc, QString name, int port);
  ~QxrdSimpleServerThread();

  void shutdown();

  QxrdSimpleServer *server() const;

protected:
  void run();

private:
  QxrdExperiment   *m_Experiment;
  QString           m_Name;
  int               m_Port;
  QAtomicPointer<QxrdSimpleServer> m_Server;
};

#endif // QXRDSIMPLESERVERTHREAD_H
