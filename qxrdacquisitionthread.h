#ifndef QXRDACQUISITIONTHREAD_H
#define QXRDACQUISITIONTHREAD_H

#include <QThread>
#include <QVector>

class QxrdAcquisition;
class QxrdApplication;

class QxrdAcquisitionThread : public QThread
{
  Q_OBJECT;

 public:
  QxrdAcquisitionThread(QxrdApplication *app);
  ~QxrdAcquisitionThread();

  void shutdown();
  void msleep(int msec);
  QVector<double> integrationTimes();

 signals:
  void acquisitionRunning();
  void newDataAvailable();
  void printMessage(QString msg);
  void acquireComplete();

 public slots:
  void acquire(double expos, int nsum, int nframes);
  void cancel();

 signals:
  void _acquire(double expos, int nsum, int nframes);

 protected:
  void run();

 private:
  QxrdApplication   *m_Application;
  QxrdAcquisition   *m_Acquisition;
};

#endif
