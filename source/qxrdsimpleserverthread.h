#ifndef QXRDSIMPLESERVERTHREAD_H
#define QXRDSIMPLESERVERTHREAD_H

#include "qcepmacros.h"
#include "qxrdthread.h"
#include <QDateTime>
#include "qxrdsimpleserver.h"
#include "qxrdexperiment.h"

class QxrdSimpleServerThread : public QxrdThread
{
  Q_OBJECT
public:
  QxrdSimpleServerThread(QxrdExperiment *doc, QString name, int port);
  ~QxrdSimpleServerThread();

  void shutdown();

  QxrdSimpleServerPtr server() const;

protected:
  void run();

private:
  QxrdExperiment     *m_Experiment;
  QString             m_Name;
  int                 m_Port;
  QxrdSimpleServerPtr m_Server;
};

#endif // QXRDSIMPLESERVERTHREAD_H

class QxrdSimpleServerThread;
typedef QSharedPointer<QxrdSimpleServerThread> QxrdSimpleServerThreadPtr;
