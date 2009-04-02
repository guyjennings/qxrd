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

 public slots:
  void doAcquire();
  void acquire(QString outDir, QString filePattern, int fileIndex, int integmode, int nsum, int nframes);
  void cancel();

  void doAcquireDark();
  void acquireDark(QString outDir, QString filePattern, int fileIndex, int integmode, int nsum);
  void cancelDark();

  void setExposureTime(double t);
  void setIntegrationMode(int mode);
  void setNSummed(int nsummed);
  void setNFrames(int nframes);
  void setFileIndex(int index);
  void setFilePattern(QString pattern);
  void setOutputDirectory(QString path);
  void setDarkNSummed(int nsummed);

 public:
  int acquisitionStatus(double time);

  QxrdImageData *takeNextFreeImage();
  QxrdImageData *takeNextAcquiredImage();

  void returnImageToPool(QxrdImageData *img);
  void newAcquiredImage(QxrdImageData *img);

  double  exposureTime();
  int     integrationMode();
  int     nSummed();
  int     nFrames();
  int     fileIndex();
  QString filePattern();
  QString outputDirectory();
  int     darkNSummed();

signals:
  void acquisitionRunning();
  void printMessage(QString msg);
  void statusMessage(QString msg);

  void acquireComplete();
  void acquiredFrame(QString fileName, int fileIndex, int isum, int nsum, int iframe, int nframe);
  void acquiredImageAvailable();

  void _acquire(QString outDir, QString filePattern, int fileIndex, int integmode, int nsum, int nframes);
  void _acquireDark(QString outDir, QString filePattern, int fileIndex, int integmode, int nsum);

  void exposureTimeChanged(double t);
  void integrationModeChanged(int mode);
  void nSummedChanged(int nsummed);
  void nFramesChanged(int nframes);
  void fileIndexChanged(int index);
  void filePatternChanged(QString pattern);
  void outputDirectoryChanged(QString path);
  void darkNSummedChanged(int nsummed);

 protected:
  void run();

 private:
  QReadWriteLock         m_Lock;
  QxrdAcquisition       *m_Acquisition;
  QxrdImageQueue         m_FreeImages;
  QxrdImageQueue         m_AcquiredImages;
  double                 m_ExposureTime;
  int                    m_IntegrationMode;
  int                    m_NSummed;
  int                    m_NFrames;
  int                    m_FileIndex;
  QString                m_FilePattern;
  QString                m_OutputDirectory;
  int                    m_DarkNSummed;
};

#endif
