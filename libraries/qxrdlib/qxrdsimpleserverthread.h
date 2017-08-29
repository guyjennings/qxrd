#ifndef QXRDSIMPLESERVERTHREAD_H
#define QXRDSIMPLESERVERTHREAD_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qxrdthread.h"
#include "qxrdsimpleserver-ptr.h"
#include "qxrdexperiment-ptr.h"

class QXRD_EXPORT QxrdSimpleServerThread : public QxrdThread
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
