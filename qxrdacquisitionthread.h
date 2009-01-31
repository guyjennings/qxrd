#ifndef QXRDACQUISITIONTHREAD_H
#define QXRDACQUISITIONTHREAD_H

#include <QThread>

class QxrdAcquisition;
class QxrdApplication;

class QxrdAcquisitionThread : public QThread
{
  Q_OBJECT;

 public:
  QxrdAcquisitionThread(QxrdApplication *app);
  ~QxrdAcquisitionThread();

  void shutdown();

 signals:
  void acquisitionRunning();
  void newDataAvailable();

 protected:
  void run();

 private:
  QxrdApplication   *m_Application;
  QxrdAcquisition   *m_Acquisition;
};

#endif
