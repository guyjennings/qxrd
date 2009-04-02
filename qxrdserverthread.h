#ifndef QXRDSERVERTHREAD_H
#define QXRDSERVERTHREAD_H

#include <QThread>

class QxrdServer;
class QxrdAcquisitionThread;

class QxrdServerThread : public QThread
{
  Q_OBJECT;

public:
  QxrdServerThread(QxrdAcquisitionThread *acq, QString name);
  ~QxrdServerThread();

  void shutdown();

signals:
  void printMessage(QString msg);

protected:
  void run();

private:
  QxrdAcquisitionThread *m_AcquisitionThread;
  QString                m_Name;
  QxrdServer            *m_Server;
};

#endif // QXRDSERVERTHREAD_H
