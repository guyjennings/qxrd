#ifndef QXRDSERVERTHREAD_H
#define QXRDSERVERTHREAD_H

#include "qcepmacros.h"

#include "qxrdthread.h"
#include "qxrdserver.h"
#include <QDateTime>
#include "qxrdexperiment.h"

class QxrdServerThread : public QxrdThread
{
  Q_OBJECT

public:
  QxrdServerThread(QxrdExperiment *doc, QString name, int port);
  ~QxrdServerThread();

  QxrdServerPtr server() const;

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
  QxrdServerPtr     m_Server;
};

#endif // QXRDSERVERTHREAD_H

class QxrdServerThread;
typedef QSharedPointer<QxrdServerThread> QxrdServerThreadPtr;
