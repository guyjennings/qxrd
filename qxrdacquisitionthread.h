#ifndef QXRDACQUISITIONTHREAD_H
#define QXRDACQUISITIONTHREAD_H

#include <QThread>
#include <QVector>
#include <QVariant>

#include "qxrdimagequeue.h"

class QxrdAcquisition;

class QxrdAcquisitionThread : public QThread
{
  Q_OBJECT;

 public:
  QxrdAcquisitionThread();
  ~QxrdAcquisitionThread();

  void shutdown();
  void msleep(int msec);
  QVector<double> integrationTimes();

 public slots:
  void doAcquire();
//  void acquire(QString outDir, QString filePattern, int fileIndex, int integmode, int nsum, int nframes);
  void cancel();

  void doAcquireDark();
//  void acquireDark(QString outDir, QString filePattern, int fileIndex, int integmode, int nsum);
  void cancelDark();

  QVariant evaluate(QString cmd);

 public:
  int acquisitionStatus(double time);

  QxrdImageData *takeNextFreeImage();
  QxrdImageData *takeNextAcquiredImage();

  void returnImageToPool(QxrdImageData *img);
  void newAcquiredImage(QxrdImageData *img);

  QxrdAcquisition* acquisition() const;

signals:
  void acquisitionRunning();
  void printMessage(QString msg);
  void statusMessage(QString msg);

  void acquireStarted(int dark);
  void acquireComplete(int dark);
  void acquiredFrame(QString fileName, int fileIndex, int isum, int nsum, int iframe, int nframe);
  void acquiredImageAvailable();

  void _evaluate(QString cmd);

 protected:
  void run();

 private:
  QReadWriteLock         m_Lock;
  int                    m_Debug;
  QxrdAcquisition       *m_Acquisition;
  QxrdImageQueue         m_FreeImages;
  QxrdImageQueue         m_AcquiredImages;
};

#endif
