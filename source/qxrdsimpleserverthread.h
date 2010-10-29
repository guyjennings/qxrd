#ifndef QXRDSIMPLESERVERTHREAD_H
#define QXRDSIMPLESERVERTHREAD_H

#include "qcepmacros.h"
#include <QThread>
#include "qxrdforwardtypes.h"
//#include "qxrdsimpleserver.h"
//#include "qxrdscriptengine.h"

class QxrdSimpleServerThread : public QThread
{
  Q_OBJECT;
public:
  QxrdSimpleServerThread(QxrdAcquisitionThreadPtr acq, QString name, int port);
  ~QxrdSimpleServerThread();

  void shutdown();

  QxrdSimpleServerPtr server() const;

signals:
  void printMessage(QString msg);
  void statusMessage(QString msg);
  void criticalMessage(QString msg);

protected:
  void run();

private:
  QxrdAcquisitionThreadPtr m_AcquisitionThread;
  QString                  m_Name;
  int                      m_Port;
  QxrdSimpleServerPtr      m_Server;
};

#endif // QXRDSIMPLESERVERTHREAD_H
