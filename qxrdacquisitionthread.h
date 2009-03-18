#ifndef QXRDACQUISITIONTHREAD_H
#define QXRDACQUISITIONTHREAD_H

#include <QThread>
#include <QVector>

#include "qxrdrasterdata.h"

class QxrdAcquisition;
class QxrdApplication;
class QxrdWindow;

class QxrdAcquisitionThread : public QThread
{
  Q_OBJECT;

 public:
  QxrdAcquisitionThread(QxrdApplication *app, QxrdWindow *win);
  ~QxrdAcquisitionThread();

  void shutdown();
  void msleep(int msec);
  QVector<double> integrationTimes();

 signals:
  void acquisitionRunning();
  void newDataAvailable();
  void printMessage(QString msg);
  void acquireComplete();
  void acquiredFrame(QString fileName, int fileIndex, int isum, int nsum, int iframe, int nframe);
  void fileIndexChanged(int index);
  void statusMessage(QString msg);
  void summedFrameCompleted(QString fileName, int iframe);

 public slots:
  void acquire(QString outDir, QString filePattern, int fileIndex, int integmode, int nsum, int nframes);
  void cancel();
  void acquireDark(QString outDir, QString filePattern, int fileIndex, int integmode, int nsum);
  void cancelDark();

 public:
  int acquisitionStatus(double time);
  void setWindow(QxrdWindow *win);
  QxrdWindow *window();
  void enqueue(QxrdImageData *img);

 signals:
  void _acquire(QString outDir, QString filePattern, int fileIndex, int integmode, int nsum, int nframes);
  void _acquireDark(QString outDir, QString filePattern, int fileIndex, int integmode, int nsum);

 protected:
  void run();

 private:
  QxrdApplication   *m_Application;
  QxrdWindow        *m_Window;
  QxrdAcquisition   *m_Acquisition;
};

#endif
