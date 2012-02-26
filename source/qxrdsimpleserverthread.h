#ifndef QXRDSIMPLESERVERTHREAD_H
#define QXRDSIMPLESERVERTHREAD_H

#include "qcepmacros.h"
#include "qxrdthread.h"
#include "qxrdsimpleserver-ptr.h"
#include "qxrdexperiment-ptr.h"

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

#endif // QXRDSIMPLESERVERTHREAD_H
