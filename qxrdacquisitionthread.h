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
  void acquiredFrame(int isum, int nsum, int iframe, int nframe);

 public slots:
  void acquire(int integmode, int nsum, int nframes);
  void cancel();
  void saveData(QString name);

 signals:
  void _acquire(int integmode, int nsum, int nframes);

 protected:
  void run();

 private:
  QxrdApplication   *m_Application;
  QxrdAcquisition   *m_Acquisition;
};

#endif
