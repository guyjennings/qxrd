#ifndef QXRDSERVERTHREAD_H
#define QXRDSERVERTHREAD_H

#include <QThread>

class QxrdServer;
class QxrdApplication;
class QxrdAcquisitionThread;

class QxrdServerThread : public QThread
{
  Q_OBJECT;

 public:
  QxrdServerThread(QxrdApplication *app, QxrdAcquisitionThread *acqth);
  ~QxrdServerThread();

  void shutdown();

 signals:
  void serverRunning();
  void print_message(QString msg);

 protected:
  void run();

 private:
  QxrdApplication        *m_Application;
  QxrdServer             *m_Server;
  QxrdAcquisitionThread  *m_AcquisitionThread;
};

#endif
