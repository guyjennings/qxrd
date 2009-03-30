#ifndef QXRDACQUISITIONTHREAD_H
#define QXRDACQUISITIONTHREAD_H

#include <QThread>
#include <QVector>

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

 signals:
  void acquisitionRunning();
  void printMessage(QString msg);
  void statusMessage(QString msg);

  void acquireComplete();
  void acquiredFrame(QString fileName, int fileIndex, int isum, int nsum, int iframe, int nframe);
  void fileIndexChanged(int index);

  void acquiredImageAvailable();

 public slots:
  void acquire(QString outDir, QString filePattern, int fileIndex, int integmode, int nsum, int nframes);
  void cancel();
  void acquireDark(QString outDir, QString filePattern, int fileIndex, int integmode, int nsum);
  void cancelDark();

 public:
  int acquisitionStatus(double time);

  QxrdImageData *takeNextFreeImage();
  QxrdImageData *takeNextAcquiredImage();

  void returnImageToPool(QxrdImageData *img);
  void newAcquiredImage(QxrdImageData *img);

 signals:
  void _acquire(QString outDir, QString filePattern, int fileIndex, int integmode, int nsum, int nframes);
  void _acquireDark(QString outDir, QString filePattern, int fileIndex, int integmode, int nsum);

 protected:
  void run();

 private:
  QxrdAcquisition       *m_Acquisition;
  QxrdImageQueue         m_FreeImages;
  QxrdImageQueue         m_AcquiredImages;
};

#endif
