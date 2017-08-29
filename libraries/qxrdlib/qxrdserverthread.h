#ifndef QXRDSERVERTHREAD_H
#define QXRDSERVERTHREAD_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"

#include "qxrdthread.h"

#include "qxrdexperiment-ptr.h"
#include "qxrdserver-ptr.h"

class QXRD_EXPORT QxrdServerThread : public QxrdThread
{
  Q_OBJECT

public:
  QxrdServerThread(QxrdExperimentWPtr doc, QString name);
  ~QxrdServerThread();

  QxrdServerPtr server() const;

  void shutdown();
  void executeScript(QString cmd);

signals:
  void execute(QString cmd);

protected:
  void run();

private:
  QxrdExperimentWPtr    m_Experiment;
  QString               m_Name;
  QxrdServerPtr         m_Server;
};

#endif // QXRDSERVERTHREAD_H
