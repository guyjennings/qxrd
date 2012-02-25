#ifndef QXRDSERVERTHREAD_H
#define QXRDSERVERTHREAD_H

#include "qcepmacros.h"

#include "qxrdthread.h"
#include "qxrdserver.h"
#include <QDateTime>

class QxrdServerThread : public QxrdThread
{
  Q_OBJECT

public:
  QxrdServerThread(QxrdExperimentWPtr doc, QString name, int port);
  ~QxrdServerThread();

  QxrdServerPtr server() const;

  void shutdown();
  void executeScript(QString cmd);

signals:
  void execute(QString cmd);

protected:
  void run();

private:
  QxrdExperimentWPtr m_Experiment;
  QString            m_Name;
  int                m_Port;
  QxrdServerPtr      m_Server;
};

typedef QSharedPointer<QxrdServerThread> QxrdServerThreadPtr;

#endif // QXRDSERVERTHREAD_H
