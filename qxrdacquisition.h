#ifndef QXRDACQUISITION_H
#define QXRDACQUISITION_H

#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include <QVector>
#include <QFuture>

#include "qxrdrasterdata.h"
#include "qxrdimagequeue.h"

class QxrdApplication;
class QxrdAcquisitionThread;
class QxrdWindow;

class QxrdAcquisition : public QObject
{
  Q_OBJECT;

 public:
  QxrdAcquisition(QxrdAcquisitionThread *thread);
  ~QxrdAcquisition();

 public slots:
  void initialize();
  void acquire(QString outDir, QString filePattern, int fileIndex, int integmode, int nsum, int nframes);
  void acquireDark(QString outDir, QString filePattern, int fileIndex, int integmode, int nsum);
  void cancel();
  void cancelDark();

 signals:
  void acquireComplete();
  void printMessage(QString msg);
  void acquiredFrame(QString fileName, int index, int isum, int nsum, int iframe, int nframe);
  void fileIndexChanged(int index);
  void statusMessage(QString msg);

 public:
  void onEndFrame();
  void onEndAcquisition();
  QVector<double> integrationTimes();
  int acquisitionStatus(double time);

 private:
  void acquisitionError(int n);
  void haltAcquire();

 private:
  QMutex                 m_Acquiring;
  QWaitCondition         m_AcquisitionWaiting;
  QxrdAcquisitionThread *m_AcquisitionThread;
  QxrdApplication       *m_Application;
  int                    m_NRows;
  int                    m_NCols;
  int                    m_IntegMode;
  int                    m_NSums;
  int                    m_NFrames;
  int                    m_NBufferFrames;
  int                    m_BufferFrame;
  int                    m_CurrentSum;
  int                    m_CurrentFrame;
  int                    m_AcquiringDark;
  QxrdImageData         *m_AcquiredData;
  QVector<unsigned short> m_Buffer;
  double                 m_IntTimes[8];
  int                    m_NIntTimes;
  QString                m_OutputDir;
  QString                m_FilePattern;
  int                    m_FileIndex;
};

#endif
