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
  bool onEndFrame();
  void onEndAcquisition();
  QVector<double> integrationTimes();
  int acquisitionStatus(double time);
  bool canStart();
  void onEndFrameCallback();

 private:
  void acquisition();
  void acquisitionError(int n);
  void haltAcquire();

 private:
  QxrdAcquisitionThread *m_AcquisitionThread;
  QxrdApplication       *m_Application;
  QMutex                 m_Acquiring;
  QWaitCondition         m_AcquisitionWaiting;
  QWaitCondition         m_StatusWaiting;
  int                    m_Cancelling;
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
  QVector<quint16>       m_Buffer;
  double                 m_IntTimes[8];
  int                    m_NIntTimes;
  QString                m_OutputDir;
  QString                m_FilePattern;
  int                    m_FileIndex;
};

#endif
