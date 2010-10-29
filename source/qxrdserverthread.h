#ifndef QXRDSERVERTHREAD_H
#define QXRDSERVERTHREAD_H

#include "qcepmacros.h"

#include <QThread>
#include "qxrdforwardtypes.h"

class QxrdServerThread : public QThread
{
  Q_OBJECT;

public:
  QxrdServerThread(QxrdAcquisitionThreadPtr acq, QString name, int port);
  ~QxrdServerThread();

  QxrdServerPtr server() const;

  void shutdown();
  void executeScript(QString cmd);

signals:
  void printMessage(QString msg);
  void statusMessage(QString msg);
  void criticalMessage(QString msg);
  void execute(QString cmd);

protected:
  void run();

private:
  QxrdAcquisitionThreadPtr m_AcquisitionThread;
  QString                  m_Name;
  int                      m_Port;
  QxrdServerPtr            m_Server;
};

#endif // QXRDSERVERTHREAD_H
