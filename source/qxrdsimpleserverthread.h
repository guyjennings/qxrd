#ifndef QXRDSIMPLESERVERTHREAD_H
#define QXRDSIMPLESERVERTHREAD_H

#include "qcepmacros.h"
#include "qxrdthread.h"
#include <QDateTime>
#include "qxrdsimpleserver.h"

class QxrdSimpleServerThread : public QxrdThread
{
  Q_OBJECT
public:
  QxrdSimpleServerThread(QxrdExperimentWPtr doc, QString name, int port);
  ~QxrdSimpleServerThread();

  void shutdown();

  QxrdSimpleServerPtr server() const;

protected:
  void run();

private:
  QxrdExperimentWPtr  m_Experiment;
  QString             m_Name;
  int                 m_Port;
  QxrdSimpleServerPtr m_Server;
};

typedef QSharedPointer<QxrdSimpleServerThread> QxrdSimpleServerThreadPtr;

#endif // QXRDSIMPLESERVERTHREAD_H
