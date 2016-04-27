#ifndef QXRDSIMPLESERVERTHREAD_H
#define QXRDSIMPLESERVERTHREAD_H

#include "qcepmacros.h"
#include "qxrdthread.h"
#include "qxrdsimpleserver-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qcepsettingssaver-ptr.h"

class QxrdSimpleServerThread : public QxrdThread
{
  Q_OBJECT
public:
  QxrdSimpleServerThread(QxrdExperimentWPtr doc, QString name);
  ~QxrdSimpleServerThread();

  void shutdown();

  QxrdSimpleServerPtr server() const;

protected:
  void run();

private:
  QxrdExperimentWPtr    m_Experiment;
  QString               m_Name;
  QxrdSimpleServerPtr   m_Server;
};

#endif // QXRDSIMPLESERVERTHREAD_H
